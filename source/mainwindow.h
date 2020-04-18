#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QMediaPlayer>
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

private slots:
    void on_pbStart_clicked();
    void on_pbStop_clicked();
    void slotDelayTimer();
    void on_lnEd_returnPressed();

private:
    Ui::MainWindow *ui=nullptr;
    QTimer *delayTimer=nullptr;
    QMediaPlaylist *playlist = nullptr;
    QMediaPlayer *player=nullptr;
    QLabel *statusLabel=nullptr;
};

#endif // MAINWINDOW_H
