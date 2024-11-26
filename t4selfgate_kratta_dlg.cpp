#include "t4selfgate_kratta_dlg.h"
#include "ui_t4selfgate_kratta_dlg.h"
#include "t4picture_angle.h"
#include <string>
using namespace std;
#include "Tself_gate_kratta_descr.h"
#include <QMessageBox>
#include "Tfile_helper.h"
//****************************************************************
T4selfgate_kratta_dlg::T4selfgate_kratta_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4selfgate_kratta_dlg)
{
    ui->setupUi(this);
}
//****************************************************************
T4selfgate_kratta_dlg::~T4selfgate_kratta_dlg()
{
    delete ui;
}
//****************************************************************

//****************************************************************
void T4selfgate_kratta_dlg::init()
{
    //this_is_germanium = false ; //true ; // for false - means this is Hector selfgate
    update_checked_unchecked();
}

//****************************************************************
void T4selfgate_kratta_dlg::update_checked_unchecked()
{

    ui->lineEdit_pd0_low->setEnabled(ui->checkBox_energy_pd0->isChecked());
    ui->lineEdit_pd0_high->setEnabled(ui->checkBox_energy_pd0->isChecked());

    ui->lineEdit_pd1_low->setEnabled(ui->checkBox_energy_pd1->isChecked());
    ui->lineEdit_pd1_high->setEnabled(ui->checkBox_energy_pd1->isChecked());

    ui->lineEdit_pd2_low->setEnabled(ui->checkBox_energy_pd2->isChecked());
    ui->lineEdit_pd2_high->setEnabled(ui->checkBox_energy_pd2->isChecked());


    ui->lineEdit_pd0_time_low->setEnabled(ui->checkBox_time_pd0->isChecked());
    ui->lineEdit_pd0_time_high->setEnabled(ui->checkBox_time_pd0->isChecked());

    ui->lineEdit_pd1_time_low->setEnabled(ui->checkBox_time_pd1->isChecked());
    ui->lineEdit_pd1_time_high->setEnabled(ui->checkBox_time_pd1->isChecked());

    ui->lineEdit_pd2_time_low->setEnabled(ui->checkBox_time_pd2->isChecked());
    ui->lineEdit_pd2_time_high->setEnabled(ui->checkBox_time_pd2->isChecked());





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
void T4selfgate_kratta_dlg::set_parameters( const Tself_gate_kratta_descr *d )
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

    ui->checkBox_energy_pd0->setChecked(d->enable_pd0_cal_gate) ;
    ui->lineEdit_pd0_low->setText(QString::number(d->pd0_cal_gate [0]));
    ui->lineEdit_pd0_high->setText(QString::number(d->pd0_cal_gate [1]));


    ui->checkBox_energy_pd1->setChecked(d->enable_pd1_cal_gate) ;
    ui->lineEdit_pd1_low->setText(QString::number(d->pd1_cal_gate [0]));
    ui->lineEdit_pd1_high->setText(QString::number(d->pd1_cal_gate [1]));

    ui->checkBox_energy_pd2->setChecked(d->enable_pd0_cal_gate) ;
    ui->lineEdit_pd2_low->setText(QString::number(d->pd2_cal_gate [0]));
    ui->lineEdit_pd2_high->setText(QString::number(d->pd2_cal_gate [1]));

// time
    ui->checkBox_time_pd0->setChecked(d->enable_pd0_time_cal_gate) ;
    ui->lineEdit_pd0_time_low->setText(QString::number(d->pd0_time_cal_gate [0]));
    ui->lineEdit_pd0_time_high->setText(QString::number(d->pd0_time_cal_gate [1]));

    ui->checkBox_time_pd1->setChecked(d->enable_pd1_time_cal_gate) ;
    ui->lineEdit_pd1_time_low->setText(QString::number(d->pd1_time_cal_gate [0]));
    ui->lineEdit_pd1_time_high->setText(QString::number(d->pd1_time_cal_gate [1]));

    ui->checkBox_time_pd2->setChecked(d->enable_pd2_time_cal_gate) ;
    ui->lineEdit_pd2_time_low->setText(QString::number(d->pd2_time_cal_gate [0]));
    ui->lineEdit_pd2_time_high->setText(QString::number(d->pd2_time_cal_gate [1]));




    //--------------
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
void T4selfgate_kratta_dlg::get_parameters( Tself_gate_kratta_descr * d )
{

    string s = d->name = ui->lineEdit_name->text().toStdString();
    // if there is no extension, add it

    string ext = (d->sg_type).give_type_extension() ;
    string::size_type i = s.find(ext);
    if(i == string::npos)
    {
        d->name += ext;
    }

    d->enable_pd0_cal_gate = ui->checkBox_energy_pd0->isChecked() ;
    d-> pd0_cal_gate[0] = ui->lineEdit_pd0_low->text().toDouble();
    d-> pd0_cal_gate[1] = ui->lineEdit_pd0_high->text().toDouble();

    d->enable_pd1_cal_gate = ui->checkBox_energy_pd1->isChecked() ;
    d-> pd1_cal_gate[0] = ui->lineEdit_pd1_low->text().toDouble();
    d-> pd1_cal_gate[1] = ui->lineEdit_pd1_high->text().toDouble();

    d->enable_pd2_cal_gate = ui->checkBox_energy_pd2->isChecked() ;
    d-> pd2_cal_gate[0] = ui->lineEdit_pd2_low->text().toDouble();
    d-> pd2_cal_gate[1] = ui->lineEdit_pd2_high->text().toDouble();

    // time

    d->enable_pd0_time_cal_gate = ui->checkBox_time_pd0->isChecked() ;
    d-> pd0_time_cal_gate[0] = ui->lineEdit_pd0_time_low->text().toDouble();
    d-> pd0_time_cal_gate[1] = ui->lineEdit_pd0_time_high->text().toDouble();

    d->enable_pd1_time_cal_gate = ui->checkBox_time_pd1->isChecked() ;
    d-> pd1_time_cal_gate[0] = ui->lineEdit_pd1_time_low->text().toDouble();
    d-> pd1_time_cal_gate[1] = ui->lineEdit_pd1_time_high->text().toDouble();

    d->enable_pd2_time_cal_gate = ui->checkBox_time_pd2->isChecked() ;
    d-> pd2_time_cal_gate[0] = ui->lineEdit_pd2_time_low->text().toDouble();
    d-> pd2_time_cal_gate[1] = ui->lineEdit_pd2_time_high->text().toDouble();

    //---------------


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
void T4selfgate_kratta_dlg::accept()
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

void T4selfgate_kratta_dlg::on_pushButton_OK_clicked()
{
    accept();
}
//********************************************************************************************
void T4selfgate_kratta_dlg::on_pushButton_Cancel_clicked()
{
    reject();
}
//********************************************************************************************
void T4selfgate_kratta_dlg::on_checkBox_geom_theta_clicked()
{
    update_checked_unchecked();
}
//********************************************************************************************
void T4selfgate_kratta_dlg::on_checkBox_geom_phi_clicked()
{
    update_checked_unchecked();
}
//********************************************************************************************
void T4selfgate_kratta_dlg::on_checkBox_gp_theta_clicked()
{
    update_checked_unchecked();
}
//********************************************************************************************
void T4selfgate_kratta_dlg::on_checkBox_gp_phi_clicked()
{
    update_checked_unchecked();
}
//********************************************************************************************

void T4selfgate_kratta_dlg::on_pushButton_picture_clicked()
{
    T4picture_angle dlg;
    dlg.exec();
}

void T4selfgate_kratta_dlg::on_checkBox_energy_pd0_clicked()
{
    update_checked_unchecked();
}

void T4selfgate_kratta_dlg::on_checkBox_energy_pd1_clicked()
{
    update_checked_unchecked();
}

void T4selfgate_kratta_dlg::on_checkBox_energy_pd2_clicked()
{
    update_checked_unchecked();
}

void T4selfgate_kratta_dlg::on_checkBox_time_pd0_clicked()
{
    update_checked_unchecked();
}

void T4selfgate_kratta_dlg::on_checkBox_time_pd1_clicked()
{
    update_checked_unchecked();
}

void T4selfgate_kratta_dlg::on_checkBox_time_pd2_clicked()
{
    update_checked_unchecked();
}
