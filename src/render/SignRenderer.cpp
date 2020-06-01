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
#include <numeric>
#include <unicode/schriter.h>

#include "SignRenderer.h"
#include "../sign/Sign.h"
#include "../sign/SignImage.h"
#include "../sign/SignCellSplit.h"
#include "../sign/SignCellText.h"
#include "../sign/MarqueeAnimation.h"
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
    SignImage *textRender = new SignImage(s.getWidth(), s.getHeight());

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
        // Compute biggest char's vertical position
        uint32_t yy;
        switch(s.getVAlign()) {
        case SignCellText::VALIGN_TOP_TOP:
        case SignCellText::VALIGN_TOP_CENTER:
        case SignCellText::VALIGN_TOP_BOTTOM:
            yy = 0;
            break;

        case SignCellText::VALIGN_CENTER_TOP:
        case SignCellText::VALIGN_CENTER_CENTER:
        case SignCellText::VALIGN_CENTER_BOTTOM:
            yy = (cellHeight - maxHeight) / 2;
            break;

        case SignCellText::VALIGN_BOTTOM_TOP:
        case SignCellText::VALIGN_BOTTOM_CENTER:
        case SignCellText::VALIGN_BOTTOM_BOTTOM:
            yy = cellHeight - maxHeight;
            break;
        }

        uint32_t xx;
        switch(s.getHAlign()) {
        case SignCellText::HALIGN_LEFT:
        case SignCellText::HALIGN_JUSTIFY:
            xx = 0;
            break;
        case SignCellText::HALIGN_CENTER:
            if(cellWidth >= totalWidth)
                xx = (cellWidth - totalWidth) / 2;
            else
                xx = 0;
            break;
        case SignCellText::HALIGN_RIGHT:
            if(cellWidth >= totalWidth)
                xx = cellWidth - totalWidth;
            else
                xx = 0;
            break;
        }

        uint32_t justifiedSpaceBetweenChars = 0;
        if(nbNonVoidChars > 1)
            justifiedSpaceBetweenChars =
                    (cellWidth - totalWidth) / (nbNonVoidChars - 1);

        for(int32_t i = 0; i < text->length(); ++i) {
            UChar32 unic = text->char32At(i);
            // Look up character in font,
            try {
                Character* c = cellFont->get(unic);
                // Render (mask ?).
                const enum Character::Bit* bmap = c->getMap();
                unsigned int charWidth = c->getWidth();
                unsigned int charHeight = c->getHeight();

                uint32_t yyy;
                switch(s.getVAlign()) {
                case SignCellText::VALIGN_TOP_TOP:
                case SignCellText::VALIGN_CENTER_TOP:
                case SignCellText::VALIGN_BOTTOM_TOP:
                    yyy = 0;
                    break;

                case SignCellText::VALIGN_TOP_CENTER:
                case SignCellText::VALIGN_CENTER_CENTER:
                case SignCellText::VALIGN_BOTTOM_CENTER:
                    yyy = (maxHeight - charHeight) / 2;
                    break;

                case SignCellText::VALIGN_TOP_BOTTOM:
                case SignCellText::VALIGN_CENTER_BOTTOM:
                case SignCellText::VALIGN_BOTTOM_BOTTOM:
                    yyy = maxHeight - charHeight;
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
    }

    this->resultTree = new SignImageTree(textRender);
}

void SignRenderer::SignRenderVisitor::visit(const MarqueeAnimation &s) {
    SignImage *animRender = new SignImage(s.getWidth(), s.getHeight());
    // Render the child
    s.getSubject()->accept(*this);
    SignImage* subjectImage = this->resultTree->compose();

    enum MarqueeAnimation::Direction direction = s.getDirection();

    unsigned int duration = s.getDurationFrames();
    unsigned int frame = (this->frame % duration);
    unsigned int totalSpace = s.getWidth() + s.getSpace();
    unsigned int textWidth = s.getWidth();

    int dirM = (direction == MarqueeAnimation::LEFT)?-1:1;

    int leftPosition = (-1) * dirM * (int)textWidth + dirM * (
        ((int)totalSpace * (int)frame) / ((int)duration));

    // Merge right part of the image
    animRender->merge(subjectImage, leftPosition - totalSpace, 0);
    // Merge center part of the image
    animRender->merge(subjectImage, leftPosition, 0);
    // Merge right part of the image
    animRender->merge(subjectImage, leftPosition + totalSpace, 0);

    this->resultTree = new SignImageTree(animRender);
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

// Duration computer

void SignRenderer::DurationComputerVisitor::visit(const Sign &s) {
    unsigned int signTotalFrames = 1;
    std::vector<SignDisplay*> displays = s.getDisplays();
    for(auto i = displays.begin(); i < displays.end(); ++i) {
        (*i)->accept(*this);
        unsigned int displayTotal = this->totalFrames;
        // Eclipse doesn't handle this LCM which is from C++17.
        signTotalFrames = std::lcm<unsigned int, unsigned int> // @suppress("Function cannot be resolved") // @suppress("Symbol is not resolved")
            (signTotalFrames, displayTotal);
    }
    this->totalFrames = signTotalFrames;
}

void SignRenderer::DurationComputerVisitor::visit(const SignDisplay &s) {
    s.getRootCell()->accept(*this);
}

void SignRenderer::DurationComputerVisitor::visit(const SignCellSplit &s) {
    s.getTopOrLeftChild()->accept(*this);
    unsigned int topLeftFrames = this->totalFrames;
    s.getBottomOrRightChild()->accept(*this);
    this->totalFrames = std::lcm<unsigned int, unsigned int> // @suppress("Function cannot be resolved") // @suppress("Symbol is not resolved")
        (this->totalFrames, topLeftFrames);
}

void SignRenderer::DurationComputerVisitor::visit(const SignCellText &s) {
    this->totalFrames = 1;
}

void SignRenderer::DurationComputerVisitor::visit(const MarqueeAnimation &s) {
    s.getSubject()->accept(*this);
    this->totalFrames = std::lcm<unsigned int, unsigned int> // @suppress("Function cannot be resolved") // @suppress("Symbol is not resolved")
        (this->totalFrames, s.getDurationFrames());
}

unsigned int SignRenderer::computeTotalFrames(const Sign *s) {
    DurationComputerVisitor visitor;
    s->accept(visitor);

    return visitor.getTotalFrames();
}
