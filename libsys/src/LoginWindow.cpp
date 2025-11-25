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
#include <QAbstractButton>
#include <QSettings>

#include "headers/LoginWindow.h"
#include "headers/TimeClass.h"
#include "headers/Database.h"
#include "headers/Utils.h"
#include "headers/SerialReader.h"
#include "headers/LibrarySystem.h"

QTranslator LoginWindow::translator;
QString LoginWindow::language = ":/translations/translations_en.qm"; // defined as static in LoginWindow.h

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent)
{
    setWindowIcon(QIcon(":/LibSys.ico"));
    setWindowTitle("LibSys");
    setMinimumSize(600, 450);
    setMaximumWidth(600);
    setMaximumHeight(450);

    // === LOGO ===
    QLabel *libsys_Label = new QLabel(this);
    QPixmap pixmap(":/LibSys.png");
    libsys_Label->setPixmap(pixmap.scaled(450, 250, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    libsys_Label->setAlignment(Qt::AlignCenter);
    // === INPUTS ===
    username_Edit = new QLineEdit;
    schoolNo_Edit = new QLineEdit;
    password_Edit = new QLineEdit;
    password_Edit->setEchoMode(QLineEdit::Password);

    RFID_Data = new QLabel(this);
    RFID_Data_Value = new QLabel(this);

    RFID_Data->setObjectName("RFID_Data");
    RFID_Data_Value->setObjectName("RFID_Data_Value");

    login_Button = new QPushButton(this);

    username_Edit->setPlaceholderText("");
    schoolNo_Edit->setPlaceholderText("");
    password_Edit->setPlaceholderText("");

    // === RADIO BUTTONS ===
    radioButton_Group = new QButtonGroup(this);
    loginRadioButton_Group = new QButtonGroup(this);
    themeRadioButton_Group = new QButtonGroup(this);

    accountType_Admin_Button = new QRadioButton(this);
    accountType_Student_Button = new QRadioButton(this);
    accountType_Student_Button->setChecked(true);

    radioButton_Group->addButton(accountType_Student_Button);
    radioButton_Group->addButton(accountType_Admin_Button);

    normalLogin_Button = new QRadioButton(this);
    quickLogin_Button = new QRadioButton(this);

    // === FLAGS FOR TRANSLATION ===

    btnTr = new QPushButton(this);
    btnEn = new QPushButton(this);

    btnTr->setIcon(QIcon(":/flags/tr.png"));
    btnEn->setIcon(QIcon(":/flags/en.png"));

    btnTr->setIconSize(QSize(32, 32));
    btnEn->setIconSize(QSize(32, 32));

    btnTr->setFixedSize(40, 40);
    btnEn->setFixedSize(40, 40);

    toggleDarkTheme = new QRadioButton("", this);
    toggleLightTheme = new QRadioButton("", this);

    toggleLightTheme->setChecked(true);

    loginRadioButton_Group->addButton(normalLogin_Button);
    loginRadioButton_Group->addButton(quickLogin_Button);

    themeRadioButton_Group->addButton(toggleLightTheme);
    themeRadioButton_Group->addButton(toggleDarkTheme);

    // === DATE / TIME ===
    dateLabel = new QLabel();
    dayLabel = new QLabel();
    timeLabel = new QLabel();

    // === LAYOUTS ===

    QVBoxLayout *layout_Form = new QVBoxLayout;
    layout_Form->addWidget(username_Edit);
    layout_Form->addWidget(schoolNo_Edit);
    layout_Form->addWidget(password_Edit);
    layout_Form->addSpacing(5);
    layout_Form->addWidget(login_Button, 0, Qt::AlignHCenter);
    layout_Form->setSpacing(8);

    QHBoxLayout *flagLayout = new QHBoxLayout();
    flagLayout->setSpacing(0);
    flagLayout->addWidget(btnTr);
    flagLayout->addWidget(btnEn);

    QVBoxLayout *layout_Login = new QVBoxLayout;
    layout_Login->addWidget(normalLogin_Button);
    layout_Login->addWidget(quickLogin_Button);
    layout_Login->setSpacing(0);
    normalLogin_Button->setChecked(true);

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

    // === THEME LAYOUT ===
    QHBoxLayout *themeLayout = new QHBoxLayout;
    themeLayout->addStretch();
    themeLayout->addWidget(toggleDarkTheme);
    themeLayout->addSpacing(10);
    themeLayout->addWidget(toggleLightTheme);

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
    layout_Main->addLayout(themeLayout);
    layout_Main->addStretch();
    layout_Main->addLayout(flagLayout);
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
        QLabel#dateLabel, QLabel#dayLabel, QLabel#timeLabel {
            font-size: 12px;
        }
        QLabel#RFID_Data, QLabel#RFID_Data_Value {
            font-size: 12px;
            color: red;
        }
    )");

    // === ADJUST SIZE ===

    // QLineEdit
    setWidgetSize(username_Edit, 0, 35);
    setWidgetSize(schoolNo_Edit, 0, 35);
    setWidgetSize(password_Edit, 0, 35);

    // Buttons
    setWidgetSize(login_Button, 120, 40);
    setWidgetSize(btnTr, 50, 50);
    setWidgetSize(btnEn, 50, 50);

    // RadioButtons
    setWidgetSize(accountType_Admin_Button, 0, 30);
    setWidgetSize(accountType_Student_Button, 0, 30);
    setWidgetSize(normalLogin_Button, 0, 30);
    setWidgetSize(quickLogin_Button, 0, 30);

    // === OBJECT NAMES
    dateLabel->setObjectName("dateLabel");
    dayLabel->setObjectName("dayLabel");
    timeLabel->setObjectName("timeLabel");
    toggleLightTheme->setObjectName("LightTheme");
    toggleDarkTheme->setObjectName("DarkTheme");

    // === TOGGLE THEME SETTINGS ===

    connect(themeRadioButton_Group, &QButtonGroup::buttonClicked, this,
            [&](QAbstractButton *btn)
            {
                if (btn == toggleLightTheme)
                    LibrarySystem::toggleTheme(1);
                else if (btn == toggleDarkTheme)
                    LibrarySystem::toggleTheme(2);
            });

    connect(btnTr, &QPushButton::clicked, this, [=]()
            { switchLanguage("tr"); });

    connect(btnEn, &QPushButton::clicked, this, [=]()
            { switchLanguage("en"); });

    if (translator.load(language)) {
        qApp->installTranslator(&translator);
    }
    retranslateUi();
}

void LoginWindow::handleLogin()
{
    QString username = username_Edit->text().trimmed();
    QString schoolNo = schoolNo_Edit->text().trimmed();
    QString password = password_Edit->text();

    if (username.isEmpty() || schoolNo.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, tr("Input Error"), tr("Please fill in Username, School Number, and Password."));
        return;
    }

    // Database setup
    QString exePath = QCoreApplication::applicationDirPath();
    QString dbDirPath = exePath + "/databases";
    QDir dbDir(dbDirPath);
    if (!dbDir.exists() && !dbDir.mkpath("."))
    {
        QMessageBox::critical(this, tr("Error"), tr("Could not create databases directory!"));
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
        QMessageBox::critical(this, tr("Error"), tr("Could not open users database!"));
        return;
    }
    if (!libraryDb.openDB())
    {
        QMessageBox::critical(this, tr("Error"), tr("Could not open library database!"));
        return;
    }

    userDb.createUsersTable();
    userDb.createUserEmailsTable();
    libraryDb.createBooksTable();
    libraryDb.createBorrowedBooksTable();
    libraryDb.createBorrowRequestsTable();
    libraryDb.createReturnRequestsTable();
    libraryDb.createOverdueBooksTable();

    userDb.addUserIfNotExists("Admin", "0", "admin", "Admin");

    // don't need to add again 
    QSettings settings("LibSys", "LibSysApp");
    bool initialDataAdded = settings.value("initialDataAdded", false).toBool();

    if (!initialDataAdded)
    {
        LibrarySystem::addUsersToDatabase();
        LibrarySystem::addBooksToDatabase();

        settings.setValue("initialDataAdded", true);
    }


    bool loginSuccessFlag = false;
    QString accountType;
    QString loginUsername;
    QString loginSchoolNumber;
    QString user_password;

    if (loginRadioButton_Group->checkedButton() == normalLogin_Button)
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
    else if (loginRadioButton_Group->checkedButton() == quickLogin_Button)
    {
        
        if (LibrarySystem::rfid_data.empty())
        {
            QMessageBox::warning(this, tr("Error"), tr("No RFID data detected!"));
            QMessageBox::information(this, tr("Attention"), tr("Please scan your card."));
            return;
        }

        QString rfid = QString::fromStdString(LibrarySystem::rfid_data);


        loginSuccessFlag = userDb.isUserExistsUID(rfid);

        if (loginSuccessFlag)
        {
            accountType = userDb.getAccountTypeWithUID(rfid);
            loginUsername = userDb.getUsernameWithUID(rfid);
            loginSchoolNumber = userDb.getSchoolNoWithUID(rfid);
            user_password = userDb.getPasswordWithUID(rfid);
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
        QMessageBox::warning(this, tr("Error"), tr("Login Error!\nInvalid credentials or UID not found."));
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

    Database userDb("databases/users.db", "DB_USERS");
    if (!userDb.openDB()) return;

    if (userDb.isUserExistsUID(RFIDdata) && loginRadioButton_Group->checkedButton() == quickLogin_Button)
    {
        username_Edit->setText(userDb.getUsernameWithUID(RFIDdata));
        schoolNo_Edit->setText(userDb.getSchoolNoWithUID(RFIDdata));
        password_Edit->setText(convertFromAes(userDb.getPasswordWithUID(RFIDdata)));
    }
}

void LoginWindow::switchLanguage(const QString &langCode)
{
    if (langCode == "tr")
        LoginWindow::language = ":/translations/tr.qm";
    else
        LoginWindow::language = ":/translations/en.qm";

    emit languageChanged(langCode);

    retranslateUi();
}


void LoginWindow::retranslateUi()
{
    // Window title
    this->setWindowTitle(tr("Login LibSys"));

    // Placeholders
    username_Edit->setPlaceholderText(tr("Username"));
    schoolNo_Edit->setPlaceholderText(tr("School Number"));
    password_Edit->setPlaceholderText(tr("Password"));

    // Login button
    login_Button->setText(tr("Login"));

    // Radio buttons
    accountType_Admin_Button->setText(tr("Admin"));
    accountType_Student_Button->setText(tr("Student"));
    normalLogin_Button->setText(tr("Normal Login"));
    quickLogin_Button->setText(tr("Quick Login"));

    // RFID
    RFID_Data->setText(tr("RFID Data"));
}

void LoginWindow::setWidgetSize(QWidget* widget, int width, int height)
{
    if (width > 0 && height > 0)
        widget->setFixedSize(width, height);
    else if (width > 0)
        widget->setFixedWidth(width);
    else if (height > 0)
        widget->setFixedHeight(height);
}
