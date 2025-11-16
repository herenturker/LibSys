/*
    LibSys Updater, Library Management System Software Updater
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

#ifndef UPDATER_WINDOW_H_
#define UPDATER_WINDOW_H_

#include <QWidget>
#include <QQueue>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPair>
#include <QString>

class QPushButton;
class QLineEdit;
class QLabel;
class QCheckBox;
class QProgressBar;

class UpdaterWindow : public QWidget
{
    Q_OBJECT

public:
    explicit UpdaterWindow(QWidget *parent = nullptr);
    bool downloadFile(const QString &urlStr, const QString &savePath);
    QString getJson(const QString &urlJson);
    bool isRemoteVersionNewer(const QString &local, const QString &remote);

private slots:
    void chooseDirectory();
    void startUpdate(int selection);

private:
    QLineEdit *pathEdit;
    QPushButton *browseButton;
    QPushButton *updateButton;
    QProgressBar *progressBar;
    QCheckBox *LibSys_CheckBox;
    QCheckBox *LibSysInstaller_CheckBox;
};

#endif // UPDATER_WINDOW_H_
