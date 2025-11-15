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

#include "headers/SerialReader.h"
#include <QThread>

SerialReader::SerialReader(QObject* parent)
    : QObject(parent)
{
    moveToThread(&workerThread);
    connect(&workerThread, &QThread::started, this, &SerialReader::readLoop);
}

SerialReader::~SerialReader()
{
    workerThread.requestInterruption();
    workerThread.quit();
    workerThread.wait();
    serial.closeDevice();
}

void SerialReader::startSerialConnection(const char* port, int baudrate)
{
    if (serial.openDevice((char*)port, baudrate) != 1)
    {
        emit eightCharReceived("Serial port error");
        return;
    }

    workerThread.start();
}

void SerialReader::readLoop()
{
    char c;

    while (!QThread::currentThread()->isInterruptionRequested())
    {
        if (serial.readChar(&c) == 1)
        {

            buffer += c;

            if (c == '\n')
            {
                std::string cleaned = buffer;
                cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), '\r'), cleaned.end());
                cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), '\n'), cleaned.end());

                if (cleaned.size() == 8)
                {
                    emit eightCharReceived(QString::fromStdString(cleaned));
                }

                buffer.clear();
            }
        }

        QThread::msleep(2);
    }
}


