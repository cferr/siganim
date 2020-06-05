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

#include <vector>
#include <numeric>
#include "DurationComputer.h"
#include "../sign/Sign.h"
#include "../sign/cells/MarqueeAnimation.h"
#include "../sign/cells/BlinkAnimation.h"
#include "../sign/cells/Compose.h"
#include "../sign/cells/Display.h"
#include "../sign/cells/Split.h"

DurationComputer::DurationComputer() {
}

unsigned int DurationComputer::computeTotalFrames(const Sign *s) {
    DurationComputerVisitor visitor;
    s->accept(visitor);

    return visitor.getTotalFrames();
}

void DurationComputer::DurationComputerVisitor::visit(const Sign &s) {
    unsigned int signTotalFrames = 1;
    std::vector<Display*> displays = s.getDisplays();
    for(auto i = displays.begin(); i < displays.end(); ++i) {
        try {
            (*i)->accept(*this);
        } catch(std::exception& e) {
            this->totalFrames = 1;
        }
        unsigned int displayTotal = this->totalFrames;
        signTotalFrames = std::lcm<unsigned int, unsigned int>
            (signTotalFrames, displayTotal);
    }
    this->totalFrames = signTotalFrames;
}

void DurationComputer::DurationComputerVisitor::visit(const Display &s) {
    try {
        s.getRootCell()->accept(*this);
    } catch(std::exception& e) {
        this->totalFrames = 1;
    }
}

void DurationComputer::DurationComputerVisitor::visit(const Compose &s) {
    try {
        s.getBackground()->accept(*this);
    } catch(std::exception& e) {
        this->totalFrames = 1;
    }
    unsigned int bgFrames = this->totalFrames;
    try {
        s.getForeground()->accept(*this);
    } catch(std::exception& e) {
        this->totalFrames = 1;
    }
    this->totalFrames = std::lcm<unsigned int, unsigned int>
        (this->totalFrames, bgFrames);
}

void DurationComputer::DurationComputerVisitor::visit(const Split &s) {
    try {
        s.getTopOrLeftChild()->accept(*this);
    } catch(std::exception& e) {
        this->totalFrames = 1;
    }
    unsigned int topLeftFrames = this->totalFrames;
    try {
        s.getBottomOrRightChild()->accept(*this);
    } catch(std::exception& e) {
        this->totalFrames = 1;
    }
    this->totalFrames = std::lcm<unsigned int, unsigned int>
        (this->totalFrames, topLeftFrames);
}

void DurationComputer::DurationComputerVisitor::visit(const Text &s) {
    this->totalFrames = 1;
}

void DurationComputer::DurationComputerVisitor::visit(const MarqueeAnimation &s) {
    try {
        s.getSubject()->accept(*this);
        this->totalFrames = std::lcm<unsigned int, unsigned int>
            (this->totalFrames, s.getDurationFrames());
    } catch(std::exception& e) {
        this->totalFrames = 1;
    }

}

void DurationComputer::DurationComputerVisitor::visit(const BlinkAnimation &s) {
    try {
        s.getSubject()->accept(*this);
        this->totalFrames = std::lcm<unsigned int, unsigned int>
            (this->totalFrames, s.getDurationFrames());
    } catch(std::exception& e) {
        this->totalFrames = 1;
    }

}

void DurationComputer::DurationComputerVisitor::visit(const Fill &s) {
    this->totalFrames = 1;
}
