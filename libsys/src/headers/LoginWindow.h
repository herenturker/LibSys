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
#include <QTimer>



class LoginWindow : public QWidget
{
    Q_OBJECT

public:
  
    explicit LoginWindow(QWidget *parent = nullptr);

    

signals:
    void loginSuccess(const QString &accountType);

private slots:
    void handleLogin();
    void updateDateTime();

private:

    QLineEdit *username_Edit;
    QLineEdit *password_Edit;
    QLineEdit *schoolNo_Edit;
    QButtonGroup *radioButton_Group;

    QRadioButton *accountType_Admin_Button;
    QRadioButton *accountType_Teacher_Button;
    QRadioButton *accountType_Student_Button;

    QPushButton *login_Button;
    QTimer *timer;
    QLabel *dateLabel;
    QLabel *dayLabel;
    QLabel *timeLabel;

};


#endif // LOGINWINDOW_H_