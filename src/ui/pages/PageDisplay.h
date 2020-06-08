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


#ifndef UI_PAGES_PAGEDISPLAY_H_
#define UI_PAGES_PAGEDISPLAY_H_

#include <QRadioButton>
#include <QSpinBox>
#include <QWidget>
#include "../../sign/cells/Display.h"

class PageDisplay : public QWidget {
    Q_OBJECT
private:
    Display* treeNode;
    QSpinBox* heightSpinner;
    QSpinBox* widthSpinner;
    QRadioButton* displayType_DISPLAY_RGB_LED;
    QRadioButton* displayType_DISPLAY_MONOCHROME_LED;
    QRadioButton* displayType_DISPLAY_FLIPDISC;

public:
    PageDisplay(Display* treeNode);
    virtual ~PageDisplay() { }

public slots:
    void setHeight(int height);
    void setWidth(int width);
    void setDisplayType_DISPLAY_RGB_LED(bool set);
    void setDisplayType_DISPLAY_MONOCHROME_LED(bool set);
    void setDisplayType_DISPLAY_FLIPDISC(bool set);
};

#endif /* UI_PAGES_PAGEDISPLAY_H_ */
