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

// database.h is for database operations.
// ALL BOOL FUNCTIONS RETURN TRUE IF THE OPERATION IS PERFORMED SUCCESSFULLY

#ifndef DATABASE_H_
#define DATABASE_H_

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include <QDebug>
#include <QString>
#include <QWidget>

class Database
{
public:

    /**
     * @brief Construct a new Database object.
     * 
     * @param dbName 
     * @param connectionName 
     */
    Database(const QString &dbName, const QString &connectionName);

    /**
     * @brief Destroy the Database object.
     * 
     */
    ~Database();

    /**
     * @brief Returns database.
     * 
     * @return QSqlDatabase
     */
    QSqlDatabase getDB() const { return m_db; }

    /**
     * @brief Opens database.
     * 
     * @return true 
     * @return false 
     */
    bool openDB();

    /**
     * @brief Closes database.
     * 
     */
    void closeDB();

    /**
     * @brief Creates a Users Table in the database.
     * 
     * @return true 
     * @return false 
     */
    bool createUsersTable();

    /**
     * @brief Adds user to database.
     * 
     * @param username 
     * @param schoolNo 
     * @param password 
     * @param accountType Admin/Student
     * @param uid is empty by default.
     * @return true 
     * @return false 
     */
    bool addUser(const QString &username, const QString &schoolNo,
                 const QString &password, const QString &accountType, const QString &uid = "");

    
    /**
     * @brief Updates an user's information.
     * 
     * @param parent 
     * @param username 
     * @param schoolNo 
     * @param password 
     * @param accountType 
     * @param uid 
     * @return true 
     * @return false 
     */
    bool updateUserInfo(QWidget *parent, const QString &username, const QString &schoolNo,
                        const QString &password, const QString &accountType, const QString &uid = "");

    /**
     * @brief Updates an user's password. Kept for future use.
     * 
     * @param username 
     * @param newPassword 
     * @return true 
     * @return false 
     */
    bool updateUserPassword(const QString &username, const QString &newPassword);

    /**
     * @brief Deletes user from database.
     * 
     * @param username 
     * @return true 
     * @return false 
     */
    bool deleteUser(const QString &username);

    /**
     * @brief Get the Borrowed Books from student using school number.
     * 
     * @param schoolNo 
     * @return QList<QMap<QString, QString>> 
     */
    QList<QMap<QString, QString>> getBorrowedBooksByStudent(const QString &schoolNo);

    /**
     * @brief Creates a Books Table in the database.
     * 
     * @return true 
     * @return false 
     */
    bool createBooksTable();

    /**
     * @brief Creates a Borrowed Books Table in the database.
     * 
     * @return true 
     * @return false 
     */
    bool createBorrowedBooksTable();

    /**
     * @brief Checks if the book provided with ISBN is borrowed by a student.
     * 
     * @param schoolNo 
     * @param bookISBN 
     * @return true 
     * @return false 
     */
    bool isBookBorrowedByStudent(const QString &schoolNo, const QString &bookISBN);

    /**
     * @brief Borrows a book from database.
     * 
     * @param schoolNo 
     * @param bookISBN 
     * @param borrowDate 
     * @param dueDate 
     * @param uid 
     * @return true 
     * @return false 
     */
    bool borrowBook(const QString &schoolNo, const QString &bookISBN, const QString &borrowDate, const QString &dueDate, const QString &uid = "");

    /**
     * @brief Returns a book to database.
     * 
     * @param schoolNo 
     * @param bookISBN 
     * @param uid 
     * @return true 
     * @return false 
     */
    bool returnBook(const QString &schoolNo, const QString &bookISBN, const QString &uid = "");

    /**
     * @brief Gets the Username With UID information.
     * 
     * @param uid 
     * @return QString 
     */
    QString getUsernameWithUID(const QString &uid);

    /**
     * @brief Gets the School Number With UID information.
     * 
     * @param uid 
     * @return QString 
     */
    QString getSchoolNoWithUID(const QString &uid);

    /**
     * @brief Gets the Password With UID information.
     * 
     * @param uid 
     * @return QString 
     */
    QString getPasswordWithUID(const QString &uid);

    /**
     * @brief Gets the Account Type With UID information.
     * 
     * @param uid 
     * @return QString 
     */
    QString getAccountTypeWithUID(const QString &uid);

    /**
     * @brief Gets the UID With School Number information.
     * 
     * @param schoolNo 
     * @return QString 
     */
    QString getUIDWithSchoolNo(const QString &schoolNo);

    /**
     * @brief Adds UID information to an user. Kept for future use.
     * 
     * @param schoolNo 
     * @param uid 
     * @return true 
     * @return false 
     */
    bool addUIDtoUser(const QString &schoolNo, const QString &uid);

    /**
     * @brief Checks if the book is borrowed by an user.
     * 
     * @param bookISBN 
     * @param borrowedBy 
     * @return true 
     * @return false 
     */
    bool getBookBorrowInfo(const QString &bookISBN, QString &borrowedBy);

    /**
     * @brief Checks if the book exists in the database.
     * 
     * @param ISBN 
     * @return true 
     * @return false 
     */
    bool isBookExists(const QString &ISBN);

    /**
     * @brief Checks if the book exists in the database.
     * 
     * @param uid 
     * @return true 
     * @return false 
     */
    bool isBookExistsUID(const QString &uid);

    /**
     * @brief Checks if the user exists in the database.
     * 
     * @param username 
     * @return true 
     * @return false 
     */
    bool isUserExists(const QString &username);

    /**
     * @brief Checks if the user exists in the database by UID.
     * 
     * @param uid 
     * @return true 
     * @return false 
     */
    bool isUserExistsUID(const QString &uid);

    /**
     * @brief Gets the Borrowed Book Count from an user
     * 
     * @param schoolNo 
     * @return int 
     */
    int getBorrowedBookCount(const QString &schoolNo);

    /**
     * @brief Selects records from the users table.
     *
     * Executes a SELECT query on the users table. If a condition string is
     * provided, it is appended as a SQL WHERE clause. If no condition is 
     * specified, all user records are returned.
     *
     * @param condition Optional SQL condition (e.g. "age > 18" or "id = 5").
     *                  If empty, all records are selected.
     *
     * @return QSqlQuery Executed query object containing the result set.
     *                   The caller can iterate using query.next().
     */
    QSqlQuery selectUsers(const QString &condition = "");

    /**
     * @brief Checks if the user is matched in the database
     * 
     * @param username 
     * @param schoolNo 
     * @param password 
     * @param accountType 
     * @param uid 
     * @return true 
     * @return false 
     */
    bool isUserMatchedInDataBase(const QString &username,
                                 const QString &schoolNo,
                                 const QString &password,
                                 const QString &accountType, const QString &uid = "") const;

    /**
     * @brief Adds an user to database if user is not in the database already.
     * 
     * @param username 
     * @param schoolNo 
     * @param password 
     * @param accountType 
     * @param uid 
     * @return true 
     * @return false 
     */
    bool addUserIfNotExists(const QString &username,
                            const QString &schoolNo,
                            const QString &password,
                            const QString &accountType, const QString &uid = "");

    /**
     * @brief Gets the All User UIDs from users table.
     * 
     * @return QStringList 
     */
    QStringList getAllUserUIDs();
    
    /**
     * @brief Prints all users in the database.
     * 
     */
    void debugPrintAllUsers() const;

    /**
     * @brief Deletes a book from database.
     * 
     * @param parent 
     * @param bookTitle 
     * @param author
     * @param ISBN 
     * @param uid 
     * @return true 
     * @return false 
     */
    bool deleteBook(QWidget *parent, const QString &bookTitle, const QString &author, const QString &ISBN, const QString &uid = "");

    /**
     * @brief Adds book to database.
     * 
     * @param parent 
     * @param bookTitle 
     * @param author
     * @param publisher 
     * @param publicationYear 
     * @param edition 
     * @param ISBN 
     * @param volume 
     * @param pageCount 
     * @param seriesInformation 
     * @param language 
     * @param DDC 
     * @param additionalInfo 
     * @param uid 
     * @return true 
     * @return false 
     */
    bool addBook(QWidget* parent, const QString &bookTitle, const QString &author,
                 const QString &publisher, const QString &publicationYear,
                 const QString &edition, const QString &ISBN,
                 const QString &volume, const QString &pageCount,
                 const QString &seriesInformation, const QString &language,
                 const QString &DDC, const QString &additionalInfo, const QString &uid = "");

    /**
     * @brief Updates a book's information in the database.
     * 
     * @param parent 
     * @param bookTitle 
     * @param author
     * @param publisher 
     * @param publicationYear 
     * @param edition 
     * @param ISBN 
     * @param volume 
     * @param pageCount 
     * @param seriesInformation 
     * @param language 
     * @param DDC 
     * @param additionalInfo 
     * @param uid 
     * @return true 
     * @return false 
     */
    bool updateBook(QWidget* parent, const QString &bookTitle, const QString &author,
                          const QString &publisher, const QString &publicationYear,
                          const QString &edition, const QString &ISBN,
                          const QString &volume, const QString &pageCount,
                          const QString &seriesInformation, const QString &language,
                          const QString &DDC, const QString &additionalInfo, const QString &uid = "");

    /**
    * @brief Creates a Borrow Requests Table object
    * 
    * @return true 
    * @return false 
    */
    bool createBorrowRequestsTable();

    /**
     * @brief Creates a Return Requests Table object
     * 
     * @return true 
     * @return false 
     */
    bool createReturnRequestsTable();

    /**
     * @brief Checks all overdue books for a student. Adds them to overdue_books table.
     * 
     * @param schoolNo 
     * @return true 
     * @return false 
     */
    bool checkAndAddOverdueBooksForStudent(const QString &schoolNo);

    /**
     * @brief Perform borrow request data from database
     * 
     * @return true 
     * @return false 
     */
    bool borrowRequest(const QString &schoolNo, const QString &bookISBN, const QString &borrowDate, const QString &dueDate);

    /**
     * @brief Get return request data from database
     * 
     * @return true 
     * @return false 
     */
    bool returnRequest(const QString &schoolNo, const QString &bookISBN);

    /**
     * @brief Creates a Overdue Books Table object
     * 
     * @return true 
     * @return false 
     */
    bool createOverdueBooksTable();

    /**
     * @brief Adds book to overdue books table
     * 
     * @param schoolNo 
     * @param bookISBN 
     * @param borrowDate 
     * @param title 
     * @param author 
     * @return true 
     * @return false 
     */
    bool addOverdueBook(const QString &schoolNo,
                              const QString &bookISBN,
                              const QString &borrowDate,
                              const QString &title,
                              const QString &author);

    /**
     * @brief Deletes book from overdue_books table
     * 
     * @param schoolNo 
     * @param title 
     * @param author 
     * @return true 
     * @return false 
     */

    bool deleteOverdueBook(const QString &schoolNo,
                                 const QString &title,
                                 const QString &author);
    /**
     * @brief Get the All Overdue Books from overdue_books table
     * 
     * @return QStringList 
     */
    QStringList getAllOverdueBooks();

    /**
     * @brief Create a User Emails Table object
     * 
     * @return true 
     * @return false 
     */
    bool createUserEmailsTable();
    
    /**
     * @brief Get the School Number By Email 
     * 
     * @param email 
     * @return QString 
     */
    QString getSchoolNoByEmail(const QString &email)  ;

    /**
     * @brief Get the Email By School Number
     * 
     * @param schoolNo 
     * @return QString 
     */
    QString getEmailBySchoolNo(const QString &schoolNo);

    /**
     * @brief Add an user an email related to the user's school number.
     * 
     * @param schoolNo 
     * @param email 
     * @return true 
     * @return false 
     */
    bool addUserEmail(const QString &schoolNo, const QString &email);

    /**
     * @brief Delete an user email from database using user's school number.
     * 
     * @param schoolNo 
     * @param email 
     * @return true 
     * @return false 
     */
    bool deleteUserEmail(const QString &schoolNo, const QString &email);

    /**
     * @brief Update an user's email using user's school number.
     * 
     * @param schoolNo 
     * @param email 
     * @return true 
     * @return false 
     */
    bool updateUserEmail(const QString &schoolNo, const QString &email);

    // === REVISED FUNCTIONS ===

    // Instead of using book isbn, use title and author to perform operations.
    // _TITLE_AUTHOR
    
    /**
     * @brief Checks if the book provided with title and author is borrowed by a student.
     * 
     * @param schoolNo 
     * @param bookTitle 
     * @param author
     * @return true 
     * @return false 
     */
    bool isBookBorrowedByStudent_TITLE_AUTHOR(const QString &schoolNo, const QString &bookTitle, const QString author);

    /**
     * @brief  Borrows a book from database using title and author.
     * 
     * @param schoolNo 
     * @param borrowDate 
     * @param dueDate 
     * @param bookTitle 
     * @param author
     * @return true 
     * @return false 
     */
    bool borrowBook_TITLE_AUTHOR(const QString &schoolNo, const QString &borrowDate, const QString &dueDate, const QString &bookTitle, const QString author);

    /**
     * @brief Returns a book to database using title and author
     * 
     * @param schoolNo 
     * @param bookTitle 
     * @param author
     * @return true 
     * @return false 
     */
    bool returnBook_TITLE_AUTHOR(const QString &schoolNo, const QString &bookTitle, const QString author);

    /**
     * @brief Checks if the book is borrowed by an user using title and author.
     * 
     * @param borrowedBy 
     * @param bookTitle 
     * @param author
     * @return true 
     * @return false 
     */
    bool getBookBorrowInfo_TITLE_AUTHOR(QString &borrowedBy, const QString &bookTitle, const QString author);

    /**
     * @brief Checks if the book exists in the database using title and author.
     * 
     * @param bookTitle 
     * @param author
     * @return true 
     * @return false 
     */
    bool isBookExists_TITLE_AUTHOR(const QString &bookTitle, const QString author);

    /**
     * @brief Perform borrow request data from database using title and author
     * 
     * @param schoolNo 
     * @param borrowDate 
     * @param dueDate 
     * @param bookTitle 
     * @param author 
     * @return true 
     * @return false 
     */
    bool borrowRequest_TITLE_AUTHOR(const QString &schoolNo, const QString &borrowDate, const QString &dueDate, const QString &bookTitle, const QString author);

    /**
     * @brief Perform return request data from database using title and author
     * 
     * @param schoolNo 
     * @param bookTitle 
     * @param author
     * @return true 
     * @return false 
     */
    bool returnRequest_TITLE_AUTHOR(const QString &schoolNo, const QString &bookTitle, const QString &author);

    /**
     * @brief Gets borrow requests
     * 
     * @return QList<QMap<QString, QString>> 
     */
    QList<QMap<QString, QString>> getBorrowRequests();

    /**
     * @brief Gets return requests
     * 
     * @return QList<QMap<QString, QString>> 
     */
    QList<QMap<QString, QString>> getReturnRequests();

    /**
     * @brief Deletes book borrow request using title and author
     * 
     * @param schoolNo 
     * @param title 
     * @param author 
     * @return true 
     * @return false 
     */
    bool deleteBorrowRequest_TITLE_AUTHOR(const QString &schoolNo,
                                                const QString &title,
                                                const QString &author);

    /**
     * @brief Deletes return request using title and author
     * 
     * @param schoolNo 
     * @param title 
     * @param author 
     * @return true 
     * @return false 
     */
    bool deleteReturnRequest_TITLE_AUTHOR(const QString &schoolNo,
                                                const QString &title,
                                                const QString &author);

private:
    QSqlDatabase m_db;
    QString m_dbName;
};

#endif // DATABASE_H_