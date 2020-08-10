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
#include <QGridLayout>
#include "PageBlinkAnimation.h"

PageBlinkAnimation::PageBlinkAnimation(BlinkAnimation* treeNode) :
    treeNode(treeNode) {
    QGridLayout* layout = new QGridLayout(this);
    QLabel* onFramesLabel = new QLabel("Frames on:", this);
    this->onFramesSpinner = new QSpinBox(this);
    this->onFramesSpinner->setMinimum(0);
    this->onFramesSpinner->setMaximum(999);
    this->onFramesSpinner->setValue(treeNode->getFramesOn());
    QLabel* offFramesLabel = new QLabel("Frames off:", this);
    this->offFramesSpinner = new QSpinBox(this);
    this->offFramesSpinner->setMinimum(0);
    this->offFramesSpinner->setMaximum(999);
    this->offFramesSpinner->setValue(treeNode->getFramesOff());
    QLabel* phaseFramesLabel = new QLabel("Initial Phase:", this);
    this->phaseFramesSpinner = new QSpinBox(this);
    this->phaseFramesSpinner->setMinimum(0);
    this->phaseFramesSpinner->setMaximum(999);
    this->phaseFramesSpinner->setValue(treeNode->getInitialPhaseFrames());

    layout->addWidget(onFramesLabel, 0, 0);
    layout->addWidget(onFramesSpinner, 0, 1);
    layout->addWidget(offFramesLabel, 1, 0);
    layout->addWidget(offFramesSpinner, 1, 1);
    layout->addWidget(phaseFramesLabel, 2, 0);
    layout->addWidget(phaseFramesSpinner, 2, 1);

    this->setLayout(layout);

    connect(this->onFramesSpinner, SIGNAL(valueChanged(int)),
            this, SLOT(setFramesOn(int)));
    connect(this->offFramesSpinner, SIGNAL(valueChanged(int)),
                this, SLOT(setFramesOff(int)));
    connect(this->phaseFramesSpinner, SIGNAL(valueChanged(int)),
                this, SLOT(setPhaseFrames(int)));
}

void PageBlinkAnimation::setFramesOn(int frames) {
    this->treeNode->setFramesOn((unsigned int)frames);
}

void PageBlinkAnimation::setFramesOff(int frames) {
    this->treeNode->setFramesOff((unsigned int)frames);
}

void PageBlinkAnimation::setPhaseFrames(int frames) {
    this->treeNode->setInitialPhaseFrames(frames);
}
