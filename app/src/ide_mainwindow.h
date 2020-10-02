#ifndef IDE_MAINWINDOW_H
#define IDE_MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class IDE_MainWindow;
}

class IDE_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit IDE_MainWindow(QWidget *parent = nullptr);
    ~IDE_MainWindow();

private:
    Ui::IDE_MainWindow *ui;
};

#endif // IDE_MAINWINDOW_H
