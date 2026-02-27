/*****************************************************************************
** About.h
** Author: Kraku
*****************************************************************************/
#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QPushButton>
#include <QPixmap>
#include <QLabel>
#include <QBoxLayout>
#include <QGroupBox>

class About:public QDialog
{
  Q_OBJECT QPushButton * ok_btn;
  QLabel *image;
  QVBoxLayout *all;
  QGroupBox *box;
  QLabel *name, *copy, *desc1, *desc2, *email, *warning, *translator;
  QVBoxLayout *labels;
public:
  explicit About(QWidget* parent = nullptr);
};

#endif
