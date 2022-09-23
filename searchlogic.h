#ifndef SEARCHLOGIC_H
#define SEARCHLOGIC_H

#include <QObject>
#include <QFuture>
#include <QMutex>

class QThreadPool;

class SearchLogic : public QObject
{
    Q_OBJECT
public:
    explicit SearchLogic(QObject *parent = nullptr);

    void selectFiles();

    Q_SLOT void startSearch(const QString &startDir);
    Q_SLOT void stopSearch();

    Q_SIGNAL void finishSearch(const QVector<std::pair<QString, int>> &);

private:
    void addFile(const QString &fileName);
    void search(const QString &startDir);

    static bool _stop;
    QMutex _mutex;
    QFuture<void> _globalFuture;
    QThreadPool *_threadPool;
    QHash<QString, int> _files;
};

#endif // SEARCHLOGIC_H
