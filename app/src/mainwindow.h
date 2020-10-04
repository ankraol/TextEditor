#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QDebug>
#include <QFileInfo>

class CodeEditor;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void setFileSystem(const QString* sPath);

private slots:
    void on_actionFile_triggered(); //should open working directory of a file and file;

    void on_actionDirectory_triggered(); //only opens working directory;

private:
    Ui::MainWindow *ui;
    QFileSystemModel *dirmodel;
    CodeEditor *m_codeEditor;
};

#endif // MAINWINDOW_H
