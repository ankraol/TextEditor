#include "ProjectsView.h"

ProjectsView::ProjectsView(QWidget* parent, QFileSystemModel* model, QString dirname) :
                            QWidget(parent), m_dirName(dirname), m_model(model) {
    m_treeView = new QTreeView(this);
    m_layout = new QVBoxLayout;

    m_treeView->setModel(model);
    m_treeView->setRootIndex(model->setRootPath(m_dirName));

    for (int i = 1; i < model->columnCount(); ++i)
        m_treeView->hideColumn(i);
    m_layout->addWidget(m_treeView);
    setLayout(m_layout);

    m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(m_treeView, &QTreeView::doubleClicked, this, &ProjectsView::on_doubleClick);
    connect(m_treeView, &QTreeView::customContextMenuRequested, this, &ProjectsView::showContextMenu);
    show();
}

void ProjectsView::showContextMenu(const QPoint& pos) {
    m_point = pos;

    QMenu myMenu;
    QAction *NewFile = new QAction("New File", this);
    QAction *NewDir = new QAction("New Directory", this);
    QAction *Rename = new QAction("Rename", this);
    QAction *Delete = new QAction("Delete", this);

    myMenu.addAction(NewFile);
    myMenu.addAction(NewDir);
    myMenu.addAction(Rename);
    myMenu.addAction(Delete);

    connect(NewFile, &QAction::triggered, this, &ProjectsView::on_newFile_action);
    connect(NewDir, &QAction::triggered, this, &ProjectsView::on_newDir_action);
    connect(Rename, &QAction::triggered, this, &ProjectsView::on_rename_action);
    connect(Delete, &QAction::triggered, this, &ProjectsView::on_delete_action);

    myMenu.exec(m_treeView->viewport()->mapToGlobal(pos));
}

void ProjectsView::on_newFile_action() {
    emit newFile_triggered(m_model->filePath(m_treeView->indexAt(m_point)));
}

void ProjectsView::on_newDir_action() {
    emit newDir_triggered(m_model->filePath(m_treeView->indexAt(m_point)));
}

void ProjectsView::on_rename_action() {
    emit rename_triggered(m_model->filePath(m_treeView->indexAt(m_point)));
}

void ProjectsView::on_delete_action() {
    emit delete_triggered(m_model->filePath(m_treeView->indexAt(m_point)));
}

QTreeView*  ProjectsView::getTreeView() {
    return m_treeView;
}

void ProjectsView::on_doubleClick(const QModelIndex &index) {
    emit file_chosen(m_model->filePath(index));
}


