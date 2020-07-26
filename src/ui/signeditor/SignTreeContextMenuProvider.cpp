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

#include "../../sign/Sign.h"
#include "../../sign/cells/Display.h"
#include "../../sign/cells/Text.h"
#include "../../sign/cells/Split.h"
#include "../../sign/cells/MarqueeAnimation.h"
#include "../../sign/cells/BlinkAnimation.h"
#include "../../sign/cells/Compose.h"
#include "../../sign/cells/Fill.h"
#include "SignTreeContextMenuProvider.h"

SignTreeContextMenuProvider::SignTreeContextMenuProvider
    (SignTree *currentTree, SignTree* parentTree) :
    currentTree(currentTree), parentTree(parentTree), cellBuilder(nullptr),
    displayBuilder(nullptr), contextMenu(nullptr) {
    if(currentTree != nullptr) {
        this->contextMenu = new QMenu();
        QAction* deleteAction = new QAction("Delete");
        deleteAction->connect(deleteAction, &QAction::triggered,
                    this, &SignTreeContextMenuProvider::deleteNode);
        this->contextMenu->addAction(deleteAction);
    }
}

SignTreeContextMenuProvider::SignTreeContextMenuProvider
    (SignCell::Builder *builder) :
        currentTree(nullptr), parentTree(nullptr), cellBuilder(builder),
        displayBuilder(nullptr), contextMenu(nullptr) {
    this->contextMenu = new QMenu();

    QAction* newBlinkAnimation = new QAction("New Blink");
    QAction* newMarqueeAnimation = new QAction("New Marquee");
    QAction* newCompose = new QAction("New Compose");
    QAction* newFill = new QAction("New Fill");
    QAction* newSplit = new QAction("New Split");
    QAction* newText = new QAction("New Text");

    newBlinkAnimation->connect(newBlinkAnimation, &QAction::triggered,
            this, &SignTreeContextMenuProvider::buildBlinkAnimation);
    newMarqueeAnimation->connect(newMarqueeAnimation, &QAction::triggered,
            this, &SignTreeContextMenuProvider::buildMarqueeAnimation);
    newCompose->connect(newCompose, &QAction::triggered,
            this, &SignTreeContextMenuProvider::buildCompose);
    newFill->connect(newFill, &QAction::triggered,
            this, &SignTreeContextMenuProvider::buildFill);
    newSplit->connect(newSplit, &QAction::triggered,
            this, &SignTreeContextMenuProvider::buildSplit);
    newText->connect(newText, &QAction::triggered,
            this, &SignTreeContextMenuProvider::buildText);

    this->contextMenu->addAction(newBlinkAnimation);
    this->contextMenu->addAction(newMarqueeAnimation);
    this->contextMenu->addAction(newCompose);
    this->contextMenu->addAction(newFill);
    this->contextMenu->addAction(newSplit);
    this->contextMenu->addAction(newText);
}

SignTreeContextMenuProvider::SignTreeContextMenuProvider
    (Sign::DisplayBuilder *builder) :
        currentTree(nullptr), parentTree(nullptr), cellBuilder(nullptr),
        displayBuilder(builder), contextMenu(nullptr) {
    this->contextMenu = new QMenu();
    QAction* newDisplay = new QAction("New display");
    newDisplay->connect(newDisplay, &QAction::triggered,
            this, &SignTreeContextMenuProvider::buildDisplay);
    this->contextMenu->addAction(newDisplay);
}


void SignTreeContextMenuProvider::buildSign(bool checked) {
}

void SignTreeContextMenuProvider::buildDisplay(bool checked) {
    this->displayBuilder->build(new Display());
}

void SignTreeContextMenuProvider::buildText(bool checked) {
    this->cellBuilder->build(new Text());
}

void SignTreeContextMenuProvider::buildSplit(bool checked) {
    this->cellBuilder->build(new Split());
}

void SignTreeContextMenuProvider::buildMarqueeAnimation(bool checked) {
    this->cellBuilder->build(new MarqueeAnimation());
}

void SignTreeContextMenuProvider::buildBlinkAnimation(bool checked) {
    this->cellBuilder->build(new BlinkAnimation());
}

void SignTreeContextMenuProvider::buildCompose(bool checked) {
    this->cellBuilder->build(new Compose());
}

void SignTreeContextMenuProvider::buildFill(bool checked) {
    this->cellBuilder->build(new Fill());
}

void SignTreeContextMenuProvider::deleteNode(bool checked) {
    if(this->parentTree != nullptr) {
        this->parentTree->deleteChild(this->currentTree);
    }
}

QMenu* SignTreeContextMenuProvider::getContextMenu() {
    return this->contextMenu;
}

