#include <QApplication>
#include "src/mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow mW;
    mW.show();
    return app.exec();
}
