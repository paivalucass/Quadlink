/********************************************************************************
** Form generated from reading UI file 'quadlink_main_window.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QUADLINK_MAIN_WINDOW_H
#define UI_QUADLINK_MAIN_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_quadlink_main_window
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *quadlink_main_window)
    {
        if (quadlink_main_window->objectName().isEmpty())
            quadlink_main_window->setObjectName(QString::fromUtf8("quadlink_main_window"));
        quadlink_main_window->resize(800, 600);
        centralwidget = new QWidget(quadlink_main_window);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        quadlink_main_window->setCentralWidget(centralwidget);
        menubar = new QMenuBar(quadlink_main_window);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        quadlink_main_window->setMenuBar(menubar);
        statusbar = new QStatusBar(quadlink_main_window);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        quadlink_main_window->setStatusBar(statusbar);

        retranslateUi(quadlink_main_window);

        QMetaObject::connectSlotsByName(quadlink_main_window);
    } // setupUi

    void retranslateUi(QMainWindow *quadlink_main_window)
    {
        quadlink_main_window->setWindowTitle(QCoreApplication::translate("quadlink_main_window", "quadlink_main_window", nullptr));
    } // retranslateUi

};

namespace Ui {
    class quadlink_main_window: public Ui_quadlink_main_window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QUADLINK_MAIN_WINDOW_H
