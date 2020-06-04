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
#include "../sign/cells/Display.h"
#include "../sign/cells/Text.h"
#include "../sign/cells/Split.h"
#include "../sign/cells/MarqueeAnimation.h"
#include "../sign/cells/BlinkAnimation.h"
#include "../sign/cells/Compose.h"
#include "../sign/cells/Fill.h"
#include "SignTreeQtModel.h"


SignTreeQtModel::SignTreeQtModel(SignTree *underlyingTreeItem,
        SignTreeDetailsWidget* detailsWidget) :
    underlyingTreeItem(underlyingTreeItem), detailsWidget(detailsWidget) {
    if(underlyingTreeItem != nullptr) {
        underlyingTreeItem->attachStructureObserver(this);
    }
    this->rebuild();
}

void SignTreeQtModel::observeStructure(const SignTree *sender) {
    if(sender == this->underlyingTreeItem) {
        this->rebuild();
    }
}

void SignTreeQtModel::rebuild() {
    // Delete our children
    QList<QTreeWidgetItem*> children = this->takeChildren();
    for(auto i = children.begin(); i < children.end(); ++i)
        delete *i;

    this->underlyingTreeItem->callbackDispatch(this);

}

// Class-specific builder functions

void SignTreeQtModel::dispatchCallback(const Sign &s) {
    this->setText(0, "Sign");

    std::vector<Display*> displays = s.getDisplays();
    for(auto i = displays.begin(); i < displays.end(); ++i) {
        this->addChild(new SignTreeQtModel(*i, this->detailsWidget));
    }
}

void SignTreeQtModel::dispatchCallback(const Display &s) {
    this->setText(0, "Display");
    try {
        this->addChild(new SignTreeQtModel(s.getRootCell(),
                this->detailsWidget));
    } catch(SignCell::NoSuchChildException& e) {

    }
}

void SignTreeQtModel::dispatchCallback(const Text &s) {
    this->setText(0, "Text");
}

void SignTreeQtModel::dispatchCallback(const Split &s) {
    this->setText(0, "Split");
    try {
        this->addChild(new SignTreeQtModel(s.getTopOrLeftChild(),
                this->detailsWidget));
    } catch(SignCell::NoSuchChildException& e) {

    }
    try {
        this->addChild(new SignTreeQtModel(s.getBottomOrRightChild(),
                this->detailsWidget));
    } catch(SignCell::NoSuchChildException& e) {

    }
}

void SignTreeQtModel::dispatchCallback(const MarqueeAnimation &s) {
    this->setText(0, "Marquee");
    try {
        this->addChild(new SignTreeQtModel(s.getSubject(),
                this->detailsWidget));
    } catch(SignCell::NoSuchChildException& e) {

    }
}

void SignTreeQtModel::dispatchCallback(const BlinkAnimation &s) {
    this->setText(0, "Blink");
    try {
        this->addChild(new SignTreeQtModel(s.getSubject(),
                this->detailsWidget));
    } catch(SignCell::NoSuchChildException& e) {

    }
}

void SignTreeQtModel::dispatchCallback(const Compose &s) {
    this->setText(0, "Compose");
    try {
        this->addChild(new SignTreeQtModel(s.getBackground(),
                this->detailsWidget));
    } catch(SignCell::NoSuchChildException& e) {

    }
    try {
        this->addChild(new SignTreeQtModel(s.getForeground(),
                this->detailsWidget));
    } catch(SignCell::NoSuchChildException& e) {

    }
}

void SignTreeQtModel::dispatchCallback(const Fill &s) {
    this->setText(0, "Fill");
}
