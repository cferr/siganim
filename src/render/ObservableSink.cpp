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

#include <chrono>
#include "ObservableSink.h"
#include "SignRenderer.h"
#include "DurationComputer.h"

ObservableSink::ObservableSink(Sign* sign, const FontSet* fontSet,
        const Rasterizer* rasterizer) :
    sign(sign), fontSet(fontSet), signCopy(nullptr), rasterizer(rasterizer),
    continueRunning(true),
    signUpdated(false) {

    this->copySign();
    this->runner = std::thread(&ObservableSink::run, this);

    if(this->sign != nullptr) {
        this->sign->attach(this);
    }
}

ObservableSink::~ObservableSink() {
    // Detach from whom we're observing...
    this->sign->detach(this);
    // Our observers will be detached by the Observable destructor.
    this->continueRunning = false;
    // Wait for runner to finish
    if(this->runner.joinable())
        this->runner.join();
    // Delete frames
    for(auto i = this->frames.begin(); i < this->frames.end(); ++i)
        delete *i;
}

void ObservableSink::run() {

    bool doRender = true;
    if(sign == nullptr)
        return;

    while(this->continueRunning) {
        {
            std::lock_guard<std::mutex> guard(this->signUpdatedMutex);
            if(this->signUpdated) {
                doRender = true;
                this->copySign();
                this->signUpdated = false;
            }
        // Mutex gets unlocked after this point
        }
        // Render with mutex unlocked
        if(doRender) {
            this->render();
            doRender = false;
        }
        this->changed();
        if(!this->frames.empty()) {
            this->currentFrame++;
            if(this->currentFrame == this->frames.end())
                this->currentFrame = this->frames.begin();
        }
        std::this_thread::sleep_for(std::chrono::nanoseconds(100000000));
    }
}

void ObservableSink::copySign() {
    if(this->signCopy != nullptr)
        delete this->signCopy;
    this->signCopy = new Sign(this->sign);
}

void ObservableSink::render() {
    for(auto i = this->frames.begin(); i < this->frames.end(); ++i)
        delete *i;
    this->frames.clear();
    SignRenderer r;
    DurationComputer c;
    unsigned int frames = c.computeTotalFrames(this->signCopy);
    for(unsigned int frame = 0; frame < frames; frame++) {
        Bitmap* bmap = r.render(this->rasterizer, this->signCopy,
                this->fontSet, frame);
        this->frames.push_back(bmap);
    }
    this->currentFrame = this->frames.begin();
}



Bitmap* ObservableSink::getFrame() const {
    return *(this->currentFrame);
}

void ObservableSink::observe(const Observable *sender) {
    if(sender == this->sign) {
        std::lock_guard<std::mutex> guard(this->signUpdatedMutex);
        this->signUpdated = true;
    }
}

void ObservableSink::setRasterizer(const Rasterizer *rasterizer) {
    this->rasterizer = rasterizer;
}
