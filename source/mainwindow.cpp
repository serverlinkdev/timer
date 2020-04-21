#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDebug>
#include <QEvent>
#include <QIntValidator>
#include <QMediaPlaylist>
#include <QMenu>
#include <QTime>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    isRunning(false)
{
    createActions();
    stopAction->setDisabled(true);
    createTrayIcon();
    QIcon iconDef(":/images/stopwatch.png");
    setIcon(iconDef);
    connect(trayIcon, &QSystemTrayIcon::activated,
            this, &MainWindow::iconActivated);
    connect(trayIcon, &QSystemTrayIcon::messageClicked,
            this, &MainWindow::messageClicked);
    trayIcon->setToolTip("Not running a timer");
    trayIcon->show();

    delayTimer = new QTimer(this);
    connect(delayTimer, &QTimer::timeout,
            this, &MainWindow::slotDelayTimer,Qt::UniqueConnection);

    player = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
    playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl("qrc:/sound/pop.wav"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    player->setPlaylist(playlist);

    ui->setupUi(this);
    ui->lblStatus->setText("Habouji!");
    setButtonHoverColor(green);

    auto icon = ":/images/stopwatch.png";
    setWindowIcon(QIcon(icon));

    // Sadly when put into the CSS in designer, it won't alow modifying
    // the QlineEdit, which that too completely ignores the CSS in desginer
    // The CSS in Qt is broken !!!!
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(54,57,63));
    this->setPalette(palette);

    // we'll allow a timer for a full week's worth of minutes
    ui->lnEd->setValidator(new QIntValidator(1,10080, this));
    ui->lnEd->setStyleSheet("background-color:QColor(33,33,33)");

    ui->txtEdMsg->setStyleSheet("background-color:QColor(33,33,33)");

    // text edits do not have a return key press event, so we'll make our own:
    ui->txtEdMsg->installEventFilter(this);

    // Disable resize of the mainwindow, the minimize and ? buttons in toolbar
    // and also disable resizing the window
    setWindowFlags(Qt::Tool | Qt::MSWindowsFixedSizeDialogHint);
    setFixedSize(177,280);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

void MainWindow::createActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, &QAction::triggered, this, &QMainWindow::hide);

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &MainWindow::showAndSetActive);

    stopAction = new QAction(tr("&Stop Alarm"), this);
    connect(stopAction, &QAction::triggered, this, &MainWindow::on_pbAction_clicked);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(stopAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if ((watched==ui->txtEdMsg) && (event->type()==QEvent::KeyPress))
    {
        if ((static_cast<QKeyEvent *>(event)->key()==Qt::Key_Return) ||
                (static_cast<QKeyEvent *>(event)->key()==Qt::Key_Enter))
        {
            on_pbAction_clicked(); // Run the Timer on enter key Press!
            event->accept();
            return true;
        }
    }
    // return Qt's default implementation:
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        (isHidden()) ? showAndSetActive() : QMainWindow::hide();
        break;
    case QSystemTrayIcon::DoubleClick:
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        ;
    }
}

void MainWindow::on_pbAction_clicked()
{
    if ((!delayTimer->isActive()) && !isRunning) // start
    {
        isRunning=true;
        const int val = ui->lnEd->text().toInt();
        if (val==0) return;
        ui->lnEd->setReadOnly(true);
        const int delay = val * 60000;
        delayTimer->setSingleShot(true);
        delayTimer->start(delay);

        const QTime now = QTime::currentTime();
        const QString nowShort = now.toString("hh:mm");
        const QTime later = now.addSecs(val*60);
        const QString laterShort=later.toString("hh:mm");
        ui->lblStatus->setText("Timer ends ~ " + laterShort + " ");
        setWindowTitle(laterShort + " Timer ends");

        ui->txtEdMsg->setReadOnly(true);
        QString userMessage = ui->txtEdMsg->toPlainText();
        if (userMessage.length()==0) userMessage = "";
        trayIcon->setToolTip(userMessage + " - Timer ends ~ " + laterShort);
        QIcon iconRun(":/images/stopwatch-running");
        setIcon(iconRun);

        setButtonHoverColor(red);
        ui->pbAction->setText("Stop");

        stopAction->setDisabled(false);
    }
    else // stop
    {
        isRunning=false;
        player->stop();
        delayTimer->stop();

        const QString msg = "Not running a timer";
        ui->lblStatus->setText(msg);

        setWindowTitle("Timer");

        setButtonHoverColor(green);
        ui->pbAction->setText("Start");

        stopAction->setDisabled(true);

        trayIcon->setToolTip(msg);

        ui->lnEd->setReadOnly(false);

        ui->txtEdMsg->setReadOnly(false);

        QIcon iconDef(":/images/stopwatch.png");
        setIcon(iconDef);
    }
}

void MainWindow::setButtonHoverColor(MainWindow::ButtonColor color)
{
    QString cssButton;

    (color == green) ?
        cssButton =
            "QPushButton:hover {"
            "background-color: green;"
            "}"
        :
        cssButton =
            "QPushButton:hover {"
            "background-color: red;"
            "}";

    ui->pbAction->setStyleSheet(cssButton);
}

void MainWindow::setIcon(QIcon icon)
{
    trayIcon->setIcon(icon);
    setWindowIcon(icon);
}

void MainWindow::showAndSetActive()
{
    QMainWindow::showNormal();
    QMainWindow::raise();
    QMainWindow::activateWindow();
}

void MainWindow::slotDelayTimer()
{
    const QString msg = "Timer Expired!";
    ui->lblStatus->setText(msg);
    setWindowTitle(msg);
    trayIcon->setToolTip(msg);

    player->play();

    QIcon iconExpired(":/images/stopwatch-expired.png");
    setIcon(iconExpired);

    QString userMessage = ui->txtEdMsg->toPlainText();
    if (userMessage.length()==0) userMessage = "";
    trayIcon->showMessage(msg, userMessage, iconExpired, 2000);
}

void MainWindow::on_lnEd_returnPressed()
{
    ui->txtEdMsg->setFocus();
}

// NOTE: MS Windows requires that if the pop up is activated, and a user
// interacts with the system tray icon, this function must be called.
// So, if the pop up activates, and user clicks on tray icon this will fire
// no matter what.  If this is an issue, you will neeed to override the
// behavior, or perhaps do some validation with an internal variable.
void MainWindow::messageClicked()
{
   on_pbAction_clicked();
   showAndSetActive();
}

MainWindow::~MainWindow()
{
    delete delayTimer;
    delete playlist;
    delete player;
    delete ui;
    delete minimizeAction;
    delete restoreAction;
    delete stopAction;
    delete quitAction;
    delete trayIconMenu;
    delete trayIcon;
}
