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
        const Rasterizer* rasterizer, QWidget *parent) :
    SignWidget(new Sign(), fontSet, rasterizer, parent) {
    this->previewTextCell = new Text();
    this->previewDisplay = new Display();
    this->previewDisplay->setRootCell(this->previewTextCell);
    this->sign->addDisplay(this->previewDisplay);
}

FontVisualEditor::~FontVisualEditor() {


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
