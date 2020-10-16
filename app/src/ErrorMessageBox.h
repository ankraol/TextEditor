#ifndef UTEXT_ERRORMESSAGEBOX_H
#define UTEXT_ERRORMESSAGEBOX_H

#include <QMessageBox>
#include <QWidget>
#include <QString>

class ErrorMessageBox : public QMessageBox {
public:
    ErrorMessageBox(QWidget *parent = nullptr, const QString& msg = "", const QString& infText = "");

//private:
    //some private members;
};

#endif //UTEXT_ERRORMESSAGEBOX_H
