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

#ifndef SRC_SIGNPIXEL_H_
#define SRC_SIGNPIXEL_H_

#include <cstdint>

enum SignPixelType {
	DISPLAY_RGB_LED,
	DISPLAY_MONOCHROME_LED,
	DISPLAY_FLIPDISC
};

struct SignRgbPixel {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

enum SignDotStatus {
	DOT_OFF,
	DOT_ON
};

#endif /* SRC_SIGNPIXEL_H_ */
