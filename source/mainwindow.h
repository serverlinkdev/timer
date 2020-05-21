#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QSystemTrayIcon>
#include <QTimer>

#include "wizard.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString configFile,
                        const QString &publisher,
                        const QString &appName,
                        QWidget *parent = nullptr);
    ~MainWindow();

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

    void createPalette();

    void createDelayTimer();
    QTimer *delayTimer=nullptr;

    void createPlayer();
    QMediaPlayer *player=nullptr;
    QMediaPlaylist *playlist = nullptr;

    void createActions();
    QAction *hideAction = nullptr;
    QAction *restoreAction = nullptr;
    QAction *stopAction = nullptr;
    QAction *quitAction = nullptr;

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

    Wizard *w = nullptr;
    QMenu *contextMenu = nullptr;

private slots:
    void slotDelayTimer();

    void on_pbAction_clicked();
    void on_lnEd_returnPressed();

    void setIcon(QIcon icon);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void messageClicked();
    void runSoundFilePickerWizard();

protected:
    void closeEvent(QCloseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

};

#endif // MAINWINDOW_H
