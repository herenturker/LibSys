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

#ifndef BOOKSEARCHWINDOW_H_
#define BOOKSEARCHWINDOW_H_

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class BookSearchWindow : public QWidget
{
    Q_OBJECT

    public:
        explicit BookSearchWindow(QWidget *parent = nullptr);

    signals:
        void windowClosed();

    protected:
        void closeEvent(QCloseEvent *event) override;
};

#endif // BOOKSEARCHWINDOW_H_