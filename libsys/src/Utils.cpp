/*
    LibSys, Library Management System Software
    Copyright (C) 2025 Habil Eren Türker

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <QMessageBox>

#include "headers/Utils.h"

void showMessage(QWidget *parent, const QString &title, const QString &text, bool isError) {
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);

    QString backgroundColor = "#dadada";
    QString labelColor = isError ? "red" : "black";

    msgBox.setStyleSheet(
        QString(
            "QMessageBox { background-color: %1; color: black; }"
            "QLabel { color: %2; font-weight: bold; font-size: 14px; background-color: %1; }"
            "QPushButton { background-color: #8b8b8b; color: white; border-radius: 4px; padding: 5px; min-width: 80px; }"
            "QPushButton:hover { background-color: #5f5f5f; }"
            "QPushButton:pressed { background-color: #353535; }"
        ).arg(backgroundColor, labelColor)
    );

    msgBox.exec();
}
