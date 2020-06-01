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

#include "SignCellText.h"

#include <unicode/ustream.h>

SignCellText::SignCellText(const Font* font, const icu::UnicodeString& text) :
    background(SignColor::defaultColor(SignColor::BACKGROUND)),
    foreground(SignColor::defaultColor(SignColor::FOREGROUND)),
    font(font) {
    this->text = new icu::UnicodeString(text);
}

SignCellText::~SignCellText() {

}

unsigned int SignCellText::getChildHeight(const SignCell* child) const {
    throw NoSuchChildException(this, child);
}

unsigned int SignCellText::getChildWidth(const SignCell* child) const {
    throw NoSuchChildException(this, child);
}

unsigned int SignCellText::getHeight() const {
    if(this->parent != NULL) {
        return this->parent->getChildHeight(this);
    } else return 0; // Orphan node
}

unsigned int SignCellText::getWidth() const {
    if(this->parent != NULL) {
        return this->parent->getChildWidth(this);
    } else return 0; // Orphan node
}

SignCell::Type SignCellText::getType() const {
    return Type::TEXT;
}

void SignCellText::accept(SignTreeVisitor &visitor) {
    visitor.visit(*this);
}

void SignCellText::setText(const icu::UnicodeString& text) {
    delete this->text;
    this->text = new icu::UnicodeString(text);
}

icu::UnicodeString* SignCellText::getText() const {
    return this->text->clone();
}

const Font* SignCellText::getFont() const {
    return this->font;
}

bool SignCellText::setParent(const SignCell *parent) {
    this->parent = parent;
    return true;
}

const SignColor SignCellText::getForegroundColor() const {
    return this->foreground;
}

const SignColor SignCellText::getBackgroundColor() const {
    return this->background;
}

std::ostream& SignCellText::serialize(std::ostream &strm) const {
    return strm << "{ " << this->getWidth() << "x" << this->getHeight() <<
            " : \"" << *(this->text) << "\" }";
}

std::ostream& operator<<(std::ostream &strm, const SignCellText &s) {
    return s.serialize(strm);
}
