#include "themepicker.h"
#include <QDebug>
#include <QLabel>

ThemePicker::ThemePicker(QDialog *parent) : QDialog (parent)
{
    setWindowTitle("Theme Picker");

    lbl = new QLabel(this);
    lbl->setText("Please choose a color theme.\n\n"
                 "All changes are immediately saved\n");

    box = new QComboBox(this);

    btn = new QDialogButtonBox(QDialogButtonBox::Ok);
    layout = new QVBoxLayout(this);
    connect(btn, &QDialogButtonBox::accepted,
            [=](){hide();});

    layout->addWidget(lbl);
    layout->addWidget(box);
    layout->addWidget(btn);

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
    delete btn;
    delete box;
    delete lbl;
    delete layout;
}
