#include "t4good_minibal.h"
#include "ui_t4good_minibal.h"
#include <qinputdialog.h>
#include <iostream>
#include "Tfile_helper.h"
#include "paths.h"
//**************************************************************************
T4good_minibal::T4good_minibal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4good_minibal)
{
    ui->setupUi(this);
    init();
}
//**************************************************************************
T4good_minibal::~T4good_minibal()
{
    delete ui;
}
//**************************************************************************
void T4good_minibal::on_push_default_clicked()
{
set_default();
}
//**************************************************************************
void T4good_minibal::init()
{

  set_default();

  // reading from the file
  string pname = gpath.options + "good_miniball.txt" ;
  ifstream plik(pname.c_str());
  if(!plik)
  {
    cout << "Cant open file " << pname << endl;
    return ;

  }

  try{

    // reading
    bool flag ;

    flag = (bool) Nfile_helper::find_in_file(plik, "increment_segment_energy_cal_with_zero") ;
    ui->checkBox_call_req_energy->setChecked(flag);

    flag = (bool) Nfile_helper::find_in_file(plik, "increment_time_cal_with_zero") ;
    ui->checkBox_call_req_tim->setChecked(flag);

  //----------

    //-------
    flag = (bool) Nfile_helper::find_in_file(plik, "good_core_energy_requires_threshold") ;
    ui->checkBox_good_core_energy->setChecked(flag);

    double ddd =  Nfile_helper::find_in_file(plik, "core_energy_threshold") ;
    ui->pushline_core_energy_good_threshold->setText(QString::number(ddd));
    ui->pushline_core_energy_good_threshold->setEnabled(flag);

    // time
    flag = (bool) Nfile_helper::find_in_file(plik, "good_time_requires_threshold") ;
    ui->checkBox_good_time->setChecked(flag);


    ddd =  Nfile_helper::find_in_file(plik, "good_time_threshold") ;
    ui->push_time_threshold->setText(QString::number(ddd));
    ui->push_time_threshold->setEnabled(flag);

  }
  catch(Tfile_helper_exception k)
  {
    cout << "Error while reading " << pname << "\n" << k.message << endl;
  }

}
//*********************************************************************
void T4good_minibal::accept()
{
  // save to the file
  // reading from the file
  string pname = gpath.options + "good_miniball.txt" ;
  ofstream plik(pname.c_str());
  if(!plik)
  {


  }


    // writing
  plik
    << "// This !!!! file contains specification about good miniball signals\n"

    << "\nincrement_segment_energy_cal_with_zero\t"
    << ui->checkBox_call_req_energy->isChecked()

    << "\nincrement_time_cal_with_zero\t"
    << ui->checkBox_call_req_tim->isChecked()

    //----------

    //-------
    << "\n\ngood_core_energy_requires_threshold\t"
    <<  ui->checkBox_good_core_energy->isChecked()

    << "\ncore_energy_threshold\t"
    << ui->pushline_core_energy_good_threshold->text().toStdString()

    // time
    << "\n\ngood_time_requires_threshold\t"
    <<  ui->checkBox_good_time->isChecked()

    << "\n\ngood_time_threshold\t"
    <<  ui->push_time_threshold->text().toStdString()
    << endl;


  QDialog::accept();
}
//*********************************************************************
void T4good_minibal::set_default()
{
  ui->checkBox_call_req_energy->setChecked(false);
  ui->checkBox_call_req_tim->setChecked(false);

  ui->checkBox_good_core_energy->setChecked(true);
  ui->pushline_core_energy_good_threshold->setText("100");
  ui->pushline_core_energy_good_threshold->setEnabled(true);

  ui->checkBox_good_time->setChecked(false);
  ui->push_time_threshold->setText("0");
  ui->push_time_threshold->setEnabled(false);

}
//***************************************************************
void T4good_minibal::set_en4_thresh()
{
    bool ok = false ;
    double value  = QInputDialog::getDouble
                          ( this, "Type the value of threshold for signal 'core energy'",
                            "threshold ",
                ui->pushline_core_energy_good_threshold->text().toDouble(),
                0.0, 4096, 1, &ok) ;
     if(ok)
     {
         ui->pushline_core_energy_good_threshold->setText(QString("%1").arg(value));
     }
}
//*****************************************************************
void T4good_minibal::set_time_threshold()
{
    bool ok = false ;
    double value  = QInputDialog::getDouble
                          (this, "Type the value of threshold for signal 'time'",
                            "threshold ",
                ui->push_time_threshold->text().toDouble() ,
                0.0, 4096, 1, &ok) ;
     if(ok)
     {
         ui->push_time_threshold->setText(QString("%1").arg(value));
     }
}
//********************************************************************

void T4good_minibal::on_pushline_core_energy_good_threshold_clicked()
{
    set_en4_thresh();
}
//********************************************************************
void T4good_minibal::on_push_time_threshold_clicked()
{
    set_time_threshold();
}
//********************************************************************
void T4good_minibal::on_checkBox_good_core_energy_clicked(bool checked)
{
    ui->pushline_core_energy_good_threshold->setEnabled(checked);
}
//********************************************************************
void T4good_minibal::on_checkBox_good_time_clicked(bool checked)
{
    ui->push_time_threshold->setEnabled(checked);
}
