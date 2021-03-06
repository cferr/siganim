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


#include "Compose.h"

Compose::Compose() : Compose(nullptr, nullptr) {

}

Compose::Compose(SignCell *background, SignCell *foreground) :
    background(background), foreground(foreground) {
    if(background != nullptr)
        background->setParent(this);
    if(foreground != nullptr)
        foreground->setParent(this);
}

Compose::Compose(const Compose* a) {
    try {
        this->background = a->getBackground()->copy();
        this->background->setParent(this);
    } catch(NoSuchChildException& e) {
        this->background = nullptr;
    }
    try {
        this->foreground= a->getForeground()->copy();
        this->foreground->setParent(this);
    } catch(NoSuchChildException& e) {
        this->foreground = nullptr;
    }
}

SignCell* Compose::copy() {
    return new Compose(this);
}

Compose::~Compose() {
    if(this->background != nullptr)
        delete this->background;
    if(this->foreground != nullptr)
        delete this->foreground;
}

Compose::SignCell* Compose::getForeground() const {
    if(this->foreground != nullptr)
        return this->foreground;
    throw NoSuchChildException(this, nullptr);
}

Compose::SignCell* Compose::getBackground() const {
    if(this->background != nullptr)
        return this->background;
    throw NoSuchChildException(this, nullptr);
}

bool Compose::setForeground(SignCell *foreground) {
    SignCell* oldForeground = this->foreground;
    this->foreground = foreground;
    try {
        if(this->foreground != nullptr)
            this->foreground->setParent(this);
        if(oldForeground != nullptr)
            oldForeground->setParent(nullptr);
        this->modified();
        this->structureChanged();
    } catch(SetParentFailedException& e) {
        this->foreground = oldForeground;
        return false;
    }
    return true;
}

bool Compose::setBackground(SignCell *background) {
    SignCell* oldBackground = this->background;
    this->background = background;
    try {
        if(this->background != nullptr)
            this->background->setParent(this);
        if(oldBackground != nullptr)
            oldBackground->setParent(nullptr);
        this->modified();
        this->structureChanged();
    } catch(SetParentFailedException& e) {
        this->background = oldBackground;
        return false;
    }
    return true;
}

unsigned int Compose::getHeight() const {
    return this->getParent()->getChildHeight(this);
}

unsigned int Compose::getWidth() const {
    return this->getParent()->getChildWidth(this);
}

unsigned int Compose::getChildHeight(const SignCell *child) const {
    if(child == this->foreground || child == this->background) {
        return this->getHeight();
    } else throw NoSuchChildException(this, child);
}

unsigned int Compose::getChildWidth(const SignCell *child) const {
    if(child == this->foreground || child == this->background) {
        return this->getWidth();
    } else throw NoSuchChildException(this, child);
}

void Compose::accept(SignTreeVisitor &visitor) {
    visitor.visit(*this);
}

void Compose::accept(ConstSignTreeVisitor &visitor) const {
    visitor.visit(*this);
}

void Compose::callbackDispatch(ConstSignTreeDispatcher *s) const {
    s->dispatchCallback(*this);
}

void Compose::callbackDispatch(SignTreeDispatcher *s) {
    s->dispatchCallback(*this);
}

void Compose::deleteChild(SignTree *child) {
    try {
        if(child == this->getBackground()) {
            this->setBackground(nullptr);
            delete child;
        }
    } catch(NoSuchChildException& e) {

    }

    try {
        if(child == this->getForeground()) {
            this->setForeground(nullptr);
            delete child;
        }
    } catch(NoSuchChildException& e) {

    }
}

std::ostream& Compose::serialize(std::ostream &strm) const {
    return strm << "Compose [" << *(this->background) << ", "
            << *(this->foreground) << "]";
}

std::ostream& operator <<(std::ostream &strm, const Compose &s) {
    return s.serialize(strm);
}

Compose::ForegroundBuilder::ForegroundBuilder(Compose *compose) :
    compose(compose) {
}

bool Compose::ForegroundBuilder::build(SignCell *child) {
    return this->compose->setForeground(child);
}

Compose::BackgroundBuilder::BackgroundBuilder(Compose *compose) :
    compose(compose) {
}

bool Compose::BackgroundBuilder::build(SignCell *child) {
    return this->compose->setBackground(child);
}

Compose::BackgroundBuilder* Compose::backgroundBuilder() {
    return new BackgroundBuilder(this);
}

Compose::ForegroundBuilder* Compose::foregroundBuilder() {
    return new ForegroundBuilder(this);
}

void Compose::deepDetachStructureObserver(SignTreeStructureObserver *observer) {
    this->detachStructureObserver(observer);
    try {
        this->getBackground()->deepDetachStructureObserver(observer);
    } catch(NoSuchChildException& e) { }
    try {
        this->getForeground()->deepDetachStructureObserver(observer);
    } catch(NoSuchChildException& e) { }
}

json_object* Compose::toJSON() const {
    json_object* ret = json_object_new_object();
    json_object_object_add(ret, "type", json_object_new_string("Compose"));

    try {
        json_object_object_add(ret, "background",
                this->getBackground()->toJSON());
    } catch(NoSuchChildException& e) {

    }
    try {
        json_object_object_add(ret, "foreground",
                this->getForeground()->toJSON());
    } catch(NoSuchChildException& e) {

    }

    return ret;
}
