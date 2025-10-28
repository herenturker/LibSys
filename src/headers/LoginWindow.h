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

#ifndef LOGINWINDOW_H_
#define LOGINWINDOW_H_

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QRadioButton>

#include "ProgramInterface.h"
#include "Input.h"


class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);

signals:
    void loginSuccess();

private slots:
    void handleLogin();

private:

    QLineEdit *username_Edit;
    QLineEdit *password_Edit;
    QLineEdit *schoolNo_Edit;

    QRadioButton *accountType_Admin_Button;
    QRadioButton *accountType_Student_Button;
    QRadioButton *accountType_Teacher_Button;

    QPushButton *login_Button;

    ProgramInterface programInterface;
    Input input;
};


#endif // LOGINWINDOW_H_