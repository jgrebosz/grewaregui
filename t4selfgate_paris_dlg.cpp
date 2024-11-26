#include "t4selfgate_paris_dlg.h"
#include "ui_t4selfgate_paris_dlg.h"
#include "t4picture_angle.h"
#include <string>
using namespace std;
#include "Tself_gate_paris_descr_simple.h"
#include <QMessageBox>
#include <QFileDialog>
#include "Tfile_helper.h"
#include "paths.h"

#include "tplate_spectrum.h"
//****************************************************************
T4selfgate_paris_dlg::T4selfgate_paris_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4selfgate_paris_dlg)
{

    ui->setupUi(this);
    init();
}
//****************************************************************
T4selfgate_paris_dlg::~T4selfgate_paris_dlg()
{
    delete ui;
}
//****************************************************************

//****************************************************************
void T4selfgate_paris_dlg::init()
{
    //this_is_germanium = false ; //true ; // for false - means this is Hector selfgate
    ui->push_banana->setText("click to choose a polygon gate");
    update_checked_unchecked();
}

//****************************************************************
void T4selfgate_paris_dlg::update_checked_unchecked()
{

    ui->lineEdit_energy_fast_low->setEnabled(ui->checkBox_energy_fast->isChecked());
    ui->lineEdit_energy_fast_high->setEnabled(ui->checkBox_energy_fast->isChecked());

    ui->lineEdit_energy_slow_low->setEnabled(ui->checkBox_energy_slow->isChecked());
    ui->lineEdit_energy_slow_high->setEnabled(ui->checkBox_energy_slow->isChecked());

    ui->lineEdit_time_low->setEnabled(ui->checkBox_time->isChecked());
    ui->lineEdit_time_high->setEnabled(ui->checkBox_time->isChecked());

    ui->push_banana->setEnabled(ui->checkBox_fast_slow_banana->isChecked());

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

    //   ui->lineEdit_clus_mult_low->setEnabled(ui->checkBox_mult->isChecked());
    //   ui->lineEdit_clus_mult_high->setEnabled(ui->checkBox_mult->isChecked());
    //
    //    if(this_is_germanium == false)  // e.g. this is hector
    //    {


    //    }
}
//********************************************************************
void T4selfgate_paris_dlg::set_parameters( const Tself_gate_paris_descr *d )
{
    string nnn = d->name;
    string ext = (d->sg_type).give_type_extension() ;
    string::size_type i = nnn.find(ext);
    if(i != string::npos)
    {
        nnn.erase(i) ;    // do not show extension in the edit field
    }

    ui->lineEdit_name->setText(nnn.c_str());
    ui->label_extension->setText(ext.c_str() );

    ui->checkBox_energy_fast->setChecked(d->enable_energy_fast_gate) ;
    ui->lineEdit_energy_fast_low->setText(QString::number(d-> en_fast_gate[0]));
    ui->lineEdit_energy_fast_high->setText(QString::number(d-> en_fast_gate[1]));

    ui->checkBox_energy_slow->setChecked(d->enable_energy_slow_gate) ;
    ui->lineEdit_energy_slow_low->setText(QString::number(d-> en_slow_gate[0]));
    ui->lineEdit_energy_slow_high->setText(QString::number(d-> en_slow_gate[1]));

    ui->checkBox_time->setChecked(d-> enable_time_gate) ;
    ui->lineEdit_time_low->setText(QString::number(d-> time_gate[0]));
    ui->lineEdit_time_high->setText(QString::number(d-> time_gate[1]));

    ui->checkBox_fast_slow_banana->setChecked(d->enable_fast_vs_slow_polygon_gate);
    ui->push_banana->setText(d->name_fast_vs_slow_polygon_gate.c_str());


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

    update_checked_unchecked();

}
//*******************************************************************
void T4selfgate_paris_dlg::get_parameters( Tself_gate_paris_descr * d )
{

    string s = d->name = ui->lineEdit_name->text().toStdString();
    // if there is no extension, add it

    string ext = (d->sg_type).give_type_extension() ;
    string::size_type i = s.find(ext);
    if(i == string::npos)
    {
        d->name += ext;
    }

    d->enable_energy_fast_gate = ui->checkBox_energy_fast->isChecked() ;
    d-> en_fast_gate[0] = ui->lineEdit_energy_fast_low->text().toDouble();
    d-> en_fast_gate[1] = ui->lineEdit_energy_fast_high->text().toDouble();

    d->enable_energy_slow_gate = ui->checkBox_energy_slow->isChecked() ;
    d-> en_slow_gate[0] = ui->lineEdit_energy_slow_low->text().toDouble();
    d-> en_slow_gate[1] = ui->lineEdit_energy_slow_high->text().toDouble();

    d-> enable_time_gate = ui->checkBox_time->isChecked() ;
    d-> time_gate[0] = ui->lineEdit_time_low->text().toDouble();
    d-> time_gate[1] = ui->lineEdit_time_high->text().toDouble();


    d->enable_fast_vs_slow_polygon_gate = ui->checkBox_fast_slow_banana->isChecked();
    cout << "Enable banana is " << d->enable_fast_vs_slow_polygon_gate  << endl;

    // I do not know why, but sometimes in front there is a character '&'
    // so I have to remove it
    string tmp = ui->push_banana->text().toStdString() ;
    if(!tmp.empty() )
    {
        auto where = tmp.find("&");
        if( where != string::npos){
            cout << "Weong Name of polygon is: " << ui->push_banana->text().toStdString() << endl;
        tmp.erase(where, 1);
        cout << "Erased '&'' in a name of the polygon gate " << endl;
        }
    }

    cout << "QT Name of polygon is: " << tmp << endl;
    d->name_fast_vs_slow_polygon_gate = tmp;

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
}
//********************************************************************************************
void T4selfgate_paris_dlg::accept()
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
//********************************************************************************************

void T4selfgate_paris_dlg::on_pushButton_OK_clicked()
{
    accept();
}
//********************************************************************************************
void T4selfgate_paris_dlg::on_pushButton_Cancel_clicked()
{
    reject();
}
//********************************************************************************************
void T4selfgate_paris_dlg::on_checkBox_energy_fast_clicked()
{
    update_checked_unchecked();
}
//********************************************************************************************
void T4selfgate_paris_dlg::on_checkBox_energy_slow_clicked()
{
    update_checked_unchecked();
}
//********************************************************************************************
void T4selfgate_paris_dlg::on_checkBox_time_clicked()
{
    update_checked_unchecked();
}
//********************************************************************************************
void T4selfgate_paris_dlg::on_checkBox_geom_theta_clicked()
{
    update_checked_unchecked();
}
//********************************************************************************************
void T4selfgate_paris_dlg::on_checkBox_geom_phi_clicked()
{
    update_checked_unchecked();
}
//********************************************************************************************
void T4selfgate_paris_dlg::on_checkBox_gp_theta_clicked()
{
    update_checked_unchecked();
}
//********************************************************************************************
void T4selfgate_paris_dlg::on_checkBox_gp_phi_clicked()
{
    update_checked_unchecked();
}
//********************************************************************************************

void T4selfgate_paris_dlg::on_pushButton_picture_clicked()
{
    T4picture_angle dlg;
    dlg.exec();
}
//********************************************************************************************
void T4selfgate_paris_dlg::on_push_banana_clicked()
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
    //cout << "Name of the polygon: " << fileName << endl ;
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
        cout << "The polygon name is set to: " << naked_name << endl;
      }
    else{


        switch ( QMessageBox::information ( this,
                                            "You pressed: Cancel",
                                            "Do you want to set 'no_polygon' situation?",

                                            //"( Yes => all, No => only the one selected)",
                                            " no_polygon ",  // QMessageBox::Yes | QMessageBox::Default,
                                            "Leave it as it was previously",   // QMessageBox::No,
                                            "Cancel", 1 ) )
          {
          case 0 : // Yes
            ui->push_banana->setText("no_polygon");
            break ;
          case 1:   // No
            break ;
          default:
            return ;
          }
      }
}

void T4selfgate_paris_dlg::on_checkBox_fast_slow_banana_clicked(bool /*checked*/)
{
     update_checked_unchecked();
}

void T4selfgate_paris_dlg::on_lineEdit_name_editingFinished()
{

}
