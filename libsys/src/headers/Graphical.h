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

#ifndef GRAPHICAL_H_
#define GRAPHICAL_H_

#include <QWidget>
#include <QString>
#include <QList>

#include "LibrarySystem.h"

class Graphical

{
    // Q_OBJECT

    public:
        explicit Graphical(QWidget *parent = nullptr);
        bool performAction(QWidget *parent, const QString &text);
        bool addUserGraphical(QWidget *parent);
        bool deleteUserGraphical(QWidget *parent);
        bool updateUserGraphical(QWidget *parent);
        void displayBooksWithFilters(QWidget *parent, QList<LibrarySystem::Book> results);

};

#endif // GRAPHICAL_H_