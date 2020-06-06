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
#include "PageFill.h"

PageFill::PageFill(Fill* treeNode) : treeNode(treeNode) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    this->pickColor = new QPushButton("Color");
    this->colorPicker = new QColorDialog(this);

    this->pickColor->connect(this->pickColor,
            &QPushButton::pressed,
            this,
            &PageFill::selectColor);

    this->colorPicker->connect(this->colorPicker,
            &QColorDialog::colorSelected,
            this,
            &PageFill::setColor);

    layout->addWidget(pickColor);
    this->setLayout(layout);
}

void PageFill::selectColor() {
    SignColor currentColor = treeNode->getColor();
    QColor qtColor(0, 0, 0, 0);
    try {
        SignColor::RGB val = currentColor.getValue();
        qtColor.setRgb(val.r, val.g, val.b, 0);
    } catch(SignColor::NoRGBValueException& e) {

    }
    this->colorPicker->setCurrentColor(qtColor);
    this->colorPicker->show();
}

void PageFill::setColor(const QColor& color) {
    this->treeNode->setColor(
            SignColor(SignColor::OFF,
                    color.red(), color.green(), color.blue()));
}
