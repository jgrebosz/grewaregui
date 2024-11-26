#include "t4doppler_dlg.h"
#include "ui_t4doppler_dlg.h"

#include <QMessageBox>
#include <QInputDialog>

#include <fstream>
#include <iostream>
#include <cmath>
#include "Tfile_helper.h"

#include "paths.h"
//extern paths path;

//************************************************************************************
T4doppler_dlg::T4doppler_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4doppler_dlg)
{
    ui->setupUi(this);
    init();
}
//************************************************************************************
T4doppler_dlg::~T4doppler_dlg()
{
    delete ui;
}


//*********************************************************************
void T4doppler_dlg::set_parameters( int mw, int lycca, int algorithm)
{
    ui->radioButton_projectile_tracked->setChecked(mw==1);
    ui->radioButton_projectile_not_tracked->setChecked(mw==2);
    ui->radioButton_projectile_use_when_available->setChecked(mw==3);
    ui->radio_xy_from_Si_target->setChecked(mw==4);

    ui->radioButton_lycca_ok->setChecked(lycca==1);
    ui->radioButton_lycca_only_segment->setChecked(lycca==2);
    ui->radioButton_lycca_broken->setChecked(lycca==3);
    ui->radioButton_lycca_use_when_available->setChecked(lycca==4);

    //	ui->radioButton_polar_coordinates->setChecked(algorithm==1);
    ui->radio_tracked_algorithm->setChecked(algorithm==1);
    ui->radio_basic_algorithm->setChecked(algorithm==2);
    on_radio_tracked_algorithm_toggled(algorithm==1);


    ui->radio_beta_manually->setChecked(beta_from_where != 1);
    ui->push_beta->setText(QString("beta =  %1 ").arg(beta_fixed_value));
    ui->push_beta->setEnabled(!beta_from_where) ;

    ui->push_Ed->setText(QString("Ed =  %1  [MeV/u]").arg(energy_deposit_in_target));


}
//*********************************************************************
void T4doppler_dlg::init()
{
    // reading the parameters from the disk
    string name = gpath.spy_dir + "options/doppler.options" ;
    ifstream plik(name.c_str()) ;

    int tracking_type = 1, lycca_type = 1, algorithm_type = 1;

    beta_from_where =  1 ;  // 1 = frs, 2 = manual

    beta_fixed_value =  0 ;
    energy_deposit_in_target  = 0;

    try
    {
        tracking_type =  (int) Nfile_helper::find_in_file(plik, "projectile_tracking");

        algorithm_type =  (int) Nfile_helper::find_in_file(plik, "doppler_corr_algorithm");
        beta_fixed_value =  Nfile_helper::find_in_file(plik, "beta_fixed_value");
        energy_deposit_in_target  =  Nfile_helper::find_in_file(plik, "energy_deposit_in_target");

        beta_from_where =  (int) Nfile_helper::find_in_file(plik, "beta_from_where");

    }
    catch(Tfile_helper_exception &kaps)
    {
        cout << "Error while reading file with the doppler correction option. File "
             << name << "\n" << kaps.message << endl;
        lycca_type  = 0;
    }

    if(plik)
    {
        set_parameters( tracking_type, lycca_type, algorithm_type ) ;
    }
    //else set_parameters( 1, 1, 1);
    else set_parameters( 4, 0, 1);
    plik.close() ;


    ui->radio_beta_manually->setChecked(beta_from_where == 1);
    set_beta_radio_chosen();

    ui->textLabel_result->setText(" ");

}
//*********************************************************************
void T4doppler_dlg::store_parameters()
{
    int tracking_type = 0, lycca_type = 0, algorithm_type = 0  ;

    if(ui->radioButton_projectile_tracked->isChecked())
        tracking_type = 1 ;
    else if(ui->radioButton_projectile_not_tracked->isChecked())
        tracking_type = 2 ;
    else if(ui->radioButton_projectile_use_when_available->isChecked())
        tracking_type = 3 ;
    else if(ui->radio_xy_from_Si_target->isChecked())
        tracking_type = 4;

    if(ui->radioButton_lycca_ok->isChecked())
        lycca_type = 1 ;
    else if (ui->radioButton_lycca_only_segment->isChecked())
        lycca_type = 2 ;
    else if (ui->radioButton_lycca_broken->isChecked())
        lycca_type = 3 ;
    else if (ui->radioButton_lycca_use_when_available->isChecked())
        lycca_type = 4 ;


    //    if
    //    (ui->radioButton_polar_coordinates->isChecked())
    //	    algorithm_type = 1 ;
    //    else
    if (ui->radio_tracked_algorithm->isChecked())
        algorithm_type = 1 ;
    else if (ui->radio_basic_algorithm->isChecked())
        algorithm_type = 2 ;

    // storing the parameters from the disk

    //    cout << "katalog = " << gpath.spy_dir << endl;
    string name = gpath.spy_dir + "options/doppler.options" ;
    ofstream plik(name.c_str()) ;


    plik
            << "// this file contains the information which algorythm should be used\n"
               "// to perform doppler corrections.\n"
               "// A. It is important to know if the projectile ion \n"
               "// was correctly tracked (by mutliwire chambers) before hitting the target\n"
               "// and\n"
               "// B. it is important to know the position where the product of the reaction \n"
               "// hit the lycca detector\n\n"

            << "projectile_tracking\t\t" <<  tracking_type
            << "\t// 1 = obligatory,   2 = NO tracking available, 3=use if available,  4= from the Si_det at target\n"
            << "lycca_tracking\t\t"
            << lycca_type
            << "\t\t// 1 = obligatory, 2 = only which segment , 3 = NO, 4=use if available\n"
            << "doppler_corr_algorithm\t\t"
            <<  algorithm_type
             << "\t// 1 = tracked  , 2= basic \n"
             << "\n\n// You can use the beta value from FRS (1)  you can give fixed value (2)"
             << "\n\nbeta_from_where\t\t" << beta_from_where
             <<    "\nbeta_fixed_value\t\t" << beta_fixed_value
                <<"\n\nenergy_deposit_in_target\t\t" << energy_deposit_in_target
               << endl;


    if(!plik)
    {
        // warning - error while writing parameters
    }
    plik.close() ;
}
//*********************************************************************
void T4doppler_dlg::set_energy_deposit()
{
    bool ok = false ;
    double value  = QInputDialog::getDouble
            ( this, "Type the value of energy deposit in the target",
              "Energy deposid [MeV/u] ",
              energy_deposit_in_target,
              0, 300, 6, &ok) ;
    if(ok)
    {
        energy_deposit_in_target = value ;
        ui->push_Ed->setText(QString("Ed =  %1  [MeV/u]").arg(energy_deposit_in_target));
    }
}
//*********************************************************************
void T4doppler_dlg::on_push_beta_clicked()     // set_beta()
{
    bool ok = false ;
    double value  = QInputDialog::getDouble
            ( this, "Type the value of beta",
              "beta ", beta_fixed_value, 0.0, 1.0, 6, &ok) ;
    if(ok)
    {
        beta_fixed_value = value ;
        ui->push_beta->setText(QString("beta =  %1 ").arg(beta_fixed_value));
    }
}
//*********************************************************************
void T4doppler_dlg::set_beta_radio_chosen()
{
    if(ui->radio_beta_manually->isChecked() )
    {
        beta_from_where = beta_type_fixed ;
    }
    else   beta_from_where = 0 ;

    // depending of situation - some buttons are enabled/disabled)
    ui->push_beta->setEnabled(beta_from_where == beta_type_fixed ) ;

}
//**********************************************************************
void T4doppler_dlg::make_picture_small()
{

    ui->pixmapLabel1->setText("No Picture now");
    adjustSize();
    ui->push_minimise->hide();
}
//**********************************************************************
void T4doppler_dlg::beta_payfield()
{

    //    Tbeta_dlg * dlg = new Tbeta_dlg;

    //    dlg->exec();

}

//*********************************************************************
void T4doppler_dlg::calculator_from_Tpn()
{
    bool ok = false ;
    double value  = QInputDialog::getDouble
            ( this, "Kinetic Energy per nucleon",
              "Type the value of Kinetic Energy per nucleon  [MeV/u]: ",
              0, 0.0, 10000, 6, &ok) ;
    if(ok)
    {
        double beta_result = sqrt( (value   * value  ) + (1863 * value)  ) /  (931.5 + value);
        ui->push_beta->setText(QString("beta =  %1 ").arg(beta_fixed_value));

        ui->textLabel_result->setText(QString("For Tpn = %1 MeV,     Beta =  %2").arg(value).arg(beta_result));
    }
}
//**************************************************************************************************
void T4doppler_dlg::on_push_minimise_clicked()
{
    make_picture_small();
}
//**************************************************************************************************
void T4doppler_dlg::accept()
{
    store_parameters();
    QDialog::accept();
}
//**************************************************************************************************
void T4doppler_dlg::on_radio_tracked_algorithm_toggled(bool checked)
{
    //    cout << "now on_radio_tracked_algorithm_toggled is " << checked << endl;
    ui->buttonGroup_lycca->setEnabled(checked);
    ui->buttonGroup_tracking->setEnabled(checked);
}
//**************************************************************************************************
void T4doppler_dlg::on_radio_beta_manually_toggled(bool /*checked*/ )
{
    set_beta_radio_chosen();
}
//**************************************************************************************************
void T4doppler_dlg::on_push_other_betas_clicked()
{
    beta_payfield();
}
//**************************************************************************************************
void T4doppler_dlg::on_push_Tpn_clicked()
{
    calculator_from_Tpn();
}
//**************************************************************************************************
void T4doppler_dlg::on_push_Ed_clicked()
{
    set_energy_deposit();
}
