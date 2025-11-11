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
#include <QFile>
#include <QMessageBox>

#include "headers/AdminInterface.h"
#include "headers/AdminOperations.h"
#include "headers/LoginWindow.h"
#include "headers/TimeClass.h"
#include "headers/BookSearchWindow.h"
#include "headers/Database.h"


AdminInterface::AdminInterface(QWidget *parent) : QWidget(parent), 
    userDb(QCoreApplication::applicationDirPath() + "/users.db", "DB_USERS"),
      libraryDb(QCoreApplication::applicationDirPath() + "/library.db", "DB_LIBRARY")
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
    logHistory_Button->setToolTip("View the system's activity and log history.");

    books_Button = new QPushButton("Books", this);
    books_Button->setToolTip("Browse all available books in the library.");

    users_Button = new QPushButton("Users", this);
    users_Button->setToolTip("View and manage registered users.");

    addBook_Button = new QPushButton("Add Book", this);
    addBook_Button->setToolTip("Add a new book to the library database.");

    deleteBook_Button = new QPushButton("Delete Book", this);
    deleteBook_Button->setToolTip("Remove a book from the library database.");

    changeBookInfo_Button = new QPushButton("Update\nBook Info", this);
    changeBookInfo_Button->setToolTip("Edit or update details of an existing book.");

    confirmationRequests_Button = new QPushButton("Confirmation Requests", this);
    confirmationRequests_Button->setToolTip("Review and manage pending confirmation requests.");

    inquireBookSubmission_Button = new QPushButton("Book Submissions", this);
    inquireBookSubmission_Button->setToolTip("Check the submission status of borrowed books.");

    reportLostBook_Button = new QPushButton("Report Lost Book", this);
    reportLostBook_Button->setToolTip("Report a lost book and update its record.");

    inquireBookRegistiration_Button = new QPushButton("Inquiry Book Registiration", this);
    inquireBookRegistiration_Button->setToolTip("Check whether a book is registered to a user and view its details.");

    backToLoginWindow_Button = new QPushButton("Return to\nLogin", this);
    backToLoginWindow_Button->setToolTip("Return to the login window.");

    addUser_Button =  new QPushButton("Add User", this);
    addUser_Button->setToolTip("Add a new user to the users database.");

    deleteUser_Button = new QPushButton("Delete User", this);
    deleteUser_Button->setToolTip("Delete an user from the users database.");

    updateUserInfo_Button = new QPushButton("Update User Info", this);
    updateUserInfo_Button->setToolTip("Update or edit an user's info.");

    punishUser_Button = new QPushButton("Punish User", this);
    punishUser_Button->setToolTip("Punish an user in LibSys.");


    unsigned short buttonWidth = 130;
    unsigned short buttonHeight = 50;
    unsigned short buttonSquare = 170;

    // TODO: Add go back button

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

    backToLoginWindow_Button->setGeometry(860, 140, buttonWidth, buttonHeight);


    addUser_Button->setGeometry(75, 410, buttonSquare, buttonSquare);

    deleteUser_Button->setGeometry(265, 410, buttonSquare, buttonSquare);

    updateUserInfo_Button->setGeometry(460, 410, buttonSquare, buttonSquare);

    punishUser_Button->setGeometry(655, 410, buttonSquare, buttonSquare);


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

    backToLoginWindow_Button->setStyleSheet(buttonStyle);

    addUser_Button->setStyleSheet(buttonStyle);

    deleteUser_Button->setStyleSheet(buttonStyle);

    updateUserInfo_Button->setStyleSheet(buttonStyle);

    punishUser_Button->setStyleSheet(buttonStyle);

    // CONNECTIONS

    QString exePath = QCoreApplication::applicationDirPath();
    QString userdbPath = exePath + "/users.db";
    QString librarydbPath = exePath + "/library.db";

    if (!QFile::exists(userdbPath)) {
        QFile file(userdbPath);
        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "Could not create \"users.db\" !";
        } else {
            file.close();
            qDebug() << "Created \"users.db\" .";
        }
    }

    if (!QFile::exists(librarydbPath)) {
        QFile file(librarydbPath);
        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "Could not create \"library.db\" !";
        } else {
            file.close();
            qDebug() << "Created \"library.db\" .";
        }
    }


    if (!userDb.openDB()) {
        QMessageBox::critical(this, "Error", "Could not open the database!");
        return;
    }

    if (!libraryDb.openDB()) {
        QMessageBox::critical(this, "Error", "Could not open the database!");
        return;
    }


    userDb.createUsersTable();
    libraryDb.createBooksTable();

    bookSearchWindow = new BookSearchWindow(this);

    connect(addBook_Button, &QPushButton::clicked, [&](){
        bookSearchWindow->setMode(BookSearchWindow::Add);
        bookSearchWindow->show();
        bookSearchWindow->raise();
        bookSearchWindow->activateWindow();
    });

    connect(bookSearchWindow, &BookSearchWindow::bookAddDataReady,
            [&](const QString &bookTitle,
                const QString &author1,
                const QString &author2,
                const QString &author3,
                const QString &author4,
                const QString &author5,
                const QString &publisher,
                const QString &publicationYear,
                const QString &edition,
                const QString &ISBN,
                const QString &volume,
                const QString &pageCount,
                const QString &seriesInformation,
                const QString &language,
                const QString &DDC,
                const QString &additionalInfo
            ) 
    {
        bool success = libraryDb.addBook(bookTitle, author1, author2, author3, author4, author5,
                        publisher, publicationYear, edition, ISBN, volume,
                        pageCount, seriesInformation, language, DDC, additionalInfo);

        if (!success) {
            QMessageBox::warning(this, "Error", "Could not add book to database!");
        } else {
            bookSearchWindow->hide();
        }
    });

    connect(deleteBook_Button, &QPushButton::clicked, [&](){
        bookSearchWindow->setMode(BookSearchWindow::Delete);
        bookSearchWindow->show();
        bookSearchWindow->raise();
        bookSearchWindow->activateWindow();
    });

    connect(bookSearchWindow, &BookSearchWindow::bookDeleteDataReady,
            [&](const QString &bookTitle,
                const QString &author1,
                const QString &ISBN
            ) 
    {
        bool success = libraryDb.deleteBook(bookTitle, author1, ISBN);

        if (!success) {
            QMessageBox::warning(this, "Error", "Could not delete book!");
        } else {
            bookSearchWindow->hide();
        }
    });

    connect(changeBookInfo_Button, &QPushButton::clicked, [&](){
        bookSearchWindow->setMode(BookSearchWindow::Update);
        bookSearchWindow->show();
        bookSearchWindow->raise();
        bookSearchWindow->activateWindow();
    });

    connect(bookSearchWindow, &BookSearchWindow::bookUpdateDataReady,
            [&](const QString &bookTitle,
                const QString &author1,
                const QString &author2,
                const QString &author3,
                const QString &author4,
                const QString &author5,
                const QString &publisher,
                const QString &publicationYear,
                const QString &edition,
                const QString &ISBN,
                const QString &volume,
                const QString &pageCount,
                const QString &seriesInformation,
                const QString &language,
                const QString &DDC,
                const QString &additionalInfo
            ) 
    {
        bool success = libraryDb.updateBook(bookTitle, author1, author2, author3, author4, author5,
                        publisher, publicationYear, edition, ISBN, volume,
                        pageCount, seriesInformation, language, DDC, additionalInfo);

        if (!success) {
            QMessageBox::warning(this, "Error", "Could not update book info!");
        } else {
            bookSearchWindow->hide();
        }
    });


}

void AdminInterface::updateDateTime()
{
    dateLabel->setText("Date: " + TimeClass::showDate());
    dayLabel->setText("Day: " + TimeClass::showDay());
    timeLabel->setText("Time: " + TimeClass::showTime());
}
