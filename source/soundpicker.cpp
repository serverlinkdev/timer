#include "soundpicker.h"
#include "ui_soundpicker.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>

SoundPicker::SoundPicker(const QString &configFile, const QString &publisher,
               const QString &appName, QDialog *parent) :
    QDialog(parent),
    configFile(configFile),
    publisher(publisher),
    appName(appName),
    ui(new Ui::SoundPicker),
    soundFileLocationDone(false)
{
    ui->setupUi(this);
    setWindowTitle("Sound Picker");
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

QString SoundPicker::getSetting(const QString &someSetting) const
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
void SoundPicker::doWizard()
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

void SoundPicker::on_pbBrowse_clicked()
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

void SoundPicker::on_pbCancel_clicked()
{
    SoundPicker::reject();
}

void SoundPicker::on_pbOK_clicked()
{
    doWizard();
}

void SoundPicker::reject()
{
    // There is 3 ways a user can cancel and all must be treated differently.
    // This handles closing the window X and hitting cancel button

    writeSettings("soundFileLocation", originalSoundFileLocation);
    resetMemberVarsForNextRun();
    emit soundFileChanged();
    QDialog::reject();
}

void SoundPicker::resetMemberVarsForNextRun()
{
    // in case the user wants to run the wizard more than once, let's clean
    // our slate instead of doing 'new' over and over in mainwindow and
    // cleaning pointers etc

    soundFileLocationDone = false;
    doWizard(); // when called with above as false, it just resets the labels
}

SoundPicker::~SoundPicker()
{
    disconnect();
    delete ui;
}

void SoundPicker::writeSettings(const QString &key, const QString &value)
{
    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       publisher,
                       appName);

    settings.setValue(key, value);
}
