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


#ifndef UI_PAGES_PAGEMARQUEEANIMATION_H_
#define UI_PAGES_PAGEMARQUEEANIMATION_H_

#include <QRadioButton>
#include <QSpinBox>
#include <QWidget>
#include "../../sign/cells/MarqueeAnimation.h"

class PageMarqueeAnimation : public QWidget {
    Q_OBJECT
private:
    MarqueeAnimation* treeNode;
    QRadioButton* direction_LEFT;
    QRadioButton* direction_RIGHT;
    QSpinBox* spaceSpinner;
    QSpinBox* durationSpinner;

public:
    PageMarqueeAnimation(MarqueeAnimation* treeNode);
    virtual ~PageMarqueeAnimation() { }

public slots:
    void setSpace(int space);
    void setDurationFrames(int frames);
    void setDirection_LEFT(bool set);
    void setDirection_RIGHT(bool set);
};

#endif /* UI_PAGES_PAGEMARQUEEANIMATION_H_ */
