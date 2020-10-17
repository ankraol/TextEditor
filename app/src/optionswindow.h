#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include "mainwindow.h"

#include <QDialog>

namespace Ui {
class OptionsWindow;
}

class OptionsWindow : public QDialog {
    Q_OBJECT

public:
    explicit OptionsWindow(QWidget *parent = nullptr);
    ~OptionsWindow();

    QCheckBox* getWrapBox() const;
    QCheckBox* getDarkBox() const;

private:
    Ui::OptionsWindow *m_ui;
};

#endif // OPTIONSWINDOW_H
