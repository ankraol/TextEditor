#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>

#include <QMainWindow>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QDebug>
#include <QFileInfo>

class CodeEditor;

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

public slots:
    void setFileSystem(const QString* sPath);

private slots:
    void on_actionFile_triggered(); //should open working directory of a file and file;
    void on_actionDirectory_triggered(); //only opens working directory;

private:
    Ui::MainWindow* ui;

    QFileSystemModel* dirmodel;
    CodeEditor* m_codeEditor;

    friend class CodeEditor;
};

#endif // MAINWINDOW_H
