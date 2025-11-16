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

#include <QApplication>
#include <QTimer>
#include <QSettings>
#include <QMessageBox>
#include <QDebug>

#include "headers/AdminInterface.h"
#include "headers/StudentInterface.h"
#include "headers/LoginWindow.h"
#include "headers/Database.h"
#include "headers/Utils.h"
#include "headers/SerialReader.h"
#include "headers/LibrarySystem.h"

int main(int argc, char *argv[])
{
    QApplication libsys(argc, argv);
    libsys.setWindowIcon(QIcon(":/LibSys.ico"));
    libsys.setStyleSheet("QWidget { background-color: #DFDEDE; }");

    // ---- Login Window ----
    LoginWindow loginWindow;
    loginWindow.show();

    // ---- Serial Reader ----
    SerialReader reader;

    // ---- QSettings for reading COM port ----
    QSettings settings("LibSys", "ArduinoSettings");
    QString savedPort = settings.value("ArduinoCOMPort", "").toString();
    LibrarySystem::ArduinoCOMPort = savedPort;

    if (!savedPort.isEmpty())
    {
        std::string fullPort = "\\\\.\\" + QStringTostdString(savedPort);
        reader.startSerialConnection(fullPort.c_str(), 115200);
        qDebug() << "SerialReader started on port:" << savedPort;
    }
    else
    {
        qDebug() << "Arduino COM port not set. Admin must enter port first.";
    }

    // ---- RFID signal connections ----
    QObject::connect(&reader, &SerialReader::eightCharReceived,
                     [](const QString &data)
                     { LibrarySystem::updateRFIDDataValue(data); });

    QObject::connect(&reader, &SerialReader::eightCharReceived,
                     &loginWindow, &LoginWindow::updateRFIDLabel);

    // ---- Timer to reset RFID labels ----
    QTimer *resetTimer = new QTimer(&libsys);
    resetTimer->start(5000);
    QObject::connect(resetTimer, &QTimer::timeout, &loginWindow, [&]()
                     { loginWindow.updateRFIDLabel(""); });

    // ---- Login success ----
    QObject::connect(&loginWindow, &LoginWindow::loginSuccess,
                     [&](const QString &accountType, const QString &schoolNo)
                     {
                         loginWindow.close();

                         if (accountType == "Admin")
                         {
                             AdminInterface *adminInterface = new AdminInterface();

                             QObject::connect(&reader, &SerialReader::eightCharReceived,
                                              adminInterface, &AdminInterface::updateRFIDLabel);

                             QObject::connect(resetTimer, &QTimer::timeout,
                                              adminInterface, [=]()
                                              { adminInterface->updateRFIDLabel(""); });

                             adminInterface->show();
                         }
                         else if (accountType == "Student")
                         {
                             StudentInterface *studentInterface = new StudentInterface();
                             studentInterface->setCurrentStudentSchoolNo(schoolNo);

                             QObject::connect(&reader, &SerialReader::eightCharReceived,
                                              studentInterface, &StudentInterface::updateRFIDLabel);

                             QObject::connect(resetTimer, &QTimer::timeout,
                                              studentInterface, [=]()
                                              { studentInterface->updateRFIDLabel(""); });

                             studentInterface->show();
                         }
                     });

    return libsys.exec();
}
