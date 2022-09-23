#include "searchdialog.h"

#include <QProgressBar>
#include <QHBoxLayout>
#include <QLabel>


SearchDialog::SearchDialog()
    : QProgressDialog("Searching for files", "Stop searching", 0, 0)
{
    auto bar = new QProgressBar;
    bar->setRange(0, 0);

    _lProgress = new QLabel("files found: 0");

    QHBoxLayout *layout = new QHBoxLayout(bar);
    layout->addWidget(_lProgress);
    layout->setContentsMargins(5, 0, 5, 0);

    setBar(bar);
    show();
}

void SearchDialog::progressIncrement()
{
    _lProgress->setText(QString("files found: %1").arg(++_countFiles));
}
