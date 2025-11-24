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

#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QSettings>

#include "headers/mailer.h"
#include "headers/Utils.h"

std::string apiKeyValue = QStringTostdString(
    QSettings("LibSys", "EmailSettings").value("EmailAPI", "").toString()
);

Mailer::Mailer(const QString &apiKey, QObject *parent) : QObject(parent), apiKey(apiKey)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Mailer::onFinished);
}

void Mailer::sendEmail(const QString &toEmail,
                       const QString &toName,
                       const QString &subject,
                       const QString &html,
                       const QString &plain)
{
    QUrl url("https://smtp.maileroo.com/api/v2/emails");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("Bearer " + apiKey).toUtf8());

    QSettings settings("LibSys", "EmailSettings");
    QString senderEmail = settings.value("SenderEmail", "").toString();

    if (senderEmail.isEmpty()) {
        showMessage(nullptr, "Error", "Sender email is not configured!", true);
        return;
    }

    QJsonObject fromObj;
    fromObj["address"] = senderEmail;

    fromObj["display_name"] = "LibSys from Cemberlitas Anatolian High School";

    QJsonObject toObj;
    toObj["address"] = toEmail;
    toObj["display_name"] = toName;

    QJsonArray toArray;
    toArray.append(toObj);

    QJsonObject emailObj;
    emailObj["from"] = fromObj;
    emailObj["to"] = toArray;
    emailObj["subject"] = subject;
    emailObj["html"] = html;
    emailObj["plain"] = plain;

    QJsonDocument doc(emailObj);
    QByteArray data = doc.toJson();

    manager->post(request, data);
}

void Mailer::onFinished(QNetworkReply *reply)
{
    QByteArray respData = reply->readAll();

    if (reply->error() != QNetworkReply::NoError)
    {
        QMessageBox::critical(nullptr, "Email Failed", reply->errorString());
        emit emailFailed(reply->errorString());
    }
    else
    {
        QJsonDocument respDoc = QJsonDocument::fromJson(respData);
        QJsonObject obj = respDoc.object();

        if (obj.contains("success") && obj["success"].toBool())
        {
            QString refId;
            if (obj.contains("data"))
            {
                QJsonObject dataObj = obj["data"].toObject();
                refId = dataObj.value("reference_id").toString();
            }
            QMessageBox::information(nullptr, "Email Sent",
                                     "Email sent successfully!\nReference ID: " + refId);
            emit emailSent(refId);
        }
        else
        {
            QString errMsg = obj.contains("message") ? obj["message"].toString() : "Unknown error";
            QMessageBox::warning(nullptr, "Email Failed", errMsg);
            emit emailFailed(errMsg);
        }
    }

    reply->deleteLater();
}
