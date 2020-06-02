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

#ifndef SRC_RENDER_DURATIONCOMPUTER_H_
#define SRC_RENDER_DURATIONCOMPUTER_H_

#include "../sign/SignTreeVisitor.h"

class DurationComputer {
private:
    class DurationComputerVisitor : public ConstSignTreeVisitor {
    private:
        unsigned int totalFrames;

    public:
        DurationComputerVisitor() : totalFrames(1) {

        }
        virtual ~DurationComputerVisitor() {
        }

        virtual void visit(const Sign &s);
        virtual void visit(const SignDisplay &s);
        virtual void visit(const SignCellSplit &s);
        virtual void visit(const SignCellText &s);
        virtual void visit(const MarqueeAnimation &s);
        virtual void visit(const BlinkAnimation &s);
        virtual void visit(const Compose &s);

        unsigned int getTotalFrames() {
            return totalFrames;
        }

    };

public:
    DurationComputer();
    virtual ~DurationComputer() {}

    unsigned int computeTotalFrames(const Sign *s);
};

#endif /* SRC_RENDER_DURATIONCOMPUTER_H_ */
