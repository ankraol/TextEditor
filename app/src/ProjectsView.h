#ifndef UTEXT_PROJECTSDIR_H
#define UTEXT_PROJECTSDIR_H

#include <vector>

#include <QWidget>
#include <QVBoxLayout>
#include <QTreeView>
#include <QFileSystemModel>

class ProjectsView : public QWidget {
    Q_OBJECT

public:
    ProjectsView(QWidget* parent, QFileSystemModel* model, QString dirname);
//    ~ProjectsView();

private:
    QString m_dirName;
    QVBoxLayout* m_layout;
    QTreeView* m_treeView;
    QFileSystemModel* m_model;
};

#endif //UTEXT_PROJECTSDIR_H
