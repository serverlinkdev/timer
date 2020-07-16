#include "themepicker.h"
#include <QLabel>

ThemePicker::ThemePicker(QDialog *parent) : QDialog (parent)
{
    setWindowTitle("Theme Picker");

    lbl = new QLabel(
                "Please choose a color theme.\n\n"
                "All changes are immediately\n"
                "applied and saved\n");

    box = new QComboBox();
    connect(box, QOverload<int>::of(&QComboBox::activated),
            this, &ThemePicker::onComboBoxItemActivated);

    spacer = new QSpacerItem(20,40,QSizePolicy::Minimum,QSizePolicy::Expanding);

    btn = new QDialogButtonBox(QDialogButtonBox::Ok);
    connect(btn, &QDialogButtonBox::accepted,
            [=](){hide();});

    layout = new QVBoxLayout();
    layout->addWidget(lbl);
    layout->addWidget(box);
    layout->addSpacerItem(spacer);
    layout->addWidget(btn);

    setLayout(layout);
}

void ThemePicker::onComboBoxItemActivated()
{
    // Tell mainwindow to set this style
    emit setCssStyleStyleSheet(box->currentText());
}

void ThemePicker::onCurrentTheme(QString theme)
{
    // set the users current theme as theme name shown when window first opens
    box->setCurrentIndex(box->findText(theme));
}

void ThemePicker::onSendThemesList(QStringList cssStylesList)
{
    // do something with data sent by mainwindow
    populateCombobox(cssStylesList);
}

void ThemePicker::populateCombobox(QStringList cssStylesList)
{
    box->clear();

    cssStylesList.sort();

    for (const auto itm : cssStylesList)
    {
        box->addItem(itm);
    }

    emit getCurrentTheme();
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

