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

Animation::Animation(SignCell* subject, unsigned int durationFrames,
        unsigned int initialPhaseFrames) :
    subject(subject), durationFrames(durationFrames),
    initialPhaseFrames(initialPhaseFrames) {
    if(subject != nullptr)
        subject->setParent(this);
}

Animation::Animation(const Animation* a) : durationFrames(a->durationFrames),
        initialPhaseFrames(a->initialPhaseFrames) {
    try {
        this->subject = a->getSubject()->copy();
        this->subject->setParent(this);
    } catch(NoSuchChildException& e) {
        this->subject = nullptr;
    }
}

Animation::~Animation() {
    if(this->subject != nullptr)
        delete this->subject;
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
    this->structureChanged();
    return true;
}

SignCell* Animation::getSubject() const {
    if(this->subject != nullptr)
        return this->subject;
    throw NoSuchChildException(this, nullptr);
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

Animation::SubjectBuilder* Animation::subjectBuilder() {
    return new SubjectBuilder(this);
}

unsigned int Animation::getDurationFrames() const {
    return this->durationFrames;
}

Animation::SubjectBuilder::SubjectBuilder(Animation *animation) :
    animation(animation) {
}

bool Animation::SubjectBuilder::build(SignCell* child) {
    return this->animation->setSubject(child);
}

void Animation::deleteChild(SignTree *child) {
    try {
        if(child == this->getSubject()) {
            this->setSubject(nullptr);
            delete child;
        }
    } catch(NoSuchChildException& e) {

    }
}

void Animation::deepDetachStructureObserver(
        SignTreeStructureObserver *observer) {
    this->detachStructureObserver(observer);
    try {
        this->getSubject()->deepDetachStructureObserver(observer);
    } catch(NoSuchChildException& e) { }
}

void Animation::setInitialPhaseFrames(unsigned int initialPhaseFrames) {
    this->initialPhaseFrames = initialPhaseFrames;
    this->modified();
}

unsigned int Animation::getInitialPhaseFrames() const {
    return this->initialPhaseFrames;
}
