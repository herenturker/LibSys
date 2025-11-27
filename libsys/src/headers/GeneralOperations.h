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

#ifndef GENERAL_OPERATIONS_H_
#define GENERAL_OPERATIONS_H_

#include <QString>
#include <QList>

#include "LibrarySystem.h"
#include "database.h"

class GeneralOperations

{
    public:
        /**
         * @brief Construct a new General Operations object
         * 
         * @param db 
         */
        explicit GeneralOperations(Database *db) : libraryDb(db) {}

        /**
         * @brief Searches a book in the database with provided information.
         * 
         * @param id
         * @param bookTitle
         * @param author 
         * @param publisher 
         * @param publicationYear 
         * @param edition 
         * @param ISBN 
         * @param volume 
         * @param pageCount 
         * @param seriesInformation 
         * @param language 
         * @param DDC 
         * @param additionalInfo 
         * @param uid 
         * @return QList<LibrarySystem::Book> 
         */
        QList<LibrarySystem::Book> searchBook(const QString &id = QString(), const QString &bookTitle = QString(), const QString &author = QString(),
                        const QString &publisher = QString(), const QString &publicationYear = QString(),
                        const QString &edition = QString(), const QString &ISBN = QString(),
                        const QString &volume = QString(), const QString &pageCount = QString(),
                        const QString &seriesInformation = QString(), const QString &language = QString(),
                        const QString &DDC = QString(), const QString &additionalInfo = QString(),  const QString &uid = QString());
     
    protected:
        Database *libraryDb;


};

#endif // GENERAL_OPERATIONS_H_