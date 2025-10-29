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

#ifndef TIMECLASS_H_
#define TIMECLASS_H_

#include <QString>
#include <QDateTime>

class TimeClass 

{

    public:
        static QString showDate ()
        {
            QDate currentDate = QDate::currentDate();
            QString date = currentDate.toString("yyyy-MM-dd");
            return date;
        }

        static QString showTime ()
        {
            QTime currentTime = QTime::currentTime();
            QString time = currentTime.toString("hh:mm:ss");
            return time;
        }

        static QString showDay ()
        {
            QDate currentDate = QDate::currentDate();
            QString day = currentDate.toString("dddd");
            return day;
        }

    };

#endif // TIMECLASS_H_