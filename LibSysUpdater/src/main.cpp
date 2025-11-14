/*
    LibSys Updater, Library Management System Software Updater
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

#include <QApplication>
#include <QIcon>

#include "headers/UpdaterWindow.h"

int main(int argc, char *argv[])
{
    QApplication LibSysUpdater(argc, argv);

    LibSysUpdater.setWindowIcon(QIcon(":/LibSysUpdater.ico"));

    LibSysUpdater.setStyleSheet("QWidget { background-color: #DFDEDE; }");

    UpdaterWindow window;
    window.show();

    return LibSysUpdater.exec();
}