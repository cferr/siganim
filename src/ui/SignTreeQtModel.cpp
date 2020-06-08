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

SignTreeQtModel::SignTreeQtModel(SignTree *tree) : tree(tree) {
    this->childrenTable[nullptr] = QModelIndexVector();
    if(tree != nullptr) {
        ModelBuilder builder(this);
        builder.run(tree);
    }
}

void SignTreeQtModel::observeStructure(const SignTree *sender) {
    // Find the IRPointer linked to sender

    IRPointer* irPointer = nullptr;

    for(auto i = this->pointers.begin(); i < this->pointers.end(); ++i) {
        try {
            if((*i)->getTree() == sender) {
                irPointer = *i;
                break;
            }
        } catch(IRPointer::NoSuchItemException& e) {

        }
    }
    if(irPointer != nullptr) {
        SignTree* nonConstTree = irPointer->getTree();
        // Unregister
        nonConstTree->deepDetachStructureObserver(this);
        QModelIndex observedNodeIndex = this->modelBackward[irPointer];
        QModelIndex parent = observedNodeIndex.parent();
        unsigned int row = observedNodeIndex.row();
        // Delete
        this->removeNode(observedNodeIndex);
        // Rebuild
        ModelBuilder builder(this, parent, row);
        builder.run(nonConstTree);
    }

}

void SignTreeQtModel::addNode(const char *label, QModelIndex &index,
        QModelIndex &parent) {

    this->beginInsertRows(parent, index.row(), index.row());
    IRPointer* ptrHolder = ((IRPointer*)index.internalPointer());
    this->modelBackward[ptrHolder] = index;
    this->parentTable[ptrHolder] = parent;
    this->childrenTable[ptrHolder] = QModelIndexVector();
    this->labels[ptrHolder] = QString(label);
    this->pointers.push_back(ptrHolder);

    IRPointer* parentsPointer = ((IRPointer*)parent.internalPointer());
    QModelIndexVector children = this->childrenTable[parentsPointer];
    children.push_back(index);
    this->childrenTable[parentsPointer] = children;
    this->endInsertRows();

}


void SignTreeQtModel::removeNode(QModelIndex &index) {
    IRPointer* ptrHolder = ((IRPointer*)index.internalPointer());
    // Delete children first
    QModelIndexVector childrenToDelete = this->childrenTable[ptrHolder];
    for(auto i = childrenToDelete.begin(); i != childrenToDelete.end(); ++i)
        this->removeNode(*i);

    QModelIndex parent = index.parent();
    unsigned int row = index.row();
    this->beginRemoveRows(parent, row, row);
    IRPointer* parentPtrHolder = (IRPointer*)(index.parent().internalPointer());
    QModelIndexVector parentsChildren = this->childrenTable[parentPtrHolder];
    if(!parentsChildren.empty()) {
        auto i = parentsChildren.begin();
        while(i != parentsChildren.end())
            if((*i).internalPointer() == ptrHolder)
                i = parentsChildren.erase(i);
            else
                i++;
    }
    this->childrenTable[parentPtrHolder] = parentsChildren;
    this->modelBackward.erase(ptrHolder);
    this->parentTable.erase(ptrHolder);
    this->childrenTable.erase(ptrHolder);
    this->labels.erase(ptrHolder);

    {
        auto i = this->pointers.begin();
        while(i < this->pointers.end())
            if(*i == ptrHolder)
                i = this->pointers.erase(i);
            else ++i;
    }

    delete ptrHolder;
    this->endRemoveRows();
}


QModelIndex SignTreeQtModel::parent(
        const QModelIndex &child) const {
    return this->parentTable.at(((IRPointer*)child.internalPointer()));
}

QVariant SignTreeQtModel::data(const QModelIndex &index, int role) const {
    switch(role) {
    case Qt::DisplayRole:
        return this->labels.at(((IRPointer*)index.internalPointer()));
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
       IRPointer* parentPtr = ((IRPointer*)parent.internalPointer());
       std::map<IRPointer*, QModelIndexVector>::const_iterator iterator =
               this->childrenTable.find(parentPtr);
       if(iterator != this->childrenTable.end()) {
           QModelIndexVector children = (*iterator).second;
           if(row >= 0 && (unsigned int)row < children.size())
               return children.at(row);
           else return QModelIndex();
       } else return QModelIndex();
   }
}

int SignTreeQtModel::rowCount(const QModelIndex &parent) const {
    // TODO case where parent is not in table
    return this->childrenTable.at(((IRPointer*)parent.internalPointer()))
            .size();
}

int SignTreeQtModel::columnCount(const QModelIndex &parent) const {
    return 1;
}

SignTreeQtModel::ModelBuilder::ModelBuilder(SignTreeQtModel *model,
        QModelIndex parentIndex, unsigned int currentRow)
    : parentIndex(parentIndex), currentRow(currentRow), model(model) {
}

SignTreeQtModel::ModelBuilder::ModelBuilder(SignTreeQtModel *model)
    : ModelBuilder(model, QModelIndex(), 0) {
    // Set parent to be invalid.
    // That one bears a null pointer on which we're relying.
}

void SignTreeQtModel::ModelBuilder::run(SignTree* root) {
    root->accept(*this);
}


void SignTreeQtModel::ModelBuilder::visit(Sign &s) {
    s.attachStructureObserver(this->model);
    QModelIndex parent = this->parentIndex;
    unsigned int currentRow = this->currentRow;
    IRPointer* ptrHolder = new IRPointer(&s);
    QModelIndex idx = this->model->createIndex(currentRow, 0,
            (void*)(ptrHolder));
    this->model->addNode("Sign", idx, parent);
    this->parentIndex = idx;
    this->currentRow = 0;
    std::vector<Display*> displays = s.getDisplays();
    for(auto i = displays.begin(); i < displays.end(); ++i) {
        (*i)->accept(*this);
    }
    Sign::DisplayBuilder* displayBuilder = s.displayBuilder();
    IRPointer* dbPtrHolder = new IRPointer(displayBuilder);
    currentRow += 1;
    QModelIndex builderIdx = this->model->createIndex(currentRow, 0,
            (void*)dbPtrHolder);
    this->model->addNode("Add display...", builderIdx, idx);
    this->parentIndex = parent;
    this->currentRow = currentRow + 1;
}

void SignTreeQtModel::ModelBuilder::visit(Display &s) {
    s.attachStructureObserver(this->model);
    QModelIndex parent = this->parentIndex;
    unsigned int currentRow = this->currentRow;
    IRPointer* ptrHolder = new IRPointer(&s);
    QModelIndex idx = this->model->createIndex(currentRow, 0,
            (void*)(ptrHolder));
    this->model->addNode("Display", idx, parent);
    this->parentIndex = idx;
    this->currentRow = 0;
    try {
        s.getRootCell()->accept(*this);
    } catch(SignCell::NoSuchChildException& e) {
        SignCell::Builder* cb = s.rootCellBuilder();
        IRPointer* cbPtrHolder = new IRPointer(cb);
        QModelIndex emptyIdx = this->model->createIndex(0, 0,
                (void*)cbPtrHolder);
        this->model->addNode("Empty", emptyIdx, idx);
    }
    this->parentIndex = parent;
    this->currentRow = currentRow + 1;
}

void SignTreeQtModel::ModelBuilder::visit(Text &s) {
    s.attachStructureObserver(this->model);
    QModelIndex parent = this->parentIndex;
    unsigned int currentRow = this->currentRow;
    IRPointer* ptrHolder = new IRPointer(&s);
    QModelIndex idx = this->model->createIndex(currentRow, 0,
            (void*)(ptrHolder));
    this->model->addNode("Text", idx, parent);
    this->parentIndex = idx;
    // Process children...
    this->parentIndex = parent;
    this->currentRow = currentRow + 1;
}

void SignTreeQtModel::ModelBuilder::visit(Split &s) {
    s.attachStructureObserver(this->model);
    QModelIndex parent = this->parentIndex;
    unsigned int currentRow = this->currentRow;
    IRPointer* ptrHolder = new IRPointer(&s);
    QModelIndex idx = this->model->createIndex(currentRow, 0,
            (void*)(ptrHolder));
    this->model->addNode("Split", idx, parent);
    this->parentIndex = idx;
    this->currentRow = 0;
    try {
        s.getTopOrLeftChild()->accept(*this);
    } catch(SignCell::NoSuchChildException& e) {
        SignCell::Builder* cb = s.topOrLeftChildBuilder();
        IRPointer* cbPtrHolder = new IRPointer(cb);
        QModelIndex emptyIdx = this->model->createIndex(0, 0,
                (void*)cbPtrHolder);
        this->model->addNode("Empty", emptyIdx, idx);
    }
    try {
        s.getBottomOrRightChild()->accept(*this);
    } catch(SignCell::NoSuchChildException& e) {
        SignCell::Builder* cb = s.bottomOrRightChildBuilder();
        IRPointer* cbPtrHolder = new IRPointer(cb);
        QModelIndex emptyIdx = this->model->createIndex(1, 0,
                (void*)cbPtrHolder);
        this->model->addNode("Empty", emptyIdx, idx);
    }
    this->parentIndex = parent;
    this->currentRow = currentRow + 1;
}

void SignTreeQtModel::ModelBuilder::visit(MarqueeAnimation &s) {
    s.attachStructureObserver(this->model);
    QModelIndex parent = this->parentIndex;
    unsigned int currentRow = this->currentRow;
    IRPointer* ptrHolder = new IRPointer(&s);
    QModelIndex idx = this->model->createIndex(currentRow, 0,
            (void*)(ptrHolder));
    this->model->addNode("Marquee", idx, parent);
    this->parentIndex = idx;
    this->currentRow = 0;
    try {
        s.getSubject()->accept(*this);
    } catch(SignCell::NoSuchChildException& e) {
        SignCell::Builder* cb = s.subjectBuilder();
        IRPointer* cbPtrHolder = new IRPointer(cb);
        QModelIndex emptyIdx = this->model->createIndex(0, 0,
                (void*)cbPtrHolder);
        this->model->addNode("Empty", emptyIdx, idx);
    }
    this->parentIndex = parent;
    this->currentRow = currentRow + 1;
}

void SignTreeQtModel::ModelBuilder::visit(BlinkAnimation &s) {
    s.attachStructureObserver(this->model);
    QModelIndex parent = this->parentIndex;
    unsigned int currentRow = this->currentRow;
    IRPointer* ptrHolder = new IRPointer(&s);
    QModelIndex idx = this->model->createIndex(currentRow, 0,
            (void*)(ptrHolder));
    this->model->addNode("Blink", idx, parent);
    this->parentIndex = idx;
    this->currentRow = 0;
    try {
        s.getSubject()->accept(*this);
    } catch(SignCell::NoSuchChildException& e) {
        SignCell::Builder* cb = s.subjectBuilder();
        IRPointer* cbPtrHolder = new IRPointer(cb);
        QModelIndex emptyIdx = this->model->createIndex(0, 0,
                (void*)cbPtrHolder);
        this->model->addNode("Empty", emptyIdx, idx);
    }
    this->parentIndex = parent;
    this->currentRow = currentRow + 1;
}

void SignTreeQtModel::ModelBuilder::visit(Compose &s) {
    s.attachStructureObserver(this->model);
    QModelIndex parent = this->parentIndex;
    unsigned int currentRow = this->currentRow;
    IRPointer* ptrHolder = new IRPointer(&s);
    QModelIndex idx = this->model->createIndex(currentRow, 0,
            (void*)(ptrHolder));
    this->model->addNode("Compose", idx, parent);
    this->parentIndex = idx;
    this->currentRow = 0;
    try {
        s.getBackground()->accept(*this);
    } catch(SignCell::NoSuchChildException& e) {
        SignCell::Builder* cb = s.backgroundBuilder();
        IRPointer* cbPtrHolder = new IRPointer(cb);
        QModelIndex emptyIdx = this->model->createIndex(0, 0,
                (void*)cbPtrHolder);
        this->model->addNode("Empty", emptyIdx, idx);
    }
    try {
        s.getForeground()->accept(*this);
    } catch(SignCell::NoSuchChildException& e) {
        SignCell::Builder* cb = s.foregroundBuilder();
        IRPointer* cbPtrHolder = new IRPointer(cb);
        QModelIndex emptyIdx = this->model->createIndex(1, 0,
                (void*)cbPtrHolder);
        this->model->addNode("Empty", emptyIdx, idx);
    }
    this->parentIndex = parent;
    this->currentRow = currentRow + 1;
}

void SignTreeQtModel::ModelBuilder::visit(Fill &s) {
    s.attachStructureObserver(this->model);
    QModelIndex parent = this->parentIndex;
    unsigned int currentRow = this->currentRow;
    IRPointer* ptrHolder = new IRPointer(&s);
    QModelIndex idx = this->model->createIndex(currentRow, 0,
            (void*)(ptrHolder));
    this->model->addNode("Fill", idx, parent);
    this->parentIndex = idx;
    this->currentRow = 0;
    // Process children...
    this->parentIndex = parent;
    this->currentRow = currentRow + 1;
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
