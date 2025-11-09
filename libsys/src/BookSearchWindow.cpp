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

#include "headers/BookSearchWindow.h"
#include <QCloseEvent>

BookSearchWindow::BookSearchWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Child Window");
    resize(300, 200);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel("This is a child window.", this);
    QPushButton *okButton = new QPushButton("OK", this);

    layout->addWidget(label);
    layout->addWidget(okButton);

    connect(okButton, &QPushButton::clicked, this, &BookSearchWindow::close);

    
}

void BookSearchWindow::closeEvent(QCloseEvent *event)
{
    emit windowClosed(); 
    QWidget::closeEvent(event);
}