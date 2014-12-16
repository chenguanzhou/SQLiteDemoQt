#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QFileSystemModel;
class QStringListModel;
class QItemSelection;
class QTableView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void fileChanged(const QItemSelection &selected,const QItemSelection &deselected);
    void tableChanged(const QItemSelection &selected,const QItemSelection &deselected);

    void setDataBase(const QString &filePath);
    void clearTableList();
    void clearTableView();

private:
    QTableView          *tableView;
    QFileSystemModel    *fileSystemModel;
    QStringListModel    *tableListModel;
    QString dbPath;
};

#endif // MAINWINDOW_H
