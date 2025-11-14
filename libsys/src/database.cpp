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

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QDebug>

#include "headers/Database.h"
#include "headers/Utils.h"

Database::Database(const QString &dbName, const QString &connectionName)
    : m_dbName(dbName)
{
    if (QSqlDatabase::contains(connectionName))
    {
        m_db = QSqlDatabase::database(connectionName);
    }
    else
    {
        m_db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        m_db.setDatabaseName(m_dbName);
    }
}

Database::~Database()
{

    if (m_db.isOpen())
    {
        m_db.close();
    }
}

bool Database::openDB()
{
    if (!m_db.open())
    {
        //  qDebug() << "Could not open the database:" << m_db.lastError().text();
        return false;
    }
    //  qDebug() << "Opened the database:" << m_dbName;
    return true;
}

void Database::closeDB()
{
    if (m_db.isOpen())
        m_db.close();
}

bool Database::createUsersTable()
{
    QSqlQuery query(m_db);
    QString createTable = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            school_no TEXT NOT NULL,
            password TEXT NOT NULL,
            account_type TEXT NOT NULL
        )
    )";

    if (!query.exec(createTable))
    {
        //  qDebug() << "Could not create the table:" << query.lastError().text();
        return false;
    }
    //  qDebug() << "users table is ready.";
    return true;
}

bool Database::addUser(const QString &username, const QString &schoolNo,
                       const QString &password, const QString &accountType)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO users (username, school_no, password, account_type)
        VALUES (:username, :school_no, :password, :account_type)
    )");

    if ((username == "") || (schoolNo == "") || (password == "") || (accountType == ""))
    {
        return false;
    }

    // QString passwordAES = convertToAes(password);

    query.bindValue(":username", username);
    query.bindValue(":school_no", schoolNo);
    query.bindValue(":password", password);
    query.bindValue(":account_type", accountType);

    if (!query.exec())
    {
        //  qDebug() << "Could not add the user:" << query.lastError().text();
        return false;
    }
    //  qDebug() << "Added user:" << username;
    return true;
}

bool Database::updateUserPassword(const QString &username, const QString &newPassword)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE users SET password = :password WHERE username = :username");

    QString passwordAES = convertToAes(newPassword);

    query.bindValue(":password", passwordAES);
    query.bindValue(":username", username);

    if (!query.exec())
    {
        //  qDebug() << "Could not update the password:" << query.lastError().text();
        return false;
    }
    //  qDebug() << "Updated password:" << username;
    return true;
}

bool Database::updateUserInfo(QWidget *parent, const QString &username, const QString &schoolNo,
                              const QString &password, const QString &accountType)
{
    if (username.isEmpty())
    {
        showMessage(parent, "Error", "Username cannot be empty!", true);
        return false;
    }

    QStringList updates;
    QMap<QString, QVariant> bindings;

    if (!schoolNo.isEmpty())
    {
        updates << "school_no = :school_no";
        bindings[":school_no"] = schoolNo;
    }
    QString passwordAES = convertToAes(password);

    if (!password.isEmpty())
    {
        updates << "password = :password";
        bindings[":password"] = passwordAES;
    }

    if (!accountType.isEmpty())
    {
        updates << "account_type = :account_type";
        bindings[":account_type"] = accountType;
    }

    if (updates.isEmpty())
    {
        showMessage(parent, "Warning", "No fields to update.", true);
        return false;
    }

    QString sql = QString("UPDATE users SET %1 WHERE username = :username")
                      .arg(updates.join(", "));

    QSqlQuery query(m_db);
    query.prepare(sql);

    for (auto it = bindings.constBegin(); it != bindings.constEnd(); ++it)
        query.bindValue(it.key(), it.value());

    query.bindValue(":username", username);

    if (!query.exec())
    {
        //  qDebug() << "Could not update user info:" << query.lastError().text();
        showMessage(parent, "Error", "Database update failed!", true);
        return false;
    }

    //  qDebug() << "Updated user info for:" << username;
    showMessage(parent, "Success", "User information updated successfully!", false);
    return true;
}

bool Database::deleteUser(const QString &username)
{
    QSqlQuery query(m_db);

    if (username == "Admin") return false;

    if (username == "") return false;

    if (!(isUserExists(username))) return false;

    query.prepare("DELETE FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec())
    {
        //  qDebug() << "Could not delete the user:" << query.lastError().text();
        return false;
    }
    //  qDebug() << "Deleted the user:" << username;
    return true;
}

QSqlQuery Database::selectUsers(const QString &condition)
{
    QSqlQuery query(m_db);
    QString sql = "SELECT * FROM users";
    if (!condition.isEmpty())
    {
        sql += " WHERE " + condition;
    }

    if (!query.exec(sql))
    {
        //  qDebug() << "SELECT error:" << query.lastError().text();
    }

    return query;
}

bool Database::isUserMatchedInDataBase(const QString &username,
                                       const QString &schoolNo,
                                       const QString &password,
                                       const QString &accountType) const
{
    //  qDebug() << "DEBUG — Login check:";
    //  qDebug() << "username:" << username;
    //  qDebug() << "schoolNo:" << schoolNo;
    //  qDebug() << "password:" << password;
    //  qDebug() << "accountType:" << accountType;

    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT COUNT(*) 
        FROM users 
        WHERE username = :username 
          AND school_no = :school_no 
          AND password = :password 
          AND account_type = :account_type
    )");

    // QString passwordAES = convertToAes(password);

    query.bindValue(":username", username);
    query.bindValue(":school_no", schoolNo);
    query.bindValue(":password", password);
    query.bindValue(":account_type", accountType);

    if (!query.exec())
    {
        //  qDebug() << "Login check error:" << query.lastError().text();
        return false;
    }

    if (query.next())
    {
        int count = query.value(0).toInt();
        //  qDebug() << "Matching user count:" << count;
        return count > 0;
    }
    else
    {
        //  qDebug() << "Could not get data from query.";
    }

    return false;
}
bool Database::addUserIfNotExists(const QString &username,
                                  const QString &schoolNo,
                                  const QString &password,
                                  const QString &accountType)
{
    QSqlQuery checkQuery(m_db);
    checkQuery.prepare("SELECT COUNT(*) FROM users WHERE username = :username");
    checkQuery.bindValue(":username", username);

    if (!checkQuery.exec())
    {
        //  qDebug() << "User check error:" << checkQuery.lastError().text();
        return false;
    }

    if (checkQuery.next() && checkQuery.value(0).toInt() > 0)
    {
        //  qDebug() << "User already exists:" << username;
        // showMessage(nullptr, "Error", "User already exists.", true);
        return true;
    }

    // User not found -> safely add
    QString passwordAES = convertToAes(password);

    return addUser(username, schoolNo, passwordAES, accountType);
}

void Database::debugPrintAllUsers() const
{
    QSqlQuery query(m_db);
    if (!query.exec("SELECT username, school_no, password, account_type FROM users"))
    {
        //  qDebug() << "Error in listing users" << query.lastError().text();
        return;
    }

    qDebug() << "---- USERS TABLE CONTENTS ----";
    while (query.next())
    {
        qDebug() << "username:" << query.value(0).toString()
                 << "| school_no:" << query.value(1).toString()
                 << "| password:" << query.value(2).toString()
                 << "| account_type:" << query.value(3).toString();
    }
    qDebug() << "------------------------------";
}

bool Database::addBook(QWidget *parent,
                       const QString &bookTitle, const QString &author1,
                       const QString &publisher, const QString &publicationYear,
                       const QString &edition, const QString &ISBN,
                       const QString &volume, const QString &pageCount,
                       const QString &seriesInformation, const QString &language,
                       const QString &DDC, const QString &additionalInfo)
{
    if (bookTitle.trimmed().isEmpty() || ISBN.trimmed().isEmpty())
    {
        showMessage(parent, "Add Book Error", "Book Title and ISBN cannot be empty!", true);
        return false;
    }

    if (!m_db.isOpen() && !m_db.open())
    {
        //  qDebug() << "Could not open database when adding book.";
        showMessage(parent, "Database Error", "Could not open database connection.", true);
        return false;
    }

    QSqlQuery checkQuery(m_db);
    checkQuery.prepare("SELECT COUNT(*) FROM books WHERE isbn = :isbn AND title = :title");
    checkQuery.bindValue(":isbn", ISBN);
    checkQuery.bindValue(":title", bookTitle);
    if (!checkQuery.exec())
    {
        //  qDebug() << "Check query failed:" << checkQuery.lastError().text();
        showMessage(parent, "Database Error", "Failed to check if book exists: " + checkQuery.lastError().text(), true);
        return false;
    }

    if (checkQuery.next() && checkQuery.value(0).toInt() > 0)
    {
        showMessage(parent, "Duplicate Book", QString("Book with Title \"%1\" and ISBN \"%2\" already exists.").arg(bookTitle, ISBN), true);
        return false;
    }

    QSqlQuery query(m_db);
    QString sql = R"(
        INSERT INTO books (
            title, author1,
            publisher, publication_year, edition, isbn, volume,
            page_count, series_information, language, ddc, additional_info,
            is_borrowed, borrowed_by
        )
        VALUES (
            :title, :author1,
            :publisher, :publication_year, :edition, :isbn, :volume,
            :page_count, :series_information, :language, :ddc, :additional_info,
            0, ''
        )
    )";

    if (!query.prepare(sql))
    {
        //  qDebug() << "Failed to prepare SQL query:" << query.lastError().text();
        showMessage(parent, "Database Error", "Failed to prepare SQL query: " + query.lastError().text(), true);
        return false;
    }

    query.bindValue(":title", bookTitle);
    query.bindValue(":author1", author1);
    query.bindValue(":publisher", publisher);
    query.bindValue(":publication_year", publicationYear);
    query.bindValue(":edition", edition);
    query.bindValue(":isbn", ISBN);
    query.bindValue(":volume", volume);
    query.bindValue(":page_count", pageCount);
    query.bindValue(":series_information", seriesInformation);
    query.bindValue(":language", language);
    query.bindValue(":ddc", DDC);
    query.bindValue(":additional_info", additionalInfo);

    if (!query.exec())
    {
        //  qDebug() << "Could not add book:" << query.lastError().text();
        showMessage(parent, "Add Book Error", "Could not add book: " + query.lastError().text(), true);
        return false;
    }

    showMessage(parent, "Book Added", QString("Successfully added book:\nTitle: \"%1\"\nAuthor: \"%2\"\nISBN: \"%3\"").arg(bookTitle, author1, ISBN), false);
    return true;
}

bool Database::deleteBook(QWidget *parent, const QString &bookTitle, const QString &author1, const QString &ISBN)
{
    if (!m_db.isOpen() && !m_db.open())
    {
        //  qDebug() << "Could not open database when deleting book.";
        showMessage(parent, "Database Error", "Could not open database.", true);
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        DELETE FROM books 
        WHERE title = :title AND author1 = :author1 AND isbn = :isbn
    )");

    query.bindValue(":title", bookTitle);
    query.bindValue(":author1", author1);
    query.bindValue(":isbn", ISBN);

    if (!query.exec())
    {
        //  qDebug() << "Could not delete book:" << query.lastError().text();
        showMessage(parent, "Deletion Error", "Could not delete book: " + query.lastError().text(), true);
        return false;
    }

    if (query.numRowsAffected() == 0)
    {
        //  qDebug() << "No book found with the given Title, Author, and ISBN.";
        showMessage(parent, "Book Not Found",
                    QString("No book found with Title: \"%1\", Author: \"%2\", ISBN: \"%3\"")
                        .arg(bookTitle, author1, ISBN),
                    true);
        return false;
    }

    //  qDebug() << "Deleted book:" << bookTitle << "by" << author1 << "(ISBN:" << ISBN << ")";
    showMessage(parent, "Book Deleted",
                QString("Deleted book:\nTitle: \"%1\"\nAuthor: \"%2\"\nISBN: \"%3\"")
                    .arg(bookTitle, author1, ISBN),
                false);

    return true;
}

bool Database::updateBook(QWidget *parent, const QString &bookTitle, const QString &author1,
                          const QString &publisher, const QString &publicationYear,
                          const QString &edition, const QString &ISBN,
                          const QString &volume, const QString &pageCount,
                          const QString &seriesInformation, const QString &language,
                          const QString &DDC, const QString &additionalInfo)
{
    if (ISBN.trimmed().isEmpty() || bookTitle.trimmed().isEmpty())
    {
        //  qDebug() << "Cannot update book: ISBN or Title is empty!";
        showMessage(parent, "Update Error", "Cannot update book: ISBN or Title cannot be empty!", true);
        return false;
    }

    if (!m_db.isOpen() && !m_db.open())
    {
        //  qDebug() << "Could not open database when updating book info.";
        showMessage(parent, "Database Error", "Could not open database connection.", true);
        return false;
    }

    QSqlQuery checkQuery(m_db);
    checkQuery.prepare("SELECT COUNT(*) FROM books WHERE isbn = :isbn AND title = :title");
    checkQuery.bindValue(":isbn", ISBN);
    checkQuery.bindValue(":title", bookTitle);
    if (!checkQuery.exec() || !checkQuery.next() || checkQuery.value(0).toInt() == 0)
    {
        //  qDebug() << "No book found with Title:" << bookTitle << "and ISBN:" << ISBN;
        showMessage(parent, "Book Not Found",
                    QString("No book found with Title: \"%1\" and ISBN: \"%2\"")
                        .arg(bookTitle, ISBN),
                    true);
        return false;
    }

    QStringList updates;
    QMap<QString, QVariant> binds;

    if (!author1.trimmed().isEmpty())
    {
        updates << "author1 = :author1";
        binds[":author1"] = author1;
    }
    if (!publisher.trimmed().isEmpty())
    {
        updates << "publisher = :publisher";
        binds[":publisher"] = publisher;
    }
    if (!publicationYear.trimmed().isEmpty())
    {
        updates << "publication_year = :publication_year";
        binds[":publication_year"] = publicationYear;
    }
    if (!edition.trimmed().isEmpty())
    {
        updates << "edition = :edition";
        binds[":edition"] = edition;
    }
    if (!volume.trimmed().isEmpty())
    {
        updates << "volume = :volume";
        binds[":volume"] = volume;
    }
    if (!pageCount.trimmed().isEmpty())
    {
        updates << "page_count = :page_count";
        binds[":page_count"] = pageCount;
    }
    if (!seriesInformation.trimmed().isEmpty())
    {
        updates << "series_information = :series_information";
        binds[":series_information"] = seriesInformation;
    }
    if (!language.trimmed().isEmpty())
    {
        updates << "language = :language";
        binds[":language"] = language;
    }
    if (!DDC.trimmed().isEmpty())
    {
        updates << "ddc = :ddc";
        binds[":ddc"] = DDC;
    }
    if (!additionalInfo.trimmed().isEmpty())
    {
        updates << "additional_info = :additional_info";
        binds[":additional_info"] = additionalInfo;
    }

    if (updates.isEmpty())
    {
        //  qDebug() << "No fields provided to update for book:" << bookTitle;
        showMessage(parent, "No Changes", "No fields were provided to update.", true);
        return false;
    }

    QString sql = QString("UPDATE books SET %1 WHERE isbn = :isbn AND title = :title")
                      .arg(updates.join(", "));

    QSqlQuery query(m_db);
    query.prepare(sql);

    for (auto it = binds.begin(); it != binds.end(); ++it)
        query.bindValue(it.key(), it.value());

    query.bindValue(":isbn", ISBN);
    query.bindValue(":title", bookTitle);

    if (!query.exec())
    {
        //  qDebug() << "Could not update book:" << query.lastError().text();
        showMessage(parent, "Update Error", "Could not update book: " + query.lastError().text(), true);
        return false;
    }

    if (query.numRowsAffected() == 0)
    {
        //  qDebug() << "No rows updated. Possibly no changes were made.";
        showMessage(parent, "No Changes", "No rows were updated. Possibly the same values were submitted.", true);
        return false;
    }

    //  qDebug() << "Successfully updated book:" << bookTitle << "(ISBN:" << ISBN << ")";
    showMessage(parent, "Book Updated",
                QString("Book updated successfully:\nTitle: \"%1\"\nISBN: \"%2\"")
                    .arg(bookTitle, ISBN),
                false);

    return true;
}

bool Database::createBooksTable()
{
    QSqlQuery query(m_db);
    QString createTable = R"(
        CREATE TABLE IF NOT EXISTS books (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            author1 TEXT,
            publisher TEXT,
            publication_year TEXT,
            edition TEXT,
            isbn TEXT,
            volume TEXT,
            page_count TEXT,
            series_information TEXT,
            language TEXT,
            ddc TEXT,
            additional_info TEXT,
            is_borrowed INTEGER DEFAULT 0,
            borrowed_by TEXT DEFAULT ''
        )
    )";

    if (!query.exec(createTable))
    {
        //  qDebug() << "Could not create books table:" << query.lastError().text();
        return false;
    }
    //  qDebug() << "books table is ready.";
    return true;
}

bool Database::createBorrowedBooksTable()
{
    QSqlQuery query(m_db);
    QString createTable = R"(
        CREATE TABLE IF NOT EXISTS borrowed_books (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            school_no TEXT NOT NULL,
            book_isbn TEXT NOT NULL,
            borrow_date TEXT,
            due_date TEXT,
            UNIQUE(school_no, book_isbn)
        )
    )";

    if (!query.exec(createTable))
    {
        //  qDebug() << "Could not create borrowed_books table:" << query.lastError().text();
        return false;
    }
    //  qDebug() << "borrowed_books table is ready.";
    return true;
}

bool Database::borrowBook(const QString &schoolNo, const QString &bookISBN, const QString &borrowDate, const QString &dueDate)
{
    if (!m_db.isOpen() && !m_db.open())
    {
        return false;
    }

    int borrowedCount = getBorrowedBookCount(schoolNo);
    if (borrowedCount > 3)
    {
        return false;
    }

    QSqlQuery statusQuery(m_db);
    statusQuery.prepare("SELECT additional_info FROM books WHERE isbn = :isbn");
    statusQuery.bindValue(":isbn", bookISBN);

    if (!statusQuery.exec() || !statusQuery.next())
    {
        return false;
    }

    QString additionalInfo = statusQuery.value(0).toString();
    if (additionalInfo.contains("LOST", Qt::CaseInsensitive))
    {
        return false;
    }

    QString existingBorrowedBy;
    if (getBookBorrowInfo(bookISBN, existingBorrowedBy))
    {
        return false;
    }

    QSqlQuery insertQuery(m_db);
    insertQuery.prepare(R"(
        INSERT INTO borrowed_books (school_no, book_isbn, borrow_date, due_date)
        VALUES (:school_no, :book_isbn, :borrow_date, :due_date)
    )");
    insertQuery.bindValue(":school_no", schoolNo);
    insertQuery.bindValue(":book_isbn", bookISBN);
    insertQuery.bindValue(":borrow_date", borrowDate);
    insertQuery.bindValue(":due_date", dueDate);

    if (!insertQuery.exec())
    {
        return false;
    }

    QSqlQuery updateQuery(m_db);
    updateQuery.prepare(R"(
        UPDATE books SET is_borrowed = 1, borrowed_by = :school_no WHERE isbn = :isbn
    )");
    updateQuery.bindValue(":school_no", schoolNo);
    updateQuery.bindValue(":isbn", bookISBN);

    if (!updateQuery.exec())
    {
        return false;
    }

    return true;
}

bool Database::returnBook(const QString &schoolNo, const QString &bookISBN)
{
    if (!m_db.isOpen() && !m_db.open())
    {
        return false;
    }

    QSqlQuery statusQuery(m_db);
    statusQuery.prepare("SELECT additional_info FROM books WHERE isbn = :isbn");
    statusQuery.bindValue(":isbn", bookISBN);

    if (!statusQuery.exec() || !statusQuery.next())
    {
        return false;
    }

    QString additionalInfo = statusQuery.value(0).toString();
    if (additionalInfo.contains("LOST", Qt::CaseInsensitive))
    {
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("DELETE FROM borrowed_books WHERE school_no = :school_no AND book_isbn = :book_isbn");
    query.bindValue(":school_no", schoolNo);
    query.bindValue(":book_isbn", bookISBN);

    if (!query.exec())
    {
        return false;
    }

    QSqlQuery updateQuery(m_db);
    updateQuery.prepare(R"(
        UPDATE books SET is_borrowed = 0, borrowed_by = '' WHERE isbn = :isbn
    )");
    updateQuery.bindValue(":isbn", bookISBN);

    if (!updateQuery.exec())
    {
        return false;
    }

    return true;
}

int Database::getBorrowedBookCount(const QString &schoolNo)
{
    if (!m_db.isOpen() && !m_db.open())
        return 0;

    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) FROM borrowed_books WHERE school_no = :school_no");
    query.bindValue(":school_no", schoolNo);

    if (!query.exec())
    {
        //  qDebug() << "Count borrowed books failed:" << query.lastError().text();
        return 0;
    }

    if (query.next())
    {
        return query.value(0).toInt();
    }

    return 0;
}

bool Database::getBookBorrowInfo(const QString &bookISBN, QString &borrowedBy)
{
    if (!m_db.isOpen() && !m_db.open())
        return false;

    QSqlQuery query(m_db);
    query.prepare("SELECT school_no FROM borrowed_books WHERE book_isbn = :isbn");
    query.bindValue(":isbn", bookISBN);

    if (!query.exec())
    {
        //  qDebug() << "Failed to check borrowed book:" << query.lastError().text();
        return false;
    }

    if (query.next())
    {
        borrowedBy = query.value(0).toString();
        return true;
    }

    borrowedBy = "";
    return false;
}

bool Database::isBookExists(const QString &ISBN)
{
    if (!m_db.isOpen() && !m_db.open())
    {
        //  qDebug() << "Database not open for checking book existence.";
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) FROM books WHERE isbn = :isbn");
    query.bindValue(":isbn", ISBN);

    if (!query.exec())
    {
        //  qDebug() << "Failed to check book existence:" << query.lastError().text();
        return false;
    }

    if (query.next())
    {
        int count = query.value(0).toInt();
        return count > 0;
    }

    return false;
}

bool Database::isBookBorrowedByStudent(const QString &schoolNo, const QString &bookISBN)
{
    if (!m_db.isOpen() && !m_db.open())
    {
        //  qDebug() << "Database not open for checking book borrow status.";
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT COUNT(*) 
        FROM borrowed_books 
        WHERE school_no = :school_no 
          AND book_isbn = :book_isbn
    )");

    query.bindValue(":school_no", schoolNo);
    query.bindValue(":book_isbn", bookISBN);

    if (!query.exec())
    {
        //  qDebug() << "Failed to check if book is borrowed by student:" << query.lastError().text();
        return false;
    }

    if (query.next())
    {
        int count = query.value(0).toInt();
        return count > 0;
    }

    return false;
}

QList<QMap<QString, QString>> Database::getBorrowedBooksByStudent(const QString &schoolNo)
{
    QList<QMap<QString, QString>> borrowedBooks;

    if (!m_db.isOpen() && !m_db.open())
    {
        //  qDebug() << "Database not open for fetching borrowed books.";
        return borrowedBooks;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT 
            b.title AS book_title, 
            b.author1 AS book_author1, 
            b.isbn AS book_isbn, 
            bb.borrow_date AS borrow_date, 
            bb.due_date AS due_date
        FROM borrowed_books AS bb
        JOIN books AS b ON bb.book_isbn = b.isbn
        WHERE bb.school_no = :school_no
    )");
    query.bindValue(":school_no", schoolNo);

    if (!query.exec())
    {
        //  qDebug() << "Failed to fetch borrowed books:" << query.lastError().text();
        return borrowedBooks;
    }

    while (query.next())
    {
        QMap<QString, QString> book;
        book["title"] = query.value("book_title").toString();
        book["author1"] = query.value("book_author1").toString();
        book["isbn"] = query.value("book_isbn").toString();
        book["borrow_date"] = query.value("borrow_date").toString();
        book["due_date"] = query.value("due_date").toString();
        borrowedBooks.append(book);
    }

    return borrowedBooks;
}

bool Database::isUserExists(const QString &username)
{
    if (!m_db.isOpen() && !m_db.open())
    {
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec())
    {
        return false;
    }

    if (query.next())
    {
        int count = query.value(0).toInt();
        return count > 0;
    }

    return false;
}