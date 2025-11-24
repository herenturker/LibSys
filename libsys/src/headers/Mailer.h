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

#ifndef MAILER_H
#define MAILER_H

#include <string>

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>

extern std::string apiKeyValue;

class Mailer : public QObject
{
    Q_OBJECT
    
public:
    explicit Mailer(const QString &apiKey, QObject *parent = nullptr);

    void sendEmail(const QString &toEmail,
                   const QString &toName,
                   const QString &subject,
                   const QString &html,
                   const QString &plain);

signals:
    void emailSent(const QString &referenceId);
    void emailFailed(const QString &error);

private slots:
    void onFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
    QString apiKey;
};

#endif // MAILER_H
