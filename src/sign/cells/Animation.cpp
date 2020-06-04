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

#include "Animation.h"

Animation::Animation(SignCell* subject, unsigned int durationFrames) :
    subject(subject), durationFrames(durationFrames) {
    if(subject != nullptr)
        subject->setParent(this);
}

bool Animation::setSubject(SignCell* subject) {
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
    this->modified();
    return true;
}

SignCell* Animation::getSubject() const {
    return this->subject;
}

unsigned int Animation::getHeight() const {
    return this->getParent()->getChildHeight(this);
}

unsigned int Animation::getWidth() const {
    return this->getParent()->getChildWidth(this);
}

unsigned int Animation::getChildHeight(const SignCell *child) const {
    if(child == this->subject)
        return this->getHeight();
    else throw NoSuchChildException(this, child);
}

unsigned int Animation::getChildWidth(const SignCell *child) const {
    if(child == this->subject)
        return this->getWidth();
    else throw NoSuchChildException(this, child);
}

unsigned int Animation::getDurationFrames() const {
    return this->durationFrames;
}
