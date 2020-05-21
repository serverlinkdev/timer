#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDebug>
#include <QEvent>
#include <QIntValidator>
#include <QMediaPlaylist>
#include <QMenu>
#include <QSettings>
#include <QTime>
#include <QUrl>

MainWindow::MainWindow(const QString &configFile,
                       const QString &publisher,
                       const QString &appName,
                       QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    configFile(configFile),
    publisher(publisher),
    appName(appName),
    isRunning(false),
    mainwindowHeight(280),
    mainwindowWidth(177),
    factorySoundFile("qrc:/sound/pop.wav")
{
    createPalette();
    createActions();
    createTrayIcon();
    createDelayTimer();
    createPlayer();
    createWizard();

    ui->setupUi(this);

    tweakUi();
    tweakWindowFlags();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    updateMainwindowMemberVars();

    hideAction->setDisabled(true);
    restoreAction->setDisabled(false);

    QMainWindow::closeEvent(event);
    QApplication::exit(0); // TODO delete for release
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    if (isRunning) return; // Do NOT handle change of sound file if active timer
    contextMenu = new QMenu(this);

    contextMenu->addAction("&Choose new alarm sound Wizard", this,
                           SLOT(runSoundFilePickerWizard()));

    contextMenu->exec(event->globalPos());

    contextMenu->clear();
    delete contextMenu;
    contextMenu = nullptr;
}

void MainWindow::createActions()
{
    hideAction = new QAction(tr("&Hide"), this);
    connect(hideAction, &QAction::triggered,
            this, &MainWindow::hideMainWindow);

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &MainWindow::onRestore);

    stopAction = new QAction(tr("&Stop Alarm"), this);
    connect(stopAction, &QAction::triggered, this, &MainWindow::on_pbAction_clicked);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    hideAction->setDisabled(true);
    restoreAction->setDisabled(false);
    stopAction->setDisabled(true);
}

void MainWindow::createDelayTimer()
{
    delayTimer = new QTimer(this);
    connect(delayTimer,
            &QTimer::timeout,
            this,
            &MainWindow::slotDelayTimer,Qt::UniqueConnection);
}

void MainWindow::createPalette()
{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(54,57,63));

    palette.setColor(QPalette::WindowText, Qt::white);

    // this item has inconsistent behavior tween 5.12 and 5.14.  lesser of two
    // evils is to set this here and set stylesheet special for the tray
    palette.setColor(QPalette::Base, QColor(33,33,33));

    // the trays normal text color
    palette.setColor(QPalette::Text, Qt::white);

    // the trays disabled text color
    palette.setColor(QPalette::Disabled, QPalette::Text, QColor("#474747"));

    // mouse over of items in tray, and also highlighted text on mainwindow
    palette.setColor(QPalette::Highlight, QColor("#727272"));
    palette.setColor(QPalette::HighlightedText, QColor(33,33,33));

    QApplication::setPalette(palette);
}

void MainWindow::createPlayer()
{
    player = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
    playlist = new QMediaPlaylist();
    // get user's sound file on startup
    createPlaylist();
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    player->setPlaylist(playlist);
}

void MainWindow::createPlaylist()
{
    QString soundFile = getSetting("soundFileLocation");

    if (soundFile.isEmpty() || soundFile == "FACTORY")
    {
        soundFile = factorySoundFile;
    }
    playlist->addMedia(QUrl(soundFile));
}

void MainWindow::changePlaylist()
{
    playlist->clear();
    createPlaylist();
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(hideAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(stopAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    QIcon iconDef(":/images/stopwatch.png");
    setIcon(iconDef);
    connect(trayIcon, &QSystemTrayIcon::activated,
            this, &MainWindow::iconActivated);
    connect(trayIcon, &QSystemTrayIcon::messageClicked,
            this, &MainWindow::messageClicked);
    trayIcon->setToolTip("Not running a timer");

    // Tray theming is diff in 5.12 & 5.14 ; we'll set the tray manualy
    // so it will behave the same in both versions
    auto css =
                /* this is menu background color */
                "QMenu {"
                    "color: white;"
                    "background-color: #36393F;}"

                /* this is mouse over colors for the context menu */
                "QMenu::item:selected {"
                    "background-color: #212121;}"

                /* this is colors when an item is disabled */
                "QMenu::item::disabled{"
                    "color: #858A96;"
                    "background-color: #36393F;}";

    trayIconMenu->setStyleSheet(css);
    trayIcon->show();
    trayIcon->showMessage("Webcams", "Running in your tray", iconDef, 2000);
}

void MainWindow::createWizard()
{
    w = new Wizard(configFile, publisher, appName);
    connect(w, &Wizard::soundFileChanged,
            this, &MainWindow::changePlaylist);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if ((watched == ui->txtEdMsg) && (event->type() == QEvent::KeyPress))
    {
        auto key = static_cast<QKeyEvent *>(event)->key();

        if ((key == Qt::Key_Return) || (key == Qt::Key_Enter))
        {
            on_pbAction_clicked(); // Run the Timer on enter key Press!
            event->accept();
            return true;
        }
    }

    return QMainWindow::eventFilter(watched, event);
}

QString MainWindow::getSetting(const QString &someSetting) const
{
    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       publisher,
                       appName);

    return settings.value(someSetting).toString();
}

void MainWindow::hideMainWindow()
{
    updateMainwindowMemberVars();

    hideAction->setDisabled(true);
    restoreAction->setDisabled(false);
    hide();
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        if (this->isHidden())
        {
            onRestore();
            return;
        }
        else
        {
            hideMainWindow();
            return;
        }
    case QSystemTrayIcon::DoubleClick:
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        ;
    }
}

MainWindow::~MainWindow()
{
    delete w;
    delete delayTimer;
    delete playlist;
    delete player;
    delete ui;
    delete hideAction;
    delete restoreAction;
    delete stopAction;
    delete quitAction;
    delete trayIconMenu;
    delete trayIcon;
}

// NOTE: MS Windows requires that if the pop up is activated, and a user
// interacts with the system tray icon, this function must be called.
// So, if the pop up activates, and user clicks on tray icon this will fire
// no matter what.  If this is an issue, you will neeed to override the
// behavior, or perhaps do some validation with an internal variable.
void MainWindow::messageClicked()
{
    if ((!delayTimer->isActive()) && !isRunning)
    {
        // on startup, user clicks notification pop up message
        showAndSetActive();
    }
    else
    {
        // users clicks notification pop up message that alarm has expired
        on_pbAction_clicked();
        onRestore();
    }
}

void MainWindow::on_lnEd_returnPressed()
{
    ui->txtEdMsg->setFocus();
}

void MainWindow::on_pbAction_clicked()
{
    if ((!delayTimer->isActive()) && !isRunning) // start
    {
        isRunning=true;
        const int val = ui->lnEd->text().toInt();
        if (val==0) return;
        ui->lnEd->setReadOnly(true);
        const int delay = val * 600; // changd for DEV mode TODO restore at rel time
//        const int delay = val * 60000; // TODO restore me for rel
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

        setFocus();
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

        setFocus();
    }
}

void MainWindow::onRestore()
{
    // hard coding pts is no errors, eg. move(100,100); Qt please fix!

    /* We're doing this hack to accomdate box window managers on GNU,
     * However the titlebar will be off screen in top left of monitor
     * on startup using the tray menu restore action.  But this hack
     * will allow them to single click on the tray and never know
     * the better.  If icem or other box users want right click restore
     * from tray on initial startup then toggle the lines below.
     */

    // hack for GNU not restoring variable Qpoints!
    resize(mainwindowWidth + 1, mainwindowHeight);

    // BUG: comment this out for icewm or fluxbox; don't forget to
    // uncomment the line below as well.
    move(mainwindowScreenCoordinates);

    showAndSetActive();

    resize(mainwindowWidth, mainwindowHeight);

    // BUG: uncomment this for icewm or fluxbox and comment out above
    // move(mainwindowScreenCoordinates);
}

void MainWindow::runSoundFilePickerWizard()
{
    w->show();
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
    QMainWindow::show();
    QMainWindow::raise();
    QMainWindow::activateWindow();
    hideAction->setDisabled(false);
    restoreAction->setDisabled(true);
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

void MainWindow::tweakUi()
{
    ui->lblStatus->setText("Habouji!");
    setButtonHoverColor(green);

    auto icon = ":/images/stopwatch.png";
    setWindowIcon(QIcon(icon));

    // we'll allow a timer for a full week's worth of minutes
    ui->lnEd->setValidator(new QIntValidator(1,10080, this));

    // text edits do not have a return key press event, so we'll make our own:
    ui->txtEdMsg->installEventFilter(this);

    auto placeHolderText = "Type your message here.  "
                           "Hit enter to start the timer.";
    ui->txtEdMsg->setPlaceholderText(placeHolderText);
}

void MainWindow::tweakWindowFlags()
{
    // must allow for resize in W for our hack for restore window position
    setMinimumWidth(177);
    setMaximumWidth(178);

    setMinimumHeight(280);
    setMaximumHeight(280);

    Qt::WindowFlags windowFlags = (Qt::Widget | Qt::CustomizeWindowHint);
    setWindowFlags(windowFlags |= Qt::WindowCloseButtonHint);
}

void MainWindow::updateMainwindowMemberVars()
{
    mainwindowHeight = height();
    mainwindowWidth = width();
    mainwindowScreenCoordinates = parentWidget()->mapFromGlobal(pos());
}


