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

#include <QVBoxLayout>
#include "PageBlinkAnimation.h"

PageBlinkAnimation::PageBlinkAnimation(BlinkAnimation* treeNode) :
    treeNode(treeNode) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    this->onFramesSpinner = new QSpinBox(this);
    this->onFramesSpinner->setMinimum(0);
    this->onFramesSpinner->setMaximum(999);
    this->onFramesSpinner->setValue(treeNode->getFramesOn());
    this->offFramesSpinner = new QSpinBox(this);
    this->offFramesSpinner->setMinimum(0);
    this->offFramesSpinner->setMaximum(999);
    this->offFramesSpinner->setValue(treeNode->getFramesOff());

    layout->addWidget(onFramesSpinner);
    layout->addWidget(offFramesSpinner);

    this->setLayout(layout);

    connect(this->onFramesSpinner, SIGNAL(valueChanged(int)),
            this, SLOT(setFramesOn(int)));
    connect(this->offFramesSpinner, SIGNAL(valueChanged(int)),
                this, SLOT(setFramesOff(int)));
}

void PageBlinkAnimation::setFramesOn(int frames) {
    this->treeNode->setFramesOn((unsigned int)frames);
}

void PageBlinkAnimation::setFramesOff(int frames) {
    this->treeNode->setFramesOff((unsigned int)frames);
}
