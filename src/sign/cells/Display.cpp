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

#include "Display.h"
#include "../Sign.h"

Display::Display() : Display(20, 20, Type::DISPLAY_FLIPDISC) {

}

Display::Display(unsigned int width, unsigned int height,
        enum Type type) :
        Display(width, height, type, NULL) {

}

Display::Display(unsigned int width, unsigned int height,
        enum Type type, SignCell *rootCell) :
        width(width), height(height), displayType(type), rootCell(rootCell),
        parentSign(nullptr) {
    if(rootCell != nullptr)
        rootCell->setParent(this);
}

Display::Display(const Display* a) : width(a->width), height(a->height),
        displayType(a->displayType), parentSign(nullptr) {
    try {
        this->rootCell = a->getRootCell()->copy();
        this->rootCell->setParent(this);
    } catch(NoSuchChildException& e) {
        this->rootCell = nullptr;
    }
}

SignCell* Display::copy() {
    return new Display(this);
}

Display::~Display() {
    if(this->rootCell != nullptr)
        delete this->rootCell;
}

void Display::accept(SignTreeVisitor &visitor) {
    visitor.visit(*this);
}

void Display::accept(ConstSignTreeVisitor &visitor) const {
    visitor.visit(*this);
}

bool Display::setParent(const SignCell *parent) {
    throw SetParentFailedException(this, parent);
}

bool Display::setParentSign(const Sign *parent) {
    this->parentSign = parent;
    return true;
}

const Sign* Display::getParentSign() const {
    if(this->parentSign != nullptr)
        return this->parentSign;
    else throw OrphanNodeException(this);
}

bool Display::setRootCell(SignCell *rootCell) {
    SignCell* oldRoot = this->rootCell;
    this->rootCell = rootCell;
    if(rootCell != nullptr)
        try {
            rootCell->setParent(this);
            if(oldRoot != nullptr) {
                oldRoot->setParent(nullptr); // Cannot fail.
            }
        } catch(SignCell::SetParentFailedException& e) {
            this->rootCell = oldRoot;
            return false;
        }
    this->modified();
    this->structureChanged();
    return true;
}

SignCell* Display::getRootCell() const {
    if(this->rootCell != nullptr)
        return this->rootCell;
    throw NoSuchChildException(this, nullptr);
}

enum Display::Type Display::getDisplayType() const {
    return this->displayType;
}

bool Display::setDisplayType(enum Type displayType) {
    this->displayType = displayType;
    this->modified();
    return true;
}

unsigned int Display::getChildHeight(const SignCell* child) const {
    if(child == this->rootCell)
        return this->height;
    else throw NoSuchChildException(this, child);
}

unsigned int Display::getChildWidth(const SignCell* child) const {
    if(child == this->rootCell)
        return this->width;
    else throw NoSuchChildException(this, child);
}

unsigned int Display::getHeight() const {
    return this->height;
}

unsigned int Display::getWidth() const {
    return this->width;
}

void Display::setHeight(const unsigned int height) {
    this->resize(this->width, height);
}

void Display::setWidth(const unsigned int width) {
    this->resize(width, this->height);
}

void Display::resize(const unsigned int width, const unsigned int height) {
    this->height = height;
    this->width = width;
    this->modified();
}

Display::RootCellBuilder* Display::rootCellBuilder() {
    return new RootCellBuilder(this);
}

std::ostream& Display::serialize(std::ostream &strm) const {
    return strm << "Disp { " << this->width << "x" << this->height << " ("
            << this->displayType << ") : " << *(this->rootCell) << " }";
}

std::ostream& operator<<(std::ostream &strm, const Display &s) {
    return s.serialize(strm);
}

void Display::modified() const {
    try {
        this->getParentSign()->modified();\
    } catch(OrphanNodeException& e) { };
}

void Display::callbackDispatch(ConstSignTreeDispatcher *s) const {
    s->dispatchCallback(*this);
}

void Display::callbackDispatch(SignTreeDispatcher *s) {
    s->dispatchCallback(*this);
}

Display::RootCellBuilder::RootCellBuilder(Display *display) : display(display) {
}

bool Display::RootCellBuilder::build(SignCell *child) {
    return this->display->setRootCell(child);
}

void Display::deleteChild(SignTree *child) {
    try {
        if(child == this->getRootCell()) {
            this->setRootCell(nullptr);
            delete child;
        }
    } catch(NoSuchChildException& e) {

    }
}

void Display::deepDetachStructureObserver(SignTreeStructureObserver *observer) {
    this->detachStructureObserver(observer);
    try {
        this->getRootCell()->deepDetachStructureObserver(observer);
    } catch(NoSuchChildException& e) { }
}

json_object* Display::toJSON() const {
    json_object* ret = json_object_new_object();
    json_object_object_add(ret, "type", json_object_new_string("Display"));
    json_object_object_add(ret, "width", json_object_new_int(this->width));
    json_object_object_add(ret, "height", json_object_new_int(this->height));

    std::string displayTypeStr;
    switch(this->displayType) {
    case Display::Type::DISPLAY_FLIPDISC:
        displayTypeStr = "flipdisc";
        break;
    case Display::Type::DISPLAY_MONOCHROME_LED:
        displayTypeStr = "monochrome_led";
        break;
    case Display::Type::DISPLAY_RGB_LED:
        displayTypeStr = "rgb_led";
        break;
    }
    json_object_object_add(ret, "displayType",
            json_object_new_string(displayTypeStr.c_str()));

    try {
        json_object_object_add(ret, "rootCell", this->getRootCell()->toJSON());
    } catch(NoSuchChildException& e) {

    }

    return ret;
}
