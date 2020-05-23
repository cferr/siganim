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

#ifndef SRC_SIGNCELLLEAF_H_
#define SRC_SIGNCELLLEAF_H_

#include <iostream>
#include <unicode/utypes.h>
#include <unicode/unistr.h>

#include "SignCellTree.h"
#include "SignImage.h"

class SignCellLeaf : public SignCellTree {
protected:
	bool checkResize() const override;

private:
	icu::UnicodeString text; // Unicode-encoded text

public:
	SignCellLeaf(unsigned int height, unsigned int width);
	virtual ~SignCellLeaf();

	SignTreeType getType() const;
	void accept(SignTreeVisitor& visitor);

	void setText(const char* text);

	void render(SignImage* target, const unsigned int frame,
			const unsigned int x, const unsigned int y, SignRgbPixel background,
			SignRgbPixel foreground) const;

	std::ostream& serialize(std::ostream &strm) const;

};

std::ostream& operator<<(std::ostream &strm, const SignCellLeaf &s);

#endif /* SRC_SIGNCELLLEAF_H_ */
