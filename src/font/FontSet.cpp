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

#include "FontSet.h"

FontSet::FontSet() {
}

FontSet::FontSet(std::initializer_list<Font*> fonts) {
    for(auto i = fonts.begin(); i < fonts.end(); ++i)
        this->fonts.push_back(*i);
}

FontSet::~FontSet() {
}

void FontSet::addFont(Font *font) {
    this->fonts.push_back(font);
}

void FontSet::addFonts(std::vector<Font*> fonts) {
    for(auto i = fonts.begin(); i < fonts.end(); ++i)
        this->fonts.push_back(*i);
}

std::vector<Font*> FontSet::getFonts() const {
    return this->fonts;
}

bool FontSet::removeFont(std::string family, std::string style) {
    return false;
}

bool FontSet::removeFont(Font *f) {
    return false;
}

std::vector<Font*> FontSet::multiLookup(std::string family,
        std::string style) const {
    std::vector<Font*> ret;

    bool lookupFamily = !(family.empty());
    bool lookupStyle = !(style.empty());
    for(std::vector<Font*>::const_iterator i = this->fonts.begin();
            i < this->fonts.end(); ++i) {
        if( (lookupFamily && (*i)->getFamily() == family)
                || (lookupStyle && (*i)->getStyle() == family)){
            ret.push_back(*i);
        }
    }

    return ret;
}

Font* FontSet::lookup(std::string family, std::string style) const {
    for(std::vector<Font*>::const_iterator i = this->fonts.begin();
                i < this->fonts.end(); ++i) {
        bool lookupFamily = !(family.empty());
        bool lookupStyle = !(style.empty());
        if( (lookupFamily && (*i)->getFamily() == family)
            || (lookupStyle && (*i)->getStyle() == family)) {
            return *i;
        }
    }
    throw FontNotFoundException(this, family, style);
}
