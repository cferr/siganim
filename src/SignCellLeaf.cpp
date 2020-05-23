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

#include "SignCellLeaf.h"

SignCellLeaf::SignCellLeaf(unsigned int height,
		unsigned int width)
	: SignCellTree(height, width) {

}

SignCellLeaf::~SignCellLeaf() {

}

SignTreeType SignCellLeaf::getType() const {
	return SignTreeType::SIGN_CELL_LEAF;
}

void SignCellLeaf::accept(SignTreeVisitor& visitor) {
	visitor.visit(*this);
}

bool SignCellLeaf::checkResize() const {
	return true;
}

void SignCellLeaf::render(SignImage* target, const unsigned int frame,
		const unsigned int x, const unsigned int y, SignRgbPixel background,
		SignRgbPixel foreground) const {
	// TODO move this to visitor SignRenderer
}

std::ostream& SignCellLeaf::serialize(std::ostream &strm) const {
	return strm << "{ " << this->height << "x" <<
			this->width << " }";
}

std::ostream& operator<<(std::ostream &strm, const SignCellLeaf &s) {
	return s.serialize(strm);
}
