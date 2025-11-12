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

#include "headers/Graphical.h"
#include "headers/Database.h"
#include "headers/GeneralOperations.h"

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

    username->setPlaceholderText("Username");
    schoolNo->setPlaceholderText("School Number");
    password->setPlaceholderText("Password");

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

                         bool isAdded = db.addUserIfNotExists(username->text(), schoolNo->text(), password->text(), accountType);
                         if (isAdded)
                             dialog.accept();
                     });
    QObject::connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    return dialog.exec() == QDialog::Accepted;
}

bool Graphical::deleteUserGraphical(QWidget *parent){
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

    dialog.setStyleSheet(
        "QDialog { background-color: #f0f0f0; color: black;}"
        "QLineEdit { color: black; border: 1px solid black; border-radius: 4px; padding: 3px; }"
        "QPushButton { background-color: #8b8b8b; color: white; border-radius: 4px; padding: 5px; }"
        "QPushButton:hover { background-color: #5f5f5f; }"
        "QPushButton:pressed { background-color: #353535ff; }"
        "QLabel {color: black; background-color: #f0f0f0;}"

    );

    QObject::connect(deleteButton, &QPushButton::clicked, [&]()
                     {
                         QString exePath = QCoreApplication::applicationDirPath();
                         QString userdbPath = exePath + "/users.db";

                         Database db(userdbPath, "DB_USERS");
                         if ((username->text() == ""))
                         {
                             dialog.reject();
                         }

                         bool isDeleted = db.deleteUser(username->text());
                         if (isDeleted)
                             dialog.accept();
                     });
                     
    QObject::connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    return dialog.exec() == QDialog::Accepted;
}

bool Graphical::updateUserGraphical(QWidget *parent){
    QDialog dialog(parent);
    dialog.setWindowTitle("Update An User's Information");

    QLineEdit *username = new QLineEdit;

    QLineEdit *schoolNo = new QLineEdit;

    QLineEdit *password = new QLineEdit;

    username->setPlaceholderText("Username");
    schoolNo->setPlaceholderText("School Number");
    password->setPlaceholderText("Password");

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

                         bool isUpdated = db.updateUserInfo(parent, username->text(), schoolNo->text(), password->text(), accountType);
                         if (isUpdated)
                             dialog.accept();
                     });
    QObject::connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    return dialog.exec() == QDialog::Accepted;
}

void Graphical::displayBooksWithFilters(QWidget *parent, QList<LibrarySystem::Book> results) {

    if (!bookWindow) {
        bookWindow = new QWidget(nullptr, Qt::Window);
        bookWindow->setWindowTitle("Filtered Books");
        bookWindow->resize(1100, 600);
        bookWindow->setAttribute(Qt::WA_DeleteOnClose);

        bookWindow->setStyleSheet(R"(
            QWidget {
                border: 1px solid black;
                border-radius: 2px;
                background-color: #cccbcb;
            }
        )");

        QVBoxLayout *layout = new QVBoxLayout(bookWindow);

        QTableWidget *table = new QTableWidget(bookWindow);
        table->setObjectName("BookTable");
        table->setColumnCount(18);
        table->setHorizontalHeaderLabels({
            "Title", "Author1", "Author2", "Author3", "Author4", "Author5",
            "Publisher", "Year", "Edition", "ISBN", "Volume", "Page Count",
            "Series Info", "Language", "DDC", "Additional Info", "Borrowed", "Borrowed By"
        });

        table->setShowGrid(true);
        table->setGridStyle(Qt::DashLine);
        table->horizontalHeader()->setStretchLastSection(true);
        table->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        table->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        table->setStyleSheet(R"(
            QTableWidget { font-size: 16px; gridline-color: #888888; }
            QTableWidget::item { color: black; }
            QHeaderView::section { background-color: #f0f0f0; color: black; border: 1px solid #a0a0a0; }
        )");

        layout->addWidget(table);

        QPushButton *closeBtn = new QPushButton("Close", bookWindow);
        closeBtn->setFixedSize(80, 25);
        layout->addWidget(closeBtn, 0, Qt::AlignRight);
        closeBtn->setStyleSheet(R"(
            QPushButton {
                color: black;
                background-color: #e9e8e8;
                border: 1px solid #a0a0a0;
                border-radius: 4px;
                font-size: 12px;
                padding: 2px 5px;
            }
            QPushButton:hover { background-color: #d0d0d0; border: 1px solid #888888; }
            QPushButton:pressed { background-color: #a8a8a8; border: 1px solid #555555; }
        )");

        QObject::connect(closeBtn, &QPushButton::clicked, bookWindow, &QWidget::close);
        QObject::connect(bookWindow, &QWidget::destroyed, [this](){
            this->bookWindow = nullptr;
        });

        int x = 0, y = 0;
        if (parent) {
            QRect parentRect = parent->geometry();
            x = parentRect.x() + (parentRect.width() - bookWindow->width()) / 2;
            y = parentRect.y() + (parentRect.height() - bookWindow->height()) / 2;
        } else {
            QScreen *screen = QGuiApplication::primaryScreen();
            QRect screenGeometry = screen->geometry();
            x = (screenGeometry.width() - bookWindow->width()) / 2;
            y = (screenGeometry.height() - bookWindow->height()) / 2;
        }
        bookWindow->move(x, y);
    }

    QTableWidget *table = bookWindow->findChild<QTableWidget*>("BookTable");
    if (table) {
        table->setRowCount(results.size());

        for (int row = 0; row < results.size(); ++row) {
            const LibrarySystem::Book &book = results[row];

            QStringList cells = {
                book.title, book.author1, book.author2, book.author3, book.author4, book.author5,
                book.publisher, book.publicationYear, book.edition, book.ISBN, book.volume,
                book.pageCount, book.seriesInformation, book.language, book.DDC, book.additionalInfo
            };

            for (int col = 0; col < cells.size(); ++col) {
                table->setItem(row, col, new QTableWidgetItem(cells[col]));
            }

            table->setItem(row, 16, new QTableWidgetItem(book.isBorrowed ? "Yes" : "No"));
            table->setItem(row, 17, new QTableWidgetItem(book.borrowedBy.isEmpty() ? "-" : book.borrowedBy));
        }
    }

    bookWindow->show();
    bookWindow->raise();
    bookWindow->activateWindow();
}


bool Graphical::borrowBookGraphical(QWidget *parent){

}

QTableWidget* Graphical::getBookTable() {
    if (bookWindow)
        return bookWindow->findChild<QTableWidget*>("BookTable");
    return nullptr;
}
