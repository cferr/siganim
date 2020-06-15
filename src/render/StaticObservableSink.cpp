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

#include "StaticObservableSink.h"
#include "SignRenderer.h"

StaticObservableSink::StaticObservableSink(Sign* sign, const FontSet* fontSet,
        const Rasterizer* rasterizer) :
    sign(sign), fontSet(fontSet), rasterizer(rasterizer), frame(nullptr) {

    if(this->sign != nullptr) {
        this->render();
        this->sign->attach(this);
    }
}

StaticObservableSink::~StaticObservableSink() {
    // Detach from whom we're observing...
    this->sign->detach(this);
    delete this->frame;
}

void StaticObservableSink::render() {
    if(this->frame != nullptr)
        delete this->frame;
    SignRenderer r;
    this->frame = r.render(this->rasterizer, this->sign,
            this->fontSet, 0);
    this->changed();
}

Bitmap* StaticObservableSink::getFrame() const {
    return this->frame;
}

void StaticObservableSink::observe(const Observable *sender) {
    if(sender == this->sign) {
        this->render();
    }
}

void StaticObservableSink::setRasterizer(const Rasterizer *rasterizer) {
    this->rasterizer = rasterizer;
}
