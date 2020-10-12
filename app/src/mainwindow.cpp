#include "mainwindow.h"
#include <ui_mainwindow.h>

#include "tabWidget.h"
#include "ClickableLabel.h"
#include "ProjectsView.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //creating working project tree area
    m_projectsView = new ProjectsView(this);
    ui->scrollArea->setWidget(m_projectsView);

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
    ui->splitter->addWidget(m_widget);
    ui->splitter->setSizes(QList<int>() << -1 << INT_MAX);

    //setting up connections
    m_hintButton_OpenFile->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_O));
    m_hintButton_CreateFile->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_N));
    connect(ui->actionFile, &QAction::triggered, this, &MainWindow::on_actionFileOpened);
    connect(m_hintButton_OpenFile, &QPushButton::clicked, this, &MainWindow::on_actionFile_triggered);
    //connect(m_codeEditor, &CodeEditor::updateRequest, this, &MainWindow::setLinesText); // to set on which line i am
    connect(ui->actionCreateFile_Project, &QAction::triggered, this, &MainWindow::createNewFile);
    connect(m_hintButton_CreateFile, &QPushButton::clicked, this, &MainWindow::createNewFile);
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
        messageBox.critical(0,"Error","An error has occurred!");
        messageBox.setFixedSize(500,200);
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
        m_codeEditors_Tabs = new QTabWidget;
        m_codeEditors_Tabs->setTabsClosable(true);
        m_widget->close(); //if there is delete on close flag it will eventually get deleted;
        ui->splitter->addWidget(m_codeEditors_Tabs);
        m_codeEditors_Tabs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        //connections of m_codeEditors_Tabs
        connect(m_codeEditors_Tabs, &QTabWidget::tabCloseRequested, this, &MainWindow::closeRequestedTab); //removes tab, when exit button pressed
        connect(ui->actionSave_File, &QAction::triggered, this, &MainWindow::saveChanges); //saves changes on current tab;
    }
    qDebug() << "new tab must appear";
    tabWidget* newTab = new tabWidget;
    m_codeEditors_Tabs->addTab(newTab, filePath);
    m_codeEditors_Vector.push_back(newTab);

    //here connection of tab widgets should be written
    connect((*m_codeEditors_Tabs->currentWidget()->findChildren<CodeEditor *>().begin()), &CodeEditor::updateRequest, this, &MainWindow::setLinesText); //connects line counter label to Text editor
    show();
    return newTab;
}

/* closes requested tab */
void MainWindow::closeRequestedTab(int index) {
    m_codeEditors_Tabs->currentWidget()->disconnect();
    m_codeEditors_Tabs->removeTab(index);
    ui->l_linesCount->setText("Lines: 0");
}

/* reads from filepath and sets up the text of file
 * to the new tab code editor */
void MainWindow::on_actionFileOpened() {
    tabWidget* widget = addNewTab();

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
        widget->getEditor()->setTextFromFile(QString::fromUtf8(memblock));
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

    ui->l_linesCount->setText(
        QString("Lines: ") + std::to_string((*m_codeEditors_Tabs->currentWidget()->findChildren<CodeEditor *>().begin())->blockCount()).c_str()
        );
}

/* deletes all allocated memory
 * (at least has to but right now doesn't)*/
MainWindow::~MainWindow() {
    delete ui;
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

        QString projectName = info.dir().absolutePath();
        QFileInfo info2(projectName);

        m_projectsView->addProject(dirmodel, projectName, info2.dir().absolutePath());
        on_actionFileOpened();
        ui->splitter->setSizes(QList<int>({-1, INT_MAX}));
    }
}

/* creates QFileDialog to choose file to open */
void MainWindow::on_actionFile_triggered() {
    QString filename = QFileDialog::getOpenFileName(this, "Open a File", QDir::home().absolutePath());

    openFile(filename);
}

/* creates QFileDialog to choose directory and set up working tree sidebar */
void MainWindow::on_actionDirectory_triggered() {
    QString dirname = QFileDialog::getExistingDirectory(this, "", QDir::home().absolutePath(), QFileDialog::ShowDirsOnly
                                                            | QFileDialog::DontResolveSymlinks);
    //check directory name
    qDebug() << dirname;

    //gets location
    QFileInfo info(dirname);

    dirmodel = new QFileSystemModel(this);
    m_projectsView->addProject(dirmodel, dirname, info.dir().absolutePath());
    setLinesText();
}
