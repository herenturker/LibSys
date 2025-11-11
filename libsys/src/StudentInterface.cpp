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

#include <QPushButton>
#include <QGuiApplication>
#include <QScreen>
#include <QTimer>

#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStyle>
#include <QIcon>

#include "headers/StudentInterface.h"
#include "headers/StudentOperations.h"
#include "headers/LoginWindow.h"
#include "headers/TimeClass.h"
#include "headers/BookSearchWindow.h"


StudentInterface::StudentInterface(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("LibSys Student Dashboard");

    resize(1080, 720);
    setMinimumSize(1080, 720);
    setMaximumSize(1080, 720);

    dateLabel = new QLabel(this);
    dayLabel  = new QLabel(this);
    timeLabel = new QLabel(this);

    overdueBooks = new QLabel("Overdue Books", this);
    unreturnedBooks = new QLabel("Unreturned Books", this);
    borrowedBooks = new QLabel("Borrowed Books", this);


    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &StudentInterface::updateDateTime);
    timer->start(1000);

    dateLabel->setGeometry(75, 650, 200, 30);
    dayLabel->setGeometry(75, 620, 200, 30);
    timeLabel->setGeometry(75, 590, 200, 30);

    borrowedBooks->setGeometry(75, 160, 190, 40);
    unreturnedBooks->setGeometry(75, 310, 190, 40);
    overdueBooks->setGeometry(75, 460, 190, 40);

    overdueBooks->setObjectName("overdueBooks");
    unreturnedBooks->setObjectName("unreturnedBooks");
    borrowedBooks->setObjectName("borrowedBooks");


    dateLabel->setObjectName("dateLabel");
    dayLabel->setObjectName("dayLabel");
    timeLabel->setObjectName("timeLabel");

    bookSearchWindow = new BookSearchWindow(this);

    openButton = new QPushButton("Open Book Search", this);
    openButton->setGeometry(500, 40, 150, 40);

    bookSearchWindow->close();

    connect(openButton, &QPushButton::clicked, [=]() {

        if (bookSearchWindow->isVisible()) {
            bookSearchWindow->close();
        } else {
            bookSearchWindow->show();
        }
    });

    searchContainer = new QWidget(this);
    searchContainer->setGeometry(75, 40, 400, 40);

    displayBookList = new QPushButton("Books", this);
    displayBookList->setToolTip("Browse all available books in the library.");

    unsigned short buttonWidth = 130;
    unsigned short buttonHeight = 50;
    // unsigned short buttonSquare = 170;

    displayBookList->setGeometry(840, 640, buttonWidth, buttonHeight);

    QHBoxLayout *searchLayout = new QHBoxLayout(searchContainer);
    searchLayout->setContentsMargins(0, 0, 0, 0);


    QToolButton *searchButton = new QToolButton(searchContainer);
    searchButton->setIcon(QIcon(":/magnifier.ico"));
    searchButton->setIconSize(QSize(24, 24));
    searchButton->setCursor(Qt::PointingHandCursor);
    searchButton->setToolTip("Show search filters");

    QLineEdit *searchEdit = new QLineEdit(searchContainer);
    searchEdit->setPlaceholderText("Search for books, authors, or topics...");
    searchEdit->setMinimumHeight(32);

    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(searchEdit);

    searchButton->setObjectName("searchButton");
    searchEdit->setObjectName("searchEdit");
    openButton->setObjectName("bookSearchButton");

        this->setStyleSheet(R"(
            QLabel#dateLabel, QLabel#dayLabel, QLabel#timeLabel {
                font-size: 20px;
                font-weight: bold;
                color: #333333;
            }

            QToolButton#searchButton, QLineEdit#searchEdit {
                color: black;
                font-size: 14px;
            }

            QPushButton#bookSearchButton {

                color: black;
                font-size: 14px;
                border: 1px solid black;
                border-radius: 4px;
                background-color: #e9e9e9;
            
            }

            QPushButton#bookSearchButton:hover {
                border: 1px solid black;
                background-color: #a8a3a3;
            }

            QPushButton#bookSearchButton:pressed {
                border: 1px solid black;
                background-color: #555252;
            }

            QLabel#overdueBooks, QLabel#unreturnedBooks, QLabel#borrowedBooks {
                font-size: 20px;
                font-weight: italic;
                color: #333333;
                border: 1px solid black;
            }

        )");

    QString buttonStyle = R"(
        QPushButton { 
            background-color: #f1b5f1; 
            color: white; 
            border-radius: 5px; 
            font-size: 18px;
        }
        QPushButton:hover { 
            background-color: #9c859e;
        }
    )";

        displayBookList->setStyleSheet(buttonStyle);

}

void StudentInterface::updateDateTime()
{
    dateLabel->setText("Date: " + TimeClass::showDate());
    dayLabel->setText("Day: " + TimeClass::showDay());
    timeLabel->setText("Time: " + TimeClass::showTime());
}
