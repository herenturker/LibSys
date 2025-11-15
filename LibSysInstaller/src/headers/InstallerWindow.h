/*
    LibSys Installer, Library Management System Software Installer
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

#ifndef INSTALLER_WINDOW_H_
#define INSTALLER_WINDOW_H_

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

class InstallerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit InstallerWindow(QWidget *parent = nullptr);
    bool downloadFile(const QString &urlStr, const QString &savePath);
    QString getJson(const QString &urlJson);

private slots:
    void chooseDirectory();
    void startInstall(int selection);

private:
    QLineEdit *pathEdit;
    QPushButton *browseButton;
    QPushButton *installButton;
    QProgressBar *progressBar;
    QCheckBox *LibSys_CheckBox;
    QCheckBox *LibSysUpdater_CheckBox;
};

#endif // INSTALLER_WINDOW_H_