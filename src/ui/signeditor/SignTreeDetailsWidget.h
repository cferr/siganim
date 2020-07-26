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

#ifndef UI_SIGNTREEDETAILSWIDGET_H_
#define UI_SIGNTREEDETAILSWIDGET_H_

#include <QWidget>
#include <QVBoxLayout>
#include "../../font/FontSet.h"
#include "../../sign/SignTree.h"

class SignTreeDetailsWidget: public QWidget, public SignTreeDispatcher {
    Q_OBJECT

private:
    QVBoxLayout *layout;
    QWidget *currentWidget;
    const FontSet* fontSet;

    void updateWidget(QWidget *widget);

public:
    SignTreeDetailsWidget(const FontSet* fontSet);
    virtual ~SignTreeDetailsWidget() { }

    void update(SignTree* t);
    void updateEmpty();

    void dispatchCallback(Sign& s);
    void dispatchCallback(Display& s);
    void dispatchCallback(Text& s);
    void dispatchCallback(Split& s);
    void dispatchCallback(MarqueeAnimation& s);
    void dispatchCallback(BlinkAnimation& s);
    void dispatchCallback(Compose& s);
    void dispatchCallback(Fill& s);

};

#endif /* UI_SIGNTREEDETAILSWIDGET_H_ */
