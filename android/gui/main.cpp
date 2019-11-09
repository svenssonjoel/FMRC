#include "mainwindow.h"
#include "version.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_SynthesizeTouchForUnhandledMouseEvents);
    QCoreApplication::setAttribute(Qt::AA_SynthesizeMouseForUnhandledTouchEvents);
    QCoreApplication::setOrganizationName("BJS");
    QCoreApplication::setOrganizationDomain("svenssonjoel.github.io");
    QCoreApplication::setApplicationName("FMRC GUI");
    QCoreApplication::setApplicationVersion(QString("Version %1.%2").arg(VERSION_MAJOR).arg(VERSION_MINOR));

    QApplication a(argc, argv);   
    a.setApplicationDisplayName("FMRC-GUI");


    MainWindow w;
    w.show();

    return a.exec();
}
