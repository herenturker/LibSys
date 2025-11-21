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

#ifndef LOGINWINDOW_H_
#define LOGINWINDOW_H_

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QTimer>
#include <QButtonGroup>
#include <QTranslator>

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new Login Window object
     * 
     * @param parent 
     */
    explicit LoginWindow(QWidget *parent = nullptr);

    /**
     * @brief Gets the current school number.
     * 
     * @return QString 
     */
    QString getSchoolNo() const;


    /**
     * @brief Switches program language
     * 
     * @param langCode 
     */
    void switchLanguage(const QString &langCode);

    /**
     * @brief Translates the UI dynamically.
     * 
     */
    void retranslateUi();

    static QTranslator translator;
    static QString language;
    QLabel *RFID_Data_Value;

signals:
    void loginSuccess(const QString &accountType, const QString &schoolNo);
    void languageChanged(const QString &langCode);
    
public slots:
    void updateRFIDLabel(const QString& RFIDdata);
private slots:
    void handleLogin();
    void updateDateTime();

private:

    QLabel *username_Label;
    QLabel *schoolNo_Label;
    QLabel *password_Label;
    QLabel *RFID_Data;

    QLineEdit *username_Edit;
    QLineEdit *password_Edit;
    QLineEdit *schoolNo_Edit;
    
    QButtonGroup *radioButton_Group;
    QButtonGroup *loginRadioButton_Group;
    QButtonGroup *themeRadioButton_Group;

    QRadioButton *accountType_Admin_Button;
    QRadioButton *accountType_Student_Button;
    QRadioButton *normalLogin_Button;
    QRadioButton *quickLogin_Button;
    QRadioButton* toggleDarkTheme;
    QRadioButton* toggleLightTheme;

    QPushButton *login_Button;
    QTimer *timer;
    QLabel *dateLabel;
    QLabel *dayLabel;
    QLabel *timeLabel;
    QString schoolNumber;

    QPushButton* btnTr;
    QPushButton* btnEn;


};

#endif // LOGINWINDOW_H_