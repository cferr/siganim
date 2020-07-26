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

Text::Text() : Text("", "", HorizontalAlignment::HALIGN_CENTER,
        VerticalAlignment::VALIGN_CENTER_BOTTOM, ""){
}

Text::Text(const std::string fontFamily, const std::string fontStyle,
        const enum HorizontalAlignment hAlign,
        const enum VerticalAlignment vAlign,
        const icu::UnicodeString& text) :
        Text(fontFamily, fontStyle, hAlign, vAlign, text, SignColor::on()) {
}


Text::Text(const std::string fontFamily, const std::string fontStyle,
        const enum HorizontalAlignment hAlign,
        const enum VerticalAlignment vAlign, const icu::UnicodeString &text,
        const SignColor &color) :
        foreground(color),
            fontFamily(fontFamily),
            fontStyle(fontStyle),
            hAlign(hAlign),
            vAlign(vAlign) {
    this->text = new icu::UnicodeString(text);
}

Text::Text(const Text *a) : foreground(a->foreground),
        fontFamily(a->fontFamily), fontStyle(a->fontStyle),
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

std::string Text::getFontFamily() const {
    return this->fontFamily;
}

std::string Text::getFontStyle() const {
    return this->fontStyle;
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

void Text::deleteChild(SignTree *child) {
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

void Text::deepDetachStructureObserver(SignTreeStructureObserver *observer) {
    this->detachStructureObserver(observer);
}

void Text::setFontFamily(const std::string &fontFamily) {
    this->fontFamily = std::string(fontFamily);
    this->modified();
}

void Text::setFontStyle(const std::string &fontStyle) {
    this->fontStyle = std::string(fontStyle);
    this->modified();
}

json_object* Text::toJSON() const {
    json_object* ret = json_object_new_object();
    json_object_object_add(ret, "type", json_object_new_string("Text"));

    json_object_object_add(ret, "color", this->foreground.toJSON());

    std::string textStr;
    textStr = this->text->toUTF8String(textStr);
    json_object_object_add(ret, "text",
            json_object_new_string(textStr.c_str()));

    json_object_object_add(ret, "fontFamily",
            json_object_new_string(this->fontFamily.c_str()));
    json_object_object_add(ret, "fontStyle",
            json_object_new_string(this->fontStyle.c_str()));

    std::string hAlignStr;
    switch(this->hAlign) {
    case Text::HorizontalAlignment::HALIGN_LEFT:
        hAlignStr = "left";
        break;
    case Text::HorizontalAlignment::HALIGN_CENTER:
        hAlignStr = "center";
        break;
    case Text::HorizontalAlignment::HALIGN_RIGHT:
        hAlignStr = "right";
        break;
    case Text::HorizontalAlignment::HALIGN_JUSTIFY:
        hAlignStr = "justify";
        break;
    }
    json_object_object_add(ret, "hAlign",
            json_object_new_string(hAlignStr.c_str()));

    std::string vAlignStr;
    switch(this->vAlign) {
    case Text::VerticalAlignment::VALIGN_TOP_TOP:
        vAlignStr = "top_top";
        break;
    case Text::VerticalAlignment::VALIGN_TOP_CENTER:
        vAlignStr = "top_center";
        break;
    case Text::VerticalAlignment::VALIGN_TOP_BOTTOM:
        vAlignStr = "top_bottom";
        break;
    case Text::VerticalAlignment::VALIGN_CENTER_TOP:
        vAlignStr = "center_top";
        break;
    case Text::VerticalAlignment::VALIGN_CENTER_CENTER:
        vAlignStr = "center_center";
        break;
    case Text::VerticalAlignment::VALIGN_CENTER_BOTTOM:
        vAlignStr = "center_bottom";
        break;
    case Text::VerticalAlignment::VALIGN_BOTTOM_TOP:
        vAlignStr = "bottom_top";
        break;
    case Text::VerticalAlignment::VALIGN_BOTTOM_CENTER:
        vAlignStr = "bottom_center";
        break;
    case Text::VerticalAlignment::VALIGN_BOTTOM_BOTTOM:
        vAlignStr = "bottom_bottom";
        break;
    }
    json_object_object_add(ret, "vAlign",
            json_object_new_string(vAlignStr.c_str()));

    return ret;
}
