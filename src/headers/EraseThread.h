/*****************************************************************************
** EraseThread.h - Header file of thread responsible for process of 
** FLASH and RAM erase
** Author: Kraku
*****************************************************************************/
#ifndef ERASETHREAD_H
#define ERASETHREAD_H

#include <QThread>
#include "AbstractPort.h"

class EraseThread : public QThread
{
    Q_OBJECT

public:
    char mem; /* memory type for erase */
    char mbc; /* Memory Bank Controler type */
    char par;
    char dap;
    bool end; /* end of thread marker */
    AbstractPort* port;
    /* main point of thread  */
    virtual void run();

public slots:
    void canceled() { end = true; }
signals:
    void set_progress(int ile, int max);
    void error(int err);
};

#endif
