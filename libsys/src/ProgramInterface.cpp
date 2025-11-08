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


#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QApplication>

#include "headers/ProgramInterface.h"

ProgramInterface::ProgramInterface(QWidget *parent) : QMainWindow(parent)
{
    setWindowIcon(QIcon(":/LibSys.ico"));
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    QWidget *central = new QWidget(this);
    central->setStyleSheet("background-color: white;");
    setCentralWidget(central);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    QFrame *titleBar = new QFrame(this);
    titleBar->setStyleSheet("background-color: #706F6F;");
    titleBar->setFixedHeight(35);

    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(5,0,5,0);

    QLabel *iconLabel = new QLabel;
    QIcon icon(":/LibSys.ico");
    iconLabel->setPixmap(icon.pixmap(20, 20));

    QLabel *titleLabel = new QLabel("LibSys");
    titleLabel->setStyleSheet("color: white; font-weight: bold;");

    titleLayout->addWidget(iconLabel);
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();

    QPushButton *minimizeButton = new QPushButton("-");
    minimizeButton->setFixedSize(30,30);
    minimizeButton->setStyleSheet(
        "QPushButton { background-color: #CCCCCC; font-weight: bold; }"
        "QPushButton:hover { background-color: #AAAAAA; }"
    );
    connect(minimizeButton, &QPushButton::clicked, this, &QWidget::showMinimized);
    titleLayout->addWidget(minimizeButton);

    QPushButton *fullscreenButton = new QPushButton("[ ]");
    fullscreenButton->setFixedSize(30,30);
    fullscreenButton->setStyleSheet(
        "QPushButton { background-color: #CCCCCC; font-weight: bold; }"
        "QPushButton:hover { background-color: #AAAAAA; }"
    );
    connect(fullscreenButton, &QPushButton::clicked, [=](){
        if(isFullScreen()) showNormal();
        else showFullScreen();
    });
    titleLayout->addWidget(fullscreenButton);

    QPushButton *closeButton = new QPushButton("X");
    closeButton->setFixedSize(30,30);
    closeButton->setStyleSheet(
        "QPushButton { background-color: #9A0707; font-weight: bold; }"
        "QPushButton:hover { background-color: #FF0000; }"
    );
    connect(closeButton, &QPushButton::clicked, [=](){
        QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity");
        anim->setDuration(300);
        anim->setStartValue(1);
        anim->setEndValue(0);
        connect(anim, &QPropertyAnimation::finished, this, &QWidget::close);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    });
    titleLayout->addWidget(closeButton);

    mainLayout->addWidget(titleBar);

    QLabel *label = new QLabel("Welcome!");
    label->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(label);

    titleBar->installEventFilter(this);
    resize(400,300);
}

bool ProgramInterface::eventFilter(QObject *obj, QEvent *event)
{
    if(obj->inherits("QFrame")) {
        static QPoint dragPosition;
        if(event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::LeftButton)
                dragPosition = mouseEvent->globalPos() - frameGeometry().topLeft();
        }
        else if(event->type() == QEvent::MouseMove) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->buttons() & Qt::LeftButton)
                move(mouseEvent->globalPos() - dragPosition);
        }
    }
    return QMainWindow::eventFilter(obj,event);
}

