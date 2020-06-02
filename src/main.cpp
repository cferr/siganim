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
#include "font/parsers/GirouetteFontsParser.h"
#include "font/FontSet.h"


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


    FontSet fset;
    // Test import font.
    std::vector<Font*> girouetteFonts = GirouetteFontsParser::parseGirouetteXML(
                "./font.xml");
    fset.addFonts(girouetteFonts);
    Font* font1912 = fset.lookup("Test font 19x12", "");
    Font* font1207 = fset.lookup("Test font 12x7", "");
    Font* font0704 = fset.lookup("Test font 7x4", "");
    Text* modifiableText = new Text(font1207,
            Text::HALIGN_CENTER,
            Text::VALIGN_CENTER_BOTTOM,
            "UNION HUB");

    // Set fancy colors...
    Compose* lineNumber = new Compose(
            new Fill(SignColor(SignColor::OFF, 155, 0, 255)),
            new Text(font1912,
            Text::HALIGN_CENTER,
            Text::VALIGN_CENTER_BOTTOM,
            "7", SignColor(SignColor::ON, 255, 255, 0))
    );

    // Mock sign just to test.
    Sign *testSign = new Sign(120, 24,
            { new Display(120, 24, Display::DISPLAY_RGB_LED,
                    new MarqueeAnimation(
                    new Split(Split::SPLIT_VERTICAL, 30,
                            new MarqueeAnimation(lineNumber, 20,
                                    MarqueeAnimation::LEFT, true, 10),
                            new Split(
                                    Split::SPLIT_HORIZONTAL, 12,
                                    modifiableText,
                                    new BlinkAnimation(
                                            new Text(font0704,
                                            Text::HALIGN_CENTER,
                                            Text::VALIGN_CENTER_BOTTOM,
                                            "VIA 17TH & 18TH ST"),
                                            10, 5
                                    )
                            )
                    ), 60, MarqueeAnimation::RIGHT, true, 0
                    )
                )
            }
    );



    std::cout << *testSign << std::endl;

    DurationComputer r;
    unsigned int totalFrames = r.computeTotalFrames(testSign);
    std::cout << "Sign has " << totalFrames << " frames";

    GIFSink sink(testSign);
    sink.render("40.gif");

#ifdef GUI
    SiganimMainWindow m(testSign, modifiableText);
    m.show();
    ret |= a.exec();
#endif

    delete testSign; // also deletes modifiableText

    return ret;
}
