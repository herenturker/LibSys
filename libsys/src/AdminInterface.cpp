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

#include <QVBoxLayout>
#include <QPushButton>

#include "headers/AdminInterface.h"
#include "headers/AdminOperations.h"

AdminInterface::AdminInterface(QWidget *parent) : ProgramInterface(parent)
{
    ProgramInterface::createWindowFrame();

    QWidget* cw = centralWidget();
    cw->setLayout(nullptr);

        QPushButton* logHistory_Button = new QPushButton("Log History", cw);
        QPushButton* books_Button = new QPushButton("Books", cw);
        QPushButton* users_Button = new QPushButton("Users", cw);

        logHistory_Button->setFixedSize(100, 30);
        books_Button->setFixedSize(70, 30);
        users_Button->setFixedSize(70, 30);

        logHistory_Button->setGeometry(50, 100, 150, 50);
        books_Button->setGeometry(250, 100, 150, 50);
        users_Button->setGeometry(150, 200, 150, 50);



        QString buttonStyle = R"(
            QPushButton { 
                background-color: #b4bccfff; 
                color: black; 
                border-radius: 5px; 
                padding: 6px 12px; 
                font-weight: bold; 
            }
            QPushButton:hover { 
                background-color: #67757aff;
            }
        )";
        logHistory_Button->setStyleSheet(buttonStyle);
        books_Button->setStyleSheet(buttonStyle);
        users_Button->setStyleSheet(buttonStyle);
    

    this->setStyleSheet(R"(
        QWidget { 
            background-color: #f5f5f5; 
        }
        QLabel { 
            color: #000000ff; 
            font-weight: bold; 
        }
    )");

    cw->update();
}
