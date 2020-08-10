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


#ifndef UI_PAGES_PAGEBLINKANIMATION_H_
#define UI_PAGES_PAGEBLINKANIMATION_H_

#include <QSpinBox>
#include <QWidget>
#include "../../../sign/cells/BlinkAnimation.h"

class PageBlinkAnimation : public QWidget {
    Q_OBJECT
private:
    BlinkAnimation* treeNode;
    QSpinBox* onFramesSpinner;
    QSpinBox* offFramesSpinner;
    QSpinBox* phaseFramesSpinner;

public:
    PageBlinkAnimation(BlinkAnimation* treeNode);
    virtual ~PageBlinkAnimation() { }

public slots:
    void setFramesOn(int frames);
    void setFramesOff(int frames);
    void setPhaseFrames(int frames);
};

#endif /* UI_PAGES_PAGEBLINKANIMATION_H_ */
