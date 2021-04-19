/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTextEdit *textEdit;
    QTextEdit *terminalTextEdit;
    QTextEdit *applicationLogTextEdit;
    QPushButton *pushButton_2;
    QLabel *label;
    QPushButton *pushButton;
    QPushButton *deleteButton;
    QPushButton *nextButton;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QTextEdit *memoryTextEdit;
    QTextEdit *valueTextEdit;
    QTextEdit *nameTextEdit;
    QTextEdit *referenceTextEdit;
    QWidget *portWidget;
    QLabel *label_2;
    QPushButton *backButton;
    QPlainTextEdit *portTextEdit;
    QLabel *label_3;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1087, 746);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(211, 215, 207);"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(10, 40, 571, 391));
        textEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(238, 238, 236);"));
        terminalTextEdit = new QTextEdit(centralwidget);
        terminalTextEdit->setObjectName(QString::fromUtf8("terminalTextEdit"));
        terminalTextEdit->setGeometry(QRect(10, 430, 571, 101));
        terminalTextEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(238, 238, 236);"));
        terminalTextEdit->setReadOnly(true);
        applicationLogTextEdit = new QTextEdit(centralwidget);
        applicationLogTextEdit->setObjectName(QString::fromUtf8("applicationLogTextEdit"));
        applicationLogTextEdit->setGeometry(QRect(13, 579, 571, 121));
        applicationLogTextEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(238, 238, 236);"));
        applicationLogTextEdit->setReadOnly(true);
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(490, 540, 89, 25));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 540, 131, 31));
        label->setStyleSheet(QString::fromUtf8("font: 75 12pt \"Ubuntu Condensed\";\n"
"font: 75 12pt \"Waree\";"));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(10, 10, 89, 25));
        pushButton->setStyleSheet(QString::fromUtf8(""));
        deleteButton = new QPushButton(centralwidget);
        deleteButton->setObjectName(QString::fromUtf8("deleteButton"));
        deleteButton->setGeometry(QRect(490, 10, 89, 25));
        deleteButton->setStyleSheet(QString::fromUtf8(""));
        nextButton = new QPushButton(centralwidget);
        nextButton->setObjectName(QString::fromUtf8("nextButton"));
        nextButton->setGeometry(QRect(110, 10, 89, 25));
        nextButton->setStyleSheet(QString::fromUtf8(""));
        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(600, 40, 471, 661));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        memoryTextEdit = new QTextEdit(horizontalLayoutWidget);
        memoryTextEdit->setObjectName(QString::fromUtf8("memoryTextEdit"));
        memoryTextEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(238, 238, 236);"));
        memoryTextEdit->setReadOnly(true);

        horizontalLayout->addWidget(memoryTextEdit);

        valueTextEdit = new QTextEdit(horizontalLayoutWidget);
        valueTextEdit->setObjectName(QString::fromUtf8("valueTextEdit"));
        valueTextEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(238, 238, 236);"));
        valueTextEdit->setReadOnly(true);

        horizontalLayout->addWidget(valueTextEdit);

        nameTextEdit = new QTextEdit(horizontalLayoutWidget);
        nameTextEdit->setObjectName(QString::fromUtf8("nameTextEdit"));
        nameTextEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(238, 238, 236);"));
        nameTextEdit->setReadOnly(true);

        horizontalLayout->addWidget(nameTextEdit);

        referenceTextEdit = new QTextEdit(horizontalLayoutWidget);
        referenceTextEdit->setObjectName(QString::fromUtf8("referenceTextEdit"));
        referenceTextEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(238, 238, 236);"));
        referenceTextEdit->setReadOnly(true);

        horizontalLayout->addWidget(referenceTextEdit);

        horizontalLayout->setStretch(0, 13);
        horizontalLayout->setStretch(1, 10);
        horizontalLayout->setStretch(2, 10);
        horizontalLayout->setStretch(3, 10);
        portWidget = new QWidget(centralwidget);
        portWidget->setObjectName(QString::fromUtf8("portWidget"));
        portWidget->setGeometry(QRect(320, 190, 481, 201));
        portWidget->setStyleSheet(QString::fromUtf8("border-color: rgb(85, 87, 83);\n"
"background-color: rgb(196, 221, 226);\n"
"selection-color: rgb(238, 238, 236);"));
        label_2 = new QLabel(portWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(50, 30, 391, 17));
        backButton = new QPushButton(portWidget);
        backButton->setObjectName(QString::fromUtf8("backButton"));
        backButton->setGeometry(QRect(200, 130, 89, 25));
        portTextEdit = new QPlainTextEdit(portWidget);
        portTextEdit->setObjectName(QString::fromUtf8("portTextEdit"));
        portTextEdit->setGeometry(QRect(60, 59, 391, 61));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(600, 10, 171, 21));
        label_3->setStyleSheet(QString::fromUtf8(""));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1087, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Application Log", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Run", nullptr));
        deleteButton->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        nextButton->setText(QCoreApplication::translate("MainWindow", "Next", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "INSERTE EL PUERTO", nullptr));
        backButton->setText(QCoreApplication::translate("MainWindow", "Back", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "RAM LIVE VIEW", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
