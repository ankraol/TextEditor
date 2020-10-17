#ifndef UTEXT_CODEEDITOR_H
#define UTEXT_CODEEDITOR_H

#include <QPlainTextEdit>
#include <QPainter>
#include <QTextBlock>
#include <QWidget>
#include <QResizeEvent>
#include <QRect>
#include <QPaintEvent>
#include <QTextCharFormat>
#include <QLabel>

#include "mainwindow.h"

class Highlighter;

class CodeEditor : public QPlainTextEdit {
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    void setTextFromFile(QString text);
    QString getText();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth();
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    Highlighter* m_highlighter;
    QWidget* m_lineNumberArea;
    QFont m_font;
};

#endif //UTEXT_CODEEDITOR_H
