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

#define GUI
#define TEST

#ifdef GUI
#include <QApplication>
#endif
#include <iostream>
#include "sign/Sign.h"
#include "sign/SignCellSplit.h"
#include "sign/SignCellText.h"
#include "render/SignRenderer.h"
#include "font/parsers/GirouetteFontsParser.h"

#ifdef TEST
#include "test.h"
#endif

#ifdef GUI
#include "ui/SiganimMainWindow.h"
#endif

int main(int argc, char *argv[]) {
#ifdef GUI
    QApplication a(argc, argv);
#endif
    int ret = 0;

#ifdef TEST
    if(!siganimTests())
        ret = 1;
#endif


#ifdef GUI

    // Test import font.
    std::vector<Font*> f = GirouetteFontsParser::parseGirouetteXML(
                "./font.xml");

    Font* font = *(f.begin());

    // Mock sign just to test.
    Sign *testSign = new Sign(120, 24,
            { new SignDisplay(120, 24, DisplayType::DISPLAY_MONOCHROME_LED,
                    new SignCellSplit(SignCellSplit::SPLIT_VERTICAL, 30,
                            new SignCellText(font, "40"),
                            new SignCellText(font, "METRO TIMONE")
                    ))
            }
    );

    SiganimMainWindow m(testSign);
    m.show();
    ret |= a.exec();
#endif

    return ret;
}
