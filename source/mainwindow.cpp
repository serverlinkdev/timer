#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDebug>
#include <QIntValidator>
#include <QMediaPlaylist>
#include <QMenu>
#include <QTime>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    createActions();
    // on start up, no timer running
    stopAction->setDisabled(true);
    createTrayIcon();
    setIcon();
    connect(trayIcon, &QSystemTrayIcon::activated,
            this, &MainWindow::iconActivated);
    connect(trayIcon, &QSystemTrayIcon::messageClicked,
            this, &MainWindow::messageClicked);
    trayIcon->setToolTip("Not running a timer");
    trayIcon->show();

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

    // we'll allow a timer for a full week's worth of minutes
    ui->lnEd->setValidator(new QIntValidator(1,10080, this));

    ui->pbStop->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete delayTimer;
    delete playlist;
    delete player;
    delete statusLabel;
    delete ui;
    delete minimizeAction;
//    delete maximizeAction;
    delete restoreAction;
    delete stopAction;
    delete quitAction;
    delete trayIconMenu;
    delete trayIcon;
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
    trayIcon->setToolTip("Timer ends ~ " + laterShort + " ");
    ui->pbStop->setDisabled(false);
    ui->pbStart->setDisabled(true);
    stopAction->setDisabled(false);
}

void MainWindow::slotDelayTimer()
{
    statusLabel->setText("Timer Expired!");
    setWindowTitle("Timer Expired!");
    trayIcon->setToolTip("Timer Expired!");
    player->play();
    QIcon icon(":/images/stopwatch.png");
    const QString msg = "Timer has expired!";
    trayIcon->showMessage("Timer", msg, icon,1000);
}

void MainWindow::on_pbStop_clicked()
{
    player->stop();
    delayTimer->stop();
    statusLabel->setText("Not Running");
    setWindowTitle("Timer");
    ui->pbStart->setDisabled(false);
    ui->pbStop->setDisabled(true);
    stopAction->setDisabled(true);
    trayIcon->setToolTip("Not running a timer");
}

void MainWindow::on_lnEd_returnPressed()
{
    MainWindow::on_pbStart_clicked();
}

void MainWindow::setIcon()
{
    QIcon icon(":/images/stopwatch.png");
    trayIcon->setIcon(icon);
    setWindowIcon(icon);
    //    trayIcon->setToolTip(iconComboBox->itemText(index));
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        // this is single click
//        qDebug().noquote() << "trigger";
        QMainWindow::showNormal();
        break;
    case QSystemTrayIcon::DoubleClick:
//        qDebug().noquote() << "dbl clicked";
        QMainWindow::hide();
//        iconComboBox->setCurrentIndex((iconComboBox->currentIndex() + 1) % iconComboBox->count());
        break;
    case QSystemTrayIcon::MiddleClick:
//        showMessage();
        break;
    default:
        ;
    }
}

// NOTE: MS Windows requires that if the pop up is activated, and a user
// interacts with the system tray icon, this function must be called.
// So, if the pop up activates, and user clicks on tray icon this will fire
// no matter what.  If this is an issue, you will neeed to override the
// behavior, or perhaps do some validation with an internal variable.
void MainWindow::messageClicked()
{
   on_pbStop_clicked();
   showNormal();
}

void MainWindow::createActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, &QAction::triggered, this, &QMainWindow::hide);

//    maximizeAction = new QAction(tr("Ma&ximize"), this);
//    connect(maximizeAction, &QAction::triggered, this, &QMainWindow::showMaximized);

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &QMainWindow::showNormal);

    stopAction = new QAction(tr("&Stop Alarm"), this);
    connect(stopAction, &QAction::triggered, this, &MainWindow::on_pbStop_clicked);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
//    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(stopAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}
