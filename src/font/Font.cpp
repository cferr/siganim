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

#include <vector>
#include "Font.h"
#include "Character.h"

Font::Font(const std::string& family, const std::string& style) :
    family(family), style(style) {

}

Font::Font(const std::string& family, const std::string& style,
        std::initializer_list<Character*> chars) :
    Font(family, style) {
    for(auto i = chars.begin(); i < chars.end(); ++i)
        this->chars.insert(std::make_pair((*i)->getUTF8Code(), *i));
}

Font::Font(const Font &f) : Font(f.family, f.style) {
    // Deep copy
    for(auto i = f.chars.begin(); i != f.chars.end(); ++i)
        this->chars.insert(std::make_pair((*i).first,
                new Character(*((*i).second))));
}

Font::~Font() {
    for(auto i = this->chars.begin(); i != this->chars.end(); ++i)
        delete (*i).second;
}

std::string Font::getFamily() const {
    return this->family;
}

std::string Font::getStyle() const {
    return this->style;
}

Character* Font::get(const UChar32 index) const {
    auto iterator = this->chars.find(index);
    if(iterator != this->chars.end()) {
        return (*iterator).second;
    } else throw CharNotFoundException(this, index);
}

void Font::addCharacter(Character* character) {
    this->chars.insert(std::make_pair(character->getUTF8Code(), character));
}

bool Font::removeCharacter(UChar32 index) {
    return (this->chars.erase(index) > 0);
}

bool Font::removeCharacter(Character* character) {
    return (this->chars.erase(character->getUTF8Code()) > 0);
}

unsigned int Font::getNbCharacters() {
    return this->chars.size();
}

std::vector<UChar32> Font::listCharCodes() const {
    std::vector<UChar32> ret;
    for(auto i = this->chars.begin(); i != chars.end(); ++i)
        ret.push_back((*i).first);
    return ret;
}

void Font::setFamily(const std::string &family) {
    this->family = family;
}

void Font::setStyle(const std::string &style) {
    this->style = style;
}

json_object* Font::toJSON() const {
    json_object* ret = json_object_new_object();
    json_object_object_add(ret, "family", json_object_new_string(
            family.c_str()));
    json_object_object_add(ret, "style", json_object_new_string(style.c_str()));

    json_object* json_chars = json_object_new_array();
    for(auto i = this->chars.begin(); i != chars.end(); ++i)
        json_object_array_add(json_chars, (*i).second->toJSON());

    json_object_object_add(ret, "chars", json_chars);

    return ret;
}
