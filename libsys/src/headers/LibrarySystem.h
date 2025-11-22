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

#ifndef LIBRARYSYSTEM_H_
#define LIBRARYSYSTEM_H_

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <QString>

#include "Utils.h"


class LibrarySystem

{
    public:
        /**
         * @brief A Book Struct
         * 
         */
        struct Book {
            QString title;
            QString author1;
            QString publisher;
            QString publicationYear;
            QString edition;
            QString ISBN;
            QString volume;
            QString pageCount;
            QString seriesInformation;
            QString language;
            QString DDC;
            QString additionalInfo;
            bool isBorrowed = false;
            QString borrowedBy;
            QString uid;
        };

        /**
         * @brief RFID Data variable
         * 
         */
        static std::string rfid_data;

        /**
         * @brief Updates rfid_data variable value.
         * 
         * @param RFIDdata 
         */
        static void updateRFIDDataValue(const QString& RFIDdata);

        /**
         * @brief Arduino serial communication port 
         * 
         */
        static QString ArduinoCOMPort;

        /**
         * @brief Theme variable (dark/light)
         * 
         */
        static QString theme;

        /**
         * @brief Toggles theme
         * 
         * @param newTheme 
         */
        static void toggleTheme(int newTheme);

        /**
         * @brief Add users to database from a .csv file
         * 
         * @return true 
         * @return false 
         */
        static void addUsersToDatabase();

        /**
         * @brief Add books to database from a .csv file
         * 
         * @return true 
         * @return false 
         */
        static void addBooksToDatabase();

};

#endif // LIBRARYSYSTEM_H_