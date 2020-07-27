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

#include "FontVisualEditor.h"

FontVisualEditor::FontVisualEditor(const Rasterizer* rasterizer,
        QWidget *parent) : QWidget(parent), currentBit(Character::Bit::OFF),
        image(nullptr), pixelData(nullptr) {
    this->sign = new Sign();
    this->previewTextCell = new Text();
    this->previewDisplay = new Display();
    this->previewDisplay->setRootCell(this->previewTextCell);
    this->sign->addDisplay(this->previewDisplay);

    this->fontSet = new FontSet();
    this->font = nullptr;
    this->currentCharacter = nullptr;
    this->sink = new StaticObservableSink(this->sign, this->fontSet,
            rasterizer, 2);
    this->sink->attach(this);
}

FontVisualEditor::~FontVisualEditor() {
    this->sink->detach(this);
    delete this->sink;
    delete this->sign;
    if(this->pixelData != nullptr)
        free(this->pixelData);
    if(this->image != nullptr)
        delete this->image;
    this->fontSet->clear();
    delete this->fontSet;
}

// TODO implement these functions to modify the current character

void FontVisualEditor::mousePressEvent(QMouseEvent *event) {
    unsigned x = event->x() / 10;
    unsigned y = event->y() / 10;

    enum Character::Bit b = this->currentCharacter->getBit(x, y);
    switch(b) {
    case Character::Bit::OFF:
        this->currentBit = Character::Bit::ON;
        break;
    case Character::Bit::ON:
        this->currentBit = Character::Bit::OFF;
        break;
    }
    this->currentCharacter->toggleBit(x, y);
    this->refresh();
}

void FontVisualEditor::mouseMoveEvent(QMouseEvent *event) {
    unsigned x = event->x() / 10;
    unsigned y = event->y() / 10;
    this->currentCharacter->setBit(x, y, this->currentBit);
    this->refresh();
}

void FontVisualEditor::mouseReleaseEvent(QMouseEvent *event) {
}

void FontVisualEditor::setFont(Font* font) {
    this->fontSet->clear();
    this->fontSet->addFont(font);
    this->font = font;
    std::string fontFamily = font->getFamily();
    std::string fontStyle = font->getStyle();
    this->previewTextCell->setFontFamily(fontFamily);
    this->previewTextCell->setFontStyle(fontStyle);
    this->setCharacter(this->currentCharacter);
}

void FontVisualEditor::setCharacter(Character* c) {
    if(c != nullptr) {
        this->currentCharacter = c;
        UChar32 l_index = c->getUTF8Code();
        this->previewTextCell->setText(
                icu::UnicodeString::fromUTF32(&l_index, 1));
        unsigned int width = c->getWidth();
        unsigned int height = c->getHeight();
        this->previewDisplay->setWidth(width);
        this->previewDisplay->setHeight(height);
    }
}

void FontVisualEditor::paintEvent(QPaintEvent *event) {
    if(this->image != nullptr) {
        QPainter painter(this);
        painter.drawImage(event->rect(),
                *this->image, this->image->rect());
    }
}

void FontVisualEditor::signChangedEvent() {
    Bitmap* result = this->sink->getFrame();
    if(this->pixelData != nullptr)
        free(this->pixelData);
    this->pixelData = result->toRGB32();
    if(this->image != nullptr)
        delete this->image;
    this->image = new QImage(this->pixelData, result->getWidth(),
            result->getHeight(), QImage::Format_RGB32);
    this->setMinimumHeight(result->getHeight());
    this->setMinimumWidth(result->getWidth());
    this->setMaximumHeight(result->getHeight());
    this->setMaximumWidth(result->getWidth());
    this->update();
}

void FontVisualEditor::observe(const Observable *sender) {
    if(sender == this->sink) {
        this->signChangedEvent();
    }
}

void FontVisualEditor::setRasterizer(const Rasterizer *rasterizer) {
    this->sink->setRasterizer(rasterizer);
}

void FontVisualEditor::refresh() {
    // TODO this feels a bit hack-ish.
    // Fonts should be observed and trigger this by
    // themselves without UI assistance.
    this->sign->modified();
    unsigned int width = this->currentCharacter->getWidth();
    unsigned int height = this->currentCharacter->getHeight();
    this->previewDisplay->setWidth(width);
    this->previewDisplay->setHeight(height);
}
