#include "t4selfgate_ge_crystal_galileo.h"
#include "ui_t4selfgate_ge_crystal_galileo.h"

#include <iostream>   // for cout
#include <string>
using namespace std;

#include "Tself_gate_ge_galileo_descr.h"
#include "t4picture_angle.h"

#include "paths.h"

#include <QMessageBox>
#include <QFileDialog>
#include "tplate_spectrum.h"



//****************************************************************

T4selfgate_ge_galileo::T4selfgate_ge_galileo(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::T4selfgate_ge_galileo)
{
  ui->setupUi(this);
  init();
}
//****************************************************************
T4selfgate_ge_galileo::~T4selfgate_ge_galileo()
{
  delete ui;
}
//****************************************************************
void T4selfgate_ge_galileo::show_picture_gp()
{
  T4picture_angle dlg;
  dlg.exec();
}
//****************************************************************
void T4selfgate_ge_galileo::init()
{
  //this_is_germanium = false ; //true ; // for false - means this is Hector selfgate
  ui->push_banana->setText("click to choose a polygon gate");
  update_checked_unchecked();

  //  ui->checkBox_mult->hide() ; //setVisible(false);
  //  ui->lineEdit_clus_mult_low->hide() ; //setVisible(false);
  //  ui->lineEdit_clus_mult_high->hide() ;  //setVisible(false);
}
//****************************************************************
void T4selfgate_ge_galileo::update_checked_unchecked()
{

  ui->lineEdit_energy_low->setEnabled(ui->checkBox_en4->isChecked());
  ui->lineEdit_energy_high->setEnabled(ui->checkBox_en4->isChecked());


  // original time
  ui->lineEdit_time_low->setEnabled(ui->checkBox_time->isChecked());
  ui->lineEdit_time_high->setEnabled(ui->checkBox_time->isChecked());

  // Long Range and Short Range time
  ui->lineEdit_AC_energy_low->setEnabled(ui->checkBox_AC_energy->isChecked());
  ui->lineEdit_AC_energy_high->setEnabled(ui->checkBox_AC_energy->isChecked());
  ui->lineEdit_AC_time_low->setEnabled(ui->checkBox_AC_time->isChecked());
  ui->lineEdit_AC_time_high->setEnabled(ui->checkBox_AC_time->isChecked());

  ui->radioButton_Anticoincidence->setEnabled(ui->checkBox_AC_energy->isChecked()
                                         ||
                                         ui->checkBox_AC_time->isChecked()
                                         );
  ui->radioButton_coincidence->setEnabled(ui->checkBox_AC_energy->isChecked()
                                         ||
                                         ui->checkBox_AC_time->isChecked()
                                         );

//    ui->radioButton_Anticoincidence->setChecked(flag_anticoincidence);
//    ui->radioButton_coincidence->setChecked(! flag_anticoincidence);


  ui->push_banana->setEnabled(ui->checkBox_en_tim_banana->isChecked());

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

  //    ui->lineEdit_clus_mult_low->setEnabled(ui->checkBox_mult->isChecked());
  //    ui->lineEdit_clus_mult_high->setEnabled(ui->checkBox_mult->isChecked());

  // this is AC of the one cluster --------------------------

//      ui->spinBox_AC_low->setEnabled(ui->checkBox_AC->isChecked());
//      ui->spinBox_AC_high->setEnabled(ui->checkBox_AC->isChecked());



}
class Tself_gate_ge_galileo_descr;
//********************************************************************
void T4selfgate_ge_galileo::set_parameters( const Tself_gate_ge_galileo_descr *d )
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
  ui->lineEdit_energy_low->setText(QString::number(d-> en4_gate[0]));
  ui->lineEdit_energy_high->setText(QString::number(d-> en4_gate[1]));

  //    ui->checkBox_en20->setChecked(d->enable_en20gate) ;
  //    ui->lineEdit_energy20_low->setText(QString::number(d-> en20_gate[0]));
  //    ui->lineEdit_energy20_high->setText(QString::number(d-> en20_gate[1]));

  ui->checkBox_time->setChecked(d-> enable_time_gate) ;
  ui->lineEdit_time_low->setText(QString::number(d-> time_gate[0]));
  ui->lineEdit_time_high->setText(QString::number(d-> time_gate[1]));

  //=============
  ui->check_xy_difference->setChecked(d->enable_xy_gamma_time_difference);

  ui->checkBox_en_tim_banana->setChecked(d-> enable_energy_time_polygon_gate);
  ui->push_banana->setText(d->energy_time_polygon_gate.c_str());

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
  //    ui->checkBox_mult->setChecked(d-> enable_mult_in_cluster_gate);
  //    ui->lineEdit_clus_mult_low->setText(QString::number(d->mult_in_cluster_gate[0]));
  //    ui->lineEdit_clus_mult_high->setText(QString::number(d->mult_in_cluster_gate[1]));

  // this is the AC energy inside the cluster where this crystal belongs
  ui->checkBox_AC_energy->setChecked(d-> enable_AC_energy_gate);
  ui->lineEdit_AC_energy_low->setText(QString::number(d->AC_energy_gate[0]));
  ui->lineEdit_AC_energy_high->setText(QString::number(d->AC_energy_gate[1]));

  ui->checkBox_AC_time->setChecked(d-> enable_AC_time_gate);
  ui->lineEdit_AC_time_low->setText(QString::number(d->AC_time_gate[0]));
  ui->lineEdit_AC_time_high->setText(QString::number(d->AC_time_gate[1]));

  ui->radioButton_Anticoincidence->setEnabled(d-> enable_AC_energy_gate || d->enable_AC_time_gate);
  ui->radioButton_coincidence->setEnabled( ! (d-> enable_AC_energy_gate || d->enable_AC_time_gate ));

  ui->radioButton_Anticoincidence->setChecked(d->flag_ACS_antycoincydence);
  ui->radioButton_coincidence->setChecked( ! d->flag_ACS_antycoincydence);

  update_checked_unchecked();

}
//*******************************************************************
void T4selfgate_ge_galileo::get_parameters( Tself_gate_ge_galileo_descr * d )
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
  d-> en4_gate[0] = ui->lineEdit_energy_low->text().toDouble();
  d-> en4_gate[1] = ui->lineEdit_energy_high->text().toDouble();

  //    d->enable_en20gate = ui->checkBox_en20->isChecked() ;
  //    d-> en20_gate[0] = ui->lineEdit_energy20_low->text().toDouble();
  //    d-> en20_gate[1] = ui->lineEdit_energy20_high->text().toDouble();

  d-> enable_time_gate = ui->checkBox_time->isChecked() ;
  d-> time_gate[0] = ui->lineEdit_time_low->text().toDouble();
  d-> time_gate[1] = ui->lineEdit_time_high->text().toDouble();


  // polygon gate for Marek Pfutzner
  d-> enable_energy_time_polygon_gate = ui->checkBox_en_tim_banana->isChecked();
  d->energy_time_polygon_gate = ui->push_banana->text().toStdString();

    if(d->energy_time_polygon_gate.empty())d->energy_time_polygon_gate = "no_polygon";


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
  //    d-> enable_mult_in_cluster_gate = ui->checkBox_mult->isChecked();
  //    d->mult_in_cluster_gate[0] = ui->lineEdit_clus_mult_low->text().toDouble();
  //    d->mult_in_cluster_gate[1] = ui->lineEdit_clus_mult_high->text().toDouble();

  // this is the AC
  d-> enable_AC_energy_gate = ui->checkBox_AC_energy->isChecked();
  d->AC_energy_gate[0] = ui->lineEdit_AC_energy_low->text().toDouble();
  d->AC_energy_gate[1] = ui->lineEdit_AC_energy_high->text().toDouble();

  d-> enable_AC_time_gate = ui->checkBox_AC_time->isChecked();
  d->AC_time_gate[0] = ui->lineEdit_AC_time_low->text().toDouble();
  d->AC_time_gate[1] = ui->lineEdit_AC_time_high->text().toDouble();

  d->flag_ACS_antycoincydence = ui->radioButton_Anticoincidence->isChecked();

  //------------------------
  d->enable_xy_gamma_time_difference = ui->check_xy_difference->isChecked() ;
}
//*******************************************************************
void T4selfgate_ge_galileo::accept()
{
  if(ui->lineEdit_name->text() == "" )
    {

      showWarningMessage( "The name is missing",
                             "Please specify the (unique) name of this 'self-gate' ",
                             QMessageBox::Critical);  // error
      return ;
    }
  QDialog::accept();

}
//*************************************************************************************
void T4selfgate_ge_galileo::info_xy_selfgate()
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
void T4selfgate_ge_galileo::change_text_of_time( bool state )
{
  ui->checkBox_time->setText(  state==0 ?
                                 "Calibrated Time value is in the following range:"
                               :
                                 " | (X time_cal - Y time_cal) |    is in range:");
}
//*************************************************************************************
void T4selfgate_ge_galileo::choosing_banana()
{

    Tplate_spectrum::flag_repainting_allowed = false;
    QString filter;
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
    else{


        switch ( askQuestionWithButtons(        // +
                                            "You pressed: Cancel",
                                            "Do you want to set 'no_polygon' situation?",

                                            " no_polygon ",  // 1
                                            "Leave it as it was previously",   // 2
                                            "Cancel", 2 ) )
          {
          case 1 :
            ui->push_banana->setText("no_polygon");
            break ;
          case 2:   // No
            break ;

          default:              
            return ;
          }
      }
}
//************************************************************************************
void T4selfgate_ge_galileo::on_checkBox_en4_clicked()
{
  update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_ge_galileo::on_checkBox_time_clicked()
{
  update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_ge_galileo::on_check_xy_difference_clicked()
{
  update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_ge_galileo::on_push_xy_info_pressed()
{
  info_xy_selfgate();
}
//************************************************************************************
void T4selfgate_ge_galileo::on_checkBox_en_tim_banana_clicked()
{
  update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_ge_galileo::on_push_banana_pressed()
{
  choosing_banana();
}
//************************************************************************************
void T4selfgate_ge_galileo::on_checkBox_geom_theta_clicked()
{
  update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_ge_galileo::on_checkBox_geom_phi_clicked()
{
  update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_ge_galileo::on_checkBox_gp_theta_clicked()
{
  update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_ge_galileo::on_checkBox_gp_phi_clicked()
{
  update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_ge_galileo::on_pushButton_angle_picture_pressed()
{
  show_picture_gp();
}
//************************************************************************************
void T4selfgate_ge_galileo::on_checkBox_AC_energy_clicked()
{
  update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_ge_galileo::on_checkBox_AC_time_clicked()
{
  update_checked_unchecked();
}
//************************************************************************************

void T4selfgate_ge_galileo::on_radioButton_Anticoincidence_toggled([[maybe_unused]] bool checked)
{
    update_checked_unchecked();

}
//************************************************************************************
void T4selfgate_ge_galileo::on_radioButton_coincidence_toggled([[maybe_unused]] bool checked)
{
   update_checked_unchecked();
}
