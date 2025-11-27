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

#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQuery>

#include "headers/GeneralOperations.h"
#include "headers/database.h"

QList<LibrarySystem::Book> GeneralOperations::searchBook(
    const QString &id, 
    const QString &bookTitle, const QString &author,
    const QString &publisher, const QString &publicationYear,
    const QString &edition, const QString &ISBN,
    const QString &volume, const QString &pageCount,
    const QString &seriesInformation, const QString &language,
    const QString &DDC, const QString &additionalInfo, const QString &uid)
{
    QList<LibrarySystem::Book> results;

    if (!libraryDb->openDB())
        return results;

    QSqlQuery query(libraryDb->getDB());
    QString sql = "SELECT * FROM books WHERE 1=1";

    if (!ISBN.isEmpty())
    {
        sql += " AND isbn = :ISBN";
    }
    else
    {
        if (!bookTitle.isEmpty())
            sql += " AND title LIKE '%' || :bookTitle || '%'";
        if (!author.isEmpty())
            sql += " AND author LIKE '%' || :author || '%'";
    }

    if (!id.isEmpty()) sql += " AND id LIKE '%' || :id || '%'";

    if (!publisher.isEmpty()) sql += " AND publisher LIKE '%' || :publisher || '%'";
    if (!publicationYear.isEmpty()) sql += " AND publication_year = :publicationYear";
    if (!edition.isEmpty()) sql += " AND edition LIKE '%' || :edition || '%'";
    if (!volume.isEmpty()) sql += " AND volume LIKE '%' || :volume || '%'";
    if (!pageCount.isEmpty()) sql += " AND page_count = :pageCount";
    if (!seriesInformation.isEmpty()) sql += " AND series_information LIKE '%' || :seriesInformation || '%'";
    if (!language.isEmpty()) sql += " AND language LIKE '%' || :language || '%'";
    if (!DDC.isEmpty()) sql += " AND ddc LIKE '%' || :DDC || '%'";
    if (!additionalInfo.isEmpty()) sql += " AND additional_info LIKE '%' || :additionalInfo || '%'";
    if (!uid.isEmpty()) sql += " AND uid LIKE '%' || :uid || '%'";

    query.prepare(sql);

    if (!ISBN.isEmpty())
        query.bindValue(":ISBN", ISBN);
    else
    {
        if (!bookTitle.isEmpty()) query.bindValue(":bookTitle", bookTitle);
        if (!author.isEmpty()) query.bindValue(":author", author);
    }

    if (!id.isEmpty()) query.bindValue(":id", id);

    if (!publisher.isEmpty()) query.bindValue(":publisher", publisher);
    if (!publicationYear.isEmpty()) query.bindValue(":publicationYear", publicationYear);
    if (!edition.isEmpty()) query.bindValue(":edition", edition);
    if (!volume.isEmpty()) query.bindValue(":volume", volume);
    if (!pageCount.isEmpty()) query.bindValue(":pageCount", pageCount);
    if (!seriesInformation.isEmpty()) query.bindValue(":seriesInformation", seriesInformation);
    if (!language.isEmpty()) query.bindValue(":language", language);
    if (!DDC.isEmpty()) query.bindValue(":DDC", DDC);
    if (!additionalInfo.isEmpty()) query.bindValue(":additionalInfo", additionalInfo);
    if (!uid.isEmpty()) query.bindValue(":uid", uid);

    if (!query.exec())
        return results;

    while (query.next())
    {
        LibrarySystem::Book book;
        book.id = query.value("id").toString();
        book.title = query.value("title").toString();
        book.author = query.value("author").toString();
        book.publisher = query.value("publisher").toString();
        book.publicationYear = query.value("publication_year").toString();
        book.edition = query.value("edition").toString();
        book.ISBN = query.value("isbn").toString();
        book.volume = query.value("volume").toString();
        book.pageCount = query.value("page_count").toString();
        book.seriesInformation = query.value("series_information").toString();
        book.language = query.value("language").toString();
        book.DDC = query.value("ddc").toString();
        book.additionalInfo = query.value("additional_info").toString();
        book.uid = query.value("uid").toString();

        QString borrowedBy;
        if (!book.ISBN.isEmpty())
        {
            book.isBorrowed = libraryDb->getBookBorrowInfo(book.id, book.ISBN, borrowedBy);
        }
        else
        {
            book.isBorrowed = libraryDb->getBookBorrowInfo_TITLE_AUTHOR(book.id, borrowedBy, book.title, book.author);
        }
        book.borrowedBy = borrowedBy;

        results.append(book);
    }

    return results;
}
