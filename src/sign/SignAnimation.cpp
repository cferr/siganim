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

#include "SignAnimation.h"

SignAnimation::SignAnimation(SignCell* subject, unsigned int durationFrames) :
    subject(nullptr), durationFrames(durationFrames) {
    this->setSubject(subject);
}

bool SignAnimation::setSubject(SignCell* subject) {
    SignCell* oldSubject = this->subject;
    this->subject = subject;
    try {
        if(this->subject != nullptr)
            this->subject->setParent(this);
        if(oldSubject != nullptr)
            oldSubject->setParent(nullptr);
    } catch(SetParentFailedException& e) {
        this->subject = oldSubject;
        return false;
    }
    return true;
}

bool SignAnimation::setParent(const SignCell* parent) {
    this->parent = parent;
    return true;
}

SignCell* SignAnimation::getSubject() const {
    return this->subject;
}

unsigned int SignAnimation::getHeight() const {
    if(this->parent != nullptr) {
        return this->parent->getChildHeight(this);
    } else throw OrphanNodeException(this);
}

unsigned int SignAnimation::getWidth() const {
    if(this->parent != nullptr) {
        return this->parent->getChildWidth(this);
    } else throw OrphanNodeException(this);
}

unsigned int SignAnimation::getChildHeight(const SignCell *child) const {
    if(child == this->subject)
        return this->getHeight();
    else throw NoSuchChildException(this, child);
}

unsigned int SignAnimation::getChildWidth(const SignCell *child) const {
    if(child == this->subject)
        return this->getWidth();
    else throw NoSuchChildException(this, child);
}

unsigned int SignAnimation::getDurationFrames() const {
    return this->durationFrames;
}
