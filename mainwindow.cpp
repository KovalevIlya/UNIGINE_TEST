#include "mainwindow.h"
#include "global.h"
#include "searchlogic.h"

#include <QApplication>
#include <QFileDialog>
#include <QPushButton>
#include <QLayout>
#include <QLabel>
#include <QGroupBox>
#include <QTableWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _dirName(qApp->applicationDirPath())
{
    setCentralWidget(makeCentralWidget());

    auto searchLogic = new SearchLogic(this);
    connect(this, &MainWindow::startSearch, searchLogic, &SearchLogic::startSearch);

    connect(searchLogic, &SearchLogic::finishSearch, this, &MainWindow::setFiles);
    connect(qApp, &QGuiApplication::lastWindowClosed, searchLogic, &SearchLogic::stopSearch);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setFiles(const QVector<QPair<QString, int>> &files)
{
    auto countFiles = files.count();
    for (int i = 0; i < countDisplayedFiles, i < countFiles; ++i) {
        _tabel->item(i, 0)->setText(files[i].first);
        _tabel->item(i, 1)->setText(QString("%1 count").arg(files[i].second));
    }
}

QWidget *MainWindow::makeCentralWidget()
{
    auto lDirName = new QLabel(_dirName);

    auto gbDirName = new QGroupBox("Selected directory");
    gbDirName->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    auto layDirName = new QVBoxLayout(gbDirName);
    layDirName->addWidget(lDirName);

    auto pbSelectDir = new QPushButton("Select directory");

    connect(pbSelectDir, &QPushButton::clicked, this, [this, lDirName]() {
        _dirName = QFileDialog::getExistingDirectory(this, "Select directory", _dirName,
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
        lDirName->setText(_dirName);
    });

    auto pbSearchFiles = new QPushButton("Search files");
    connect(pbSearchFiles, &QPushButton::clicked, this, [this]() {
        startSearch(_dirName);
    });

    auto line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    _tabel = new QTableWidget(countDisplayedFiles, 2);
    _tabel->setHorizontalHeaderLabels({"File name", "Count files"});
    _tabel->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for (int i = 0; i < countDisplayedFiles; ++i) {
        _tabel->setItem(i, 0, new QTableWidgetItem);

        auto itemCountFiles = new QTableWidgetItem;
        itemCountFiles->setText("? count");
        _tabel->setItem(i, 1, itemCountFiles);
    }

    auto widget = new QWidget;
    auto lay = new QVBoxLayout(widget);
    lay->addWidget(gbDirName);
    lay->addWidget(pbSelectDir);
    lay->addWidget(pbSearchFiles);
    lay->addWidget(line);
    lay->addWidget(_tabel);

    return widget;
}

