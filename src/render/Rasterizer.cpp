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

#include "Rasterizer.h"
#include "../SiganimDefaults.h"

Rasterizer::Rasterizer(const Rasterizer &rasterizer) :
    Rasterizer(rasterizer.name, rasterizer.pixelOverlay, rasterizer.width,
            rasterizer.height) {
}

Rasterizer::Rasterizer(std::string name, unsigned int width,
        unsigned int height) :
        name(name), width(width), height(height) {
    this->pixelOverlay = new Bitmap(width, height); // calloc'd -> transparent
}

Rasterizer::Rasterizer(std::string name, Bitmap *pixelOverlay,
        unsigned int width, unsigned int height) :
    name(name), width(width), height(height) {
    this->pixelOverlay = new Bitmap(*pixelOverlay);
}

Rasterizer::~Rasterizer() {
    delete this->pixelOverlay;
}

void Rasterizer::rasterizeAdd(Bitmap* dest, SignImage* source,
        Display::Type sourceType, unsigned int x, unsigned int y,
        unsigned int scaleFactor) const {
    // Crop the result into its own box, in case it hasn't been done before.
    SignImage* sourceCropped = source->cropToBox();

    const SignColor* simgPixels = sourceCropped->getPixels();
    struct Bitmap::pixel* imgPixels = dest->getPixels();

    const unsigned int simgHeight = sourceCropped->getHeight();
    const unsigned int simgWidth = sourceCropped->getWidth();

    const unsigned int dimgWidth = dest->getWidth();

    struct SignColor::RGBA defaultRGBOn;
    struct SignColor::RGBA defaultRGBOff;

    switch(sourceType) {
    case Display::DISPLAY_FLIPDISC:
        defaultRGBOn = SiganimDefaults::defaultFlipDiscBG.getValue();
        defaultRGBOff = SiganimDefaults::defaultFlipDiscFG.getValue();
        break;
    case Display::DISPLAY_MONOCHROME_LED:
        defaultRGBOn = SiganimDefaults::defaultMonoLEDBG.getValue();
        defaultRGBOff = SiganimDefaults::defaultMonoLEDFG.getValue();
        break;
    case Display::DISPLAY_RGB_LED:
        defaultRGBOn = SiganimDefaults::defaultRGBLEDBG.getValue();
        defaultRGBOff = SiganimDefaults::defaultRGBLEDFG.getValue();
        break;
    }

    struct Bitmap::pixel* overlayPixels = this->pixelOverlay->getPixels();

    for(unsigned int i = 0; i < simgHeight; ++i) {
        for(unsigned int j = 0; j < simgWidth; ++j) {
            struct SignColor::RGBA p;
            if(sourceType == Display::DISPLAY_RGB_LED) {
                try {
                    p = simgPixels[i*simgWidth+j].getValue();
                } catch(SignColor::NoRGBValueException& e) {
                    p = simgPixels[i*simgWidth+j].getSemantic()?defaultRGBOn:
                            defaultRGBOff;
                }
            } else {
                p = simgPixels[i*simgWidth+j].getSemantic()?defaultRGBOn:
                    defaultRGBOff;
            }
            unsigned int H = this->height * scaleFactor;
            unsigned int W = this->width * scaleFactor;
            for(unsigned int ii = 0; ii < this->height; ++ii) {
                for(unsigned int jj = 0; jj < this->width; ++jj) {
                    struct Bitmap::pixel q = overlayPixels[ii*this->width+jj];
                    for(unsigned int iii = 0; iii < scaleFactor; ++iii) {
                        for(unsigned int jjj = 0; jjj < scaleFactor; ++jjj) {
                            imgPixels[(H*(i+y)+ii*scaleFactor+iii)*dimgWidth
                                  + W*(j+x)+jj*scaleFactor+jjj] = {
                                (unsigned char)(((int)p.r * (255 - (int)q.a)
                                        + (int)q.r * 255) / 255),
                                (unsigned char)(((int)p.g * (255 - (int)q.a)
                                        + (int)q.g * 255) / 255),
                                (unsigned char)(((int)p.b * (255 - (int)q.a)
                                        + (int)q.b * 255) / 255),
                                p.a
                            };
                        }
                    }
                }
            }
        }
    }

    delete sourceCropped;
}

Bitmap* Rasterizer::rasterize(SignImage* source, Display::Type sourceType,
        unsigned int scaleFactor) const {
    Bitmap* ret = new Bitmap(scaleFactor * this->width * source->getWidth(),
            scaleFactor * this->height * source->getHeight());
    this->rasterizeAdd(ret, source, sourceType, 0, 0, scaleFactor);
    return ret;
}

std::string Rasterizer::getName() const {
    return this->name;
}
