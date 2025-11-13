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

#include "headers/Utils.h"

void showMessage(QWidget *parent, const QString &title, const QString &text, bool isError) {
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setWindowModality(Qt::ApplicationModal);

    QString backgroundColor = "#dadada";
    QString labelColor = isError ? "red" : "black";

    msgBox.setStyleSheet(
        QString(
            "QMessageBox { background-color: %1; color: black; }"
            "QLabel { color: %2; font-weight: bold; font-size: 14px; background-color: %1; }"
            "QPushButton { background-color: #8b8b8b; color: white; border-radius: 4px; padding: 5px; min-width: 80px; }"
            "QPushButton:hover { background-color: #5f5f5f; }"
            "QPushButton:pressed { background-color: #353535; }"
        ).arg(backgroundColor, labelColor)
    );

    msgBox.raise();
    msgBox.activateWindow();
    msgBox.exec();
}

std::string xorEncryptDecrypt(const std::string &input, char key) {
    std::string output = input;
    for (size_t i = 0; i < input.size(); ++i) {
        output[i] = input[i] ^ key;
    }
    return output;
}

void writeEncryptedLog(const std::string &message, char key) {
    std::filesystem::path exePath = std::filesystem::current_path();
    std::filesystem::path logPath = exePath / "log.log";

    std::ofstream logFile(logPath, std::ios::app | std::ios::binary);
    if (!logFile.is_open()) {
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

std::vector<QString> readEncryptedLog(char key) {
    std::vector<QString> lines;
    std::ifstream logFile("log.log", std::ios::binary);
    if (!logFile.is_open()) {
        std::cerr << "Cannot open log file for reading!" << std::endl;
        return lines;
    }

    std::string encryptedLine;
    std::string decryptedLine;
    char ch;

    while (logFile.get(ch)) {
        encryptedLine += ch;
        if (ch == '\n') {
            decryptedLine = xorEncryptDecrypt(encryptedLine, key);
            lines.push_back(QString::fromStdString(decryptedLine));
            encryptedLine.clear();
        }
    }

    if (!encryptedLine.empty()) {
        decryptedLine = xorEncryptDecrypt(encryptedLine, key);
        lines.push_back(QString::fromStdString(decryptedLine));
    }

    logFile.close();
    return lines;
}