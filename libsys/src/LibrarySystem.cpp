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
#include <QDir>

#include "headers/LibrarySystem.h"
#include "headers/database.h"
#include "headers/Utils.h"

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

void LibrarySystem::addUsersToDatabase()
{
        /* Database directory */
    QString exePath = QCoreApplication::applicationDirPath();
    QString dbDirPath = exePath + "/databases";

    QDir().mkpath(dbDirPath);

    QString userdbPath = dbDirPath + "/users.db";

    Database *userDb = new Database(userdbPath, "DB_USERS");

    std::ifstream file("studentList.csv");
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Could not open file!" << std::endl;
    }

    // FIRST ID (automatic)
    // SECOND PASSWORD
    // THIRD NAME
    // FOURTH SCHOOLNO
    // FIFTH UID

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string id, password, name, schoolNo, uid;

        std::getline(ss, id, ',');
        std::getline(ss, password, ',');
        std::getline(ss, name, ',');
        std::getline(ss, schoolNo, ',');
        std::getline(ss, uid, ',');

        QString userUsername = stdStringToQString(name);
        QString userPassword = stdStringToQString(password);
        QString userSchoolNo = stdStringToQString(schoolNo);
        QString userUID = stdStringToQString(uid);

        userDb->addUserIfNotExists(userUsername, userSchoolNo, userPassword, "Student", userUID);
    }

    file.close();
}

void LibrarySystem::addBooksToDatabase()
{
        /* Database directory */
    QString exePath = QCoreApplication::applicationDirPath();
    QString dbDirPath = exePath + "/databases";

    QDir().mkpath(dbDirPath);

    QString librarydbPath = dbDirPath + "/library.db";

    Database *librarydb = new Database(librarydbPath, "DB_LIBRARY");

    std::ifstream file("bookList.csv");
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Could not open file!" << std::endl;
    }

    // FIRST ID (AUTOMATIC)
    // SECOND DDC
    // THIRD BOOK TITLE
    // FOURTH AUTHOR
    // FIFTH PUBLISHER
    // SIXTH SHORT DDC

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string id, ddc, bookTitle, author, publisher, shortDDC;

        std::getline(ss, id, ',');
        std::getline(ss, ddc, ',');
        std::getline(ss, bookTitle, ',');
        std::getline(ss, author, ',');
        std::getline(ss, publisher, ',');
        std::getline(ss, shortDDC, ',');

        QString bookID = stdStringToQString(id);
        QString bookDDC = stdStringToQString(ddc);
        QString bookBookTitle = stdStringToQString(bookTitle);
        QString bookAuthor = stdStringToQString(author);
        QString bookPublisher = stdStringToQString(publisher);
        QString bookShortDDC = stdStringToQString(shortDDC);

        librarydb->addBook(nullptr, bookBookTitle, bookAuthor, bookPublisher, "UNKNOWN", "UNKNOWN", "", "UNKNOWN", "UNKNOWN", "UNKNOWN", "UNKNOWN", bookDDC, bookShortDDC, "");

    }

    file.close();
}