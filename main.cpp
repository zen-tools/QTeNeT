#include <QtGui/QApplication>
#include <QTranslator>
#include "mainwindow.h"

int main (int argc, char *argv[])
{

    QApplication a (argc, argv);

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name());
    a.installTranslator(&qtTranslator);

    QTranslator translator;
    //translator.load(myapp_de_qm_data, myapp_de_qm_len);
    translator.load("qtenet_" + QLocale::system().name());
    a.installTranslator(&translator);

    MainWindow w;

    return a.exec ();
}
