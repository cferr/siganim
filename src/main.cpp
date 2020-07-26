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

//#define GUI
//#define TEST

#ifdef GUI
#include <QApplication>
#endif
#include <iostream>
#include "sign/Sign.h"
#include "sign/cells/Display.h"
#include "sign/cells/Split.h"
#include "sign/cells/Text.h"
#include "sign/cells/MarqueeAnimation.h"
#include "sign/cells/BlinkAnimation.h"
#include "sign/cells/Compose.h"
#include "sign/cells/Fill.h"
#include "render/DurationComputer.h"
#include "render/GIFSink.h"
#include "render/Rasterizer.h"
#include "render/RasterizerSet.h"
#include "font/parsers/GirouetteFontsParser.h"
#include "font/FontSet.h"
#include "SiganimFileHandler.h"
#include "SiganimCore.h"

#ifdef GUI
#include "ui/SiganimUICore.h"
#endif

int main(int argc, char *argv[]) {
#ifdef GUI
    QApplication a(argc, argv);
#endif
    int ret = 0;

    // Core containing database
    SiganimCore core("fonts.json");
    core.setCurrentSignFile("sign.json");
    core.saveSignsToFile();

#ifdef GUI
    SiganimUICore uiCore(&core);
    uiCore.showUI();
    ret |= a.exec();
#endif

    return ret;
}
