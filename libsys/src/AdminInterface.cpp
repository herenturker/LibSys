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
#include "headers/StudentInterface.h"
#include "headers/LoginWindow.h"
#include "headers/TimeClass.h"
#include "headers/BookSearchWindow.h"
#include "headers/Database.h"
#include "headers/Graphical.h"
#include "headers/Utils.h"
#include "headers/LibrarySystem.h"
#include "headers/GeneralOperations.h"
#include "headers/Mailer.h"

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

    dateLabel = new QLabel(this);
    dayLabel = new QLabel(this);
    timeLabel = new QLabel(this);

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

    addEmail_Button = new QPushButton("Add Email", this);
    addEmail_Button->setToolTip("Add email address to an user.");

    reportLostBook_Button = new QPushButton("Report Lost Book", this);
    reportLostBook_Button->setToolTip("Report a lost book and update its record.");

    requests_Button = new QPushButton("Requests", this);
    requests_Button->setToolTip("View borrow & return requests.");

    addUser_Button = new QPushButton("Add User", this);
    addUser_Button->setToolTip("Add a new user to the users database.");

    deleteUser_Button = new QPushButton("Delete User", this);
    deleteUser_Button->setToolTip("Delete an user from the users database.");

    updateUserInfo_Button = new QPushButton("Update User Info", this);
    updateUserInfo_Button->setToolTip("Update or edit an user's info.");

    emailSettings_Button = new QPushButton("Email Config", this);
    emailSettings_Button->setToolTip("Configure email settings.");

    displayOverdueBooks_Button = new QPushButton("Overdue Books", this);
    displayOverdueBooks_Button->setToolTip("Display overdue books.");

    deleteEmail_Button = new QPushButton("Delete Email", this);
    deleteEmail_Button->setToolTip("Delete email address of an user.");

    updateEmail_Button = new QPushButton("Update Email", this);
    updateEmail_Button->setToolTip("Update email address of an user.");

    RFID_Data = new QLabel("RFID Data", this);
    RFID_Data_Value = new QLabel("", this);

    enterCOM_button = new QPushButton("COM", this);
    enterCOM_button->setToolTip("Enter COM Info for RFID Serial Port");

    checkOverdueBooks_Button = new QPushButton("Check Overdue Books", this);
    checkOverdueBooks_Button->setToolTip("Check Overdue Books");

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

    displayOverdueBooks_Button->setGeometry(710, 480, buttonWidth, buttonHeight);

    requests_Button->setGeometry(710, 550, buttonWidth, buttonHeight);
    emailSettings_Button->setGeometry(860, 550, buttonWidth, buttonHeight);

    addEmail_Button->setGeometry(860, 480, buttonWidth, buttonHeight);
    deleteEmail_Button->setGeometry(860, 410, buttonWidth, buttonHeight);
    updateEmail_Button->setGeometry(860, 340, buttonWidth, buttonHeight);
    checkOverdueBooks_Button->setGeometry(710, 340, buttonWidth, buttonHeight);

    enterCOM_button->setGeometry(710, 50, buttonWidth, buttonHeight);

    dateLabel->setGeometry(505, 620, 240, 30);
    dayLabel->setGeometry(285, 620, 210, 30);
    timeLabel->setGeometry(75, 620, 210, 30);

    addBook_Button->setGeometry(75, 215, buttonSquare, buttonSquare);

    deleteBook_Button->setGeometry(265, 215, buttonSquare, buttonSquare);

    changeBookInfo_Button->setGeometry(460, 215, buttonSquare, buttonSquare);

    reportLostBook_Button->setGeometry(710, 410, buttonWidth, buttonHeight);

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

    bookSearchWindow = new BookSearchWindow(this);

    // REQUESTS

    connect(requests_Button, &QPushButton::clicked, this, [this]()
            { showRequestsWindow(); });

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
        QLineEdit *comEdit = new QLineEdit(&comDialog);

        QSettings settings("LibSys", "ArduinoSettings"); // SAVE TO SETTINGS SO PROGRAM WOULD NOT FORGET
        QString savedPort = settings.value("ArduinoCOMPort", "").toString();
        if (!savedPort.isEmpty()) {
            comEdit->setText(savedPort);
        }

        QHBoxLayout *btnLayout = new QHBoxLayout();
        QPushButton *okBtn = new QPushButton("OK", &comDialog);
        QPushButton *cancelBtn = new QPushButton("Cancel", &comDialog);

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
        userWindow->resize(700, 400);

        QVBoxLayout *layout = new QVBoxLayout(userWindow);

        // === Search Bar ===
        QLineEdit *searchEdit = new QLineEdit;
        searchEdit->setPlaceholderText("Enter school number to search...");
        layout->addWidget(searchEdit);

        QTableWidget *table = new QTableWidget;
        table->setColumnCount(6);
        table->setHorizontalHeaderLabels({"Username", "School No", "Password", "Account Type", "UID", "Email"});
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
            QTableWidgetItem *emailItem = new QTableWidgetItem(userDb->getEmailBySchoolNo(query.value("school_no").toString()));

            usernameItem->setForeground(QBrush(Qt::black));
            schoolNoItem->setForeground(QBrush(Qt::black));
            passwordItem->setForeground(QBrush(Qt::black));
            accountTypeItem->setForeground(QBrush(Qt::black));
            uidItem->setForeground(QBrush(Qt::black));
            emailItem->setForeground(QBrush(Qt::black));

            table->setItem(row, 0, usernameItem);
            table->setItem(row, 1, schoolNoItem);
            table->setItem(row, 2, passwordItem);
            table->setItem(row, 3, accountTypeItem);
            table->setItem(row, 4, uidItem);
            table->setItem(row, 5, emailItem);

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
        userDb->closeDB(); });

    // DISPLAY OVERDUE BOOKS
    connect(displayOverdueBooks_Button, &QPushButton::clicked, [&]()
    {
        QStringList overdueBooks = libraryDb->getAllOverdueBooks();

        if (overdueBooks.isEmpty()) {
            QMessageBox::information(this, tr("No Overdue Books"), tr("There are no overdue books."));
            return;
        }

        QWidget *win = new QWidget(nullptr);
        win->setAttribute(Qt::WA_DeleteOnClose);
        win->setWindowTitle(tr("Overdue Books"));
        win->setMinimumSize(480, 360);
        win->setWindowModality(Qt::ApplicationModal);

        QVBoxLayout *mainLayout = new QVBoxLayout(win);
        mainLayout->setContentsMargins(8, 8, 8, 8);
        mainLayout->setSpacing(6);

        QScrollArea *scroll = new QScrollArea(win);
        scroll->setWidgetResizable(true);
        scroll->setFrameShape(QFrame::NoFrame);

        QWidget *container = new QWidget;
        QVBoxLayout *vbox = new QVBoxLayout(container);
        vbox->setContentsMargins(6, 6, 6, 6);
        vbox->setSpacing(8);

        for (const QString &s : overdueBooks) {
            QFrame *row = new QFrame(container);
            row->setFrameShape(QFrame::StyledPanel);
            row->setFrameShadow(QFrame::Raised);
            row->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

            QHBoxLayout *rowLayout = new QHBoxLayout(row);
            rowLayout->setContentsMargins(8, 6, 8, 6);

            QLabel *lbl = new QLabel(s, row);
            lbl->setWordWrap(true);
            lbl->setTextInteractionFlags(Qt::TextSelectableByMouse);
            rowLayout->addWidget(lbl);

            vbox->addWidget(row);
        }

        vbox->addStretch(1);

        container->setLayout(vbox);
        scroll->setWidget(container);

        QHBoxLayout *btnLayout = new QHBoxLayout;
        btnLayout->addStretch();
        QPushButton *closeBtn = new QPushButton(tr("Close"), win);
        QObject::connect(closeBtn, &QPushButton::clicked, win, &QWidget::close);
        btnLayout->addWidget(closeBtn);

        mainLayout->addWidget(scroll);
        mainLayout->addLayout(btnLayout);

        win->setLayout(mainLayout);
        win->show();
    });

    // CHECK OVERDUE BOOKS FOR ALL USERS.

    connect(checkOverdueBooks_Button, &QPushButton::clicked, this, [this](){
        QStringList allUIDs = userDb->getAllUserUIDs();

        for (const QString &uid : allUIDs) {
            QString accountType = userDb->getAccountTypeWithUID(uid);
            if (accountType == "Student") {
                QString school_no = userDb->getSchoolNoWithUID(uid);
                libraryDb->checkAndAddOverdueBooksForStudent(school_no);
            }
        }

        QMessageBox::information(this, tr("Done"), tr("Overdue books updated for all students."));
    });

    // ADD EMAIL TO AN USER
    connect(addEmail_Button, &QPushButton::clicked, this, [this]()
    {
        QDialog addEmailDialog;
        addEmailDialog.setWindowTitle("Add Email to an User");
        addEmailDialog.setModal(true);
        addEmailDialog.resize(300, 150);

        QVBoxLayout *layout = new QVBoxLayout(&addEmailDialog);

        QLabel *schoolNoLabel = new QLabel("Enter School No:", &addEmailDialog);
        QLineEdit *schoolNoEdit = new QLineEdit(&addEmailDialog);

        QLabel *emailLabel = new QLabel("Enter User Email:", &addEmailDialog);
        QLineEdit *emailEdit = new QLineEdit(&addEmailDialog);

        layout->addWidget(schoolNoLabel);
        layout->addWidget(schoolNoEdit);
        layout->addWidget(emailLabel);
        layout->addWidget(emailEdit);

        QHBoxLayout *btnLayout = new QHBoxLayout();
        QPushButton *okBtn = new QPushButton("OK", &addEmailDialog);
        QPushButton *cancelBtn = new QPushButton("Cancel", &addEmailDialog);
        btnLayout->addWidget(okBtn);
        btnLayout->addWidget(cancelBtn);
        layout->addLayout(btnLayout);

        QObject::connect(okBtn, &QPushButton::clicked, [&]() {
            QString schoolNo = schoolNoEdit->text().trimmed();
            QString email = emailEdit->text().trimmed();

            if (schoolNo.isEmpty() || email.isEmpty()) {
                showMessage(nullptr, "Error", "Please fill in both school number and email!", true);
                return;
            }

            if (userDb->addUserEmail(schoolNo, email)) {
                showMessage(nullptr, "Success", "Email added successfully to the database!", false);
                addEmailDialog.accept();
            } else {
                showMessage(nullptr, "Error", "Failed to add email. It might already exist!", true);
            }
        });

        QObject::connect(cancelBtn, &QPushButton::clicked, &addEmailDialog, &QDialog::reject);

        addEmailDialog.exec();
    });

    // DELETE EMAIL TO AN USER
    connect(deleteEmail_Button, &QPushButton::clicked, this, [this]()
    {
        QDialog deleteEmailDialog;
        deleteEmailDialog.setWindowTitle("Delete Email from an User");
        deleteEmailDialog.setModal(true);
        deleteEmailDialog.resize(300, 150);

        QVBoxLayout *layout = new QVBoxLayout(&deleteEmailDialog);

        QLabel *schoolNoLabel = new QLabel("Enter School No:", &deleteEmailDialog);
        QLineEdit *schoolNoEdit = new QLineEdit(&deleteEmailDialog);

        QLabel *emailLabel = new QLabel("Enter User Email:", &deleteEmailDialog);
        QLineEdit *emailEdit = new QLineEdit(&deleteEmailDialog);

        layout->addWidget(schoolNoLabel);
        layout->addWidget(schoolNoEdit);
        layout->addWidget(emailLabel);
        layout->addWidget(emailEdit);

        QHBoxLayout *btnLayout = new QHBoxLayout();
        QPushButton *okBtn = new QPushButton("OK", &deleteEmailDialog);
        QPushButton *cancelBtn = new QPushButton("Cancel", &deleteEmailDialog);
        btnLayout->addWidget(okBtn);
        btnLayout->addWidget(cancelBtn);
        layout->addLayout(btnLayout);

        QObject::connect(okBtn, &QPushButton::clicked, [&]() {
            QString schoolNo = schoolNoEdit->text().trimmed();
            QString email = emailEdit->text().trimmed();

            if (schoolNo.isEmpty() || email.isEmpty()) {
                showMessage(nullptr, "Error", "Please fill in both school number and email!", true);
                return;
            }

            if (userDb->deleteUserEmail(schoolNo, email)) {
                showMessage(nullptr, "Success", "Email deleted successfully!", false);
                deleteEmailDialog.accept();
            } else {
                showMessage(nullptr, "Error", "Failed to delete email.", true);
            }
        });

        QObject::connect(cancelBtn, &QPushButton::clicked, &deleteEmailDialog, &QDialog::reject);

        deleteEmailDialog.exec();
    });

    // UPDATE EMAIL OF AN USER
    connect(updateEmail_Button, &QPushButton::clicked, this, [this]()
    {
        QDialog updateEmailDialog;
        updateEmailDialog.setWindowTitle("Update Email of an User");
        updateEmailDialog.setModal(true);
        updateEmailDialog.resize(300, 150);

        QVBoxLayout *layout = new QVBoxLayout(&updateEmailDialog);

        QLabel *schoolNoLabel = new QLabel("Enter School No:", &updateEmailDialog);
        QLineEdit *schoolNoEdit = new QLineEdit(&updateEmailDialog);

        QLabel *emailLabel = new QLabel("Enter User Email:", &updateEmailDialog);
        QLineEdit *emailEdit = new QLineEdit(&updateEmailDialog);

        layout->addWidget(schoolNoLabel);
        layout->addWidget(schoolNoEdit);
        layout->addWidget(emailLabel);
        layout->addWidget(emailEdit);

        QHBoxLayout *btnLayout = new QHBoxLayout();
        QPushButton *okBtn = new QPushButton("OK", &updateEmailDialog);
        QPushButton *cancelBtn = new QPushButton("Cancel", &updateEmailDialog);
        btnLayout->addWidget(okBtn);
        btnLayout->addWidget(cancelBtn);
        layout->addLayout(btnLayout);

        QObject::connect(okBtn, &QPushButton::clicked, [&]() {
            QString schoolNo = schoolNoEdit->text().trimmed();
            QString email = emailEdit->text().trimmed();

            if (schoolNo.isEmpty() || email.isEmpty()) {
                showMessage(nullptr, "Error", "Please fill in both school number and email!", true);
                return;
            }

            if (userDb->updateUserEmail(schoolNo, email)) {
                showMessage(nullptr, "Success", "Email updated successfully!", false);
                updateEmailDialog.accept();
            } else {
                showMessage(nullptr, "Error", "Failed to update email.", true);
            }
        });

        QObject::connect(cancelBtn, &QPushButton::clicked, &updateEmailDialog, &QDialog::reject);

        updateEmailDialog.exec();
    });

    connect(emailSettings_Button, &QPushButton::clicked, this, [this]()
    {
        QDialog emailDialog;
        emailDialog.setWindowTitle("Enter Email Configuration");
        emailDialog.setModal(true);
        emailDialog.resize(300, 220);

        QVBoxLayout *layout = new QVBoxLayout(&emailDialog);

        QLabel *apiLabel = new QLabel("Enter API:", &emailDialog);
        QLineEdit *apiEdit = new QLineEdit(&emailDialog);

        QLabel *senderLabel = new QLabel("Enter Sender email:", &emailDialog);
        QLineEdit *senderEdit = new QLineEdit(&emailDialog);

        QLabel *adminLabel = new QLabel("Enter Admin email:", &emailDialog);
        QLineEdit *adminEdit = new QLineEdit(&emailDialog);

        QSettings settings("LibSys", "EmailSettings");

        QString savedApi = settings.value("EmailAPI", "").toString();
        if (!savedApi.isEmpty()) {
            apiEdit->setText(savedApi);
        }

        QString savedSender = settings.value("SenderEmail", "").toString();
        if (!savedSender.isEmpty()) {
            senderEdit->setText(savedSender);
        }

        QString savedAdmin = settings.value("AdminEmail", "").toString();
        if (!savedAdmin.isEmpty()) {
            adminEdit->setText(savedAdmin);
        }

        layout->addWidget(apiLabel);
        layout->addWidget(apiEdit);
        layout->addWidget(senderLabel);
        layout->addWidget(senderEdit);
        layout->addWidget(adminLabel);
        layout->addWidget(adminEdit);

        QHBoxLayout *btnLayout = new QHBoxLayout();
        QPushButton *okBtn = new QPushButton("OK", &emailDialog);
        QPushButton *cancelBtn = new QPushButton("Cancel", &emailDialog);
        btnLayout->addWidget(okBtn);
        btnLayout->addWidget(cancelBtn);
        layout->addLayout(btnLayout);

        QObject::connect(okBtn, &QPushButton::clicked, [&]() {
            QString api = apiEdit->text().trimmed();
            QString sender = senderEdit->text().trimmed();
            QString admin = adminEdit->text().trimmed();

            if (api.isEmpty() || sender.isEmpty() || admin.isEmpty()) {
                showMessage(nullptr, "Error", "Please fill in API, Sender email, and Admin email!", true);
                return;
            }

            settings.setValue("EmailAPI", api);
            settings.setValue("SenderEmail", sender);
            settings.setValue("AdminEmail", admin);

            showMessage(nullptr, "Saved", "Email configuration saved!", false);
            emailDialog.accept();
        });

        QObject::connect(cancelBtn, &QPushButton::clicked, &emailDialog, &QDialog::reject);

        emailDialog.exec();
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

    if (studentInterface)
        delete studentInterface;
}
void AdminInterface::updateRFIDLabel(const QString &RFIDdata)
{
    RFID_Data_Value->setText(RFIDdata);
}

void AdminInterface::showRequestsWindow()
{
    QWidget *reqWindow = new QWidget;
    reqWindow->setWindowTitle("Borrow / Return Requests");
    reqWindow->resize(800, 500);

    QVBoxLayout *layout = new QVBoxLayout(reqWindow);

    QTableWidget *table = new QTableWidget;
    table->setColumnCount(7);
    table->setHorizontalHeaderLabels({"Type", "School No", "Title", "Author", "ISBN", "Approve", "Reject"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QList<QMap<QString, QString>> borrowReqs = libraryDb->getBorrowRequests();
    QList<QMap<QString, QString>> returnReqs = libraryDb->getReturnRequests();

    int row = 0;

    for (auto &req : borrowReqs)
    {
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem("Borrow"));
        table->setItem(row, 1, new QTableWidgetItem(req["school_no"]));
        table->setItem(row, 2, new QTableWidgetItem(req["title"]));
        table->setItem(row, 3, new QTableWidgetItem(req["author1"]));
        table->setItem(row, 4, new QTableWidgetItem(req["book_isbn"]));

        QPushButton *approveBtn = new QPushButton("Approve");
        table->setCellWidget(row, 5, approveBtn);

        QPushButton *rejectBtn = new QPushButton("Reject");
        table->setCellWidget(row, 6, rejectBtn);

        connect(approveBtn, &QPushButton::clicked, this, [=]()
                {
            approveBorrowRequest(req["school_no"], req["title"], req["author1"]);
            if (studentInterface && studentInterface->getCurrentStudentSchoolNo() == req["school_no"]) {
                studentInterface->refreshBorrowedBooks(req["school_no"]);
            } });

        connect(rejectBtn, &QPushButton::clicked, this, [=]()
                {
            libraryDb->deleteBorrowRequest_TITLE_AUTHOR(req["school_no"], req["title"], req["author1"]);
            showMessage(nullptr, "Rejected", "Borrow request rejected.", false);
            if (studentInterface && studentInterface->getCurrentStudentSchoolNo() == req["school_no"]) {
                studentInterface->refreshBorrowedBooks(req["school_no"]);
            } });

        row++;
    }

    for (auto &req : returnReqs)
    {
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem("Return"));
        table->setItem(row, 1, new QTableWidgetItem(req["school_no"]));
        table->setItem(row, 2, new QTableWidgetItem(req["title"]));
        table->setItem(row, 3, new QTableWidgetItem(req["author1"]));
        table->setItem(row, 4, new QTableWidgetItem(req["book_isbn"]));

        QPushButton *approveBtn = new QPushButton("Approve");
        table->setCellWidget(row, 5, approveBtn);

        QPushButton *rejectBtn = new QPushButton("Reject");
        table->setCellWidget(row, 6, rejectBtn);

        connect(approveBtn, &QPushButton::clicked, this, [=]()
                {
            approveReturnRequest(req["school_no"], req["title"], req["author1"]);
            if (studentInterface && studentInterface->getCurrentStudentSchoolNo() == req["school_no"]) {
                studentInterface->refreshBorrowedBooks(req["school_no"]);
            } });

        connect(rejectBtn, &QPushButton::clicked, this, [=]()
                {
            libraryDb->deleteReturnRequest_TITLE_AUTHOR(req["school_no"], req["title"], req["author1"]);
            showMessage(nullptr, "Rejected", "Return request rejected.", false);
            if (studentInterface && studentInterface->getCurrentStudentSchoolNo() == req["school_no"]) {
                studentInterface->refreshBorrowedBooks(req["school_no"]);
            } });

        row++;
    }

    layout->addWidget(table);
    reqWindow->show();
}

void AdminInterface::approveBorrowRequest(const QString &schoolNo,
                                          const QString &title,
                                          const QString &author1)
{
    libraryDb->deleteBorrowRequest_TITLE_AUTHOR(schoolNo, title, author1);

    QString borrowDate = QDate::currentDate().toString("yyyy-MM-dd");
    QString dueDate = QDate::currentDate().addDays(15).toString("yyyy-MM-dd");

    bool ok = libraryDb->borrowBook_TITLE_AUTHOR(schoolNo, borrowDate, dueDate, title, author1);

    if (!ok)
    {
        showMessage(nullptr, "Error", "Borrowing failed (maybe book is already borrowed or LOST).", true);
        return;
    }
    emit bookBorrowed(schoolNo);
    showMessage(nullptr, "Success", "Borrow request approved and book assigned.", false);

    std::string logStr = "APPROVED BORROW: " + title.toStdString() + " by " + author1.toStdString() +
                         " to student " + schoolNo.toStdString();
    writeEncryptedLog(logStr);
}

void AdminInterface::approveReturnRequest(const QString &schoolNo,
                                          const QString &title,
                                          const QString &author1)
{
    libraryDb->deleteReturnRequest_TITLE_AUTHOR(schoolNo, title, author1);

    bool ok = libraryDb->returnBook_TITLE_AUTHOR(schoolNo, title, author1);

    if (!ok)
    {
        showMessage(nullptr, "Error", "Returning failed (maybe book is LOST or not borrowed).", true);
        return;
    }
    emit bookReturned(schoolNo);

    showMessage(nullptr, "Success", "Return request approved and book returned.", false);
    libraryDb->deleteOverdueBook(schoolNo, title, author1);
    std::string logStr = "APPROVED RETURN: " + title.toStdString() + " by " + author1.toStdString() +
                         " from student " + schoolNo.toStdString();
    writeEncryptedLog(logStr);
}

bool Database::deleteBorrowRequest_TITLE_AUTHOR(const QString &schoolNo,
                                                const QString &title,
                                                const QString &author1)
{
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM borrow_requests WHERE school_no=? AND title=? AND author1=?");
    q.addBindValue(schoolNo);
    q.addBindValue(title);
    q.addBindValue(author1);
    return q.exec();
}

bool Database::deleteReturnRequest_TITLE_AUTHOR(const QString &schoolNo,
                                                const QString &title,
                                                const QString &author1)
{
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM return_requests WHERE school_no=? AND title=? AND author1=?");
    q.addBindValue(schoolNo);
    q.addBindValue(title);
    q.addBindValue(author1);
    return q.exec();
}
