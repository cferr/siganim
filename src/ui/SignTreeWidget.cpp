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

#include <QMenu>
#include <QContextMenuEvent>
#include "SignTreeContextMenuProvider.h"
#include "SignTreeQtModel.h"
#include "SignTreeWidget.h"

SignTreeWidget::SignTreeWidget(QWidget* parent,
        SignTreeDetailsWidget* detailsWidget) : QTreeView(parent),
        detailsWidget(detailsWidget) {
    this->setUniformRowHeights(true);
}

void SignTreeWidget::contextMenuEvent(QContextMenuEvent* event) {
    QModelIndex idx = this->currentIndex();
    if(idx.isValid()) {
        SignTreeQtModel::IRPointer* ptr =
                ((SignTreeQtModel::Chain*)(idx.internalPointer()))
                    ->getSignModelPtr();
        QMenu *menu = nullptr;
        SignTreeContextMenuProvider* menuProvider = nullptr;
        // Any better way to do this except a visitor?
        if(ptr != nullptr) {
            try {
                SignTree* current = ptr->getTree();
                if(idx.parent().isValid()) {
                    SignTreeQtModel::IRPointer* parentPtr =
                            ((SignTreeQtModel::Chain*)(idx.parent()
                                    .internalPointer()))->getSignModelPtr();
                    if(parentPtr != nullptr) {
                        SignTree* parent = parentPtr->getTree();
                        menuProvider = new SignTreeContextMenuProvider(current,
                                parent);
                        menu = menuProvider->getContextMenu();
                    }
                }
            } catch(SignTreeQtModel::IRPointer::NoSuchItemException& e) {
                try {
                    SignCell::Builder* builder = ptr->getSignCellBuilder();
                    menuProvider = new SignTreeContextMenuProvider(builder);
                    menu = menuProvider->getContextMenu();
                } catch(SignTreeQtModel::IRPointer::NoSuchItemException& e) {
                    try {
                        Sign::DisplayBuilder* builder =
                                ptr->getDisplayBuilder();
                        menuProvider = new SignTreeContextMenuProvider(builder);
                        menu = menuProvider->getContextMenu();
                    } catch(SignTreeQtModel::IRPointer::NoSuchItemException&
                            e) {

                    }
                }
            }
            if(menu != nullptr) {
                menu->exec(event->globalPos());
                delete menu;
                delete menuProvider;
            }
        }
    }
}

void SignTreeWidget::currentChanged(const QModelIndex &current,
        const QModelIndex &previous) {
    if(current.isValid()) {
        SignTreeQtModel::IRPointer* ptr =
                ((SignTreeQtModel::Chain*)current.internalPointer())
                    ->getSignModelPtr();
        try {
            if(ptr != nullptr) {
                SignTree* t = ptr->getTree();
                this->detailsWidget->update(t);
            }
        } catch(SignTreeQtModel::IRPointer::NoSuchItemException& e) {
            this->detailsWidget->updateEmpty();
        }
    }
}

void SignTreeWidget::expandTreeRows(const QModelIndex &parent, int first,
        int last) {
    this->expand(parent);
    this->setCurrentIndex(parent);
}
