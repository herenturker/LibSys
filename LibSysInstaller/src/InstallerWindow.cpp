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

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QProgressBar>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QCheckBox>
#include <QGroupBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QUrl>

#include "headers/InstallerWindow.h"

InstallerWindow::InstallerWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("LibSys Installer");
    setMinimumSize(600, 450);
    setMaximumSize(600, 450);

    QLabel *libsys_Label = new QLabel(this);
    QPixmap pixmap(":/LibSysInstaller.png");
    libsys_Label->setPixmap(pixmap.scaled(450, 250, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    libsys_Label->setAlignment(Qt::AlignCenter);

    auto *mainLayout = new QVBoxLayout(this);

    mainLayout->addWidget(libsys_Label);

    auto *pathLayout = new QHBoxLayout;
    pathEdit = new QLineEdit("C:/Program Files/LibSys", this);
    browseButton = new QPushButton("Browse...", this);
    browseButton->setObjectName("BrowseButton");

    pathLayout->addWidget(pathEdit);
    pathLayout->addWidget(browseButton);

    mainLayout->addWidget(new QLabel("Select installation directory:", this));
    mainLayout->addLayout(pathLayout);

    LibSys_CheckBox = new QCheckBox("LibSys", this);
    LibSysUpdater_CheckBox = new QCheckBox("LibSys Updater", this);
    LibSys_CheckBox->setChecked(true);

    QGroupBox *group = new QGroupBox("Settings", this);
    auto *vbox = new QVBoxLayout;
    vbox->addWidget(LibSys_CheckBox);
    vbox->addWidget(LibSysUpdater_CheckBox);
    group->setLayout(vbox);

    mainLayout->addWidget(group);

    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setVisible(false);

    mainLayout->addWidget(progressBar);

    installButton = new QPushButton("Install", this);
    installButton->setStyleSheet("font-weight: bold; padding: 6px;");
    installButton->setObjectName("InstallButton");

    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(installButton);
    mainLayout->addLayout(buttonLayout);

    mainLayout->addStretch();

    this->setStyleSheet(R"(
        QWidget {
            background-color: #d8d8d6; 
            color: black;
        }

        QCheckBox {
            color: black;
        }

        QCheckBox::indicator {
            width: 16px;
            height: 16px;
        }

        QCheckBox::indicator:checked {
            background-color: green;
            border: 1px solid black;
        }

        QCheckBox::indicator:unchecked {
            background-color: white;
            border: 1px solid black;
        }

        QPushButton#BrowseButton, QPushButton#InstallButton {
            background-color: #fffffa; 
            color: black;
            border: 1px solid #000000;
            border-radius: 2px;
    
        }
        QPushButton#BrowseButton:hover, QPushButton#InstallButton:hover {
            background-color: #969695; 
            color: black;
            border: 1px solid #000000;
            border-radius: 2px;
        }

        QPushButton#BrowseButton:pressed, QPushButton#InstallButton:pressed  {
            background-color: #3e3e3e; 
            color: white;
            border: 1px solid #000000;
            border-radius: 2px;
        }
    )");

    connect(browseButton, &QPushButton::clicked, this, &InstallerWindow::chooseDirectory);

    connect(installButton, &QPushButton::clicked, this, [this]()
            {
        int selection = 0;

        if (LibSys_CheckBox->isChecked() && LibSysUpdater_CheckBox->isChecked())
            selection = 1; // Both
        else if (LibSys_CheckBox->isChecked())
            selection = 2; // Only LibSys
        else if (LibSysUpdater_CheckBox->isChecked())
            selection = 3; // Only Updater
        else
            selection = 0; // None

        startInstall(selection); });
}

void InstallerWindow::chooseDirectory()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select installation directory", pathEdit->text());
    if (!dir.isEmpty())
        pathEdit->setText(dir);
}

bool InstallerWindow::downloadFile(const QString &urlStr, const QString &savePath)
{
    QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(urlStr)));

    QFile file(savePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Cannot open file for writing:" << savePath;
        reply->deleteLater();
        return false;
    }

    QObject::connect(reply, &QNetworkReply::downloadProgress, this,
                     [=](qint64 bytesReceived, qint64 bytesTotal)
                     {
                         if (bytesTotal > 0)
                         {
                             int progress = static_cast<int>((bytesReceived * 100) / bytesTotal);
                             progressBar->setValue(progress);
                         }
                     });

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Download error:" << reply->errorString();
        reply->deleteLater();
        return false;
    }

    file.write(reply->readAll());
    file.close();
    reply->deleteLater();
    return true;
}

bool InstallerWindow::isRemoteVersionNewer(const QString &local, const QString &remote)
{
    QStringList localParts = local.split('.');
    QStringList remoteParts = remote.split('.');
    int maxParts = qMax(localParts.size(), remoteParts.size());

    for (int i = 0; i < maxParts; ++i)
    {
        int l = (i < localParts.size()) ? localParts[i].toInt() : 0;
        int r = (i < remoteParts.size()) ? remoteParts[i].toInt() : 0;
        if (r > l)
            return true;
        if (r < l)
            return false;
    }
    return false;
}

void InstallerWindow::startInstall(int selection)
{
    QString target = pathEdit->text();
    if (target.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Installation directory is not selected.");
        return;
    }

    QString rawJson = getJson("https://raw.githubusercontent.com/herenturker/LibSys/master/version.json");
    if (rawJson.isEmpty())
    {
        QMessageBox::critical(this, "Error", "Failed to fetch version information.");
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(rawJson.toUtf8());
    QJsonObject obj = doc.object();

    // --------------------- LOCAL VERSION ----------------------------------
    QString currentRepoVersion = "1.0.0";
    QString remoteRepoVersion = obj["version"].toString();

    if (isRemoteVersionNewer(currentRepoVersion, remoteRepoVersion))
    {
        QMessageBox::information(this, "Update Available",
                                 "A newer version (" + remoteRepoVersion + ") is available and will be installed.");
    }

    QString urlLibSys = obj["libsys_url"].toString();
    QString urlUpdater = obj["updater_url"].toString();

    QString saveLibSys = target + "/LibSys.exe";
    QString saveUpdater = target + "/LibSysUpdater.exe";

    progressBar->setVisible(true);

    if (selection == 1 || selection == 2) // LibSys
    {
        if (QFile::exists(saveLibSys) &&
            QMessageBox::question(this, "File exists", "LibSys.exe already exists. Overwrite?") != QMessageBox::Yes)
            return;

        progressBar->setValue(0);
        if (!downloadFile(urlLibSys, saveLibSys))
        {
            progressBar->setVisible(false);
            QMessageBox::critical(this, "Error", "Failed to download LibSys.exe");
            return;
        }
    }

    if (selection == 1 || selection == 3) // Updater
    {
        if (QFile::exists(saveUpdater) &&
            QMessageBox::question(this, "File exists", "LibSysUpdater.exe already exists. Overwrite?") != QMessageBox::Yes)
            return;

        progressBar->setValue(0);
        if (!downloadFile(urlUpdater, saveUpdater))
        {
            progressBar->setVisible(false);
            QMessageBox::critical(this, "Error", "Failed to download LibSysUpdater.exe");
            return;
        }
    }

    progressBar->setVisible(false);
    QMessageBox::information(this, "Install", "Installation completed successfully!");
}

QString InstallerWindow::getJson(const QString &urlJson)
{
    QNetworkAccessManager manager;
    QEventLoop loop;

    QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(urlJson)));
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError)
    {
        reply->deleteLater();
        return "";
    }

    QString result = reply->readAll();
    reply->deleteLater();
    return result;
}
