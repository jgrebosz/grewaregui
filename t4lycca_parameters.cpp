#include "t4lycca_parameters.h"
#include "ui_t4lycca_parameters.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
#include "paths.h"
#include "Tfile_helper.h"
//*************************************************************************************
T4lycca_parameters::T4lycca_parameters(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4lycca_parameters)
{
    ui->setupUi(this);
    init();
}
//*************************************************************************************
T4lycca_parameters::~T4lycca_parameters()
{
    delete ui;
}
//*************************************************************************************
void T4lycca_parameters::accept()
{
  // save to the file
  string pname = gpath.options + "lycca_settings.txt" ;

  ofstream plik(pname.c_str() );
  if(!plik)
  {
    cout << "Can't open file " << pname << " for saving the Lycca  options" << endl;

  }

  // writing
  plik
  << "// This file contains specification about Lycca  settings (options) \n"

  << "\n\nlower_gate_for_si_energy\t"
  << ui->lineEdit_energy_si_threshold->text().toStdString()

  << "\n\nupper_gate_for_si_energy\t"
  << ui->lineEdit_energy_si_threshold_upper->text().toStdString()

  << "\nlower_gate_for_csi_energy\t"
  << ui->lineEdit_energy_csi_threshold->text().toStdString()

  << "\nupper_gate_for_csi_energy\t"
  << ui->lineEdit_energy_csi_threshold_upper->text().toStdString()
  // good
  << "\nsci42_acceptable_energy_low\t"
  << ui->lineEdit_sci42_low->text().toStdString()

  << "\nsci42_acceptable_energy_high\t"
  << ui->lineEdit_sci42_high->text().toStdString()

  << "\n\naccept_more_hits\t"
  << ui->radio_accept_more_implantations->isChecked()

  << "\n\nalgorithm_for_implantation_position_calculation_is_mean\t"   // otherwise MAX
  << ui->radio_position_algorithm_mean->isChecked()


  << "\n\ntype_of_zet\t" << ui->comboBox_zet->currentIndex()
  << "\nsome_zet_low\t" << ui->lineEdit_somezet_low->text().toStdString()
  << "\nsome_zet_high\t" << ui->lineEdit_somezet_high->text().toStdString()

  << "\n\ntype_of_aoq\t" << ui->comboBox_aoq->currentIndex()
  << "\nsome_aoq_low\t" << ui->lineEdit_someaoq_low->text().toStdString()
  << "\nsome_aoq_high\t" << ui->lineEdit_someaoq_high->text().toStdString()


  << "\ncollect_si_en_raw_spectrum\t"  <<
  ui->checkBox_si_en_raw_spectrum->isChecked()
  << "\ncollect_si_en_cal_spectrum\t"  <<
  ui->checkBox_si_en_cal_spectrum->isChecked()
  << "\ncollect_csi_en_raw_spectrum\t"  <<
  ui->checkBox_csi_en_raw_spectrum->isChecked()
  << "\ncollect_csi_en_cal_spectrum\t"  <<
  ui->checkBox_csi_en_cal_spectrum->isChecked()


 << "\ncollect_si_tim_raw_spectrum\t"  <<
  ui->checkBox_si_tim_raw_spectrum->isChecked()
  << "\ncollect_si_tim_cal_spectrum\t"  <<
  ui->checkBox_si_tim_cal_spectrum->isChecked()
  << "\ncollect_csi_tim_raw_spectrum\t"  <<
  ui->checkBox_csi_tim_raw_spectrum->isChecked()
  << "\ncollect_csi_tim_cal_spectrum\t"  <<
  ui->checkBox_csi_tim_cal_spectrum->isChecked()


  << endl;

  //==========================
  QDialog::accept();
}

//*************************************************************************************
void T4lycca_parameters::init()
{

  ui->checkBox_csi_tim_raw_spectrum->hide();
  ui->checkBox_csi_tim_cal_spectrum->hide();
  set_default();

  // reading from the file

  string pname = gpath.options + "lycca_settings.txt" ;
  ifstream plik(pname.c_str());
  if(!plik)
  {
    cout << "Cant open file " << pname << endl;
    return ;
  }

  try
  {
    // reading
    //    =====================================
    //---------
    double ddd;
    ddd =  Nfile_helper::find_in_file(plik, "lower_gate_for_si_energy") ;
    ui->lineEdit_energy_si_threshold->setText(QString::number(ddd));

    ddd =  Nfile_helper::find_in_file(plik, "upper_gate_for_si_energy") ;
    ui->lineEdit_energy_si_threshold_upper->setText(QString::number(ddd));

    ddd =  Nfile_helper::find_in_file(plik, "lower_gate_for_csi_energy") ;
    ui->lineEdit_energy_csi_threshold->setText(QString::number(ddd));
    ddd =  Nfile_helper::find_in_file(plik, "upper_gate_for_csi_energy") ;
    ui->lineEdit_energy_csi_threshold_upper->setText(QString::number(ddd));
    //-------

    // good
    ddd =  Nfile_helper::find_in_file(plik, "sci42_acceptable_energy_low");
    ui->lineEdit_sci42_low->setText(QString::number(ddd));

    ddd =  Nfile_helper::find_in_file(plik, "sci42_acceptable_energy_high");
    ui->lineEdit_sci42_high->setText(QString::number(ddd));



    bool flag = (bool) Nfile_helper::find_in_file(plik, "accept_more_hits");
    ui->radio_accept_more_implantations->setChecked(flag);
    ui->radio_reject_more_implantations->setChecked(!flag);

    flag = (bool) Nfile_helper::find_in_file(plik, "algorithm_for_implantation_position_calculation_is_mean");
    // otherwise MAX
    ui->radio_position_algorithm_mean->setChecked(flag);
    ui->radio_position_algorithm_max->setChecked(!flag);


    // Zet vs Aoq condition
    ddd =  Nfile_helper::find_in_file(plik, "type_of_zet");
    ui->comboBox_zet->setCurrentIndex(int (ddd) );

    ddd =  Nfile_helper::find_in_file(plik, "some_zet_low");
    ui->lineEdit_somezet_low->setText(QString::number(ddd));

    ddd =  Nfile_helper::find_in_file(plik, "some_zet_high");
    ui->lineEdit_somezet_high->setText(QString::number(ddd));


    ddd =  Nfile_helper::find_in_file(plik, "type_of_aoq");
    ui->comboBox_aoq->setCurrentIndex((int)ddd);

    ddd =  Nfile_helper::find_in_file(plik, "some_aoq_low");
    ui->lineEdit_someaoq_low->setText(QString::number(ddd));

    ddd =  Nfile_helper::find_in_file(plik, "some_aoq_high");
    ui->lineEdit_someaoq_high->setText(QString::number(ddd));


    flag = (bool) Nfile_helper::find_in_file(plik, "collect_si_en_raw_spectrum");
    ui->checkBox_si_en_raw_spectrum->setChecked(flag);

    flag = (bool) Nfile_helper::find_in_file(plik, "collect_si_en_cal_spectrum");
    ui->checkBox_si_en_cal_spectrum->setChecked(flag);

    flag = (bool) Nfile_helper::find_in_file(plik, "collect_csi_en_raw_spectrum");
    ui->checkBox_csi_en_raw_spectrum->setChecked(flag);

    flag = (bool) Nfile_helper::find_in_file(plik, "collect_csi_en_cal_spectrum");
    ui->checkBox_csi_en_cal_spectrum->setChecked(flag);

    // tim
  flag = (bool) Nfile_helper::find_in_file(plik, "collect_si_tim_raw_spectrum");
    ui->checkBox_si_tim_raw_spectrum->setChecked(flag);

    flag = (bool) Nfile_helper::find_in_file(plik, "collect_si_tim_cal_spectrum");
    ui->checkBox_si_tim_cal_spectrum->setChecked(flag);

    flag = (bool) Nfile_helper::find_in_file(plik, "collect_csi_tim_raw_spectrum");
    ui->checkBox_csi_tim_raw_spectrum->setChecked(flag);

    flag = (bool) Nfile_helper::find_in_file(plik, "collect_csi_tim_cal_spectrum");
    ui->checkBox_csi_tim_cal_spectrum->setChecked(flag);

  }
  catch(Tfile_helper_exception k)
  {
    cout << "Error while reading " << pname << "\n" << k.message << endl;
  }

}
//*************************************************************************************
void T4lycca_parameters::help_Slot()
{
  string mess = "Here you may specify how the Lycca  works. "

                "You may define the 'sensible' values of energies  "
                "You may also define how to calculate the position";
}
//*************************************************************************************
void T4lycca_parameters::set_default()
{

  ui->lineEdit_energy_si_threshold->setText("1");
  ui->lineEdit_energy_csi_threshold->setText("1");
  ui->lineEdit_energy_si_threshold_upper->setText("8191");
  ui->lineEdit_energy_csi_threshold_upper->setText("8191");

  // good
  ui->lineEdit_sci42_low->setText("0");
  ui->lineEdit_sci42_high->setText("8192");

  ui->radio_accept_more_implantations->setChecked(true);
  ui->radio_position_algorithm_mean->setChecked(true);
  ui->radio_position_algorithm_max->setChecked(false);


  ui->comboBox_zet->addItem ("zet");
  ui->comboBox_zet->addItem ("zet2");
  ui->comboBox_zet->addItem ("zet_tpc");
  ui->comboBox_zet->addItem ("zet2_tpc");

  ui->comboBox_aoq-> addItem ("aoq_mw_corr");
  ui->comboBox_aoq-> addItem ("aoq_sci_corr");
  ui->comboBox_aoq-> addItem ("aoq_tpc_tpc_corr");

  //ui->comboBox_aoq-> addItem ("aoq_NON_corr");

  ui->comboBox_zet->setCurrentIndex(0);
  ui->lineEdit_somezet_low->setText(QString::number(1));
  ui->lineEdit_somezet_high->setText(QString::number(100));

  ui->comboBox_aoq->setCurrentIndex(0);

  ui->lineEdit_someaoq_low->setText(QString::number(1));
  ui->lineEdit_someaoq_high->setText(QString::number(4));

  ui->checkBox_si_en_raw_spectrum->setChecked(false);
  ui->checkBox_si_en_cal_spectrum->setChecked(false);
  ui->checkBox_csi_en_raw_spectrum->setChecked(false);
  ui->checkBox_csi_en_cal_spectrum->setChecked(false);

  ui->checkBox_si_tim_raw_spectrum->setChecked(false);
  ui->checkBox_si_tim_cal_spectrum->setChecked(false);
  ui->checkBox_csi_tim_raw_spectrum->setChecked(false);
  ui->checkBox_csi_tim_cal_spectrum->setChecked(false);

}
//*************************************************************************************
