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

#include "SiganimDefaults.h"

SignColor SiganimDefaults::defaultFlipDiscBG =
        SignColor(SignColor::ON, 100, 100, 100);
SignColor SiganimDefaults::defaultFlipDiscFG =
        SignColor(SignColor::OFF, 255, 255, 0);
SignColor SiganimDefaults::defaultMonoLEDBG =
        SignColor(SignColor::ON, 100, 100, 100);
SignColor SiganimDefaults::defaultMonoLEDFG =
        SignColor(SignColor::OFF, 255, 127, 0);
SignColor SiganimDefaults::defaultRGBLEDBG =
        SignColor(SignColor::ON, 0, 0, 0);
SignColor SiganimDefaults::defaultRGBLEDFG =
        SignColor(SignColor::OFF, 255, 255, 255);
Rasterizer SiganimDefaults::defaultRasterizer =
        Rasterizer("Default", 5, 5);

SiganimDefaults::SiganimDefaults() {
}
