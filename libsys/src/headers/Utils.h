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

#ifndef UTILS_H_
#define UTILS_H_

#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>

#include <QWidget>
#include <QString>

/**
 * @brief Checks if the pointer is nullptr or not
 * 
 */
#define DBG_PTR(ptr) qDebug() << #ptr ":" << ((ptr) ? "OK" : "nullptr")

/**
 * @brief Shows a message box
 * 
 * @param parent 
 * @param title 
 * @param text 
 * @param isError 
 */
void showMessage(QWidget *parent, const QString &title, const QString &text, bool isError = true);

/**
 * @brief Performs XOR Encryption/Decryption to input with provided key
 * 
 * @param input 
 * @param key 
 * @return std::string 
 */
std::string xorEncryptDecrypt(const std::string &input, char key);

/**
 * @brief Writes encrypted log with default key 'K'
 * 
 * @param message 
 * @param key 
 */
void writeEncryptedLog(const std::string &message, char key = 'K');

/**
 * @brief Reads encrypted log
 * 
 * @param key 
 * @return std::vector<QString> 
 */
std::vector<QString> readEncryptedLog(char key);

/**
 * @brief Converts the password to AES
 * 
 * @param password 
 * @return QString 
 */
QString convertToAes(const QString &password);

/**
 * @brief Restores the text from AES
 * 
 * @param aesText 
 * @return * QString 
 */
QString convertFromAes(const QString &aesText);

/**
 * @brief Convert std::string to QString
 * 
 * @param text 
 * @return QString 
 */
inline QString stdStringToQString(const std::string& text){
    return QString::fromStdString(text);
}

/**
 * @brief Convert QString to std::string
 * 
 * @param text 
 * @return std::string 
 */
inline std::string QStringTostdString(const QString& text){
    return text.toStdString();
}

#endif // UTILS_H_