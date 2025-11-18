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

#include <QMessageBox>
#include <QByteArray>
#include <QString>

#include "headers/AES.h"

#include "headers/Utils.h"

void showMessage(QWidget *parent, const QString &title, const QString &text, bool isError)
{

    if (parent)
    {
        parent->raise();
        parent->activateWindow();
    }

    QMessageBox msgBox(parent);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setWindowModality(Qt::WindowModal);

    msgBox.raise();
    msgBox.activateWindow();
    msgBox.exec();
}

std::string xorEncryptDecrypt(const std::string &input, char key)
{
    std::string output = input;
    for (size_t i = 0; i < input.size(); ++i)
    {
        output[i] = input[i] ^ key;
    }
    return output;
}

void writeEncryptedLog(const std::string &message, char key)
{
    std::filesystem::path exePath = std::filesystem::current_path();
    std::filesystem::path logPath = exePath / "log.log";

    std::ofstream logFile(logPath, std::ios::app | std::ios::binary);
    if (!logFile.is_open())
    {
        std::cerr << "Cannot open or create log file: " << logPath << std::endl;
        return;
    }

    std::time_t now = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    std::string logLine = "[" + std::string(buf) + "] " + message + "\n";

    std::string encrypted = xorEncryptDecrypt(logLine, key);

    logFile.write(encrypted.c_str(), encrypted.size());
    logFile.close();
}

std::vector<QString> readEncryptedLog(char key)
{
    std::vector<QString> logEntries;
    std::ifstream logFile("log.log", std::ios::binary);
    if (!logFile.is_open())
    {
        std::cerr << "Cannot open log file for reading!" << std::endl;
        return logEntries;
    }

    std::string buffer;
    std::string currentEntry;
    char ch;

    while (logFile.get(ch))
    {
        buffer += ch;

        if (ch == '\n')
        {
            std::string decrypted = xorEncryptDecrypt(buffer, key);
            QString qline = QString::fromStdString(decrypted).trimmed();

            if (qline.startsWith('['))
            {
                if (!currentEntry.empty())
                {
                    logEntries.push_back(QString::fromStdString(currentEntry));
                }
                currentEntry = qline.toStdString();
            }
            else
            {
                if (!currentEntry.empty())
                {
                    currentEntry += " " + qline.toStdString();
                }
                else
                {
                    currentEntry = qline.toStdString();
                }
            }
            buffer.clear();
        }
    }

    if (!buffer.empty())
    {
        std::string decrypted = xorEncryptDecrypt(buffer, key);
        QString qline = QString::fromStdString(decrypted).trimmed();
        if (qline.startsWith('['))
        {
            if (!currentEntry.empty())
            {
                logEntries.push_back(QString::fromStdString(currentEntry));
            }
            currentEntry = qline.toStdString();
        }
        else
        {
            if (!currentEntry.empty())
            {
                currentEntry += " " + qline.toStdString();
            }
            else
            {
                currentEntry = qline.toStdString();
            }
        }
    }

    if (!currentEntry.empty())
    {
        logEntries.push_back(QString::fromStdString(currentEntry));
    }

    logFile.close();
    return logEntries;
}

static const QByteArray AES_KEY = QByteArray::fromHex("9f1a3b7c5d8e2f0412ab6c9d0e3f4a1b");

static const QByteArray AES_IV = QByteArray::fromHex("1c2d3e4f5a6b7c8091a2b3c4d5e6f708");

QString convertToAes(const QString &password)
{
    AES aes(AESKeyLength::AES_128);

    QByteArray passBytes = password.toUtf8();

    int padLen = 16 - (passBytes.size() % 16);
    QByteArray padded = passBytes;
    padded.append(QByteArray(padLen, char(padLen)));

    std::vector<unsigned char> input(padded.begin(), padded.end());
    std::vector<unsigned char> key(AES_KEY.begin(), AES_KEY.end());
    std::vector<unsigned char> iv(AES_IV.begin(), AES_IV.end());

    std::vector<unsigned char> encrypted = aes.EncryptCBC(input, key, iv);

    QByteArray encryptedBA(reinterpret_cast<const char *>(encrypted.data()), encrypted.size());
    return QString(encryptedBA.toBase64());
}

QString convertFromAes(const QString &aesText)
{
    AES aes(AESKeyLength::AES_128);

    QByteArray encryptedBA = QByteArray::fromBase64(aesText.toUtf8());

    std::vector<unsigned char> input(encryptedBA.begin(), encryptedBA.end());
    std::vector<unsigned char> key(AES_KEY.begin(), AES_KEY.end());
    std::vector<unsigned char> iv(AES_IV.begin(), AES_IV.end());

    std::vector<unsigned char> decrypted = aes.DecryptCBC(input, key, iv);

    QByteArray decryptedBA(reinterpret_cast<const char *>(decrypted.data()), decrypted.size());

    int padLen = decryptedBA.back();
    if (padLen > 0 && padLen <= 16)
    {
        decryptedBA.chop(padLen);
    }

    return QString::fromUtf8(decryptedBA);
}
