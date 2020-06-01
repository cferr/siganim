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
#include <unicode/schriter.h>

#include "SignRenderer.h"
#include "../sign/Sign.h"
#include "../sign/SignImage.h"
#include "../sign/SignCellSplit.h"
#include "../sign/SignCellText.h"
#include "../font/Character.h"

SignRenderer::SignRenderer() : resultTree(NULL),
    resultBitmap(NULL) {

}

SignRenderer::~SignRenderer() {

}

Bitmap* SignRenderer::render(const Sign *s, unsigned int frame) {
    // Prepare s, if need be.

    // Render by visiting
    s->accept(*this);

    return this->resultBitmap;
}

void SignRenderer::visit(const Sign &s) {
    std::vector<SignDisplay*> displays = s.getDisplays();
    this->resultBitmap = new Bitmap(5 * s.getWidth(), 5 * s.getHeight());
    for (Sign::SignDisplayVectIt i = displays.begin(); i < displays.end();
            ++i) {
        (*i)->accept(*this);
        // set bitmap
        SignImage* displayImage = this->resultTree->compose();
        this->signImageToBitmap(this->resultBitmap, displayImage,
                (*i)->getDisplayType(), 0, 0);
        // TODO add display position coords
    }
}

void SignRenderer::visit(const SignDisplay &s) {
    (s.getRootCell())->accept(*this);
}

void SignRenderer::visit(const SignCellSplit &s) {

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

void SignRenderer::visit(const SignCellText &s) {
    SignImage *textRender = new SignImage(s.getWidth(), s.getHeight());

    icu::UnicodeString* text = s.getText();
    const Font* cellFont = s.getFont();
    if(cellFont != NULL) {
        unsigned int xx = 0;
        for(int32_t i = 0; i < text->length(); i++) {
            UChar32 unic = text->char32At(i);
            // Look up character in font,
            try {
                Character* c = cellFont->get(unic);
                // Render (mask ?).
                const enum Character::Bit* bmap = c->getMap();
                unsigned int charWidth = c->getWidth();
                unsigned int charHeight = c->getHeight();
                for(unsigned int x = 0; x < charWidth; ++x) {
                    for(unsigned int y = 0; y < charHeight; ++y) {
                        textRender->setPixel(xx + x, y,
                            (bmap[y * charWidth + x] == Character::Bit::ON)?
                            s.getForegroundColor():s.getBackgroundColor()
                        );
                    }
                }
                xx += charWidth;
            } catch(Font::CharNotFoundException& e) {

            }
        }
    }

    this->resultTree = new SignImageTree(textRender);
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

void SignRenderer::signImageToBitmap(Bitmap* dest, SignImage* source,
        DisplayType sourceType, unsigned int x, unsigned int y) {
    // TODO check for boundaries
    const SignColor* simgPixels = source->getPixels();
    struct Bitmap::pixel* imgPixels = dest->getPixels();

    const unsigned int simgHeight = source->getHeight();
    const unsigned int simgWidth = source->getWidth();

    const unsigned int dimgWidth = dest->getWidth();

    for(unsigned int i = 0; i < simgHeight; ++i) {
        for(unsigned int j = 0; j < simgWidth; ++j) {
            struct SignColor::RGB p = simgPixels[i*simgWidth+j].getValue(
                    sourceType);
            for(unsigned int ii = 0; ii < 5; ++ii) {
                for(unsigned int jj = 0; jj < 5; ++jj) {
                    imgPixels[(5*(i+y)+ii)*dimgWidth+5*(j+x)+jj] = {
                            p.r, p.g, p.b
                    };
                }
            }
        }
    }
}


