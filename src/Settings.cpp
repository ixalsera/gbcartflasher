/*****************************************************************************
** Settings.cpp
** Author: Kraku
*****************************************************************************/
#include <cmath>
#include <QLabel>
#include <QMessageBox>
#include <QSettings>
#include "headers/Settings.h"
#include "headers/Gui.h"

bool Settings::commanual = false;
alg_t Settings::algorythm = ALG16;
dap_t Settings::dap = TOGGLE;
bool Settings::showbbl = false;
speed_type Settings::speed = STANDARD;
QThread::Priority Settings::priority = QThread::NormalPriority;

Settings::Settings(QWidget* parent) : QGroupBox(tr("Settings"), parent)
{
    const QSettings set;

    labels = new QVBoxLayout();
    combo_boxes = new QVBoxLayout();
    down = new QHBoxLayout();
    north = new QHBoxLayout();
    all = new QVBoxLayout();


    com_label = new QLabel("PORT:", this);
    labels->addWidget(com_label);
    com_combo = new QComboBox(this);
    combo_boxes->addWidget(com_combo);

#ifdef Q_OS_LINUX
    com_combo->insertItem(0, "/dev/ttyS0");
    com_combo->insertItem(1, "/dev/ttyS1");
    com_combo->insertItem(2, "/dev/ttyS2");
    if (set.contains("user_com"))
    {
        com_combo->insertItem(3, set.value("user_com").toString());
        com_combo->setCurrentIndex(3);
    }
    else
    {
        // /dev/flasher can be symbolic link to device if it's connected to some strange port
        com_combo->insertItem(3, "/dev/flasher");
    }
#endif

#ifdef Q_OS_WIN
    com_combo->insertItem(0, "COM1");
    com_combo->insertItem(1, "COM2");
    com_combo->insertItem(2, "COM3");
    if (set.contains("user_com"))
    {
        com_combo->insertItem(3, set.value("user_com").toString());
        com_combo->setCurrentIndex(3);
    }
    else
    {
        com_combo->insertItem(3, "COM4");
    }
#endif

    com_combo->insertItem(4, "USB");

    mbc_label = new QLabel("MBC:", this);

    labels->addWidget(mbc_label);
    mbc_combo = new QComboBox(this);
    combo_boxes->addWidget(mbc_combo);
    // TODO: Grab these from our declared list in Logic (or split out)
    mbc_combo->insertItem(0, "MBC AUTO");
    mbc_combo->insertItem(1, "MBC1");
    mbc_combo->insertItem(2, "MBC2");
    mbc_combo->insertItem(3, "MBC3");
    mbc_combo->insertItem(4, "ROM ONLY");
    mbc_combo->insertItem(5, "MBC5");
    mbc_combo->insertItem(6, "RUMBLE");

    flash_label = new QLabel("FLASH:", this);
    labels->addWidget(flash_label);
    flash_combo = new QComboBox(this);
    combo_boxes->addWidget(flash_combo);
    // TODO: Grab these from our declared list in Logic (or split out)
    flash_combo->insertItem(0, "32 KB");
    flash_combo->insertItem(1, "64 KB");
    flash_combo->insertItem(2, "128 KB");
    flash_combo->insertItem(3, "256 KB");
    flash_combo->insertItem(4, "512 KB");
    flash_combo->insertItem(5, "1024 KB");
    flash_combo->insertItem(6, "2048 KB");
    flash_combo->insertItem(7, "4096 KB");
    flash_combo->insertItem(8, "8192 KB");
    flash_combo->setCurrentIndex(4);

    ram_label = new QLabel("RAM:", this);
    labels->addWidget(ram_label);
    ram_combo = new QComboBox(this);
    combo_boxes->addWidget(ram_combo);
    // TODO: Grab these from our declared list in Logic (or split out)
    ram_combo->insertItem(0, "2 KB");
    ram_combo->insertItem(1, "8 KB");
    ram_combo->insertItem(2, "32 KB");
    ram_combo->insertItem(3, "128 KB");
    ram_combo->setCurrentIndex(1);

    lang_label = new QLabel(tr("LANGUAGE:"), this);
    labels->addWidget(lang_label);
    lang_combo = new QComboBox(this);
    combo_boxes->addWidget(lang_combo);
    get_langs();


    auto_check = new QCheckBox(tr("Check file size"), this);
    auto_check->setCheckState(Qt::Checked);
    down->addWidget(auto_check);


    north->addLayout(labels);
    north->addLayout(combo_boxes);
    all->addLayout(north);
    all->addLayout(down);
    setLayout(all);

    auto_size = true;
    mbc = MBCAUTO;
    ram_size = 8;
    flash_size = 512;
    com_name = com_combo->currentText();
    Gui::port_type = SERIAL;


    connect(com_combo, SIGNAL(activated(int)), this, SLOT(setCom(int)));
    connect(mbc_combo, SIGNAL(activated(int)), this, SLOT(setMbc(int)));
    connect(flash_combo, SIGNAL(activated(int)), this,
            SLOT(setFlash(int)));
    connect(ram_combo, SIGNAL(activated(int)), this, SLOT(setRam(int)));
    connect(lang_combo, SIGNAL(activated(QString)), this,
            SLOT(setLang(QString)));
    connect(auto_check, SIGNAL(stateChanged(int)), this,
            SLOT(setAuto(int)));
}

void Settings::setCom(const int index)
{
    com_name = getCom(index);
    com_combo->setCurrentIndex(index);
    if (index < PORTS_COUNT)
        Gui::port_type = SERIAL;
    else
        Gui::port_type = USB;
}

void Settings::setFlash(const int index)
{
    /* size = 32 * 2 ^ index */
    flash_size = 32 * static_cast<int>(pow(2.0, index));
}

void Settings::setRam(const int index)
{
    switch (index)
    {
    case 0:
        ram_size = 2;
        break;
    case 1:
        ram_size = 8;
        break;
    case 2:
        ram_size = 32;
        break;
    case 3:
        ram_size = 128;
        break;
    }
}

void Settings::setMbc(int mbc_nr)
{
    mbc = static_cast<mbc_t>(mbc_nr);
    switch (mbc)
    {
    case MBCAUTO:
        ram_types(4);
        flash_types(8);
        break;
    case MBC1:
        ram_types(3);
        flash_types(6);
        break;
    case MBC2:
        ram_types(0);
        flash_types(3);
        break;
    case MBC3:
        ram_types(3);
        flash_types(6);
        break;
    case ROMONLY:
        ram_types(0);
        flash_types(0);
        break;
    case MBC5:
        ram_types(4);
        flash_types(8);
        break;
    case RUMBLE:
        ram_types(3);
        flash_types(8);
        break;
    }
    setFlash(flash_combo->currentIndex());
    setRam(ram_combo->currentIndex());
    emit refresh_ram_buttons();
}

//fill ram_combo depending on cart type
void Settings::ram_types(const int type) const
{
    const int ile = ram_combo->count();
    for (int i = ile - 1; i >= 0; i--)
        ram_combo->removeItem(i);
    switch (type)
    {
    case 0:
        break;
    case 3:
        ram_combo->insertItem(0, "2 KB");
        ram_combo->insertItem(1, "8 KB");
        ram_combo->insertItem(2, "32 KB");
        break;
    case 4:
        ram_combo->insertItem(0, "2 KB");
        ram_combo->insertItem(1, "8 KB");
        ram_combo->insertItem(2, "32 KB");
        ram_combo->insertItem(3, "128 KB");
        break;
    }
}

void Settings::flash_types(const int type) const
{
    const int count = flash_combo->count();
    for (int i = count - 1; i >= 0; i--)
        flash_combo->removeItem(i);

    for (int i = 0; i <= type; i++)
    {
        const int capacity = 32 * static_cast<int>(pow(2.0, i));
        flash_combo->insertItem(i, QString::number(capacity) + " KB");
    }
}

void Settings::get_langs() const
{
    QSettings settings;
    int selectedIndex = 0;
    const QString selectedLang = settings.value("selected_lang").toString();
    settings.beginGroup("lang");
    const QStringList keys = settings.childKeys();

    for (int i = 0; i < keys.size(); ++i)
    {
        lang_combo->insertItem(i, settings.value(keys.at(i)).toString());
        if (selectedLang == keys.at(i))
            selectedIndex = i;
    }
    settings.endGroup();
    lang_combo->setCurrentIndex(selectedIndex);
}

void Settings::setLang(const QString& lang)
{
    QSettings settings;
    settings.beginGroup("lang");
    QString langFileName = settings.value("selected_lang").toString();
    QStringList keys = settings.childKeys();
    for (const auto& key : keys)
    {
        if (lang == settings.value(key).toString())
            langFileName = key;
    }
    settings.endGroup();
    settings.setValue("selected_lang", langFileName);

    QMessageBox::information(this, tr("Change language"),
                             tr
                             ("After program restart language will be changed to: ")
                             + lang, QMessageBox::Ok);
}

void Settings::setAuto(const int state)
{
    if (state == Qt::Checked)
        auto_size = true;
    else
        auto_size = false;
}
