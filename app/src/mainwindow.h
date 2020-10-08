#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>

#include <QMainWindow>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QFileInfo>
#include <QPushButton>

#include <QDebug>

// class PlainTextEdit;
class CodeEditor;
class SearchWindow;

namespace Ui {
class MainWindow;
// class CodeEditor;
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

public slots:
    void setFileSystem(const QString* sPath);

private slots:
    void on_actionFile_triggered(); //should open working directory of a file and file;
    void on_actionDirectory_triggered(); //only opens working directory;

private:
    Ui::MainWindow* m_ui;

    QFileSystemModel* dirmodel;
    CodeEditor* m_codeEditor;
    SearchWindow* m_searchWindow;

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
