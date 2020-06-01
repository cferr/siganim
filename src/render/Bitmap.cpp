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
#include <cstdint>
#include "Bitmap.h"

Bitmap::Bitmap(unsigned int width, unsigned int height) : width(width),
    height(height) {
    this->pixels = (struct pixel*)calloc(height * width, sizeof(pixel));
}

Bitmap::~Bitmap() {

}

unsigned int Bitmap::getHeight() const {
    return this->height;
}

unsigned int Bitmap::getWidth() const {
    return this->width;
}


struct Bitmap::pixel* Bitmap::getPixels() {
    return this->pixels;
}

unsigned char* Bitmap::toRGB32() {
    uint32_t* result = (uint32_t*)malloc(this->height * this->width
            * sizeof(uint32_t));
    // unsigned int height = this->height;
    unsigned int width = this->width;
    if(result != NULL) {
        for(unsigned int i = 0; i < this->height; ++i) {
            for(unsigned int j = 0; j < this->width; ++j) {
                struct pixel p = this->pixels[i*width+j];
                result[i*width+j] = ((0xFF) << 24) + ((uint32_t)p.r << 16)
                        + ((uint32_t)p.g << 8) + ((uint32_t)p.b);
            }
        }
    }

    return (unsigned char*)result;
}

std::vector<uint8_t> Bitmap::toRGBA8Vector() {
    std::vector<uint8_t> result;

    unsigned int width = this->width;
    for(unsigned int i = 0; i < this->height; ++i) {
        for(unsigned int j = 0; j < this->width; ++j) {
            struct pixel p = this->pixels[i*width+j];
            result.push_back((uint8_t)p.r);
            result.push_back((uint8_t)p.g);
            result.push_back((uint8_t)p.b);
            result.push_back((uint8_t)0);
        }
    }

    return result;
}
