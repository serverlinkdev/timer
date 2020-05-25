#include "themepicker.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>

ThemePicker::ThemePicker(QDialog *parent) : QDialog (parent)
{
    setWindowTitle("Theme Picker");

    auto *layout = new QVBoxLayout(this);
    box = new QComboBox(this);
    layout->addWidget(box);
    setLayout(layout);

    connect(box, QOverload<int>::of(&QComboBox::activated),
            this, &ThemePicker::onComboBoxItemActivated);
}

void ThemePicker::onComboBoxItemActivated()
{
    // Tell mainwindow to set this style
    emit setCssStyleStyleSheet(box->currentText());
}

void ThemePicker::populateCombobox(QStringList cssStylesList)
{
    box->clear();

    cssStylesList.sort();

    for (const auto itm : cssStylesList)
    {
        box->addItem(itm);
    }
}

void ThemePicker::onSendThemesList(QStringList cssStylesList)
{
    // do something with data sent by mainwindow
    populateCombobox(cssStylesList);
}

void ThemePicker::showEvent(QShowEvent *event)
{
    // ask mainwindow for data
    emit getThemesList();
    QDialog::showEvent(event);
}

ThemePicker::~ThemePicker()
{
}
