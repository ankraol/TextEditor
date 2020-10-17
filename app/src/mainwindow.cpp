#include "mainwindow.h"
#include "optionswindow.h"
#include <ui_mainwindow.h>

#include "tabWidget.h"
#include "ProjectsView.h"
#include "searchwindow.h"
#include "CodeEditor.h"
#include "ErrorMessageBox.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_ui(new Ui::MainWindow) {
    m_ui->setupUi(this);

    m_searchWindow = new SearchWindow(this);
    m_searchWindow->hide();

    m_optionsWindow = new OptionsWindow(this);
    m_optionsWindow->hide();

    setupImages();

    m_widget = new QWidget;
    m_layout = new QVBoxLayout;
    m_hintButton_CreateFile = new QPushButton("Create New File  |  ⌘N");
    m_hintButton_OpenFile = new QPushButton("Open File  |  ⌘O");

    // Setup buttons
    m_hintButton_CreateFile->setFlat(true);
    m_hintButton_OpenFile->setFlat(true);

    m_hintButton_CreateFile->setObjectName("CreateFile_Button");
    m_hintButton_OpenFile->setObjectName("CreateFile_Button");

    // Adding buttons
    m_layout->addWidget(m_hintButton_OpenFile, QSizePolicy::Maximum);
    m_layout->addWidget(m_hintButton_CreateFile, QSizePolicy::Maximum);
    m_layout->setAlignment(m_hintButton_OpenFile, Qt::AlignBottom);
    m_layout->setAlignment(m_hintButton_CreateFile, Qt::AlignTop);
    m_widget->setLayout(m_layout);
    m_ui->splitter->addWidget(m_widget);

    // Setup connections
    m_hintButton_OpenFile->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_O));
    m_hintButton_CreateFile->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_N));
    connect(m_hintButton_OpenFile, &QPushButton::clicked, this, &MainWindow::on_actionFile_triggered);
    connect(m_ui->actionCreateFile_Project, &QAction::triggered, this, &MainWindow::createNewFile);
    connect(m_hintButton_CreateFile, &QPushButton::clicked, this, &MainWindow::createNewFile);
    connect(m_ui->findBtn, &QPushButton::clicked, this, &MainWindow::findBtn);
    connect(m_ui->printBtn, &QPushButton::clicked, this, &MainWindow::printerDialog);
    connect(m_ui->zoomInBtn, &QPushButton::clicked, this, &MainWindow::zoomIn);
    connect(m_ui->zoomOutBtn, &QPushButton::clicked, this, &MainWindow::zoomOut);
    connect(m_ui->optionsBtn, &QPushButton::clicked, m_optionsWindow, &QWidget::show);

    // Options
    connect(m_optionsWindow->getWrapBox(), &QCheckBox::clicked, this, &MainWindow::setupWrap);
    connect(m_optionsWindow->getDarkBox(), &QCheckBox::clicked, this, &MainWindow::setupDark);

    // Hotkeys
    m_searchShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), this);
    connect(m_searchShortcut, &QShortcut::activated, m_ui->findBtn, &QPushButton::click);
    m_zoomOutShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus), this);
    connect(m_zoomOutShortcut, &QShortcut::activated, this, &MainWindow::zoomOut);
    m_zoomInShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Equal), this);
    connect(m_zoomInShortcut, &QShortcut::activated, this, &MainWindow::zoomIn);

    show();
}

MainWindow::~MainWindow() {
    // Shortcuts
    delete m_searchShortcut;
    delete m_zoomOutShortcut;
    delete m_zoomInShortcut;

    // Windows
    delete m_optionsWindow;
    delete m_searchWindow;

    delete m_ui;
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
    QString file = m_codeEditors_Tabs->tabText(m_codeEditors_Tabs->currentIndex());
    QFile outputFile(file);
    outputFile.open(QIODevice::WriteOnly);
    QString text = (*m_codeEditors_Tabs->currentWidget()->findChildren<CodeEditor *>().begin())->getText();

    if (outputFile.isOpen()) {
        QTextStream outStream(&outputFile);
        outStream << text;
        outputFile.close();
    } else
        ErrorMessageBox(this, "An error occurred:", "Changes can't be saved.");
}

void MainWindow::createNewFile() {
    m_newFile_Dialog = new QFileDialog;
    m_newFile_Dialog->setModal(true);
    m_newFile_Dialog->setFileMode(QFileDialog::AnyFile);
    QString fileName = m_newFile_Dialog->getSaveFileName(NULL, "Create New File","Create New File","Create New File");
    openFile(fileName);
}

tabWidget* MainWindow::addNewTab() {
    if (m_codeEditors_Tabs == nullptr) {
        m_codeEditors_Tabs = new QTabWidget(this);
        m_codeEditors_Tabs->setTabsClosable(true);
        m_widget->close();
        m_ui->splitter->addWidget(m_codeEditors_Tabs);
        m_codeEditors_Tabs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // Connections of m_codeEditors_Tabs
        connect(m_codeEditors_Tabs, &QTabWidget::tabCloseRequested, this, &MainWindow::closeRequestedTab);
        connect(m_ui->actionSave_File, &QAction::triggered, this, &MainWindow::saveChanges);
    }

    m_codeEditors_Vector.push_back(new tabWidget());
    m_codeEditors_Tabs->addTab(m_codeEditors_Vector.back(), filePath);

    connect(m_codeEditors_Vector.back()->getEditor(), &CodeEditor::updateRequest, this, &MainWindow::setLinesText);
    connect(m_ui->undoBtn, &QPushButton::clicked, this, &MainWindow::on_undo);
    connect(m_ui->redoBtn, &QPushButton::clicked, this, &MainWindow::on_redo);
    connect(m_ui->copyBtn, &QPushButton::clicked, m_codeEditors_Vector.back()->getEditor(), &CodeEditor::copy);
    connect(m_ui->cutBtn, &QPushButton::clicked, m_codeEditors_Vector.back()->getEditor(), &CodeEditor::cut);
    connect(m_ui->pasteBtn, &QPushButton::clicked, this, &MainWindow::on_paste);

    show();
    return m_codeEditors_Vector.back();
}

void MainWindow::on_paste() {
    if (m_codeEditors_Tabs != nullptr && m_codeEditors_Tabs->count() != 0)
        (*m_codeEditors_Tabs->currentWidget()->findChildren<CodeEditor *>().begin())->paste();
}

void MainWindow::on_redo() {
    if (m_codeEditors_Tabs != nullptr && m_codeEditors_Tabs->count() != 0)
        (*m_codeEditors_Tabs->currentWidget()->findChildren<CodeEditor *>().begin())->redo();
}

void MainWindow::on_undo() {
    if (m_codeEditors_Tabs != nullptr && m_codeEditors_Tabs->count() != 0)
        (*m_codeEditors_Tabs->currentWidget()->findChildren<CodeEditor *>().begin())->undo();
}

void MainWindow::closeRequestedTab(int index) {
    (*m_codeEditors_Tabs->currentWidget()->findChildren<CodeEditor *>().begin())->disconnect();
    m_codeEditors_Tabs->currentWidget()->disconnect();
    m_codeEditors_Tabs->removeTab(index);
    m_ui->l_linesCount->setText("Lines: 0");
}

void MainWindow::on_actionFileOpened() {
    QFile file(filePath);

    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        ErrorMessageBox msgBox(this, "Can't open file:", "File has access restrictions.");
    else {
        addNewTab();
        QString data;
        data = file.readAll();
        m_codeEditors_Vector.back()->setText(data);
    }
}

void MainWindow::setLinesText() {
    if (m_codeEditors_Tabs != nullptr && m_codeEditors_Tabs->count() != 0) {
        CodeEditor* current = (*m_codeEditors_Tabs->currentWidget()->findChildren<CodeEditor*>().begin());
        int x = current->textCursor().positionInBlock() + 1;

        m_ui->l_linesCount->setText(
            QString("Col ") + std::to_string(x).c_str() +
            " | " + std::to_string(current->blockCount()).c_str() + " lines"
        );
    }
}

void MainWindow::findBtn() {
    if (m_codeEditors_Tabs != nullptr && m_codeEditors_Tabs->count() != 0) {
        CodeEditor* current = (*m_codeEditors_Tabs->currentWidget()->findChildren<CodeEditor*>().begin());

        QString s = current->textCursor().selectedText();
        m_searchWindow->setupStartParams(current, s);
        m_searchWindow->show();
    }
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

    m_ui->printBtn->setIcon(m_printIcon);
    m_ui->printBtn->setIconSize(m_icSize);

    m_ui->zoomInBtn->setIcon(m_zoomInIcon);
    m_ui->zoomInBtn->setIconSize(m_icSize);

    m_ui->zoomOutBtn->setIcon(m_zoomOutIcon);
    m_ui->zoomOutBtn->setIconSize(m_icSize);
}

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

void MainWindow::printerDialog() {
    if (m_codeEditors_Tabs != nullptr && m_codeEditors_Tabs->count() != 0) {
        CodeEditor* current = (*m_codeEditors_Tabs->currentWidget()->findChildren<CodeEditor*>().begin());
        QPrinter printer;
        QPrintDialog dialog(&printer, this);

        dialog.setWindowTitle("Print Document");
        if (current->textCursor().hasSelection())
            dialog.addEnabledOption(QAbstractPrintDialog::PrintSelection);
        if (dialog.exec() == QDialog::Accepted)
            current->print(&printer);
    }
}

void MainWindow::zoomIn() {
    if (m_codeEditors_Tabs != nullptr && m_codeEditors_Tabs->count() != 0) {
        CodeEditor* current = (*m_codeEditors_Tabs->currentWidget()->findChildren<CodeEditor*>().begin());
        current->zoomIn();
    }
}

void MainWindow::zoomOut() {
    if (m_codeEditors_Tabs != nullptr && m_codeEditors_Tabs->count() != 0) {
        CodeEditor* current = (*m_codeEditors_Tabs->currentWidget()->findChildren<CodeEditor*>().begin());
        current->zoomOut();
    }
}

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
    else if (info.isDir()) {
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
    else if (info.isDir()) {
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
    else if (info.isDir()) {
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
    else if (info.isDir()) {
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

void MainWindow::on_actionDirectory_triggered() {
    QString dirname = QFileDialog::getExistingDirectory(
        this,
        "",
        QDir::home().absolutePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );

    if (dirname != "") {
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
        } else
            ErrorMessageBox msgBox(this, "Can't open directory:", "Directory has already been opened");
    }
}

void MainWindow::setupWrap() {
    if (m_codeEditors_Tabs != nullptr && m_codeEditors_Tabs->count() != 0) {
        QList<CodeEditor*> cErs = m_codeEditors_Tabs->currentWidget()->findChildren<CodeEditor*>();
        QCheckBox* wrapBox = m_optionsWindow->getWrapBox();

        if (wrapBox->isChecked()) {
            for (auto it = cErs.begin(); it != cErs.end(); it++) {
                (*it)->setLineWrapMode(QPlainTextEdit::LineWrapMode::WidgetWidth);
            }
        } else {
            for (auto it = cErs.begin(); it != cErs.end(); it++) {
                (*it)->setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);
            }
        }
    }
}

void MainWindow::setupDark() {
    QCheckBox* darkBox = m_optionsWindow->getDarkBox();

    if (darkBox->isChecked()) {
        m_qss_dark.open(QFile::ReadOnly);
        this->setStyleSheet(m_qss_dark.readAll());
        m_qss_dark.close();
    } else
        this->setStyleSheet("");
}
