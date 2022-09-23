#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QProgressDialog>

class SearchDialog : public QProgressDialog
{
    Q_OBJECT
public:
    SearchDialog();

    Q_SLOT void progressIncrement();

private:
    int _countFiles = 0;
    QLabel *_lProgress;
};

#endif // SEARCHDIALOG_H

