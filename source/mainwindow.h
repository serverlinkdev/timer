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
    Ui::MainWindow *ui=nullptr;
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
    QAction *minimizeAction = nullptr;
    QAction *restoreAction = nullptr;
    QAction *stopAction = nullptr;
    QAction *quitAction = nullptr;

    void createTrayIcon();
    QMenu *trayIconMenu = nullptr;
    QSystemTrayIcon *trayIcon = nullptr;

    void setButtonHoverColor(ButtonColor color);
    void showAndSetActive();

private slots:
    void slotDelayTimer();

    void on_pbAction_clicked();
    void on_lnEd_returnPressed();

    void setIcon(QIcon icon);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void messageClicked();

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
