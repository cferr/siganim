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

#ifndef SRC_SIGNIMAGE_H_
#define SRC_SIGNIMAGE_H_

#include <vector>
#include "../render/Bitmap.h"
#include "SignColor.h"

class SignImage {
private:
    unsigned int width;
    unsigned int height;
    unsigned int boxWidth;
    unsigned int boxHeight;
    SignColor *pixels;
    const SignColor background;

public:

    SignImage(unsigned int width, unsigned int height,
            unsigned int boxWidth, unsigned int boxHeight);
    SignImage(unsigned int width, unsigned int height,
            unsigned int boxWidth, unsigned int boxHeight,
            const SignColor& fill);

    virtual ~SignImage();

    unsigned int getWidth() const;
    unsigned int getHeight() const;

    unsigned int getBoxWidth() const;
    unsigned int getBoxHeight() const;

    bool setPixel(const unsigned int x, const unsigned int y,
            const SignColor& value);
    SignColor& getPixel(const unsigned int x, const unsigned int y) const;
    const SignColor* getPixels() const;

    const SignColor getBackgroundColor() const;

    void merge(const SignImage* top, const int x,
            const int y);

    SignImage* cropToBox() const;

};



#endif /* SRC_SIGNIMAGE_H_ */
