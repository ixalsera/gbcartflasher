/*****************************************************************************
** WriteFlashThread.h
** Author: Kraku
*****************************************************************************/
#ifndef WRITEFLASHTHREAD_H
#define WRITEFLASHTHREAD_H

#include <QThread>
#include "AbstractPort.h"

class WriteFlashThread : public QThread
{
    Q_OBJECT public:
    WriteFlashThread() = default;
    virtual void run();
    FILE* file{};
    char mbc{};
    char algorythm{};
    char dap{};
    int page_count{};
    bool end{};
    AbstractPort* port{};

public slots:
    void canceled();
signals:
    void set_progress(int ile, int max);
    void error(int err);
};

#endif
