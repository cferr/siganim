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
#include "../sign/MarqueeAnimation.h"
#include "../sign/BlinkAnimation.h"
#include "../font/Character.h"

SignRenderer::SignRenderer() {

}

SignRenderer::~SignRenderer() {

}

Bitmap* SignRenderer::render(const Sign *s, unsigned int frame) {
    // Prepare s, if need be.
    SignRenderVisitor visitor(*this, frame);
    // Render by visiting
    s->accept(visitor);

    return visitor.getBitmap();
}

void SignRenderer::SignRenderVisitor::visit(const Sign &s) {
    std::vector<SignDisplay*> displays = s.getDisplays();
    this->resultBitmap = new Bitmap(5 * s.getWidth(), 5 * s.getHeight());
    for (Sign::SignDisplayVectIt i = displays.begin(); i < displays.end();
            ++i) {
        (*i)->accept(*this);
        // set bitmap
        SignImage* displayImage = this->resultTree->compose();
        this->rendererInstance.signImageToBitmap(this->resultBitmap,
                displayImage, (*i)->getDisplayType(), 0, 0);
        // TODO add display position coords
    }
}

void SignRenderer::SignRenderVisitor::visit(const SignDisplay &s) {
    (s.getRootCell())->accept(*this);
}

void SignRenderer::SignRenderVisitor::visit(const SignCellSplit &s) {

    std::vector<struct SignImageTree::SignImageTreeChild>* childrenImg
     = new std::vector<struct SignImageTree::SignImageTreeChild>();

    s.getTopOrLeftChild()->accept(*this);
    childrenImg->push_back({this->resultTree, 0, 0});
    s.getBottomOrRightChild()->accept(*this);
    switch(s.getSplitDirection()) {
    case SignCellSplit::SPLIT_HORIZONTAL:
        childrenImg->push_back({this->resultTree, 0, (int)s.getSplitPos()});
        break;
    case SignCellSplit::SPLIT_VERTICAL:
        childrenImg->push_back({this->resultTree, (int)s.getSplitPos(), 0});
        break;
    }
    SignImageTree* result = new SignImageTree(s.getWidth(), s.getHeight(),
            childrenImg);
    this->resultTree = result;
}

void SignRenderer::SignRenderVisitor::visit(const SignCellText &s) {
    uint32_t cellHeight = s.getHeight();
    uint32_t cellWidth = s.getWidth();

    icu::UnicodeString* text = s.getText();
    const Font* cellFont = s.getFont();
    if(cellFont != nullptr) {
        // Compute min, max height of all characters.
        uint32_t minHeight = UINT_LEAST32_MAX;
        uint32_t maxHeight = 0;
        uint32_t minWidth = UINT_LEAST32_MAX;
        uint32_t maxWidth = 0;
        uint32_t totalWidth = 0;
        uint32_t nbNonVoidChars = 0;
        for(int32_t i = 0; i < text->length(); ++i) {
            UChar32 unic = text->char32At(i);
            try {
                Character* c = cellFont->get(unic);
                uint32_t cHeight = c->getHeight();
                uint32_t cWidth = c->getWidth();
                minHeight = std::min(minHeight, cHeight);
                maxHeight = std::max(maxHeight, cHeight);
                minWidth = std::min(minWidth, cWidth);
                maxWidth = std::max(maxWidth, cWidth);
                totalWidth += cWidth;
                ++nbNonVoidChars;
            } catch(Font::CharNotFoundException& e) { }
        }
        // How wide do we need to be?
        uint32_t actualWidth = std::max(cellWidth, totalWidth);
        SignImage *textRender = new SignImage(actualWidth, s.getHeight(),
                s.getWidth(), s.getHeight(), s.getBackgroundColor());

        // Compute biggest char's vertical position
        int32_t yy;
        switch(s.getVAlign()) {
        case SignCellText::VALIGN_TOP_TOP:
        case SignCellText::VALIGN_TOP_CENTER:
        case SignCellText::VALIGN_TOP_BOTTOM:
            yy = 0;
            break;

        case SignCellText::VALIGN_CENTER_TOP:
        case SignCellText::VALIGN_CENTER_CENTER:
        case SignCellText::VALIGN_CENTER_BOTTOM:
            yy = ((int32_t)cellHeight - (int32_t)maxHeight) / 2;
            break;

        case SignCellText::VALIGN_BOTTOM_TOP:
        case SignCellText::VALIGN_BOTTOM_CENTER:
        case SignCellText::VALIGN_BOTTOM_BOTTOM:
            yy = (int32_t)cellHeight - (int32_t)maxHeight;
            break;
        }

        int32_t xx;
        switch(s.getHAlign()) {
        case SignCellText::HALIGN_LEFT:
        case SignCellText::HALIGN_JUSTIFY:
            xx = 0;
            break;
        case SignCellText::HALIGN_CENTER:
            xx = ((int32_t)actualWidth - (int32_t)totalWidth) / 2;
            break;
        case SignCellText::HALIGN_RIGHT:
            xx = (int32_t)actualWidth - (int32_t)totalWidth;
            break;
        }

        uint32_t justifiedSpaceBetweenChars = 0;
        if(nbNonVoidChars > 1)
            justifiedSpaceBetweenChars =
                    (actualWidth - totalWidth) / (nbNonVoidChars - 1);

        for(int32_t i = 0; i < text->length(); ++i) {
            UChar32 unic = text->char32At(i);
            // Look up character in font,
            try {
                Character* c = cellFont->get(unic);
                // Render (mask ?).
                const enum Character::Bit* bmap = c->getMap();
                unsigned int charWidth = c->getWidth();
                unsigned int charHeight = c->getHeight();

                int32_t yyy;
                switch(s.getVAlign()) {
                case SignCellText::VALIGN_TOP_TOP:
                case SignCellText::VALIGN_CENTER_TOP:
                case SignCellText::VALIGN_BOTTOM_TOP:
                    yyy = 0;
                    break;

                case SignCellText::VALIGN_TOP_CENTER:
                case SignCellText::VALIGN_CENTER_CENTER:
                case SignCellText::VALIGN_BOTTOM_CENTER:
                    yyy = ((int32_t)maxHeight - (int32_t)charHeight) / 2;
                    break;

                case SignCellText::VALIGN_TOP_BOTTOM:
                case SignCellText::VALIGN_CENTER_BOTTOM:
                case SignCellText::VALIGN_BOTTOM_BOTTOM:
                    yyy = (int32_t)maxHeight - (int32_t)charHeight;
                    break;
                }

                for(unsigned int x = 0; x < charWidth; ++x) {
                    for(unsigned int y = 0; y < charHeight; ++y) {
                        textRender->setPixel(xx + x, y + yy + yyy,
                            (bmap[y * charWidth + x] == Character::Bit::ON)?
                            s.getForegroundColor():s.getBackgroundColor()
                        );
                    }
                }

                switch(s.getHAlign()) {
                case SignCellText::HALIGN_LEFT:
                case SignCellText::HALIGN_CENTER:
                case SignCellText::HALIGN_RIGHT:
                    xx += charWidth;
                    break;
                case SignCellText::HALIGN_JUSTIFY:
                    xx += charWidth + justifiedSpaceBetweenChars;
                    break;
                }

            } catch(Font::CharNotFoundException& e) {

            }
        }
        this->resultTree = new SignImageTree(textRender);
    } else {
        // Return an empty image of the right height and width
        this->resultTree = new SignImageTree(
                new SignImage(s.getWidth(), s.getHeight(),
                        s.getWidth(), s.getHeight()));
    }
}

void SignRenderer::SignRenderVisitor::visit(const MarqueeAnimation &s) {

    // Render the child
    s.getSubject()->accept(*this);
    SignImage* subjectImage = this->resultTree->compose();

    // Preserve background color (will be default in case the subject image
    // is already a composite).
    SignImage *animRender = new SignImage(s.getWidth(), s.getHeight(),
        s.getWidth(), s.getHeight(), subjectImage->getBackgroundColor());

    enum MarqueeAnimation::Direction direction = s.getDirection();

    unsigned int duration = s.getDurationFrames();
    unsigned int frame = (this->frame % duration);
    unsigned int totalSpace = subjectImage->getWidth() + s.getSpace();
    unsigned int subjectWidth = subjectImage->getWidth();

    int dirM = (direction == MarqueeAnimation::LEFT)?-1:1;

    int leftPosition = (-1) * dirM * (int)subjectWidth + dirM * (
        ((int)totalSpace * (int)frame) / ((int)duration));

    // Merge right part of the image
    animRender->merge(subjectImage, leftPosition - totalSpace, 0);
    // Merge center part of the image
    animRender->merge(subjectImage, leftPosition, 0);
    // Merge right part of the image
    animRender->merge(subjectImage, leftPosition + totalSpace, 0);

    delete this->resultTree;
    this->resultTree = new SignImageTree(animRender);
}

void SignRenderer::SignRenderVisitor::visit(const BlinkAnimation &s) {
    s.getSubject()->accept(*this);
    unsigned int framesOn = s.getFramesOn();
    unsigned int framesOff = s.getFramesOff();

    if(this->frame % (framesOn + framesOff) > framesOn) {
        SignImage* empty = new SignImage(s.getWidth(), s.getHeight(),
                s.getWidth(), s.getHeight());
        delete this->resultTree;
        this->resultTree = new SignImageTree(empty);
    }
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
        SignImage* retImg = new SignImage(this->width, this->height,
                this->width, this->height);
        for(auto i = this->children->begin();
                i < this->children->end(); ++i) {
            struct SignImageTree::SignImageTreeChild childNode = *i;
            SignImage* childImg = childNode.child->compose()->cropToBox();
            retImg->merge(childImg, childNode.x, childNode.y);
        }
        return retImg;
    } else {
        return this->image;
    }
}

void SignRenderer::signImageToBitmap(Bitmap* dest, SignImage* source,
        DisplayType sourceType, unsigned int x, unsigned int y) {
    // Crop the result into its own box, in case it hasn't been done before.
    SignImage* sourceCropped = source->cropToBox();

    const SignColor* simgPixels = sourceCropped->getPixels();
    struct Bitmap::pixel* imgPixels = dest->getPixels();

    const unsigned int simgHeight = sourceCropped->getHeight();
    const unsigned int simgWidth = sourceCropped->getWidth();

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
