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
#include <stdexcept>

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

void Character::setHeight(unsigned int height) {
    unsigned int prevHeight = this->height;
    this->height = height;

    enum Bit* newBitmap = (enum Bit*)calloc(height * this->width,
            sizeof(enum Bit));
    // Update bit map
    for(unsigned int i = 0; i < this->width; ++i) {
        for(unsigned int j = 0; j < std::min(height, prevHeight); ++j) {
            newBitmap[j*this->width+i] = this->map[j*this->width+i];
        }
    }

    free(this->map);
    this->map = newBitmap;
}

void Character::setWidth(unsigned int width) {
    unsigned int prevWidth = this->width;
    this->width = width;

    enum Bit* newBitmap = (enum Bit*)calloc(this->height * width,
            sizeof(enum Bit));
    // Update bit map
    for(unsigned int i = 0; i < std::min(width, prevWidth); ++i) {
        for(unsigned int j = 0; j < this->height; ++j) {
            newBitmap[j*this->width+i] = this->map[j*prevWidth+i];
        }
    }

    free(this->map);
    this->map = newBitmap;
}

enum Character::Bit Character::getBit(unsigned int x, unsigned int y) {
    if(x < this->width && y < this->height) {
        return this->map[y*this->width+x];
    } else throw std::out_of_range("getBit");
}

void Character::toggleBit(unsigned int x, unsigned int y) {
    if(x < this->width && y < this->height) {
        switch(this->map[y*this->width+x]) {
        case Bit::ON:
            this->map[y*this->width+x] = Bit::OFF;
            break;
        case Bit::OFF:
            this->map[y*this->width+x] = Bit::ON;
            break;
        }
    }
}
