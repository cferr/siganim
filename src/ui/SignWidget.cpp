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

#include "SignWidget.h"
#include "../Sign.h"
#include "../SignCellLeaf.h"
#include "../SignRenderer.h"

#include <QRect>
#include <QPaintEvent>

SignWidget::SignWidget(QWidget *parent) :
        QWidget(parent), sign(NULL) {

    // Mock sign just to test.
    Sign *testSign = new Sign(120, 80,
            { new SignDisplay(120, 80, SignPixelType::DISPLAY_FLIPDISC,
                    new SignCellNode(120, 80,
                            { std::make_tuple(new SignCellLeaf(20, 80), 0, 0),
                              std::make_tuple(new SignCellLeaf(100, 60), 20, 0)
                            }
                    ))
            }
    );


    SignRenderer r;
    Bitmap *result = r.render(testSign, 0);

    unsigned char* img = result->toRGB32();

    // this->image = new QImage(200, 200, QImage::Format_RGB32);
    // this->image->fill(qRgb(255, 255, 255));
    // Yay, now we're generating it!
    this->image = new QImage(img, result->getWidth(),
            result->getHeight(), QImage::Format_RGB32);

    // No need to fill anymore.
    // QPainter painter(this->image);
    // painter.fillRect(QRect(0, 0, 100, 50), QColor::fromRgb(51, 102, 204));

    this->update();
}

SignWidget::~SignWidget() {

}

void SignWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, *this->image, dirtyRect);
}

void SignWidget::signChangedEvent(Sign *s) {
    // Repaint ourselves according to that sign.

}
