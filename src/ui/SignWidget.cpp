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
#include "../sign/Sign.h"
#include "../render/SignRenderer.h"
#include "../sign/SignCellText.h"
#include "../font/parsers/GirouetteFontsParser.h"

SignWidget::SignWidget(Sign* sign, QWidget *parent) :
        QWidget(parent), sign(sign) {

    if(sign != NULL) {
        SignRenderer r;
        Bitmap *result = r.render(sign, 0);
        unsigned char* img = result->toRGB32();
        this->image = new QImage(img, result->getWidth(),
                result->getHeight(), QImage::Format_RGB32);

        this->update();
    }
}

SignWidget::~SignWidget() {

}

void SignWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawImage(this->image->rect(), *this->image, this->image->rect());
}

void SignWidget::signChangedEvent(Sign *s) {
    // Repaint ourselves according to that sign.

}
