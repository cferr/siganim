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

#include "Character.h"
#include <cstring>

Character::Character(UChar32 UTF8Code, unsigned int width, unsigned int height)
    : UTF8Code(UTF8Code), width(width), height(height), map(NULL) {

    this->map = (enum Bit*)calloc(width * height, sizeof(enum Bit));
}

Character::Character(UChar32 UTF8Code, unsigned int width, unsigned int height,
            const enum Bit* map) : Character(UTF8Code, width, height) {
    memcpy(this->map, map, width * height * sizeof(enum Bit));
}


Character::~Character() {

}

unsigned int Character::getHeight() const {
    return this->height;
}

unsigned int Character::getWidth() const {
    return this->width;
}

const enum Character::Bit* Character::getMap() {
    return this->map; // Becomes const at this point.
}

void Character::setBit(unsigned int x, unsigned int y, enum Bit value) {
    if(x < this->width && y < this->height) {
        this->map[y * this->width + x] = value;
    }
}

UChar32 Character::getUTF8Code() {
    return this->UTF8Code;
}
