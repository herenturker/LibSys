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

#include "headers/GeneralOperations.h"
#include "headers/database.h"

QList<LibrarySystem::Book> GeneralOperations::searchBook( const QString &bookTitle, const QString &author1,
                                    const QString &author2, const QString &author3,
                                    const QString &author4, const QString &author5,
                                    const QString &publisher, const QString &publicationYear,
                                    const QString &edition, const QString &ISBN,
                                    const QString &volume, const QString &pageCount,
                                    const QString &seriesInformation, const QString &language,
                                    const QString &DDC, const QString &additionalInfo
                                )
{

    QList<LibrarySystem::Book> results;

    if (!libraryDb->openDB()) {
        qDebug() << "Database not open!";
        return results;
    }

    QSqlQuery query(libraryDb->getDB());

    QString sql = "SELECT * FROM books WHERE 1=1";

    if (!bookTitle.isEmpty()) sql += " AND title LIKE '%' || :bookTitle || '%'";
    if (!author1.isEmpty()) sql += " AND author1 LIKE '%' || :author1 || '%'";
    if (!author2.isEmpty()) sql += " AND author2 LIKE '%' || :author2 || '%'";
    if (!author3.isEmpty()) sql += " AND author3 LIKE '%' || :author3 || '%'";
    if (!author4.isEmpty()) sql += " AND author4 LIKE '%' || :author4 || '%'";
    if (!author5.isEmpty()) sql += " AND author5 LIKE '%' || :author5 || '%'";
    if (!publisher.isEmpty()) sql += " AND publisher LIKE '%' || :publisher || '%'";
    if (!publicationYear.isEmpty()) sql += " AND publicationYear = :publicationYear";
    if (!edition.isEmpty()) sql += " AND edition LIKE '%' || :edition || '%'";
    if (!ISBN.isEmpty()) sql += " AND ISBN = :ISBN";
    if (!volume.isEmpty()) sql += " AND volume LIKE '%' || :volume || '%'";
    if (!pageCount.isEmpty()) sql += " AND pageCount = :pageCount";
    if (!seriesInformation.isEmpty()) sql += " AND seriesInformation LIKE '%' || :seriesInformation || '%'";
    if (!language.isEmpty()) sql += " AND language LIKE '%' || :language || '%'";
    if (!DDC.isEmpty()) sql += " AND DDC LIKE '%' || :DDC || '%'";
    if (!additionalInfo.isEmpty()) sql += " AND additionalInfo LIKE '%' || :additionalInfo || '%'";

    query.prepare(sql);

    if (!bookTitle.isEmpty()) query.bindValue(":bookTitle", bookTitle);
    if (!author1.isEmpty()) query.bindValue(":author1", author1);
    if (!author2.isEmpty()) query.bindValue(":author2", author2);
    if (!author3.isEmpty()) query.bindValue(":author3", author3);
    if (!author4.isEmpty()) query.bindValue(":author4", author4);
    if (!author5.isEmpty()) query.bindValue(":author5", author5);
    if (!publisher.isEmpty()) query.bindValue(":publisher", publisher);
    if (!publicationYear.isEmpty()) query.bindValue(":publicationYear", publicationYear);
    if (!edition.isEmpty()) query.bindValue(":edition", edition);
    if (!ISBN.isEmpty()) query.bindValue(":ISBN", ISBN);
    if (!volume.isEmpty()) query.bindValue(":volume", volume);
    if (!pageCount.isEmpty()) query.bindValue(":pageCount", pageCount);
    if (!seriesInformation.isEmpty()) query.bindValue(":seriesInformation", seriesInformation);
    if (!language.isEmpty()) query.bindValue(":language", language);
    if (!DDC.isEmpty()) query.bindValue(":DDC", DDC);
    if (!additionalInfo.isEmpty()) query.bindValue(":additionalInfo", additionalInfo);

    if (!query.exec()) {
        qDebug() << "Search query failed:" << query.lastError().text();
        return results;
    }

    while (query.next()) {
        LibrarySystem::Book book;

        book.title = query.value("title").toString();
        book.author1 = query.value("author1").toString();
        book.author2 = query.value("author2").toString();
        book.author3 = query.value("author3").toString();
        book.author4 = query.value("author4").toString();
        book.author5 = query.value("author5").toString();
        book.publisher = query.value("publisher").toString();
        book.publicationYear = query.value("publicationYear").toString();
        book.edition = query.value("edition").toString();
        book.ISBN = query.value("ISBN").toString();
        book.volume = query.value("volume").toString();
        book.pageCount = query.value("pageCount").toString();
        book.seriesInformation = query.value("seriesInformation").toString();
        book.language = query.value("language").toString();
        book.DDC = query.value("DDC").toString();
        book.additionalInfo = query.value("additionalInfo").toString();

        results.append(book);
    }

    return results;    

}

bool GeneralOperations::listBooks(){
    
}

bool GeneralOperations::showBookInfo(LibrarySystem::Book book){
    
}