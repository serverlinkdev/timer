#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QStringList>

#include "themepicker.h"
#include "wizard.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString &configFile,
                        const QString &publisher,
                        const QString &appName,
                        QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onGetThemesList();

signals:
    void sendThemesList(QStringList cssStylesList);

private:
    enum ButtonColor { green, red };

    bool eventFilter(QObject *watched, QEvent *event) override;
    Ui::MainWindow *ui=nullptr;
    QString configFile;
    QString publisher;
    QString appName;
    bool isRunning;
    int mainwindowHeight;
    int mainwindowWidth;
    QPoint mainwindowScreenCoordinates;

    QString getSetting(const QString &someSetting) const;
    void writeSettings(const QString &key,
                       const QString &value);

    void setCssStyleSheet(const QString &themeName);
    void createPalette();

    void createDelayTimer();
    QTimer *delayTimer=nullptr;

    void createPlayer();
    void createPlaylist();
    void changePlaylist();
    QMediaPlayer *player=nullptr;
    QString factorySoundFile;
    QMediaPlaylist *playlist = nullptr;

    void createActions();
    QAction *aboutAction = nullptr;
    QAction *hideAction = nullptr;
    QAction *quitAction = nullptr;
    QAction *restoreAction = nullptr;
    QAction *stopAction = nullptr;
    QAction *themeAction = nullptr;
    QAction *wizardAction = nullptr;

    void createTrayIcon();
    QMenu *trayIconMenu = nullptr;
    QSystemTrayIcon *trayIcon = nullptr;

    void hideMainWindow();

    void onRestore();
    void setButtonHoverColor(ButtonColor color);
    void updateMainwindowMemberVars();
    void showAndSetActive();

    void tweakWindowFlags();
    void tweakUi();

    void createThemePicker();
    ThemePicker *t = nullptr;

    void createWizard();
    Wizard *w = nullptr;
    QMenu *contextMenu = nullptr;

private slots:
    void slotDelayTimer();
    void on_pbAction_clicked();
    void on_lnEd_returnPressed();

    void setIcon(QIcon icon);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void messageClicked();

    void onAboutClicked();
    void onSoundFilePickerClicked();
    void onThemePickerClicked();

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
