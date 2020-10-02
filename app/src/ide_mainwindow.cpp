#include "ide_mainwindow.h"
#include "ui_ide_mainwindow.h"

IDE_MainWindow::IDE_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IDE_MainWindow)
{
    ui->setupUi(this);
}

IDE_MainWindow::~IDE_MainWindow()
{
    delete ui;
}
