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

#ifndef SRC_SIGNRENDERER_H_
#define SRC_SIGNRENDERER_H_

#include "Bitmap.h"
#include "Rasterizer.h"
#include "../font/FontSet.h"
#include "../sign/cells/Display.h"
#include "../sign/Sign.h"
#include "../sign/SignImage.h"
#include "../sign/SignTreeVisitor.h"

class SignRenderer {
private:
    // Tree structure to be created by a visitor of Sign
    class SignImageTree {
    public:
        struct SignImageTreeChild {
            SignImageTree* child;
            int x;
            int y;
        };
    private:
        union {
            struct {
                unsigned int width;
                unsigned int height;
                std::vector<SignImageTreeChild>* children;
            };
            SignImage* image;
        };
        enum Type {
            NODE, LEAF
        };
        enum Type type;
    public:
        SignImageTree(unsigned int width, unsigned int height,
                std::vector<struct SignImageTreeChild>* children);
        SignImageTree(SignImage* image);
        virtual ~SignImageTree();

        SignImage* compose();
    };



    // Renderer Visitor
    class SignRenderVisitor : public ConstSignTreeVisitor {
    private:
        const Rasterizer* rasterizer;
        SignImageTree* resultTree;
        Bitmap* resultBitmap;
        unsigned int frame;
        const FontSet* fontSet;

    public:
        SignRenderVisitor(const Rasterizer* rasterizer,
                const FontSet* fontSet,unsigned int frame) :
            rasterizer(rasterizer), resultTree(nullptr),
            resultBitmap(nullptr), frame(frame), fontSet(fontSet) {
        }

        virtual ~SignRenderVisitor();

        Bitmap* getBitmap() {
            return this->resultBitmap;
        }

        void visit(const Sign &s);
        void visit(const Display &s);
        void visit(const Split &s);
        void visit(const Text &s);
        void visit(const MarqueeAnimation &s);
        void visit(const BlinkAnimation &s);
        void visit(const Compose &s);
        void visit(const Fill &s);
    };


public:
    SignRenderer();
    Bitmap* render(const Rasterizer* rasterizer,
            const Sign *s, const FontSet* fontSet, unsigned int frame);

    virtual ~SignRenderer();
};

#endif /* SRC_SIGNRENDERER_H_ */
