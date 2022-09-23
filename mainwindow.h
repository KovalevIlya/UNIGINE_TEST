#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTableWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Q_SLOT void setFiles(const QVector<std::pair<QString, int>> &files);

    Q_SIGNAL void startSearch(const QString &startDir);

private:
    QWidget *makeCentralWidget();

    QTableWidget *_tabel;
    QString _dirName;
};
#endif // MAINWINDOW_H
