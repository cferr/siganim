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


#include "SignTreeDetailsWidget.h"
#include "pages/PageBlinkAnimation.h"
#include "pages/PageCompose.h"
#include "pages/PageDisplay.h"
#include "pages/PageFill.h"
#include "pages/PageMarqueeAnimation.h"
#include "pages/PageSign.h"
#include "pages/PageSplit.h"
#include "pages/PageText.h"

SignTreeDetailsWidget::SignTreeDetailsWidget() : currentWidget(nullptr) {
    this->layout = new QVBoxLayout();
}

void SignTreeDetailsWidget::update(SignTree *t) {
    t->callbackDispatch(this);
}

void SignTreeDetailsWidget::updateWidget(QWidget *widget) {
    if(this->currentWidget != nullptr) {
        this->layout->removeWidget(this->currentWidget);
        delete this->currentWidget;
    }
    this->currentWidget = widget;
    this->layout->addWidget(widget);
}

void SignTreeDetailsWidget::dispatchCallback(Sign &s) {
    this->updateWidget(new PageSign(&s));
}

void SignTreeDetailsWidget::dispatchCallback(Display &s) {
    this->updateWidget(new PageDisplay(&s));
}

void SignTreeDetailsWidget::dispatchCallback(Text &s) {
    this->updateWidget(new PageText(&s));
}

void SignTreeDetailsWidget::dispatchCallback(Split &s) {
    this->updateWidget(new PageSplit(&s));
}

void SignTreeDetailsWidget::dispatchCallback(MarqueeAnimation &s) {
    this->updateWidget(new PageMarqueeAnimation(&s));
}

void SignTreeDetailsWidget::dispatchCallback(BlinkAnimation &s) {
    this->updateWidget(new PageBlinkAnimation(&s));
}

void SignTreeDetailsWidget::dispatchCallback(Compose &s) {
    this->updateWidget(new PageCompose(&s));
}

void SignTreeDetailsWidget::dispatchCallback(Fill &s) {
    this->updateWidget(new PageFill(&s));
}
