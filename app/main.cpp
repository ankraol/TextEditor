#include <QApplication>
#include <QDebug>
#include "src/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mW;
    mW.show();
    qDebug() << "Hello World";
    return app.exec();
}
