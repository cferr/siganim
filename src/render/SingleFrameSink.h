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

#ifndef RENDER_SINGLEFRAMESINK_H_
#define RENDER_SINGLEFRAMESINK_H_

#include "../font/FontSet.h"
#include "../sign/Sign.h"
#include "Bitmap.h"
#include "Rasterizer.h"

class SingleFrameSink {
    const FontSet* fontSet;
    const Rasterizer* rasterizer;
    unsigned int scaleFactor;

public:
    SingleFrameSink(const FontSet* fontSet, const Rasterizer* rasterizer,
            unsigned int scaleFactor = 1);
    virtual ~SingleFrameSink() {
    }

    void setScaleFactor(unsigned int scaleFactor);
    unsigned int getScaleFactor() const;

    void setRasterizer(const Rasterizer* rasterizer);

    Bitmap* render(const Sign* s, unsigned int frame);
};

#endif /* RENDER_SINGLEFRAMESINK_H_ */
