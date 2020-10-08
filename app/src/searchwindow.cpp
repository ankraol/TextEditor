#include "searchwindow.h"
#include "CodeEditor.h"

#include <ui_searchwindow.h>

SearchWindow::SearchWindow(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SearchWindow)
{
    m_ui->setupUi(this);

    connect(m_ui->pushButton, &QPushButton::clicked, this, &SearchWindow::searchBtn);
}

SearchWindow::~SearchWindow()
{
    delete m_ui;
}

void SearchWindow::setupStartParams(CodeEditor*& cdEdit, QString selected) {
    m_codeEditor = cdEdit;

    m_ui->findText->setText(selected);
    m_ui->replaceText->setText("");
}

void SearchWindow::searchBtn() {
    QString fStr = m_ui->findText->text();
    QString rStr = m_ui->replaceText->text();

    checkMode();
    if (!m_ui->regexCheckBox->isChecked()) {
        if (m_codeEditor->find(fStr, m_flgMode)) {
            if (m_ui->replaceCheckBox->isChecked())
                doReplace(rStr);
        }
    }
    else {
        QRegExp exp(fStr);
        if (m_codeEditor->find(exp, m_flgMode)) {
            if (m_ui->replaceCheckBox->isChecked())
                doReplace(rStr);
        }
    }
}

void SearchWindow::doReplace(QString replaceString) {
    m_codeEditor->textCursor().clearSelection();
    m_codeEditor->textCursor().insertText(replaceString);
}

void SearchWindow::checkMode() {
    if (m_ui->findModeComboBox->currentIndex() == 0)
        m_flgMode = QTextDocument::FindFlag::FindBackward;
    else if (m_ui->findModeComboBox->currentIndex() == 1)
        m_flgMode = QTextDocument::FindFlag::FindCaseSensitively;
    else
        m_flgMode = QTextDocument::FindFlag::FindWholeWords;
}
