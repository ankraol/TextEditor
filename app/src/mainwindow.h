#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <string>
#include <vector>

#include <QMainWindow>
#include <QLabel>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QSplitter>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QTreeView>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QDebug>

#include <fstream>

// class PlainTextEdit;
class CodeEditor;
class SearchWindow;
class PlainTextEdit;
class tabWidget;
class ClickableLabel;
class ProjectsView;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void setLinesText();
    void setupImages();

    // BUTTONS
    void findBtn();

private slots:
    void on_actionFile_triggered(); //should open working directory of a file and file;
    void on_actionDirectory_triggered(); //only opens working directory;
    void on_actionFileOpened(); //to set text into tab widget;

    void closeRequestedTab(int index); //closes requested tab;
    void createNewFile(); //opens new dialog and creates new file;
    void saveChanges(); //saves all changes done to file;

//    bool doubleClick_onTreeView(const QModelIndex &index); //opens item if it is a file on double click;

    void TestPrint(); //to test signal slot connection;

protected:
    tabWidget* addNewTab(); //adds new tab to QTabWidget
    bool fileIsOpened(); //checks whether the chosen file has already been opened
    void openFile(QString fileName); //opens file and sets it to Text Editor

private:
    Ui::MainWindow* m_ui;

    //before file was opened or created
    QWidget *m_widget;
    QVBoxLayout *m_layout; //box for label hints
    QPushButton *m_hintButton_CreateFile;
    QPushButton *m_hintButton_OpenFile;

    //working tree sidebar
    QTabWidget* m_workTree_tabWidget{nullptr};
    std::vector<ProjectsView*> m_projectsVector;

    //for work with file
    QString filePath;
    QFileSystemModel* dirmodel;
    QTabWidget* m_codeEditors_Tabs{nullptr};
    std::vector<tabWidget*> m_codeEditors_Vector; //each new tab editor is added here;
    QFileDialog* m_newFile_Dialog; //to create new file;
    CodeEditor* m_codeEditor;
    SearchWindow* m_searchWindow;

    QPushButton* button;

    QSize m_icSize;
    QIcon m_undoIcon{"app/res/images/undo.png"};
    QIcon m_redoIcon{"app/res/images/redo.png"};
    QIcon m_copyIcon{"app/res/images/copy.png"};
    QIcon m_cutIcon{"app/res/images/cut.png"};
    QIcon m_pasteIcon{"app/res/images/paste.png"};
    QIcon m_findIcon{"app/res/images/search.png"};
    QIcon m_optionsIcon{"app/res/images/settings.png"};

    friend class CodeEditor;
};

#endif // MAINWINDOW_H
