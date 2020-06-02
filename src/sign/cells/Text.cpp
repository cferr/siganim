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

#include <unicode/ustream.h>

#include "Text.h"

Text::Text(const Font* font,
        const enum HorizontalAlignment hAlign,
        const enum VerticalAlignment vAlign,
        const icu::UnicodeString& text) :
    background(SignColor::defaultColor(SignColor::BACKGROUND)),
    foreground(SignColor::defaultColor(SignColor::FOREGROUND)),
    font(font),
    hAlign(hAlign),
    vAlign(vAlign) {
    this->text = new icu::UnicodeString(text);
}

Text::~Text() {

}

unsigned int Text::getChildHeight(const SignCell* child) const {
    throw NoSuchChildException(this, child);
}

unsigned int Text::getChildWidth(const SignCell* child) const {
    throw NoSuchChildException(this, child);
}

unsigned int Text::getHeight() const {
    if(this->parent != NULL) {
        return this->parent->getChildHeight(this);
    } else return 0; // Orphan node
}

unsigned int Text::getWidth() const {
    if(this->parent != NULL) {
        return this->parent->getChildWidth(this);
    } else return 0; // Orphan node
}

void Text::accept(SignTreeVisitor &visitor) {
    visitor.visit(*this);
}

void Text::accept(ConstSignTreeVisitor &visitor) const {
    visitor.visit(*this);
}

void Text::setText(const icu::UnicodeString& text) {
    delete this->text;
    this->text = new icu::UnicodeString(text);
}

icu::UnicodeString* Text::getText() const {
    return this->text->clone();
}

const Font* Text::getFont() const {
    return this->font;
}

bool Text::setParent(const SignCell *parent) {
    this->parent = parent;
    return true;
}

const SignColor Text::getForegroundColor() const {
    return this->foreground;
}

const SignColor Text::getBackgroundColor() const {
    return this->background;
}

enum Text::HorizontalAlignment Text::getHAlign() const {
    return this->hAlign;
}

enum Text::VerticalAlignment Text::getVAlign() const {
    return this->vAlign;
}

void Text::setHAlign(enum HorizontalAlignment hAlign) {
    this->hAlign = hAlign;
}

void Text::setVAlign(enum VerticalAlignment vAlign) {
    this->vAlign = vAlign;
}

std::ostream& Text::serialize(std::ostream &strm) const {
    return strm << "{ " << this->getWidth() << "x" << this->getHeight() <<
            " : \"" << *(this->text) << "\" }";
}

std::ostream& operator<<(std::ostream &strm, const Text &s) {
    return s.serialize(strm);
}

void Text::setBackgroundColor(const SignColor &background) {
    SignColor c(background);
    this->background = c;
}

void Text::setForegroundColor(const SignColor &foreground) {
    SignColor c(foreground);
    this->foreground = c;
}
