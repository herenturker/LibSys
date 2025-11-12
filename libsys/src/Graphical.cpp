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
#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QRadioButton>
#include <QCoreApplication>

#include "headers/Graphical.h"
#include "headers/Database.h"

Graphical::Graphical(QWidget *parent)
{
    Q_UNUSED(parent);
}

bool Graphical::performAction(QWidget *parent, const QString &text)
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        parent,
        "Confirmation",
        text,
        QMessageBox::Yes | QMessageBox::No);
    return reply == QMessageBox::Yes;
}

bool Graphical::addUserGraphical(QWidget *parent)
{
    QDialog dialog(parent);
    dialog.setWindowTitle("Enter New User's Information");

    QLineEdit *username = new QLineEdit;

    QLineEdit *schoolNo = new QLineEdit;

    QLineEdit *password = new QLineEdit;

    username->setPlaceholderText("Username");
    schoolNo->setPlaceholderText("School Number");
    password->setPlaceholderText("Password");

    QButtonGroup *radioButton_Group = new QButtonGroup(&dialog);
    QRadioButton *accountType_Admin_Button = new QRadioButton("Admin", &dialog);
    QRadioButton *accountType_Student_Button = new QRadioButton("Student", &dialog);
    accountType_Student_Button->setChecked(true);

    radioButton_Group->addButton(accountType_Student_Button);
    radioButton_Group->addButton(accountType_Admin_Button);

    QFormLayout *formLayout = new QFormLayout;

    formLayout->addRow("Username:", username);
    formLayout->addRow("School Number:", schoolNo);
    formLayout->addRow("Password:", password);
    QHBoxLayout *radioLayout = new QHBoxLayout;
    radioLayout->addWidget(accountType_Student_Button);
    radioLayout->addWidget(accountType_Admin_Button);
    formLayout->addRow("Account Type:", radioLayout);

    QPushButton *addButton = new QPushButton("Add");
    QPushButton *cancelButton = new QPushButton("Cancel");

    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    buttonsLayout->addWidget(addButton);
    buttonsLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonsLayout);

    dialog.setStyleSheet(
        "QDialog { background-color: #f0f0f0; color: black;}"
        "QLineEdit { color: black; border: 1px solid black; border-radius: 4px; padding: 3px; }"
        "QPushButton { background-color: #8b8b8b; color: white; border-radius: 4px; padding: 5px; }"
        "QPushButton:hover { background-color: #5f5f5f; }"
        "QPushButton:pressed { background-color: #353535ff; }"
        "QLabel {color: black; background-color: #f0f0f0;}"
        "QRadioButton { padding: 2px; }"
        "QRadioButton::indicator:checked { background-color: #8a0b0b; border-radius: 6px; }"

    );

    QObject::connect(addButton, &QPushButton::clicked, [&]()
                     {
                         QString accountType = radioButton_Group->checkedButton()->text();
                         QString exePath = QCoreApplication::applicationDirPath();
                         QString userdbPath = exePath + "/users.db";

                         Database db(userdbPath, "DB_USERS");
                         if ((username->text() == "") | (schoolNo->text() == "") | (password->text() == "") | (accountType == ""))
                         {
                             dialog.reject();
                         }

                         bool isAdded = db.addUser(username->text(), schoolNo->text(), password->text(), accountType);
                         if (isAdded)
                             dialog.accept();
                     });
    QObject::connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    return dialog.exec() == QDialog::Accepted;
}