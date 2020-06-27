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

#include <iostream>

class SignColor {
public:
    class NoRGBValueException : public std::exception
    {
    public:
        NoRGBValueException() {
        }

        const char* what() const throw () {
            return "Color has no RGB value";
        }
    };

    struct RGBA {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };
    enum Semantic {
        ON, OFF
    };

private:

    enum Semantic semantic;
    bool isCustom;
    struct RGBA value;

public:
    SignColor(const SignColor& that);
    virtual ~SignColor();

    SignColor(enum Semantic type);
    SignColor(enum Semantic type, const unsigned char r, const unsigned char g,
            const unsigned char b, const unsigned char a = 255);

    static SignColor on();
    static SignColor off();

    bool hasRGBValue() const;
    enum Semantic getSemantic() const;

    SignColor Or(const SignColor& that) const;

    struct RGBA getValue() const;

    std::ostream& serialize(std::ostream &strm) const;

};

std::ostream& operator<<(std::ostream &strm, const SignColor &s);

#endif /* SRC_SIGNCOLOR_H_ */
