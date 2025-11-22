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
#include <QGroupBox>
#include <QComboBox>
#include <QApplication>
#include <QClipboard>

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

    overdueBooksList = new QListWidget(this);
    overdueBooksList->setGeometry(690, 190, 190, 400);

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

    myAccount_Button = new QPushButton("Account", this);
    myAccount_Button->setToolTip("Display account settings.");

    bookCitation_Button = new QPushButton("Citation", this);
    bookCitation_Button->setToolTip("Perform book citation.");

    unsigned short buttonWidth = 130;
    unsigned short buttonHeight = 50;

    borrowBook_Button->setGeometry(540, 640, buttonWidth, buttonHeight);
    returnBook_Button->setGeometry(680, 640, buttonWidth, buttonHeight);
    myAccount_Button->setGeometry(820, 640, buttonWidth, buttonHeight);
    bookCitation_Button->setGeometry(450, 250, buttonWidth, buttonHeight);

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

    connect(myAccount_Button, &QPushButton::clicked, [=]()
    {
        QDialog *infoDialog = new QDialog(this);
        infoDialog->setWindowTitle("My Account Info");
        infoDialog->setFixedSize(380, 230);

        QVBoxLayout *mainLayout = new QVBoxLayout(infoDialog);

        QString studentUID = userDb->getUIDWithSchoolNo(currentStudentSchoolNo);

        QGridLayout *grid = new QGridLayout();

        auto makeBold = [](const QString &text){
            QLabel *lbl = new QLabel("<b>" + text + "</b>");
            return lbl;
        };

        grid->addWidget(makeBold("Username:"),      0, 0);
        grid->addWidget(new QLabel(userDb->getUsernameWithUID(studentUID)), 0, 1);

        grid->addWidget(makeBold("School Number:"), 1, 0);
        grid->addWidget(new QLabel(currentStudentSchoolNo), 1, 1);

        grid->addWidget(makeBold("Password:"),      2, 0);
        grid->addWidget(new QLabel(convertFromAes(userDb->getPasswordWithUID(studentUID))), 2, 1);

        grid->addWidget(makeBold("UID:"),           3, 0);
        grid->addWidget(new QLabel(studentUID), 3, 1);

        grid->setVerticalSpacing(10);
        grid->setHorizontalSpacing(15);

        mainLayout->addLayout(grid);

        // Close button
        QPushButton *closeBtn = new QPushButton("Close", infoDialog);
        closeBtn->setFixedWidth(80);
        closeBtn->setStyleSheet("padding: 5px;");
        connect(closeBtn, &QPushButton::clicked, infoDialog, &QDialog::accept);

        mainLayout->addWidget(closeBtn, 0, Qt::AlignRight);

        infoDialog->exec();
    });

    connect(bookCitation_Button, &QPushButton::clicked, [=]()
    {
        QDialog *citationDialog = new QDialog(this);
        citationDialog->setWindowTitle("Citation Generator");
        citationDialog->setFixedSize(840, 610);

        QVBoxLayout *mainLayout = new QVBoxLayout(citationDialog);

        QGroupBox *infoBox = new QGroupBox("Book Information");
        QGridLayout *grid = new QGridLayout(infoBox);

        QLineEdit *titleEdit = new QLineEdit();
        QLineEdit *authorEdit = new QLineEdit();
        QLineEdit *yearEdit = new QLineEdit();
        QLineEdit *publisherEdit = new QLineEdit();
        QLineEdit *editionEdit = new QLineEdit();
        QLineEdit *placeEdit = new QLineEdit();
        QLineEdit *pagesEdit = new QLineEdit();
        QLineEdit *doiEdit = new QLineEdit();

        grid->addWidget(new QLabel("Title:"), 0, 0);
        grid->addWidget(titleEdit, 0, 1);

        grid->addWidget(new QLabel("Author(s):"), 1, 0);
        grid->addWidget(authorEdit, 1, 1);

        grid->addWidget(new QLabel("Year:"), 2, 0);
        grid->addWidget(yearEdit, 2, 1);

        grid->addWidget(new QLabel("Publisher:"), 3, 0);
        grid->addWidget(publisherEdit, 3, 1);

        grid->addWidget(new QLabel("Edition:"), 4, 0);
        grid->addWidget(editionEdit, 4, 1);

        grid->addWidget(new QLabel("Place:"), 5, 0);
        grid->addWidget(placeEdit, 5, 1);

        grid->addWidget(new QLabel("Pages (optional):"), 6, 0);
        grid->addWidget(pagesEdit, 6, 1);

        grid->addWidget(new QLabel("DOI (optional):"), 7, 0);
        grid->addWidget(doiEdit, 7, 1);

        mainLayout->addWidget(infoBox);

        QHBoxLayout *formatLayout = new QHBoxLayout();
        QLabel *formatLabel = new QLabel("Format:");
        QComboBox *formatCombo = new QComboBox();
        formatCombo->addItems({"APA", "MLA", "Chicago", "Harvard", "IEEE", "AMA"});

        formatLayout->addWidget(formatLabel);
        formatLayout->addWidget(formatCombo);
        mainLayout->addLayout(formatLayout);

        QTextEdit *outputEdit = new QTextEdit();
        outputEdit->setReadOnly(true);
        mainLayout->addWidget(outputEdit);

        QHBoxLayout *btnLayout = new QHBoxLayout();
        QPushButton *copyBtn = new QPushButton("Copy");
        QPushButton *exportBibBtn = new QPushButton("Export BibTeX");
        QPushButton *closeBtn = new QPushButton("Close");

        btnLayout->addWidget(copyBtn);
        btnLayout->addWidget(exportBibBtn);
        btnLayout->addStretch();
        btnLayout->addWidget(closeBtn);

        mainLayout->addLayout(btnLayout);

        auto splitAuthors = [&](QString authors) {
            QStringList list = authors.split(",", Qt::SkipEmptyParts);
            QStringList converted;
            for (QString a : list) {
                QStringList parts = a.trimmed().split(" ");
                if (parts.size() < 2)
                    converted.append(a.trimmed());
                else {
                    QString last = parts.takeLast();
                    QString first = parts.join(" ");
                    converted.append(last + ", " + first);
                }
            }
            return converted;
        };

        auto gen = [&]() {
            QString t = titleEdit->text();
            QString a = authorEdit->text();
            QString y = yearEdit->text();
            QString p = publisherEdit->text();
            QString e = editionEdit->text();
            QString c = placeEdit->text();
            QString pages = pagesEdit->text();
            QString doi = doiEdit->text();

            QStringList authAPA = splitAuthors(a);
            QStringList authMLA = splitAuthors(a);

            QString authAPA_joined = authAPA.join(", ");
            QString authMLA_first = authMLA.join(", ");

            QString italTitle = "<i>" + t + "</i>";

            QString result;
            QString format = formatCombo->currentText();

            if (format == "APA") {
                result = QString("%1. (%2). %3. %4: %5.")
                            .arg(authAPA_joined).arg(y).arg(italTitle).arg(c).arg(p);
                if (!pages.isEmpty()) result += " pp. " + pages + ".";
                if (!doi.isEmpty()) result += " https://doi.org/" + doi;
            }
            else if (format == "MLA") {
                result = QString("%1. %2. %3, %4.")
                            .arg(authMLA_first).arg(italTitle).arg(p).arg(y);
                if (!pages.isEmpty()) result += " pp. " + pages + ".";
            }
            else if (format == "Chicago") {
                result = QString("%1. %2. %3: %4, %5.")
                            .arg(authAPA_joined).arg(italTitle).arg(c).arg(p).arg(y);
                if (!pages.isEmpty()) result += " pp. " + pages + ".";
            }
            else if (format == "Harvard") {
                result = QString("%1 (%2) %3. %4: %5.")
                            .arg(authAPA_joined).arg(y).arg(italTitle).arg(c).arg(p);
                if (!pages.isEmpty()) result += " pp. " + pages + ".";
            }
            else if (format == "IEEE") {
                result = QString("%1, %2, %3, %4, %5.")
                            .arg(authAPA_joined.replace(",", ""))
                            .arg(italTitle).arg(c).arg(p).arg(y);
                if (!pages.isEmpty()) result += " pp. " + pages + ".";
            }
            else if (format == "AMA") {
                result = QString("%1. %2. %3: %4; %5.")
                            .arg(authAPA_joined.replace(",", ""))
                            .arg(italTitle).arg(c).arg(p).arg(y);
                if (!pages.isEmpty()) result += " " + pages + ".";
            }

            outputEdit->setHtml(result);
        };

        connect(titleEdit, &QLineEdit::textChanged, gen);
        connect(authorEdit, &QLineEdit::textChanged, gen);
        connect(yearEdit, &QLineEdit::textChanged, gen);
        connect(publisherEdit, &QLineEdit::textChanged, gen);
        connect(editionEdit, &QLineEdit::textChanged, gen);
        connect(placeEdit, &QLineEdit::textChanged, gen);
        connect(pagesEdit, &QLineEdit::textChanged, gen);
        connect(doiEdit, &QLineEdit::textChanged, gen);
        connect(formatCombo, &QComboBox::currentTextChanged, gen);

        connect(copyBtn, &QPushButton::clicked, [=](){
            QApplication::clipboard()->setText(outputEdit->toPlainText());
        });

        connect(closeBtn, &QPushButton::clicked, citationDialog, &QDialog::accept);

        connect(exportBibBtn, &QPushButton::clicked, [=]() {
            QString t = titleEdit->text();
            QString a = authorEdit->text();
            QString y = yearEdit->text();
            QString p = publisherEdit->text();
            QString e = editionEdit->text();
            QString c = placeEdit->text();
            QString pages = pagesEdit->text();
            QString doi = doiEdit->text();

            QStringList authors = a.split(",", Qt::SkipEmptyParts);
            for (int i=0;i<authors.size();i++) authors[i] = authors[i].trimmed();

            QString bibKey = authors.first().split(" ").last() + y;

            QString bib = "@book{" + bibKey + ",\n";
            bib += "  author = {" + authors.join(" and ") + "},\n";
            bib += "  title = {" + t + "},\n";
            bib += "  year = {" + y + "},\n";
            bib += "  publisher = {" + p + "},\n";
            if (!e.isEmpty()) bib += "  edition = {" + e + "},\n";
            if (!c.isEmpty()) bib += "  address = {" + c + "},\n";
            if (!pages.isEmpty()) bib += "  pages = {" + pages + "},\n";
            if (!doi.isEmpty()) bib += "  doi = {" + doi + "},\n";
            bib += "}";

            QApplication::clipboard()->setText(bib);
            showMessage(this, "BibTeX Export", "BibTeX copied to clipboard!", false);
        });

        citationDialog->exec();
    });

    connect(searchButton, &QToolButton::clicked, [=]()
    {
        QString bookTitle = bookSearchWindow->bookTitle->text();
        QString author1 = bookSearchWindow->author1->text();
        QString ISBN = bookSearchWindow->ISBN->text();
        GeneralOperations generalOperations(libraryDb);

        QList<LibrarySystem::Book> results = generalOperations.searchBook(
            bookTitle, author1,
            "", "", "", ISBN,
            "", "", "", "", "", "", ""
        );

        for (auto &book : results)
        {
            QString borrowedBy;
            if (!book.ISBN.isEmpty())
                book.isBorrowed = libraryDb->getBookBorrowInfo(book.ISBN, borrowedBy);
            else
                book.isBorrowed = libraryDb->getBookBorrowInfo_TITLE_AUTHOR(borrowedBy, book.title, book.author1);
            book.borrowedBy = borrowedBy;
        }

        bookSearchWindow->graphical->displayBooksWithFilters(this, results);
    });

    connect(borrowBook_Button, &QPushButton::clicked, [=]()
            {
        QDialog *borrowDialog = new QDialog(this);
        borrowDialog->setWindowTitle("Borrow Book");
        borrowDialog->resize(300, 250);

        QVBoxLayout *layout = new QVBoxLayout(borrowDialog);

        QLabel *titleLabel = new QLabel("Enter Book Title:", borrowDialog);
        QLineEdit *titleEdit = new QLineEdit(borrowDialog);

        QLabel *authorLabel = new QLabel("Enter Book Author:", borrowDialog);
        QLineEdit *authorEdit = new QLineEdit(borrowDialog);

        layout->addWidget(titleLabel);
        layout->addWidget(titleEdit);

        layout->addWidget(authorLabel);
        layout->addWidget(authorEdit);

        QLabel *dueLabel = new QLabel("Select Due Date:", borrowDialog);
        QCalendarWidget *calendar = new QCalendarWidget(borrowDialog);

        calendar->setMinimumDate(QDate::currentDate());
        calendar->setMaximumDate(QDate::currentDate().addDays(15));

        layout->addWidget(dueLabel);
        layout->addWidget(calendar);

        QHBoxLayout *btnLayout = new QHBoxLayout();
        QPushButton *okBtn = new QPushButton("OK", borrowDialog);
        QPushButton *cancelBtn = new QPushButton("Cancel", borrowDialog);
        btnLayout->addWidget(okBtn);
        btnLayout->addWidget(cancelBtn);
        layout->addLayout(btnLayout);

        // uidEdit->setText(stdStringToQString(LibrarySystem::rfid_data));

        QString schoolNo = this->currentStudentSchoolNo;

        QObject::connect(okBtn, &QPushButton::clicked, [=]() {

            QString bookTitle = titleEdit->text().trimmed();
            
            if (bookTitle.isEmpty()) {
                showMessage(this, "Error", "Please enter a book title!", true);
                return;
            }

            QString author = authorEdit->text().trimmed();
            
            if (author.isEmpty()) {
                showMessage(this, "Error", "Please enter a book author!", true);
                return;
            }
            
            QString borrowedBy;
            libraryDb->openDB();
            
            if (libraryDb->getBookBorrowInfo_TITLE_AUTHOR(borrowedBy, bookTitle, author) && !borrowedBy.isEmpty()) {
                showMessage(this, "Error", "This book is already borrowed!", true);
                libraryDb->closeDB();
                return;
            }
            
            QDate borrowDate = QDate::currentDate();
            QDate dueDate = calendar->selectedDate();

            
            if (libraryDb->isBookExists_TITLE_AUTHOR(bookTitle, author) == false) {
                showMessage(this, "Error", "No book found with the provided title and author!", true);
                libraryDb->closeDB();
                return;
            }
            

            if (libraryDb->getBorrowedBookCount(schoolNo) > 3) {
                showMessage(this, "Error", "You have reached the maximum borrow limit!", true);
                libraryDb->closeDB();
                return;
            }

            // request permission from admin here
            if (!libraryDb->borrowRequest_TITLE_AUTHOR(schoolNo,
                            borrowDate.toString("yyyy-MM-dd"),
                            dueDate.toString("yyyy-MM-dd"), bookTitle, author)) {
                showMessage(this, "Error", "Failed to borrow request book!", true);
            } else {
                showMessage(this, "Success", "Requested book borrow successfully!", false);
                std::string logString = "BORROW REQUEST: " + bookTitle.toStdString() + " by school number: " + schoolNo.toStdString();
                writeEncryptedLog(logString);
                // refreshBorrowedBooks();
            }
            

            libraryDb->closeDB();
            borrowDialog->accept();
        });

        QObject::connect(cancelBtn, &QPushButton::clicked, borrowDialog, &QDialog::reject);

        borrowDialog->exec(); 
    });

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
            QLabel *titleLabel = new QLabel("Enter Book Title:", returnDialog);
            QLineEdit *titleEdit = new QLineEdit(returnDialog);

            QLabel *authorLabel = new QLabel("Enter Book Author:", returnDialog);
            QLineEdit *authorEdit = new QLineEdit(returnDialog);

            layout->addWidget(titleLabel);
            layout->addWidget(titleEdit);

            layout->addWidget(authorLabel);
            layout->addWidget(authorEdit);

            QHBoxLayout *btnLayout = new QHBoxLayout();
            QPushButton *okBtn = new QPushButton("OK", returnDialog);
            QPushButton *cancelBtn = new QPushButton("Cancel", returnDialog);
            
            // uidEdit->setText(stdStringToQString(LibrarySystem::rfid_data));

            btnLayout->addWidget(okBtn);
            btnLayout->addWidget(cancelBtn);
            layout->addLayout(btnLayout);

            QString schoolNo = this->currentStudentSchoolNo;

            
            connect(okBtn, &QPushButton::clicked, [=]() {
                QString bookTitle = titleEdit->text().trimmed();
                QString author = authorEdit->text().trimmed();

                if (bookTitle.isEmpty()) {
                    showMessage(this, "Error", "Please enter a book title!", true);
                    return;
                }

                if (!libraryDb->openDB()) {
                    showMessage(this, "Error", "Could not open database!", true);
                    return;
                }

                if (!libraryDb->isBookBorrowedByStudent_TITLE_AUTHOR(schoolNo, bookTitle, author)) {
                    showMessage(this, "Error", "This book is not borrowed by you!", true);
                    libraryDb->closeDB();
                    return;
                }

                if (!libraryDb->returnRequest_TITLE_AUTHOR(schoolNo, bookTitle, author)) {
                    showMessage(this, "Error", "Failed to return request book!", true);
                } else {
                    showMessage(this, "Success", "Requested book return successfully!", false);
                    std::string logString = "RETURN REQUEST:  " + bookTitle.toStdString() + " by school number: " + schoolNo.toStdString();
                    writeEncryptedLog(logString);
                    // refreshBorrowedBooks();
                }
                
                libraryDb->closeDB();
                returnDialog->accept();
            });

            connect(cancelBtn, &QPushButton::clicked, returnDialog, &QDialog::reject);

            returnDialog->exec(); });
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
    refreshBorrowedBooks(currentStudentSchoolNo);
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

void StudentInterface::refreshBorrowedBooks(const QString &studentNo)
{
    borrowedBooksList->clear();

    if (!libraryDb->openDB())
    {
        return;
    }

    QList<QMap<QString, QString>> books = libraryDb->getBorrowedBooksByStudent(studentNo);
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

QString StudentInterface::getCurrentStudentSchoolNo() const {
    return currentStudentSchoolNo;
}