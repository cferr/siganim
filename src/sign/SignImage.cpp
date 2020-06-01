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

#include <cstdlib>
#include <cassert>
#include <cstring>
#include <iostream>
#include <exception>
#include "SignImage.h"

SignImage::SignImage(unsigned int width, unsigned int height) :
        width(width), height(height) {
    this->pixels = (SignColor*)calloc(height * width,
                sizeof(SignColor));
}

SignImage::~SignImage() {
    free(this->pixels);
}

bool SignImage::setPixel(const unsigned int x, const unsigned int y,
        const SignColor& value) {
    if(x < this->width && y < this->height) {
        this->pixels[y * this->width + x] = value;
        return true;
    }
    else return false;
}

SignColor& SignImage::getPixel(const unsigned int x, const unsigned int y)
    const {
    if(x < this->width && y < this->height) {
        return this->pixels[y * this->width + x];
    } else throw std::out_of_range("Pixel out of range");
}

const SignColor* SignImage::getPixels() const {
    return this->pixels; // Const-ifies pixels in doing this
}

unsigned int SignImage::getWidth() const {
    return this->width;
}

unsigned int SignImage::getHeight() const {
    return this->height;
}

void SignImage::merge(const SignImage* top, const unsigned int x,
        const unsigned int y) {
    assert(x + top->width <= this->width);
    assert(y + top->height <= this->height);

    const SignColor* pixelsToMerge = top->getPixels();
    const unsigned int topWidth = top->getWidth();

    for (unsigned int i = 0; i < top->height; ++i) {
        for (unsigned int j = 0; j < top->width; ++j) {
            this->pixels[((i + y) * this->width) + j + x] =
                    pixelsToMerge[i * topWidth + j];
        }
    }

}



