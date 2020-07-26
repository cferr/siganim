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

#ifndef SRC_SIGNWIDGET_H_
#define SRC_SIGNWIDGET_H_

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QObject>
#include "../../font/FontSet.h"
#include "../../render/AnimatedObservableSink.h"
#include "../../sign/Sign.h"
#include "../../sign/Observer.h"
#include "../../render/Rasterizer.h"

class SignWidget: public QWidget, public Observer {
    Q_OBJECT
protected:
    Sign *sign;
    AnimatedObservableSink* sink;

private:
    QImage *image;
    unsigned char* pixelData;

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    SignWidget(Sign* sign, const FontSet* fontSet,
            const Rasterizer* rasterizer, QWidget *parent = nullptr);
    virtual ~SignWidget();

    void signChangedEvent();
    void observe(const Observable* sender);

    void setRasterizer(const Rasterizer* rasterizer);
};

#endif /* SRC_SIGNWIDGET_H_ */
