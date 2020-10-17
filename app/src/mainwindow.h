#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "IncludeHeaders.h"

class CodeEditor;
class SearchWindow;
class PlainTextEdit;
class tabWidget;
class ProjectsView;

class OptionsWindow;

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

    void findBtn();

private slots:
    void on_actionFile_triggered();
    void on_actionDirectory_triggered();
    void on_actionFileOpened();

    void printerDialog();
    void zoomIn();
    void zoomOut();
    void closeRequestedTab(int index);
    void createNewFile();
    void saveChanges();

    bool doubleClick_onTreeView(QString path);
    void closeRequestedProject(int index);

    void on_newFile(const QString& path);
    void on_newDir(const QString& path);
    void on_rename(const QString& path);
    void on_delete(const QString& path);
    void setupWrap();
    void setupDark();

    void on_paste();
    void on_redo();
    void on_undo();


protected:
    tabWidget* addNewTab();
    bool fileIsOpened(QString newFile);
    bool dirIsOpened(QString dirName);
    void openFile(QString fileName);

private:
    Ui::MainWindow* m_ui;

    QWidget *m_widget;
    QVBoxLayout *m_layout;
    QPushButton *m_hintButton_CreateFile;
    QPushButton *m_hintButton_OpenFile;

    QTabWidget* m_workTree_tabWidget{nullptr};
    std::vector<ProjectsView*> m_projectsVector;

    QString filePath;
    QFileSystemModel* dirmodel;
    QTabWidget* m_codeEditors_Tabs{nullptr};
    std::vector<tabWidget*> m_codeEditors_Vector;
    QFileDialog* m_newFile_Dialog;
    CodeEditor* m_codeEditor;
    SearchWindow* m_searchWindow;
    OptionsWindow* m_optionsWindow;

    QPushButton* button;

    QSize m_icSize;
    QIcon m_undoIcon{"app/res/images/undo.png"};
    QIcon m_redoIcon{"app/res/images/redo.png"};
    QIcon m_copyIcon{"app/res/images/copy.png"};
    QIcon m_cutIcon{"app/res/images/cut.png"};
    QIcon m_pasteIcon{"app/res/images/paste.png"};
    QIcon m_findIcon{"app/res/images/search.png"};
    QIcon m_optionsIcon{"app/res/images/settings.png"};
    QIcon m_printIcon{"app/res/images/printer.png"};
    QIcon m_zoomInIcon{"app/res/images/zoom-in.png"};
    QIcon m_zoomOutIcon{"app/res/images/zoom-out.png"};

    // HOTKEYS
    QShortcut* m_searchShortcut{nullptr};
    QShortcut* m_zoomInShortcut{nullptr};
    QShortcut* m_zoomOutShortcut{nullptr};

    QFile m_qss_dark{"app/res/DarkMode.qss"};


    friend class CodeEditor;
};

#endif // MAINWINDOW_H
