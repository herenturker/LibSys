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

#ifndef STUDENT_INTERFACE_H_
#define STUDENT_INTERFACE_H_

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTimer>

#include "BookSearchWindow.h"
#include "database.h"
class StudentInterface : public QWidget

{
    Q_OBJECT

public:
    explicit StudentInterface(QWidget *parent = nullptr);

private slots:
    void updateDateTime();

private:
    QLabel *dateLabel;
    QLabel *dayLabel;
    QLabel *timeLabel;
    QTimer *timer;
    QWidget *searchContainer;

    BookSearchWindow *bookSearchWindow;

    QPushButton *openButton;
    QPushButton *borrowBook_Button;
    QPushButton *returnBook_Button;
    QPushButton *myAccount_Button;

    QLabel *overdueBooks;
    QLabel *unreturnedBooks;
    QLabel *borrowedBooks;

    Database* userDb;
    Database* libraryDb;
};

#endif // STUDENT_INTERFACE_H_