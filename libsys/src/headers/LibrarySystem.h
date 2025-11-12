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

#include <QString>

class LibrarySystem

{
    public:
        struct Book {
            QString title;
            QString author1;
            QString author2;
            QString author3;
            QString author4;
            QString author5;
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
        };

};

#endif // LIBRARYSYSTEM_H_