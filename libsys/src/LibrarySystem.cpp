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

#include <string>

#include <QString>
#include <QFile>
#include <QApplication>

#include "headers/LibrarySystem.h"

std::string LibrarySystem::rfid_data;

QString LibrarySystem::ArduinoCOMPort = "";

QString LibrarySystem::theme = ":/themes/light.qss";

void LibrarySystem::updateRFIDDataValue(const QString &RFIDdata)
{
    rfid_data = QStringTostdString(RFIDdata);
}

void LibrarySystem::toggleTheme(int theme)
{
    if (theme == 1)
        LibrarySystem::theme = ":/themes/light.qss";
    else // 2, for example
        LibrarySystem::theme = ":/themes/dark.qss";

    QFile file(LibrarySystem::theme);
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QString qss = file.readAll();
        qApp->setStyleSheet(qss);
        file.close();
    }
}
