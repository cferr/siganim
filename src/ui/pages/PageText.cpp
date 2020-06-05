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

#include <unicode/unistr.h>
#include <QString>
#include <QVBoxLayout>
#include "PageText.h"
#include "../../sign/SignColor.h"

PageText::PageText(Text* treeNode) : treeNode(treeNode) {
    this->textLine = new QLineEdit(this);
    icu::UnicodeString* text = treeNode->getText();
    this->textLine->setText(
            QString::fromUtf16(text->getBuffer(), text->length()));
    this->halignLeft = new QPushButton("Left");
    this->halignCenter = new QPushButton("Center");
    this->halignRight = new QPushButton("Right");
    this->halignJustify = new QPushButton("Justify");
    this->pickColor = new QPushButton("Color");
    this->colorPicker = new QColorDialog(this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(this->textLine);
    layout->addWidget(this->halignLeft);
    layout->addWidget(this->halignCenter);
    layout->addWidget(this->halignRight);
    layout->addWidget(this->halignJustify);
    layout->addWidget(this->pickColor);
    this->setLayout(layout);

    this->textLine->connect(this->textLine,
            &QLineEdit::textChanged,
            this,
            &PageText::updateText);
    this->halignLeft->connect(this->halignLeft,
            &QPushButton::pressed,
            this,
            &PageText::setHAlignLeft);
    this->halignCenter->connect(this->halignCenter,
            &QPushButton::pressed,
            this,
            &PageText::setHAlignCenter);
    this->halignRight->connect(this->halignRight,
            &QPushButton::pressed,
            this,
            &PageText::setHAlignRight);
    this->halignJustify->connect(this->halignJustify,
            &QPushButton::pressed,
            this,
            &PageText::setHAlignJustify);

    this->pickColor->connect(this->pickColor,
            &QPushButton::pressed,
            this,
            &PageText::selectColor);

    this->colorPicker->connect(this->colorPicker,
            &QColorDialog::colorSelected,
            this,
            &PageText::setColor);
}

void PageText::updateText(const QString& text) {
    this->treeNode->setText(icu::UnicodeString(text.toUtf8().data()));
}

void PageText::setHAlignLeft() {
    this->treeNode->setHAlign(Text::HALIGN_LEFT);
}

void PageText::setHAlignCenter() {
    this->treeNode->setHAlign(Text::HALIGN_CENTER);
}

void PageText::setHAlignRight() {
    this->treeNode->setHAlign(Text::HALIGN_RIGHT);
}

void PageText::setHAlignJustify() {
    this->treeNode->setHAlign(Text::HALIGN_JUSTIFY);
}

void PageText::selectColor() {
    SignColor currentColor = treeNode->getForegroundColor();
    QColor qtColor(0, 0, 0, 0);
    try {
        SignColor::RGB val = currentColor.getValue();
        qtColor.setRgb(val.r, val.g, val.b, 0);
    } catch(SignColor::NoRGBValueException& e) {

    }
    this->colorPicker->setCurrentColor(qtColor);
    this->colorPicker->show();
}

void PageText::setColor(const QColor& color) {
    this->treeNode->setForegroundColor(
            SignColor(SignColor::ON,
                    color.red(), color.green(), color.blue()));
}

