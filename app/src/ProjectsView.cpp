#include "ProjectsView.h"

ProjectsView::ProjectsView(QWidget* parent, QFileSystemModel* model, QString dirname) :
                            QWidget(parent), m_dirName(dirname), m_model(model) {
    QTreeView* newTree = new QTreeView;
    m_layout = new QVBoxLayout;

    newTree->setModel(model);
    newTree->setRootIndex(model->setRootPath(m_dirName));

    for (int i = 1; i < model->columnCount(); ++i)
        newTree->hideColumn(i);
    m_layout->addWidget(newTree);
    setLayout(m_layout);
    show();
}
