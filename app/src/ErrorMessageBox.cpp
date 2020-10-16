#include "ErrorMessageBox.h"

ErrorMessageBox::ErrorMessageBox(QWidget *parent, const QString& msg, const QString& infText) : QMessageBox(parent) {
    setWindowTitle("Error");
    setIcon(QMessageBox::Warning);
    setText(msg);
    setInformativeText(infText);
    exec();
}
