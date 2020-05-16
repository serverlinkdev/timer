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
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_2;
    QLabel *lblMins;
    QLineEdit *lnEd;
    QLabel *lblMsg;
    QTextEdit *txtEdMsg;
    QVBoxLayout *verticalLayout_5;
    QPushButton *pbAction;
    QLabel *lblStatus;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(177, 280);
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
"\n"
""));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_3 = new QVBoxLayout(centralWidget);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        lblMins = new QLabel(centralWidget);
        lblMins->setObjectName(QString::fromUtf8("lblMins"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lblMins->sizePolicy().hasHeightForWidth());
        lblMins->setSizePolicy(sizePolicy);
        lblMins->setFont(font);
        lblMins->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(lblMins);

        lnEd = new QLineEdit(centralWidget);
        lnEd->setObjectName(QString::fromUtf8("lnEd"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lnEd->sizePolicy().hasHeightForWidth());
        lnEd->setSizePolicy(sizePolicy1);
        lnEd->setFont(font);
        lnEd->setFrame(false);
        lnEd->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(lnEd);


        verticalLayout->addLayout(verticalLayout_2);

        lblMsg = new QLabel(centralWidget);
        lblMsg->setObjectName(QString::fromUtf8("lblMsg"));
        sizePolicy.setHeightForWidth(lblMsg->sizePolicy().hasHeightForWidth());
        lblMsg->setSizePolicy(sizePolicy);
        lblMsg->setFont(font);
        lblMsg->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lblMsg);

        txtEdMsg = new QTextEdit(centralWidget);
        txtEdMsg->setObjectName(QString::fromUtf8("txtEdMsg"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(txtEdMsg->sizePolicy().hasHeightForWidth());
        txtEdMsg->setSizePolicy(sizePolicy2);
        txtEdMsg->setMaximumSize(QSize(16777215, 100));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        QBrush brush1(QColor(255, 255, 255, 128));
        brush1.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Active, QPalette::PlaceholderText, brush1);
#endif
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush1);
#endif
        QBrush brush2(QColor(164, 166, 168, 96));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        QBrush brush3(QColor(208, 208, 208, 128));
        brush3.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush3);
#endif
        txtEdMsg->setPalette(palette);
        txtEdMsg->setFont(font);
        txtEdMsg->setToolTipDuration(-1);
        txtEdMsg->setFrameShape(QFrame::NoFrame);

        verticalLayout->addWidget(txtEdMsg);


        verticalLayout_3->addLayout(verticalLayout);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(42, -1, -1, -1);
        pbAction = new QPushButton(centralWidget);
        pbAction->setObjectName(QString::fromUtf8("pbAction"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(pbAction->sizePolicy().hasHeightForWidth());
        pbAction->setSizePolicy(sizePolicy3);
        pbAction->setMinimumSize(QSize(72, 33));
        pbAction->setFont(font);

        verticalLayout_5->addWidget(pbAction);


        verticalLayout_3->addLayout(verticalLayout_5);

        lblStatus = new QLabel(centralWidget);
        lblStatus->setObjectName(QString::fromUtf8("lblStatus"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(lblStatus->sizePolicy().hasHeightForWidth());
        lblStatus->setSizePolicy(sizePolicy4);
        lblStatus->setFont(font);
        lblStatus->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(lblStatus);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Timer", nullptr));
        lblMins->setText(QApplication::translate("MainWindow", "Minutes", nullptr));
#ifndef QT_NO_TOOLTIP
        lnEd->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        lnEd->setPlaceholderText(QString());
        lblMsg->setText(QApplication::translate("MainWindow", "Message", nullptr));
#ifndef QT_NO_TOOLTIP
        txtEdMsg->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        txtEdMsg->setPlaceholderText(QApplication::translate("MainWindow", "Type your message here.  Hit enter to start the timer.", nullptr));
        pbAction->setText(QApplication::translate("MainWindow", "Start", nullptr));
        lblStatus->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
