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


SignImage::SignImage(unsigned int width, unsigned int height,
        unsigned int boxWidth, unsigned int boxHeight, const SignColor &fill):
        width(width), height(height), boxWidth(boxWidth), boxHeight(boxHeight),
        background(fill) {
    this->pixels = (SignColor*)malloc(height * width *
                sizeof(SignColor));

    SignColor bg(fill);
    for(uint32_t pos = 0; pos < height * width; ++pos) {
        this->pixels[pos] = bg;
    }
}

SignImage::SignImage(unsigned int width, unsigned int height,
        unsigned int boxWidth, unsigned int boxHeight) :
        SignImage(width, height, boxWidth, boxHeight,
        SignColor::defaultColor(SignColor::BACKGROUND))
{

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

void SignImage::merge(const SignImage* top, const int x,
        const int y) {
    // Copy all pixels of top that fit into the current image
    const SignColor* pixelsToMerge = top->getPixels();
    const unsigned int topWidth = top->getWidth();

    for (unsigned int i = 0; i < top->height; ++i) {
        for (unsigned int j = 0; j < top->width; ++j) {
            if(i + y >= 0 && i + y < this->height &&
                    j + x >= 0 && j + x < this->width) {
                this->pixels[((i + y) * this->width) + j + x] =
                        pixelsToMerge[i * topWidth + j];
            }
        }
    }

}

unsigned int SignImage::getBoxWidth() const {
    return this->boxWidth;
}

unsigned int SignImage::getBoxHeight() const {
    return this->boxHeight;
}

SignImage* SignImage::cropToBox() const {
    SignImage* ret = new SignImage(this->boxWidth, this->boxHeight,
            this->boxWidth, this->boxHeight);
    ret->merge(this, 0, 0);
    return ret;
}

const SignColor SignImage::getBackgroundColor() const {
    return this->background;
}
