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
#include <QDir>
#include <QList>
#include <QPlainTextEdit>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHeaderView>

#include "headers/AdminInterface.h"
#include "headers/LoginWindow.h"
#include "headers/TimeClass.h"
#include "headers/BookSearchWindow.h"
#include "headers/Database.h"
#include "headers/Graphical.h"
#include "headers/Utils.h"
#include "headers/LibrarySystem.h"
#include "headers/GeneralOperations.h"

AdminInterface::AdminInterface(QWidget *parent) : QWidget(parent)

{
    QString exePath = QCoreApplication::applicationDirPath();
    QString dbDirPath = exePath + "/databases";

    QDir().mkpath(dbDirPath);

    QString userdbPath = dbDirPath + "/users.db";
    QString librarydbPath = dbDirPath + "/library.db";

    userDb = new Database(userdbPath, "DB_USERS");
    libraryDb = new Database(librarydbPath, "DB_LIBRARY");

    setWindowTitle("LibSys Admin Dashboard");

    resize(1080, 720);
    setMinimumSize(1080, 720);
    setMaximumSize(1080, 720);

    dateLabel = new QLabel(this);
    dayLabel = new QLabel(this);
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

    QLabel *adminDashboard = new QLabel("Admin\nDashboard", this);
    adminDashboard->setStyleSheet("QLabel { color: black; font-size: 48pt; font-weight: bold; }");
    adminDashboard->move(75, 30);
    adminDashboard->resize(500, 170);

    // confirmationRequests_Button = new QPushButton("Confirmation Requests", this);
    // confirmationRequests_Button->setToolTip("Review and manage pending confirmation requests.");

    // inquireBookSubmission_Button = new QPushButton("Book Submissions", this);
    // inquireBookSubmission_Button->setToolTip("Check the submission status of borrowed books.");

    reportLostBook_Button = new QPushButton("Report Lost Book", this);
    reportLostBook_Button->setToolTip("Report a lost book and update its record.");

    // inquireBookRegistiration_Button = new QPushButton("Inquiry Book Registiration", this);
    // inquireBookRegistiration_Button->setToolTip("Check whether a book is registered to a user and view its details.");

    // backToLoginWindow_Button = new QPushButton("Return to\nLogin", this);
    // backToLoginWindow_Button->setToolTip("Return to the login window.");

    addUser_Button = new QPushButton("Add User", this);
    addUser_Button->setToolTip("Add a new user to the users database.");

    deleteUser_Button = new QPushButton("Delete User", this);
    deleteUser_Button->setToolTip("Delete an user from the users database.");

    updateUserInfo_Button = new QPushButton("Update User Info", this);
    updateUserInfo_Button->setToolTip("Update or edit an user's info.");

    // punishUser_Button = new QPushButton("Punish User", this);
    // punishUser_Button->setToolTip("Punish an user in LibSys.");

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

    // confirmationRequests_Button->setGeometry(75, 140, 210, buttonHeight);

    // inquireBookSubmission_Button->setGeometry(330, 50, 170, buttonHeight);

    reportLostBook_Button->setGeometry(265, 620, 160, buttonHeight);

    // inquireBookRegistiration_Button->setGeometry(75, 50, 240, buttonHeight);

    // backToLoginWindow_Button->setGeometry(860, 140, buttonWidth, buttonHeight);

    addUser_Button->setGeometry(75, 410, buttonSquare, buttonSquare);

    deleteUser_Button->setGeometry(265, 410, buttonSquare, buttonSquare);

    updateUserInfo_Button->setGeometry(460, 410, buttonSquare, buttonSquare);

    // punishUser_Button->setGeometry(655, 410, buttonSquare, buttonSquare);

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

    logHistory_Button->setStyleSheet(buttonStyle);

    books_Button->setStyleSheet(buttonStyle);

    users_Button->setStyleSheet(buttonStyle);

    // inquireBookSubmission_Button->setStyleSheet(buttonStyle);

    // inquireBookRegistiration_Button->setStyleSheet(buttonStyle);

    // confirmationRequests_Button->setStyleSheet(buttonStyle);

    reportLostBook_Button->setStyleSheet(buttonStyle);

    addBook_Button->setStyleSheet(buttonStyle);

    deleteBook_Button->setStyleSheet(buttonStyle);

    changeBookInfo_Button->setStyleSheet(buttonStyle);

    // backToLoginWindow_Button->setStyleSheet(buttonStyle);

    addUser_Button->setStyleSheet(buttonStyle);

    deleteUser_Button->setStyleSheet(buttonStyle);

    updateUserInfo_Button->setStyleSheet(buttonStyle);

    // punishUser_Button->setStyleSheet(buttonStyle);

    // CONNECTIONS

    if (!QFile::exists(userdbPath))
    {
        QFile file(userdbPath);
        if (!file.open(QIODevice::WriteOnly))
        {
            //  qDebug() << "Could not create \"users.db\" !";
        }
        else
        {
            file.close();
            //  qDebug() << "Created \"users.db\" .";
        }
    }

    if (!QFile::exists(librarydbPath))
    {
        QFile file(librarydbPath);
        if (!file.open(QIODevice::WriteOnly))
        {
            //  qDebug() << "Could not create \"library.db\" !";
        }
        else
        {
            file.close();
            //  qDebug() << "Created \"library.db\" .";
        }
    }

    if (!userDb->openDB())
    {
        QMessageBox::critical(this, "Error", "Could not open the database!");
        return;
    }

    if (!libraryDb->openDB())
    {
        QMessageBox::critical(this, "Error", "Could not open the database!");
        return;
    }

    userDb->createUsersTable();
    libraryDb->createBooksTable();

    bookSearchWindow = new BookSearchWindow(this);

    connect(addBook_Button, &QPushButton::clicked, [&]()
            {
        bookSearchWindow->setMode(BookSearchWindow::Add);
        bookSearchWindow->show();
        bookSearchWindow->raise();
        bookSearchWindow->activateWindow();
         });

         

    connect(bookSearchWindow, &BookSearchWindow::bookAddDataReady,
            [&](const QString &bookTitle,
                const QString &author1,
                const QString &publisher,
                const QString &publicationYear,
                const QString &edition,
                const QString &ISBN,
                const QString &volume,
                const QString &pageCount,
                const QString &seriesInformation,
                const QString &language,
                const QString &DDC,
                const QString &additionalInfo)
            {
                bool success = libraryDb->addBook(this, bookTitle, author1,
                                                  publisher, publicationYear, edition, ISBN, volume,
                                                  pageCount, seriesInformation, language, DDC, additionalInfo);

                if (!success)
                {
                    showMessage(bookSearchWindow, "Error", "Could not add book to database!", true);
                }
                else
                {
                    bookSearchWindow->hide();
                    std::string logString = "ADD BOOK:  " + ISBN.toStdString();
                    writeEncryptedLog(logString);
                    showMessage(bookSearchWindow, "Success", "Added new book!", false);
                }
            });

    connect(deleteBook_Button, &QPushButton::clicked, [&]()
            {
        bookSearchWindow->setMode(BookSearchWindow::Delete);
        bookSearchWindow->show();
        bookSearchWindow->raise();
        bookSearchWindow->activateWindow(); });

    connect(bookSearchWindow, &BookSearchWindow::bookDeleteDataReady,
            [&](const QString &bookTitle,
                const QString &author1,
                const QString &ISBN)
            {
                bool success = libraryDb->deleteBook(this, bookTitle, author1, ISBN);

                if (!success)
                {
                    showMessage(this, "Error", "Could not delete book!", true);
                }
                else
                {
                    bookSearchWindow->hide();
                    std::string logString = "DELETE BOOK:  " + ISBN.toStdString();
                    writeEncryptedLog(logString);
                    showMessage(this, "Success", "Deleted the book!", false);
                }
            });

    connect(changeBookInfo_Button, &QPushButton::clicked, [&]()
            {
        bookSearchWindow->setMode(BookSearchWindow::Update);
        bookSearchWindow->show();
        bookSearchWindow->raise();
        bookSearchWindow->activateWindow(); });

    connect(bookSearchWindow, &BookSearchWindow::bookUpdateDataReady,
            [&](const QString &bookTitle,
                const QString &author1,
                const QString &publisher,
                const QString &publicationYear,
                const QString &edition,
                const QString &ISBN,
                const QString &volume,
                const QString &pageCount,
                const QString &seriesInformation,
                const QString &language,
                const QString &DDC,
                const QString &additionalInfo)
            {
                bool success = libraryDb->updateBook(this, bookTitle, author1,
                                                     publisher, publicationYear, edition, ISBN, volume,
                                                     pageCount, seriesInformation, language, DDC, additionalInfo);

                if (!success)
                {
                    showMessage(this, "Error", "Could not update book info!", true);
                }
                else
                {
                    bookSearchWindow->hide();
                    std::string logString = "UPDATE BOOK INFO:  " + ISBN.toStdString();
                    writeEncryptedLog(logString);
                    showMessage(this, "Success", "Updated the book info!", false);
                }
            });

    connect(addUser_Button, &QPushButton::clicked, [=]()
            {
                Graphical graphicalAddUser(this);
                bool success = graphicalAddUser.addUserGraphical(this);

                if (!success)
                {
                    showMessage(this, "Error", "Could not add new user!", true);
                }
                else
                {
                    std::string logString = "ADDED NEW USER";
                    writeEncryptedLog(logString);
                    showMessage(this, "Success", "Added new user!", false);
                }
            });

    connect(deleteUser_Button, &QPushButton::clicked, [=]()
            {
                Graphical graphicalDeleteUser(this);
                bool success = graphicalDeleteUser.deleteUserGraphical(this);

                if (!success)
                {
                    showMessage(this, "Error", "Could not delete the user!", true);
                }
                else
                {
                    std::string logString = "DELETED AN USER";
                    writeEncryptedLog(logString);
                    showMessage(this, "Success", "Deleted the user!", false);
                }
            });

    connect(updateUserInfo_Button, &QPushButton::clicked, [=]()
            {
        Graphical graphicalUpdateUser(this);
        bool success = graphicalUpdateUser.updateUserGraphical(this);

        if (!success) {
            showMessage(this, "Error", "Could not update the user info!", true);
        } else {
            std::string logString = "UPDATED USER INFO";
            writeEncryptedLog(logString);
            showMessage(this, "Success", "Updated the user info!", false);
        } });

    connect(books_Button, &QPushButton::clicked, [=]()
            {
        GeneralOperations generalOperations(libraryDb);

        QList<LibrarySystem::Book> results = generalOperations.searchBook(
            "", "", "", "", "", "", "", "", "", "", "", ""
        );

        if (results.isEmpty()) {
            showMessage(this, "No Books Found", "No books found in the database.", true);
            return;
        }

        bookSearchWindow->graphical->displayBooksWithFilters(this, results); });

    connect(reportLostBook_Button, &QPushButton::clicked, [=]()
            {
                Graphical graphicalReportLostBook(this);
                bool success = graphicalReportLostBook.reportLostBookGraphical(this);

                if (!success)
                {
                    showMessage(this, "Error", "Could not report book as lost", true);
                }
                else
                {
                    showMessage(this, "Success", "Reported book as lost!", false);
                }
            });

    connect(logHistory_Button, &QPushButton::clicked, this, [this]() {

        if (!logWindow) {
            char key = 0x4B; // letter K
            std::vector<QString> logLines = readEncryptedLog(key);

            if (logLines.empty()) {
                showMessage(nullptr, "Log", "No log entries found.", false);
                return;
            }

            logWindow = new QWidget;
            logWindow->setWindowTitle("Log History");
            logWindow->resize(600, 400);

            QVBoxLayout *layout = new QVBoxLayout(logWindow);
            QPlainTextEdit *textEdit = new QPlainTextEdit;
            textEdit->setReadOnly(true);
            textEdit->setStyleSheet(
                "QPlainTextEdit { background-color: #ffffff; color: black; font-size: 12px; }"
            );

            for (const QString &line : logLines) {
                textEdit->appendPlainText(line);
            }

            layout->addWidget(textEdit);
            logWindow->setLayout(layout);

            logWindow->setAttribute(Qt::WA_DeleteOnClose);
            QObject::connect(logWindow, &QWidget::destroyed, [=]() {
                logWindow = nullptr;
            });

            logWindow->show();
        } else {
            logWindow->raise();
            logWindow->activateWindow();
        }
    });


    connect(users_Button, &QPushButton::clicked, this, [this]() {
        if (userWindow) {
            userWindow->raise();
            userWindow->activateWindow();
            return;
        }

        if (!userDb->openDB()) {
            showMessage(nullptr, "Error", "Could not open users.db", true);
            return;
        }

        QSqlQuery query = userDb->selectUsers("");

        userWindow = new QWidget;
        userWindow->setWindowTitle("Users List");
        userWindow->resize(600, 400);

        QVBoxLayout *layout = new QVBoxLayout(userWindow);
        QTableWidget *table = new QTableWidget;
        table->setColumnCount(4);
        table->setHorizontalHeaderLabels({"Username", "School No", "Password", "Account Type"});
        table->horizontalHeader()->setStretchLastSection(true);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);

        table->setStyleSheet(
            "QTableWidget { background-color: white; font-size: 12pt; }"
            "QTableWidget::item { color: black; }"
            "QHeaderView::section { background-color: #dadada; font-weight: bold; }"
        );

        int row = 0;
        while (query.next()) {
            table->insertRow(row);

            QTableWidgetItem *usernameItem = new QTableWidgetItem(query.value("username").toString());
            QTableWidgetItem *schoolNoItem = new QTableWidgetItem(query.value("school_no").toString());
            QTableWidgetItem *passwordItem = new QTableWidgetItem(convertFromAes(query.value("password").toString())); 
            QTableWidgetItem *accountTypeItem = new QTableWidgetItem(query.value("account_type").toString());

            usernameItem->setForeground(QBrush(Qt::black));
            schoolNoItem->setForeground(QBrush(Qt::black));
            passwordItem->setForeground(QBrush(Qt::black));
            accountTypeItem->setForeground(QBrush(Qt::black));

            table->setItem(row, 0, usernameItem);
            table->setItem(row, 1, schoolNoItem);
            table->setItem(row, 2, passwordItem);
            table->setItem(row, 3, accountTypeItem);

            row++;
        }

        layout->addWidget(table);
        userWindow->setLayout(layout);

        userWindow->setAttribute(Qt::WA_DeleteOnClose);
        QObject::connect(userWindow, &QWidget::destroyed, [this]() {
            userWindow = nullptr;
        });

        userWindow->show();
        userDb->closeDB();
    });

}

void AdminInterface::updateDateTime()
{
    dateLabel->setText("Date: " + TimeClass::showDate());
    dayLabel->setText("Day: " + TimeClass::showDay());
    timeLabel->setText("Time: " + TimeClass::showTime());
}

AdminInterface::~AdminInterface()
{
    delete userDb;
    delete libraryDb;
}