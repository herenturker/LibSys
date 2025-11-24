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

#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QProcess>
#include <QDebug>
#include <QSettings>

#include "headers/MonthlyBackup.h"
#include "headers/Mailer.h"

MonthlyBackup::MonthlyBackup(QObject *parent)
    : QObject(parent)
{
}

void MonthlyBackup::run()
{
    QString baseDir = QCoreApplication::applicationDirPath();
    QString statusFile = baseDir + "/backup_status.txt";
    QString thisMonth = QDate::currentDate().toString("yyyy-MM");

    QString lastSent = readLastSent(statusFile);

    if (lastSent != thisMonth) {
        QString zipPath = createZip(baseDir);
        if (!zipPath.isEmpty()) {
            qDebug() << "ZIP created:" << zipPath;

            sendBackupEmail(zipPath);
            writeLastSent(statusFile, thisMonth);
        }
    } else {
        qDebug() << "Backup is already created this month.";
    }
}

void MonthlyBackup::sendBackupEmail(const QString &zipPath)
{
    QFile file(zipPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open ZIP file for sending:" << zipPath;
        return;
    }

    QByteArray zipData = file.readAll();
    file.close();

    QString base64Zip = zipData.toBase64();

    QSettings settings("LibSys", "EmailSettings");
    QString apiKey = settings.value("EmailAPI", "").toString();

    if (apiKey.isEmpty()) {
        qDebug() << "Mail API key not configured!";
        return;
    }

    Mailer mailer(apiKey);

    QString toEmail = settings.value("AdminEmail", "").toString();
    QString toName = "Dear LibSys Admin";
    QString subject = "Monthly Backup - " + QDate::currentDate().toString("yyyy_MM");

    QString htmlBody = "<p>Attached is the monthly backup.</p>";
    htmlBody += "<p>ZIP file is attached as base64.</p>";

    QString plainBody = "Attached is the monthly backup (ZIP in base64).";

    htmlBody += "<pre>" + base64Zip.left(2000) + "...</pre>";

    mailer.sendEmail(toEmail, toName, subject, htmlBody, plainBody);

    qDebug() << "Backup email sent (check Maileroo API response).";
}


QString MonthlyBackup::readLastSent(const QString &statusFile)
{
    QFile file(statusFile);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        QString line = in.readLine(); // last_sent=yyyy-MM
        if (line.startsWith("last_sent=")) {
            return line.mid(10);
        }
    }
    return "";
}

void MonthlyBackup::writeLastSent(const QString &statusFile, const QString &month)
{
    QFile file(statusFile);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&file);
        out << "last_sent=" << month;
    }
}

QString MonthlyBackup::createZip(const QString &baseDir)
{
    QString folderToBackup = baseDir + "/databases";
    QString zipFileName = "backup_" + QDate::currentDate().toString("yyyy_MM") + ".zip";
    QString zipPath = baseDir + "/" + zipFileName;

    QString cmd = QString(
        "powershell.exe Compress-Archive -Path \"%1\" -DestinationPath \"%2\" -Force"
    ).arg(folderToBackup).arg(zipPath);

    int ret = QProcess::execute(cmd);
    if (ret == 0 && QFile::exists(zipPath)) {
        return zipPath;
    } else {
        qDebug() << "Could not create zip";
        return "";
    }
}
