#include "t4active_stopper_dlg.h"
#include "ui_t4active_stopper_dlg.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
#include "paths.h"
#include "Tfile_helper.h"
#include <QMessageBox>
//*************************************************************************************************
T4active_stopper_dlg::T4active_stopper_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4active_stopper_dlg)
{
    ui->setupUi(this);
    init();
}
//*************************************************************************************************
T4active_stopper_dlg::~T4active_stopper_dlg()
{
    delete ui;
}
//*************************************************************************************************
void T4active_stopper_dlg::accept()
{
    // save to the file
    string pname = gpath.options + "active_stopper_settings.txt" ;

    ofstream plik(pname.c_str() );
    if(!plik)
    {
        cout << "Can't open file " << pname << " for storeing the Active stopper options" << endl;

    }

    // writing
    plik
            << "// This file contains specification about active stopper settings (options) \n"
            << "\nimplantation_decay_time_gate_low\t"
            << ui->lineEdit_time_low->text().toStdString()

            << "\nimplantation_decay_time_gate_high\t"
            << ui->lineEdit_time_high->text().toStdString()

            <<"\n\n // radio buttons for pixel search - only one of them can be true (in an time)\n"
           << "only_same_pixel\t"
           << ui->radio_only_same_pixel->isChecked()
           << "\nalso_8_neighbours\t"
           <<  ui->radio_also_8_neighbours->isChecked()

            << "\n\n// radio buttons for XY information to be used"
            << "\nstopper_L_xy_both \t"
            << ui->radio_L_xy_both->isChecked()
            << "\nstopper_L_x_only\t"
            << ui->radio_L_x_only->isChecked()
            << "\nstopper_L_y_only\t"
            << ui->radio_L_y_only->isChecked()


            << "\n\nstopper_M_xy_both \t"
            << ui->radio_M_xy_both->isChecked()
            << "\nstopper_M_x_only\t"
            << ui->radio_M_x_only->isChecked()
            << "\nstopper_M_y_only\t"
            << ui->radio_M_y_only->isChecked()


            << "\n\nstopper_R_xy_both \t"
            << ui->radio_R_xy_both->isChecked()
            << "\nstopper_R_x_only\t"
            << ui->radio_R_x_only->isChecked()
            << "\nstopper_R_y_only\t"
            << ui->radio_R_y_only->isChecked()

               // New style, without the word "stopper"
               // this above is for the compatibility

            << "\nL_xy_both \t"
            << ui->radio_L_xy_both->isChecked()
            << "\nL_x_only\t"
            << ui->radio_L_x_only->isChecked()
            << "\nL_y_only\t"
            << ui->radio_L_y_only->isChecked()


            << "\n\nM_xy_both \t"
            << ui->radio_M_xy_both->isChecked()
            << "\nM_x_only\t"
            << ui->radio_M_x_only->isChecked()
            << "\nM_y_only\t"
            << ui->radio_M_y_only->isChecked()


            << "\n\nR_xy_both \t"
            << ui->radio_R_xy_both->isChecked()
            << "\nR_x_only\t"
            << ui->radio_R_x_only->isChecked()
            << "\nR_y_only\t"
            << ui->radio_R_y_only->isChecked()



            << "\n\nthreshold_for_implantation_energy\t"
            << ui->lineEdit_energy_impl_threshold->text().toStdString()

            << "\n\nthreshold_upper_for_implantation_energy\t"
            << ui->lineEdit_energy_impl_threshold_upper->text().toStdString()

            << "\nthreshold_for_decay_energy\t"
            << ui->lineEdit_energy_decay_threshold->text().toStdString()

            << "\nthreshold_upper_for_decay_energy\t"
            << ui->lineEdit_energy_decay_threshold_upper->text().toStdString()
               // good
            << "\nsci42_acceptable_energy_low\t"
            << ui->lineEdit_sci42_low->text().toStdString()

            << "\nsci42_acceptable_energy_high\t"
            << ui->lineEdit_sci42_high->text().toStdString()

               // veto
            << "\nsci43_veto_energy_low\t"
            << ui->lineEdit_sci43_low->text().toStdString()

            << "\nsci43_veto_energy_high\t"
            << ui->lineEdit_sci43_high->text().toStdString()

            << "\n\naccept_more_implantations\t"
            << ui->radio_accept_more_implantations->isChecked()

            << "\n\nalgorithm_for_implantation_position_calculation_is_mean\t"   // otherwise MAX
            << ui->radio_position_algorithm_mean->isChecked()


            << "\n\ntype_of_zet\t" << ui->comboBox_zet->currentIndex()
            << "\nsome_zet_low\t" << ui->lineEdit_somezet_low->text().toStdString()
            << "\nsome_zet_high\t" << ui->lineEdit_somezet_high->text().toStdString()

            << "\n\ntype_of_aoq\t" << ui->comboBox_aoq->currentIndex()
            << "\nsome_aoq_low\t" << ui->lineEdit_someaoq_low->text().toStdString()
            << "\nsome_aoq_high\t" << ui->lineEdit_someaoq_high->text().toStdString()

            << endl;

    //==========================
    QDialog::accept();
}

//*********************************************************************
void T4active_stopper_dlg::init()
{
    set_default();


    // reading from the file

    string pname = gpath.options + "active_stopper_settings.txt" ;
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
        // writing

        double ddd =  Nfile_helper::find_in_file(plik, "implantation_decay_time_gate_low") ;
        ui->lineEdit_time_low->setText(QString::number(ddd));

        ddd =  Nfile_helper::find_in_file(plik, "implantation_decay_time_gate_high") ;
        ui->lineEdit_time_high->setText(QString::number(ddd));

        bool flag ;
        flag = (bool) Nfile_helper::find_in_file(plik, "only_same_pixel");
        ui->radio_only_same_pixel->setChecked(flag);

        flag = (bool) Nfile_helper::find_in_file(plik, "also_8_neighbours");
        ui->radio_also_8_neighbours->setChecked(flag);

        // A
        flag = (bool) Nfile_helper::find_in_file(plik, "stopper_L_xy_both");
        ui->radio_L_xy_both->setChecked(flag);

        flag = (bool) Nfile_helper::find_in_file(plik, "stopper_L_x_only");
        ui->radio_L_x_only->setChecked(flag);

        flag = (bool) Nfile_helper::find_in_file(plik, "stopper_L_y_only");
        ui->radio_L_y_only->setChecked(flag);

        //    B
        flag = (bool) Nfile_helper::find_in_file(plik, "stopper_M_xy_both");
        ui->radio_M_xy_both->setChecked(flag);

        flag = (bool) Nfile_helper::find_in_file(plik, "stopper_M_x_only");
        ui->radio_M_x_only->setChecked(flag);

        flag = (bool) Nfile_helper::find_in_file(plik, "stopper_M_y_only");
        ui->radio_M_y_only->setChecked(flag);

        // C
        flag = (bool) Nfile_helper::find_in_file(plik, "stopper_R_xy_both");
        ui->radio_R_xy_both->setChecked(flag);

        flag = (bool) Nfile_helper::find_in_file(plik, "stopper_R_x_only");
        ui->radio_R_x_only->setChecked(flag);

        flag = (bool) Nfile_helper::find_in_file(plik, "stopper_R_y_only");
        ui->radio_R_y_only->setChecked(flag);
        //---------

        ddd =  Nfile_helper::find_in_file(plik, "threshold_for_implantation_energy") ;
        ui->lineEdit_energy_impl_threshold->setText(QString::number(ddd));

        ddd =  Nfile_helper::find_in_file(plik, "threshold_upper_for_implantation_energy") ;
        ui->lineEdit_energy_impl_threshold_upper->setText(QString::number(ddd));

        ddd =  Nfile_helper::find_in_file(plik, "threshold_for_decay_energy") ;
        ui->lineEdit_energy_decay_threshold->setText(QString::number(ddd));
        ddd =  Nfile_helper::find_in_file(plik, "threshold_upper_for_decay_energy") ;
        ui->lineEdit_energy_decay_threshold_upper->setText(QString::number(ddd));
        //-------

        // good
        ddd =  Nfile_helper::find_in_file(plik, "sci42_acceptable_energy_low");
        ui->lineEdit_sci42_low->setText(QString::number(ddd));

        ddd =  Nfile_helper::find_in_file(plik, "sci42_acceptable_energy_high");
        ui->lineEdit_sci42_high->setText(QString::number(ddd));

        // veto
        ddd =  Nfile_helper::find_in_file(plik, "sci43_veto_energy_low");
        ui->lineEdit_sci43_low->setText(QString::number(ddd));

        ddd =  Nfile_helper::find_in_file(plik, "sci43_veto_energy_high");
        ui->lineEdit_sci43_high->setText(QString::number(ddd));


        flag = (bool) Nfile_helper::find_in_file(plik, "accept_more_implantations");
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


    }
    catch(Tfile_helper_exception k)
    {
        cout << "Error while reading " << pname << "\n" << k.message << endl;
    }

}
//********************************************************************
//*******************************************************************
void T4active_stopper_dlg::set_default()
{
    ui->lineEdit_time_low->setText("0");
    ui->lineEdit_time_high->setText("1000");

    ui->radio_only_same_pixel->setChecked(true);
    ui->radio_also_8_neighbours->setChecked(false);

    ui->radio_L_xy_both->setChecked(true);
    ui->radio_L_x_only->setChecked(false);
    ui->radio_L_y_only->setChecked(false);

    ui->radio_M_xy_both->setChecked(true);
    ui->radio_M_x_only->setChecked(false);
    ui->radio_M_y_only->setChecked(false);

    ui->radio_R_xy_both->setChecked(true);
    ui->radio_R_x_only->setChecked(false);
    ui->radio_R_y_only->setChecked(false);

    ui->lineEdit_energy_impl_threshold->setText("1");
    ui->lineEdit_energy_decay_threshold->setText("1");
    ui->lineEdit_energy_impl_threshold_upper->setText("8191");
    ui->lineEdit_energy_decay_threshold_upper->setText("8191");

    // good
    ui->lineEdit_sci42_low->setText("0");
    ui->lineEdit_sci42_high->setText("8192");

    // veto
    ui->lineEdit_sci43_low->setText("8191");
    ui->lineEdit_sci43_high->setText("8191");



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


}

void T4active_stopper_dlg::on_buttonHelp_clicked()
{
    string mess = "Here you may specify how the active stopper works. "
            "You may define the time gate to match implantation and the decay. "
            "You may define where to search the implantation and decay subevents. "
            "You may also define how to calculate the position";
    QMessageBox::information( this,
                              "Info",
                              mess.c_str(),
                              QMessageBox::Ok,
                              QMessageBox::NoButton,
                              QMessageBox::NoButton);
}
