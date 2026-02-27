/*****************************************************************************
** ReadFlashThread.h 
** Author: Kraku
*****************************************************************************/
#ifndef _READFLASHTHREAD_H_
#define _READFLASHTHREAD_H_

#include <QThread>
#include "AbstractPort.h"

class ReadFlashThread : public QThread
{
    Q_OBJECT public:
    ReadFlashThread() = default;
    virtual void run();
    bool end{};
    FILE* file{};
    char mbc{};
    char algorythm{};
    char dap{};
    int page_count{};
    AbstractPort* port{};

public slots:
    void canceled();

signals:
    void set_progress(int ile, int max);
    void error(int err);
};

#endif
