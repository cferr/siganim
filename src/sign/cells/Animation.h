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

#ifndef SRC_SIGN_CELLS_ANIMATION_H_
#define SRC_SIGN_CELLS_ANIMATION_H_

#include "../SignCell.h"

class Animation: public SignCell {
public:
    class SubjectBuilder : public Builder {
    private:
        Animation* animation;
    public:
        SubjectBuilder(Animation* animation);
        bool build(SignCell* child);
    };

protected:
    SignCell* subject;
    unsigned int durationFrames;
    unsigned int initialPhaseFrames;

public:
    Animation(SignCell* subject, unsigned int durationFrames,
            unsigned int initialPhaseFrames);
    Animation(const Animation* a);
    virtual ~Animation();

    unsigned int getHeight() const;
    unsigned int getWidth() const;
    unsigned int getChildHeight(const SignCell* child) const;
    unsigned int getChildWidth(const SignCell* child) const;

    bool setSubject(SignCell* subject);
    SignCell* getSubject() const;
    SubjectBuilder* subjectBuilder();

    void setInitialPhaseFrames(unsigned int initialPhaseFrames);

    unsigned int getDurationFrames() const;
    unsigned int getInitialPhaseFrames() const;

    void deleteChild(SignTree* child);

    void deepDetachStructureObserver(SignTreeStructureObserver* observer);

};

#endif /* SRC_SIGN_CELLS_ANIMATION_H_ */
