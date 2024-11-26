#include "t4other_spy_options.h"
#include "ui_t4other_spy_options.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std ;

#include "paths.h"
#include "Tfile_helper.h"


//****************************************************************************
T4other_spy_options::T4other_spy_options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4other_spy_options)
{
    ui->setupUi(this);
    init();
}
//****************************************************************************
T4other_spy_options::~T4other_spy_options()
{
    delete ui;
}
//***************************************************************************
void T4other_spy_options::accept()
{
  ofstream plik(( gpath.options + "other_options.dat").c_str());

  plik << "autosave_period_in_minutes\t" <<  ui->spinBox_autosave_time->value() ;

  QDialog::accept();
}
//**************************************************************************
void T4other_spy_options::init()
{
  ui->spinBox_autosave_time->setValue(60);

  ifstream plik(( gpath.options + "other_options.dat").c_str());
  if(!plik)
  {
    cout << "Cant open other_options.dat file" << endl;
    return ;
  }

  try
  {
    int t = (int) Nfile_helper::find_in_file(plik, "autosave_period_in_minutes") ;
    ui->spinBox_autosave_time->setValue(t);
  }
  catch(...)
  {
    cout << "Exception while reading other_options.dat file " << endl;
  }

}

void T4other_spy_options::on_buttonOk_clicked()
{
    accept();
}
