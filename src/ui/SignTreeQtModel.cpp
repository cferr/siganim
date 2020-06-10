// Siganim - Sign Editor and Animator
// Copyright (C) 2020 Corentin Ferry
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "../sign/Sign.h"
#include "../sign/SignTreeVisitor.h"
#include "../sign/cells/Display.h"
#include "../sign/cells/Text.h"
#include "../sign/cells/Split.h"
#include "../sign/cells/MarqueeAnimation.h"
#include "../sign/cells/BlinkAnimation.h"
#include "../sign/cells/Compose.h"
#include "../sign/cells/Fill.h"
#include "SignTreeQtModel.h"

SignTreeQtModel::SignTreeQtModel(SignTree *tree) : tree(tree),
    treeAsChain(nullptr) {
    if(tree != nullptr) {
        ModelBuilder builder(this);
        builder.run(tree);
    }
}

SignTreeQtModel::Chain::Chain(IRPointer* ptr, Chain* parent, Chain* previous,
        Chain* next) : parent(parent), firstChild(nullptr), previous(previous),
                next(next), signModelPtr(ptr) {
    try {
        this->getPrevious()->setNext(this);
    } catch(EndOfChainException& e) {}
    try {
        this->getNext()->setPrevious(this);
    } catch(EndOfChainException& e) {}
    try {
        Chain* firstChild = this->getParent()->getFirstChildOrNull();
        if(firstChild == nullptr || firstChild == this->next)
            this->getParent()->setFirstChild(this);
    } catch(EndOfChainException& e) {}
};

SignTreeQtModel::Chain::~Chain() {
    try {
        this->getNext()->setPrevious(this->previous);
    } catch(EndOfChainException& e) { }

    try {
        this->getPrevious()->setNext(this->next);
    } catch(EndOfChainException& e) { }

    try {
        if(this->getParent()->getFirstChild() == this)
            this->getParent()->setFirstChild(this->next);
    } catch(EndOfChainException& e) { }

}

SignTreeQtModel::Chain*
    SignTreeQtModel::Chain::getNext() const {
    if(this->next != nullptr)
        return next;
    else throw EndOfChainException();
}

SignTreeQtModel::Chain*
    SignTreeQtModel::Chain::getNextOrNull() const {
    return next;
}

void SignTreeQtModel::Chain::setNext(Chain *next) {
    this->next = next;
}

SignTreeQtModel::Chain*
    SignTreeQtModel::Chain::getParent() const {
    if(this->parent != nullptr)
        return parent;
    else throw EndOfChainException();
}

SignTreeQtModel::Chain*
    SignTreeQtModel::Chain::getParentOrNull() const {
    return parent;
}


void SignTreeQtModel::Chain::setParent(
        Chain *parent) {
    this->parent = parent;
}

SignTreeQtModel::Chain*
    SignTreeQtModel::Chain::getPrevious() const {
    if(this->previous != nullptr)
        return previous;
    else throw EndOfChainException();
}

SignTreeQtModel::Chain*
    SignTreeQtModel::Chain::getPreviousOrNull() const {
    return previous;
}

void SignTreeQtModel::Chain::setPrevious(
        Chain *previous) {
    this->previous = previous;
}

void SignTreeQtModel::Chain::setFirstChild(
        Chain *firstChild) {
    this->firstChild = firstChild;
}

SignTreeQtModel::Chain* SignTreeQtModel::Chain::getFirstChild() const {
    if(this->firstChild != nullptr)
        return firstChild;
    else throw EndOfChainException();
}

SignTreeQtModel::Chain* SignTreeQtModel::Chain::getFirstChildOrNull()
    const {
    return this->firstChild;
}

SignTreeQtModel::IRPointer*
    SignTreeQtModel::Chain::getSignModelPtr() const {
    return signModelPtr;
}

unsigned int SignTreeQtModel::Chain::row() const {
    try {
        return 1 + this->getPrevious()->row();
    } catch(EndOfChainException& e) {
        return 0;
    }
}

unsigned int SignTreeQtModel::Chain::nbSiblingsNext() const {
    try {
        return 1 + this->getNext()->nbSiblingsNext();
    } catch(EndOfChainException& e) {
        return 0;
    }
}

unsigned int SignTreeQtModel::Chain::nbSiblingsPrevious() const {
    try {
        return 1 + this->getPrevious()->nbSiblingsPrevious();
    } catch(EndOfChainException& e) {
        return 0;
    }
}

unsigned int SignTreeQtModel::Chain::nbChildren() const {
    try {
        return 1 + this->getFirstChild()->nbSiblingsNext();
    } catch(EndOfChainException& e) {
        return 0;
    }
}

SignTreeQtModel::Chain* SignTreeQtModel::Chain::getNthSibling(
        unsigned int sibling) {
    if(sibling == 0)
        return this;
    else {
        return this->getNext()->getNthSibling(sibling - 1);
    }
}

QModelIndex SignTreeQtModel::indexFrom(SignTreeQtModel::Chain* chain) const {
    return this->createIndex(chain->row(), 0, chain);
}

void SignTreeQtModel::observeStructure(const SignTree *sender) {
    Chain* chainElt = nullptr;
    for(auto i = this->allChainNodes.begin(); i < this->allChainNodes.end();
            ++i) {
        try {
            if((*i)->getSignModelPtr()->getTree() == sender) {
                chainElt = *i;
                break;
            }
        } catch(IRPointer::NoSuchItemException& e) { }
    }
    if(chainElt != nullptr) {
        SignTree* nonConstTree = chainElt->getSignModelPtr()->getTree();
        // Unregister
        nonConstTree->deepDetachStructureObserver(this);

        Chain* parentChain = nullptr;
        try {
            parentChain = chainElt->getParent();
        } catch(Chain::EndOfChainException& e) { }

        Chain* previousChain = nullptr;
        try {
            previousChain = chainElt->getPrevious();
        } catch(Chain::EndOfChainException& e) { }

        Chain* nextChain = nullptr;
        try {
            nextChain = chainElt->getNext();
        } catch(Chain::EndOfChainException& e) { }

        QModelIndex observedNodeIndex = this->indexFrom(chainElt);
        // Delete
        this->removeNode(observedNodeIndex, false);

        // Rebuild
        ModelBuilder builder(this, parentChain, previousChain, nextChain);
        builder.run(nonConstTree);
    }

}

void SignTreeQtModel::addNode(Chain* node) {
    QModelIndex index = this->indexFrom(node);
    QModelIndex parent = index.parent();
    this->beginInsertRows(parent, index.row(), index.row());
    if(this->treeAsChain == nullptr)
        this->treeAsChain = node;
    this->allChainNodes.push_back(node);
    this->endInsertRows();
}

void SignTreeQtModel::removeNode(QModelIndex &index, bool removeNext) {
    // Delete children first
    Chain* indexAsChain = (Chain*)index.internalPointer();
    try {
        QModelIndex childIndex = this->indexFrom(indexAsChain->getFirstChild());
        this->removeNode(childIndex, true);
    } catch(Chain::EndOfChainException& e) {}

    if(removeNext) {
        try {
            QModelIndex nextIndex = this->indexFrom(indexAsChain->getNext());
            this->removeNode(nextIndex, true);
        } catch(Chain::EndOfChainException& e) {}
    }

    QModelIndex parent = index.parent();
    unsigned int row = index.row();
    this->beginRemoveRows(parent, row, row);

    auto i = this->allChainNodes.begin();
    while(i != this->allChainNodes.end())
        if((*i) == indexAsChain)
            i = this->allChainNodes.erase(i);
        else
            ++i;
    delete indexAsChain;

    this->endRemoveRows();
}


QModelIndex SignTreeQtModel::parent(const QModelIndex &child) const {
    Chain* childAsChain = (Chain*)child.internalPointer();
    try {
        return this->indexFrom(childAsChain->getParent());
    } catch(Chain::EndOfChainException& e) {
        return QModelIndex();
    }

}

QVariant SignTreeQtModel::data(const QModelIndex &index, int role) const {
    switch(role) {
    case Qt::DisplayRole:
        return ((Chain*)index.internalPointer())->getSignModelPtr()->getLabel();
        break;
    default:
        return QVariant();
        break;
    }
}

QModelIndex SignTreeQtModel::index(int row, int column,
        const QModelIndex &parent) const {
   if(column != 0) {
       return QModelIndex();
   } else {
       if(parent.isValid()) {
           try {
               Chain* parentAsChain = (Chain*)parent.internalPointer();
               return this->indexFrom(parentAsChain->getFirstChild()
                       ->getNthSibling(row));
           } catch(Chain::EndOfChainException& e) {
               return QModelIndex();
           }
       } else {
           if(this->treeAsChain != nullptr)
               return this->indexFrom(this->treeAsChain->getNthSibling(row));
           else
               return QModelIndex();
       }
   }
}

int SignTreeQtModel::rowCount(const QModelIndex &parent) const {
    if(parent.isValid()) {
        Chain* parentAsChain = (Chain*)parent.internalPointer();
        return parentAsChain->nbChildren();
    } else {
        if(this->treeAsChain != nullptr)
            return 1 + this->treeAsChain->nbSiblingsNext();
        else
            return 0;
    }
}

int SignTreeQtModel::columnCount(const QModelIndex &parent) const {
    return 1;
}

SignTreeQtModel::ModelBuilder::ModelBuilder(SignTreeQtModel *model,
        Chain* parent, Chain* previous, Chain* next)
    : parent(parent), previous(previous), next(next), model(model) {
}

SignTreeQtModel::ModelBuilder::ModelBuilder(SignTreeQtModel *model)
    : ModelBuilder(model, nullptr, nullptr, nullptr) {
}

void SignTreeQtModel::ModelBuilder::run(SignTree* root) {
    root->accept(*this);
}


void SignTreeQtModel::ModelBuilder::visit(Sign &s) {
    s.attachStructureObserver(this->model);
    Chain* parent = this->parent;
    Chain* previous = this->previous;
    Chain* next = this->next;
    IRPointer* ptrHolder = new IRPointer(&s, "Sign");
    Chain* current = new Chain(ptrHolder, parent, previous, next);
    this->model->addNode(current);

    this->parent = current;
    this->previous = nullptr;
    this->next = nullptr;

    std::vector<Display*> displays = s.getDisplays();
    for(auto i = displays.begin(); i < displays.end(); ++i) {
        (*i)->accept(*this);
    }

    Sign::DisplayBuilder* displayBuilder = s.displayBuilder();
    IRPointer* dbPtrHolder = new IRPointer(displayBuilder, "Add display...");
    Chain* dbChain = new Chain(dbPtrHolder, this->parent, this->previous);

    this->model->addNode(dbChain);

    this->parent = parent;
    this->previous = current;
    this->next = next;
}

void SignTreeQtModel::ModelBuilder::visit(Display &s) {
    s.attachStructureObserver(this->model);
    Chain* parent = this->parent;
    Chain* previous = this->previous;
    Chain* next = this->next;
    IRPointer* ptrHolder = new IRPointer(&s, "Display");
    Chain* current = new Chain(ptrHolder, parent, previous, next);
    this->model->addNode(current);

    this->parent = current;
    this->previous = nullptr;
    this->next = nullptr;

    try {
        s.getRootCell()->accept(*this);
    } catch(SignCell::NoSuchChildException& e) {
        SignCell::Builder* cb = s.rootCellBuilder();
        IRPointer* cbPtrHolder = new IRPointer(cb, "Empty");
        Chain* cbChain = new Chain(cbPtrHolder, this->parent, this->previous);
        this->model->addNode(cbChain);
        this->previous = cbChain;
    }
    this->parent = parent;
    this->previous = current;
    this->next = next;
}

void SignTreeQtModel::ModelBuilder::visit(Text &s) {
    s.attachStructureObserver(this->model);
    Chain* parent = this->parent;
    Chain* previous = this->previous;
    Chain* next = this->next;
    IRPointer* ptrHolder = new IRPointer(&s, "Text");
    Chain* current = new Chain(ptrHolder, parent, previous, next);
    this->model->addNode(current);

    this->parent = current;
    this->previous = nullptr;
    this->next = nullptr;
    // Process children...
    this->parent = parent;
    this->previous = current;
    this->next = next;
}

void SignTreeQtModel::ModelBuilder::visit(Split &s) {
    s.attachStructureObserver(this->model);
    Chain* parent = this->parent;
    Chain* previous = this->previous;
    Chain* next = this->next;
    IRPointer* ptrHolder = new IRPointer(&s, "Split");
    Chain* current = new Chain(ptrHolder, parent, previous, next);
    this->model->addNode(current);

    this->parent = current;
    this->previous = nullptr;
    this->next = nullptr;
    try {
        s.getTopOrLeftChild()->accept(*this);
    } catch(SignCell::NoSuchChildException& e) {
        SignCell::Builder* cb = s.topOrLeftChildBuilder();
        IRPointer* cbPtrHolder = new IRPointer(cb, "Empty");
        Chain* cbChain = new Chain(cbPtrHolder, this->parent, this->previous);
        this->model->addNode(cbChain);
        this->previous = cbChain;
    }
    try {
        s.getBottomOrRightChild()->accept(*this);
    } catch(SignCell::NoSuchChildException& e) {
        SignCell::Builder* cb = s.bottomOrRightChildBuilder();
        IRPointer* cbPtrHolder = new IRPointer(cb, "Empty");
        Chain* cbChain = new Chain(cbPtrHolder, this->parent, this->previous);
        this->model->addNode(cbChain);
        this->previous = cbChain;
    }
    this->parent = parent;
    this->previous = current;
    this->next = next;
}

void SignTreeQtModel::ModelBuilder::visit(MarqueeAnimation &s) {
    s.attachStructureObserver(this->model);
    Chain* parent = this->parent;
    Chain* previous = this->previous;
    Chain* next = this->next;
    IRPointer* ptrHolder = new IRPointer(&s, "Marquee");
    Chain* current = new Chain(ptrHolder, parent, previous, next);
    this->model->addNode(current);

    this->parent = current;
    this->previous = nullptr;
    this->next = nullptr;
    try {
        s.getSubject()->accept(*this);
    } catch(SignCell::NoSuchChildException& e) {
        SignCell::Builder* cb = s.subjectBuilder();
        IRPointer* cbPtrHolder = new IRPointer(cb, "Empty");
        Chain* cbChain = new Chain(cbPtrHolder, this->parent, this->previous);
        this->model->addNode(cbChain);
        this->previous = cbChain;
    }
    this->parent = parent;
    this->previous = current;
    this->next = next;
}

void SignTreeQtModel::ModelBuilder::visit(BlinkAnimation &s) {
    s.attachStructureObserver(this->model);
    Chain* parent = this->parent;
    Chain* previous = this->previous;
    Chain* next = this->next;
    IRPointer* ptrHolder = new IRPointer(&s, "Blink");
    Chain* current = new Chain(ptrHolder, parent, previous, next);
    this->model->addNode(current);

    this->parent = current;
    this->previous = nullptr;
    this->next = nullptr;
    try {
        s.getSubject()->accept(*this);
    } catch(SignCell::NoSuchChildException& e) {
        SignCell::Builder* cb = s.subjectBuilder();
        IRPointer* cbPtrHolder = new IRPointer(cb, "Empty");
        Chain* cbChain = new Chain(cbPtrHolder, this->parent, this->previous);
        this->model->addNode(cbChain);
        this->previous = cbChain;
    }
    this->parent = parent;
    this->previous = current;
    this->next = next;
}

void SignTreeQtModel::ModelBuilder::visit(Compose &s) {
    s.attachStructureObserver(this->model);
    Chain* parent = this->parent;
    Chain* previous = this->previous;
    Chain* next = this->next;
    IRPointer* ptrHolder = new IRPointer(&s, "Compose");
    Chain* current = new Chain(ptrHolder, parent, previous, next);
    this->model->addNode(current);

    this->parent = current;
    this->previous = nullptr;
    this->next = nullptr;
    try {
        s.getBackground()->accept(*this);
    } catch(SignCell::NoSuchChildException& e) {
        SignCell::Builder* cb = s.backgroundBuilder();
        IRPointer* cbPtrHolder = new IRPointer(cb, "Empty");
        Chain* cbChain = new Chain(cbPtrHolder, this->parent, this->previous);
        this->model->addNode(cbChain);
        this->previous = cbChain;
    }
    try {
        s.getForeground()->accept(*this);
    } catch(SignCell::NoSuchChildException& e) {
        SignCell::Builder* cb = s.foregroundBuilder();
        IRPointer* cbPtrHolder = new IRPointer(cb, "Empty");
        Chain* cbChain = new Chain(cbPtrHolder, this->parent, this->previous);
        this->model->addNode(cbChain);
        this->previous = cbChain;
    }
    this->parent = parent;
    this->previous = current;
    this->next = next;
}

void SignTreeQtModel::ModelBuilder::visit(Fill &s) {
    s.attachStructureObserver(this->model);
    Chain* parent = this->parent;
    Chain* previous = this->previous;
    Chain* next = this->next;
    IRPointer* ptrHolder = new IRPointer(&s, "Fill");
    Chain* current = new Chain(ptrHolder, parent, previous, next);
    this->model->addNode(current);

    this->parent = current;
    this->previous = nullptr;
    this->next = nullptr;
    // Process children...
    this->parent = parent;
    this->previous = current;
    this->next = next;
}

SignTree* SignTreeQtModel::IRPointer::getTree() {
    if(this->type == SIGN_TREE)
        return this->signTree;
    else throw NoSuchItemException();
}

SignCell::Builder* SignTreeQtModel::IRPointer::getSignCellBuilder() {
    if(this->type == SIGN_CELL_BUILDER)
        return this->signCellBuilder;
    else throw NoSuchItemException();
}

Sign::DisplayBuilder* SignTreeQtModel::IRPointer::getDisplayBuilder() {
    if(this->type == DISPLAY_BUILDER)
        return this->displayBuilder;
    else throw NoSuchItemException();
}


