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

#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>
#include "PageMarqueeAnimation.h"

PageMarqueeAnimation::PageMarqueeAnimation(MarqueeAnimation* treeNode) :
    treeNode(treeNode) {
    QWidget* radio_direction = new QWidget(this);
    QGridLayout *layout = new QGridLayout(this);
    QHBoxLayout *radio_direction_layout = new QHBoxLayout(radio_direction);

    QLabel* directionLabel = new QLabel("Direction:", this);
    this->direction_LEFT = new QRadioButton("Right-to-left", radio_direction);
    this->direction_RIGHT = new QRadioButton("Left-to-right", radio_direction);

    QLabel* spaceLabel = new QLabel("Blank space:", this);
    this->spaceSpinner = new QSpinBox(this);
    this->spaceSpinner->setMinimum(0);
    this->spaceSpinner->setMaximum(999);
    this->spaceSpinner->setValue(this->treeNode->getSpace());

    QLabel* durationLabel = new QLabel("Duration:", this);
    this->durationSpinner = new QSpinBox(this);
    this->durationSpinner->setMinimum(0);
    this->durationSpinner->setMaximum(999);
    this->durationSpinner->setValue(this->treeNode->getDurationFrames());

    QLabel* phaseFramesLabel = new QLabel("Initial Phase:", this);
    this->phaseFramesSpinner = new QSpinBox(this);
    this->phaseFramesSpinner->setMinimum(0);
    this->phaseFramesSpinner->setMaximum(999);
    this->phaseFramesSpinner->setValue(treeNode->getInitialPhaseFrames());

    switch(this->treeNode->getDirection()) {
    case MarqueeAnimation::LEFT:
        this->direction_LEFT->setChecked(true);
        break;
    case MarqueeAnimation::RIGHT:
        this->direction_RIGHT->setChecked(true);
        break;
    }

    radio_direction_layout->addWidget(direction_LEFT);
    radio_direction_layout->addWidget(direction_RIGHT);

    layout->addWidget(directionLabel, 0, 0);
    layout->addWidget(radio_direction, 0, 1);
    layout->addWidget(spaceLabel, 1, 0);
    layout->addWidget(this->spaceSpinner, 1, 1);
    layout->addWidget(durationLabel, 2, 0);
    layout->addWidget(this->durationSpinner, 2, 1);
    layout->addWidget(phaseFramesLabel, 3, 0);
    layout->addWidget(phaseFramesSpinner, 3, 1);

    this->setLayout(layout);

    connect(this->direction_LEFT,
            &QRadioButton::toggled,
            this,
            &PageMarqueeAnimation::setDirection_LEFT);

    connect(this->direction_RIGHT,
            &QRadioButton::toggled,
            this,
            &PageMarqueeAnimation::setDirection_RIGHT);

    connect(this->spaceSpinner, SIGNAL(valueChanged(int)),
            this, SLOT(setSpace(int)));

    connect(this->durationSpinner, SIGNAL(valueChanged(int)),
            this, SLOT(setDurationFrames(int)));

    connect(this->phaseFramesSpinner, SIGNAL(valueChanged(int)),
                this, SLOT(setPhaseFrames(int)));
}

void PageMarqueeAnimation::setSpace(int space) {
    this->treeNode->setSpace((unsigned int)space);
}

void PageMarqueeAnimation::setDurationFrames(int frames) {
    this->treeNode->setDurationFrames((unsigned int)frames);
}

void PageMarqueeAnimation::setDirection_LEFT(bool set) {
    if(set) {
        this->treeNode->setDirection(MarqueeAnimation::LEFT);
    }
}

void PageMarqueeAnimation::setDirection_RIGHT(bool set) {
    if(set) {
        this->treeNode->setDirection(MarqueeAnimation::RIGHT);
    }
}

void PageMarqueeAnimation::setPhaseFrames(int frames) {
    this->treeNode->setInitialPhaseFrames(frames);
}
