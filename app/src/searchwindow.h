#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QDialog>
#include <QTextDocument>
#include <QRegExp>

class CodeEditor;

namespace Ui {
class SearchWindow;
}

class SearchWindow : public QDialog {
    Q_OBJECT

public:
    explicit SearchWindow(QWidget *parent = nullptr);
    ~SearchWindow();

    void setupStartParams(CodeEditor*& cdEdit, QString selected);
    void searchBtn();
    void doReplace(QString replaceString);
    void checkMode();

private:
    Ui::SearchWindow *m_ui;

    CodeEditor* m_codeEditor;
    QString m_selected;

    QTextDocument::FindFlags m_flgMode;
};

#endif // SEARCHWINDOW_H
