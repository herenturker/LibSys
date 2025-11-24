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

#include <QMessageBox>
#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QRadioButton>
#include <QCoreApplication>
#include <QTableWidget>
#include <QHeaderView>
#include <QGuiApplication>
#include <QScreen>
#include <QDir>
#include <QInputDialog>
#include <QtSql/QSqlError>

#include "headers/Graphical.h"
#include "headers/Database.h"
#include "headers/GeneralOperations.h"
#include "headers/Utils.h"
#include "headers/LibrarySystem.h"

Graphical::Graphical(QWidget *parent)
{
    Q_UNUSED(parent);
}

bool Graphical::performAction(QWidget *parent, const QString &text)
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        parent,
        "Confirmation",
        text,
        QMessageBox::Yes | QMessageBox::No);
    return reply == QMessageBox::Yes;
}

bool Graphical::addUserGraphical(QWidget *parent)
{
    QDialog dialog(parent);
    dialog.setWindowTitle("Enter New User's Information");

    QLineEdit *username = new QLineEdit;

    QLineEdit *schoolNo = new QLineEdit;

    QLineEdit *password = new QLineEdit;

    QLineEdit *uid = new QLineEdit;

    uid->setText(stdStringToQString(LibrarySystem::rfid_data));

    username->setPlaceholderText("Username");
    schoolNo->setPlaceholderText("School Number");
    password->setPlaceholderText("Password");
    uid->setPlaceholderText("UID");

    QButtonGroup *radioButton_Group = new QButtonGroup(&dialog);
    QRadioButton *accountType_Admin_Button = new QRadioButton("Admin", &dialog);
    QRadioButton *accountType_Student_Button = new QRadioButton("Student", &dialog);
    accountType_Student_Button->setChecked(true);

    radioButton_Group->addButton(accountType_Student_Button);
    radioButton_Group->addButton(accountType_Admin_Button);

    QFormLayout *formLayout = new QFormLayout;

    formLayout->addRow("Username:", username);
    formLayout->addRow("School Number:", schoolNo);
    formLayout->addRow("Password:", password);
    formLayout->addRow("UID:", uid);

    QHBoxLayout *radioLayout = new QHBoxLayout;
    radioLayout->addWidget(accountType_Student_Button);
    radioLayout->addWidget(accountType_Admin_Button);
    formLayout->addRow("Account Type:", radioLayout);

    QPushButton *addButton = new QPushButton("Add");
    QPushButton *cancelButton = new QPushButton("Cancel");

    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    buttonsLayout->addWidget(addButton);
    buttonsLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonsLayout);

    /*
    dialog.setStyleSheet(
        "QDialog { background-color: #f0f0f0; color: black;}"
        "QLineEdit { color: black; border: 1px solid black; border-radius: 4px; padding: 3px; }"
        "QPushButton { background-color: #8b8b8b; color: white; border-radius: 4px; padding: 5px; }"
        "QPushButton:hover { background-color: #5f5f5f; }"
        "QPushButton:pressed { background-color: #353535ff; }"
        "QLabel {color: black; background-color: #f0f0f0;}"
        "QRadioButton { padding: 2px; }"
        "QRadioButton::indicator:checked { background-color: #8a0b0b; border-radius: 6px; }"

    );
    */

    QObject::connect(addButton, &QPushButton::clicked, [&]()
                     {
                         QString accountType = radioButton_Group->checkedButton()->text();
                         QString exePath = QCoreApplication::applicationDirPath();
                         QString userdbPath = exePath + "/users.db";

                         Database db(userdbPath, "DB_USERS");
                         if ((username->text() == "") || (schoolNo->text() == "") || (password->text() == "") || (accountType == ""))
                         {
                             dialog.reject();
                         }

                         bool isAdded = db.addUserIfNotExists(username->text(), schoolNo->text(), password->text(), accountType, uid->text());
                         if (isAdded)
                             dialog.accept(); });

    QObject::connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    return dialog.exec() == QDialog::Accepted;
}

bool Graphical::deleteUserGraphical(QWidget *parent)
{
    QDialog dialog(parent);
    dialog.setWindowTitle("Delete An User from LibSys");

    QLineEdit *username = new QLineEdit;

    username->setPlaceholderText("Username");

    QFormLayout *formLayout = new QFormLayout;

    formLayout->addRow("Username:", username);

    QPushButton *deleteButton = new QPushButton("Delete");
    QPushButton *cancelButton = new QPushButton("Cancel");

    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    buttonsLayout->addWidget(deleteButton);
    buttonsLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonsLayout);

    QObject::connect(deleteButton, &QPushButton::clicked, [&]()
                     {
                         QString exePath = QCoreApplication::applicationDirPath();
                         QString userdbPath = exePath + "/users.db";

                         Database db(userdbPath, "DB_USERS");
                         if ((username->text() == "") || (username->text() == "Admin"))
                         {
                             dialog.reject();
                         }

                         bool isDeleted = db.deleteUser(username->text());
                         if (isDeleted)
                             dialog.accept(); });

    QObject::connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    return dialog.exec() == QDialog::Accepted;
}

bool Graphical::updateUserGraphical(QWidget *parent)
{
    QDialog dialog(parent);
    dialog.setWindowTitle("Update An User's Information");

    QLineEdit *username = new QLineEdit;

    QLineEdit *schoolNo = new QLineEdit;

    QLineEdit *password = new QLineEdit;

    QLineEdit *uid = new QLineEdit;

    username->setPlaceholderText("Username");
    schoolNo->setPlaceholderText("School Number");
    password->setPlaceholderText("Password");
    uid->setPlaceholderText("UID");

    uid->setText(stdStringToQString(LibrarySystem::rfid_data));

    QButtonGroup *radioButton_Group = new QButtonGroup(&dialog);
    QRadioButton *accountType_Admin_Button = new QRadioButton("Admin", &dialog);
    QRadioButton *accountType_Student_Button = new QRadioButton("Student", &dialog);
    accountType_Student_Button->setChecked(true);

    radioButton_Group->addButton(accountType_Student_Button);
    radioButton_Group->addButton(accountType_Admin_Button);

    QFormLayout *formLayout = new QFormLayout;

    formLayout->addRow("Username:", username);
    formLayout->addRow("School Number:", schoolNo);
    formLayout->addRow("Password:", password);
    formLayout->addRow("UID:", uid);

    QHBoxLayout *radioLayout = new QHBoxLayout;
    radioLayout->addWidget(accountType_Student_Button);
    radioLayout->addWidget(accountType_Admin_Button);
    formLayout->addRow("Account Type:", radioLayout);

    QPushButton *updateButton = new QPushButton("Update");
    QPushButton *cancelButton = new QPushButton("Cancel");

    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    buttonsLayout->addWidget(updateButton);
    buttonsLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonsLayout);

    QObject::connect(updateButton, &QPushButton::clicked, [&]()
                     {
                         QString accountType = radioButton_Group->checkedButton()->text();
                         QString exePath = QCoreApplication::applicationDirPath();
                         QString userdbPath = exePath + "/users.db";

                         Database db(userdbPath, "DB_USERS");
                         if ((username->text() == "") || (schoolNo->text() == "") || (password->text() == "") || (accountType == ""))
                         {
                             dialog.reject();
                         }

                         bool isUpdated = db.updateUserInfo(parent, username->text(), schoolNo->text(), password->text(), accountType, uid->text());
                         if (isUpdated)
                             dialog.accept(); });

    QObject::connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    return dialog.exec() == QDialog::Accepted;
}

void Graphical::displayBooksWithFilters(QWidget *parent, QList<LibrarySystem::Book> results)
{
    if (!bookWindow)
    {
        bookWindow = new QWidget(nullptr, Qt::Window);
        bookWindow->setWindowTitle("Filtered Books");
        bookWindow->resize(1150, 600);
        bookWindow->setAttribute(Qt::WA_DeleteOnClose);

        QVBoxLayout *layout = new QVBoxLayout(bookWindow);

        bookTable = new QTableWidget(bookWindow);
        bookTable->setObjectName("BookTable");
        bookTable->setColumnCount(15);
        bookTable->setHorizontalHeaderLabels({"Title", "Author",
                                              "Publisher", "Year", "Edition", "ISBN", "Volume", "Page Count",
                                              "Series Info", "Language", "DDC", "Additional Info", "Borrowed", "Borrowed By", "UID"});
        bookTable->setShowGrid(true);
        bookTable->horizontalHeader()->setStretchLastSection(true);
        layout->addWidget(bookTable);

        QPushButton *closeBtn = new QPushButton("Close", bookWindow);
        closeBtn->setFixedSize(80, 25);
        layout->addWidget(closeBtn, 0, Qt::AlignRight);

        QObject::connect(closeBtn, &QPushButton::clicked, bookWindow, &QWidget::close);
        QObject::connect(bookWindow, &QWidget::destroyed, [this](){ this->bookWindow = nullptr; });

        int x = 0, y = 0;
        if (parent)
        {
            QRect parentRect = parent->geometry();
            x = parentRect.x() + (parentRect.width() - bookWindow->width()) / 2;
            y = parentRect.y() + (parentRect.height() - bookWindow->height()) / 2;
        }
        else
        {
            QScreen *screen = QGuiApplication::primaryScreen();
            QRect screenGeometry = screen->geometry();
            x = (screenGeometry.width() - bookWindow->width()) / 2;
            y = (screenGeometry.height() - bookWindow->height()) / 2;
        }
        bookWindow->move(x, y);
    }

    QTableWidget *table = bookWindow->findChild<QTableWidget *>("BookTable");
    if (!table) return;

    table->clearContents();
    table->setRowCount(results.size());

    for (int row = 0; row < results.size(); ++row)
    {
        const LibrarySystem::Book &book = results[row];

        QStringList cells = {
            book.title, book.author1,
            book.publisher, book.publicationYear, book.edition, book.ISBN, book.volume,
            book.pageCount, book.seriesInformation, book.language, book.DDC, book.additionalInfo
        };

        for (int col = 0; col < cells.size(); ++col)
            table->setItem(row, col, new QTableWidgetItem(cells[col]));

        table->setItem(row, 12, new QTableWidgetItem(book.isBorrowed ? "Yes" : "No"));
        table->setItem(row, 13, new QTableWidgetItem(book.borrowedBy.isEmpty() ? "-" : book.borrowedBy));
        table->setItem(row, 14, new QTableWidgetItem(book.uid.isEmpty() ? "-" : book.uid));
    }

    bookWindow->show();
}


QTableWidget *Graphical::getBookTable()
{
    return bookTable;
}

bool Graphical::reportLostBookGraphical(QWidget *parent)
{
    QString exePath = QCoreApplication::applicationDirPath();
    QString dbDirPath = exePath + "/databases";

    QDir().mkpath(dbDirPath);

    QString librarydbPath = dbDirPath + "/library.db";
    libraryDb = new Database(librarydbPath, "DB_LIBRARY");

    QInputDialog inputDialog(parent);
    inputDialog.setWindowTitle("Report Lost Book");
    inputDialog.setLabelText("Enter ISBN of the lost book:");
    inputDialog.setTextValue("");
    inputDialog.setInputMode(QInputDialog::TextInput);
    inputDialog.setModal(true);

    inputDialog.setStyleSheet(R"(
        QLabel { font-weight: bold; }
    )");

    bool ok = false;
    QString ISBN;
    if (inputDialog.exec() == QDialog::Accepted)
    {
        ISBN = inputDialog.textValue().trimmed();
        if (ISBN.isEmpty())
        {
            showMessage(parent, "Input Error", "ISBN cannot be empty.", true);
            return false;
        }
        ok = true;
    }

    if (!ok)
        return false;

    QSqlQuery checkQuery(libraryDb->getDB());
    checkQuery.prepare("SELECT additional_info FROM books WHERE isbn = :isbn");
    checkQuery.bindValue(":isbn", ISBN);

    if (!checkQuery.exec() || !checkQuery.next())
    {
        showMessage(parent, "Database Error", "Failed to query the database.", true);
        return false;
    }

    QString currentInfo = checkQuery.value(0).toString();

    QSqlQuery updateQuery(libraryDb->getDB());
    if (currentInfo == "LOST")
    {
        updateQuery.prepare("UPDATE books SET additional_info = '' WHERE isbn = :isbn");
        updateQuery.bindValue(":isbn", ISBN);

        if (!updateQuery.exec())
        {
            showMessage(parent, "Update Error", "Could not restore book info: " + updateQuery.lastError().text(), true);
            return false;
        }

        showMessage(parent, "Success", "Book with ISBN " + ISBN + " restored from LOST.", false);
    }
    else
    {
        updateQuery.prepare("UPDATE books SET additional_info = 'LOST' WHERE isbn = :isbn");
        updateQuery.bindValue(":isbn", ISBN);

        if (!updateQuery.exec())
        {
            showMessage(parent, "Update Error", "Could not mark book as lost: " + updateQuery.lastError().text(), true);
            return false;
        }

        showMessage(parent, "Success", "Book with ISBN " + ISBN + " marked as LOST.", false);
    }

    return true;
}
