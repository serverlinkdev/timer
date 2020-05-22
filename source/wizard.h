#ifndef WIZARD_H
#define WIZARD_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Wizard; }
QT_END_NAMESPACE

class Wizard : public QDialog
{
    Q_OBJECT

public:
    Wizard(const QString &configFile, const QString &publisher,
           const QString &appName, QDialog *parent = nullptr);

    ~Wizard();

signals:
    void soundFileChanged();

public slots:
    void reject() override;

private:
    const QString configFile;
    const QString publisher;
    const QString appName;
    Ui::Wizard *ui;
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
#endif // WIZARD_H