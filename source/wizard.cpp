#include "wizard.h"
#include "ui_wizard.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>

Wizard::Wizard(const QString &configFile, const QString &publisher,
               const QString &appName, QDialog *parent) :
    QDialog(parent),
    configFile(configFile),
    publisher(publisher),
    appName(appName),
    ui(new Ui::Wizard),
    soundFileLocationDone(false)
{
    // The QDialog does not pick up our font size at all plus we want bigger
    QFont f;
    f.setPointSize(14);
    QApplication::setFont(f);

    ui->setupUi(this);
    setWindowTitle("Wizard");
    setWindowIcon(QIcon(":/images/stopwatch.png"));

    auto tmp = getSetting("soundFileLocation");
    if (tmp.isEmpty())
    {
        originalSoundFileLocation="FACTORY";
    }
    else
    {
        originalSoundFileLocation=tmp;
    }

    doWizard();
}

QString Wizard::getSetting(const QString &someSetting) const
{
    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       publisher,
                       appName);

    return settings.value(someSetting).toString();
}

// So this func will prepare the UI for its respective config option.
// User clicking ok/next brings us back where the func preps the UI for the
// next config option.  When there are no more to do, well we just close
// the wizard window.
void Wizard::doWizard()
{
    if (!soundFileLocationDone)
    {
        ui->lblGeneral->setText("Please select a new sound file.");

        ui->lblSpecific->setText("HINT:  The file will loop over and over so "
                                 "even a simple sound clip will suffice.");
        ui->pbOK->setText("Next");
        ui->pbOK->setEnabled(false);
        return;
    }

    QDialog::accept();

    emit soundFileChanged();

    auto msg = "Please note: \n\n"
               "This program does not modify your registry (Windows).\n\n"
               "Your settings are saved in file:\n" + configFile + "\n\n"
               "GL HF!";

    QMessageBox::information(this,
                             "Setup Complete!",
                             msg);

    resetMemberVarsForNextRun();
}

void Wizard::on_pbBrowse_clicked()
{
    if (!soundFileLocationDone)
    {
        auto initialPath =
                QStandardPaths::writableLocation(
                    QStandardPaths::MusicLocation);

        if (initialPath.isEmpty()) initialPath = QDir::currentPath();

        auto soundFileLocation =
                QFileDialog::getOpenFileName(this,
                                             "Pick sound file",
                                             initialPath);

        // user pressed esc when file dialog open so as to cancel.
        if (soundFileLocation.isEmpty())
        {
            ui->lblGeneral->setText("No changes will be made to your sound "
                                    "file preferences.");
            ui->lblSpecific->setText("HINT:  You can run this wizard at any "
                                     "time later.");

            soundFileLocationDone = true;
            writeSettings("soundFileLocation", originalSoundFileLocation);
            ui->pbOK->setEnabled(true);
            return;
        }
        else // select file then click cancel in file picker is here as well
        {
            ui->lblGeneral->setText("HINT:  You can run this wizard at any "
                                    "time later.\n\nWill now use the following "
                                    "sound file for alarms: ");
            ui->lblSpecific->setText(soundFileLocation);
            writeSettings("soundFileLocation", soundFileLocation);
            soundFileLocationDone = true;
            ui->pbOK->setEnabled(true);
            return;
        }
    }
}

void Wizard::on_pbCancel_clicked()
{
    Wizard::reject();
}

void Wizard::on_pbOK_clicked()
{
    doWizard();
}

void Wizard::reject()
{
    // There is 3 ways a user can cancel and all must be treated differently.
    // This handles closing the window X and hitting cancel button
    writeSettings("soundFileLocation", originalSoundFileLocation);
    resetMemberVarsForNextRun();
    emit soundFileChanged();
    QDialog::reject();
}

void Wizard::resetMemberVarsForNextRun()
{
    // in case the user wants to run the wizard more than once, let's clean
    // our slate instead of doing 'new' over and over in mainwindow and
    // cleaning pointers etc

    soundFileLocationDone = false;
    doWizard(); // when called with above as false, it just resets the labels
}

Wizard::~Wizard()
{
    disconnect();
    delete ui;
}

void Wizard::writeSettings(const QString &key, const QString &value)
{
    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       publisher,
                       appName);

    settings.setValue(key, value);
}
