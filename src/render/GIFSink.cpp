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

GIFSink::GIFSink(const Sign* sign) : sign(sign) {
}

GIFSink::~GIFSink() {

}

void GIFSink::render(const char* fileName) {
    SignRenderer r;
    Bitmap* bmap = r.render(this->sign, 0); // TODO add frame support

    unsigned int width = bmap->getWidth();
    unsigned int height = bmap->getHeight();


    int delay = 100;
    GifWriter g;
    GifBegin(&g, fileName, width, height, delay);
    GifWriteFrame(&g, bmap->toRGBA8Vector().data(), width, height, delay);
    GifEnd(&g);
}
