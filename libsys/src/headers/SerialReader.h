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

#ifndef SERIALREADER_H_
#define SERIALREADER_H_

#include <QObject>
#include <QString>
#include <QThread>

#include "serialib.h"

class SerialReader : public QObject
{
    Q_OBJECT

public:
    explicit SerialReader(QObject* parent = nullptr);
    ~SerialReader();

    void startSerialConnection(const char* port, int baudrate);

signals:
    void eightCharReceived(const QString& data);

private:
    void readLoop();

    QThread workerThread;
    serialib serial;
    std::string buffer;
};

#endif // SERIALREADER_H_