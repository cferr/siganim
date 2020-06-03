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

#ifndef SRC_SIGANIMMAINWINDOW_H_
#define SRC_SIGANIMMAINWINDOW_H_

#include <QMainWindow>
#include <QTabWidget>

#include "SignEditor.h"
#include "FontStudio.h"
#include "../sign/cells/Text.h"

class SiganimMainWindow: public QMainWindow {
    Q_OBJECT
private:

    QTabWidget* tabs;
    SignEditor* editor;
    FontStudio* studio;

public:

    SiganimMainWindow();
    // Temporary.
    SiganimMainWindow(Sign* sign, Text* textCell);
    virtual ~SiganimMainWindow();


};

#endif /* SRC_SIGANIMMAINWINDOW_H_ */
