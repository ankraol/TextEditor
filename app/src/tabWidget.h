#ifndef UTEXT_TABWIDGET_H
#define UTEXT_TABWIDGET_H

#include "CodeEditor.h"

#include <QWidget>
#include <QVBoxLayout>

class tabWidget : public QWidget {
public:
    tabWidget(QWidget* parent = nullptr);
    ~tabWidget();

    CodeEditor* getEditor() const;

private:
    QVBoxLayout* m_layout;
    CodeEditor* m_editor;
};

#endif //UTEXT_TABWIDGET_H
