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

#ifndef UI_SIGANIMUICORE_H_
#define UI_SIGANIMUICORE_H_

#include <QObject>
#include "../SiganimCore.h"

class SiganimMainWindow;

class SiganimUICore : public QObject {
    Q_OBJECT

    SiganimMainWindow* mainWindow;
    SiganimCore* core;

public:
    SiganimUICore(SiganimCore* core);
    virtual ~SiganimUICore();

    void showUI();
    void saveSignToGIF();

    SiganimCore* getCore() const;
};

#endif /* UI_SIGANIMUICORE_H_ */
