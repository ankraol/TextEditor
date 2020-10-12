#include "ProjectsView.h"

ProjectsView::ProjectsView(QWidget* parent) : QWidget(parent) {
    m_layout = new QVBoxLayout;
    setLayout(m_layout);
    show();
}

void ProjectsView::addProject(QFileSystemModel* model, QString dirname, QString path) {
    m_modelsVector.push_back(model);

    QTreeView* newTree = new QTreeView;
    newTree->setModel(model);
//    qDebug() << path;
    newTree->setRootIndex(model->setRootPath(path));

//    for (int i = 0; i < model->rowCount(); ++i) {
//        if ()
//    }

//    newTree->setFilterKeyColumn(0);
    for (int i = 1; i < model->columnCount(); ++i)
        newTree->hideColumn(i);

    m_treeViewsVector.push_back(newTree);

    m_layout->addWidget(newTree);
    show();
}
