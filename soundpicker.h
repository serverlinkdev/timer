#ifndef SOUNDPICKER_H
#define SOUNDPICKER_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class SoundPicker;
}
QT_END_NAMESPACE

class SoundPicker : public QDialog
{
    Q_OBJECT

public:
    SoundPicker(const QString &configFile, const QString &publisher,
                const QString &appName, QDialog *parent = nullptr);

    ~SoundPicker();

signals:
    void soundFileChanged();

public slots:
    void reject() override;

private:
    const QString configFile;
    const QString publisher;
    const QString appName;
    Ui::SoundPicker *ui;
    bool soundFileLocationDone;
    QString originalSoundFileLocation;
    void writeSettings(const QString &key, const QString &value);
    QString getSetting(const QString &someSetting) const;
    void doWizard();
    void resetMemberVarsForNextRun();

private slots:
    void on_pbBrowse_clicked();
    void on_pbCancel_clicked();
    void on_pbOK_clicked();
};
#endif // SOUNDPICKER_H
