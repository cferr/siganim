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
#include "../sign/Sign.h"
#include "../render/SignRenderer.h"

#include <QRect>
#include <QPaintEvent>
#include "../sign/SignCellText.h"

SignWidget::SignWidget(QWidget *parent) :
        QWidget(parent), sign(NULL) {

    // Mock sign just to test.
    Sign *testSign = new Sign(120, 80,
            { new SignDisplay(120, 80, DisplayType::DISPLAY_MONOCHROME_LED,
                    new SignCellSplit(SignCellSplit::SPLIT_VERTICAL, 39,
                            new SignCellText("40"),
                            new SignCellText("METRO TIMONE")
                    ))
            }
    );

    std::cout << *testSign << std::endl;


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
//    QRect dirtyRect = event->rect();
    painter.drawImage(this->image->rect(), *this->image, this->image->rect());
}

void SignWidget::signChangedEvent(Sign *s) {
    // Repaint ourselves according to that sign.

}
