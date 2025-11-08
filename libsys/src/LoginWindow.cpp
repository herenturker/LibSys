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
#include <QCoreApplication>
#include <QFile>

#include "headers/LoginWindow.h"
#include "headers/TimeClass.h"
#include "headers/Database.h"

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent)
{
    setWindowIcon(QIcon(":/LibSys.ico"));
    setWindowTitle("Login LibSys");
    setMinimumSize(600, 450);
    setMaximumWidth(600);
    setMaximumHeight(450);

    // === LOGO ===
    QLabel *libsys_Label = new QLabel(this);
    QPixmap pixmap(":/LibSys.png");
    libsys_Label->setPixmap(pixmap.scaled(450, 250, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    libsys_Label->setAlignment(Qt::AlignCenter);

    // === INPUTS ===
    QLabel *username_Label = new QLabel("Enter Username:");
    username_Edit = new QLineEdit;
    QLabel *schoolNo_Label = new QLabel("Enter School No:");
    schoolNo_Edit = new QLineEdit;
    QLabel *password_Label = new QLabel("Enter Password:");
    password_Edit = new QLineEdit;
    password_Edit->setEchoMode(QLineEdit::Password);

    login_Button = new QPushButton("Login");

    username_Edit->setPlaceholderText("Username");
    schoolNo_Edit->setPlaceholderText("School Number");
    password_Edit->setPlaceholderText("Password");

    // === RADIO BUTTONS ===
    radioButton_Group = new QButtonGroup(this);
    accountType_Admin_Button = new QRadioButton("Admin", this);
    accountType_Student_Button = new QRadioButton("Student", this);
    accountType_Teacher_Button = new QRadioButton("Teacher", this);
    accountType_Student_Button->setChecked(true);

    radioButton_Group->addButton(accountType_Student_Button);
    radioButton_Group->addButton(accountType_Teacher_Button);
    radioButton_Group->addButton(accountType_Admin_Button);

    // === DATE / TIME ===
    dateLabel = new QLabel();
    dayLabel  = new QLabel();
    timeLabel = new QLabel();


    // === LAYOUTS ===

    QVBoxLayout *layout_Form = new QVBoxLayout;
    layout_Form->addWidget(username_Label);
    layout_Form->addWidget(username_Edit);
    layout_Form->addWidget(schoolNo_Label);
    layout_Form->addWidget(schoolNo_Edit);
    layout_Form->addWidget(password_Label);
    layout_Form->addWidget(password_Edit);
    layout_Form->addSpacing(5);
    layout_Form->addWidget(login_Button, 0, Qt::AlignHCenter);
    layout_Form->setSpacing(8);

   
    QVBoxLayout *layout_Radio = new QVBoxLayout;
    layout_Radio->addWidget(accountType_Student_Button);
    layout_Radio->addWidget(accountType_Teacher_Button);
    layout_Radio->addWidget(accountType_Admin_Button);
    layout_Radio->addSpacing(15);
    layout_Radio->addWidget(dateLabel);
    layout_Radio->addWidget(dayLabel);
    layout_Radio->addWidget(timeLabel);
    layout_Radio->addStretch();
    layout_Radio->setSpacing(6);
    layout_Radio->setContentsMargins(20, 0, 20, 10);


    QHBoxLayout *layout_Center = new QHBoxLayout;
    layout_Center->addStretch();
    layout_Center->addLayout(layout_Radio, 1);
    layout_Center->addSpacing(40);
    layout_Center->addLayout(layout_Form, 2);
    layout_Center->addStretch();


    QVBoxLayout *layout_Main = new QVBoxLayout(this);
    layout_Main->addSpacing(0);
    layout_Main->addWidget(libsys_Label, 0, Qt::AlignHCenter);
    layout_Main->addStretch(10);
    layout_Main->addLayout(layout_Center);
    layout_Main->addStretch();
    layout_Main->setContentsMargins(20, 5, 20, 20);
    layout_Main->addStretch(20);

    setLayout(layout_Main);

    // === TIMER ===
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &LoginWindow::updateDateTime);
    timer->start(1000);
    updateDateTime();

    // === LOGIN BUTTON ===
    connect(login_Button, &QPushButton::clicked, this, &LoginWindow::handleLogin);

    // === STYLESHEET ===
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
            padding: 4px; 
        }
        QPushButton { 
            background-color: #4CAF50; 
            color: white; 
            border-radius: 5px; 
            padding: 6px 12px; 
            font-weight: bold; 
        }
        QPushButton:hover { 
            background-color: #2A6D2D;
        }
        QRadioButton { 
            color: #333333; 
            font-size: 15px;
            font-weight: bold;
        }
        QLabel#dateLabel, QLabel#dayLabel, QLabel#timeLabel {
            font-size: 15px;
        }
    )");

    dateLabel->setObjectName("dateLabel");
    dayLabel->setObjectName("dayLabel");
    timeLabel->setObjectName("timeLabel");
}


void LoginWindow::handleLogin()
{
    QString username = username_Edit->text();
    QString schoolNo = schoolNo_Edit->text();
    QString password = password_Edit->text();

    QString exePath = QCoreApplication::applicationDirPath();
    QString dbPath = exePath + "/users.db";

    if (!QFile::exists(dbPath)) {
        QFile file(dbPath);
        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "Could not create \"users.db\" !";
        } else {
            file.close();
            qDebug() << "Created \"users.db\" .";
        }
    }

    database userDb(dbPath, "DB_USERS");


    if (!userDb.openDB()) {
        QMessageBox::critical(this, "Error", "Could not open the database!");
        return;
    }

    userDb.createUsersTable();
    userDb.addUserIfNotExists("Eren", "110", "1234", "Student");
    userDb.addUserIfNotExists("Ahmet", "110", "1234", "Student");

    bool loginSuccessFlag = userDb.isUserMatchedInDataBase(
        username_Edit->text(),
        schoolNo_Edit->text(),
        password_Edit->text(),
        radioButton_Group->checkedButton()->text()
    );

    qDebug() << "DEBUG — Login check:";
    qDebug() << "username:" << username;
    qDebug() << "schoolNo:" << schoolNo;
    qDebug() << "password:" << password;
    qDebug() << "accountType:" << radioButton_Group->checkedButton()->text();
    userDb.debugPrintAllUsers();


    if (loginSuccessFlag) {
        emit loginSuccess();
        close();
    } else {
        QMessageBox::warning(this, "Error", "Login Error!");
    }
    
}

void LoginWindow::updateDateTime()
{
    dateLabel->setText("Date: " + TimeClass::showDate());
    dayLabel->setText("Day: " + TimeClass::showDay());
    timeLabel->setText("Time: " + TimeClass::showTime());
}
