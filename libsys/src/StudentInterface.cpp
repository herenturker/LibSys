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
#include <QTimer>

#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStyle>
#include <QIcon>
#include <QDir>
#include <QDialog>
#include <QCalendarWidget>
#include <QTableWidget>
#include <QPointer>
#include <QListWidget>

#include "headers/StudentInterface.h"
#include "headers/StudentOperations.h"
#include "headers/LoginWindow.h"
#include "headers/TimeClass.h"
#include "headers/BookSearchWindow.h"
#include "headers/GeneralOperations.h"
#include "headers/database.h"
#include "headers/LibrarySystem.h"
#include "headers/Graphical.h"
#include "headers/Utils.h"

StudentInterface::StudentInterface(QWidget *parent) : QWidget(parent)
{
    graphical = new Graphical(this);
    QString exePath = QCoreApplication::applicationDirPath();
    QString dbDirPath = exePath + "/databases";

    QDir().mkpath(dbDirPath);

    QString userdbPath = dbDirPath + "/users.db";
    QString librarydbPath = dbDirPath + "/library.db";

    userDb = new Database(userdbPath, "DB_USERS");
    libraryDb = new Database(librarydbPath, "DB_LIBRARY");

    setWindowTitle("LibSys Student Dashboard");

    resize(1080, 720);
    setMinimumSize(1080, 720);
    setMaximumSize(1080, 720);

    dateLabel = new QLabel(this);
    dayLabel = new QLabel(this);
    timeLabel = new QLabel(this);

    RFID_Data = new QLabel("RFID Data", this);
    RFID_Data_Value = new QLabel("", this);

    RFID_Data->setObjectName("RFID_Data");
    RFID_Data_Value->setObjectName("RFID_Data_Value");

    RFID_Data->setGeometry(450, 140, 120, 30);
    RFID_Data_Value->setGeometry(450, 180, 120, 30);

    updateRFIDLabel(stdStringToQString(LibrarySystem::rfid_data));

    overdueBooks = new QLabel("Overdue Books", this);
    borrowedBooks = new QLabel("Borrowed Books", this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &StudentInterface::updateDateTime);
    timer->start(1000);

    dateLabel->setGeometry(75, 650, 200, 30);
    dayLabel->setGeometry(75, 620, 200, 30);
    timeLabel->setGeometry(75, 590, 200, 30);

    borrowedBooks->setGeometry(90, 140, 190, 40);
    overdueBooks->setGeometry(690, 140, 190, 40);

    borrowedBooksList = new QListWidget(this);
    borrowedBooksList->setGeometry(90, 190, 190, 400);
    // borrowedBooksList->setStyleSheet("color: black; font-size: 14px;");

    overdueBooksList = new QListWidget(this);
    overdueBooksList->setGeometry(690, 190, 190, 400);
    // overdueBooksList->setStyleSheet("color: black; font-size: 14px;");

    overdueBooks->setObjectName("overdueBooks");
    borrowedBooks->setObjectName("borrowedBooks");

    dateLabel->setObjectName("dateLabel");
    dayLabel->setObjectName("dayLabel");
    timeLabel->setObjectName("timeLabel");

    bookSearchWindow = new BookSearchWindow(this);

    openButton = new QPushButton("Open Book Search", this);
    openButton->setGeometry(500, 40, 150, 40);

    bookSearchWindow->close();

    connect(openButton, &QPushButton::clicked, [=]()
            {

        if (bookSearchWindow->isVisible()) {
            bookSearchWindow->close();
        } else {
            bookSearchWindow->show();
        } });

    searchContainer = new QWidget(this);
    searchContainer->setGeometry(75, 40, 400, 40);

    borrowBook_Button = new QPushButton("Borrow", this);
    borrowBook_Button->setToolTip("Borrow a book from the library.");

    returnBook_Button = new QPushButton("Return", this);
    returnBook_Button->setToolTip("Return a book to the library.");

    //myAccount_Button = new QPushButton("Account", this);
    //myAccount_Button->setToolTip("Display account settings");

    unsigned short buttonWidth = 130;
    unsigned short buttonHeight = 50;

    borrowBook_Button->setGeometry(540, 640, buttonWidth, buttonHeight);
    returnBook_Button->setGeometry(680, 640, buttonWidth, buttonHeight);
    //myAccount_Button->setGeometry(820, 640, buttonWidth, buttonHeight);

    QHBoxLayout *searchLayout = new QHBoxLayout(searchContainer);
    searchLayout->setContentsMargins(0, 0, 0, 0);

    QToolButton *searchButton = new QToolButton(searchContainer);
    searchButton->setIcon(QIcon(":/magnifier.ico"));
    searchButton->setIconSize(QSize(24, 24));
    searchButton->setCursor(Qt::PointingHandCursor);
    searchButton->setToolTip("Show search filters");

    QLineEdit *searchEdit = new QLineEdit(searchContainer);
    searchEdit->setPlaceholderText("Search for books, authors, or topics...");
    searchEdit->setMinimumHeight(32);

    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(searchEdit);

    searchButton->setObjectName("searchButton");
    searchEdit->setObjectName("searchEdit");
    openButton->setObjectName("bookSearchButton");

    connect(searchButton, &QToolButton::clicked, [=]()
            {
                QString bookTitle = bookSearchWindow->bookTitle->text();
                QString author1 = bookSearchWindow->author1->text();
                QString publisher = bookSearchWindow->publisher->text();
                QString publicationYear = bookSearchWindow->publicationYear->text();
                QString edition = bookSearchWindow->edition->text();
                QString ISBN = bookSearchWindow->ISBN->text();
                QString volume = bookSearchWindow->volume->text();
                QString pageCount = bookSearchWindow->pageCount->text();
                QString seriesInformation = bookSearchWindow->seriesInformation->text();
                QString language = bookSearchWindow->language->text();
                QString DDC = bookSearchWindow->DDC->text();
                QString additionalInfo = bookSearchWindow->additionalInfo->toPlainText();
                QString uid = bookSearchWindow->uid->text();

                GeneralOperations generalOperations(libraryDb);

                QList<LibrarySystem::Book> results = generalOperations.searchBook(
                    bookTitle, author1,
                    publisher, publicationYear, edition, ISBN,
                    volume, pageCount, seriesInformation, language, DDC, additionalInfo, uid);

                for (auto &book : results)
                {
                    QString borrowedBy = "";
                    if (libraryDb->getBookBorrowInfo(book.ISBN, borrowedBy))
                    {
                        book.isBorrowed = !borrowedBy.isEmpty();
                        book.borrowedBy = borrowedBy;
                    }
                    else
                    {
                        book.isBorrowed = false;
                        book.borrowedBy = "";
                    }
                }

                bookSearchWindow->graphical->displayBooksWithFilters(this, results);
            });

    connect(borrowBook_Button, &QPushButton::clicked, [=]()
            {
        QDialog *borrowDialog = new QDialog(this);
        borrowDialog->setWindowTitle("Borrow Book");
        borrowDialog->resize(300, 250);

        QVBoxLayout *layout = new QVBoxLayout(borrowDialog);

        QLabel *isbnLabel = new QLabel("Enter Book ISBN:", borrowDialog);
        // isbnLabel->setStyleSheet("color: black;");
        QLineEdit *isbnEdit = new QLineEdit(borrowDialog);
        // isbnEdit->setStyleSheet("color: black;");

        QLabel *uidLabel = new QLabel("Enter Book UID:", borrowDialog);
        // uidLabel->setStyleSheet("color: black;");
        QLineEdit *uidEdit = new QLineEdit(borrowDialog);
        // uidEdit->setStyleSheet("color: black;");

        layout->addWidget(isbnLabel);
        layout->addWidget(isbnEdit);

        layout->addWidget(uidLabel);
        layout->addWidget(uidEdit);

        QLabel *dueLabel = new QLabel("Select Due Date:", borrowDialog);
        // dueLabel->setStyleSheet("color: black;");
        QCalendarWidget *calendar = new QCalendarWidget(borrowDialog);

        calendar->setMinimumDate(QDate::currentDate());
        calendar->setMaximumDate(QDate::currentDate().addMonths(1));

        /*
        calendar->setStyleSheet(R"(
            QCalendarWidget QWidget { 
                background-color: #f0f0f0; 
                color: black;
            }
            QCalendarWidget QToolButton {
                background-color: #8b8b8b; 
                color: black;
                border-radius: 4px; 
                padding: 2px 5px;
            }
            QCalendarWidget QToolButton:hover {
                background-color: #999999;
            }
            QCalendarWidget QMenu {
                background-color: #f0f0f0; 
                color: black;
            }
            QCalendarWidget QSpinBox {
                min-width: 60px;
                color: black;
            }
            QCalendarWidget QAbstractItemView:enabled {
                color: black;
                background-color: white;
                selection-background-color: #0078d7;
                selection-color: white;
            }
            QCalendarWidget QAbstractItemView::item:selected {
                background-color: #0078d7;
                color: white;
            }
        )");
        */

        layout->addWidget(dueLabel);
        layout->addWidget(calendar);

        QHBoxLayout *btnLayout = new QHBoxLayout();
        QPushButton *okBtn = new QPushButton("OK", borrowDialog);
        // okBtn->setStyleSheet("color: black;");
        QPushButton *cancelBtn = new QPushButton("Cancel", borrowDialog);
        // cancelBtn->setStyleSheet("color: black;");
        btnLayout->addWidget(okBtn);
        btnLayout->addWidget(cancelBtn);
        layout->addLayout(btnLayout);

        uidEdit->setText(stdStringToQString(LibrarySystem::rfid_data));

        QString schoolNo = this->currentStudentSchoolNo;

        QObject::connect(okBtn, &QPushButton::clicked, [=]() {
            QString bookISBN = isbnEdit->text().trimmed();
            if (bookISBN.isEmpty()) {
                showMessage(this, "Error", "Please enter an ISBN!", true);
                return;
            }

            QString borrowedBy;
            libraryDb->openDB();
            if (libraryDb->getBookBorrowInfo(bookISBN, borrowedBy) && !borrowedBy.isEmpty()) {
                showMessage(this, "Error", "This book is already borrowed!", true);
                libraryDb->closeDB();
                return;
            }

            QDate borrowDate = QDate::currentDate();
            QDate dueDate = calendar->selectedDate();

            if (libraryDb->isBookExists(bookISBN) == false) {
                showMessage(this, "Error", "No book found with the provided ISBN!", true);
                libraryDb->closeDB();
                return;
            }

            if (libraryDb->getBorrowedBookCount(schoolNo) > 3) {
                showMessage(this, "Error", "You have reached the maximum borrow limit!", true);
                libraryDb->closeDB();
                return;
            }

            if (!libraryDb->borrowBook(schoolNo, bookISBN,
                            borrowDate.toString("yyyy-MM-dd"),
                            dueDate.toString("yyyy-MM-dd"), uidEdit->text())) {
                showMessage(this, "Error", "Failed to borrow book!", true);
            } else {
                showMessage(this, "Success", "Book borrowed successfully!", false);
                std::string logString = "BORROW: " + bookISBN.toStdString() + " by school number: " + schoolNo.toStdString();
                writeEncryptedLog(logString);
                refreshBorrowedBooks();
            }

            libraryDb->closeDB();
            borrowDialog->accept();
        });

        QObject::connect(cancelBtn, &QPushButton::clicked, borrowDialog, &QDialog::reject);

        borrowDialog->exec(); });

    connect(searchEdit, &QLineEdit::returnPressed, [=]()
            {
        QString query = searchEdit->text().trimmed();
        if (query.isEmpty()) {
            showMessage(this, "Error", "Please enter a search term!", true);
            return;
        }

        GeneralOperations generalOperations(libraryDb);

        QList<LibrarySystem::Book> results = generalOperations.searchBook(
            query, "", "", "", "", "", "", "", "", "", "", "", ""
        );

        for (auto &book : results) {
            QString borrowedBy;
            if (libraryDb->getBookBorrowInfo(book.ISBN, borrowedBy)) {
                book.isBorrowed = !borrowedBy.isEmpty();
                book.borrowedBy = borrowedBy;
            } else {
                book.isBorrowed = false;
                book.borrowedBy = "";
            }
        }

        bookSearchWindow->graphical->displayBooksWithFilters(this, results); });

    connect(returnBook_Button, &QPushButton::clicked, [this]()
            {
            QDialog *returnDialog = new QDialog(this);
            returnDialog->setWindowTitle("Return Book");
            returnDialog->resize(300, 100);

            QVBoxLayout *layout = new QVBoxLayout(returnDialog);
            QLabel *isbnLabel = new QLabel("Enter Book ISBN:", returnDialog);
            // isbnLabel->setStyleSheet("color: black;");
            QLineEdit *isbnEdit = new QLineEdit(returnDialog);
            // isbnEdit->setStyleSheet("color: black;");

            QLabel *uidLabel = new QLabel("Enter Book UID:", returnDialog);
            // uidLabel->setStyleSheet("color: black;");
            QLineEdit *uidEdit = new QLineEdit(returnDialog);
            // uidEdit->setStyleSheet("color: black;");

            layout->addWidget(isbnLabel);
            layout->addWidget(isbnEdit);

            layout->addWidget(uidLabel);
            layout->addWidget(uidEdit);

            QHBoxLayout *btnLayout = new QHBoxLayout();
            QPushButton *okBtn = new QPushButton("OK", returnDialog);
            // okBtn->setStyleSheet("color: black;");
            QPushButton *cancelBtn = new QPushButton("Cancel", returnDialog);
            // cancelBtn->setStyleSheet("color: black;");
            
            uidEdit->setText(stdStringToQString(LibrarySystem::rfid_data));

            btnLayout->addWidget(okBtn);
            btnLayout->addWidget(cancelBtn);
            layout->addLayout(btnLayout);

            QString schoolNo = this->currentStudentSchoolNo;

            connect(okBtn, &QPushButton::clicked, [this, returnDialog, isbnEdit, schoolNo, uidEdit]() {
                QString bookISBN = isbnEdit->text().trimmed();
                if (bookISBN.isEmpty()) {
                    showMessage(this, "Error", "Please enter an ISBN!", true);
                    return;
                }

                if (!libraryDb->openDB()) {
                    showMessage(this, "Error", "Could not open database!", true);
                    return;
                }

                if (!libraryDb->isBookBorrowedByStudent(schoolNo, bookISBN)) {
                    showMessage(this, "Error", "This book is not borrowed by you!", true);
                    libraryDb->closeDB();
                    return;
                }

                if (!libraryDb->returnBook(schoolNo, bookISBN, uidEdit->text())) {
                    showMessage(this, "Error", "Failed to return book!", true);
                } else {
                    showMessage(this, "Success", "Book returned successfully!", false);
                    std::string logString = "RETURN:  " + bookISBN.toStdString() + " by school number: " + schoolNo.toStdString();
                    writeEncryptedLog(logString);
                    refreshBorrowedBooks();
                }

                libraryDb->closeDB();
                returnDialog->accept();
            });

            connect(cancelBtn, &QPushButton::clicked, returnDialog, &QDialog::reject);

            returnDialog->exec(); });

            /*
    this->setStyleSheet(R"(
            QLabel#dateLabel, QLabel#dayLabel, QLabel#timeLabel {
                font-size: 20px;
                font-weight: bold;
                color: #333333;
            }

            QToolButton#searchButton, QLineEdit#searchEdit {
                color: black;
                font-size: 14px;
            }

            QPushButton#bookSearchButton, QToolButton#searchButton {

                color: black;
                font-size: 14px;
                border: 1px solid black;
                border-radius: 4px;
                background-color: #e9e9e9;
            
            }

            QPushButton#bookSearchButton:hover, QToolButton#searchButton:hover {
                border: 1px solid black;
                background-color: #a8a3a3;
            }

            QPushButton#bookSearchButton:pressed, QToolButton#searchButton:pressed {
                border: 1px solid black;
                background-color: #555252;
            }

            QLabel#overdueBooks, QLabel#borrowedBooks {
                font-size: 20px;
                font-weight: italic;
                color: #333333;
                border: 1px solid black;
            }
            QLabel#RFID_Data, QLabel#RFID_Data_Value {
                font-size: 20px;
                font-weight: bold;
                color: #8c1818;
        }

        )");

        */
}

void StudentInterface::updateDateTime()
{
    dateLabel->setText(TimeClass::showDate());
    dayLabel->setText(TimeClass::showDay());
    timeLabel->setText(TimeClass::showTime());
}

void StudentInterface::refreshBookLists()
{
    borrowedBooksList->clear();
    overdueBooksList->clear();

    if (!libraryDb->openDB())
    {
        qDebug() << "Cannot open library DB!";
        return;
    }

    QList<QMap<QString, QString>> books = libraryDb->getBorrowedBooksByStudent(currentStudentSchoolNo);
    int bookCounter = 1;

    QDate today = QDate::currentDate();

    for (const auto &book : books)
    {
        QString bookText = QString(
                               "BOOK: %1\n"
                               "Title: %2\n"
                               "Author: %3\n"
                               "Publisher: %4\n"
                               "Publication Year: %5\n"
                               "Edition: %6\n"
                               "ISBN: %7\n"
                               "Volume: %8\n"
                               "Page Count: %9\n"
                               "Series Information: %10\n"
                               "Language: %11\n"
                               "DDC: %12\n"
                               "Additional Info: %13\n"
                                "UID: %14\n\n")
                               .arg(bookCounter)
                               .arg(book.value("title"))
                               .arg(book.value("author1"))
                               .arg(book.value("publisher"))
                               .arg(book.value("publication_year"))
                               .arg(book.value("edition"))
                               .arg(book.value("isbn"))
                               .arg(book.value("volume"))
                               .arg(book.value("page_count"))
                               .arg(book.value("series_information"))
                               .arg(book.value("language"))
                               .arg(book.value("ddc"))
                               .arg(book.value("additional_info"))
                               .arg(book.value("uid"));

        // ---------- Borrowed Books ----------
        QListWidgetItem *borrowedItem = new QListWidgetItem(bookText, borrowedBooksList);
        borrowedItem->setTextAlignment(Qt::AlignLeft | Qt::AlignTop);
        borrowedItem->setSizeHint(QSize(borrowedItem->sizeHint().width(), 120));

        // ---------- Unreturned Books ----------
        // (This section has been removed as per recent edits)

        // ---------- Overdue Books ----------
        QDate dueDate = QDate::fromString(book.value("due_date"), "yyyy-MM-dd");
        if (dueDate.isValid() && dueDate < today)
        {
            QListWidgetItem *overdueItem = new QListWidgetItem(bookText, overdueBooksList);
            overdueItem->setTextAlignment(Qt::AlignLeft | Qt::AlignTop);
            overdueItem->setSizeHint(QSize(overdueItem->sizeHint().width(), 120));
        }

        bookCounter++;
    }

    libraryDb->closeDB();
}

void StudentInterface::setCurrentStudentSchoolNo(const QString &schoolNo)
{
    currentStudentSchoolNo = schoolNo;
    refreshBookLists();
    refreshBorrowedBooks();
}

QStringList StudentInterface::getBorrowedBooksTextList() const
{
    QStringList borrowedBooksText;

    if (!libraryDb->openDB())
    {
        return borrowedBooksText;
    }

    QList<QMap<QString, QString>> books = libraryDb->getBorrowedBooksByStudent(currentStudentSchoolNo);

    for (const auto &book : books)
    {
        QString bookLine = QString("Title: %1 | Author1: %2 | ISBN: %3 | Borrow Date: %4 | Due Date: %5 | Book UID: %6")
                               .arg(book["title"])
                               .arg(book["author1"])
                               .arg(book["isbn"])
                               .arg(book["borrow_date"])
                               .arg(book["due_date"])
                               .arg(book["uid"]);

        borrowedBooksText.append(bookLine);
    }

    libraryDb->closeDB();
    return borrowedBooksText;
}

void StudentInterface::refreshBorrowedBooks()
{
    borrowedBooksList->clear();

    if (!libraryDb->openDB())
    {
        return;
    }

    QList<QMap<QString, QString>> books = libraryDb->getBorrowedBooksByStudent(currentStudentSchoolNo);
    int bookCounter = 1;

    for (const auto &book : books)
    {
        QString bookText = QString(
                               "BOOK: %1\n"
                               "Title: %2\n"
                               "Author: %3\n"
                               "Publisher: %4\n"
                               "Publication Year: %5\n"
                               "Edition: %6\n"
                               "ISBN: %7\n"
                               "Volume: %8\n"
                               "Page Count: %9\n"
                               "Series Information: %10\n"
                               "Language: %11\n"
                               "DDC: %12\n"
                               "Additional Info: %13\n"
                               "UID: %14\n\n")
                               .arg(bookCounter++)
                               .arg(book.value("title"))
                               .arg(book.value("author1"))
                               .arg(book.value("publisher"))
                               .arg(book.value("publication_year"))
                               .arg(book.value("edition"))
                               .arg(book.value("isbn"))
                               .arg(book.value("volume"))
                               .arg(book.value("page_count"))
                               .arg(book.value("series_information"))
                               .arg(book.value("language"))
                               .arg(book.value("ddc"))
                               .arg(book.value("additional_info"))
                               .arg(book.value("uid"));

        QListWidgetItem *item = new QListWidgetItem(bookText, borrowedBooksList);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignTop);
        item->setSizeHint(QSize(item->sizeHint().width(), 120));
    }

    libraryDb->closeDB();
}
void StudentInterface::updateRFIDLabel(const QString &RFIDdata)
{
    RFID_Data_Value->setText(RFIDdata);
}
