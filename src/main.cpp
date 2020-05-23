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

#ifdef GUI
#include <QApplication>
#endif
#include <iostream>
#include "Sign.h"
#include "SignCellNode.h"
#include "SignCellLeaf.h"

#ifdef GUI
#include "SiganimMainWindow.h"
#endif

int main(int argc, char* argv[]) {

	int ret = 0;

#ifdef GUI
	QApplication a(argc, argv);

	SiganimMainWindow m;
	m.show();

	ret = a.exec();

#endif

	// Are we able to create a sign easily? Yes.
	Sign* testSign = new Sign(80, 120, {
			new SignDisplay(80, 120, SignPixelType::DISPLAY_FLIPDISC,
					new SignCellNode(80, 120, {
							std::make_tuple(new SignCellLeaf(80, 20), 0, 0),
							std::make_tuple(new SignCellLeaf(80, 100), 20, 0)
					})
			)
	});

	// This will display the tree structure we just created.
	std::cout << *testSign << std::endl;
	delete testSign;

	return ret;
}
