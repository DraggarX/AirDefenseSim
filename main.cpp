#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow w;
    w.show();          // ОЧЕНЬ ВАЖНО: без этого окно не появится

    return app.exec();
}
