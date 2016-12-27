/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "mainview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *mainGB;
    QPushButton *pushButton;
    QPushButton *ImportOBJ;
    MainView *mainView;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1356, 768);
        MainWindow->setStyleSheet(QLatin1String("QGroupBox {\n"
"    border: 1px solid #CCCCCC;\n"
"    border-radius: 9px;\n"
"    margin-top: 8px;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px 0 3px;\n"
"}"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(6, 6, 6, 6);
        mainGB = new QGroupBox(centralWidget);
        mainGB->setObjectName(QStringLiteral("mainGB"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mainGB->sizePolicy().hasHeightForWidth());
        mainGB->setSizePolicy(sizePolicy);
        mainGB->setMinimumSize(QSize(230, 0));
        mainGB->setStyleSheet(QLatin1String("QGroupBox {\n"
"    margin-top: 0px;\n"
"}"));
        pushButton = new QPushButton(mainGB);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 60, 85, 27));
        ImportOBJ = new QPushButton(mainGB);
        ImportOBJ->setObjectName(QStringLiteral("ImportOBJ"));
        ImportOBJ->setGeometry(QRect(10, 20, 85, 27));

        horizontalLayout->addWidget(mainGB);

        mainView = new MainView(centralWidget);
        mainView->setObjectName(QStringLiteral("mainView"));

        horizontalLayout->addWidget(mainView);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        mainGB->setTitle(QString());
        pushButton->setText(QApplication::translate("MainWindow", "Show coords", 0));
        ImportOBJ->setText(QApplication::translate("MainWindow", "Open File", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
