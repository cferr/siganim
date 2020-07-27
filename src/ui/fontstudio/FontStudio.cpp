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
#include <QInputDialog>
#include <QString>
#include <QVBoxLayout>
#include "FontStudio.h"
#include "../../font/Character.h"
#include "../../font/UnicodeUtils.h"

FontStudio::FontStudio(SiganimUICore* uiCore, QWidget* parent) :
    QWidget(parent), uiCore(uiCore), currentCharacter(nullptr) {
    this->rasterizerSet = uiCore->getCore()->getRasterizerSet();
    assert(!this->rasterizerSet->getRasterizers().empty());

    this->fontSet = uiCore->getCore()->getFontSet();

    this->mainLayout = new QGridLayout(this);
    this->welcome = new QLabel("Welcome to Siganim Font Studio.", this);
    this->rasterizerCombo = new QComboBox(this);

    this->editorsRasterizer = (const Rasterizer*)
        *(this->rasterizerSet->getRasterizers().begin());
    this->visualEditor = new FontVisualEditor(this->editorsRasterizer,
            this);

    this->characterList = new CharacterListWidget(this);

    QHBoxLayout* familyStyleLayout = new QHBoxLayout();
    this->fontFamilyCombo = new QComboBox(this);
    this->fontFamilyCombo->setEditable(true);
    this->fontStyleCombo = new QComboBox(this);
    this->fontStyleCombo->setEditable(true);

    this->UnicodeBlockCombo = new QComboBox(this);

    this->renameFontButton = new QPushButton(QString("Rename..."), this);
    this->changeStyleButton = new QPushButton(QString("Change..."), this);

    familyStyleLayout->addWidget(this->fontFamilyCombo);
    familyStyleLayout->addWidget(this->renameFontButton);
    familyStyleLayout->addWidget(this->fontStyleCombo);
    familyStyleLayout->addWidget(this->changeStyleButton);

    QGridLayout* editorLayout = new QGridLayout();
    this->heightSpinner = new QSpinBox(this);
    this->widthSpinner = new QSpinBox(this);
    this->noSuchChar = new NoSuchCharWidget(this);
    editorLayout->addWidget(this->noSuchChar, 0, 0, 1, 2, Qt::AlignCenter);
    editorLayout->addWidget(this->visualEditor, 0, 0, 1, 2, Qt::AlignCenter);
    editorLayout->addWidget(this->widthSpinner, 1, 0);
    editorLayout->addWidget(this->heightSpinner, 2, 0);

    this->noSuchChar->setVisible(false);

    this->commitButton = new QPushButton("Commit changes", this);

    this->mainLayout->addWidget(this->welcome, 0, 1);
    this->mainLayout->addWidget(this->rasterizerCombo, 1, 1);
    this->mainLayout->addLayout(familyStyleLayout, 2, 1);
    this->mainLayout->addWidget(this->UnicodeBlockCombo, 3, 1);
    this->mainLayout->addWidget(this->characterList, 4, 1);
    this->mainLayout->addLayout(editorLayout, 0, 0, 5, 1,
            Qt::AlignCenter);
    this->mainLayout->addWidget(this->commitButton, 5, 0, 1, 2,
                Qt::AlignCenter);
    this->mainLayout->setColumnMinimumWidth(0, 200);
    this->setLayout(this->mainLayout);

    this->modelRasterizer = (const Rasterizer*)
        *(this->rasterizerSet->getRasterizers().begin());
    this->currentFont = nullptr;

    // FIXME what to do if there are no fonts? Disable UI?
    this->model = new FontQtModel(nullptr,
            this->modelRasterizer, Display::DISPLAY_MONOCHROME_LED);

    this->characterList->setModel(this->model);
//    this->characterList->setUniformItemSizes(true);
//    this->characterList->setItemAlignment(Qt::AlignCenter);
//    this->characterList->setViewMode(QListView::IconMode);
    this->characterList->setResizeMode(QListView::ResizeMode::Adjust);
    this->characterList->setWordWrap(true);

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
    connect(this->characterList,
            &CharacterListWidget::unavailableCharacterSelected,
            this,
            &FontStudio::unavailableCharacterPrompt);
    connect(this->heightSpinner,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(setCharacterHeight(int)));
    connect(this->widthSpinner,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(setCharacterWidth(int)));
    connect(this->renameFontButton,
            &QPushButton::pressed,
            this,
            &FontStudio::promptRenameFont);
    connect(this->changeStyleButton,
            &QPushButton::pressed,
            this,
            &FontStudio::promptChangeStyle);
    connect(this->UnicodeBlockCombo,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(unicodeBlockChanged(int)));
    connect(this->commitButton,
            &QPushButton::clicked,
            this,
            &FontStudio::commitFontChanges);
    connect(this->noSuchChar,
            &NoSuchCharWidget::createCharacter,
            this,
            &FontStudio::createCharacterInFont);

    this->populateFontFamilies();
    this->populateUnicodeBlocks();
    this->populateRasterizers();
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
}

void FontStudio::populateFontFamilies() {
    this->fontFamilyCombo->clear();

    std::vector<Font*> fonts = this->fontSet->getFonts();
    for(auto i = fonts.begin(); i < fonts.end(); ++i) {
        QString family = QString::fromStdString((*i)->getFamily());
        this->fontFamilyCombo->addItem(family, family);
    }
}

void FontStudio::populateRasterizers() {
    this->rasterizerCombo->clear();
    // Populate rasterizers
    std::vector<Rasterizer*> rasterizers =
            this->rasterizerSet->getRasterizers();
    for(auto i = rasterizers.begin(); i < rasterizers.end(); ++i) {
        this->rasterizerCombo->addItem(QString::fromStdString((*i)->getName()),
                QString::fromStdString((*i)->getName()));
    }
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

    this->visualEditor->setVisible(true);
    this->widthSpinner->setVisible(true);
    this->heightSpinner->setVisible(true);
    this->noSuchChar->setVisible(false);
}

void FontStudio::unavailableCharacterPrompt(UChar32 code) {
    this->visualEditor->setVisible(false);
    this->widthSpinner->setVisible(false);
    this->heightSpinner->setVisible(false);
    this->noSuchChar->setVisible(true);
    this->currentCharacterCode = code;
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

void FontStudio::promptRenameFont() {
    bool validated = false;
    QString newName = QInputDialog::getText(this,
            QString("Enter new font name"), QString("Enter new font name"),
            QLineEdit::EchoMode::Normal,
            QString::fromStdString(this->currentFont->getFamily()),
            &validated);
    if(validated && !newName.isEmpty()) {
        this->currentFont->setFamily(newName.toStdString());
        this->populateFontFamilies();
        this->fontFamilyCombo->setCurrentText(newName);
    }
}

void FontStudio::promptChangeStyle() {
    bool validated = false;
    QString newName = QInputDialog::getText(this,
            QString("Enter new font style"), QString("Enter new font style"),
            QLineEdit::EchoMode::Normal,
            QString::fromStdString(this->currentFont->getStyle()),
            &validated);
    if(validated && !newName.isEmpty()) {
        this->currentFont->setStyle(newName.toStdString());
        this->populateFontStyles(this->currentFont->getFamily());
        this->fontStyleCombo->setCurrentText(newName);
    }
}

void FontStudio::populateUnicodeBlocks() {
    this->UnicodeBlockCombo->clear();
    this->UnicodeBlockCombo->addItem(QString("Characters in this font"),
                    0u);
    for(unsigned int block = 1; block < NB_UNICODE_BLOCKS; ++block) {
        this->UnicodeBlockCombo->addItem(QString::fromStdString(
                UnicodeBlockName(block)), block);
    }

}

void FontStudio::unicodeBlockChanged(const int block) {
    this->model->setBlockNumber(block);
}


void FontStudio::commitFontChanges() {
    this->uiCore->getCore()->saveDatabase();
}

void FontStudio::createCharacterInFont() {
    if(this->currentFont != nullptr) {
        Character* c = new Character(this->currentCharacterCode,
                10, 10); // TODO refrain from giving such a default value
        this->currentFont->addCharacter(c);
        this->setEditedCharacter(c);
    }
}
