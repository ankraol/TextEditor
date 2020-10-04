#include "mainwindow.h"
#include <ui_mainwindow.h>

#include "CodeEditor.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_codeEditor = new CodeEditor(this);
    ui->setupUi(this);
    ui->verticalLayout->addWidget(m_codeEditor);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setFileSystem(const QString* sPath) {
    qDebug() << "the file was chosen";
}

void MainWindow::on_actionFile_triggered()
{
        QString filename = QFileDialog::getOpenFileName(this, "Open a File", "/");
        QFileInfo info(filename);
        //check name of file
        qDebug() << filename;
        qDebug() << info.dir().absolutePath();
        qDebug() << info.dir();
        dirmodel = new QFileSystemModel(this);
        ui->treeView->setModel(dirmodel);
        ui->treeView->setRootIndex(dirmodel->setRootPath(info.dir().absolutePath()));
        for (int i = 1; i < dirmodel->columnCount(); ++i)
            ui->treeView->hideColumn(i);
}

void MainWindow::on_actionDirectory_triggered()
{
       QString dirname = QFileDialog::getExistingDirectory(this, "", "/", QFileDialog::ShowDirsOnly
                                                                | QFileDialog::DontResolveSymlinks);
        //check directory name
        qDebug() << dirname;
        dirmodel = new QFileSystemModel(this);
        ui->treeView->setModel(dirmodel);
        ui->treeView->setRootIndex(dirmodel->setRootPath(dirname));
        for (int i = 1; i < dirmodel->columnCount(); ++i)
            ui->treeView->hideColumn(i);
}
