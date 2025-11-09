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

#ifndef ADMIN_INTERFACE_H_
#define ADMIN_INTERFACE_H_

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>


class AdminInterface : public QWidget

{
    Q_OBJECT

    public:
        explicit AdminInterface(QWidget *parent = nullptr);

    private slots:
        void updateDateTime();

    private:
        QPushButton* logHistory_Button;
        QPushButton* books_Button;
        QPushButton* users_Button;

        QPushButton* addBook_Button;
        QPushButton* confirmationRequests_Button;
        QPushButton* inquireBookSubmission_Button;
        QPushButton* deleteBook_Button;
        QPushButton* reportLostBook_Button;
        QPushButton* changeBookInfo_Button;
        QPushButton* inquireBookRegistiration_Button;

        QPushButton* backToLoginWindow_Button;

        QLabel *dateLabel;
        QLabel *dayLabel;
        QLabel *timeLabel;
        QTimer *timer;

};


#endif // ADMIN_INTERFACE_H_