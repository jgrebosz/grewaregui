#include "t4selfgate_ger_dlg.h"
#include "ui_t4selfgate_ger_dlg.h"

#include <iostream>   // for cout
#include <string>
using namespace std;

#include "Tself_gate_ger_descr.h"
#include "t4picture_angle.h"

#include "paths.h"

#include <QMessageBox>
#include <QFileDialog>
#include "tplate_spectrum.h"

//*************************************************************************
T4selfgate_ger_dlg::T4selfgate_ger_dlg(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::T4selfgate_ger_dlg)
{
  ui->setupUi(this);
  init();
}
//*************************************************************************
T4selfgate_ger_dlg::~T4selfgate_ger_dlg()
{
  delete ui;
}
//****************************************************************
void T4selfgate_ger_dlg::show_picture_gp()
{
  T4picture_angle dlg;
  dlg.exec();
}
//****************************************************************
void T4selfgate_ger_dlg::init()
{
  //this_is_germanium = false ; //true ; // for false - means this is Hector selfgate
  ui->push_banana->setText("click to choose a polygon gate");
  update_checked_unchecked();

  //  ui->checkBox_mult->hide() ; //setVisible(false);
  //  ui->lineEdit_clus_mult_low->hide() ; //setVisible(false);
  //  ui->lineEdit_clus_mult_high->hide() ;  //setVisible(false);
}

//****************************************************************
void T4selfgate_ger_dlg::update_checked_unchecked()
{

  ui->lineEdit_energy4_low->setEnabled(ui->checkBox_en4->isChecked());
  ui->lineEdit_energy4_high->setEnabled(ui->checkBox_en4->isChecked());

  ui->lineEdit_energy20_low->setEnabled(ui->checkBox_en20->isChecked());
  ui->lineEdit_energy20_high->setEnabled(ui->checkBox_en20->isChecked());

  // original time
  ui->lineEdit_time_low->setEnabled(ui->checkBox_time->isChecked());
  ui->lineEdit_time_high->setEnabled(ui->checkBox_time->isChecked());

  // Long Range and Short Range time
  ui->lineEdit_LR_time_low->setEnabled(ui->checkBox_LR_time->isChecked());
  ui->lineEdit_LR_time_high->setEnabled(ui->checkBox_LR_time->isChecked());
  ui->lineEdit_SR_time_low->setEnabled(ui->checkBox_SR_time->isChecked());
  ui->lineEdit_SR_time_high->setEnabled(ui->checkBox_SR_time->isChecked());


  ui->push_banana->setEnabled(ui->checkBox_en_tim_banana->isChecked());
  ui->push_time_chooser->setEnabled(ui->checkBox_en_tim_banana->isChecked());

  //-------------
  ui->lineEdit_geom_theta_low->setEnabled(ui->checkBox_geom_theta->isChecked());
  ui->lineEdit_geom_theta_high->setEnabled(ui->checkBox_geom_theta->isChecked());

  ui->lineEdit_geom_phi_low->setEnabled(ui->checkBox_geom_phi->isChecked());
  ui->lineEdit_geom_phi_high->setEnabled(ui->checkBox_geom_phi->isChecked());

  //-------------  'gp'  stands for Gamma-Particle -----------------
  ui->lineEdit_gp_theta_low->setEnabled(ui->checkBox_gp_theta->isChecked());
  ui->lineEdit_gp_theta_high->setEnabled(ui->checkBox_gp_theta->isChecked());

  ui->lineEdit_gp_phi_low->setEnabled(ui->checkBox_gp_phi->isChecked());
  ui->lineEdit_gp_phi_high->setEnabled(ui->checkBox_gp_phi->isChecked());

  // this is mutliplicity inside one cluster --------------------------

  ui->lineEdit_clus_mult_low->setEnabled(ui->checkBox_mult->isChecked());
  ui->lineEdit_clus_mult_high->setEnabled(ui->checkBox_mult->isChecked());

  // this is AC of the one cluster --------------------------

  ui->spinBox_AC_low->setEnabled(ui->checkBox_AC->isChecked());
  ui->spinBox_AC_high->setEnabled(ui->checkBox_AC->isChecked());



}
//********************************************************************
void T4selfgate_ger_dlg::set_parameters( const Tself_gate_ger_descr *d )
{
  string nnn = d->name;
  string ext = (d->sg_type).give_type_extension() ;
  string::size_type i = nnn.find(ext);
  if(i != string::npos)
    {
      nnn.erase(i) ;    // do not show extension in the edit field
    }

  ui->label_extension->setText(ext.c_str() );
  ui->lineEdit_name->setText(nnn.c_str());
  ui->checkBox_en4->setChecked(d->enable_en4gate) ;
  ui->lineEdit_energy4_low->setText(QString::number(d-> en4_gate[0]));
  ui->lineEdit_energy4_high->setText(QString::number(d-> en4_gate[1]));

  ui->checkBox_en20->setChecked(d->enable_en20gate) ;
  ui->lineEdit_energy20_low->setText(QString::number(d-> en20_gate[0]));
  ui->lineEdit_energy20_high->setText(QString::number(d-> en20_gate[1]));

  ui->checkBox_time->setChecked(d-> enable_time_gate) ;
  ui->lineEdit_time_low->setText(QString::number(d-> time_gate[0]));
  ui->lineEdit_time_high->setText(QString::number(d-> time_gate[1]));

  //=============
  ui->check_xy_difference->setChecked(d->enable_xy_gamma_time_difference);


  ui->checkBox_LR_time->setChecked(d-> enable_LR_time_gate) ;
  ui->lineEdit_LR_time_low->setText(QString::number(d-> LR_time_gate[0]));
  ui->lineEdit_LR_time_high->setText(QString::number(d-> LR_time_gate[1]));

  ui->checkBox_SR_time->setChecked(d-> enable_SR_time_gate) ;
  ui->lineEdit_SR_time_low->setText(QString::number(d-> SR_time_gate[0]));
  ui->lineEdit_SR_time_high->setText(QString::number(d-> SR_time_gate[1]));


  ui->checkBox_en_tim_banana->setChecked(d-> enable_energy_time_polygon_gate);
  ui->push_banana->setText(d->energy_time_polygon_gate.c_str());
  int chosen_time_nr = d->which_time;

  switch(chosen_time_nr)
    {
    default:
    case Tself_gate_ger_descr:: time_cal: ui->push_time_chooser->setText("Time cal");
      break;
    case Tself_gate_ger_descr::LR_time_cal:   ui->push_time_chooser->setText("LR time cal");
      break;
    case Tself_gate_ger_descr::SR_time_cal:  ui->push_time_chooser->setText("SR time cal");
      break;
    }

  ui->checkBox_geom_theta->setChecked(d->enable_geom_theta_gate) ;
  ui->lineEdit_geom_theta_low->setText(QString::number(d-> geom_theta_gate[0]));
  ui->lineEdit_geom_theta_high->setText(QString::number(d-> geom_theta_gate[1]));

  ui->checkBox_geom_phi->setChecked(d-> enable_geom_phi_gate) ;
  ui->lineEdit_geom_phi_low->setText(QString::number(d-> geom_phi_gate[0]));
  ui->lineEdit_geom_phi_high->setText(QString::number(d-> geom_phi_gate[1]));

  // gamma - particle angle
  ui->checkBox_gp_theta->setChecked(d-> enable_gp_theta_gate) ;
  ui->lineEdit_gp_theta_low->setText(QString::number(d-> gp_theta_gate[0]));
  ui->lineEdit_gp_theta_high->setText(QString::number(d-> gp_theta_gate[1]));

  ui->checkBox_gp_phi->setChecked(d-> enable_gp_phi_gate); ;
  ui->lineEdit_gp_phi_low->setText(QString::number(d-> gp_phi_gate[0]));
  ui->lineEdit_gp_phi_high->setText(QString::number(d-> gp_phi_gate[1]));

  // this is the multiplicity inside the cluster where this crystal belongs
  ui->checkBox_mult->setChecked(d-> enable_mult_in_cluster_gate);
  ui->lineEdit_clus_mult_low->setText(QString::number(d->mult_in_cluster_gate[0]));
  ui->lineEdit_clus_mult_high->setText(QString::number(d->mult_in_cluster_gate[1]));

  // this is the AC energy inside the cluster where this crystal belongs
  ui->checkBox_AC->setChecked(d-> enable_AC_in_cluster_gate);
  ui->spinBox_AC_low->setValue((int) d->AC_in_cluster_gate[0]);
  ui->spinBox_AC_high->setValue((int) d->AC_in_cluster_gate[1]);

  update_checked_unchecked();

}
//*******************************************************************
void T4selfgate_ger_dlg::get_parameters( Tself_gate_ger_descr * d )
{
  string s = d->name = ui->lineEdit_name->text().toStdString();
  // if there is no extension, add it

  string ext = (d->sg_type).give_type_extension() ;
  string::size_type i = s.find(ext);
  if(i == string::npos)
    {
      d->name += ext;
    }

  d->enable_en4gate = ui->checkBox_en4->isChecked() ;
  d-> en4_gate[0] = ui->lineEdit_energy4_low->text().toDouble();
  d-> en4_gate[1] = ui->lineEdit_energy4_high->text().toDouble();

  d->enable_en20gate = ui->checkBox_en20->isChecked() ;
  d-> en20_gate[0] = ui->lineEdit_energy20_low->text().toDouble();
  d-> en20_gate[1] = ui->lineEdit_energy20_high->text().toDouble();

  d-> enable_time_gate = ui->checkBox_time->isChecked() ;
  d-> time_gate[0] = ui->lineEdit_time_low->text().toDouble();
  d-> time_gate[1] = ui->lineEdit_time_high->text().toDouble();


  //------------
  d-> enable_LR_time_gate = ui->checkBox_LR_time->isChecked() ;
  d-> LR_time_gate[0] = ui->lineEdit_LR_time_low->text().toDouble();
  d-> LR_time_gate[1] = ui->lineEdit_LR_time_high->text().toDouble();

  d-> enable_SR_time_gate = ui->checkBox_SR_time->isChecked() ;
  d-> SR_time_gate[0] = ui->lineEdit_SR_time_low->text().toDouble();
  d-> SR_time_gate[1] = ui->lineEdit_SR_time_high->text().toDouble();


  // polygon gate for Marek Pfutzner
  d-> enable_energy_time_polygon_gate = ui->checkBox_en_tim_banana->isChecked();
  d->energy_time_polygon_gate = ui->push_banana->text().toStdString();

  d->which_time = chosen_time;

  //----------
  d->enable_geom_theta_gate = ui->checkBox_geom_theta->isChecked() ;
  d-> geom_theta_gate[0] = ui->lineEdit_geom_theta_low->text().toDouble();
  d-> geom_theta_gate[1] = ui->lineEdit_geom_theta_high->text().toDouble();

  d-> enable_geom_phi_gate = ui->checkBox_geom_phi->isChecked();
  d-> geom_phi_gate[0] = ui->lineEdit_geom_phi_low->text().toDouble();
  d-> geom_phi_gate[1] = ui->lineEdit_geom_phi_high->text().toDouble();

  // gamma - particle angle
  d-> enable_gp_theta_gate = ui->checkBox_gp_theta->isChecked() ;
  d-> gp_theta_gate[0] = ui->lineEdit_gp_theta_low->text().toDouble();
  d-> gp_theta_gate[1] = ui->lineEdit_gp_theta_high->text().toDouble();

  d-> enable_gp_phi_gate = ui->checkBox_gp_phi->isChecked(); ;
  d-> gp_phi_gate[0] = ui->lineEdit_gp_phi_low->text().toDouble();
  d-> gp_phi_gate[1] = ui->lineEdit_gp_phi_high->text().toDouble();

  // this is the multiplicity inside the cluster where this crystal belongs
  d-> enable_mult_in_cluster_gate = ui->checkBox_mult->isChecked();
  d->mult_in_cluster_gate[0] = ui->lineEdit_clus_mult_low->text().toDouble();
  d->mult_in_cluster_gate[1] = ui->lineEdit_clus_mult_high->text().toDouble();

  // this is the AC inside the cluster where this crystal belongs
  d-> enable_AC_in_cluster_gate = ui->checkBox_AC->isChecked();
  d->AC_in_cluster_gate[0] = ui->spinBox_AC_low->value();
  d->AC_in_cluster_gate[1] = ui->spinBox_AC_high->value();

  //------------------------
  d->enable_xy_gamma_time_difference = ui->check_xy_difference->isChecked() ;
}
//*******************************************************************
void T4selfgate_ger_dlg::accept()
{
  if(ui->lineEdit_name->text() == "" )
    {

      QMessageBox::critical( this, "The name is missing",
                             "Please specify the (unique) name of this 'self-gate' ",
                             QMessageBox::Ok | QMessageBox::Default,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);  // error
      return ;
    }
  QDialog::accept();

}
//*************************************************************************************
void T4selfgate_ger_dlg::info_xy_selfgate()
{
  QMessageBox::information( this, "X Y gamma gamma time differense",
                            "You use this opiton when you are going to make:\na gamma gamma matrix\n"
                            "on the condition that the time difference \nbetween two gammas is in chosen time range\n\n\n"
                            "To use this option:\n 1. Make the desired gamma-gamma matrix.\n"
                            "Then:\n 2. apply this selfgate to the X incrementer of this matrix\n"
                            " 3. apply THE SAME SELFGATE to the Y incrementer of this matrix "
                            );
}
//************************************************************************************
void T4selfgate_ger_dlg::change_text_of_time( bool state )
{
  ui->checkBox_time->setText(  state==0 ?
                                 "Calibrated Time value is in the following range:"
                               :
                                 " | (X time_cal - Y time_cal) |    is in range:");
}
//*************************************************************************************
void T4selfgate_ger_dlg::choosing_banana()
{
  /*
  QFileDialog* fd = new QFileDialog( gpath.polygons, "*.poly", this, "Choose the polygon gate", true);
  fd->setMode( QFileDialog::ExistingFile );
  fd->setFilter( "polygon gate  (*.poly)" );
  */
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
      string::size_type pos_slash = naked_name.rfind("/");
      if(pos_slash !=string::npos)
        {
          naked_name.erase(0, pos_slash + 1);
        }
      ui->push_banana->setText(naked_name.c_str() );
    }

}
//************************************************************************************
void T4selfgate_ger_dlg::time_chooser_pressed()
{
  //cout << " pressed when " << (int) type_of_time << endl;
  switch(chosen_time)
    {
    default:
    case Tself_gate_ger_descr:: time_cal:
      ui->push_time_chooser->setText("LR time cal");
      chosen_time = Tself_gate_ger_descr::LR_time_cal;
      break;
    case Tself_gate_ger_descr::LR_time_cal:
      ui->push_time_chooser->setText("SR time cal");
      chosen_time = Tself_gate_ger_descr::SR_time_cal;
      break;
    case Tself_gate_ger_descr::SR_time_cal:
      ui->push_time_chooser->setText("Time cal");
      chosen_time = Tself_gate_ger_descr::time_cal;
      break;
    }
}
//************************************************************************************

void T4selfgate_ger_dlg::on_checkBox_en4_clicked()
{
  update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_ger_dlg::on_checkBox_en20_clicked()
{
  update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_ger_dlg::on_checkBox_time_clicked()
{
  update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_ger_dlg::on_check_xy_difference_clicked()
{
  update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_ger_dlg::on_checkBox_LR_time_clicked()
{
  update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_ger_dlg::on_checkBox_SR_time_clicked()
{
  update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_ger_dlg::on_checkBox_en_tim_banana_clicked()
{
  update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_ger_dlg::on_checkBox_geom_theta_clicked()
{
  update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_ger_dlg::on_checkBox_geom_phi_clicked()
{
  update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_ger_dlg::on_checkBox_gp_theta_clicked()
{
  update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_ger_dlg::on_checkBox_gp_phi_clicked()
{
  update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_ger_dlg::on_checkBox_mult_clicked()
{
  update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_ger_dlg::on_checkBox_AC_clicked()
{
  update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_ger_dlg::on_push_banana_clicked()
{
  choosing_banana();
}
//************************************************************************************
void T4selfgate_ger_dlg::on_pushButton_angle_picture_clicked()
{
  show_picture_gp();
}
//************************************************************************************
void T4selfgate_ger_dlg::on_push_xy_info_clicked()
{
  info_xy_selfgate();
}
//************************************************************************************

