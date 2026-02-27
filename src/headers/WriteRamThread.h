/*****************************************************************************
** WriteRamThread.h
** Author: Kraku
*****************************************************************************/
#ifndef WRITERAMTHREAD_H
#define WRITERAMTHREAD_H

#include <QThread>
#include "AbstractPort.h"

class WriteRamThread : public QThread
{
    Q_OBJECT public:
    WriteRamThread() = default;

    virtual void run();
    FILE* file{};
    char mbc{};
    char algorythm{};
    char dap{};
    int page_count{};
    bool end{};
    AbstractPort* port{};
    char _2k{};

public slots:
    void canceled();
signals:
    void set_progress(int ile, int max);
    void error(int err);
};

#endif
