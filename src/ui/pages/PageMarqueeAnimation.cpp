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

#include <QHBoxLayout>
#include <QVBoxLayout>
#include "PageMarqueeAnimation.h"

PageMarqueeAnimation::PageMarqueeAnimation(MarqueeAnimation* treeNode) :
    treeNode(treeNode) {
    QWidget* dirRadioGroup = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *radioLayout = new QHBoxLayout(dirRadioGroup);

    this->directionLeft = new QRadioButton("Right-to-left", dirRadioGroup);
    this->directionRight = new QRadioButton("Left-to-right", dirRadioGroup);

    this->spaceSpinner = new QSpinBox(this);
    this->spaceSpinner->setMinimum(0);
    this->spaceSpinner->setValue(this->treeNode->getSpace());

    this->durationSpinner = new QSpinBox(this);
    this->durationSpinner->setMinimum(0);
    this->durationSpinner->setValue(this->treeNode->getDurationFrames());

    switch(this->treeNode->getDirection()) {
    case MarqueeAnimation::LEFT:
        this->directionLeft->setChecked(true);
        break;
    case MarqueeAnimation::RIGHT:
        this->directionRight->setChecked(true);
        break;
    }

    radioLayout->addWidget(directionLeft);
    radioLayout->addWidget(directionRight);

    layout->addWidget(dirRadioGroup);
    layout->addWidget(this->spaceSpinner);
    layout->addWidget(this->durationSpinner);

    this->setLayout(layout);

    connect(this->directionLeft,
            &QRadioButton::toggled,
            this,
            &PageMarqueeAnimation::setDirectionLeft);

    connect(this->directionRight,
            &QRadioButton::toggled,
            this,
            &PageMarqueeAnimation::setDirectionRight);

    connect(this->spaceSpinner, SIGNAL(valueChanged(int)),
            this, SLOT(setSpace(int)));

    connect(this->durationSpinner, SIGNAL(valueChanged(int)),
            this, SLOT(setDuration(int)));
}

void PageMarqueeAnimation::setSpace(int space) {
    this->treeNode->setSpace((unsigned int)space);
}

void PageMarqueeAnimation::setDuration(int frames) {
    this->treeNode->setDurationFrames((unsigned int)frames);
}

void PageMarqueeAnimation::setDirectionLeft(bool set) {
    if(set) {
        this->treeNode->setDirection(MarqueeAnimation::LEFT);
    }
}

void PageMarqueeAnimation::setDirectionRight(bool set) {
    if(set) {
        this->treeNode->setDirection(MarqueeAnimation::RIGHT);
    }
}
