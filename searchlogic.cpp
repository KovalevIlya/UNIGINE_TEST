#include "searchlogic.h"
#include "global.h"

#include <QtConcurrent/QtConcurrent>
#include <QDir>
#include <QMutexLocker>
#include <QApplication>

#include <algorithm>
#include <utility>
#include <exception>

bool SearchLogic::_stop = false;

SearchLogic::SearchLogic(QObject *parent) : QObject(parent)
{
    _threadPool = new QThreadPool(this);
}

void SearchLogic::selectFiles()
{
    auto compare = [](std::pair<QString, int> left, std::pair<QString, int> right) {
        return left.second > right.second;
    };


    QList<std::pair<QString, int>> files;
    for (auto i = _files.keyValueBegin(); i != _files.keyValueEnd(); ++i) {
        files.append(*i);
    }

    std::sort(files.begin(), files.end(), compare);

    QVector<QPair<QString, int>> result;
    result.reserve(countDisplayedFiles);

    for (int i = 0; i < files.count(), i < countDisplayedFiles; ++i) {
        result.append({files[i].first, files[i].second});
    }

    emit finishSearch(result);
}

void SearchLogic::startSearch(const QString &startDir)
{
    _stop = false;
    _files.clear();

    auto _globalFuture = QtConcurrent::run(_threadPool, this, &SearchLogic::search, startDir);

    while (!_globalFuture.isFinished())
        QApplication::processEvents();

    selectFiles();
}

void SearchLogic::stopSearch()
{
    _stop = true;
    selectFiles();
}

void SearchLogic::addFile(const QString &fileName)
{
    QMutexLocker mutex(&_mutex);

    if (_stop)
        return;

    _files.insert(fileName, _files.value(fileName, 0) + 1);
}

void SearchLogic::search(const QString &startDir)
{
    QDir dir(startDir);

    QList<QFuture<void>> futures;

    auto files = dir.entryInfoList();

    QFutureSynchronizer<void> synchronizer;
    for (auto file : files) {
        if (file.fileName() == ".." || file.fileName() == ".")
            continue;

        if (_stop)
            return;

        if (file.isDir() & !file.isSymbolicLink()) {
            synchronizer.addFuture(QtConcurrent::run(_threadPool, this,
                                                     &SearchLogic::search, file.filePath()));
        }

        addFile(file.fileName());
    }

    synchronizer.waitForFinished();
}
