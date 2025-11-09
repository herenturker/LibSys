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

#include "headers/AdminInterface.h"
#include "headers/AdminOperations.h"
#include "headers/LoginWindow.h"
#include "headers/TimeClass.h"
#include <QPushButton>
#include <QGuiApplication>
#include <QScreen>

AdminInterface::AdminInterface(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("LibSys Admin Dashboard");

    resize(1080, 720);
    setMinimumSize(1080, 720);
    setMaximumSize(1080, 720);

    dateLabel = new QLabel(this);
    dayLabel  = new QLabel(this);
    timeLabel = new QLabel(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &AdminInterface::updateDateTime);
    timer->start(1000);


    logHistory_Button = new QPushButton("Log History", this);
    books_Button = new QPushButton("Books", this);
    users_Button = new QPushButton("Users", this);

    addBook_Button = new QPushButton("Add Book", this);

    deleteBook_Button = new QPushButton("Delete Book", this);

    changeBookInfo_Button = new QPushButton("Change\nBook Info", this);

    confirmationRequests_Button = new QPushButton("Confirmation Requests", this);

    inquireBookSubmission_Button = new QPushButton("Book Submissions", this);

    reportLostBook_Button = new QPushButton("Report Lost Book", this);

    inquireBookRegistiration_Button = new QPushButton("Inquiry Book Registiration", this);

    unsigned short buttonWidth = 130;
    unsigned short buttonHeight = 50;
    unsigned short buttonSquare = 170;

    // TODO: Add go back button and go back to login window button

    logHistory_Button->setGeometry(860, 50, buttonWidth, buttonHeight);
    books_Button->setGeometry(860, 620, buttonWidth, buttonHeight);
    users_Button->setGeometry(710, 620, buttonWidth, buttonHeight);

    dateLabel->setGeometry(75, 650, 200, 30);
    dayLabel->setGeometry(75, 620, 200, 30);
    timeLabel->setGeometry(75, 590, 200, 30);

    addBook_Button->setGeometry(75, 215, buttonSquare, buttonSquare);

    deleteBook_Button->setGeometry(265, 215, buttonSquare, buttonSquare);

    changeBookInfo_Button->setGeometry(460, 215, buttonSquare, buttonSquare);

    confirmationRequests_Button->setGeometry(75, 140, 210, buttonHeight);

    inquireBookSubmission_Button->setGeometry(330, 50, 170, buttonHeight);

    reportLostBook_Button->setGeometry(265, 620, 160, buttonHeight);

    inquireBookRegistiration_Button->setGeometry(75, 50, 240, buttonHeight);

    dateLabel->setText("Date: " + TimeClass::showDate());
    dayLabel->setText("Day: " + TimeClass::showDay());
    timeLabel->setText("Time: " + TimeClass::showTime());

    QString buttonStyle = R"(
        QPushButton { 
            background-color: #424242; 
            color: white; 
            border-radius: 5px; 
            font-size: 18px;
        }
        QPushButton:hover { 
            background-color: #a3a3a3;
        }

    )";

        QString buttonStyle2 = R"(
        QPushButton { 
            background-color: #aa0000; 
            color: white; 
            border-radius: 5px; 
            font-size: 18px;
        }
        QPushButton:hover { 
            background-color: #6b0404;
        }
    )";

        QString buttonStyle3 = R"(
        QPushButton { 
            background-color: #18a30b; 
            color: white; 
            border-radius: 5px; 
            font-size: 18px;
        }
        QPushButton:hover { 
            background-color: #054e05;
        }
    )";

        QString buttonStyle4 = R"(
        QPushButton { 
            background-color: #0543d4; 
            color: white; 
            border-radius: 5px; 
            font-size: 18px;
        }
        QPushButton:hover { 
            background-color: #06045c;
        }
    )";

        QString buttonStyle5 = R"(
        QPushButton { 
            background-color: #babd04; 
            color: white; 
            border-radius: 5px; 
            font-size: 18px;
        }
        QPushButton:hover { 
            background-color: #534e05;
        }
    )";

        QString buttonStyle6 = R"(
        QPushButton { 
            background-color: #bd08bd; 
            color: white; 
            border-radius: 5px; 
            font-size: 18px;
        }
        QPushButton:hover { 
            background-color: #4d0153;
        }
    )";

    this->setStyleSheet(R"(
        QLabel#dateLabel, QLabel#dayLabel, QLabel#timeLabel {
            font-size: 20px;
            font-weight: bold;
            color: #333333;
        }
        )");

    dateLabel->setObjectName("dateLabel");
    dayLabel->setObjectName("dayLabel");
    timeLabel->setObjectName("timeLabel");

    logHistory_Button->setStyleSheet(buttonStyle5);

    books_Button->setStyleSheet(buttonStyle6);

    users_Button->setStyleSheet(buttonStyle);

    inquireBookSubmission_Button->setStyleSheet(buttonStyle);

    inquireBookRegistiration_Button->setStyleSheet(buttonStyle);

    confirmationRequests_Button->setStyleSheet(buttonStyle4);

    reportLostBook_Button->setStyleSheet(buttonStyle2);

    addBook_Button->setStyleSheet(buttonStyle3);

    deleteBook_Button->setStyleSheet(buttonStyle2);

    changeBookInfo_Button->setStyleSheet(buttonStyle4);

}

void AdminInterface::updateDateTime()
{
    dateLabel->setText("Date: " + TimeClass::showDate());
    dayLabel->setText("Day: " + TimeClass::showDay());
    timeLabel->setText("Time: " + TimeClass::showTime());
}
