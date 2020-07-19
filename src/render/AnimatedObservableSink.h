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

#ifndef SRC_RENDER_OBSERVABLESINK_H_
#define SRC_RENDER_OBSERVABLESINK_H_

#include <thread>
#include <mutex>

#include "../font/FontSet.h"
#include "../sign/Sign.h"
#include "../sign/Observer.h"
#include "Rasterizer.h"

class AnimatedObservableSink : public Observable, public Observer {
private:
    Sign* sign; // cannot be const as attach / detach modify it

    const FontSet* fontSet;
    const Sign* signCopy;
    const Rasterizer* rasterizer;

    std::vector<Bitmap*> frames;
    std::vector<Bitmap*>::iterator currentFrame;

    bool continueRunning;
    std::thread runner;

    bool signUpdated;
    std::mutex signUpdatedMutex;

    unsigned int scaleFactor;

    void render();
    void run();
    void copySign();

public:
    AnimatedObservableSink(Sign* sign, const FontSet* fontSet,
            const Rasterizer* rasterizer, unsigned int scaleFactor = 1);
    virtual ~AnimatedObservableSink();

    Bitmap* getFrame() const;

    void setScaleFactor(unsigned int scaleFactor);
    unsigned int getScaleFactor() const;

    void setRasterizer(const Rasterizer* rasterizer);

    virtual void observe(const Observable* sender);

};

#endif /* RENDER_OBSERVABLESINK_H_ */
