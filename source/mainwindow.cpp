#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDebug>
#include <QMediaPlaylist>
#include <QTime>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    delayTimer = new QTimer(this);
    connect(delayTimer, &QTimer::timeout,
            this, &MainWindow::slotDelayTimer,Qt::UniqueConnection);

    player = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
    playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl("qrc:/sound/pop.wav"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    player->setPlaylist(playlist);

    statusLabel = new QLabel(this);
    QFont f = statusLabel->font();
    f.setPointSize(12);
    statusLabel->setFont(f);

    // add them to the statusbar:
    ui->statusBar->addPermanentWidget(statusLabel);
    statusLabel->setText("Habouji ! ");
    auto icon = ":/images/stopwatch.png";
    setWindowIcon(QIcon(icon));

    // Sadly when put into the CSS in designer, it won't alow modifying
    // the QlineEdit, which that too completely ignores the CSS in desginer
    // The CSS in Qt is broken !!!!
    QPalette pThis = this->palette();
    pThis.setColor(QPalette::Window, QColor(54,57,63));
    this->setPalette(pThis);

    ui->lnEd->setStyleSheet("background-color:QColor(33,33,33)");

    // Disable resize of the mainwindow
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    setFixedSize(177,180);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // Remove the triangle on bottom right of window that cues user they can resize
    ui->statusBar->setSizeGripEnabled(false);

    // Why oh why is this not recognized in Designers CSS !!!
    ui->lnEd->setFrame(false);

    ui->pbStop->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete delayTimer;
    delete playlist;
    delete player;
    delete statusLabel;
    delete ui;
}

void MainWindow::on_pbStart_clicked()
{
    if (delayTimer->isActive()) return;
    const int val = ui->lnEd->text().toInt();
    if (val==0) return;
    const int delay = val * 60000;
    delayTimer->setSingleShot(true);
    delayTimer->start(delay);
    const QTime now = QTime::currentTime();
    const QString nowShort = now.toString("hh:mm");
    const QTime later = now.addSecs(val*60);
    const QString laterShort=later.toString("hh:mm");
    statusLabel->setText("Timer ends ~ " + laterShort + " ");
    setWindowTitle(laterShort + " Timer ends");
    ui->pbStop->setDisabled(false);
    ui->pbStart->setDisabled(true);
}

void MainWindow::slotDelayTimer()
{
    statusLabel->setText("Timer Expired!");
    setWindowTitle("Timer Expired!");
    player->play();
}

void MainWindow::on_pbStop_clicked()
{
    player->stop();
    delayTimer->stop();
    statusLabel->setText("Not Running");
    setWindowTitle("Timer");
    ui->pbStart->setDisabled(false);
    ui->pbStop->setDisabled(true);
}

void MainWindow::on_lnEd_returnPressed()
{
    MainWindow::on_pbStart_clicked();
}
