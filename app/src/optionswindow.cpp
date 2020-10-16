#include "optionswindow.h"
#include "ui_optionswindow.h"

OptionsWindow::OptionsWindow(QWidget *parent)
    : QDialog(parent),
      m_ui(new Ui::OptionsWindow) {
    m_ui->setupUi(this);
}

OptionsWindow::~OptionsWindow() {
    delete m_ui;
}

QCheckBox* OptionsWindow::getWrapBox() const { return m_ui->wrapBox; }
