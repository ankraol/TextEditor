#include "mainwindow.h"
#include <ui_mainwindow.h>

#include "tabWidget.h"
#include "ClickableLabel.h"
#include "ProjectsView.h"
#include "searchwindow.h"
#include "CodeEditor.h"


MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);


    qDebug() << "1";
    m_searchWindow = new SearchWindow(this);
    m_searchWindow->hide();

    qDebug() << "2";

    qDebug() << "3";

    setupImages();

    qDebug() << "4";

    //creating starting window
    m_widget = new QWidget;
    m_layout = new QVBoxLayout;
    m_hintButton_CreateFile = new QPushButton ("Create New File  |  ⌘N");
    m_hintButton_OpenFile = new QPushButton ("Open File  |  ⌘O");

    qDebug() << "5";

    //set up buttons
    m_hintButton_CreateFile->setFlat(true);
    m_hintButton_OpenFile->setFlat(true);

    qDebug() << "6";

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
//    m_ui->splitter->setSizes(QList<int>() << -1 << INT_MAX);

    //setting up connections
    m_hintButton_OpenFile->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_O));
    m_hintButton_CreateFile->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_N));
    connect(m_ui->actionFile, &QAction::triggered, this, &MainWindow::on_actionFileOpened);
    connect(m_hintButton_OpenFile, &QPushButton::clicked, this, &MainWindow::on_actionFile_triggered);
    connect(m_ui->actionCreateFile_Project, &QAction::triggered, this, &MainWindow::createNewFile);
    connect(m_hintButton_CreateFile, &QPushButton::clicked, this, &MainWindow::createNewFile);
    connect(m_ui->findBtn, &QPushButton::clicked, this, &MainWindow::findBtn);

}

void MainWindow::saveChanges() {
    QString file =  m_codeEditors_Tabs->tabText(m_codeEditors_Tabs->currentIndex());
    qDebug() << file;

    QFile outputFile(file);
    outputFile.open(QIODevice::WriteOnly);
    QString text = (*m_codeEditors_Tabs->currentWidget()->findChildren<CodeEditor *>().begin())->getText();

    if (outputFile.isOpen()) {
        QTextStream outStream(&outputFile);
        outStream << text;
        outputFile.close();
    } else {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "An error has occurred!");
        messageBox.setFixedSize(500, 200);
    }
}

/* creates QDialog to select location and name of a new file
 * and creates it)) */
void MainWindow::createNewFile() {
    m_newFile_Dialog = new QFileDialog;
    m_newFile_Dialog->setModal(true);
    m_newFile_Dialog->setFileMode(QFileDialog::AnyFile);
    QString fileName = m_newFile_Dialog->getSaveFileName(NULL, "Create New File","","");
    qDebug() << fileName;
    openFile(fileName);
}

/* adds new Tab to the QTabWidget every time new
 * project/file is opened */
tabWidget* MainWindow::addNewTab() {
    if (m_codeEditors_Tabs == nullptr) {
        qDebug() << "Create new QTabWidget";
        m_codeEditors_Tabs = new QTabWidget(this);
        m_codeEditors_Tabs->setTabsClosable(true);
        m_widget->close(); //if there is delete on close flag it will eventually get deleted;
        m_ui->splitter->addWidget(m_codeEditors_Tabs);
        m_codeEditors_Tabs->setFocus();
        qDebug() << "Success on focus";
        m_codeEditors_Tabs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        //connections of m_codeEditors_Tabs
        connect(m_codeEditors_Tabs, &QTabWidget::tabCloseRequested, this, &MainWindow::closeRequestedTab); //removes tab, when exit button pressed
        connect(m_ui->actionSave_File, &QAction::triggered, this, &MainWindow::saveChanges); //saves changes on current tab;
    }
    qDebug() << "new tab must appear";
//    tabWidget* newTab = new tabWidget(this);
    m_codeEditors_Vector.push_back((new tabWidget()));
    m_codeEditors_Tabs->addTab(m_codeEditors_Vector.back(), filePath);

//    m_codeEditor = new CodeEditor(this);
//    m_widget->close();
//    m_ui->splitter->addWidget(m_codeEditor);


    //here connection of tab widgets should be written
    connect(m_codeEditors_Vector.back()->getEditor(), &CodeEditor::updateRequest, this, &MainWindow::setLinesText); //connects line counter label to Text editor
    connect(m_ui->undoBtn, &QPushButton::clicked, m_codeEditors_Vector.back()->getEditor(), &CodeEditor::undo);
    connect(m_ui->redoBtn, &QPushButton::clicked, m_codeEditors_Vector.back()->getEditor(), &CodeEditor::redo);
    connect(m_ui->copyBtn, &QPushButton::clicked, m_codeEditors_Vector.back()->getEditor(), &CodeEditor::copy);
    connect(m_ui->cutBtn, &QPushButton::clicked, m_codeEditors_Vector.back()->getEditor(), &CodeEditor::cut);
    connect(m_ui->pasteBtn, &QPushButton::clicked, m_codeEditors_Vector.back()->getEditor(), &CodeEditor::paste);
    show();
    return m_codeEditors_Vector.back();
//    return nullptr;
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
//    tabWidget* widget = addNewTab();
    addNewTab();
    qDebug() << "Widget is added";

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
//        m_codeEditors_Vector.back()->getEditor()->setTextFromFile(QString::fromUtf8(memblock));
        qDebug() << "Text is set";
        delete [] memblock;
    } else {
        std::cout << "Failed to open a file.\n";
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
//    int x = m_codeEditor->textCursor().positionInBlock() + 1;

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

//bool MainWindow::doubleClick_onTreeView(const QModelIndex &index) {
//    qDebug() << "Double clicked";
//
//    const QAbstractItemModel * model = index.model();
//    qDebug() << index.data();
//    qDebug() << model->data(model->index(0, 0), Qt::DisplayRole);
//    return true;
//}

/* calls on_actionFileOpened func to fill in the Text Editor field */
void MainWindow::openFile(QString fileName) {

    if (fileName != "") {
        QFileInfo info(fileName);
        filePath = fileName;
        dirmodel = new QFileSystemModel(this);
//        m_projectsView->addProject(dirmodel, projectName, info2.dir().absolutePath());
        if (m_workTree_tabWidget == nullptr) {
            m_workTree_tabWidget = new QTabWidget;
            m_workTree_tabWidget->setTabsClosable(true);
            qDebug() << m_ui->splitter->count();
            m_ui->splitter->addWidget(m_workTree_tabWidget);
//            m_ui->splitter->addWidget(m_widget);
            qDebug() << m_ui->splitter->count();
//            m_workTree_tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        }
        ProjectsView* m_widget = new ProjectsView (this, dirmodel, info.dir().absolutePath());
        m_workTree_tabWidget->addTab(m_widget, info.dir().dirName());
        m_projectsVector.push_back(m_widget);
        on_actionFileOpened();
        qDebug() << "on_actionFileOpened success";


//        m_ui->splitter->setSizes()
        // MOVE SPLITTER TO LEFT
//        QList<int> currentSizes = m_ui->splitter->sizes();
//        currentSizes[0] = 0;
//        currentSizes[1] = 1;
//        m_ui->splitter->setSizes(currentSizes);
    }
}

/* creates QFileDialog to choose file to open */
void MainWindow::on_actionFile_triggered() {
    QString filename = QFileDialog::getOpenFileName(this, "Open a File", QDir::home().absolutePath());
//    m_widget->hide();
    openFile(filename);
    qDebug() << "File is opened";
}

/* creates QFileDialog to choose directory and set up working tree sidebar */
void MainWindow::on_actionDirectory_triggered() {
    QString dirname = QFileDialog::getExistingDirectory(this, "", QDir::home().absolutePath(), QFileDialog::ShowDirsOnly
                                                            | QFileDialog::DontResolveSymlinks);
//    m_widget->hide();
    //check directory name
    qDebug() << dirname;

    //gets location
    qDebug() << m_ui->splitter->count();
    dirmodel = new QFileSystemModel(this);
    if (m_workTree_tabWidget == nullptr) {
        m_workTree_tabWidget = new QTabWidget;
        m_workTree_tabWidget->setTabsClosable(true);
        m_ui->splitter->addWidget(m_workTree_tabWidget);
//        m_workTree_tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }
    qDebug() << m_ui->splitter->count();
    QFileInfo info(dirname);
    ProjectsView* m_widget = new ProjectsView (this, dirmodel, info.dir().absolutePath());
    m_workTree_tabWidget->addTab(m_widget, info.dir().dirName());
    m_projectsVector.push_back(m_widget);
    setLinesText();
}
