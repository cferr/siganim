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

#include <iostream>

#include "Sign.h"
#include "SignCellText.h"
#include "SignRenderer.h"

bool UTF8Test() {
    std::cout << "// UTF-8 Test" << std::endl;
    // Sign with a UTF-8 symbol inside.

    // Are we able to create a sign easily? Yes.
    Sign *UTF8Test = new Sign(120, 80,
            { new SignDisplay(120, 80, SignPixelType::DISPLAY_FLIPDISC,
                    new SignCellSplit(SignCellSplit::SPLIT_VERTICAL, 20,
                            new SignCellText("☢"),
                            new SignCellText("")
                    ))
            }
    );

    // This will display the tree structure we just created.
    std::cout << *UTF8Test << std::endl;

    SignRenderer r;
    Bitmap *result = r.render(UTF8Test, 0);

    delete result;
    delete UTF8Test;

    return true;

}

// Test bench
bool siganimTests() {
    bool ret = true;
    std::cout << "// Begin Siganim tests" << std::endl;
    ret &= UTF8Test();

    std::cout << "// End Siganim tests" << std::endl;
    return ret;
}

