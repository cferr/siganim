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
#include "../sign/SignCellSplit.h"
#include "../sign/MarqueeAnimation.h"
#include "../sign/BlinkAnimation.h"
#include "../sign/Compose.h"

DurationComputer::DurationComputer() {
}

void DurationComputer::DurationComputerVisitor::visit(const Sign &s) {
    unsigned int signTotalFrames = 1;
    std::vector<SignDisplay*> displays = s.getDisplays();
    for(auto i = displays.begin(); i < displays.end(); ++i) {
        (*i)->accept(*this);
        unsigned int displayTotal = this->totalFrames;
        // Eclipse doesn't handle this LCM which is from C++17.
        signTotalFrames = std::lcm<unsigned int, unsigned int> // @suppress("Function cannot be resolved") // @suppress("Symbol is not resolved")
            (signTotalFrames, displayTotal);
    }
    this->totalFrames = signTotalFrames;
}

void DurationComputer::DurationComputerVisitor::visit(const SignDisplay &s) {
    s.getRootCell()->accept(*this);
}

void DurationComputer::DurationComputerVisitor::visit(const Compose &s) {
    s.getBackground()->accept(*this);
    unsigned int bgFrames = this->totalFrames;
    s.getForeground()->accept(*this);
    this->totalFrames = std::lcm<unsigned int, unsigned int> // @suppress("Function cannot be resolved") // @suppress("Symbol is not resolved")
        (this->totalFrames, bgFrames);
}

void DurationComputer::DurationComputerVisitor::visit(const SignCellSplit &s) {
    s.getTopOrLeftChild()->accept(*this);
    unsigned int topLeftFrames = this->totalFrames;
    s.getBottomOrRightChild()->accept(*this);
    this->totalFrames = std::lcm<unsigned int, unsigned int> // @suppress("Function cannot be resolved") // @suppress("Symbol is not resolved")
        (this->totalFrames, topLeftFrames);
}

void DurationComputer::DurationComputerVisitor::visit(const SignCellText &s) {
    this->totalFrames = 1;
}

void DurationComputer::DurationComputerVisitor::visit(const MarqueeAnimation &s) {
    s.getSubject()->accept(*this);
    this->totalFrames = std::lcm<unsigned int, unsigned int> // @suppress("Function cannot be resolved") // @suppress("Symbol is not resolved")
        (this->totalFrames, s.getDurationFrames());
}

void DurationComputer::DurationComputerVisitor::visit(const BlinkAnimation &s) {
    s.getSubject()->accept(*this);
    this->totalFrames = std::lcm<unsigned int, unsigned int> // @suppress("Function cannot be resolved") // @suppress("Symbol is not resolved")
        (this->totalFrames, s.getDurationFrames());
}


unsigned int DurationComputer::computeTotalFrames(const Sign *s) {
    DurationComputerVisitor visitor;
    s->accept(visitor);

    return visitor.getTotalFrames();
}
