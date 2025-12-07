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

#include "headers/UpdaterWindow.h"

UpdaterWindow::UpdaterWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("LibSys Updater");
    setMinimumSize(600, 450);
    setMaximumSize(600, 450);

    QLabel *libsys_Label = new QLabel(this);
    QPixmap pixmap(":/LibSysUpdater.png");
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
    LibSysInstaller_CheckBox = new QCheckBox("LibSys Installer", this);
    LibSys_CheckBox->setChecked(true);

    QGroupBox *group = new QGroupBox("Settings", this);
    auto *vbox = new QVBoxLayout;
    vbox->addWidget(LibSys_CheckBox);
    vbox->addWidget(LibSysInstaller_CheckBox);
    group->setLayout(vbox);

    mainLayout->addWidget(group);

    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setVisible(false);
    mainLayout->addWidget(progressBar);

    updateButton = new QPushButton("Update", this);
    updateButton->setStyleSheet("font-weight: bold; padding: 6px;");
    updateButton->setObjectName("UpdateButton");

    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(updateButton);
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

        QPushButton#BrowseButton, QPushButton#UpdateButton {
            background-color: #fffffa; 
            color: black;
            border: 1px solid #000000;
            border-radius: 2px;
    
        }
        QPushButton#BrowseButton:hover, QPushButton#UpdateButton:hover {
            background-color: #969695; 
            color: black;
            border: 1px solid #000000;
            border-radius: 2px;
        }

        QPushButton#BrowseButton:pressed, QPushButton#UpdateButton:pressed  {
            background-color: #3e3e3e; 
            color: white;
            border: 1px solid #000000;
            border-radius: 2px;
        }
        
        )");

    connect(browseButton, &QPushButton::clicked, this, &UpdaterWindow::chooseDirectory);

    connect(updateButton, &QPushButton::clicked, this, [this]()
            {
        int selection = 0;
        if (LibSys_CheckBox->isChecked() && LibSysInstaller_CheckBox->isChecked()) selection = 1;
        else if (LibSys_CheckBox->isChecked()) selection = 2;
        else if (LibSysInstaller_CheckBox->isChecked()) selection = 3;
        else selection = 0;

        startUpdate(selection); });
}

// --- Directory selection ---
void UpdaterWindow::chooseDirectory()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select installation directory", pathEdit->text());
    if (!dir.isEmpty())
        pathEdit->setText(dir);
}

// --- File download ---
bool UpdaterWindow::downloadFile(const QString &urlStr, const QString &savePath)
{
    QNetworkAccessManager manager;
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
                             progressBar->setValue(static_cast<int>((bytesReceived * 100) / bytesTotal));
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

void UpdaterWindow::startUpdate(int selection)
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

    QString remoteLibSysVersion = obj["libsys_version"].toString();
    QString remoteInstallerVersion = obj["installer_version"].toString();

    // --------------- LOCAL VERSIONS ----------------------------
    QString localLibSysVersion = "2.0.1";
    QString localInstallerVersion = "2.0.1";

    if (isRemoteVersionNewer(localLibSysVersion, remoteLibSysVersion) ||
        isRemoteVersionNewer(localInstallerVersion, remoteInstallerVersion))
    {
        QMessageBox::information(this, "Update Available",
                                 "A newer version is available and will be installed.");
    }

    QString urlLibSys = obj["libsys_url"].toString();
    QString urlInstaller = obj["installer_url"].toString();

    QString saveLibSys = target + "/LibSys.exe";
    QString saveInstaller = target + "/LibSysInstaller.exe";

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

    if (selection == 1 || selection == 3) // Installer
    {
        if (QFile::exists(saveInstaller) &&
            QMessageBox::question(this, "File exists", "LibSysInstaller.exe already exists. Overwrite?") != QMessageBox::Yes)
            return;

        progressBar->setValue(0);
        if (!downloadFile(urlInstaller, saveInstaller))
        {
            progressBar->setVisible(false);
            QMessageBox::critical(this, "Error", "Failed to download LibSysInstaller.exe");
            return;
        }
    }

    progressBar->setVisible(false);
    QMessageBox::information(this, "Update", "Update completed successfully!");
}

QString UpdaterWindow::getJson(const QString &urlJson)
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

bool UpdaterWindow::isRemoteVersionNewer(const QString &local, const QString &remote)
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