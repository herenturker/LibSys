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

database::database(const QString& dbName, const QString& connectionName)
    : m_dbName(dbName)
{
    if (QSqlDatabase::contains(connectionName)) {
        m_db = QSqlDatabase::database(connectionName);
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        m_db.setDatabaseName(m_dbName);
    }
}


database::~database()
{

     if (m_db.isOpen()){
        m_db.close();
     }
        
}

bool database::openDB()
{
    if (!m_db.open()) {
        qDebug() << "Could not open the database:" << m_db.lastError().text();
        return false;
    }
    qDebug() << "Opened the database:" << m_dbName;
    return true;
}

void database::closeDB()
{
    if (m_db.isOpen())
        m_db.close();
}

bool database::createUsersTable()
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

    if (!query.exec(createTable)) {
        qDebug() << "Could not create the table:" << query.lastError().text();
        return false;
    }
    qDebug() << "users table is ready.";
    return true;
}

bool database::addUser(const QString& username, const QString& schoolNo,
                       const QString& password, const QString& accountType)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO users (username, school_no, password, account_type)
        VALUES (:username, :school_no, :password, :account_type)
    )");
    query.bindValue(":username", username);
    query.bindValue(":school_no", schoolNo);
    query.bindValue(":password", password);
    query.bindValue(":account_type", accountType);

    if (!query.exec()) {
        qDebug() << "Could not add the user:" << query.lastError().text();
        return false;
    }
    qDebug() << "Added user:" << username;
    return true;
}

bool database::updateUserPassword(const QString& username, const QString& newPassword)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE users SET password = :password WHERE username = :username");
    query.bindValue(":password", newPassword);
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Could not update the password:" << query.lastError().text();
        return false;
    }
    qDebug() << "Updated password:" << username;
    return true;
}

bool database::deleteUser(const QString& username)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Could not delete the user:" << query.lastError().text();
        return false;
    }
    qDebug() << "Deleted the user:" << username;
    return true;
}

QSqlQuery database::selectUsers(const QString& condition)
{
    QSqlQuery query(m_db);
    QString sql = "SELECT * FROM users";
    if (!condition.isEmpty()) {
        sql += " WHERE " + condition;
    }

    if (!query.exec(sql)) {
        qDebug() << "SELECT error:" << query.lastError().text();
    }

    return query;
}
bool database::isUserMatchedInDataBase(const QString& username,
                                       const QString& schoolNo,
                                       const QString& password,
                                       const QString& accountType) const
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

    if (!query.exec()) {
        qDebug() << "Login check error:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        qDebug() << "Matching user count:" << count;
        return count > 0;
    } else {
        qDebug() << "Could not get data from query.";
    }

    return false;
}
bool database::addUserIfNotExists(const QString& username,
                                  const QString& schoolNo,
                                  const QString& password,
                                  const QString& accountType)
{
    QSqlQuery checkQuery(m_db);
    checkQuery.prepare("SELECT COUNT(*) FROM users WHERE username = :username");
    checkQuery.bindValue(":username", username);

    if (!checkQuery.exec()) {
        qDebug() << "User check error:" << checkQuery.lastError().text();
        return false;
    }

    if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        qDebug() << "User already exists:" << username;
        return true; // user already exists, not an error
    }

    // User not found -> safely add
    return addUser(username, schoolNo, password, accountType);
}

void database::debugPrintAllUsers() const
{
    QSqlQuery query(m_db);
    if (!query.exec("SELECT username, school_no, password, account_type FROM users")) {
        qDebug() << "Error in listing users" << query.lastError().text();
        return;
    }

    qDebug() << "---- USERS TABLE CONTENTS ----";
    while (query.next()) {
        qDebug() << "username:" << query.value(0).toString()
                 << "| school_no:" << query.value(1).toString()
                 << "| password:" << query.value(2).toString()
                 << "| account_type:" << query.value(3).toString();
    }
    qDebug() << "------------------------------";
}
