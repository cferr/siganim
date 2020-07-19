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

    // Define a sample sign

    Text* modifiableText = new Text("Siganim 12x7", "",
            Text::HALIGN_CENTER,
            Text::VALIGN_CENTER_BOTTOM,
            "UNION STN");

    // Set fancy colors... Palette?
    // Here you go: https://coolors.co/072ac8-1e96fc-a2d6f9-fcf300-ffc600
    Compose* lineNumber = new Compose(
            new Split(Split::SPLIT_SW_NE_DIAGONAL, 0,
                    new Split(Split::SPLIT_NW_SE_DIAGONAL, 0,
                        new Fill(SignColor(SignColor::OFF, 7, 42, 200)),
                        new Fill(SignColor(SignColor::OFF, 255, 198, 0))
                        //nullptr
                    ),
                    new BlinkAnimation(
                        new Split(Split::SPLIT_NW_SE_DIAGONAL, 0,
                            new Fill(SignColor(SignColor::OFF, 252, 243, 0)),
                            new Fill(SignColor(SignColor::OFF, 162, 214, 249))
                        ),
                        5, 5
                    )
            ),
            new Text("Siganim 19x12", "",
            Text::HALIGN_CENTER,
            Text::VALIGN_CENTER_BOTTOM,
            "17", SignColor(SignColor::ON, 30, 150, 252))
    );

    // Mock sign just to test.
    Sign *testSign = new Sign(
            { new Display(120, 24, Display::DISPLAY_RGB_LED,
                    new Split(Split::SPLIT_VERTICAL, 32,
                        lineNumber,
                        new Split(Split::SPLIT_HORIZONTAL, 12,
                            modifiableText,
                            new MarqueeAnimation(
                                new Text("Siganim 7x4", "",
                                    Text::HALIGN_CENTER,
                                    Text::VALIGN_CENTER_BOTTOM,
                                    "VIA 17TH & 18TH STREETS"
                                ),
                                50, MarqueeAnimation::LEFT, 20
                            )
                        )
                    )
                )
            }
    );

    core.setSign(testSign); // takes ownership of sign

    //core.getRasterizerSet()->addRasterizer(new Rasterizer("Default", 5, 5));


#ifdef GUI
//    SiganimMainWindow m(&core);
//    m.show();
    SiganimUICore uiCore(&core);
    uiCore.showUI();
    ret |= a.exec();
#endif

//    delete testSign; // also deletes modifiableText

    return ret;
}
