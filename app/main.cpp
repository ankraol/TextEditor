#include <QApplication>
#include "src/mainwindow.h"
#include "src/ide_mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    IDE_MainWindow mW;

    mW.show();
    return app.exec();
}
