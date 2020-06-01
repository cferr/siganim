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

#include "Font.h"

Font::Font(const std::string& name) : name(name) {

}

Font::Font(const std::string& name, std::initializer_list<Character*> chars) :
    Font(name) {
    for(auto i = chars.begin(); i < chars.end(); ++i)
        this->chars.insert(std::make_pair((*i)->getUTF8Code(), *i));
}

Font::~Font() {

}

std::string Font::getName() const {
    return this->name;
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
