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

#include "SignDisplay.h"
#include "SignImage.h"

SignDisplay::SignDisplay(unsigned int height, unsigned int width,
			enum SignPixelType type) : SignDisplay(height, width, type, NULL)
{

}

SignDisplay::SignDisplay(unsigned int height, unsigned int width,
		enum SignPixelType type, SignCellTree* rootCell)
		: height(height), width(width), type(type), rootCell(rootCell) {

}

SignDisplay::~SignDisplay() {

}

SignTreeType SignDisplay::getType() const {
	return SignTreeType::SIGN_DISPLAY;
}

void SignDisplay::accept(SignTreeVisitor& visitor) {
	visitor.visit(*this);
}

bool SignDisplay::setParent(const SignTree* parent) {
	if(parent != NULL) {
		if(parent->getType() == SignTreeType::SIGN) {
			this->parent = parent;
			return true;
		}
		return false;
	} else {
		this->parent = parent;
		return true;
	}

}

bool SignDisplay::setRootCell(SignCellTree* rootCell) {
	this->rootCell = rootCell;
	return true;
}

SignCellTree* SignDisplay::getRootCell() {
	return this->rootCell;
}

void SignDisplay::render(Bitmap* dest, unsigned int frame) {
	// TODO move this to visitor SignRenderer
	SignRgbPixel foreground;
	SignRgbPixel background;

	switch(this->type) {
	case SignPixelType::DISPLAY_FLIPDISC:
		foreground = SignImage::defaultFlipDiscFG;
		background = SignImage::defaultFlipDiscBG;
		break;
	case SignPixelType::DISPLAY_MONOCHROME_LED:
		foreground = SignImage::defaultMonoLEDFG;
		background = SignImage::defaultMonoLEDBG;
		break;
	case SignPixelType::DISPLAY_RGB_LED:
		foreground = SignImage::defaultRGBLEDFG;
		background = SignImage::defaultRGBLEDBG;
		break;
	}

	SignImage* interior = new SignImage(this->height, this->width);
	rootCell->render(interior, frame, 0, 0, background, foreground);

	// TODO do something with dest
}

std::ostream& SignDisplay::serialize(std::ostream &strm) const {
	return strm << "Disp { " << this->height << "x"
			<< this->width << " (" << this->type << ") : "
			<< *(this->rootCell) << " }";
}

std::ostream& operator<<(std::ostream& strm, const SignDisplay &s) {
	return s.serialize(strm);
}
