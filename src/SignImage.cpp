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

SignRgbPixel SignImage::defaultFlipDiscBG = { 0, 0, 0 };
SignRgbPixel SignImage::defaultFlipDiscFG = { 0, 0, 0 };
SignRgbPixel SignImage::defaultMonoLEDBG = { 0, 0, 0 };
SignRgbPixel SignImage::defaultMonoLEDFG = { 0, 0, 0 };
SignRgbPixel SignImage::defaultRGBLEDBG = { 0, 0, 0 };
SignRgbPixel SignImage::defaultRGBLEDFG = { 0, 0, 0 };


SignImage::SignImage(unsigned int height, unsigned int width) : height(height),
		width(width) {
	this->initPixelMatrix();
}

void SignImage::initPixelMatrix() {
	this->pixels = (SignRgbPixel*)calloc(this->height * this->width,
			sizeof(SignRgbPixel));
}

SignImage::~SignImage() {

}

void SignImage::merge(const SignImage top, const unsigned int x,
		const unsigned int y) {
	assert(x + top.width <= this->width);
	assert(y + top.height <= this->height);

	for(unsigned int i = 0; i < this->height; ++i) {
		memcpy(this->pixels + ((i + x) * this->width),
				top.pixels + (x * top.width),
				top.width * sizeof(SignRgbPixel));
	}


}
