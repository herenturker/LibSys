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


#include <QApplication>

#include "headers/ProgramInterface.h"
#include "headers/LoginWindow.h"

int main(int argc, char *argv[])
{
    QApplication libsys(argc, argv);

    libsys.setWindowIcon(QIcon("../icons/LibSys.ico"));

    LoginWindow loginWindow;
    ProgramInterface programInterface;

    libsys.setStyleSheet("QWidget { background-color: white; }");

    QObject::connect(&loginWindow, &LoginWindow::loginSuccess, [&programInterface](){
        programInterface.show();
    });

    loginWindow.show();


    return libsys.exec();
}
