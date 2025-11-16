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

#include <QCloseEvent>
#include <QMessageBox>
#include <QTimer>

#include "headers/BookSearchWindow.h"
#include "headers/Utils.h"
#include "headers/LibrarySystem.h"

BookSearchWindow::BookSearchWindow(QWidget *parent) : QWidget(parent)
{
    graphical = new Graphical(this);
    setWindowTitle("Enter Book Information");
    resize(855, 600);

    this->setMaximumWidth(855);
    this->setMaximumHeight(600);

    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel("", this);

    // --- Book Title ---
    QLabel *lblBookTitle = new QLabel("Book Title*", this);
    lblBookTitle->setGeometry(50, 30, 110, 20);
    bookTitle = new QLineEdit(this);
    bookTitle->setGeometry(50, 50, 300, 30);
    bookTitle->setPlaceholderText("Enter book title");

    // --- Author 1 ---
    QLabel *lblAuthor1 = new QLabel("Author 1*", this);
    lblAuthor1->setGeometry(50, 90, 110, 20);
    author1 = new QLineEdit(this);
    author1->setGeometry(50, 110, 300, 30);
    author1->setPlaceholderText("Enter author name");

    QPushButton *addAuthors = new QPushButton("+", this);
    addAuthors->setGeometry(330, 90, 20, 20);

    // --- Publisher ---
    QLabel *lblPublisher = new QLabel("Publisher", this);
    lblPublisher->setGeometry(50, 150, 110, 20);
    publisher = new QLineEdit(this);
    publisher->setGeometry(50, 170, 300, 30);
    publisher->setPlaceholderText("Enter publisher name");

    // --- Publication Year ---
    QLabel *lblPubYear = new QLabel("Publication Year", this);
    lblPubYear->setGeometry(50, 210, 110, 20);
    publicationYear = new QLineEdit(this);
    publicationYear->setGeometry(50, 230, 300, 30);
    publicationYear->setPlaceholderText("Enter publication year");

    // --- Edition ---
    QLabel *lblEdition = new QLabel("Edition", this);
    lblEdition->setGeometry(50, 270, 110, 20);
    edition = new QLineEdit(this);
    edition->setGeometry(50, 290, 300, 30);
    edition->setPlaceholderText("Enter edition");

    // --- ISBN ---
    QLabel *lblISBN = new QLabel("ISBN*", this);
    lblISBN->setGeometry(50, 330, 110, 20);
    ISBN = new QLineEdit(this);
    ISBN->setGeometry(50, 350, 300, 30);
    ISBN->setPlaceholderText("Enter ISBN");

    // --- Volume ---
    QLabel *lblVolume = new QLabel("Volume", this);
    lblVolume->setGeometry(400, 30, 110, 20);
    volume = new QLineEdit(this);
    volume->setGeometry(400, 50, 300, 30);
    volume->setPlaceholderText("Enter volume");

    // --- Page Count ---
    QLabel *lblPageCount = new QLabel("Page Count", this);
    lblPageCount->setGeometry(400, 90, 110, 20);
    pageCount = new QLineEdit(this);
    pageCount->setGeometry(400, 110, 300, 30);
    pageCount->setPlaceholderText("Enter page count");

    // --- Series Information ---
    QLabel *lblSeriesInfo = new QLabel("Series Information", this);
    lblSeriesInfo->setGeometry(400, 150, 110, 20);
    seriesInformation = new QLineEdit(this);
    seriesInformation->setGeometry(400, 170, 300, 30);
    seriesInformation->setPlaceholderText("Enter series information");

    // --- Language ---
    QLabel *lblLanguage = new QLabel("Language", this);
    lblLanguage->setGeometry(400, 210, 110, 20);
    language = new QLineEdit(this);
    language->setGeometry(400, 230, 300, 30);
    language->setPlaceholderText("Enter language");

    // --- DDC ---
    QLabel *lblDDC = new QLabel("DDC", this);
    lblDDC->setGeometry(400, 270, 110, 20);
    DDC = new QLineEdit(this);
    DDC->setGeometry(400, 290, 300, 30);
    DDC->setPlaceholderText("Enter DDC");

    // --- Topic ---
    QLabel *lblTopic = new QLabel("Topic", this);
    lblTopic->setGeometry(400, 330, 110, 20);
    Topic = new QLineEdit(this);
    Topic->setGeometry(400, 350, 300, 30);
    Topic->setPlaceholderText("Enter Topic");

    // --- Additional Info ---
    QLabel *lblAdditional = new QLabel("Additional Information", this);
    lblAdditional->setGeometry(50, 430, 150, 20);
    additionalInfo = new QTextEdit(this);
    additionalInfo->setGeometry(50, 450, 650, 100);
    additionalInfo->setPlaceholderText("Enter additional info if any");

    // --- UID Info ---
    QLabel *lblUID = new QLabel("UID", this);
    lblUID->setGeometry(400, 390, 110, 20);
    uid = new QLineEdit(this);
    uid->setGeometry(400, 410, 300, 30);
    uid->setPlaceholderText("Enter UID info if any");

    uid->setText(stdStringToQString(LibrarySystem::rfid_data));


    QPushButton *confirmBtn = new QPushButton("Confirm", this);
    confirmBtn->setGeometry(335, 555, 100, 30);

    connect(confirmBtn, &QPushButton::clicked, this, [this]()
            {
    bool success = bookOperationMode();
        if (success) {
            QTimer::singleShot(0, this, &QWidget::close);
        } });

    layout->addWidget(label);

    this->setStyleSheet(R"(

        QLabel {
            color: black;
            background-color: #dddcdc;
            font-size: 12px;
        }

        QWidget {
            background-color: #dddcdc;
            border: 1px solid #000000;
            border-radius: 2px;
        }

        QWidget#extraAuthorsWindow {
            background-color: #dddcdc;
            border: 2px solid #000000;
            border-radius: 2px;
        }

        QPushButton {
            background-color: #dddcdc;
            border: 1px solid #000000;
            border-radius: 2px;
            color: black;
        }
        
        QPushButton::hover {
            background-color: #8f8e8e;
            border: 1px solid #000000;
            border-radius: 2px;
            color: black;
        }

        QPushButton::pressed {
            background-color: rgba(70, 69, 69, 1);
            border: 1px solid #000000;
            border-radius: 2px;
            color: black;
        }

        QLineEdit {
            color: black;
            font-size: 14px;

        }
            
        QLineEdit::placeholder {
            color: #3f3f3f;
            font-style: italic;
        }

        QTextEdit {
            color: black;
            font-size: 14px;

        }
            
        QTextEdit::placeholder {
            color: #3f3f3f;
            font-style: italic;
        }
        
        
        )");
}

void BookSearchWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    move(100, 100);
}

void BookSearchWindow::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
}

bool BookSearchWindow::bookOperationMode()
{
    QString title = bookTitle->text();
    QString author = author1->text();
    QString publisherText = publisher->text();
    QString pubYear = publicationYear->text();
    QString editionText = edition->text();
    QString ISBNText = ISBN->text();
    QString volumeText = volume->text();
    QString pageCountText = pageCount->text();
    QString seriesText = seriesInformation->text();
    QString languageText = language->text();
    QString DDCText = DDC->text();
    QString additionalInfoText = additionalInfo->toPlainText();
    QString UID = uid->text();

    switch (currentMode)
    {
    case Add:
        emit bookAddDataReady(
            title, author, publisherText, pubYear, editionText,
            ISBNText, volumeText, pageCountText,
            seriesText, languageText, DDCText, additionalInfoText, UID);
        break;

    case Delete:
        emit bookDeleteDataReady(title, author, ISBNText, UID);
        break;

    case Update:
        emit bookUpdateDataReady(
            title, author, publisherText, pubYear, editionText,
            ISBNText, volumeText, pageCountText,
            seriesText, languageText, DDCText, additionalInfoText, UID);
        break;
    }

    return true;
}

BookSearchWindow::~BookSearchWindow()
{
    delete graphical;
}
