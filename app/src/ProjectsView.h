#ifndef UTEXT_PROJECTSDIR_H
#define UTEXT_PROJECTSDIR_H

#include <vector>

#include <QWidget>
#include <QVBoxLayout>
#include <QTreeView>
#include <QFileSystemModel>
#include <QString>
#include <QMenu>

class ProjectsView : public QWidget {
    Q_OBJECT

public:
    ProjectsView(QWidget* parent, QFileSystemModel* model, QString dirname);

    QTreeView*  getTreeView();

signals:
    void file_chosen(QString path);
    void newFile_triggered(QString path);
    void newDir_triggered(QString path);
    void rename_triggered(QString path);
    void delete_triggered(QString path);

public slots:
    void on_doubleClick(const QModelIndex &index);
    void showContextMenu(const QPoint& pos);
    void on_newFile_action();
    void on_newDir_action();
    void on_rename_action();
    void on_delete_action();

private:
    QPoint m_point;
    QString m_dirName;
    QVBoxLayout* m_layout;
    QTreeView* m_treeView;
    QFileSystemModel* m_model;
};

#endif //UTEXT_PROJECTSDIR_H
