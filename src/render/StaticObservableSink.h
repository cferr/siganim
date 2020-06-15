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

#ifndef SRC_RENDER_STATICOBSERVABLESINK_H_
#define SRC_RENDER_STATICOBSERVABLESINK_H_

#include <thread>
#include <mutex>

#include "../font/FontSet.h"
#include "../sign/Sign.h"
#include "../sign/Observer.h"
#include "Rasterizer.h"

class StaticObservableSink : public Observable, public Observer {
private:
    Sign* sign; // cannot be const as attach / detach modify it

    const FontSet* fontSet;
    const Rasterizer* rasterizer;

    Bitmap* frame;

    void render();
    void run();
    void copySign();

public:
    StaticObservableSink(Sign* sign, const FontSet* fontSet,
            const Rasterizer* rasterizer);
    virtual ~StaticObservableSink();

    Bitmap* getFrame() const;

    void setRasterizer(const Rasterizer* rasterizer);

    virtual void observe(const Observable* sender);

};

#endif /* RENDER_STATICOBSERVABLESINK_H_ */
