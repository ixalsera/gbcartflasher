/*****************************************************************************
** Settings.h - configuration widget
** Author: Kraku
*****************************************************************************/
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QGroupBox>
#include <QComboBox>
#include <QLabel>
#include <QLayout>
#include <QString>
#include <QCheckBox>
#include <QThread>
#include "const.h"

class Settings : public QGroupBox
{
    Q_OBJECT mbc_t mbc;
    QString com_name;
    int flash_size;
    int ram_size;
    bool auto_size;
    QComboBox* com_combo;
    QComboBox* mbc_combo;
    QComboBox* flash_combo;
    QComboBox* ram_combo;
    QComboBox* lang_combo;
    QLabel* com_label;
    QLabel* mbc_label;
    QLabel* flash_label;
    QLabel* ram_label;
    QLabel* lang_label;
    QCheckBox* auto_check;
    QVBoxLayout *labels, *combo_boxes;
    QHBoxLayout *north, *down;
    QVBoxLayout* all;

public:
    static bool commanual;
    static alg_t algorythm;
    static dap_t dap;
    static bool showbbl;
    static speed_type speed;
    static QThread::Priority priority;


    explicit Settings(QWidget* parent = nullptr);

    [[nodiscard]] int getFlash() const
    {
        return flash_size;
    }

    [[nodiscard]] int getRam() const
    {
        return ram_size;
    }

    QString getCom()
    {
        return com_name;
    }

    [[nodiscard]] QString getCom(const int index) const
    {
        return com_combo->itemText(index);
    }

    [[nodiscard]] mbc_t getMbc() const
    {
        if (mbc == MBC5 || mbc == RUMBLE)
            return MBC5;

        return mbc;
    }

    [[nodiscard]] bool isAuto() const
    {
        return auto_size;
    }

    [[nodiscard]] bool isRamDisabled() const
    {
        return (ram_combo->currentText() == "");
    }

    void ram_types(int type) const;
    void flash_types(int type) const;
    void get_langs() const;

public slots:
    void setCom(int index);
    void setMbc(int mbc_nr);
    void setFlash(int index);
    void setRam(int index);
    void setLang(const QString& lang);
    void setAuto(int state);
signals:
    void refresh_ram_buttons();
};

#endif
