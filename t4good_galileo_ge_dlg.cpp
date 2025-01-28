#include "t4good_galileo_ge_dlg.h"
#include "ui_t4good_galileo_ge_dlg.h"
#include <qinputdialog.h>
#include <string>
#include <QFileDialog>
#include <QMessageBox>

using namespace std;
#include "paths.h"
#include <fstream>
#include <iostream>
#include "Tfile_helper.h"
#include "tplate_spectrum.h"
//***************************************************************************************************
T4good_galileo_ge_dlg::T4good_galileo_ge_dlg(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::T4good_galileo_ge_dlg)
{
  ui->setupUi(this);
  ui->push_banana->setEnabled(false);
  init();
  ui->tabWidget->setCurrentIndex(2);

  //ui->label_info_polygon->font(). pointSize(5);
}
//***************************************************************************************************
T4good_galileo_ge_dlg::~T4good_galileo_ge_dlg()
{
  delete ui;
}
//***************************************************************************************************
void T4good_galileo_ge_dlg::on_buttonBox_accepted()
{
  // save to the file
  // reading from the file
  string pname = gpath.options + "good_galileo_germanium.txt" ;
  ofstream plik(pname.c_str());
  if(!plik)
    {


    }

  // writing
  plik
      << "// This !!!! file contains specification about good germanium signals\n"


         //----------

         //-------
      << "\n\ngood_energy_requires_threshold\t"
      <<  ui->checkBox_good_energy->isChecked()

       << "\nenergy_threshold_lower\t"
       << ui->pushline_energy_good_threshold_lower->text().toStdString()

       << "\nenergy_threshold_upper\t"
       << ui->pushline_energy_good_threshold_upper->text().toStdString()

          // time
       << "\n\ngood_time_requires_threshold\t"
       <<  ui->checkBox_good_time->isChecked()

        << "\n\ngood_time_threshold_lower\t"
        <<  ui->push_time_threshold_lower->text().toStdString()

         << "\n\ngood_time_threshold_upper\t"
         <<  ui->push_time_threshold_upper->text().toStdString();

  plik       << "\n\npolygon_wEnergy_vs_eEnergy\t "
             << ui->checkBox_wEn_vs_eEn_banana->isChecked()
             << "\t"
             << ui->push_banana->text().toStdString()
             << endl;

  if(!plik)ui->push_banana->setEnabled(false);

  QDialog::accept();
}
//***************************************************************************************************
void T4good_galileo_ge_dlg::init()
{
  on_push_default_clicked();

  ui->push_banana->setText("click to choose a polygon gate");


  // reading from the file
  string pname = gpath.options + "good_galileo_germanium.txt" ;
  ifstream plik(pname.c_str());
  if(!plik)
    {
      cout << "Cant open file " << pname << endl;
      return ;
    }

  try{

    // reading
    bool flag ;

    //----------

    bool flag_energy = (bool) Nfile_helper::find_in_file(plik, "good_energy_requires_threshold") ;
    ui->checkBox_good_energy->setChecked(flag_energy);

    double ddd =  Nfile_helper::find_in_file(plik, "energy_threshold_lower") ;
    ui->pushline_energy_good_threshold_lower->setText(QString::number(ddd));
    ui->pushline_energy_good_threshold_lower->setEnabled(flag_energy);

    // time
    flag = (bool) Nfile_helper::find_in_file(plik, "good_time_requires_threshold") ;
    ui->checkBox_good_time->setChecked(flag);

    ddd =  Nfile_helper::find_in_file(plik, "good_time_threshold_lower") ;
    ui->push_time_threshold_lower->setText(QString::number(ddd));
    ui->push_time_threshold_lower->setEnabled(flag);

    ddd =  Nfile_helper::find_in_file(plik, "good_time_threshold_upper") ;
    ui->push_time_threshold_upper->setText(QString::number(ddd));
    ui->push_time_threshold_upper->setEnabled(flag);

    ddd =  Nfile_helper::find_in_file(plik, "energy_threshold_upper") ;
    ui->pushline_energy_good_threshold_upper->setText(QString::number(ddd));
    ui->pushline_energy_good_threshold_upper->setEnabled(flag_energy);


    ddd =  Nfile_helper::find_in_file(plik, "polygon_wEnergy_vs_eEnergy") ;
    ui->checkBox_wEn_vs_eEn_banana->setChecked(ddd);
    ui->push_banana->setEnabled(ddd);

    string pol_name;
    plik >> pol_name ;
    ui->push_banana->setText(pol_name.c_str());



  }
  catch(Tfile_helper_exception k)
  {
    cout << "Error while reading " << pname << "\n" << k.message << endl;
  }

}
//***************************************************************************************************
void T4good_galileo_ge_dlg::on_push_default_clicked()
{
  ui->checkBox_good_energy->setChecked(true);
  ui->pushline_energy_good_threshold_lower->setText("100");
  ui->pushline_energy_good_threshold_lower->setEnabled(true);

  ui->pushline_energy_good_threshold_upper->setText("8192");
  ui->pushline_energy_good_threshold_upper->setEnabled(true);

  ui->checkBox_good_time->setChecked(false);
  ui->push_time_threshold_lower->setText("0");
  ui->push_time_threshold_lower->setEnabled(false);

  ui->push_time_threshold_upper->setText("99999");
  ui->push_time_threshold_upper->setEnabled(false);

  ui->checkBox_wEn_vs_eEn_banana->setChecked(false);
  ui->push_banana->setText("no_polygon");
}
//***************************************************************************************************
//void T4good_galileo_ge_dlg::on_pushline_energy_good_threshold_clicked()
//{
//  bool ok = false ;
//  double value  = QInputDialog::getDouble
//      ( this, "Type the value of lower threshold for signal 'energy energy'",
//        "Lower threshold: ",
//        ui->pushline_energy_good_threshold_lower->text().toDouble(),
//        0.0, 8192, 1, &ok) ;
//  if(ok)
//    {
//      ui->pushline_energy_good_threshold_lower->setText(QString("%1").arg(value));
//    }
//}
//***************************************************************************************************
void T4good_galileo_ge_dlg::on_pushline_energy_good_threshold_upper_clicked()
{
  bool ok = false ;
  double value  = QInputDialog::getDouble
      ( this, "Type the value of upper_threshold for signal 'energy energy'",
        "Upper threshold: ",
        ui->pushline_energy_good_threshold_upper->text().toDouble(),
        0.0, 8*8192, 1, &ok) ;
  if(ok)
    {
      ui->pushline_energy_good_threshold_upper->setText(QString("%1").arg(value));
    }
}
//***************************************************************************************************
void T4good_galileo_ge_dlg::on_push_time_threshold_lower_clicked()
{
  bool ok = false ;
  double value  = QInputDialog::getDouble
      ( this, "Type the value of the lower threshold for signal 'time'",
        "Lower threshold: ",
        ui->push_time_threshold_lower->text().toDouble() ,
        0.0, 30000, 1, &ok) ;
  if(ok)
    {
      ui->push_time_threshold_lower->setText(QString("%1").arg(value));
    }
}
//***************************************************************************************************
void T4good_galileo_ge_dlg::on_push_time_threshold_upper_clicked()
{
  bool ok = false ;
  double value  = QInputDialog::getDouble
      ( this, "Type the value of the upper threshold for signal 'time'",
        "Upper threshold ",
        ui->push_time_threshold_upper->text().toDouble() ,
        0.0, 32000, 1, &ok) ;
  if(ok)
    {
      ui->push_time_threshold_upper->setText(QString("%1").arg(value));
    }
}
//***************************************************************************************************
void T4good_galileo_ge_dlg::on_checkBox_good_energy_clicked(bool checked)
{
  ui->pushline_energy_good_threshold_lower->setEnabled(checked);
  ui->pushline_energy_good_threshold_upper->setEnabled(checked);
}
//***************************************************************************************************
void T4good_galileo_ge_dlg::on_checkBox_good_time_clicked(bool checked)
{
  ui->push_time_threshold_lower->setEnabled(checked);
  ui->push_time_threshold_upper->setEnabled(checked);
}
//***************************************************************************************************
void T4good_galileo_ge_dlg::on_pushline_energy_good_threshold_lower_clicked()
{
  bool ok = false ;
  double value  = QInputDialog::getDouble
      ( this, "Type the value of lower threshold for signal 'energy energy'",
        "Lower threshold: ",
        ui->pushline_energy_good_threshold_lower->text().toDouble(),
        0.0, 8192, 1, &ok) ;
  if(ok)
    {
      ui->pushline_energy_good_threshold_lower->setText(QString("%1").arg(value));
    }
}
//*************************************************************************************
void T4good_galileo_ge_dlg::on_push_banana_clicked()
{

  QString filter;
  Tplate_spectrum::flag_repainting_allowed = false;
  QString fileName =
      QFileDialog::getOpenFileName
      (  this,
         tr ( "Dialog for selecting the polygon for selfgate - what is it?" ),
         gpath.polygons.c_str(),
         tr ( "polygon gate  (*.poly)" ),
         &filter
         );
  Tplate_spectrum::flag_repainting_allowed = true;
  //cout << "Nazwa " << fileName << endl ;
  if(fileName.isEmpty() == false)
    {
      // we want to remove the path
      string naked_name = fileName.toStdString();
      string::size_type pos_found = naked_name.rfind("/");
      if(pos_found !=string::npos)
        {
          naked_name.erase(0, pos_found + 1);
        }
      pos_found = naked_name.rfind("_wEnergy_vs_eEnergy"); // strip the Ge_NR
      if(pos_found !=string::npos)
        {
          naked_name.erase(0, pos_found);  // polygon_ has 7 letters
        }

      pos_found = naked_name.rfind(".poly"); // remove extension
      if(pos_found !=string::npos)
        {
          naked_name.erase(pos_found);
        }



      ui->push_banana->setText(naked_name.c_str() );
    }
  else{


      switch ( askQuestionWithButtons(      // +
                                          "You pressed: Cancel",
                                          "Do you want to set 'no_polygon' situation?",

                                          " no_polygon ",  // 1
                                          "Leave it as it was previously",   // 2
                                          "Cancel", 2 ) )
        {
        case 1 : // Yes
          ui->push_banana->setText("no_polygon");
          break ;
        case 2:   // No
          break ;
        default:
          return ;
        }
    }

}
//******************************************************************************************************
void T4good_galileo_ge_dlg::on_checkBox_wEn_vs_eEn_banana_toggled(bool checked)
{
  ui->push_banana->setEnabled(checked);
}
