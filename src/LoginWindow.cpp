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

#include <string>
#include <QMessageBox>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QTimer>

#include "headers/LoginWindow.h"
#include "headers/TimeClass.h"

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent)
{
    setWindowIcon(QIcon("../icons/LibSys.ico"));

    QLabel *username_Label = new QLabel("Enter Username:");
    username_Edit = new QLineEdit;

    QLabel *schoolNo_Label = new QLabel("Enter School No:");
    schoolNo_Edit = new QLineEdit;

    QLabel *password_Label = new QLabel("Enter Password:");
    password_Edit = new QLineEdit;
    password_Edit->setEchoMode(QLineEdit::Password);

    login_Button = new QPushButton("Login");

    accountType_Admin_Button = new QRadioButton("Admin", this);
    accountType_Student_Button = new QRadioButton("Student", this);
    accountType_Teacher_Button = new QRadioButton("Teacher", this);

    QVBoxLayout *layout_Vertical = new QVBoxLayout;
    QButtonGroup *radioButton_Group = new QButtonGroup(this);

    dateLabel = new QLabel();
    dayLabel  = new QLabel();
    timeLabel = new QLabel();

    layout_Vertical->addWidget(username_Label);
    layout_Vertical->addWidget(username_Edit);

    layout_Vertical->addWidget(schoolNo_Label);
    layout_Vertical->addWidget(schoolNo_Edit);

    layout_Vertical->addWidget(password_Label);
    layout_Vertical->addWidget(password_Edit);

    layout_Vertical->addWidget(login_Button);

    radioButton_Group->addButton(accountType_Student_Button);
    radioButton_Group->addButton(accountType_Teacher_Button);
    radioButton_Group->addButton(accountType_Admin_Button);

    QVBoxLayout *layout_Radio = new QVBoxLayout;
    layout_Radio->addWidget(accountType_Student_Button);
    layout_Radio->addWidget(accountType_Teacher_Button);
    layout_Radio->addWidget(accountType_Admin_Button);
    layout_Radio->addStretch();

    accountType_Student_Button->setChecked(true);

    layout_Radio->addWidget(dateLabel);
    layout_Radio->addWidget(dayLabel);
    layout_Radio->addWidget(timeLabel);
    layout_Radio->addSpacing(10);

    QHBoxLayout *layout_Main = new QHBoxLayout;
    layout_Main->addStretch();
    layout_Main->addLayout(layout_Radio);
    layout_Main->addSpacing(20);
    layout_Main->addLayout(layout_Vertical);
    layout_Main->addStretch();

    layout_Main->setSpacing(15);
    layout_Main->setContentsMargins(20, 30, 20, 30);
    layout_Vertical->setSpacing(8);
    layout_Vertical->setContentsMargins(0, 0, 0, 0);
    layout_Radio->setSpacing(6);

    setLayout(layout_Main);

    username_Edit->setPlaceholderText("Username");
    schoolNo_Edit->setPlaceholderText("School Number");
    password_Edit->setPlaceholderText("Password");

    setWindowTitle("Login LibSys");
    resize(400, 200);
    setMinimumSize(400, 250);
    setMaximumWidth(500);
    setMaximumHeight(270);

    connect(login_Button, &QPushButton::clicked, this, &LoginWindow::handleLogin);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &LoginWindow::updateDateTime);
    timer->start(1000);
    updateDateTime();

    this->setStyleSheet(R"(
        QWidget { 
            background-color: #f5f5f5; 
        }
        QLabel { 
            color: #333333; 
            font-weight: bold; 
        }
        QLineEdit { 
            background-color: #ffffff; 
            color: #000000; 
            border: 1px solid #cccccc; 
            padding: 3px; 
        }
        QPushButton { 
            background-color: #4CAF50; 
            color: white; 
            border-radius: 5px; 
            padding: 5px; 
            font-weight: bold; 
        }
        QPushButton:hover { 
            background-color: #2A6D2D; /* koyu yeşil */
        }
        QRadioButton { 
            color: #333333; 
        }
    )");

}

void LoginWindow::handleLogin()
{
    QString username = username_Edit->text();
    QString schoolNo = schoolNo_Edit->text();
    QString password = password_Edit->text();

    if (username == "admin" && password == "1234") {
        emit loginSuccess();
        close();
    } else {
        QMessageBox::warning(this, "Hata", "Kullanıcı adı veya şifre yanlış!");
    }
}

void LoginWindow::updateDateTime()
{
    dateLabel->setText("Date: " + TimeClass::showDate());
    dayLabel->setText("Day: " + TimeClass::showDay());
    timeLabel->setText("Time: " + TimeClass::showTime());
}
