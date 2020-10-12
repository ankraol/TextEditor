#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <string>
#include <vector>

#include <QMainWindow>
#include <QLabel>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QDebug>
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

#include <fstream>

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
    Ui::MainWindow* ui;

    //before file was opened or created
    QWidget *m_widget;
    QVBoxLayout *m_layout; //box for label hints
    QPushButton *m_hintButton_CreateFile;
    QPushButton *m_hintButton_OpenFile;

    //working tree sidebar
    ProjectsView *m_projectsView;

    //for work with file
    QString filePath;
    QFileSystemModel* dirmodel;
    QTabWidget* m_codeEditors_Tabs{nullptr};
    std::vector<tabWidget*> m_codeEditors_Vector; //each new tab editor is added here;
    QFileDialog* m_newFile_Dialog; //to create new file;

    friend class CodeEditor;
};

#endif // MAINWINDOW_H
