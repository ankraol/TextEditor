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

// #include <QPoint>

// MY INCS
#include <QDebug>
#include "mainwindow.h"

class CodeEditor : public QPlainTextEdit {
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth();
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    QWidget* m_lineNumberArea;
    QFont m_font;
};

#endif //UTEXT_CODEEDITOR_H
