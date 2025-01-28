#include "t4selfgate_miniball_dlg.h"
#include "ui_t4selfgate_miniball_dlg.h"

#include <string>
using namespace std;

#include "t4picture_angle.h"
#include "paths.h"
#include <QMessageBox>


T4selfgate_miniball_dlg::T4selfgate_miniball_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4selfgate_miniball_dlg)
{
    ui->setupUi(this);
    update_checked_unchecked();

}

T4selfgate_miniball_dlg::~T4selfgate_miniball_dlg()
{
    delete ui;
}
//****************************************************************
void T4selfgate_miniball_dlg::on_pushButton_angle_picture_clicked()
{
    T4picture_angle dlg;
    dlg.exec();
}
//****************************************************************
void T4selfgate_miniball_dlg::update_checked_unchecked()
{

    ui->lineEdit_core_energy_low->setEnabled(ui->checkBox_core_energy->isChecked());
    ui->lineEdit_core_energy_high->setEnabled(ui->checkBox_core_energy->isChecked());

    ui->lineEdit_time_low->setEnabled(ui->checkBox_time->isChecked());
    ui->lineEdit_time_high->setEnabled(ui->checkBox_time->isChecked());

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

    // this is mutliplicity inside one cryostat --------------------------

    ui->lineEdit_clus_mult_low->setEnabled(ui->checkBox_mult->isChecked());
    ui->lineEdit_clus_mult_high->setEnabled(ui->checkBox_mult->isChecked());

}
//********************************************************************


void T4selfgate_miniball_dlg::set_parameters( const Tself_gate_mib_descr *d )
{

    string nnn = d->name;
    string::size_type i = nnn.find(".self_gate_mib_crystal");
    if(i != string::npos)
    {
        // nnn.erase(i) ;    // always give with extension
    }

    ui->lineEdit_name->setText(nnn.c_str());
    ui->checkBox_core_energy->setChecked(d->enable_core_energy_gate) ;
    ui->lineEdit_core_energy_low->setText(QString::number(d-> core_energy_gate[0]));
    ui->lineEdit_core_energy_high->setText(QString::number(d-> core_energy_gate[1]));


    ui->checkBox_time->setChecked(d-> enable_time_gate) ;
    ui->lineEdit_time_low->setText(QString::number(d-> time_gate[0]));
    ui->lineEdit_time_high->setText(QString::number(d-> time_gate[1]));

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

    // this is the multiplicity inside the cryostat where this crystal belongs
    ui->checkBox_mult->setChecked(d-> enable_mult_in_cryostat_gate);
    ui->lineEdit_clus_mult_low->setText(QString::number(d->mult_in_cryostat_gate[0]));
    ui->lineEdit_clus_mult_high->setText(QString::number(d->mult_in_cryostat_gate[1]));

    update_checked_unchecked();

}
//*******************************************************************
void T4selfgate_miniball_dlg::get_parameters( Tself_gate_mib_descr * d )
{

    string s = d->name = ui->lineEdit_name->text().toStdString();
    // if there is no extension, add it
    if(s.find(".self_gate_mib_crystal") == string::npos)
    {
        d->name += ".self_gate_mib_crystal" ;
    }

    d->enable_core_energy_gate = ui->checkBox_core_energy->isChecked() ;
    d-> core_energy_gate[0] = ui->lineEdit_core_energy_low->text().toDouble();
    d-> core_energy_gate[1] = ui->lineEdit_core_energy_high->text().toDouble();


    d-> enable_time_gate = ui->checkBox_time->isChecked() ;
    d-> time_gate[0] = ui->lineEdit_time_low->text().toDouble();
    d-> time_gate[1] = ui->lineEdit_time_high->text().toDouble();

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

    // this is the multiplicity inside the cryostat where this crystal belongs
    d-> enable_mult_in_cryostat_gate = ui->checkBox_mult->isChecked();
    d->mult_in_cryostat_gate[0] = ui->lineEdit_clus_mult_low->text().toDouble();
    d->mult_in_cryostat_gate[1] = ui->lineEdit_clus_mult_high->text().toDouble();

}
//**********************************************************************************************
void T4selfgate_miniball_dlg::accept()
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
//**********************************************************************************************
void T4selfgate_miniball_dlg::on_checkBox_core_energy_clicked()
{
    update_checked_unchecked();
}
//**********************************************************************************************
void T4selfgate_miniball_dlg::on_checkBox_time_clicked()
{
    update_checked_unchecked();
}
//**********************************************************************************************
void T4selfgate_miniball_dlg::on_checkBox_geom_theta_clicked()
{
    update_checked_unchecked();
}
//**********************************************************************************************
void T4selfgate_miniball_dlg::on_checkBox_geom_phi_clicked()
{
    update_checked_unchecked();
}
//**********************************************************************************************
void T4selfgate_miniball_dlg::on_checkBox_gp_theta_clicked()
{
    update_checked_unchecked();
}
//**********************************************************************************************
void T4selfgate_miniball_dlg::on_checkBox_gp_phi_clicked()
{
    update_checked_unchecked();
}
//**********************************************************************************************
void T4selfgate_miniball_dlg::on_checkBox_mult_clicked()
{
    update_checked_unchecked();
}
//**********************************************************************************************
