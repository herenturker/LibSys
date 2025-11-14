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
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

#include "headers/InstallerWindow.h"

InstallerWindow::InstallerWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("LibSys Installer");
    resize(480, 220);

    auto *layout = new QVBoxLayout(this);

    auto *pathLayout = new QHBoxLayout;
    pathEdit = new QLineEdit("C:/Program Files/LibSys", this);
    browseButton = new QPushButton("Browse...", this);
    pathLayout->addWidget(pathEdit);
    pathLayout->addWidget(browseButton);

    shortcutCheck = new QCheckBox("Create desktop shortcut", this);
    shortcutCheck->setChecked(true);

    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 0);
    progressBar->setVisible(false);

    installButton = new QPushButton("Install", this);
    installButton->setStyleSheet("font-weight: bold; padding: 6px;");

    layout->addWidget(new QLabel("Select installation directory:", this));
    layout->addLayout(pathLayout);
    layout->addWidget(shortcutCheck);
    layout->addWidget(progressBar);
    layout->addStretch();
    layout->addWidget(installButton, 0, Qt::AlignRight);

    connect(browseButton, &QPushButton::clicked, this, &InstallerWindow::chooseDirectory);
    connect(installButton, &QPushButton::clicked, this, &InstallerWindow::startInstall);
}

void InstallerWindow::chooseDirectory()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select installation directory", pathEdit->text());
    if (!dir.isEmpty())
        pathEdit->setText(dir);
}

bool InstallerWindow::downloadFile(const QString &urlStr, const QString &savePath)
{
    QUrl url(urlStr);
    QNetworkAccessManager manager;
    QNetworkRequest request(url);

    QNetworkReply *reply = manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Download error:" << reply->errorString();
        reply->deleteLater();
        return false;
    }

    QFile file(savePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot open file for writing:" << savePath;
        reply->deleteLater();
        return false;
    }

    file.write(reply->readAll());
    file.close();
    reply->deleteLater();
    return true;
}

void InstallerWindow::startInstall()
{
    QString target = pathEdit->text();
    if (target.isEmpty()) {
        QMessageBox::warning(this, "Error", "Installation directory is not selected.");
        return;
    }

    progressBar->setVisible(true);
    progressBar->setRange(0, 0);

    QString url = "https://raw.githubusercontent.com/<username>/<repo>/<branch>/LibSysApp.exe";
    QString savePath = target + "/LibSys.exe";

    if (!downloadFile(url, savePath)) {
        QMessageBox::critical(this, "Error", "Failed to download file from GitHub.");
        progressBar->setVisible(false);
        return;
    }

    /*
    if (shortcutCheck->isChecked()) {
        // i will add this
    }
    */

    QMessageBox::information(this, "Installation", "Installation completed successfully!");
    progressBar->setVisible(false);
}