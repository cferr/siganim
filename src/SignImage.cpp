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

#include "SignImage.h"
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <iostream>

SignRgbPixel SignImage::defaultFlipDiscBG = { 0, 0, 0 };
SignRgbPixel SignImage::defaultFlipDiscFG = { 0, 0, 0 };
SignRgbPixel SignImage::defaultMonoLEDBG = { 0, 0, 0 };
SignRgbPixel SignImage::defaultMonoLEDFG = { 0, 0, 0 };
SignRgbPixel SignImage::defaultRGBLEDBG = { 0, 0, 0 };
SignRgbPixel SignImage::defaultRGBLEDFG = { 0, 0, 0 };

SignImage::SignImage(unsigned int height, unsigned int width) :
        height(height), width(width) {
    this->pixels = (SignRgbPixel*)calloc(height * width,
                sizeof(SignRgbPixel));
}

SignImage::~SignImage() {
    free(this->pixels);
}

bool SignImage::setPixel(const unsigned int x, const unsigned int y,
        const SignRgbPixel value) {
    if(x < this->width && y < this->height) {
        this->pixels[y * this->width + x] = value;
        return true;
    }
    else return false;
}

SignRgbPixel SignImage::getPixel(const unsigned int x, const unsigned int y)
    const {
    if(x < this->width && y < this->height) {
        return this->pixels[y * this->width + x];
    } else return { 0, 0, 0 };
}

const SignRgbPixel* SignImage::getPixels() const {
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

    const SignRgbPixel* pixelsToMerge = top->getPixels();

    for (unsigned int i = 0; i < top->height; ++i) {
        memcpy(this->pixels + ((i + y) * this->width) + x, pixelsToMerge
                + (i * top->getWidth()),
                top->getWidth() * sizeof(SignRgbPixel));
    }

}

