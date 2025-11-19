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

#ifndef STUDENT_OPERATIONS_H_
#define STUDENT_OPERATIONS_H_

#include "GeneralOperations.h"
#include "database.h"

class StudentOperations

{
    public:
        /**
         * @brief Borrows a book from library database.
         * 
         * @param schoolNo 
         * @param bookISBN 
         * @param borrowDate 
         * @param dueDate 
         * @param uid 
         */
        void borrowBookFromLibrary(const QString &schoolNo, const QString &bookISBN, const QString &borrowDate, const QString &dueDate, const QString &uid = "");

        /**
         * @brief Returns a book to library database.
         * 
         * @param schoolNo 
         * @param bookISBN 
         * @param uid 
         */
        void returnBorrowedBook(const QString &schoolNo, const QString &bookISBN, const QString &uid = "");


        /**
         * @brief Requests borrow approval from admin. User can borrow a book if admin approves.
         * 
         * @param schoolNo 
         * @param bookISBN 
         * @param borrowDate 
         * @param dueDate 
         * @param uid 
         */
        void requestBorrowApproval(const QString &schoolNo, const QString &bookISBN, const QString &borrowDate, const QString &dueDate, const QString &uid = "");

        /**
         * @brief Requests return approval from admin. User can return a book if admin approves.
         * 
         * @param schoolNo 
         * @param bookISBN 
         * @param uid 
         */
        void requestReturnApproval(const QString &schoolNo, const QString &bookISBN, const QString &uid = "");

    private:
        Database* userDb;
        Database* libraryDb;
};

#endif // STUDENT_OPERATIONS_H_