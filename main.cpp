#include "genScene.h"

#include <QApplication>
#include <QtGui>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    genScene mainWin;

    mainWin.show();

    return app.exec();
}
