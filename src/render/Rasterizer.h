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

#ifndef RENDER_RASTERIZER_H_
#define RENDER_RASTERIZER_H_

#include "Bitmap.h"
#include "../sign/SignImage.h"
#include "../sign/cells/Display.h"

class Rasterizer {
    std::string name;
    Bitmap* pixelOverlay;
    unsigned int width;
    unsigned int height;

public:
    Rasterizer(const Rasterizer& rasterizer);
    Rasterizer(std::string name, unsigned int width,
            unsigned int height);
    Rasterizer(std::string name, Bitmap* pixelOverlay, unsigned int width,
            unsigned int height);
    virtual ~Rasterizer();

    std::string getName() const;

    void rasterizeAdd(Bitmap* dest, SignImage* source,
            Display::Type sourceType, unsigned int x, unsigned int y,
            unsigned int scaleFactor) const;
    Bitmap* rasterize(SignImage* source, Display::Type sourceType,
            unsigned int scaleFactor) const;
};

#endif /* RENDER_RASTERIZER_H_ */
