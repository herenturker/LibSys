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
#include <QDir>

#include "headers/StudentInterface.h"
#include "headers/StudentOperations.h"
#include "headers/LoginWindow.h"
#include "headers/TimeClass.h"
#include "headers/BookSearchWindow.h"
#include "headers/GeneralOperations.h"
#include "headers/database.h"
#include "headers/LibrarySystem.h"
#include "headers/Graphical.h"

StudentInterface::StudentInterface(QWidget *parent) : QWidget(parent)
{
    QString exePath = QCoreApplication::applicationDirPath();
    QString dbDirPath = exePath + "/databases";

    QDir().mkpath(dbDirPath);

    QString userdbPath = dbDirPath + "/users.db";
    QString librarydbPath = dbDirPath + "/library.db";

    userDb = new Database(userdbPath, "DB_USERS");
    libraryDb = new Database(librarydbPath, "DB_LIBRARY");

    setWindowTitle("LibSys Student Dashboard");

    resize(1080, 720);
    setMinimumSize(1080, 720);
    setMaximumSize(1080, 720);

    dateLabel = new QLabel(this);
    dayLabel = new QLabel(this);
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

    connect(openButton, &QPushButton::clicked, [=]()
            {

        if (bookSearchWindow->isVisible()) {
            bookSearchWindow->close();
        } else {
            bookSearchWindow->show();
        } });

    searchContainer = new QWidget(this);
    searchContainer->setGeometry(75, 40, 400, 40);

    borrowBook_Button = new QPushButton("Borrow", this);
    borrowBook_Button->setToolTip("Borrow a book from the library.");

    returnBook_Button = new QPushButton("Return", this);
    returnBook_Button->setToolTip("Return a book to the library.");

    myAccount_Button = new QPushButton("Account", this);
    myAccount_Button->setToolTip("Display account settings");

    unsigned short buttonWidth = 130;
    unsigned short buttonHeight = 50;
    // unsigned short buttonSquare = 170;

    borrowBook_Button->setGeometry(540, 640, buttonWidth, buttonHeight);
    returnBook_Button->setGeometry(680, 640, buttonWidth, buttonHeight);
    myAccount_Button->setGeometry(820, 640, buttonWidth, buttonHeight);

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

    connect(searchButton, &QToolButton::clicked, [=](){
        QString bookTitle = bookSearchWindow->bookTitle->text();
        QString author1 = bookSearchWindow->author1->text();
        QString author2 = bookSearchWindow->author2->text();
        QString author3 = bookSearchWindow->author3->text();
        QString author4 = bookSearchWindow->author4->text();
        QString author5 = bookSearchWindow->author5->text();
        QString publisher = bookSearchWindow->publisher->text();
        QString publicationYear = bookSearchWindow->publicationYear->text();
        QString edition = bookSearchWindow->edition->text();
        QString ISBN = bookSearchWindow->ISBN->text();
        QString volume = bookSearchWindow->volume->text();
        QString pageCount = bookSearchWindow->pageCount->text();
        QString seriesInformation = bookSearchWindow->seriesInformation->text();
        QString language = bookSearchWindow->language->text();
        QString DDC = bookSearchWindow->DDC->text();
        QString additionalInfo = bookSearchWindow->additionalInfo->toPlainText();

        GeneralOperations generalOperations(libraryDb);

        QList<LibrarySystem::Book> results = generalOperations.searchBook(
            bookTitle, author1, author2, author3, author4, author5,
            publisher, publicationYear, edition, ISBN,
            volume, pageCount, seriesInformation, language, DDC, additionalInfo
        );

        Graphical searchResults;
        searchResults.displayBooksWithFilters(this, results);

    });

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

            QPushButton#bookSearchButton, QToolButton#searchButton {

                color: black;
                font-size: 14px;
                border: 1px solid black;
                border-radius: 4px;
                background-color: #e9e9e9;
            
            }

            QPushButton#bookSearchButton:hover, QToolButton#searchButton:hover {
                border: 1px solid black;
                background-color: #a8a3a3;
            }

            QPushButton#bookSearchButton:pressed, QToolButton#searchButton:pressed {
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
            background-color: #a3a3a3; 
            color: white; 
            border-radius: 5px; 
            font-size: 18px;
            border: 1px solid #000000;
        }
        QPushButton:hover { 
            background-color: #727272;
            border: 1px solid #000000;
        }
        QPushButton:pressed { 
            background-color: #424242;
            border: 1px solid #000000;
        }
    )";

    borrowBook_Button->setStyleSheet(buttonStyle);
    returnBook_Button->setStyleSheet(buttonStyle);
    myAccount_Button->setStyleSheet(buttonStyle);
}

void StudentInterface::updateDateTime()
{
    dateLabel->setText("Date: " + TimeClass::showDate());
    dayLabel->setText("Day: " + TimeClass::showDay());
    timeLabel->setText("Time: " + TimeClass::showTime());
}
