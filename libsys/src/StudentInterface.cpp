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

#include "headers/StudentInterface.h"
#include "headers/StudentOperations.h"
#include "headers/LoginWindow.h"
#include "headers/TimeClass.h"
#include "headers/BookSearchWindow.h"


StudentInterface::StudentInterface(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("LibSys Student Dashboard");

    resize(1080, 720);
    setMinimumSize(1080, 720);
    setMaximumSize(1080, 720);

    dateLabel = new QLabel(this);
    dayLabel  = new QLabel(this);
    timeLabel = new QLabel(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &StudentInterface::updateDateTime);
    timer->start(1000);

    dateLabel->setGeometry(75, 650, 200, 30);
    dayLabel->setGeometry(75, 620, 200, 30);
    timeLabel->setGeometry(75, 590, 200, 30);

    dateLabel->setObjectName("dateLabel");
    dayLabel->setObjectName("dayLabel");
    timeLabel->setObjectName("timeLabel");

    bookSearchWindow = new BookSearchWindow(this);

    openButton = new QPushButton("Open Book Search", this);
    openButton->setGeometry(500, 40, 150, 40);

        connect(openButton, &QPushButton::clicked, [=]() {
        openButton->setEnabled(false);
        bookSearchWindow->show();
    });

    connect(bookSearchWindow, &BookSearchWindow::windowClosed, [=]() {
        openButton->setEnabled(true);
    });

    searchContainer = new QWidget(this);
    searchContainer->setGeometry(75, 40, 400, 40);

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

        )");
    // TODO: BU ÖZELLİKLERİ BOOK SEARCH WINDOW İÇİNE KOY
    // YORUM EKLEME ÖZELLİĞİ GETİR
    // ADMİN HER ŞEYİ GÖRÜP SİLME YETKİSİNE SAHİP OLMALI
}

void StudentInterface::updateDateTime()
{
    dateLabel->setText("Date: " + TimeClass::showDate());
    dayLabel->setText("Day: " + TimeClass::showDay());
    timeLabel->setText("Time: " + TimeClass::showTime());
}
