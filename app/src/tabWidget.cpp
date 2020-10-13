#include "tabWidget.h"

tabWidget::tabWidget(QWidget* parent) : QWidget(parent) {
    m_editor = new CodeEditor(this);
    m_layout = new QVBoxLayout;

    m_editor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_layout->addWidget(m_editor);
    setLayout(m_layout);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    show();
}

tabWidget::~tabWidget() {
    delete m_layout;
    delete m_editor;
}

CodeEditor* tabWidget::getEditor() {
    return m_editor;
}

void tabWidget::setText(QString text) {
    m_editor->setTextFromFile(text);
}
