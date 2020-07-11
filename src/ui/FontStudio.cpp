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
        rasterizerSet(rasterizerSet), currentCharacter(nullptr) {
    this->mainLayout = new QGridLayout(this);
    this->welcome = new QLabel("Welcome to Siganim Font Studio.", this);
    this->rasterizerCombo = new QComboBox(this);

    // Populate rasterizers
    std::vector<Rasterizer*> rasterizers =
            this->rasterizerSet->getRasterizers();
    for(auto i = rasterizers.begin(); i < rasterizers.end(); ++i) {
        this->rasterizerCombo->addItem(QString::fromStdString((*i)->getName()),
                QString::fromStdString((*i)->getName()));
    }

    this->editorsRasterizer = new Rasterizer("Default", 10);
    this->visualEditor = new FontVisualEditor(this->editorsRasterizer,
            this);

    this->characterList = new CharacterListWidget(this);

    QHBoxLayout* familyStyleLayout = new QHBoxLayout();
    this->fontFamilyCombo = new QComboBox(this);
    this->fontFamilyCombo->setEditable(true);
    this->fontStyleCombo = new QComboBox(this);
    this->fontStyleCombo->setEditable(true);
    familyStyleLayout->addWidget(this->fontFamilyCombo);
    familyStyleLayout->addWidget(this->fontStyleCombo);

    QGridLayout* editorLayout = new QGridLayout();
    this->heightSpinner = new QSpinBox(this);
    this->widthSpinner = new QSpinBox(this);
    editorLayout->addWidget(this->visualEditor, 0, 0, 1, 2, Qt::AlignCenter);
    editorLayout->addWidget(this->widthSpinner, 1, 0);
    editorLayout->addWidget(this->heightSpinner, 2, 0);

    this->mainLayout->addWidget(this->welcome, 0, 1);
    this->mainLayout->addWidget(this->rasterizerCombo, 1, 1);
    this->mainLayout->addLayout(familyStyleLayout, 2, 1);
    this->mainLayout->addWidget(this->characterList, 3, 1);
    this->mainLayout->addLayout(editorLayout, 0, 0, 4, 1,
            Qt::AlignCenter);
    this->mainLayout->setColumnMinimumWidth(0, 200);
    this->setLayout(this->mainLayout);

    std::vector<Font*> fonts = this->fontSet->getFonts();
    for(auto i = fonts.begin(); i < fonts.end(); ++i) {
        QString family = QString::fromStdString((*i)->getFamily());
        this->fontFamilyCombo->addItem(family, family);
    }
    // Select the first available family.
    this->currentFont = *(fonts.begin());
    this->fontFamilyCombo->setCurrentIndex(0);
//    this->fontStyleCombo->setCurrentIndex(0);
    this->populateFontStyles(this->currentFont->getFamily());
    this->visualEditor->setFont(this->currentFont);

    // TODO add default rasterizer in SiganimDefaults...
    if(rasterizers.empty()) {
        this->modelRasterizer = new Rasterizer("Default", 5);
    } else {
        this->modelRasterizer = *(rasterizers.begin());
    }
    this->model = new FontQtModel(this->currentFont,
            this->modelRasterizer, Display::DISPLAY_MONOCHROME_LED);
    // this->model->setFont(this->currentFont);
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
    connect(this->heightSpinner,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(setCharacterHeight(int)));
    connect(this->widthSpinner,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(setCharacterWidth(int)));
}

FontStudio::~FontStudio() {
    // Delete all our objects
    delete this->mainLayout;
    delete this->welcome;
    delete this->rasterizerCombo;
    delete this->visualEditor;
    delete this->fontFamilyCombo;
    delete this->fontStyleCombo;
    delete this->heightSpinner;
    delete this->widthSpinner;
    delete this->characterList;
    delete this->model;

    // Temporary.
    delete this->editorsRasterizer;
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

void FontStudio::setFontFamily(const std::string fontFamily) {
    this->populateFontStyles(fontFamily);
}

void FontStudio::setFontStyle(const std::string fontStyle) {
    std::string fontFamily = this->fontFamilyCombo->currentText().toStdString();
     try {
       this->currentFont = this->fontSet->lookup(fontFamily, fontStyle);
       this->model->setFont(this->currentFont);
       this->visualEditor->setFont(this->currentFont);
    } catch(FontSet::FontNotFoundException& e) {

    }
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
        this->modelRasterizer = r;
    } catch(RasterizerSet::RasterizerNotFoundException& e) {

    }
}

void FontStudio::setEditedCharacter(Character* c) {
    this->currentCharacter = c;
    this->visualEditor->setCharacter(c);
    this->heightSpinner->setValue(c->getHeight());
    this->widthSpinner->setValue(c->getWidth());
}

void FontStudio::setCharacterHeight(int height) {
    if(this->currentCharacter != nullptr) {
        this->currentCharacter->setHeight((unsigned int)height);
        this->visualEditor->refresh();
    }
}

void FontStudio::setCharacterWidth(int width) {
    if(this->currentCharacter != nullptr) {
        this->currentCharacter->setWidth((unsigned int)width);
        this->visualEditor->refresh();
    }
}
