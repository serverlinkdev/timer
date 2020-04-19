/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *lblMins;
    QLineEdit *lnEd;
    QVBoxLayout *verticalLayout_5;
    QPushButton *pbStart;
    QPushButton *pbStop;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(177, 180);
        QFont font;
        font.setPointSize(12);
        MainWindow->setFont(font);
        MainWindow->setStyleSheet(QString::fromUtf8("QLabel{\n"
"color: white;\n"
"}\n"
"QLineEdit{\n"
"color:white;\n"
"}\n"
"QPushButton{\n"
"background-color: rgb(33,33,33);\n"
"border-radius: 10px;\n"
"color: white;\n"
"}\n"
"QPushButton:hover {\n"
" background-color: rgb(133,130,122);\n"
"}\n"
"QPushButton#pbStart:pressed {\n"
" background-color: rgb(65, 205, 82);     \n"
"}\n"
"QPushButton#pbStop:pressed {\n"
" background-color: red;     \n"
"}\n"
""));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        lblMins = new QLabel(centralWidget);
        lblMins->setObjectName(QString::fromUtf8("lblMins"));
        lblMins->setFont(font);

        horizontalLayout->addWidget(lblMins);

        lnEd = new QLineEdit(centralWidget);
        lnEd->setObjectName(QString::fromUtf8("lnEd"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lnEd->sizePolicy().hasHeightForWidth());
        lnEd->setSizePolicy(sizePolicy);
        lnEd->setFont(font);
        lnEd->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(lnEd);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(41, -1, -1, -1);
        pbStart = new QPushButton(centralWidget);
        pbStart->setObjectName(QString::fromUtf8("pbStart"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pbStart->sizePolicy().hasHeightForWidth());
        pbStart->setSizePolicy(sizePolicy1);
        pbStart->setMinimumSize(QSize(72, 26));
        pbStart->setFont(font);

        verticalLayout_5->addWidget(pbStart);

        pbStop = new QPushButton(centralWidget);
        pbStop->setObjectName(QString::fromUtf8("pbStop"));
        sizePolicy1.setHeightForWidth(pbStop->sizePolicy().hasHeightForWidth());
        pbStop->setSizePolicy(sizePolicy1);
        pbStop->setMinimumSize(QSize(72, 26));
        pbStop->setFont(font);

        verticalLayout_5->addWidget(pbStop);


        verticalLayout->addLayout(verticalLayout_5);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QFont font1;
        font1.setPointSize(14);
        statusBar->setFont(font1);
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Timer", nullptr));
        lblMins->setText(QApplication::translate("MainWindow", "Minutes:   ", nullptr));
        lnEd->setPlaceholderText(QString());
        pbStart->setText(QApplication::translate("MainWindow", "Start", nullptr));
        pbStop->setText(QApplication::translate("MainWindow", "Stop", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
