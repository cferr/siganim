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

#include <QRect>
#include <QPaintEvent>
#include "SignWidget.h"
#include "../render/SignRenderer.h"

SignWidget::SignWidget(Sign* sign, const FontSet* fontSet,
        const Rasterizer* rasterizer, QWidget *parent) :
        QWidget(parent), sign(sign), image(nullptr), pixelData(nullptr) {
    this->sink = new AnimatedObservableSink(sign, fontSet, rasterizer);
    this->sink->attach(this);
}

SignWidget::~SignWidget() {
    if(this->image != nullptr)
        delete this->image;
    this->sink->detach(this);
    if(this->sink != nullptr)
        delete this->sink;
}

void SignWidget::paintEvent(QPaintEvent *event) {
    if(this->image != nullptr) {
        QPainter painter(this);
        painter.drawImage(event->rect(),
                *this->image, this->image->rect());
    }
}

void SignWidget::signChangedEvent() {
    // Repaint ourselves according to that sign.
    if(sign != nullptr) {
        Bitmap* result = this->sink->getFrame();
        if(this->pixelData != nullptr)
            free(this->pixelData);
        this->pixelData = result->toRGB32();
        if(this->image != nullptr)
            delete this->image;
        this->image = new QImage(this->pixelData, result->getWidth(),
                result->getHeight(), QImage::Format_RGB32);
        this->setMinimumHeight(result->getHeight());
        this->setMinimumWidth(result->getWidth());
        this->setMaximumHeight(result->getHeight());
        this->setMaximumWidth(result->getWidth());
        this->update();
    } else {
        this->setFixedHeight(0);
        this->setFixedWidth(0);
    }
}

void SignWidget::observe(const Observable *sender) {
    if(sender == this->sink) {
        this->signChangedEvent();
    }
}

void SignWidget::setRasterizer(const Rasterizer *rasterizer) {
    this->sink->setRasterizer(rasterizer);
}
