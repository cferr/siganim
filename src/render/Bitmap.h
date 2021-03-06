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

#ifndef SRC_BITMAP_H_
#define SRC_BITMAP_H_

#include <vector>
#include <cstdint>

class Bitmap {
public:
    struct pixel {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };
private:
    struct pixel* pixels;

    unsigned int width;
    unsigned int height;

public:
    Bitmap(unsigned int width, unsigned int height);
    Bitmap(const Bitmap& bitmap);
    Bitmap(const Bitmap* bitmap);

    unsigned int getHeight() const;
    unsigned int getWidth() const;

    struct pixel* getPixels();
    unsigned char* toRGB32();
    std::vector<uint8_t> toRGBA8Vector();

    void overlay(Bitmap* top, int x, int y);

    virtual ~Bitmap();
};

#endif /* SRC_BITMAP_H_ */
