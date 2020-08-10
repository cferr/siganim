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

#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include "PageSplit.h"

PageSplit::PageSplit(Split* treeNode) : treeNode(treeNode) {
    QGridLayout *layout = new QGridLayout(this);
    QGroupBox* directionRadioWidget = new QGroupBox("Split direction:", this);
    QVBoxLayout* radioLayout = new QVBoxLayout(directionRadioWidget);

    QLabel* posLabel = new QLabel("Split position:", this);
    this->posSpinner = new QSpinBox();
    this->posSpinner->setValue(treeNode->getSplitPos());
    this->posSpinner->setMinimum(0);
//    QLabel* dirLabel = new QLabel("Split direction:", this);
    this->radioDirHorizontal = new QRadioButton("Horizontal",
            directionRadioWidget);
    this->radioDirVertical = new QRadioButton("Vertical",
            directionRadioWidget);
    this->radioDirDiagonalSWNE = new QRadioButton("SW-NE Diagonal",
            directionRadioWidget);
    this->radioDirDiagonalNWSE = new QRadioButton("NW-SE Diagonal",
            directionRadioWidget);

    switch(treeNode->getSplitDirection()) {
    case Split::SPLIT_HORIZONTAL:
        this->radioDirHorizontal->setChecked(true);
        break;
    case Split::SPLIT_VERTICAL:
        this->radioDirVertical->setChecked(true);
        break;
    case Split::SPLIT_SW_NE_DIAGONAL:
        this->radioDirDiagonalSWNE->setChecked(true);
        break;
    case Split::SPLIT_NW_SE_DIAGONAL:
        this->radioDirDiagonalNWSE->setChecked(true);
        break;
    }

    this->posSpinner->connect(this->posSpinner,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(setSplitPos(int)));
    this->radioDirHorizontal->connect(this->radioDirHorizontal,
            &QRadioButton::toggled,
            this,
            &PageSplit::setDirHorizontal);
    this->radioDirVertical->connect(this->radioDirVertical,
            &QRadioButton::toggled,
            this,
            &PageSplit::setDirVertical);
    this->radioDirDiagonalNWSE->connect(this->radioDirDiagonalNWSE,
            &QRadioButton::toggled,
            this,
            &PageSplit::setDirDiagonalNWSE);
    this->radioDirDiagonalSWNE->connect(this->radioDirDiagonalSWNE,
            &QRadioButton::toggled,
            this,
            &PageSplit::setDirDiagonalSWNE);

    layout->addWidget(posLabel, 0, 0);
    layout->addWidget(this->posSpinner, 0, 1);
    radioLayout->addWidget(this->radioDirHorizontal);
    radioLayout->addWidget(this->radioDirVertical);
    radioLayout->addWidget(this->radioDirDiagonalSWNE);
    radioLayout->addWidget(this->radioDirDiagonalNWSE);

    directionRadioWidget->setLayout(radioLayout);
//    layout->addWidget(dirLabel, 1, 0);
    layout->addWidget(directionRadioWidget, 1, 0, 1, 2);

    this->setLayout(layout);
}

void PageSplit::setDirHorizontal(bool set) {
    if(set) {
        this->treeNode->setSplitDirection(Split::SPLIT_HORIZONTAL);
    }
}

void PageSplit::setDirVertical(bool set) {
    if(set) {
        this->treeNode->setSplitDirection(Split::SPLIT_VERTICAL);
    }
}

void PageSplit::setDirDiagonalSWNE(bool set) {
    if(set) {
        this->treeNode->setSplitDirection(Split::SPLIT_SW_NE_DIAGONAL);
    }
}

void PageSplit::setDirDiagonalNWSE(bool set) {
    if(set) {
        this->treeNode->setSplitDirection(Split::SPLIT_NW_SE_DIAGONAL);
    }
}

void PageSplit::setSplitPos(int pos) {
    unsigned int uPos = (unsigned int) pos;
    this->treeNode->setSplitPos(uPos);
}
