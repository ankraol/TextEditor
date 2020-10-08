#include "mainwindow.h"
#include "searchwindow.h"
#include "CodeEditor.h"

#include <ui_mainwindow.h>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_searchWindow = new SearchWindow(this);
    m_searchWindow->hide();

    m_codeEditor = new CodeEditor(this);
    m_ui->setupUi(this);
    m_ui->verticalLayout->addWidget(m_codeEditor);

    setupImages();

    // MOVE SPLITTER TO LEFT
    QList<int> currentSizes = m_ui->splitter->sizes();
    currentSizes[0] = 0;
    currentSizes[1] = 1;
    m_ui->splitter->setSizes(currentSizes);

    connect(m_codeEditor, &CodeEditor::updateRequest, this, &MainWindow::setLinesText);
    connect(m_ui->undoBtn, &QPushButton::clicked, m_codeEditor, &CodeEditor::undo);
    connect(m_ui->redoBtn, &QPushButton::clicked, m_codeEditor, &CodeEditor::redo);
    connect(m_ui->copyBtn, &QPushButton::clicked, m_codeEditor, &CodeEditor::copy);
    connect(m_ui->cutBtn, &QPushButton::clicked, m_codeEditor, &CodeEditor::cut);
    connect(m_ui->pasteBtn, &QPushButton::clicked, m_codeEditor, &CodeEditor::paste);
    connect(m_ui->findBtn, &QPushButton::clicked, this, &MainWindow::findBtn);
}

void MainWindow::setLinesText() {
    int x = m_codeEditor->textCursor().positionInBlock() + 1;

    m_ui->l_linesCount->setText(
        QString("Col ") + std::to_string(x).c_str() + /*":" + std::to_string(pos.ry()).c_str() +*/
        " | " + std::to_string(m_codeEditor->blockCount()).c_str() + " lines"
    );
}

void MainWindow::findBtn() {
    QString s = m_codeEditor->textCursor().selectedText();
    m_searchWindow->setupStartParams(m_codeEditor, s);
    m_searchWindow->show();
}

MainWindow::~MainWindow() {
    delete m_ui;
}

void MainWindow::setupImages() {
    m_icSize.setWidth(36);
    m_icSize.setHeight(36);

    m_ui->undoBtn->setIcon(m_undoIcon);
    m_ui->undoBtn->setIconSize(m_icSize);

    m_ui->redoBtn->setIcon(m_redoIcon);
    m_ui->redoBtn->setIconSize(m_icSize);

    m_ui->copyBtn->setIcon(m_copyIcon);
    m_ui->copyBtn->setIconSize(m_icSize);

    m_ui->cutBtn->setIcon(m_cutIcon);
    m_ui->cutBtn->setIconSize(m_icSize);

    m_ui->pasteBtn->setIcon(m_pasteIcon);
    m_ui->pasteBtn->setIconSize(m_icSize);

    m_ui->findBtn->setIcon(m_findIcon);
    m_ui->findBtn->setIconSize(m_icSize);

    m_ui->optionsBtn->setIcon(m_optionsIcon);
    m_ui->optionsBtn->setIconSize(m_icSize);
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
    m_ui->treeView->setModel(dirmodel);
    m_ui->treeView->setRootIndex(dirmodel->setRootPath(info.dir().absolutePath()));
    for (int i = 1; i < dirmodel->columnCount(); ++i)
        m_ui->treeView->hideColumn(i);
}

void MainWindow::on_actionDirectory_triggered() {
    QString dirname = QFileDialog::getExistingDirectory(this, "", "/", QFileDialog::ShowDirsOnly
                                                            | QFileDialog::DontResolveSymlinks);
    //check directory name
    qDebug() << dirname;
    dirmodel = new QFileSystemModel(this);
    m_ui->treeView->setModel(dirmodel);
    m_ui->treeView->setRootIndex(dirmodel->setRootPath(dirname));
    for (int i = 1; i < dirmodel->columnCount(); ++i)
        m_ui->treeView->hideColumn(i);
    setLinesText();
}
