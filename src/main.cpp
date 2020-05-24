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

#ifdef GUI
#include <QApplication>
#endif
#include <iostream>
#include "Sign.h"
#include "SignCellNode.h"
#include "SignCellLeaf.h"
#include "SignRenderer.h"

#ifdef GUI
#include "ui/SiganimMainWindow.h"
#endif

int main(int argc, char *argv[]) {
#ifdef GUI
    QApplication a(argc, argv);
#endif
    int ret = 0;

    // Are we able to create a sign easily? Yes.
    Sign *testSign = new Sign(120, 80,
            { new SignDisplay(120, 80, SignPixelType::DISPLAY_FLIPDISC,
                    new SignCellNode(120, 80,
                            { std::make_tuple(new SignCellLeaf(20, 80), 0, 0),
                              std::make_tuple(new SignCellLeaf(100, 60), 20, 0)
                            }
                    ))
            }
    );

    // This will display the tree structure we just created.
    std::cout << *testSign << std::endl;

    SignRenderer r;
    Bitmap *result = r.render(testSign, 0);

    // delete testSign;


#ifdef GUI
    SiganimMainWindow m;
    m.show();
    ret = a.exec();
#endif

    return ret;
}
