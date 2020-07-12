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

#ifndef SRC_FONT_CHARACTER_H_
#define SRC_FONT_CHARACTER_H_

#include <unicode/schriter.h>
#include "../Serializable.h"

class Character : public Serializable {
public:
    enum Bit { OFF, ON };

private:
    UChar32 UTF8Code;
    unsigned int width;
    unsigned int height;
    enum Bit* map;

public:
    Character(UChar32 UTF8Code, unsigned int width, unsigned int height);
    Character(UChar32 UTF8Code, unsigned int width, unsigned int height,
            const enum Bit* map);
    Character(const Character& c);
    virtual ~Character();

    const enum Bit* getMap();

    // TODO character borders
    void setBit(unsigned int x, unsigned int y, enum Bit value);
    enum Bit getBit(unsigned int x, unsigned int y);
    void toggleBit(unsigned int x, unsigned int y);

    unsigned int getHeight() const;
    void setHeight(unsigned int height);
    unsigned int getWidth() const;
    void setWidth(unsigned int width);

    UChar32 getUTF8Code();

    virtual json_object* toJSON() const;
};

#endif /* SRC_FONT_CHARACTER_H_ */
