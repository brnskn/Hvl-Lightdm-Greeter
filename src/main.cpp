#include <QtWidgets/QApplication>
#include <QDesktopWidget>
#include <QtGlobal>
#include <QtDebug>
#include <QSettings>
#include <QIcon>
#include <QTranslator>
#include <QString>
#include <QLocale>
#include <QRegularExpression>

#include <iostream>

#include "settings.h"

#include "mainwindow.h"

QFile logfile;
QTextStream ts;

void messageHandler(QtMsgType type, const QMessageLogContext&, const QString& msg)
{
    std::cerr << type << ": " << QTime::currentTime().toString().toLatin1().data() << " " << msg.toLatin1().data() << "\n";
}

int main(int argc, char *argv[])
{
   // use std::cerr for logs
    qInstallMessageHandler(messageHandler);
    Cache::prepare();

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);


    qDebug() << "hvl-lightdm is starting now";


    QString locale = QLocale::system().name();

    QTranslator translator;

    if(!translator.load(locale, "/usr/share/lightdm/lightdm-hvl-greeter.conf.d/lang")){

        if(!translator.load(locale.section('_', 0, 0) , "/usr/share/lightdm/lightdm-hvl-greeter.conf.d/lang")){
            qInfo() << "Failed to load translator file for locale  " + locale;
        }
    }
    a.installTranslator(&translator);

    if (! Settings().iconThemeName_loginform().isEmpty()) {
        QIcon::setThemeName(Settings().iconThemeName_loginform());
    }

    MainWindow *focusWindow = 0;
    for (int i = 0; i < QApplication::desktop()->screenCount(); ++i) {
        MainWindow *w = new MainWindow(i);
        w->show();
        if (w->showLoginForm())
            focusWindow = w;
    }

    // Ensure we set the primary screen's widget as active when there
    // are more screens
    if (focusWindow) {
        focusWindow->setFocus(Qt::OtherFocusReason);
        focusWindow->activateWindow();
    }

    return a.exec();
}