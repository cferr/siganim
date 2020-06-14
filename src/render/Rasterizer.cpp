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

Rasterizer::Rasterizer(std::string name) : name(name),
    pixelOverlay(nullptr), hasPixelOverlay(false) {

}

Rasterizer::Rasterizer(std::string name, Bitmap *pixelOverlay) :
    name(name), pixelOverlay(pixelOverlay), hasPixelOverlay(true) {

}

void Rasterizer::rasterizeAdd(Bitmap* dest, SignImage* source,
        Display::Type sourceType, unsigned int x, unsigned int y) const {
    // Crop the result into its own box, in case it hasn't been done before.
    SignImage* sourceCropped = source->cropToBox();

    const SignColor* simgPixels = sourceCropped->getPixels();
    struct Bitmap::pixel* imgPixels = dest->getPixels();

    const unsigned int simgHeight = sourceCropped->getHeight();
    const unsigned int simgWidth = sourceCropped->getWidth();

    const unsigned int dimgWidth = dest->getWidth();

    struct SignColor::RGB defaultRGBOn;
    struct SignColor::RGB defaultRGBOff;

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


    for(unsigned int i = 0; i < simgHeight; ++i) {
        for(unsigned int j = 0; j < simgWidth; ++j) {
            struct SignColor::RGB p;
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
            for(unsigned int ii = 0; ii < 5; ++ii) {
                for(unsigned int jj = 0; jj < 5; ++jj) {
                    // Assuming no transparency.
                    // TODO add alpha channel (along with Image Fill)
                    imgPixels[(5*(i+y)+ii)*dimgWidth+5*(j+x)+jj] = {
                            p.r, p.g, p.b, 255
                    };
                }
            }
        }
    }
}

Bitmap* Rasterizer::rasterize(SignImage* source, Display::Type sourceType)
    const {
    Bitmap* ret = new Bitmap(5 * source->getWidth(), 5 * source->getHeight());
    this->rasterizeAdd(ret, source, sourceType, 0, 0);
    return ret;
}

std::string Rasterizer::getName() const {
    return this->name;
}
