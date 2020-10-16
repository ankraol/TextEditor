#include "mainwindow.h"
#include <ui_mainwindow.h>

#include "tabWidget.h"
#include "ClickableLabel.h"
#include "ProjectsView.h"
#include "searchwindow.h"
#include "CodeEditor.h"
#include "ErrorMessageBox.h"


MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    m_searchWindow = new SearchWindow(this);
    m_searchWindow->hide();
    setupImages();

    //creating starting window
    m_widget = new QWidget;
    m_layout = new QVBoxLayout;
    m_hintButton_CreateFile = new QPushButton ("Create New File  |  ⌘N");
    m_hintButton_OpenFile = new QPushButton ("Open File  |  ⌘O");

    //set up buttons
    m_hintButton_CreateFile->setFlat(true);
    m_hintButton_OpenFile->setFlat(true);

    this->setStyleSheet("QPushButton { color: black; font : 14pt 'PT Mono'; font-weight : bold;}\n"
                        "QPushButton:hover{ background: transparent; color: blue; font : 14pt 'PT Mono'; font-weight : bold;}\n"
                        "QPushButton:pressed { background: transparent; color: blue; font : 14pt 'PT Mono'; font-weight : bold;}");


    //adding buttons
    m_layout->addWidget(m_hintButton_OpenFile, QSizePolicy::Maximum);
    m_layout->addWidget(m_hintButton_CreateFile, QSizePolicy::Maximum);
    m_layout->setAlignment(m_hintButton_OpenFile, Qt::AlignBottom);
    m_layout->setAlignment(m_hintButton_CreateFile, Qt::AlignTop);
    m_widget->setLayout(m_layout);
    m_ui->splitter->addWidget(m_widget);

    //setting up connections
    m_hintButton_OpenFile->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_O));
    m_hintButton_CreateFile->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_N));
    connect(m_hintButton_OpenFile, &QPushButton::clicked, this, &MainWindow::on_actionFile_triggered);
    connect(m_ui->actionCreateFile_Project, &QAction::triggered, this, &MainWindow::createNewFile);
    connect(m_hintButton_CreateFile, &QPushButton::clicked, this, &MainWindow::createNewFile);
    connect(m_ui->findBtn, &QPushButton::clicked, this, &MainWindow::findBtn);

}

bool MainWindow::fileIsOpened(QString newFile) {
    for (tabWidget* tab : m_codeEditors_Vector) {
        if (m_codeEditors_Tabs->tabText(m_codeEditors_Tabs->indexOf(tab)) == newFile) {
            m_codeEditors_Tabs->setCurrentWidget(tab);
            return true;
        }
    }
    return false;
}

void MainWindow::saveChanges() {
    QString file =  m_codeEditors_Tabs->tabText(m_codeEditors_Tabs->currentIndex());
    QFile outputFile(file);
    outputFile.open(QIODevice::WriteOnly);
    QString text = (*m_codeEditors_Tabs->currentWidget()->findChildren<CodeEditor *>().begin())->getText();

    if (outputFile.isOpen()) {
        QTextStream outStream(&outputFile);
        outStream << text;
        outputFile.close();
    } else {
        ErrorMessageBox(this, "An error occurred:", "Changes can't be saved.");
    }
}

/* creates QDialog to select location and name of a new file
 * and creates it)) */
void MainWindow::createNewFile() {
    m_newFile_Dialog = new QFileDialog;
    m_newFile_Dialog->setModal(true);
    m_newFile_Dialog->setFileMode(QFileDialog::AnyFile);
    QString fileName = m_newFile_Dialog->getSaveFileName(NULL, "Create New File","Create New File","Create New File");
    openFile(fileName);
}

/* adds new Tab to the QTabWidget every time new
 * project/file is opened */
tabWidget* MainWindow::addNewTab() {
    if (m_codeEditors_Tabs == nullptr) {
        m_codeEditors_Tabs = new QTabWidget(this);
        m_codeEditors_Tabs->setTabsClosable(true);
        m_widget->close(); //if there is delete on close flag it will eventually get deleted;
        m_ui->splitter->addWidget(m_codeEditors_Tabs);
        m_codeEditors_Tabs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        //connections of m_codeEditors_Tabs
        connect(m_codeEditors_Tabs, &QTabWidget::tabCloseRequested, this, &MainWindow::closeRequestedTab); //removes tab, when exit button pressed
        connect(m_ui->actionSave_File, &QAction::triggered, this, &MainWindow::saveChanges); //saves changes on current tab;
    }
    m_codeEditors_Vector.push_back((new tabWidget()));
    m_codeEditors_Tabs->addTab(m_codeEditors_Vector.back(), filePath);

    //here connection of tab widgets should be written
    connect(m_codeEditors_Vector.back()->getEditor(), &CodeEditor::updateRequest, this, &MainWindow::setLinesText); //connects line counter label to Text editor
    connect(m_ui->undoBtn, &QPushButton::clicked, m_codeEditors_Vector.back()->getEditor(), &CodeEditor::undo);
    connect(m_ui->redoBtn, &QPushButton::clicked, m_codeEditors_Vector.back()->getEditor(), &CodeEditor::redo);
    connect(m_ui->copyBtn, &QPushButton::clicked, m_codeEditors_Vector.back()->getEditor(), &CodeEditor::copy);
    connect(m_ui->cutBtn, &QPushButton::clicked, m_codeEditors_Vector.back()->getEditor(), &CodeEditor::cut);
    connect(m_ui->pasteBtn, &QPushButton::clicked, m_codeEditors_Vector.back()->getEditor(), &CodeEditor::paste);
    show();
    return m_codeEditors_Vector.back();
}

/* closes requested tab */
void MainWindow::closeRequestedTab(int index) {
    m_codeEditors_Tabs->currentWidget()->disconnect();
    m_codeEditors_Tabs->removeTab(index);
    m_ui->l_linesCount->setText("Lines: 0");
}

/* reads from filepath and sets up the text of file
 * to the new tab code editor */
void MainWindow::on_actionFileOpened() {
    addNewTab();
    std::ifstream fileStream;
    fileStream.open(filePath.toUtf8().constData(), std::ios::in|std::ios::binary|std::ios::ate);
    char* memblock;
    std::streampos size;

    if(fileStream.is_open()) {
        size = fileStream.tellg();
        memblock = new char [size];
        fileStream.seekg (0, std::ios::beg);
        fileStream.read (memblock, size);
        fileStream.close();
        m_codeEditors_Vector.back()->setText(QString::fromUtf8(memblock));
        delete [] memblock;
    } else {
        ErrorMessageBox(this, "An error occurred:", "Failed to open a file.");
    }
}


/* just to test connection */
void MainWindow::TestPrint() {
    qDebug() << "Clicked";
}


/* sets up lines count of file to lines label */
void MainWindow::setLinesText() {
    CodeEditor* current = (*m_codeEditors_Tabs->currentWidget()->findChildren<CodeEditor*>().begin());
     int x = current->textCursor().positionInBlock() + 1;

    m_ui->l_linesCount->setText(
        QString("Col ") + std::to_string(x).c_str() + /*":" + std::to_string(pos.ry()).c_str() +*/
        " | " + std::to_string(current->blockCount()).c_str() + " lines"
    );
}

void MainWindow::findBtn() {
    if (m_codeEditors_Tabs != nullptr) {
        CodeEditor* current = (*m_codeEditors_Tabs->currentWidget()->findChildren<CodeEditor*>().begin());

        QString s = current->textCursor().selectedText();
        m_searchWindow->setupStartParams(current, s);
        m_searchWindow->show();
    }
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

//opens double-clicked file
bool MainWindow::doubleClick_onTreeView(QString path) {
    QFileInfo info(path);

    if (info.isFile() && !fileIsOpened(path)) {
        filePath = path;
        on_actionFileOpened();
    }
    return true;
}

void MainWindow::closeRequestedProject(int index) {
    if (m_workTree_tabWidget->count() > 1) {
        m_workTree_tabWidget->currentWidget()->disconnect();
        m_workTree_tabWidget->removeTab(index);
    }
}

bool MainWindow::dirIsOpened(QString dirName) {
    for (ProjectsView* tab : m_projectsVector) {
        if (m_workTree_tabWidget->tabText(m_workTree_tabWidget->indexOf(tab)) == dirName)
            return true;
    }
    return false;
}

/* calls on_actionFileOpened func to fill in the Text Editor field */
void MainWindow::openFile(QString fileName) {

    if (fileName != "" && !fileIsOpened(fileName)) {
        QFileInfo info(fileName);
        filePath = fileName;
        dirmodel = new QFileSystemModel(this);

        if (m_workTree_tabWidget == nullptr) {
            m_workTree_tabWidget = new QTabWidget;
            m_workTree_tabWidget->setTabsClosable(true);
            m_workTree_tabWidget->setTabPosition(QTabWidget::West);
            m_ui->splitter->addWidget(m_workTree_tabWidget);
            connect(m_workTree_tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeRequestedProject);
        }
        if (!dirIsOpened(info.dir().dirName())) {
            m_projectsVector.push_back(new ProjectsView(this, dirmodel, info.dir().absolutePath()));
            m_workTree_tabWidget->addTab(m_projectsVector.back(), info.dir().dirName());
            connect(m_projectsVector.back(), &ProjectsView::file_chosen, this, &MainWindow::doubleClick_onTreeView);
            connect(m_projectsVector.back(), &ProjectsView::newFile_triggered, this, &MainWindow::on_newFile);
            connect(m_projectsVector.back(), &ProjectsView::newDir_triggered, this, &MainWindow::on_newDir);
            connect(m_projectsVector.back(), &ProjectsView::rename_triggered, this, &MainWindow::on_rename);
            connect(m_projectsVector.back(), &ProjectsView::delete_triggered, this, &MainWindow::on_delete);
        }
        on_actionFileOpened();
    }
}

/* creates QFileDialog to choose file to open */
void MainWindow::on_actionFile_triggered() {
    QString filename = QFileDialog::getOpenFileName(this, "Open a File", QDir::home().absolutePath());

    openFile(filename);
}

void MainWindow::on_newDir(const QString &path) {
    QFileInfo info(path);

    if (info.isFile()) {
        QString location = info.dir().absolutePath();

        bool ok;
        QString fileName = QInputDialog::getText(this, tr("New Directory"), tr("Directory name: "), QLineEdit::Normal,
                                                 tr("NewDir"), &ok);
        if (ok && !fileName.isEmpty()) {
            QString newDir = location + tr("/") + fileName;
            QDir dir = QDir::root();
            dir.mkpath(newDir);
        }
    }
    if (info.isDir()) {
        bool ok;
        QString fileName = QInputDialog::getText(this, tr("New Directory"), tr("Directory name: "), QLineEdit::Normal,
                                                 tr("NewDir"), &ok);
        if (ok && !fileName.isEmpty()) {
            QString newDir = path + tr("/") + fileName;
            QDir dir = QDir::root();
            dir.mkpath(newDir);
        }
    }
}

void MainWindow::on_newFile(const QString& path) {
    QFileInfo info(path);

    if (info.isFile()) {
        QString location = info.dir().absolutePath();

        bool ok;
        QString fileName = QInputDialog::getText(this, tr("New File"), tr("File name: "), QLineEdit::Normal,
                                                 tr("NewFile.txt"), &ok);
        if (ok && !fileName.isEmpty()) {
            QString newFile = location + tr("/") + fileName;
            QFile file(newFile);
            file.open(QIODevice::WriteOnly);
            file.close();
        }
    }
    if (info.isDir()) {
        bool ok;
        QString fileName = QInputDialog::getText(this, tr("New File"), tr("File name: "), QLineEdit::Normal,
                                                 tr("NewFile.txt"), &ok);
        if (ok && !fileName.isEmpty()) {
            QString newFile = path + tr("/") + fileName;
            QFile file(newFile);
            file.open(QIODevice::WriteOnly);
            file.close();
        }
    }
}

void MainWindow::on_rename(const QString &path) {
    QFileInfo info(path);

    if (info.isFile()) {
        bool ok;
        QFile file(path);
        QString newName = QInputDialog::getText(this, tr("Rename File"), tr("New file name: "), QLineEdit::Normal,
                                                info.fileName(), &ok);
        QString path = info.dir().absolutePath() + tr("/") + newName;
        if (ok && !newName.isEmpty()) {
            if (file.rename(path))
                ErrorMessageBox(this, "Failed to rename file:", "This file can't be renamed.");
        }
    }
    if (info.isDir()) {
        bool ok;
        QDir dir(path);
        QString newName = QInputDialog::getText(this, tr("Rename Directory"), tr("New directory name: "), QLineEdit::Normal,
                                                dir.dirName(), &ok);
        QString path = info.dir().absolutePath() + tr("/") + newName;
        if (ok && !newName.isEmpty()) {
            if (!dir.rename(dir.path(), path))
                ErrorMessageBox(this, "Failed to rename directory:", "This directory can't be renamed.");
        }
    }
}

void MainWindow::on_delete(const QString &path) {
    QFileInfo info(path);

    if (info.isFile()) {
        QFile file(path);
        QMessageBox msg_box;
        msg_box.setText("Confirm:");
        msg_box.setInformativeText("Delete file \"" + info.fileName() + "\"?");
        msg_box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msg_box.setDefaultButton(QMessageBox::Cancel);
        int resp = msg_box.exec();

        if (resp == QMessageBox::Ok) {
            if (!file.remove())
                ErrorMessageBox(this, "Failed to delete file:", "This file can't be deleted.");
        }
    }
    if (info.isDir()) {
        QDir dir(path);
        QMessageBox msg_box;
        msg_box.setText("Confirm:");
        msg_box.setInformativeText("Delete directory \"" + info.fileName() + "\" with all of its content?");
        msg_box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msg_box.setDefaultButton(QMessageBox::Cancel);

        int resp = msg_box.exec();

        if (resp == QMessageBox::Ok) {
            if (!dir.removeRecursively())
                ErrorMessageBox(this, "Failed to delete directory:", "This directory can't be deleted.");
        }
    }
}

/* creates QFileDialog to choose directory and set up working tree sidebar */
void MainWindow::on_actionDirectory_triggered() {
    QString dirname = QFileDialog::getExistingDirectory(this, "", QDir::home().absolutePath(), QFileDialog::ShowDirsOnly
                                                            | QFileDialog::DontResolveSymlinks);
    if (dirname != "") {
        //gets location
        dirmodel = new QFileSystemModel(this);
        if (m_workTree_tabWidget == nullptr) {
            m_workTree_tabWidget = new QTabWidget;
            m_workTree_tabWidget->setTabsClosable(true);
            m_workTree_tabWidget->setTabPosition(QTabWidget::West);
            m_ui->splitter->addWidget(m_workTree_tabWidget);
            connect(m_workTree_tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeRequestedProject);
        }
        QFileInfo info(dirname);

        if (!dirIsOpened(info.fileName())) {
            m_projectsVector.push_back(new ProjectsView(this, dirmodel, info.absoluteFilePath()));
            m_workTree_tabWidget->addTab(m_projectsVector.back(), info.fileName());
            connect(m_projectsVector.back(), &ProjectsView::file_chosen, this, &MainWindow::doubleClick_onTreeView);
            connect(m_projectsVector.back(), &ProjectsView::newFile_triggered, this, &MainWindow::on_newFile);
            connect(m_projectsVector.back(), &ProjectsView::newDir_triggered, this, &MainWindow::on_newDir);
            connect(m_projectsVector.back(), &ProjectsView::rename_triggered, this, &MainWindow::on_rename);
            connect(m_projectsVector.back(), &ProjectsView::delete_triggered, this, &MainWindow::on_delete);
            setLinesText();
        } else {
            ErrorMessageBox msgBox(this, "Can't open directory:", "Directory has already been opened");
        }
    }
}
