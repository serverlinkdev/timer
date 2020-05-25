#include "themepicker.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>

ThemePicker::ThemePicker(QDialog *parent) : QDialog (parent)
{
    qDebug().noquote() << "Call: " << Q_FUNC_INFO;
    auto *layout = new QVBoxLayout(this);
    box = new QComboBox(this);
    emit getCssStylesList();
    layout->addWidget(box);
    setLayout(layout);
    setWindowTitle("Theme Picker");
    connect(this, &ThemePicker::cssStylesStringListUpdated,
            this, &ThemePicker::populateCombobox);
    connect(box, QOverload<int>::of(&QComboBox::activated),
          [=](int index){
        /* ... */
        qDebug().noquote() << "was actviated";
        qDebug().noquote() << box->currentText();
        emit setCssStyleStyleSheet(box->currentText());
    });
}

void ThemePicker::populateCombobox(QStringList cssStylesStringList)
{
    qDebug().noquote() << "Call: " << Q_FUNC_INFO;
    box->clear();

    cssStylesStringList.sort();

    for (const auto itm : cssStylesStringList)
    {
        box->addItem(itm);
    }
}

void ThemePicker::recvCssStyles(QStringList cssStylesList)
{
    qDebug().noquote() << "Call: " << Q_FUNC_INFO;
    setCssStylesList(cssStylesList);
}

void ThemePicker::setCssStylesList(QStringList cssStylesList)
{
    qDebug().noquote() << "Call: " << Q_FUNC_INFO;
    cssStylesStringList = cssStylesList;
    emit cssStylesStringListUpdated(cssStylesStringList);
}

ThemePicker::~ThemePicker()
{
}


void ThemePicker::showEvent(QShowEvent *event)
{
    qDebug().noquote() << "Call: " << Q_FUNC_INFO;
    emit getCssStylesList();
    QDialog::showEvent(event);
}
