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
#include "PageDisplay.h"

PageDisplay::PageDisplay(Display* treeNode) : treeNode(treeNode) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    this->widthSpinner = new QSpinBox(this);
    this->widthSpinner->setMinimum(0);
    this->widthSpinner->setMaximum(999);
    this->widthSpinner->setValue(treeNode->getWidth());
    this->heightSpinner = new QSpinBox(this);
    this->heightSpinner->setMinimum(0);
    this->heightSpinner->setMaximum(999);
    this->heightSpinner->setValue(treeNode->getHeight());

    connect(this->widthSpinner,
                SIGNAL(valueChanged(int)),
                this,
                SLOT(setWidth(int)));
    connect(this->heightSpinner,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(setHeight(int)));

    layout->addWidget(this->widthSpinner);
    layout->addWidget(this->heightSpinner);

    this->setLayout(layout);

}

void PageDisplay::setHeight(int height) {
    this->treeNode->setHeight((unsigned int)height);
}

void PageDisplay::setWidth(int width) {
    this->treeNode->setWidth((unsigned int)width);
}
