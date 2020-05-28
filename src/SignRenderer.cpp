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

#include "SignRenderer.h"
#include "Sign.h"
#include "SignImage.h"
#include <unicode/schriter.h>
#include "SignCellSplit.h"
#include "SignCellText.h"

SignRenderer::SignRenderer() : resultTree(NULL),
    resultBitmap(NULL) {

}

SignRenderer::~SignRenderer() {

}

Bitmap* SignRenderer::render(Sign *s, unsigned int frame) {
    // Prepare s, if need be.

    // Render by visiting
    s->accept(*this);

    return this->resultBitmap;
}

void SignRenderer::visit(Sign &s) {
    std::vector<SignDisplay*> displays = s.getDisplays();
    this->resultBitmap = new Bitmap(s.getWidth(), s.getHeight());
    for (Sign::SignDisplayVectIt i = displays.begin(); i < displays.end();
            ++i) {
        (*i)->accept(*this);
        // set bitmap
        SignImage* displayImage = this->resultTree->compose();
        resultBitmap->addSignImage(displayImage, 0, 0); // TODO place displays!
    }
}

void SignRenderer::visit(SignDisplay &s) {
    // Keep this for later : default color for a given display
    // These colors should be used for cells, unless the cell uses
    // custom values.
//    SignRgbPixel foreground;
//    SignRgbPixel background;
//
//    switch (s.getDisplayType()) {
//    case SignPixelType::DISPLAY_FLIPDISC:
//        foreground = SignImage::defaultFlipDiscFG;
//        background = SignImage::defaultFlipDiscBG;
//        break;
//    case SignPixelType::DISPLAY_MONOCHROME_LED:
//        foreground = SignImage::defaultMonoLEDFG;
//        background = SignImage::defaultMonoLEDBG;
//        break;
//    case SignPixelType::DISPLAY_RGB_LED:
//        foreground = SignImage::defaultRGBLEDFG;
//        background = SignImage::defaultRGBLEDBG;
//        break;
//    }

    (s.getRootCell())->accept(*this);

}

void SignRenderer::visit(SignCellSplit &s) {

    std::vector<struct SignImageTree::SignImageTreeChild>* childrenImg
     = new std::vector<struct SignImageTree::SignImageTreeChild>();

    s.getTopOrLeftChild()->accept(*this);
    childrenImg->push_back({this->resultTree, 0, 0});
    s.getBottomOrRightChild()->accept(*this);
    switch(s.getSplitDirection()) {
    case SignCellSplit::SPLIT_HORIZONTAL:
        childrenImg->push_back({this->resultTree, 0, s.getSplitPos()});
        break;
    case SignCellSplit::SPLIT_VERTICAL:
        childrenImg->push_back({this->resultTree, s.getSplitPos(), 0});
        break;
    }
    SignImageTree* result = new SignImageTree(s.getWidth(), s.getHeight(),
            childrenImg);
    this->resultTree = result;
}

void SignRenderer::visit(SignCellText &s) {
    icu::UnicodeString* text = s.getText();
    for(int32_t i = 0; i < text->length(); i++) {
        UChar32 c = text->char32At(i);
        std::cout << "Char: " << c << std::endl;

        // Look up font (add support),
        // Look up character in font,
        // Render (mask ?).
    }

    // Temporary: fill with checkerboard
    SignImage *leafRender = new SignImage(s.getWidth(), s.getHeight());
    for(unsigned int x = 0; x < s.getWidth(); x++) {
        for(unsigned int y = x % 2; y < s.getHeight(); y += 2) {
            leafRender->setPixel(x, y, { 255, 255, 255 });
        }
    }
    this->resultTree = new SignImageTree(leafRender);
}

// Tree functions

SignRenderer::SignImageTree::SignImageTree(unsigned int width,
        unsigned int height,
        std::vector<struct SignImageTree::SignImageTreeChild>* children) :
            width(width), height(height), children(children), type(Type::NODE) {
}

SignRenderer::SignImageTree::SignImageTree(SignImage* image) : image(image),
        type(Type::LEAF) {

}

SignRenderer::SignImageTree::~SignImageTree() {
    switch(this->type) {
    case Type::NODE:
        for(auto i = this->children->begin();
                i < this->children->end(); ++i) {
            delete (*i).child;
        }
        delete this->children;
        break;
    case Type::LEAF:
        delete this->image;
        break;
    }
}

SignImage* SignRenderer::SignImageTree::compose() {
    if(this->type == Type::NODE) {
        SignImage* retImg = new SignImage(this->width, this->height);
        for(auto i = this->children->begin();
                i < this->children->end(); ++i) {
            struct SignImageTree::SignImageTreeChild childNode = *i;
            SignImage* childImg = childNode.child->compose();
            retImg->merge(childImg, childNode.x, childNode.y);
        }
        return retImg;
    } else {
        return this->image;
    }
}
