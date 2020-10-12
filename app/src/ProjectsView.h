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
    ProjectsView(QWidget* parent);
//    ~ProjectsView();

    void addProject(QFileSystemModel* model, QString dirname, QString path);

private:
    QVBoxLayout* m_layout;
    std::vector<QTreeView*> m_treeViewsVector;
    std::vector<QFileSystemModel*> m_modelsVector;
};

#endif //UTEXT_PROJECTSDIR_H
