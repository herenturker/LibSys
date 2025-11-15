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

#include <QString>
#include <QCoreApplication>
#include <QDir>

#include "headers/StudentOperations.h"
#include "headers/database.h"
#include "headers/Utils.h"

void StudentOperations::borrowBookFromLibrary(const QString &schoolNo, const QString &bookISBN, const QString &borrowDate, const QString &dueDate, const QString &uid)
{
    QString exePath = QCoreApplication::applicationDirPath();
    QString dbDirPath = exePath + "/databases";

    QDir().mkpath(dbDirPath);

    // QString userdbPath = dbDirPath + "/users.db";
    QString librarydbPath = dbDirPath + "/library.db";

    // userDb = new Database(userdbPath, "DB_USERS");
    libraryDb = new Database(librarydbPath, "DB_LIBRARY");

    bool success = libraryDb->borrowBook(schoolNo, bookISBN, borrowDate, dueDate, uid);
    if (!success)
    {
        showMessage(nullptr, "Error", "Could not borrow the book", true);
    }
    else
    {
        showMessage(nullptr, "Success", "Borrowed the book!", false);
    }
}

void StudentOperations::returnBorrowedBook(const QString &schoolNo, const QString &bookISBN, const QString &uid)
{
    QString exePath = QCoreApplication::applicationDirPath();
    QString dbDirPath = exePath + "/databases";

    QDir().mkpath(dbDirPath);

    // QString userdbPath = dbDirPath + "/users.db";
    QString librarydbPath = dbDirPath + "/library.db";

    // userDb = new Database(userdbPath, "DB_USERS");
    libraryDb = new Database(librarydbPath, "DB_LIBRARY");

    bool success = libraryDb->returnBook(schoolNo, bookISBN, uid);
    if (!success)
    {
        showMessage(nullptr, "Error", "Could not return the book", true);
    }
    else
    {
        showMessage(nullptr, "Success", "Returned the book!", false);
    }
}

/* Keep them for future use.

void StudentOperations::recordTransactionDate()
{
}

void StudentOperations::requestConfirmation()
{
}

void StudentOperations::displayBookDueDate()
{
}

void StudentOperations::displayBookBorrowDate()
{
}

void StudentOperations::hasBookBeenReturned()
{
}

void StudentOperations::displayBorrowedBooks()
{
}

void StudentOperations::displayUnreturnedBooks()
{
}

void StudentOperations::displayOverdueBooks()
{
}

*/