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
#include <QHBoxLayout>
#include "PageText.h"
#include "../../sign/SignColor.h"

PageText::PageText(Text* treeNode, const FontSet* fontSet) :
    treeNode(treeNode), fontSet(fontSet) {
    this->textLine = new QLineEdit(this);
    icu::UnicodeString* text = treeNode->getText();
    this->textLine->setText(
            QString::fromUtf16(text->getBuffer(), text->length()));
    delete text;
    this->halignLeft = new QPushButton("Left", this);
    this->halignCenter = new QPushButton("Center", this);
    this->halignRight = new QPushButton("Right", this);
    this->halignJustify = new QPushButton("Justify", this);
    this->pickColor = new QPushButton("Color", this);
    this->colorPicker = new QColorDialog(this);
    this->fontFamilyCombo = new QComboBox(this);
    this->fontFamilyCombo->setEditable(true);
    this->fontStyleCombo = new QComboBox(this);
    this->fontStyleCombo->setEditable(true);
    this->valignCombo = new QComboBox(this);
    this->valignCombo->setEditable(false);

    // Populate font list
    std::vector<Font*> fonts = this->fontSet->getFonts();
    for(auto i = fonts.begin(); i < fonts.end(); ++i) {
        QString family = QString::fromStdString((*i)->getFamily());
        this->fontFamilyCombo->addItem(family, family);
    }

    this->fontFamilyCombo->setCurrentText(
            QString::fromStdString(this->treeNode->getFontFamily()));
    this->populateFontStyles(this->treeNode->getFontFamily());

    // Important: keep the same order as the enumerable.
    this->valignCombo->addItem("Biggest letter aligned top, others top",
            Text::VALIGN_TOP_TOP);
    this->valignCombo->addItem("Biggest letter aligned top, others center",
            Text::VALIGN_TOP_CENTER);
    this->valignCombo->addItem("Biggest letter aligned top, others bottom",
            Text::VALIGN_TOP_BOTTOM);
    this->valignCombo->addItem("Biggest letter aligned center, others top",
            Text::VALIGN_CENTER_TOP);
    this->valignCombo->addItem("Biggest letter aligned center, others center",
            Text::VALIGN_CENTER_CENTER);
    this->valignCombo->addItem("Biggest letter aligned center, others bottom",
            Text::VALIGN_CENTER_BOTTOM);
    this->valignCombo->addItem("Biggest letter aligned bottom, others top",
            Text::VALIGN_BOTTOM_TOP);
    this->valignCombo->addItem("Biggest letter aligned bottom, others center",
            Text::VALIGN_BOTTOM_CENTER);
    this->valignCombo->addItem("Biggest letter aligned bottom, others bottom",
            Text::VALIGN_BOTTOM_BOTTOM);

    this->valignCombo->setCurrentIndex(this->treeNode->getVAlign());

    QHBoxLayout* alignBox = new QHBoxLayout();
    alignBox->addWidget(this->halignLeft);
    alignBox->addWidget(this->halignCenter);
    alignBox->addWidget(this->halignRight);
    alignBox->addWidget(this->halignJustify);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(this->textLine);
    layout->addWidget(this->fontFamilyCombo);
    layout->addWidget(this->fontStyleCombo);
    layout->addWidget(this->valignCombo);
    layout->addLayout(alignBox);
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

    this->fontFamilyCombo->connect(this->fontFamilyCombo,
            &QComboBox::currentTextChanged,
            this,
            &PageText::setFontFamily);

    this->fontFamilyCombo->connect(this->fontStyleCombo,
            &QComboBox::currentTextChanged,
            this,
            &PageText::setFontStyle);

    this->valignCombo->connect(this->valignCombo,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(setVAlign(int)));

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
        SignColor::RGBA val = currentColor.getValue();
        qtColor.setRgb(val.r, val.g, val.b, 0);
    } catch(SignColor::NoRGBValueException& e) {

    }
    this->colorPicker->setCurrentColor(qtColor);
    this->colorPicker->show();
}

void PageText::setFontFamily(const QString &fontFamily) {
    this->treeNode->setFontFamily(fontFamily.toStdString());
    this->populateFontStyles(fontFamily.toStdString());
}

void PageText::populateFontStyles(const std::string &fontFamily) {
    this->fontStyleCombo->clear();

    std::vector<Font*> fonts =
            this->fontSet->multiLookup(fontFamily);
    for(auto i = fonts.begin(); i < fonts.end(); ++i) {
        QString style = QString::fromStdString((*i)->getStyle());
        this->fontStyleCombo->addItem(style, style);
    }
}

void PageText::setFontStyle(const QString &fontStyle) {
    this->treeNode->setFontStyle(fontStyle.toStdString());
}

void PageText::setColor(const QColor& color) {
    this->treeNode->setForegroundColor(
            SignColor(SignColor::ON,
                    color.red(), color.green(), color.blue()));
}

void PageText::setVAlign(int align) {
    // This is where the order matters.
    this->treeNode->setVAlign((enum Text::VerticalAlignment)align);
}

