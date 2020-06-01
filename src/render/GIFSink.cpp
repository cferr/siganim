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

#include "../../gif-h/gif.h"
#include "GIFSink.h"
#include "SignRenderer.h"
#include "DurationComputer.h"

GIFSink::GIFSink(const Sign* sign) : sign(sign) {
}

GIFSink::~GIFSink() {

}

void GIFSink::render(const char* fileName) {
    SignRenderer r;

    Bitmap* bmap = r.render(this->sign, 0);
    unsigned int width = bmap->getWidth();
    unsigned int height = bmap->getHeight();

    DurationComputer c;
    unsigned int frames = c.computeTotalFrames(this->sign);

    int delay = 1;
    GifWriter g;
    GifBegin(&g, fileName, width, height, delay);
    GifWriteFrame(&g, bmap->toRGBA8Vector().data(), width, height, delay);

    for(unsigned int frame = 1; frame < frames; frame++) {
        bmap = r.render(this->sign, frame);
        GifWriteFrame(&g, bmap->toRGBA8Vector().data(), width, height, delay);
    }

    GifEnd(&g);
}
