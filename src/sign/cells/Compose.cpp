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

Compose::Compose(SignCell *background, SignCell *foreground) :
    background(background), foreground(foreground) {
    if(background != nullptr)
        background->setParent(this);
    if(foreground != nullptr)
        foreground->setParent(this);
}

Compose::SignCell* Compose::getForeground() const {
    return this->foreground;
}

Compose::SignCell* Compose::getBackground() const {
    return this->background;
}

void Compose::setForeground(SignCell *foreground) {
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
    }
}

void Compose::setBackground(SignCell *background) {
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
    }
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

void Compose::callbackDispatch(SignTreeStructureObserver *s) const {
    s->dispatchCallback(*this);
}

std::ostream& Compose::serialize(std::ostream &strm) const {
    return strm << "Compose [" << *(this->background) << ", "
            << *(this->foreground) << "]";
}

std::ostream& operator <<(std::ostream &strm, const Compose &s) {
    return s.serialize(strm);
}
