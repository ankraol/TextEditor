#include "mainwindow.h"
#include "CodeEditor.h"

#include <ui_mainwindow.h>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_codeEditor = new CodeEditor(this);
    ui->setupUi(this);
    ui->verticalLayout->addWidget(m_codeEditor);

    setupImages();

    // MOVE SPLITTER TO LEFT
    QList<int> currentSizes = ui->splitter->sizes();
    currentSizes[0] = 0;
    currentSizes[1] = 1;
    ui->splitter->setSizes(currentSizes);

    connect(m_codeEditor, &CodeEditor::updateRequest, this, &MainWindow::setLinesText);
    connect(ui->undoBtn, &QPushButton::clicked, this, &MainWindow::undoBtnFunc);
    connect(ui->redoBtn, &QPushButton::clicked, this, &MainWindow::redoBtnFunc);
}

void MainWindow::setLinesText() {
    int x = m_codeEditor->textCursor().positionInBlock() + 1;

    ui->l_linesCount->setText(
        QString("Col ") + std::to_string(x).c_str() + /*":" + std::to_string(pos.ry()).c_str() +*/
        " | " + std::to_string(m_codeEditor->blockCount()).c_str() + " lines"
    );
}

void MainWindow::undoBtnFunc() {
    m_codeEditor->undo();
}

void MainWindow::redoBtnFunc() {
    m_codeEditor->redo();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupImages() {
    m_icSize.setWidth(36);
    m_icSize.setHeight(36);

    ui->undoBtn->setIcon(m_undoIcon);
    ui->undoBtn->setIconSize(m_icSize);

    ui->redoBtn->setIcon(m_redoIcon);
    ui->redoBtn->setIconSize(m_icSize);

    ui->copyBtn->setIcon(m_copyIcon);
    ui->copyBtn->setIconSize(m_icSize);

    ui->cutBtn->setIcon(m_cutIcon);
    ui->cutBtn->setIconSize(m_icSize);

    ui->pasteBtn->setIcon(m_pasteIcon);
    ui->pasteBtn->setIconSize(m_icSize);

    ui->findBtn->setIcon(m_findIcon);
    ui->findBtn->setIconSize(m_icSize);

    ui->optionsBtn->setIcon(m_optionsIcon);
    ui->optionsBtn->setIconSize(m_icSize);
}

void MainWindow::setFileSystem(const QString* sPath) {
    qDebug() << "the file was chosen";
}

void MainWindow::on_actionFile_triggered() {
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

void MainWindow::on_actionDirectory_triggered() {
    QString dirname = QFileDialog::getExistingDirectory(this, "", "/", QFileDialog::ShowDirsOnly
                                                            | QFileDialog::DontResolveSymlinks);
    //check directory name
    qDebug() << dirname;
    dirmodel = new QFileSystemModel(this);
    ui->treeView->setModel(dirmodel);
    ui->treeView->setRootIndex(dirmodel->setRootPath(dirname));
    for (int i = 1; i < dirmodel->columnCount(); ++i)
        ui->treeView->hideColumn(i);
    setLinesText();
}
