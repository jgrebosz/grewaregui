#include "t4interesting_ranges_galileo.h"
#include "ui_t4interesting_ranges_galileo.h"

#include <fstream>
using namespace std;
#include <iostream>
using namespace std;
#include "paths.h"
#include "Tfile_helper.h"
#include <QMessageBox>
#include <QDoubleValidator>
//***************************************************************************************************
T4interesting_ranges_galileo::T4interesting_ranges_galileo(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::T4interesting_ranges_galileo)
{
  ui->setupUi(this);

  ui->lineEdit_Eraw_max->setText("4096");
  ui->lineEdit_Ecal_max->setText("4096");

  ui->lineEdit_Time_max->setText("4096");



  ui->lineEdit_AC_Eraw_max->setText("4096");
  ui->lineEdit_AC_Ecal_max->setText("4096");

  ui->lineEdit_AC_Time_max->setText("4096");


  // Euclides
  ui->lineEdit_E_eraw_max->setText("4096");
  ui->lineEdit_E_ecal_max->setText("4096");

  ui->lineEdit_E_Time_max->setText("4096");



  ui->lineEdit_dE_eraw_max->setText("4096");
  ui->lineEdit_dE_ecal_max->setText("4096");

  ui->lineEdit_dE_Time_max->setText("4096");







  // try to open the file with this option
  string fname = gpath.options + "galileo_preferred_ranges.option";
  ifstream plik(fname);
  if(!plik)
  {
      cout << "Can't open file: " << fname << endl;
      return ;
  }

  try{
      double value =  FH::find_in_file(plik, "Eraw_max") ;
      ui->lineEdit_Eraw_max->setValidator( new QDoubleValidator( 0, 120, 3,   ui->lineEdit_Eraw_max) );
      ui->lineEdit_Eraw_max->setText(QString::number(value, 'f', 1));

      value =  FH::find_in_file(plik, "Ecal_max") ;
      ui->lineEdit_Ecal_max->setValidator( new QDoubleValidator( 0, 120, 3,   ui->lineEdit_Ecal_max) );
      ui->lineEdit_Ecal_max->setText(QString::number(value, 'f', 1));


      value =  FH::find_in_file(plik, "Time_max") ;
      ui->lineEdit_Time_max->setValidator( new QDoubleValidator( 0, 120, 3,   ui->lineEdit_Time_max) );
      ui->lineEdit_Time_max->setText(QString::number(value, 'f', 1));



      // AC
      value =  FH::find_in_file(plik, "AC_Eraw_max") ;
      ui->lineEdit_AC_Eraw_max->setValidator( new QDoubleValidator( 0, 120, 3,   ui->lineEdit_AC_Eraw_max) );
      ui->lineEdit_AC_Eraw_max->setText(QString::number(value, 'f', 1));

      value =  FH::find_in_file(plik, "AC_Ecal_max") ;
      ui->lineEdit_AC_Ecal_max->setValidator( new QDoubleValidator( 0, 120, 3,   ui->lineEdit_AC_Ecal_max) );
      ui->lineEdit_AC_Ecal_max->setText(QString::number(value, 'f', 1));


      value =  FH::find_in_file(plik, "AC_Time_max") ;
      ui->lineEdit_AC_Time_max->setValidator( new QDoubleValidator( 0, 120, 3,   ui->lineEdit_AC_Time_max) );
      ui->lineEdit_AC_Time_max->setText(QString::number(value, 'f', 1));



  read_and_set(plik, "Euclides_E_eraw_max" , ui->lineEdit_E_eraw_max);
  read_and_set(plik, "Euclides_E_ecal_max" , ui->lineEdit_E_ecal_max);
  read_and_set(plik, "Euclides_E_Time_max" , ui->lineEdit_E_Time_max);

  read_and_set(plik, "Euclides_dE_eraw_max" , ui->lineEdit_dE_eraw_max);
  read_and_set(plik, "Euclides_dE_ecal_max" , ui->lineEdit_dE_ecal_max);
  read_and_set(plik, "Euclides_dE_Time_max" , ui->lineEdit_dE_Time_max);




  }
  catch(...)
  {
      cout << "Exception while reading addback option file " << endl;
  }

  //this->adjustSize();
}
//***************************************************************************************************
void T4interesting_ranges_galileo::read_and_set(ifstream &file, string txt, QLineEdit * ledit)
{
  double value =  FH::find_in_file(file, txt) ;
  ledit->setValidator( new QDoubleValidator( 0, 120, 3, ledit) );
  ledit->setText(QString::number(value, 'f', 1));
}
//***************************************************************************************************
T4interesting_ranges_galileo::~T4interesting_ranges_galileo()
{
  delete ui;
}
//***************************************************************************************************
void T4interesting_ranges_galileo::on_buttonBox_accepted()
{
  string fname = gpath.options + "galileo_preferred_ranges.option";
  ofstream plik(fname);
  plik
          << "Eraw_max\t" << ui->lineEdit_Eraw_max->text().toStdString()
          << "\nEcal_max\t" << ui->lineEdit_Ecal_max->text().toStdString()
          << "\nTime_max\t" << ui->lineEdit_Time_max->text().toStdString()

          << "\nAC_Eraw_max\t" << ui->lineEdit_AC_Eraw_max->text().toStdString()
          << "\nAC_Ecal_max\t" << ui->lineEdit_AC_Ecal_max->text().toStdString()
          << "\nAC_Time_max\t" << ui->lineEdit_AC_Time_max->text().toStdString()

             << "\nEuclides_E_eraw_max\t" <<ui->lineEdit_E_eraw_max->text().toStdString()
             <<"\nEuclides_E_ecal_max\t" <<  ui->lineEdit_E_ecal_max->text().toStdString()
             <<"\nEuclides_E_Time_max\t" <<  ui->lineEdit_E_Time_max->text().toStdString()

             <<"\nEuclides_dE_eraw_max\t" <<  ui->lineEdit_dE_eraw_max->text().toStdString()
             <<"\nEuclides_dE_ecal_max\t" <<  ui->lineEdit_dE_ecal_max->text().toStdString()
             <<"\nEuclides_dE_Time_max\t" <<  ui->lineEdit_dE_Time_max->text().toStdString()
          << endl ;
  plik.close() ;

  QDialog::accept();
  QMessageBox::information ( this, "Note",
                             "Hint:\nYou just made the settings about the new ranges of Energy/Time (raw and calibrated)\n"
                             "The SPY will obey your wish next time you start it\n"
                             "(as the SPY creates default spectra only during its start).\n\n\n"
                             "The previous contents of the spectra (which will have now different range)\n"
                             "will be erased.\n"
                             "All other spectra will not be changed" ,
                             QMessageBox::Ok);
}
//***************************************************************************************************
