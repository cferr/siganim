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

#include <iostream>
#include <unicode/uchar.h>
#include <QString>
#include <QVBoxLayout>
#include "FontStudio.h"
#include "../font/Character.h"
#include "../font/UnicodeUtils.h"

FontStudio::FontStudio(FontSet* fontSet, const RasterizerSet* rasterizerSet,
        QWidget* parent) : QWidget(parent), fontSet(fontSet),
        rasterizerSet(rasterizerSet) {
    this->mainLayout = new QHBoxLayout(this);
    this->welcome = new QLabel("Welcome to Siganim Font Studio.", this);
    this->rasterizerCombo = new QComboBox(this);

    // Populate rasterizers
    std::vector<Rasterizer*> rasterizers =
            this->rasterizerSet->getRasterizers();
    for(auto i = rasterizers.begin(); i < rasterizers.end(); ++i) {
        this->rasterizerCombo->addItem(QString::fromStdString((*i)->getName()),
                QString::fromStdString((*i)->getName()));
    }

    // TODO add default rasterizer in SiganimDefaults...
    if(rasterizers.empty()) {
        this->currentRasterizer = new Rasterizer("Default");
    } else {
        this->currentRasterizer = *(rasterizers.begin());
    }

    this->visualEditor = new FontVisualEditor(fontSet,
            this->currentRasterizer, this);

    this->characterList = new CharacterListWidget(this);

    QVBoxLayout* rightPanelLayout = new QVBoxLayout();
    QHBoxLayout* combosLayout = new QHBoxLayout();
    this->fontFamilyCombo = new QComboBox(this);
    this->fontFamilyCombo->setEditable(true);
    this->fontStyleCombo = new QComboBox(this);
    this->fontStyleCombo->setEditable(true);

    combosLayout->addWidget(this->fontFamilyCombo);
    combosLayout->addWidget(this->fontStyleCombo);
    rightPanelLayout->addWidget(this->welcome);
    rightPanelLayout->addWidget(this->rasterizerCombo);
    rightPanelLayout->addLayout(combosLayout);
    rightPanelLayout->addWidget(this->characterList);
    this->mainLayout->addWidget(this->visualEditor, 1);
    this->mainLayout->addLayout(rightPanelLayout, 1);
    this->setLayout(this->mainLayout);

    std::vector<Font*> fonts = this->fontSet->getFonts();
    for(auto i = fonts.begin(); i < fonts.end(); ++i) {
        QString family = QString::fromStdString((*i)->getFamily());
        this->fontFamilyCombo->addItem(family, family);
    }
    // Select the first available family.
    this->currentFont = *(fonts.begin());
    this->fontFamilyCombo->setCurrentText(
                QString::fromStdString(this->currentFont->getFamily()));
        this->fontStyleCombo->setCurrentText(
                QString::fromStdString(this->currentFont->getStyle()));

    this->model = new FontQtModel(this->currentFont,
            this->currentRasterizer, Display::DISPLAY_MONOCHROME_LED);
    this->characterList->setModel(this->model);
    this->characterList->setUniformItemSizes(true);
    this->characterList->setItemAlignment(Qt::AlignCenter);
    this->characterList->setViewMode(QListView::IconMode);
    this->characterList->setResizeMode(QListView::ResizeMode::Adjust);

    connect(this->fontFamilyCombo,
            &QComboBox::currentTextChanged,
            this,
            &FontStudio::fontFamilyComboChanged);
    connect(this->fontStyleCombo,
                &QComboBox::currentTextChanged,
                this,
                &FontStudio::fontStyleComboChanged);
    connect(this->rasterizerCombo,
            &QComboBox::currentTextChanged,
            this,
            &FontStudio::setRasterizer);
    connect(this->characterList,
            &CharacterListWidget::characterSelected,
            this,
            &FontStudio::setEditedCharacter);

}

void FontStudio::populateFontStyles(const std::string &fontFamily) {
    this->fontStyleCombo->clear();

    std::vector<Font*> fonts =
            this->fontSet->multiLookup(fontFamily);
    for(auto i = fonts.begin(); i < fonts.end(); ++i) {
        QString style = QString::fromStdString((*i)->getStyle());
        this->fontStyleCombo->addItem(style, style);
    }
}

void FontStudio::populateCharacters(const std::string& fontFamily,
        const std::string& fontStyle) {
    try {
       Font* font = this->fontSet->lookup(fontFamily, fontStyle);
       this->model->setFont(font);
    } catch(FontSet::FontNotFoundException& e) {

    }
}

void FontStudio::setFontFamily(const std::string fontFamily) {
    this->populateFontStyles(fontFamily);
}

void FontStudio::setFontStyle(const std::string fontStyle) {
    this->populateCharacters(this->fontFamilyCombo->currentText()
            .toStdString(), fontStyle);
    this->visualEditor->setFont(this->fontFamilyCombo->currentText()
            .toStdString(), fontStyle);
}

void FontStudio::fontFamilyComboChanged(const QString &fontFamily) {
    this->setFontFamily(fontFamily.toStdString());
}

void FontStudio::fontStyleComboChanged(const QString &fontStyle) {
    this->setFontStyle(fontStyle.toStdString());
}

void FontStudio::setRasterizer(const QString &rasterizer) {
    try {
        const Rasterizer* r = this->rasterizerSet->getConst(
                rasterizer.toStdString());
        this->visualEditor->setRasterizer(r);
        this->currentRasterizer = r;
    } catch(RasterizerSet::RasterizerNotFoundException& e) {

    }
}

void FontStudio::setEditedCharacter(Character* c) {
    this->visualEditor->setCharacter(c->getUTF8Code());
}
