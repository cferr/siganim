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

#include <cassert>
#include <iostream>
#include <unicode/schriter.h>

#include "SignRenderer.h"
#include "../font/FontSet.h"
#include "../font/Character.h"
#include "../sign/Sign.h"
#include "../sign/SignImage.h"
#include "../sign/cells/MarqueeAnimation.h"
#include "../sign/cells/BlinkAnimation.h"
#include "../sign/cells/Compose.h"
#include "../sign/cells/Split.h"
#include "../sign/cells/Text.h"
#include "../sign/cells/Fill.h"

SignRenderer::SignRenderer() {

}

SignRenderer::~SignRenderer() {

}

Bitmap* SignRenderer::render(const Rasterizer* rasterizer, const Sign *s,
        const FontSet* fontSet, unsigned int frame, unsigned int scaleFactor) {
    // If this condition is not met, the renderer may work anyway.
    // It shouldn't (call on null pointer)
    assert(rasterizer != nullptr);
    // Prepare s, if need be.
    SignRenderVisitor visitor(rasterizer, fontSet, frame, scaleFactor);
    // Render by visiting
    s->accept(visitor);

    Bitmap* ret = visitor.getBitmap();
    return ret;
}

SignRenderer::SignRenderVisitor::~SignRenderVisitor() {
    delete this->resultTree;
}

void SignRenderer::SignRenderVisitor::visit(const Sign &s) {
    assert(this->resultTree == nullptr);
    std::vector<Display*> displays = s.getDisplays();
    // compute total width / height
//    unsigned int totalWidth = 0;
//    unsigned int totalHeight = 0;
//    for (Sign::SignDisplayVectIt i = displays.begin(); i < displays.end();
//            ++i) {
//        totalWidth += (*i)->getWidth();
//        totalHeight = std::max(totalHeight, (*i)->getHeight());
//    }

    //this->resultBitmap = new Bitmap(5 * totalWidth, 5 * totalHeight);
    unsigned int totalWidth = 0;
    unsigned int maxHeight = 0;
    std::vector<Bitmap*> bitmaps;
    for (Sign::SignDisplayVectIt i = displays.begin(); i < displays.end();
            ++i) {
        try {
            (*i)->accept(*this);
            // set bitmap
            SignImage* displayImage = this->resultTree->compose();
            Bitmap* bmap = this->rasterizer->rasterize(displayImage,
                    (*i)->getDisplayType(), this->scaleFactor);
            bitmaps.push_back(bmap);
            totalWidth += bmap->getWidth();
            maxHeight = std::max(maxHeight, bmap->getHeight());
            delete displayImage;
            delete this->resultTree;
            this->resultTree = nullptr;
        } catch(std::exception& e) {
            // Don't go any further with this display, but don't fail.
//            std::cout << "Exception caught on rendering Sign" << std::endl;
            assert(this->resultTree == nullptr);
        }
    }

    this->resultBitmap = new Bitmap(totalWidth, maxHeight);
    unsigned int x = 0;
    for(auto i = bitmaps.begin(); i < bitmaps.end(); ++i) {
        this->resultBitmap->overlay(*i, x, 0);
        x += (*i)->getWidth();
        delete *i;
    }
}

void SignRenderer::SignRenderVisitor::visit(const Display &s) {
    assert(this->resultTree == nullptr);
    SignImage* result = new SignImage(s.getWidth(), s.getHeight(),
            s.getWidth(), s.getHeight());
    try {
        (s.getRootCell())->accept(*this);
        SignImage* rootCellImage = this->resultTree->compose();
        SignImage* rootCellCropped = rootCellImage->cropToBox();
        result->merge(rootCellCropped, 0, 0);
        delete this->resultTree;
        delete rootCellImage;
        delete rootCellCropped;
    } catch(std::exception& e) {
//        std::cout << "Exception caught on rendering Display" << std::endl;
        assert(this->resultTree == nullptr);
    }

    this->resultTree = new SignImageTree(result);

}

void SignRenderer::SignRenderVisitor::visit(const Split &s) {
    assert(this->resultTree == nullptr);
    SignImage* composite = new SignImage(s.getWidth(), s.getHeight(),
            s.getWidth(), s.getHeight());

    try {
        s.getTopOrLeftChild()->accept(*this);
        SignImage* upper = this->resultTree->compose();
        SignImage* upperCropped = upper->cropToBox();
        composite->merge(upperCropped, 0, 0);

        delete this->resultTree;
        this->resultTree = nullptr;
        delete upper;
        delete upperCropped;
    } catch(std::exception& e) {
//        std::cout << "Exception caught on rendering Split 1" << std::endl;
        assert(this->resultTree == nullptr);
    }

    try {
        s.getBottomOrRightChild()->accept(*this);

        SignImage* lower = this->resultTree->compose();
        SignImage* lowerCropped = lower->cropToBox();
        const SignColor* lowerPixels = lowerCropped->getPixels();

        unsigned int boxWidth = s.getWidth();
        unsigned int boxHeight = s.getHeight();

        switch(s.getSplitDirection()) {
        case Split::SPLIT_HORIZONTAL:
            composite->merge(lowerCropped, 0, s.getSplitPos());
            break;
        case Split::SPLIT_VERTICAL:
            composite->merge(lowerCropped, s.getSplitPos(), 0);
            break;
        case Split::SPLIT_NW_SE_DIAGONAL:
            for(unsigned int x = 0; x < boxWidth; ++x) {
                for(unsigned int y = boxHeight-1;
                        (int)(y * boxWidth) >= (int)(x * boxHeight); --y) {
                    composite->setPixel(x, y, lowerPixels[y * boxWidth + x]);
                }
            }
            break;
        case Split::SPLIT_SW_NE_DIAGONAL:
            for(unsigned int x = 0; x < boxWidth; ++x) {
                for(unsigned int y = boxHeight-1;
                    (int)(x * boxHeight + y * boxWidth)
                            >= (int)(boxHeight * boxWidth); --y) {
                    composite->setPixel(x, y, lowerPixels[y * boxWidth + x]);
                }
            }
            break;
        }
        delete this->resultTree;
        this->resultTree = nullptr;
        delete lower;
        delete lowerCropped;
    } catch(std::exception& e) {
//        std::cout << "Exception caught on rendering Split 2" << std::endl;
        assert(this->resultTree == nullptr);
    }
    this->resultTree = new SignImageTree(composite);

}

void SignRenderer::SignRenderVisitor::visit(const Text &s) {
    assert(this->resultTree == nullptr);
    uint32_t cellHeight = s.getHeight();
    uint32_t cellWidth = s.getWidth();

    SignColor off = SignColor::off();

    icu::UnicodeString* text = s.getText();
    try {
        const Font* cellFont = this->fontSet->lookup(s.getFontFamily(),
            s.getFontStyle());
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
                s.getWidth(), s.getHeight());

        // Compute biggest char's vertical position
        int32_t yy = 0;
        switch(s.getVAlign()) {
        case Text::VALIGN_TOP_TOP:
        case Text::VALIGN_TOP_CENTER:
        case Text::VALIGN_TOP_BOTTOM:
            yy = 0;
            break;

        case Text::VALIGN_CENTER_TOP:
        case Text::VALIGN_CENTER_CENTER:
        case Text::VALIGN_CENTER_BOTTOM:
            yy = ((int32_t)cellHeight - (int32_t)maxHeight) / 2;
            break;

        case Text::VALIGN_BOTTOM_TOP:
        case Text::VALIGN_BOTTOM_CENTER:
        case Text::VALIGN_BOTTOM_BOTTOM:
            yy = (int32_t)cellHeight - (int32_t)maxHeight;
            break;
        }

        int32_t xx = 0;
        switch(s.getHAlign()) {
        case Text::HALIGN_LEFT:
        case Text::HALIGN_JUSTIFY:
            xx = 0;
            break;
        case Text::HALIGN_CENTER:
            xx = ((int32_t)actualWidth - (int32_t)totalWidth) / 2;
            break;
        case Text::HALIGN_RIGHT:
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

                int32_t yyy = 0;
                switch(s.getVAlign()) {
                case Text::VALIGN_TOP_TOP:
                case Text::VALIGN_CENTER_TOP:
                case Text::VALIGN_BOTTOM_TOP:
                    yyy = 0;
                    break;

                case Text::VALIGN_TOP_CENTER:
                case Text::VALIGN_CENTER_CENTER:
                case Text::VALIGN_BOTTOM_CENTER:
                    yyy = ((int32_t)maxHeight - (int32_t)charHeight) / 2;
                    break;

                case Text::VALIGN_TOP_BOTTOM:
                case Text::VALIGN_CENTER_BOTTOM:
                case Text::VALIGN_BOTTOM_BOTTOM:
                    yyy = (int32_t)maxHeight - (int32_t)charHeight;
                    break;
                }

                for(int x = 0; (unsigned int)x < charWidth; ++x) {
                    for(int y = 0; (unsigned int)y < charHeight; ++y) {
                        textRender->setPixel(xx + x, y + yy + yyy,
                            (bmap[y * charWidth + x] == Character::Bit::ON)?
                            s.getForegroundColor():off
                        );
                    }
                }

                switch(s.getHAlign()) {
                case Text::HALIGN_LEFT:
                case Text::HALIGN_CENTER:
                case Text::HALIGN_RIGHT:
                    xx += charWidth;
                    break;
                case Text::HALIGN_JUSTIFY:
                    xx += charWidth + justifiedSpaceBetweenChars;
                    break;
                }

            } catch(Font::CharNotFoundException& e) {

            }
        }
        this->resultTree = new SignImageTree(textRender);
    } catch(FontSet::FontNotFoundException& e) {
//        std::cout << "Exception caught on rendering Text" << std::endl;
        assert(this->resultTree == nullptr);
        // Return an empty image of the right height and width
        this->resultTree = new SignImageTree(
                new SignImage(s.getWidth(), s.getHeight(),
                        s.getWidth(), s.getHeight()));
    }
    delete text;
}

void SignRenderer::SignRenderVisitor::visit(const MarqueeAnimation &s) {
    assert(this->resultTree == nullptr);
    try {
        // Render the child
        s.getSubject()->accept(*this);
        SignImage* subjectImage = this->resultTree->compose();

        // Preserve background color (will be default in case the subject image
        // is already a composite).
        SignImage *animRender = new SignImage(s.getWidth(), s.getHeight(),
            s.getWidth(), s.getHeight(), subjectImage->getBackgroundColor());

        enum MarqueeAnimation::Direction direction = s.getDirection();

        unsigned int duration = s.getDurationFrames();
        unsigned int phase = s.getInitialPhaseFrames();
        unsigned int frame = ((this->frame + phase) % duration);
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

        delete subjectImage;
    } catch(std::exception& e) {
//        std::cout << "Exception caught on rendering Marquee" << std::endl;
        assert(this->resultTree == nullptr);

        this->resultTree = new SignImageTree(new SignImage(s.getWidth(),
                s.getHeight(), s.getWidth(), s.getHeight()));
    }
}

void SignRenderer::SignRenderVisitor::visit(const BlinkAnimation &s) {
    assert(this->resultTree == nullptr);
    try {
        s.getSubject()->accept(*this);
        unsigned int framesOn = s.getFramesOn();
        unsigned int framesOff = s.getFramesOff();
        unsigned int phase = s.getInitialPhaseFrames();

        if((this->frame + phase) % (framesOn + framesOff) > framesOn) {
            SignImage* empty = new SignImage(s.getWidth(), s.getHeight(),
                    s.getWidth(), s.getHeight());
            delete this->resultTree;
            this->resultTree = new SignImageTree(empty);
        }
    } catch(std::exception& e) {
//        std::cout << "Exception caught on rendering Blink" << std::endl;
        assert(this->resultTree == nullptr);

        SignImage* empty = new SignImage(s.getWidth(), s.getHeight(),
                s.getWidth(), s.getHeight());
        this->resultTree = new SignImageTree(empty);
    }
}

void SignRenderer::SignRenderVisitor::visit(const Compose &s) {
    assert(this->resultTree == nullptr);
    std::vector<struct SignImageTree::SignImageTreeChild>* childrenImg
         = new std::vector<struct SignImageTree::SignImageTreeChild>();

    try {
        s.getBackground()->accept(*this);
        childrenImg->push_back({this->resultTree, 0, 0});
        // Do not delete the result tree yet
        this->resultTree = nullptr;
    } catch(std::exception& e) {
//        std::cout << "Exception caught on rendering Compose 1" << std::endl;
        assert(this->resultTree == nullptr);
    }


    try {
        s.getForeground()->accept(*this);
        childrenImg->push_back({this->resultTree, 0, 0});
        this->resultTree = nullptr;
    } catch(std::exception& e) {
//        std::cout << "Exception caught on rendering Compose 2" << std::endl;
        assert(this->resultTree == nullptr);
    }

    this->resultTree = new SignImageTree(s.getWidth(), s.getHeight(),
            childrenImg);
}

void SignRenderer::SignRenderVisitor::visit(const Fill &s) {
    assert(this->resultTree == nullptr);
    SignImage *animRender = new SignImage(s.getWidth(), s.getHeight(),
            s.getWidth(), s.getHeight(), s.getColor());
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
        SignImage* retImg = new SignImage(this->width, this->height,
                this->width, this->height);
        for(auto i = this->children->begin();
                i < this->children->end(); ++i) {
            struct SignImageTree::SignImageTreeChild childNode = *i;
            SignImage* childImg = childNode.child->compose();
            SignImage* childImgCropped = childImg->cropToBox();
            retImg->merge(childImg, childNode.x, childNode.y);
            delete childImg;
            delete childImgCropped;
        }
        return retImg;
    } else {
        return new SignImage(this->image);
    }
}

