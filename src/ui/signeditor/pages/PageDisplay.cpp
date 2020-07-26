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
#include "PageDisplay.h"

PageDisplay::PageDisplay(Display* treeNode) : treeNode(treeNode) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    QWidget* radioWidget_displayType = new QWidget(this);
    QHBoxLayout* radioLayout_displayType = new QHBoxLayout();

    this->widthSpinner = new QSpinBox(this);
    this->widthSpinner->setMinimum(0);
    this->widthSpinner->setMaximum(999);
    this->widthSpinner->setValue(treeNode->getWidth());
    this->heightSpinner = new QSpinBox(this);
    this->heightSpinner->setMinimum(0);
    this->heightSpinner->setMaximum(999);
    this->heightSpinner->setValue(treeNode->getHeight());
    this->displayType_DISPLAY_FLIPDISC = new QRadioButton("Flip disc",
            radioWidget_displayType);
    this->displayType_DISPLAY_MONOCHROME_LED = new QRadioButton(
            "Monochrome LEDs", radioWidget_displayType);
    this->displayType_DISPLAY_RGB_LED = new QRadioButton("RGB LEDs",
            radioWidget_displayType);

    switch(this->treeNode->getDisplayType()) {
    case Display::Type::DISPLAY_FLIPDISC:
        this->displayType_DISPLAY_FLIPDISC->setChecked(true);
        break;
    case Display::Type::DISPLAY_MONOCHROME_LED:
        this->displayType_DISPLAY_MONOCHROME_LED->setChecked(true);
        break;
    case Display::Type::DISPLAY_RGB_LED:
        this->displayType_DISPLAY_RGB_LED->setChecked(true);
        break;
    }

    connect(this->widthSpinner,
                SIGNAL(valueChanged(int)),
                this,
                SLOT(setWidth(int)));
    connect(this->heightSpinner,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(setHeight(int)));

    connect(this->displayType_DISPLAY_FLIPDISC,
            &QRadioButton::toggled,
            this,
            &PageDisplay::setDisplayType_DISPLAY_FLIPDISC);

    connect(this->displayType_DISPLAY_MONOCHROME_LED,
                &QRadioButton::toggled,
                this,
                &PageDisplay::setDisplayType_DISPLAY_MONOCHROME_LED);

    connect(this->displayType_DISPLAY_RGB_LED,
                &QRadioButton::toggled,
                this,
                &PageDisplay::setDisplayType_DISPLAY_RGB_LED);

    radioLayout_displayType->addWidget(displayType_DISPLAY_FLIPDISC);
    radioLayout_displayType->addWidget(displayType_DISPLAY_MONOCHROME_LED);
    radioLayout_displayType->addWidget(displayType_DISPLAY_RGB_LED);

    layout->addLayout(radioLayout_displayType);
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

void PageDisplay::setDisplayType_DISPLAY_RGB_LED(bool set) {
    if(set) {
        this->treeNode->setDisplayType(Display::Type::DISPLAY_RGB_LED);
    }
}

void PageDisplay::setDisplayType_DISPLAY_MONOCHROME_LED(bool set) {
    if(set) {
        this->treeNode->setDisplayType(Display::Type::DISPLAY_MONOCHROME_LED);
    }
}

void PageDisplay::setDisplayType_DISPLAY_FLIPDISC(bool set) {
    if(set) {
        this->treeNode->setDisplayType(Display::Type::DISPLAY_FLIPDISC);
    }
}
