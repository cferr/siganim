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
        Text(font, hAlign, vAlign, text, SignColor::on()) {
}


Text::Text(const Font *font, const enum HorizontalAlignment hAlign,
        const enum VerticalAlignment vAlign, const icu::UnicodeString &text,
        const SignColor &color) :
        foreground(color),
            font(font),
            hAlign(hAlign),
            vAlign(vAlign) {
    this->text = new icu::UnicodeString(text);
}

Text::Text(const Text *a) : foreground(a->foreground), font(a->font),
        hAlign(a->hAlign), vAlign(a->vAlign) {
    this->text = new icu::UnicodeString(*(a->text));
}

SignCell* Text::copy() {
    return new Text(this);
}

Text::~Text() {
    delete this->text;
}

unsigned int Text::getChildHeight(const SignCell* child) const {
    throw NoSuchChildException(this, child);
}

unsigned int Text::getChildWidth(const SignCell* child) const {
    throw NoSuchChildException(this, child);
}

unsigned int Text::getHeight() const {
    return this->getParent()->getChildHeight(this);
}

unsigned int Text::getWidth() const {
    return this->getParent()->getChildWidth(this);
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
    this->modified();
}

icu::UnicodeString* Text::getText() const {
    return this->text->clone();
}

const Font* Text::getFont() const {
    return this->font;
}

const SignColor Text::getForegroundColor() const {
    return this->foreground;
}

enum Text::HorizontalAlignment Text::getHAlign() const {
    return this->hAlign;
}

enum Text::VerticalAlignment Text::getVAlign() const {
    return this->vAlign;
}

void Text::setHAlign(enum HorizontalAlignment hAlign) {
    this->hAlign = hAlign;
    this->modified();
}

void Text::setVAlign(enum VerticalAlignment vAlign) {
    this->vAlign = vAlign;
    this->modified();
}

void Text::callbackDispatch(ConstSignTreeDispatcher *s) const {
    s->dispatchCallback(*this);
}

void Text::callbackDispatch(SignTreeDispatcher *s) {
    s->dispatchCallback(*this);
}

std::ostream& Text::serialize(std::ostream &strm) const {
    return strm << "{ " << this->getWidth() << "x" << this->getHeight() <<
            " : \"" << *(this->text) << "\" }";
}

std::ostream& operator<<(std::ostream &strm, const Text &s) {
    return s.serialize(strm);
}

void Text::setForegroundColor(const SignColor &foreground) {
    SignColor c(foreground);
    this->foreground = c;
    this->modified();
}
