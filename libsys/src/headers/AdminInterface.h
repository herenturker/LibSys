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

// Admin interface for LibSys admin dashboard.

#ifndef ADMIN_INTERFACE_H_
#define ADMIN_INTERFACE_H_

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>

#include "BookSearchWindow.h"
#include "Database.h"

class AdminInterface : public QWidget

{
    Q_OBJECT

public:
    explicit AdminInterface(QWidget *parent = nullptr);
    ~AdminInterface();
    QLabel *RFID_Data_Value;
    QLabel *RFID_Data;
    void updateRFIDLabel(const QString &RFIDdata);

private slots:
    void updateDateTime();

private:
    QPushButton *logHistory_Button;
    QPushButton *books_Button; // Button for listing all books in the database
    QPushButton *users_Button; // Button for listing all users in the database

    QPushButton *addBook_Button;

    QPushButton *deleteBook_Button;
    QPushButton *reportLostBook_Button;
    QPushButton *changeBookInfo_Button;

    QPushButton *addUser_Button;
    QPushButton *deleteUser_Button;
    QPushButton *updateUserInfo_Button;
    BookSearchWindow *bookSearchWindow;

    QLabel *dateLabel;
    QLabel *dayLabel;
    QLabel *timeLabel;
    QTimer *timer;

    Database *userDb;
    Database *libraryDb;

    QWidget *logWindow = nullptr;
    QWidget *userWindow = nullptr;
};

#endif // ADMIN_INTERFACE_H_