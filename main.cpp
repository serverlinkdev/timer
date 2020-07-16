#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QSettings>
#include <QStyleFactory>
#include <QSystemTrayIcon>

bool fileExists(const QString &someFile);

void createConfigFile(QSettings &settings,
                      const QString &publisher,
                      const QString &appName);

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

// This breaks GNU Linux users ability to theme the application at all
#ifdef WIN32
    QApplication::setStyle(QStyleFactory::create("Fusion"));
#endif

    QApplication a(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        QMessageBox::critical(nullptr,
                              "Systray",
                              "System tray not available on this system.");
        return 1;
    }

    const auto publisher = "pomware";
    const auto appName = "timer";

    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       publisher,
                       appName);

    const auto configFile = QDir::cleanPath(settings.fileName());

    if (!fileExists(configFile))
    {
        createConfigFile(settings, publisher, appName);
    }

    QApplication::setQuitOnLastWindowClosed(false);
    MainWindow w(configFile, publisher, appName);
    // no call to w.show() as we minimize to tray on start.
    return a.exec();
}

bool fileExists(const QString &someFile)
{
    QFileInfo fileInfo(someFile);
    return (fileInfo.exists() && fileInfo.isFile());
}

void createConfigFile(QSettings &settings,
                      const QString &publisher,
                      const QString &appName)
{
    settings.setValue("publisher", publisher);
    settings.setValue("appName", appName);
}
