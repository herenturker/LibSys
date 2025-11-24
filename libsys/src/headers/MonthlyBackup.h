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

#ifndef MONTHLYBACKUP_H
#define MONTHLYBACKUP_H

#include <QObject>
#include <QString>

class MonthlyBackup : public QObject
{
    Q_OBJECT
public:
    explicit MonthlyBackup(QObject *parent = nullptr);

    void run();

private:
    QString readLastSent(const QString &statusFile);

    void writeLastSent(const QString &statusFile, const QString &month);

    QString createZip(const QString &baseDir);
    
    void sendBackupEmail(const QString &zipPath);
};

#endif // MONTHLYBACKUP_H
