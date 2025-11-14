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

// Book searching window for searching books in the database.

#ifndef BOOKSEARCHWINDOW_H_
#define BOOKSEARCHWINDOW_H_

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTextEdit>

#include "Graphical.h"

class BookSearchWindow : public QWidget
{
    Q_OBJECT

public:
    explicit BookSearchWindow(QWidget *parent = nullptr);
    Graphical* graphical;
    ~BookSearchWindow();

    // For admin operations and choosing operation mode
    enum Mode
    {
        Add,
        Delete,
        Update
    };

    void setMode(Mode m) { currentMode = m; }
    Mode getMode() const { return currentMode; }
    bool bookOperationMode();

signals:
    void windowClosed();

    // For sending current mode's required datas
    
    void bookAddDataReady(
        const QString &bookTitle,
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
        const QString &additionalInfo);

    void bookDeleteDataReady(const QString &bookTitle,
                             const QString &author1,
                             const QString &ISBN);

    void bookUpdateDataReady(const QString &bookTitle,
                             const QString &author1, const QString &publisher,
                             const QString &publicationYear, const QString &edition,
                             const QString &ISBN, const QString &volume,
                             const QString &pageCount, const QString &seriesInformation,
                             const QString &language, const QString &DDC,
                             const QString &additionalInfo);

protected:
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;

public:
    QLineEdit *bookTitle = nullptr;
    QLineEdit *author1 = nullptr;
    QLineEdit *publisher = nullptr;
    QLineEdit *publicationYear = nullptr;
    QLineEdit *edition = nullptr;
    QLineEdit *ISBN = nullptr;
    QLineEdit *volume = nullptr;
    QLineEdit *pageCount = nullptr;
    QLineEdit *seriesInformation = nullptr;
    QLineEdit *language = nullptr;
    QLineEdit *DDC = nullptr;
    QLineEdit *Topic = nullptr;
    QTextEdit *additionalInfo = nullptr;

    QWidget *extraAuthorsWindow = nullptr;

private:
    Mode currentMode = Add;
};

#endif // BOOKSEARCHWINDOW_H_