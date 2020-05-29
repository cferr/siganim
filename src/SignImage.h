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
#include "Bitmap.h"
#include "SignColor.h"

class SignImage {
private:
    unsigned int width;
    unsigned int height;
    SignColor *pixels;

public:

    SignImage(unsigned int width, unsigned int height);
    virtual ~SignImage();

    unsigned int getWidth() const;
    unsigned int getHeight() const;

    bool setPixel(const unsigned int x, const unsigned int y,
            const SignColor& value);
    SignColor& getPixel(const unsigned int x, const unsigned int y) const;
    const SignColor* getPixels() const;

    void merge(const SignImage* top, const unsigned int x,
            const unsigned int y);

    Bitmap* toImage() const;

};



#endif /* SRC_SIGNIMAGE_H_ */
