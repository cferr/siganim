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

#ifndef SRC_SIGNCOLOR_H_
#define SRC_SIGNCOLOR_H_

#include "SignDisplayType.h"
#include <cstdint>

class SignColor {
public:
    struct RGB {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    };
    enum Type {
        BACKGROUND, FOREGROUND
    };
private:
    enum Status {
        DEFAULT, CUSTOM
    };

    enum Type type;
    enum Status status;
    struct RGB value;

    SignColor(enum Type type, enum Status status, struct RGB value = {0, 0, 0});

    static SignColor defaultFlipDiscBG;
    static SignColor defaultFlipDiscFG;
    static SignColor defaultMonoLEDBG;
    static SignColor defaultMonoLEDFG;
    static SignColor defaultRGBLEDBG;
    static SignColor defaultRGBLEDFG;

    static SignColor defaultBG;
    static SignColor defaultFG;

public:
    virtual ~SignColor();

    static SignColor defaultColor(enum Type type);
    static SignColor RGB(enum Type type, unsigned char r, unsigned char g,
            unsigned char b);

    bool isDefault() const;
    enum Type getType() const;
    struct RGB getValue(DisplayType displayType) const;

};

#endif /* SRC_SIGNCOLOR_H_ */
