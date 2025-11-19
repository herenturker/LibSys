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

#include <QPushButton>
#include <QGuiApplication>
#include <QScreen>
#include <QFile>
#include <QMessageBox>
#include <QDir>
#include <QList>
#include <QPlainTextEdit>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QSettings>

#include "headers/AdminInterface.h"
#include "headers/LoginWindow.h"
#include "headers/TimeClass.h"
#include "headers/BookSearchWindow.h"
#include "headers/Database.h"
#include "headers/Graphical.h"
#include "headers/Utils.h"
#include "headers/LibrarySystem.h"
#include "headers/GeneralOperations.h"

AdminInterface::AdminInterface(QWidget *parent) : QWidget(parent)

{
    /* Database directory */

    QString exePath = QCoreApplication::applicationDirPath();
    QString dbDirPath = exePath + "/databases";

    QDir().mkpath(dbDirPath);

    QString userdbPath = dbDirPath + "/users.db";
    QString librarydbPath = dbDirPath + "/library.db";

    userDb = new Database(userdbPath, "DB_USERS");
    libraryDb = new Database(librarydbPath, "DB_LIBRARY");

    /* Window settings */

    setWindowTitle("LibSys Admin Dashboard");

    // Width 1080, height 720
    resize(1080, 720);
    setMinimumSize(1080, 720);
    setMaximumSize(1080, 720);

    dateLabel = new QLabel(this); // Date label
    dayLabel = new QLabel(this);  // Day label
    timeLabel = new QLabel(this); // Time label

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &AdminInterface::updateDateTime);
    timer->start(1000); // Per 1 second, update time

    /*------ BUTTON DEFINITIONS ------*/

    logHistory_Button = new QPushButton("Log History", this);
    logHistory_Button->setToolTip("View the system's activity and log history.");

    books_Button = new QPushButton("Books", this);
    books_Button->setToolTip("Browse all available books in the library.");

    users_Button = new QPushButton("Users", this);
    users_Button->setToolTip("View and manage registered users.");

    addBook_Button = new QPushButton("Add Book", this);
    addBook_Button->setToolTip("Add a new book to the library database.");

    deleteBook_Button = new QPushButton("Delete Book", this);
    deleteBook_Button->setToolTip("Remove a book from the library database.");

    changeBookInfo_Button = new QPushButton("Update\nBook Info", this);
    changeBookInfo_Button->setToolTip("Edit or update details of an existing book.");

    QLabel *adminDashboard = new QLabel("Admin\nDashboard", this);
    adminDashboard->move(75, 30);
    adminDashboard->resize(500, 170);

    reportLostBook_Button = new QPushButton("Report Lost Book", this);
    reportLostBook_Button->setToolTip("Report a lost book and update its record.");

    addUser_Button = new QPushButton("Add User", this);
    addUser_Button->setToolTip("Add a new user to the users database.");

    deleteUser_Button = new QPushButton("Delete User", this);
    deleteUser_Button->setToolTip("Delete an user from the users database.");

    updateUserInfo_Button = new QPushButton("Update User Info", this);
    updateUserInfo_Button->setToolTip("Update or edit an user's info.");

    RFID_Data = new QLabel("RFID Data", this);
    RFID_Data_Value = new QLabel("", this);

    QPushButton *enterCOM_button = new QPushButton("COM", this);
    enterCOM_button->setToolTip("Enter COM Info for RFID Serial Port");

    RFID_Data->setObjectName("RFID_Data");
    RFID_Data_Value->setObjectName("RFID_Data_Value");

    updateRFIDLabel(stdStringToQString(LibrarySystem::rfid_data));

    RFID_Data->setGeometry(860, 120, 120, 30);
    RFID_Data_Value->setGeometry(860, 160, 120, 30);

    unsigned short buttonWidth = 130;
    unsigned short buttonHeight = 50;
    unsigned short buttonSquare = 170;

    logHistory_Button->setGeometry(860, 50, buttonWidth, buttonHeight);
    books_Button->setGeometry(860, 620, buttonWidth, buttonHeight);
    users_Button->setGeometry(710, 620, buttonWidth, buttonHeight);

    enterCOM_button->setGeometry(710, 50, buttonWidth, buttonHeight);

    dateLabel->setGeometry(75, 650, 200, 30);
    dayLabel->setGeometry(75, 620, 200, 30);
    timeLabel->setGeometry(75, 590, 200, 30);

    addBook_Button->setGeometry(75, 215, buttonSquare, buttonSquare);

    deleteBook_Button->setGeometry(265, 215, buttonSquare, buttonSquare);

    changeBookInfo_Button->setGeometry(460, 215, buttonSquare, buttonSquare);

    reportLostBook_Button->setGeometry(265, 620, 160, buttonHeight);

    addUser_Button->setGeometry(75, 410, buttonSquare, buttonSquare);

    deleteUser_Button->setGeometry(265, 410, buttonSquare, buttonSquare);

    updateUserInfo_Button->setGeometry(460, 410, buttonSquare, buttonSquare);

    dateLabel->setObjectName("dateLabel");
    dayLabel->setObjectName("dayLabel");
    timeLabel->setObjectName("timeLabel");

    // CONNECTIONS

    if (!QFile::exists(userdbPath))
    {
        QFile file(userdbPath);
        if (!file.open(QIODevice::WriteOnly))
        {
            // We should not come here.
            //  qDebug() << "Could not create \"users.db\" !";
        }
        else
        {
            file.close();
        }
    }

    if (!QFile::exists(librarydbPath))
    {
        QFile file(librarydbPath);
        if (!file.open(QIODevice::WriteOnly))
        {
            // We should not come here.
            //  qDebug() << "Could not create \"library.db\" !";
        }
        else
        {
            file.close();
        }
    }

    if (!userDb->openDB())
    {
        QMessageBox::critical(this, "Error", "Could not open the database!");
        return;
    }

    if (!libraryDb->openDB())
    {
        QMessageBox::critical(this, "Error", "Could not open the database!");
        return;
    }

    userDb->createUsersTable();
    libraryDb->createBooksTable();

    bookSearchWindow = new BookSearchWindow(this);

    // ADD BOOK

    connect(addBook_Button, &QPushButton::clicked, [&]()
            {
        bookSearchWindow->setMode(BookSearchWindow::Add);
        bookSearchWindow->show(); });

    connect(bookSearchWindow, &BookSearchWindow::bookAddDataReady,
            [&](const QString &bookTitle,
                const QString &author1,
                const QString &publisher,
                const QString &publicationYear,
                const QString &edition,
                const QString &ISBN,
                const QString &volume,
                const QString &pageCount,
                const QString &seriesInformation,
                const QString &language,
                const QString &DDC,
                const QString &additionalInfo, const QString &uid = "")
            {
                bool success = libraryDb->addBook(this, bookTitle, author1,
                                                  publisher, publicationYear, edition, ISBN, volume,
                                                  pageCount, seriesInformation, language, DDC, additionalInfo, uid);

                if (!success)
                {
                    showMessage(bookSearchWindow, "Error", "Could not add book to database!", true);
                }
                else
                {
                    bookSearchWindow->hide();
                    std::string logString = "ADD BOOK:  " + ISBN.toStdString();
                    writeEncryptedLog(logString);
                    showMessage(bookSearchWindow, "Success", "Added new book!", false);
                }
            });

    // DELETE BOOK

    connect(deleteBook_Button, &QPushButton::clicked, [&]()
            {
        bookSearchWindow->setMode(BookSearchWindow::Delete);
        bookSearchWindow->show(); });

    connect(bookSearchWindow, &BookSearchWindow::bookDeleteDataReady,
            [&](const QString &bookTitle,
                const QString &author1,
                const QString &ISBN, const QString &uid = "")
            {
                bool success = libraryDb->deleteBook(this, bookTitle, author1, ISBN, uid);

                if (!success)
                {
                    showMessage(this, "Error", "Could not delete book!", true);
                }
                else
                {
                    bookSearchWindow->hide();
                    std::string logString = "DELETE BOOK:  " + ISBN.toStdString();
                    writeEncryptedLog(logString);
                    showMessage(this, "Success", "Deleted the book!", false);
                }
            });

    // UPDATE BOOK INFO
    connect(changeBookInfo_Button, &QPushButton::clicked, [&]()
            {
        bookSearchWindow->setMode(BookSearchWindow::Update);
        bookSearchWindow->show(); });

    connect(bookSearchWindow, &BookSearchWindow::bookUpdateDataReady,
            [&](const QString &bookTitle,
                const QString &author1,
                const QString &publisher,
                const QString &publicationYear,
                const QString &edition,
                const QString &ISBN,
                const QString &volume,
                const QString &pageCount,
                const QString &seriesInformation,
                const QString &language,
                const QString &DDC,
                const QString &additionalInfo, const QString &uid = "")
            {
                bool success = libraryDb->updateBook(this, bookTitle, author1,
                                                     publisher, publicationYear, edition, ISBN, volume,
                                                     pageCount, seriesInformation, language, DDC, additionalInfo, uid);

                if (!success)
                {
                    showMessage(this, "Error", "Could not update book info!", true);
                }
                else
                {
                    bookSearchWindow->hide();
                    std::string logString = "UPDATE BOOK INFO:  " + ISBN.toStdString();
                    writeEncryptedLog(logString);
                    showMessage(this, "Success", "Updated the book info!", false);
                }
            });

    // ADD USER
    connect(addUser_Button, &QPushButton::clicked, [=]()
            {
                Graphical graphicalAddUser(this);
                bool success = graphicalAddUser.addUserGraphical(this);

                if (!success)
                {
                    showMessage(this, "Error", "Could not add new user!", true);
                }
                else
                {
                    std::string logString = "ADDED NEW USER";
                    writeEncryptedLog(logString);
                    showMessage(this, "Success", "Added new user!", false);
                } });

    // DELETE USER
    connect(deleteUser_Button, &QPushButton::clicked, [=]()
            {
                Graphical graphicalDeleteUser(this);
                bool success = graphicalDeleteUser.deleteUserGraphical(this);

                if (!success)
                {
                    showMessage(this, "Error", "Could not delete the user!", true);
                }
                else
                {
                    std::string logString = "DELETED AN USER";
                    writeEncryptedLog(logString);
                    showMessage(this, "Success", "Deleted the user!", false);
                } });

    // UPDATE USER INFO
    connect(updateUserInfo_Button, &QPushButton::clicked, [=]()
            {
        Graphical graphicalUpdateUser(this);
        bool success = graphicalUpdateUser.updateUserGraphical(this);

        if (!success) {
            showMessage(this, "Error", "Could not update the user info!", true);
        } else {
            std::string logString = "UPDATED USER INFO";
            writeEncryptedLog(logString);
            showMessage(this, "Success", "Updated the user info!", false);
        } });

    // DISPLAY ALL BOOKS
    connect(books_Button, &QPushButton::clicked, [=]()
            {
        GeneralOperations generalOperations(libraryDb);

        // NO FILTER
        QList<LibrarySystem::Book> results = generalOperations.searchBook(
            "", "", "", "", "", "", "", "", "", "", "", "", ""
        );

        if (results.isEmpty()) {
            showMessage(this, "No Books Found", "No books found in the database.", true);
            return;
        }

        bookSearchWindow->graphical->displayBooksWithFilters(this, results); });

    // SERIAL PORT ADJUSTMENT

    connect(enterCOM_button, &QPushButton::clicked, [=]()
            {
        QDialog comDialog;
        comDialog.setWindowTitle("Enter Arduino COM Port");
        comDialog.setModal(true);
        comDialog.resize(300, 120);

        QVBoxLayout *layout = new QVBoxLayout(&comDialog);

        QLabel *label = new QLabel("Enter Arduino COM port (e.g., COM3):", &comDialog);
        // label->setStyleSheet("color: black;");
        QLineEdit *comEdit = new QLineEdit(&comDialog);
        // comEdit->setStyleSheet("color: black;");

        QSettings settings("LibSys", "ArduinoSettings"); // SAVE TO SETTINGS SO PROGRAM WOULD NOT FORGET
        QString savedPort = settings.value("ArduinoCOMPort", "").toString();
        if (!savedPort.isEmpty()) {
            comEdit->setText(savedPort);
        }

        QHBoxLayout *btnLayout = new QHBoxLayout();
        QPushButton *okBtn = new QPushButton("OK", &comDialog);
        //okBtn->setStyleSheet("color: black;");
        QPushButton *cancelBtn = new QPushButton("Cancel", &comDialog);
        //cancelBtn->setStyleSheet("color: black;");
        btnLayout->addWidget(okBtn);
        btnLayout->addWidget(cancelBtn);

        layout->addWidget(label);
        layout->addWidget(comEdit);
        layout->addLayout(btnLayout);

        QObject::connect(okBtn, &QPushButton::clicked, [&]() {
            QString port = comEdit->text().trimmed();
            if (!port.isEmpty()) {
                LibrarySystem::ArduinoCOMPort = port;

                settings.setValue("ArduinoCOMPort", port);

                showMessage(nullptr, "Saved", "Arduino COM port saved: " + port, false);
                comDialog.accept();
            } else {
                showMessage(nullptr, "Error", "Please enter a COM port!", true);
            }
        });

        QObject::connect(cancelBtn, &QPushButton::clicked, &comDialog, &QDialog::reject);

        comDialog.exec(); });

    // REPORT BOOK AS LOST

    connect(reportLostBook_Button, &QPushButton::clicked, [=]()
            {
                Graphical graphicalReportLostBook(this);
                bool success = graphicalReportLostBook.reportLostBookGraphical(this);

                if (!success)
                {
                    showMessage(this, "Error", "Could not perform the operation.", true);
                }
                else
                {
                    showMessage(this, "Success", "Operation successful.", false);
                } });

    // DISPLAY LOG HISTORY FROM log.log

    connect(logHistory_Button, &QPushButton::clicked, this, [this]()
            {

        if (!logWindow) {
            char key = 0x4B; // letter K
            std::vector<QString> logLines = readEncryptedLog(key);

            if (logLines.empty()) {
                showMessage(nullptr, "Log", "No log entries found.", false);
                return;
            }

            logWindow = new QWidget;
            logWindow->setWindowTitle("Log History");
            logWindow->resize(600, 400);

            QVBoxLayout *layout = new QVBoxLayout(logWindow);
            QPlainTextEdit *textEdit = new QPlainTextEdit;
            textEdit->setReadOnly(true);

            textEdit->setStyleSheet(
                "QPlainTextEdit { background-color: #ffffff; color: black; font-size: 12px; }"
            );

            for (const QString &line : logLines) {
                textEdit->appendPlainText(line);
            }

            layout->addWidget(textEdit);
            logWindow->setLayout(layout);

            logWindow->setAttribute(Qt::WA_DeleteOnClose);
            QObject::connect(logWindow, &QWidget::destroyed, [=]() {
                logWindow = nullptr;
            });

            logWindow->show();
        } else {
            logWindow->raise();
            logWindow->activateWindow();
        } });

    // SHOW ALL USERS IN DATABASE
    connect(users_Button, &QPushButton::clicked, this, [this]()
    {
        if (userWindow) {
            userWindow->raise();
            userWindow->activateWindow();
            return;
        }

        if (!userDb->openDB()) {
            showMessage(nullptr, "Error", "Could not open users.db", true);
            return;
        }

        QSqlQuery query = userDb->selectUsers("");

        userWindow = new QWidget;
        userWindow->setWindowTitle("Users List");
        userWindow->resize(600, 400);

        QVBoxLayout *layout = new QVBoxLayout(userWindow);

        // === Search Bar ===
        QLineEdit *searchEdit = new QLineEdit;
        searchEdit->setPlaceholderText("Enter school number to search...");
        layout->addWidget(searchEdit);

        QTableWidget *table = new QTableWidget;
        table->setColumnCount(5);
        table->setHorizontalHeaderLabels({"Username", "School No", "Password", "Account Type", "UID"});
        table->horizontalHeader()->setStretchLastSection(true);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);

        int row = 0;
        while (query.next()) {
            table->insertRow(row);

            QTableWidgetItem *usernameItem = new QTableWidgetItem(query.value("username").toString());
            QTableWidgetItem *schoolNoItem = new QTableWidgetItem(query.value("school_no").toString());
            QTableWidgetItem *passwordItem = new QTableWidgetItem(convertFromAes(query.value("password").toString())); 
            QTableWidgetItem *accountTypeItem = new QTableWidgetItem(query.value("account_type").toString());
            QTableWidgetItem *uidItem = new QTableWidgetItem(query.value("uid").toString());

            usernameItem->setForeground(QBrush(Qt::black));
            schoolNoItem->setForeground(QBrush(Qt::black));
            passwordItem->setForeground(QBrush(Qt::black));
            accountTypeItem->setForeground(QBrush(Qt::black));
            uidItem->setForeground(QBrush(Qt::black));

            table->setItem(row, 0, usernameItem);
            table->setItem(row, 1, schoolNoItem);
            table->setItem(row, 2, passwordItem);
            table->setItem(row, 3, accountTypeItem);
            table->setItem(row, 4, uidItem);

            row++;
        }

        layout->addWidget(table);
        userWindow->setLayout(layout);

        QObject::connect(searchEdit, &QLineEdit::textChanged, [table](const QString &text){
            for (int i = 0; i < table->rowCount(); ++i) {
                bool match = table->item(i, 1)->text().contains(text, Qt::CaseInsensitive);
                table->setRowHidden(i, !match);
            }
        });

        userWindow->setAttribute(Qt::WA_DeleteOnClose);
        QObject::connect(userWindow, &QWidget::destroyed, [this]() {
            userWindow = nullptr;
        });

        userWindow->show();
        userDb->closeDB();
    });

}

void AdminInterface::updateDateTime()
{
    dateLabel->setText(TimeClass::showDate());
    dayLabel->setText(TimeClass::showDay());
    timeLabel->setText(TimeClass::showTime());
}

AdminInterface::~AdminInterface()
{
    delete userDb;
    delete libraryDb;
}
void AdminInterface::updateRFIDLabel(const QString &RFIDdata)
{
    RFID_Data_Value->setText(RFIDdata);
}