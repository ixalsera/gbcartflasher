/*****************************************************************************
** Gui.h - class of main dialog window
** Author: Kraku
*****************************************************************************/
#ifndef _GUI_H_
#define _GUI_H_

#include <QWidget>
#include <QProgressBar>
#include <QString>
#include "Settings.h"
#include "Console.h"
#include "About.h"

#include "ReadFlashThread.h"
#include "WriteFlashThread.h"
#include "EraseThread.h"
#include "ReadRamThread.h"
#include "WriteRamThread.h"

class Gui : public QWidget
{
    Q_OBJECT Settings* settings;
    Console* console;
    QGridLayout* grid;
    QVBoxLayout* left;
    QVBoxLayout* right;
    QVBoxLayout* center;
    QHBoxLayout* down;
    QProgressBar* progress;
    QLabel* image;
    QPixmap* logo{};
    QPushButton* cancel_btn;
    QPushButton* status_btn;
    QPushButton* rflash_btn;
    QPushButton* wflash_btn;
    QPushButton* rram_btn;
    QPushButton* wram_btn;
    QPushButton* eflash_btn;
    QPushButton* eram_btn;
    QPushButton* about_btn;
    QString file_name;
    QString path;
    About about_dlg;
    WriteFlashThread* thread_WFLA;
    ReadFlashThread* thread_RFLA;
    EraseThread* thread_E;
    ReadRamThread* thread_RRAM;
    WriteRamThread* thread_WRAM;

public:
    explicit Gui(QWidget* parent = nullptr);
    static int port_type;

public
    slots :
    
    void startup_info() const;
    void setEnabledButtons(bool state) const;
    void setRamButtons() const;
    void print_error(int err) const;
    void write_flash();
    void read_flash();
    void read_ram();
    void write_ram();
    void show_info() const;
    void erase_flash() const;
    void erase_ram() const;
    void about();
    void setProgress(int ile, int max) const;
    static AbstractPort* create_port();
};

#endif