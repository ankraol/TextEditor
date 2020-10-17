#ifndef UTEXT_LINENUMBERAREA_H
#define UTEXT_LINENUMBERAREA_H

#include <QWidget>
#include <QSize>
#include <QPaintEvent>

class CodeEditor;

class LineNumberArea : public QWidget {
    Q_OBJECT

public:
    LineNumberArea(CodeEditor *editor);

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    CodeEditor *codeEditor;
};

#endif //UTEXT_LINENUMBERAREA_H
