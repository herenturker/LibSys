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

#include "headers/database.h"

database::database(const QString& dbName, const QString& connectionName)
    : m_dbName(dbName)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    m_db.setDatabaseName(m_dbName);
}

database::~database()
{
    closeDB();
    QSqlDatabase::removeDatabase(m_db.connectionName());
}

bool database::openDB()
{
    if (!m_db.open()) {
        qDebug() << "DB açılamadı:" << m_db.lastError().text();
        return false;
    }
    qDebug() << "DB açıldı:" << m_dbName;
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
        qDebug() << "Tablo oluşturulamadı:" << query.lastError().text();
        return false;
    }
    qDebug() << "users tablosu hazır.";
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
        qDebug() << "Kullanıcı eklenemedi:" << query.lastError().text();
        return false;
    }
    qDebug() << "Kullanıcı eklendi:" << username;
    return true;
}

bool database::updateUserPassword(const QString& username, const QString& newPassword)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE users SET password = :password WHERE username = :username");
    query.bindValue(":password", newPassword);
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Şifre güncellenemedi:" << query.lastError().text();
        return false;
    }
    qDebug() << "Şifre güncellendi:" << username;
    return true;
}

bool database::deleteUser(const QString& username)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Kullanıcı silinemedi:" << query.lastError().text();
        return false;
    }
    qDebug() << "Kullanıcı silindi:" << username;
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
        qDebug() << "SELECT hatası:" << query.lastError().text();
    }

    return query;
}
bool database::isUserMatchedInDataBase(const QString& username,
                                   const QString& schoolNo,
                                   const QString& password,
                                   const QString& accountType) const
{
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
        qDebug() << "Login kontrolü hatası:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        return count > 0;
    }

    return false;
}