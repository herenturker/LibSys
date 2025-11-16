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
#include <QTableWidget>

#include "LibrarySystem.h"
#include "database.h"

class Graphical

{

public:
    /**
     * @brief Construct a new Graphical object
     * 
     * @param parent 
     */
    explicit Graphical(QWidget *parent = nullptr);

    /**
     * @brief A basic message box for confirmations.
     * 
     * @param parent 
     * @param text 
     * @return true 
     * @return false 
     */
    bool performAction(QWidget *parent, const QString &text);

    /**
     * @brief A graphical interface for adding user.
     * 
     * @param parent 
     * @return true 
     * @return false 
     */
    bool addUserGraphical(QWidget *parent);

    /**
     * @brief A graphical interface for deleting user.
     * 
     * @param parent 
     * @return true 
     * @return false 
     */
    bool deleteUserGraphical(QWidget *parent);

    /**
     * @brief A graphical interface for reporting a book as lost.
     * 
     * @param parent 
     * @return true 
     * @return false 
     */
    bool reportLostBookGraphical(QWidget *parent);

    /**
     * @brief A graphical interface for updating an user's information.
     * 
     * @param parent 
     * @return true 
     * @return false 
     */
    bool updateUserGraphical(QWidget *parent);

    /**
     * @brief Displays books with provided filters. 
     * 
     * @param parent 
     * @param results 
     */
    void displayBooksWithFilters(QWidget *parent, QList<LibrarySystem::Book> results);

    /**
     * @brief Get the Book Table object
     * 
     * @return QTableWidget* 
     */
    QTableWidget *getBookTable();

    /**
     * @brief Get the Book Window object
     * 
     * @return QWidget* 
     */
    QWidget *getBookWindow() const { return bookWindow; }

private:
    QWidget *bookWindow = nullptr;

    QTableWidget *bookTable = nullptr;

    Database *libraryDb;
};

#endif // GRAPHICAL_H_