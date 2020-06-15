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

FontVisualEditor::FontVisualEditor(FontSet *fontSet,
        const Rasterizer* rasterizer, QWidget *parent) : QWidget(parent),
        image(nullptr), pixelData(nullptr) {
    this->sign = new Sign();
    this->previewTextCell = new Text();
    this->previewDisplay = new Display();
    this->previewDisplay->setRootCell(this->previewTextCell);
    this->sign->addDisplay(this->previewDisplay);

    this->sink = new StaticObservableSink(this->sign, fontSet, rasterizer);
    this->sink->attach(this);

}

FontVisualEditor::~FontVisualEditor() {
    this->sink->detach(this);
    delete this->sink;
    delete this->sign;
}

// TODO implement these functions to modify the current character

void FontVisualEditor::mousePressEvent(QMouseEvent *event) {
    std::cout << "Mouse pressed: " << event->x() << ", " << event->y()
            << std::endl;
}

void FontVisualEditor::mouseMoveEvent(QMouseEvent *event) {
    std::cout << "Mouse moved: " << event->x() << ", " << event->y()
            << std::endl;
}

void FontVisualEditor::mouseReleaseEvent(QMouseEvent *event) {
    std::cout << "Mouse released: " << event->x() << ", " << event->y()
            << std::endl;
}

void FontVisualEditor::setFont(const std::string fontFamily,
        const std::string fontStyle) {
    this->previewTextCell->setFontFamily(fontFamily);
    this->previewTextCell->setFontStyle(fontStyle);
}

void FontVisualEditor::setCharacter(UChar32 index) {
    UChar32 l_index = index;
    this->previewTextCell->setText(
            icu::UnicodeString::fromUTF32(&l_index, 1));
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
