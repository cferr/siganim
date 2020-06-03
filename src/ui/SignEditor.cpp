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

#include "SignEditor.h"

SignEditor::SignEditor(Sign* sign, Text* textCell) : textCell(textCell) {

    this->verticalLayout = new QVBoxLayout();

    this->signWidget = new SignWidget(sign);

    if(textCell != nullptr) {
        this->text = new QLineEdit();
        this->verticalLayout->addWidget(this->text);
        this->text->connect(this->text,
                &QLineEdit::textChanged,
                this,
                &SignEditor::updateSignText);
    }

    this->verticalLayout->addWidget(this->signWidget);

    // Set image widget as central widget
    this->setLayout(this->verticalLayout);

}

void SignEditor::updateSignText(const QString& text) {
    this->textCell->setText(icu::UnicodeString(text.toUtf8().data()));
}

