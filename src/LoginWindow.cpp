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
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QSizePolicy>

#include "headers/LoginWindow.h"

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent)

{
    QLabel *username_Label = new QLabel("Enter Username:");
    username_Edit = new QLineEdit;

    QLabel *schoolNo_Label = new QLabel("Enter School No:");
    schoolNo_Edit = new QLineEdit;

    QLabel *password_Label = new QLabel("Enter Password:");
    password_Edit = new QLineEdit;
    password_Edit->setEchoMode(QLineEdit::Password);

    login_Button = new QPushButton("Login");

    accountType_Admin_Button = new QRadioButton("Admin", this);
    accountType_Student_Button = new QRadioButton("Student", this);
    accountType_Teacher_Button = new QRadioButton("Teacher", this);

    QVBoxLayout *layout_Vertical = new QVBoxLayout;
    QButtonGroup *radioButton_Group = new QButtonGroup(this);

    
    layout_Vertical->addWidget(username_Label);
    layout_Vertical->addWidget(username_Edit);

    layout_Vertical->addWidget(schoolNo_Label);
    layout_Vertical->addWidget(schoolNo_Edit);

    layout_Vertical->addWidget(password_Label);
    layout_Vertical->addWidget(password_Edit);

    layout_Vertical->addWidget(login_Button);

    radioButton_Group->addButton(accountType_Student_Button);
    radioButton_Group->addButton(accountType_Teacher_Button);
    radioButton_Group->addButton(accountType_Admin_Button);


    QVBoxLayout *layout_Radio = new QVBoxLayout;
    layout_Radio->addWidget(accountType_Student_Button);
    layout_Radio->addWidget(accountType_Teacher_Button);
    layout_Radio->addWidget(accountType_Admin_Button);
    layout_Radio->addStretch();

    accountType_Student_Button->setChecked(true);


    QHBoxLayout *layout_Main = new QHBoxLayout;
    layout_Main->addStretch();             
    layout_Main->addLayout(layout_Radio);  
    layout_Main->addSpacing(20);           
    layout_Main->addLayout(layout_Vertical);
    layout_Main->addStretch();              


    layout_Main->setSpacing(15);                         
    layout_Main->setContentsMargins(20, 30, 20, 30);      
    layout_Vertical->setSpacing(8);                      
    layout_Vertical->setContentsMargins(0, 0, 0, 0);
    layout_Radio->setSpacing(6);

    setLayout(layout_Main);
 
    setWindowTitle("Login LibSys");
    resize(400, 200);
    setMinimumSize(400, 250);
    setMaximumWidth(500);
    setMaximumHeight(270);

    connect(login_Button, &QPushButton::clicked, this, &LoginWindow::handleLogin);

};

void LoginWindow::handleLogin ()

{
    QString username = username_Edit->text();
    QString schoolNo = schoolNo_Edit->text();
    QString password = password_Edit->text();



    /*
    if (username == "admin" && password == "1234") {
        emit loginSuccess();
        close();
    } else {
        QMessageBox::warning(this, "Hata", "Kullanıcı adı veya şifre yanlış!");
    }

    */
}
