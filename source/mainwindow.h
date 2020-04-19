#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QLabel>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QSystemTrayIcon>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private:
    Ui::MainWindow *ui=nullptr;
    QTimer *delayTimer=nullptr;
    QMediaPlaylist *playlist = nullptr;
    QMediaPlayer *player=nullptr;
    QLabel *statusLabel=nullptr;

    // system tray items
    void createActions();
    void createTrayIcon();
    QAction *minimizeAction = nullptr;
//    QAction *maximizeAction = nullptr;
    QAction *restoreAction = nullptr;
    QAction *stopAction = nullptr;
    QAction *quitAction = nullptr;
    QSystemTrayIcon *trayIcon = nullptr;
    QMenu *trayIconMenu = nullptr;

private slots:
    void on_pbStart_clicked();
    void on_pbStop_clicked();
    void slotDelayTimer();
    void on_lnEd_returnPressed();

    // system tray items
    void setIcon();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
};

#endif // MAINWINDOW_H
