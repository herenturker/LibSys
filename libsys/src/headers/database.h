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

#ifndef DATABASE_H_
#define DATABASE_H_

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>


#include <QDebug>
#include <QString>

class Database
{
    public:
        Database(const QString& dbName, const QString& connectionName);
        ~Database();

        bool openDB();
        void closeDB();

        bool createUsersTable();

        bool addUser(const QString& username, const QString& schoolNo,
                    const QString& password, const QString& accountType);

        bool updateUserPassword(const QString& username, const QString& newPassword);
        bool deleteUser(const QString& username);
        QSqlQuery selectUsers(const QString& condition = "");

        bool isUserMatchedInDataBase(const QString& username,
                    const QString& schoolNo,
                    const QString& password,
                    const QString& accountType) const;

        bool addUserIfNotExists(const QString& username,
                        const QString& schoolNo,
                        const QString& password,
                        const QString& accountType);
        void debugPrintAllUsers() const;

    private:
        QSqlDatabase m_db;
        QString m_dbName;
};

#endif // DATABASE_H_