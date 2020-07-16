#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>
#include <QDirIterator>
#include <QEvent>
#include <QIntValidator>
#include <QMediaPlaylist>
#include <QMenu>
#include <QMessageBox>
#include <QScreen>
#include <QSettings>
#include <QTime>
#include <QUrl>

#include <QDebug>

#ifdef WIN32
#include "Windows.h"
#include "WinUser.h"
#include "wingdi.h"
#endif

MainWindow::MainWindow(
    const QString &configFile, const QString &publisher, const QString &appName,
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
//    createPalette();
    createActions();
    createTrayIcon();
    createDelayTimer();
    createPlayer();
    setUserCssStyleSheet();
    createThemePicker();
    createSoundFilePicker();
    ui->setupUi(this);
    tweakUi();
    tweakWindowFlags();

    // linux box window managers do not center on open. sometimes title bar
    // is off screen so brute force this.
    int screenWidth = QGuiApplication::screens().first()->geometry().width();
    int screenHeight = QGuiApplication::screens().first()->geometry().height();
    setGeometry(
        (screenWidth/2)-(width()/2), (screenHeight/2)-(height()/2),
            width(), height());
    // despite the window not shown on startup, calling this sets more vars
    // needed to center the window
    hideMainWindow();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    updateMainwindowMemberVars();

    hideAction->setDisabled(true);
    restoreAction->setDisabled(false);

    QMainWindow::closeEvent(event);
}

void MainWindow::createActions()
{
    aboutAction = new QAction("&About Timer", this);
    connect(aboutAction, &QAction::triggered,
            this, &MainWindow::onAboutClicked);

    hideAction = new QAction("&Hide", this);
    connect(hideAction, &QAction::triggered,
            this, &MainWindow::hideMainWindow);

    quitAction = new QAction("&Quit", this);
    connect(quitAction, &QAction::triggered,
            qApp, &QCoreApplication::quit);

    restoreAction = new QAction("&Restore", this);
    connect(restoreAction, &QAction::triggered, this,
            &MainWindow::onRestore);

    stopAction = new QAction("&Stop Alarm", this);
    connect(stopAction, &QAction::triggered,
            this, &MainWindow::on_pbAction_clicked);

    soundAction = new QAction("Sound &File Picker", this);
    connect(soundAction, &QAction::triggered,
            this, &MainWindow::onSoundFilePickerClicked);

    themeAction = new QAction("&Theme Picker", this);
    connect(themeAction, &QAction::triggered,
            this, &MainWindow::onThemePickerClicked);

    hideAction->setDisabled(true);
    restoreAction->setDisabled(false);
    stopAction->setDisabled(true);
}

void MainWindow::createDelayTimer()
{
    delayTimer = new QTimer(this);
    connect(delayTimer, &QTimer::timeout,
            this, &MainWindow::slotDelayTimer,Qt::UniqueConnection);
}

void MainWindow::createPalette()
{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(54,57,63));

    palette.setColor(QPalette::WindowText, Qt::white);

    // this item has inconsistent behavior tween 5.12 and 5.14.  lesser of two
    // evils is to set this here and set stylesheet special for the tray
    // this is the hover color of items in the menu
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
    createPlaylist();
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    player->setPlaylist(playlist);
}

void MainWindow::createPlaylist()
{
    auto soundFile = getSetting("soundFileLocation");

    if (soundFile.isEmpty() || soundFile == "FACTORY")
    {
        soundFile = factorySoundFile;
        playlist->addMedia(QUrl(soundFile));
        return;
    }

    playlist->addMedia(QMediaContent(QUrl::fromLocalFile(soundFile)));
}

void MainWindow::changePlaylist()
{
    playlist->clear();
    createPlaylist();
}

void MainWindow::createSoundFilePicker()
{
    soundPicker = new SoundPicker(configFile, publisher, appName);

    connect(soundPicker, &SoundPicker::soundFileChanged,
            this, &MainWindow::changePlaylist);
}

void MainWindow::createThemePicker()
{
    themePicker = new ThemePicker();

    connect(this, &MainWindow::sendThemesList,
            themePicker, &ThemePicker::onSendThemesList);

    connect(themePicker, &ThemePicker::getThemesList,
            this, &MainWindow::onGetThemesList);

    connect(themePicker, &ThemePicker::setCssStyleStyleSheet,
            this, &MainWindow::setCssStyleSheet);

    connect(themePicker, &ThemePicker::getCurrentTheme,
            this, &MainWindow::onGetCurrentTheme);

    connect(this, &MainWindow::currentTheme,
            themePicker, &ThemePicker::onCurrentTheme);
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(aboutAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(themeAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(soundAction);
    trayIconMenu->addSeparator();
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

    trayIcon->show();
    trayIcon->showMessage("Timer", "Running in your tray", iconDef, 2000);
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
    QSettings settings(
        QSettings::IniFormat, QSettings::UserScope, publisher, appName);

    return settings.value(someSetting).toString();
}

#ifdef WIN32
QString MainWindow::getWindowsFontFamily()
{
    NONCLIENTMETRICS metrics;
    metrics.cbSize = sizeof(NONCLIENTMETRICS);
    SystemParametersInfo(
        SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &metrics, 0);
    auto mystring = &metrics.lfMessageFont.lfFaceName;

    return QString::fromStdWString(*mystring);
}
#endif

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
    delete ui;
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

void MainWindow::onAboutClicked()
{
    auto msg = "Timer License: GPL V3.0\n"
               "https://github.com/serverlinkdev/timer\n"
               "Using Qt framework from:\n"
               "https://www.qt.io/";

    auto *msgAbout = new QMessageBox(this);
    msgAbout->setWindowTitle("About Timer");
    msgAbout->setIcon(QMessageBox::Information);
    msgAbout->setText(msg);
    msgAbout->setStandardButtons(QMessageBox::Ok);
    msgAbout->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    msgAbout->exec();

    delete msgAbout;
    msgAbout = nullptr;
}

void MainWindow::onGetCurrentTheme()
{
    auto theme = getSetting("theme");
    emit currentTheme(theme);
}

void MainWindow::onGetThemesList()
{
    QDirIterator it(":/qss/", QDirIterator::Subdirectories);
    QStringList themesList;
    while (it.hasNext())
    {
        QFileInfo fileInfo(it.next());
        themesList << fileInfo.baseName();
    }

    emit sendThemesList(themesList);
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
        const auto val = ui->lnEd->text().toInt();
        if (val==0) return;
        ui->lnEd->setReadOnly(true);
        const auto delay = val * 60000;
        delayTimer->setSingleShot(true);
        delayTimer->start(delay);

        const auto now = QTime::currentTime();
        const auto nowShort = now.toString("hh:mm");
        const auto later = now.addSecs(val*60);
        const auto laterShort=later.toString("hh:mm");
        ui->lblStatus->setText("Timer ends ~ " + laterShort + " ");
        setWindowTitle(laterShort + " Timer ends");

        ui->txtEdMsg->setReadOnly(true);
        auto userMessage = ui->txtEdMsg->toPlainText();
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

        const auto msg = "Not running a timer";
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

void MainWindow::onSoundFilePickerClicked()
{
    soundPicker->show();
}

void MainWindow::onThemePickerClicked()
{
    themePicker->show();
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

void MainWindow::setCssStyleSheet(const QString &themeName)
{
    QFile file(":/qss/" + themeName + ".qss");
    file.open(QFile::ReadOnly);
    auto sheet = QString::fromLatin1(file.readAll());

    // On GNU/Linux Qt's font detection *AND* inheritance is broken
    // let's take a hammer to it (Qt 5.14 and up is fine, btw).
    // TODO in 2025 do we still need this? Has GNU ecosystem progressed
    // that we don't need this?
    auto font = property("font").value<QFont>();
    auto size = font.pointSize();
    auto fontSize = QString::number(size,10);
    auto unit = "pt";
    auto fontFamily = font.family();
#ifdef WIN32
    // windows uses really small fonts by default
    fontSize = "18";
    unit = "px"; // px for Windows fonts not jagged this way
    // Win10 users who edit their registry to get custom fonts the old
    // pre-Win10 way which MS hides now but works with 'advchange' app
    fontFamily = getWindowsFontFamily();
#endif

    auto prepend = "* {font-family: " + fontFamily + ";"
                   "font-size: " + fontSize + unit + ";}";

    auto cssStyleSheet = prepend + sheet;

    qApp->setStyleSheet(cssStyleSheet);

    writeSettings("theme", themeName);
}

void MainWindow::setIcon(QIcon icon)
{
    trayIcon->setIcon(icon);
    setWindowIcon(icon);
}

void MainWindow::setUserCssStyleSheet()
{
    auto theme = getSetting("theme");

    // first run of app, or user rm's config file, or delete's line in config
    if (theme.isEmpty())
    {
        theme = "Dark";
        setCssStyleSheet("Dark");
    }
    else
    {
        setCssStyleSheet(theme);
    }
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
    const auto msg = "Timer Expired!";
    ui->lblStatus->setText(msg);
    setWindowTitle(msg);
    trayIcon->setToolTip(msg);

    player->play();

    QIcon iconExpired(":/images/stopwatch-expired.png");
    setIcon(iconExpired);

    auto userMessage = ui->txtEdMsg->toPlainText();
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
    ui->lnEd->setValidator(new QIntValidator(1, 10080, this));

    // text edits do not have a return key press event, so we'll make our own:
    ui->txtEdMsg->installEventFilter(this);

    auto placeHolderText = "Type message and hit enter to start.";
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

void MainWindow::writeSettings(const QString &key, const QString &value)
{
    QSettings settings(
        QSettings::IniFormat, QSettings::UserScope, publisher, appName);

    return settings.setValue(key, value);
}



