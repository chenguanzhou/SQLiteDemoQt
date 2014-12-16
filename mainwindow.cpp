#include "mainwindow.h"
#include <QtCore>
#include <QtSql>
#include <QHBoxLayout>
#include <QTreeView>
#include <QFileSystemModel>
#include <QListView>
#include <QStringListModel>
#include <QTableView>
#include <QSqlTableModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    fileSystemModel(new QFileSystemModel(this)),
    tableListModel(new QStringListModel(this)),
    tableView (new QTableView(this))
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *hbox = new QHBoxLayout(widget);
    widget->setLayout(hbox);
    this->setCentralWidget(widget);

    QTreeView *fileTreeView = new QTreeView(this);
    fileSystemModel->setRootPath(QDir::currentPath());
    fileTreeView->setModel(fileSystemModel);
    fileTreeView->setSelectionMode(QTreeView::SingleSelection);
    fileTreeView->setSelectionBehavior(QTreeView::SelectRows);
    hbox->addWidget(fileTreeView);
    hbox->setStretchFactor(fileTreeView,1);

    QListView *listView = new QListView(this);
    listView->setModel(tableListModel);
    listView->setSelectionMode(QTreeView::SingleSelection);
    listView->setEditTriggers(QTreeView::NoEditTriggers);
    hbox->addWidget(listView);
    hbox->setStretchFactor(listView,1);

    tableView->setEditTriggers(QTableView::NoEditTriggers);
    hbox->addWidget(tableView);
    hbox->setStretchFactor(tableView,3);

    connect(fileTreeView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),SLOT(fileChanged(QItemSelection,QItemSelection)));
    connect(listView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),SLOT(tableChanged(QItemSelection,QItemSelection)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::fileChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);

    if (selected.size()!=1)
        return;

    QItemSelectionRange range = selected[0];
    if (range.indexes().empty())
        return;

    QModelIndex index = range.indexes()[0];
    QFileInfo info = fileSystemModel->fileInfo(index);
    if (!info.isFile())
    {
        qDebug()<<"Not a file";
        return;
    }

    setDataBase(info.absoluteFilePath());
}

void MainWindow::tableChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    if (selected.size()!=1)
        return;

    QItemSelectionRange range = selected[0];
    if (range.indexes().empty())
        return;

    QModelIndex index = range.indexes()[0];
    QString tableName = tableListModel->data(index,Qt::DisplayRole).toString();
    QSqlTableModel *tableModel = qobject_cast<QSqlTableModel *>(tableView->model());
    tableModel->setTable(tableName);
    tableModel->select();
}

void MainWindow::setDataBase(const QString &filePath)
{
    if (dbPath == filePath)
        return;

    clearTableList();
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filePath);
    if (db.open())
    {
        dbPath = filePath;
        tableListModel->setStringList(db.tables());
        delete tableView->model();
        QSqlTableModel *tableModel = new QSqlTableModel(this,db);
        tableView->setModel(tableModel);
    }
}

void MainWindow::clearTableList()
{
    clearTableView();
}

void MainWindow::clearTableView()
{

}
