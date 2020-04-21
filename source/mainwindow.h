#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QKeyEvent>
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
    enum ButtonColor { green, red };

    bool eventFilter(QObject *watched, QEvent *event) override;
    bool isRunning;
    Ui::MainWindow *ui=nullptr;
    QTimer *delayTimer=nullptr;
    QMediaPlaylist *playlist = nullptr;
    QMediaPlayer *player=nullptr;

    void createActions();
    QAction *minimizeAction = nullptr;
    QAction *restoreAction = nullptr;
    QAction *stopAction = nullptr;
    QAction *quitAction = nullptr;

    void createTrayIcon();
    QSystemTrayIcon *trayIcon = nullptr;
    QMenu *trayIconMenu = nullptr;

    void showAndSetActive();
    void setButtonHoverColor(ButtonColor color);

private slots:
    void slotDelayTimer();

    void on_pbAction_clicked();
//    void on_pbStop_clicked();
    void on_lnEd_returnPressed();

    void setIcon(QIcon icon);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void messageClicked();

};

#endif // MAINWINDOW_H
