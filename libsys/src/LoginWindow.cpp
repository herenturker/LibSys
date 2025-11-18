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
#include <QDir>

#include "headers/LoginWindow.h"
#include "headers/TimeClass.h"
#include "headers/Database.h"
#include "headers/Utils.h"
#include "headers/SerialReader.h"
#include "headers/LibrarySystem.h"

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

    QLabel *RFID_Data = new QLabel("RFID Data");
    RFID_Data_Value = new QLabel("");

    RFID_Data->setObjectName("RFID_Data");
    RFID_Data_Value->setObjectName("RFID_Data_Value");

    login_Button = new QPushButton("Login");

    username_Edit->setPlaceholderText("Username");
    schoolNo_Edit->setPlaceholderText("School Number");
    password_Edit->setPlaceholderText("Password");

    // === RADIO BUTTONS ===
    radioButton_Group = new QButtonGroup(this);
    loginRadioButton_Group = new QButtonGroup(this);

    accountType_Admin_Button = new QRadioButton("Admin", this);
    accountType_Student_Button = new QRadioButton("Student", this);
    accountType_Student_Button->setChecked(true);

    radioButton_Group->addButton(accountType_Student_Button);
    radioButton_Group->addButton(accountType_Admin_Button);

    normalLogin_Button = new QRadioButton("Normal Login", this);
    quickLogin_Button = new QRadioButton("Quick Login", this);
    normalLogin_Button->setChecked(true);

    loginRadioButton_Group->addButton(normalLogin_Button);
    loginRadioButton_Group->addButton(quickLogin_Button);

    // === DATE / TIME ===
    dateLabel = new QLabel();
    dayLabel = new QLabel();
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

    QVBoxLayout *layout_Login = new QVBoxLayout;
    layout_Login->addWidget(normalLogin_Button);
    layout_Login->addWidget(quickLogin_Button);
    layout_Login->setSpacing(0);

    QVBoxLayout *layout_Radio = new QVBoxLayout;
    layout_Radio->addWidget(accountType_Student_Button);
    layout_Radio->addWidget(accountType_Admin_Button);
    layout_Radio->addSpacing(15);
    layout_Radio->addWidget(dateLabel);
    layout_Radio->addWidget(dayLabel);
    layout_Radio->addWidget(timeLabel);

    // Normally they are not radio buttons.
    // But the position is great, so why not add here :)

    layout_Radio->addWidget(RFID_Data);
    layout_Radio->addWidget(RFID_Data_Value);
    layout_Radio->addStretch();
    layout_Radio->setSpacing(6);

    layout_Radio->setContentsMargins(20, 0, 15, 10);

    QHBoxLayout *layout_Center = new QHBoxLayout;
    layout_Center->addStretch();
    layout_Center->addLayout(layout_Radio, 1); // left
    layout_Center->addSpacing(20);

    layout_Center->addLayout(layout_Form, 2);
    layout_Center->addSpacing(20);
    layout_Center->addStretch();

    layout_Center->addLayout(layout_Login, 1);
    layout_Center->addSpacing(20);

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

    dateLabel->setObjectName("dateLabel");
    dayLabel->setObjectName("dayLabel");
    timeLabel->setObjectName("timeLabel");
}

void LoginWindow::handleLogin()
{
    QString username = username_Edit->text();
    QString schoolNo = schoolNo_Edit->text();
    QString password = password_Edit->text();

    // Database setup
    QString exePath = QCoreApplication::applicationDirPath();
    QString dbDirPath = exePath + "/databases";
    QDir dbDir(dbDirPath);
    if (!dbDir.exists() && !dbDir.mkpath("."))
    {
        QMessageBox::critical(this, "Error", "Could not create databases directory!");
        return;
    }

    QString userdbPath = dbDirPath + "/users.db";
    QString librarydbPath = dbDirPath + "/library.db";

    auto createEmptyDBFile = [](const QString &path)
    {
        if (!QFile::exists(path))
        {
            QFile file(path);
            if (file.open(QIODevice::WriteOnly))
                file.close();
        }
    };
    createEmptyDBFile(userdbPath);
    createEmptyDBFile(librarydbPath);

    Database userDb(userdbPath, "DB_USERS");
    Database libraryDb(librarydbPath, "DB_LIBRARY");

    if (!userDb.openDB())
    {
        QMessageBox::critical(this, "Error", "Could not open users database!");
        return;
    }
    if (!libraryDb.openDB())
    {
        QMessageBox::critical(this, "Error", "Could not open library database!");
        return;
    }

    userDb.createUsersTable();
    libraryDb.createBooksTable();
    libraryDb.createBorrowedBooksTable();

    userDb.addUserIfNotExists("Admin", "0", "admin", "Admin");

    bool loginSuccessFlag = false;
    QString accountType;
    QString loginUsername;
    QString loginSchoolNumber;

    if (loginRadioButton_Group->checkedButton()->text() == "Normal Login")
    {
        QString accountTypeSelected = radioButton_Group->checkedButton()->text();
        loginSuccessFlag = userDb.isUserMatchedInDataBase(username, schoolNo, convertToAes(password), accountTypeSelected);

        if (loginSuccessFlag)
        {
            accountType = accountTypeSelected;
            loginUsername = username;
            loginSchoolNumber = schoolNo;
        }
    }
    else if (loginRadioButton_Group->checkedButton()->text() == "Quick Login")
    {
        if (LibrarySystem::rfid_data.empty())
        {
            QMessageBox::warning(this, "Error", "No RFID data detected!");
            return;
        }

        QString rfid = QString::fromStdString(LibrarySystem::rfid_data);
        loginSuccessFlag = userDb.isUserExistsUID(rfid);

        if (loginSuccessFlag)
        {
            accountType = userDb.getAccountTypeWithUID(rfid);
            loginUsername = userDb.getUsernameWithUID(rfid);
            loginSchoolNumber = userDb.getSchoolNoWithUID(rfid);
        }
    }

    if (loginSuccessFlag)
    {
        schoolNumber = loginSchoolNumber;
        std::string logString = "LOGIN: " + loginUsername.toStdString() + " with school number: " + loginSchoolNumber.toStdString();
        writeEncryptedLog(logString);
        emit loginSuccess(accountType, loginSchoolNumber);
        close();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Login Error!\nInvalid credentials or UID not found.");
    }
}

void LoginWindow::updateDateTime()
{
    dateLabel->setText(TimeClass::showDate());
    dayLabel->setText(TimeClass::showDay());
    timeLabel->setText(TimeClass::showTime());
}

QString LoginWindow::getSchoolNo() const
{
    return schoolNumber;
}

void LoginWindow::updateRFIDLabel(const QString &RFIDdata)
{
    RFID_Data_Value->setText(RFIDdata);
}
