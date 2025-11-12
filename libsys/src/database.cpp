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

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QDebug>

#include "headers/Database.h"
#include "headers/Utils.h"

Database::Database(const QString &dbName, const QString &connectionName)
    : m_dbName(dbName)
{
    if (QSqlDatabase::contains(connectionName))
    {
        m_db = QSqlDatabase::database(connectionName);
    }
    else
    {
        m_db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        m_db.setDatabaseName(m_dbName);
    }
}

Database::~Database()
{

    if (m_db.isOpen())
    {
        m_db.close();
    }
}

bool Database::openDB()
{
    if (!m_db.open())
    {
        qDebug() << "Could not open the database:" << m_db.lastError().text();
        return false;
    }
    qDebug() << "Opened the database:" << m_dbName;
    return true;
}

void Database::closeDB()
{
    if (m_db.isOpen())
        m_db.close();
}

bool Database::createUsersTable()
{
    QSqlQuery query(m_db);
    QString createTable = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            school_no TEXT NOT NULL,
            password TEXT NOT NULL,
            account_type TEXT NOT NULL
        )
    )";

    if (!query.exec(createTable))
    {
        qDebug() << "Could not create the table:" << query.lastError().text();
        return false;
    }
    qDebug() << "users table is ready.";
    return true;
}

bool Database::addUser(const QString &username, const QString &schoolNo,
                       const QString &password, const QString &accountType)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO users (username, school_no, password, account_type)
        VALUES (:username, :school_no, :password, :account_type)
    )");

    if ((username == "") || (schoolNo == "") || (password == "") || (accountType == ""))
    {
        return false;
    }

    query.bindValue(":username", username);
    query.bindValue(":school_no", schoolNo);
    query.bindValue(":password", password);
    query.bindValue(":account_type", accountType);

    if (!query.exec())
    {
        qDebug() << "Could not add the user:" << query.lastError().text();
        return false;
    }
    qDebug() << "Added user:" << username;
    return true;
}

bool Database::updateUserPassword(const QString &username, const QString &newPassword)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE users SET password = :password WHERE username = :username");
    query.bindValue(":password", newPassword);
    query.bindValue(":username", username);

    if (!query.exec())
    {
        qDebug() << "Could not update the password:" << query.lastError().text();
        return false;
    }
    qDebug() << "Updated password:" << username;
    return true;
}

bool Database::updateUserInfo(QWidget *parent, const QString &username, const QString &schoolNo,
                              const QString &password, const QString &accountType)
{
    if (username.isEmpty()) {
        showMessage(parent, "Error", "Username cannot be empty!", true);
        return false;
    }

    QStringList updates;
    QMap<QString, QVariant> bindings;

    if (!schoolNo.isEmpty()) {
        updates << "school_no = :school_no";
        bindings[":school_no"] = schoolNo;
    }

    if (!password.isEmpty()) {
        updates << "password = :password";
        bindings[":password"] = password;
    }

    if (!accountType.isEmpty()) {
        updates << "account_type = :account_type";
        bindings[":account_type"] = accountType;
    }

    if (updates.isEmpty()) {
        showMessage(parent, "Warning", "No fields to update.", true);
        return false;
    }

    QString sql = QString("UPDATE users SET %1 WHERE username = :username")
                      .arg(updates.join(", "));

    QSqlQuery query(m_db);
    query.prepare(sql);

    for (auto it = bindings.constBegin(); it != bindings.constEnd(); ++it)
        query.bindValue(it.key(), it.value());

    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Could not update user info:" << query.lastError().text();
        showMessage(parent, "Error", "Database update failed!", true);
        return false;
    }

    qDebug() << "Updated user info for:" << username;
    showMessage(parent, "Success", "User information updated successfully!", false);
    return true;
}


bool Database::deleteUser(const QString &username)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec())
    {
        qDebug() << "Could not delete the user:" << query.lastError().text();
        return false;
    }
    qDebug() << "Deleted the user:" << username;
    return true;
}

QSqlQuery Database::selectUsers(const QString &condition)
{
    QSqlQuery query(m_db);
    QString sql = "SELECT * FROM users";
    if (!condition.isEmpty())
    {
        sql += " WHERE " + condition;
    }

    if (!query.exec(sql))
    {
        qDebug() << "SELECT error:" << query.lastError().text();
    }

    return query;
}

bool Database::isUserMatchedInDataBase(const QString &username,
                                       const QString &schoolNo,
                                       const QString &password,
                                       const QString &accountType) const
{
    qDebug() << "DEBUG — Login check:";
    qDebug() << "username:" << username;
    qDebug() << "schoolNo:" << schoolNo;
    qDebug() << "password:" << password;
    qDebug() << "accountType:" << accountType;

    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT COUNT(*) 
        FROM users 
        WHERE username = :username 
          AND school_no = :school_no 
          AND password = :password 
          AND account_type = :account_type
    )");

    query.bindValue(":username", username);
    query.bindValue(":school_no", schoolNo);
    query.bindValue(":password", password);
    query.bindValue(":account_type", accountType);

    if (!query.exec())
    {
        qDebug() << "Login check error:" << query.lastError().text();
        return false;
    }

    if (query.next())
    {
        int count = query.value(0).toInt();
        qDebug() << "Matching user count:" << count;
        return count > 0;
    }
    else
    {
        qDebug() << "Could not get data from query.";
    }

    return false;
}
bool Database::addUserIfNotExists(const QString &username,
                                  const QString &schoolNo,
                                  const QString &password,
                                  const QString &accountType)
{
    QSqlQuery checkQuery(m_db);
    checkQuery.prepare("SELECT COUNT(*) FROM users WHERE username = :username");
    checkQuery.bindValue(":username", username);

    if (!checkQuery.exec())
    {
        qDebug() << "User check error:" << checkQuery.lastError().text();
        return false;
    }

    if (checkQuery.next() && checkQuery.value(0).toInt() > 0)
    {
        qDebug() << "User already exists:" << username;
        showMessage(nullptr, "Error", "User already exists.", true);
        return false;
    }

    // User not found -> safely add
    return addUser(username, schoolNo, password, accountType);
}

void Database::debugPrintAllUsers() const
{
    QSqlQuery query(m_db);
    if (!query.exec("SELECT username, school_no, password, account_type FROM users"))
    {
        qDebug() << "Error in listing users" << query.lastError().text();
        return;
    }

    qDebug() << "---- USERS TABLE CONTENTS ----";
    while (query.next())
    {
        qDebug() << "username:" << query.value(0).toString()
                 << "| school_no:" << query.value(1).toString()
                 << "| password:" << query.value(2).toString()
                 << "| account_type:" << query.value(3).toString();
    }
    qDebug() << "------------------------------";
}

bool Database::addBook(const QString &bookTitle, const QString &author1,
                       const QString &author2, const QString &author3,
                       const QString &author4, const QString &author5,
                       const QString &publisher, const QString &publicationYear,
                       const QString &edition, const QString &ISBN,
                       const QString &volume, const QString &pageCount,
                       const QString &seriesInformation, const QString &language,
                       const QString &DDC, const QString &additionalInfo)
{
    if (!m_db.isOpen() && !m_db.open())
    {
        qDebug() << "Could not open database when adding book.";
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO books (
            title, author1, author2, author3, author4, author5,
            publisher, publication_year, edition, isbn, volume,
            page_count, series_information, language, ddc, additional_info
        )
        VALUES (
            :title, :author1, :author2, :author3, :author4, :author5,
            :publisher, :publication_year, :edition, :isbn, :volume,
            :page_count, :series_information, :language, :ddc, :additional_info
        )
    )");

    query.bindValue(":title", bookTitle);
    query.bindValue(":author1", author1);
    query.bindValue(":author2", author2);
    query.bindValue(":author3", author3);
    query.bindValue(":author4", author4);
    query.bindValue(":author5", author5);
    query.bindValue(":publisher", publisher);
    query.bindValue(":publication_year", publicationYear);
    query.bindValue(":edition", edition);
    query.bindValue(":isbn", ISBN);
    query.bindValue(":volume", volume);
    query.bindValue(":page_count", pageCount);
    query.bindValue(":series_information", seriesInformation);
    query.bindValue(":language", language);
    query.bindValue(":ddc", DDC);
    query.bindValue(":additional_info", additionalInfo);

    if (!query.exec())
    {
        qDebug() << "Could not add book:" << query.lastError().text();
        return false;
    }

    qDebug() << "Added book:" << bookTitle << "by" << author1;
    return true;
}

bool Database::deleteBook(const QString &bookTitle, const QString &author1, const QString &ISBN)
{
    if (!m_db.isOpen() && !m_db.open())
    {
        qDebug() << "Could not open database when deleting book.";
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        DELETE FROM books 
        WHERE title = :title AND author1 = :author1 AND isbn = :isbn
    )");

    query.bindValue(":title", bookTitle);
    query.bindValue(":author1", author1);
    query.bindValue(":isbn", ISBN);

    if (!query.exec())
    {
        qDebug() << "Could not delete book:" << query.lastError().text();
        return false;
    }

    qDebug() << "Deleted book:" << bookTitle << "by" << author1 << "(ISBN:" << ISBN << ")";
    return true;
}

bool Database::updateBook(const QString &bookTitle, const QString &author1,
                          const QString &author2, const QString &author3,
                          const QString &author4, const QString &author5,
                          const QString &publisher, const QString &publicationYear,
                          const QString &edition, const QString &ISBN,
                          const QString &volume, const QString &pageCount,
                          const QString &seriesInformation, const QString &language,
                          const QString &DDC, const QString &additionalInfo)
{
    if (ISBN.trimmed().isEmpty())
    {
        qDebug() << "Cannot update book: ISBN is empty!";
        return false;
    }

    if (!m_db.isOpen() && !m_db.open())
    {
        qDebug() << "Could not open database when updating book info.";
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        UPDATE books SET
            title = :title,
            author1 = :author1,
            author2 = :author2,
            author3 = :author3,
            author4 = :author4,
            author5 = :author5,
            publisher = :publisher,
            publication_year = :publication_year,
            edition = :edition,
            volume = :volume,
            page_count = :page_count,
            series_information = :series_information,
            language = :language,
            ddc = :ddc,
            additional_info = :additional_info
        WHERE isbn = :isbn
    )");

    query.bindValue(":title", bookTitle);
    query.bindValue(":author1", author1);
    query.bindValue(":author2", author2);
    query.bindValue(":author3", author3);
    query.bindValue(":author4", author4);
    query.bindValue(":author5", author5);
    query.bindValue(":publisher", publisher);
    query.bindValue(":publication_year", publicationYear);
    query.bindValue(":edition", edition);
    query.bindValue(":volume", volume);
    query.bindValue(":page_count", pageCount);
    query.bindValue(":series_information", seriesInformation);
    query.bindValue(":language", language);
    query.bindValue(":ddc", DDC);
    query.bindValue(":additional_info", additionalInfo);
    query.bindValue(":isbn", ISBN);

    if (!query.exec())
    {
        qDebug() << "Could not update book:" << query.lastError().text();
        return false;
    }

    qDebug() << "Updated book:" << bookTitle << "(ISBN:" << ISBN << ")";
    return true;
}

bool Database::createBooksTable()
{
    QSqlQuery query(m_db);
    QString createTable = R"(
        CREATE TABLE IF NOT EXISTS books (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            author1 TEXT,
            author2 TEXT,
            author3 TEXT,
            author4 TEXT,
            author5 TEXT,
            publisher TEXT,
            publication_year TEXT,
            edition TEXT,
            isbn TEXT,
            volume TEXT,
            page_count TEXT,
            series_information TEXT,
            language TEXT,
            ddc TEXT,
            additional_info TEXT
        )
    )";

    if (!query.exec(createTable))
    {
        qDebug() << "Could not create books table:" << query.lastError().text();
        return false;
    }
    qDebug() << "books table is ready.";
    return true;
}
