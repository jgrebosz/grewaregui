#include "t4good_ger_dlg.h"
#include "ui_t4good_ger_dlg.h"
#include <qinputdialog.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace  std;
#include "paths.h"
#include "Tfile_helper.h"
//***************************************************************************************************
T4good_ger_dlg::T4good_ger_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4good_ger_dlg)
{
    ui->setupUi(this);
    init();
    ui->tabWidget->setCurrentIndex(2);
}
//***************************************************************************************************
T4good_ger_dlg::~T4good_ger_dlg()
{
    delete ui;
}
//***************************************************************************************************
void T4good_ger_dlg::init()
{

    on_push_default_clicked();

    // reading from the file
    string pname = gpath.options + "good_germanium.txt" ;
    ifstream plik(pname.c_str());
    if(!plik)
    {
        cout << "Cant open file " << pname << endl;
        return ;

    }

    try{

        // reading
        bool flag ;
        flag = (bool) Nfile_helper::find_in_file(plik, "increment_20MeV_cal_with_zero") ;
        ui->checkBox_call_req_20->setChecked(flag);

        flag = (bool) Nfile_helper::find_in_file(plik, "increment_4MeV_cal_with_zero") ;
        ui->checkBox_call_req_4->setChecked(flag);

        flag = (bool) Nfile_helper::find_in_file(plik, "increment_time_cal_with_zero") ;
        ui->checkBox_call_req_tim->setChecked(flag);

        //----------
        flag = (bool) Nfile_helper::find_in_file(plik, "good_20MeV_requires_threshold") ;
        ui->checkBox_good_en20->setChecked(flag);

        double ddd ;
        ddd =  Nfile_helper::find_in_file(plik, "en20MeV_threshold") ;
        ui->pushline_en20_good_threshold->setText(QString::number(ddd));
        ui->pushline_en20_good_threshold->setEnabled(flag);

        //-------
        bool flag_en4 = (bool) Nfile_helper::find_in_file(plik, "good_4MeV_requires_threshold") ;
        ui->checkBox_good_en4->setChecked(flag_en4);

        ddd =  Nfile_helper::find_in_file(plik, "en4MeV_threshold") ;
        ui->pushline_en4_good_threshold->setText(QString::number(ddd));
        ui->pushline_en4_good_threshold->setEnabled(flag_en4);

        // time
        flag = (bool) Nfile_helper::find_in_file(plik, "good_time_requires_threshold") ;
        ui->checkBox_good_time->setChecked(flag);


        ddd =  Nfile_helper::find_in_file(plik, "good_time_threshold_lower") ;
        ui->push_time_threshold_lower->setText(QString::number(ddd));
        ui->push_time_threshold_lower->setEnabled(flag);

        // old versions of the program had only the low threshold. Now we introduce also the upper

        try
        {
            ddd =  Nfile_helper::find_in_file(plik, "good_time_threshold_upper") ;
            ui->push_time_threshold_upper->setText(QString::number(ddd));
            ui->push_time_threshold_upper->setEnabled(flag);


            ddd =  Nfile_helper::find_in_file(plik, "en4MeV_threshold_upper") ;
            ui->pushline_en4_good_threshold_upper->setText(QString::number(ddd));
            ui->pushline_en4_good_threshold_upper->setEnabled(flag_en4);
        }
        catch(...)
        {
            // in case it is not provided (old version)
            ui->push_time_threshold_upper->setText(QString::number(99999));
            ui->pushline_en4_good_threshold_upper->setText("16000");
            ui->pushline_en4_good_threshold_upper->setEnabled(flag);
        }


    }
    catch(Tfile_helper_exception k)
    {
        cout << "Error while reading " << pname << "\n" << k.message << endl;
    }

}
//***************************************************************************************************
void T4good_ger_dlg::accept()
{
    // save to the file
    // reading from the file
    string pname = gpath.options + "good_germanium.txt" ;
    ofstream plik(pname.c_str());
    if(!plik)
    {


    }


    // writing
    plik
            << "// This !!!! file contains specification about good germanium signals\n"
            << "\nincrement_20MeV_cal_with_zero\t"
            << ui->checkBox_call_req_20->isChecked()

            << "\nincrement_4MeV_cal_with_zero\t"
            << ui->checkBox_call_req_4->isChecked()

            << "\nincrement_time_cal_with_zero\t"
            << ui->checkBox_call_req_tim->isChecked()

               //----------
            << "\ngood_20MeV_requires_threshold\t"
            <<  ui->checkBox_good_en20->isChecked()
             << "\nen20MeV_threshold\t"
             <<  ui->pushline_en20_good_threshold->text().toStdString()

                 //-------
              << "\n\ngood_4MeV_requires_threshold\t"
              <<  ui->checkBox_good_en4->isChecked()

               << "\nen4MeV_threshold\t"
               << ui->pushline_en4_good_threshold->text().toStdString()

               << "\nen4MeV_threshold_upper\t"
               << ui->pushline_en4_good_threshold_upper->text().toStdString()

                  // time
               << "\n\ngood_time_requires_threshold\t"
               <<  ui->checkBox_good_time->isChecked()

                << "\n\ngood_time_threshold_lower\t"
                <<  ui->push_time_threshold_lower->text().toStdString()

                 << "\n\ngood_time_threshold_upper\t"
                 <<  ui->push_time_threshold_upper->text().toStdString()
                  << endl;


    QDialog::accept();
}
//***************************************************************************************************
void T4good_ger_dlg::on_push_default_clicked()
{
    ui->checkBox_call_req_20->setChecked(false);
    ui->checkBox_call_req_4->setChecked(false);
    ui->checkBox_call_req_tim->setChecked(false);

    ui->checkBox_good_en20->setChecked(false);
    ui->pushline_en20_good_threshold->setText("0");
    ui->pushline_en20_good_threshold->setEnabled(false);

    ui->checkBox_good_en4->setChecked(true);
    ui->pushline_en4_good_threshold->setText("100");
    ui->pushline_en4_good_threshold->setEnabled(true);

    ui->pushline_en4_good_threshold_upper->setText("8192");
    ui->pushline_en4_good_threshold_upper->setEnabled(true);

    ui->checkBox_good_time->setChecked(false);
    ui->push_time_threshold_lower->setText("0");
    ui->push_time_threshold_lower->setEnabled(false);

    ui->push_time_threshold_upper->setText("99999");
    ui->push_time_threshold_upper->setEnabled(false);

}
//***************************************************************************************************
void T4good_ger_dlg::on_pushline_en20_good_threshold_clicked()
{
    bool ok = false ;
    double value  = QInputDialog::getDouble
            ( this, "Type the value of threshold for signal 'energy 20MeV'",
              "threshold ",
              ui->pushline_en20_good_threshold->text().toDouble(),
              0.0, 20000, 1, &ok) ;
    if(ok)
    {
        ui->pushline_en20_good_threshold->setText(QString("%1").arg(value));
    }
}
//***************************************************************************************************
void T4good_ger_dlg::on_pushline_en4_good_threshold_clicked()
{
    bool ok = false ;
    double value  = QInputDialog::getDouble
            ( this, "Type the value of lower threshold for signal 'energy 4MeV'",
              "threshold ",
              ui->pushline_en4_good_threshold->text().toDouble(),
              0.0, 8192, 1, &ok) ;
    if(ok)
    {
        ui->pushline_en4_good_threshold->setText(QString("%1").arg(value));
    }
}
//***************************************************************************************************
void T4good_ger_dlg::on_pushline_en4_good_threshold_upper_clicked()
{
    bool ok = false ;
    double value  = QInputDialog::getDouble
            ( this, "Type the value of upper_threshold for signal 'energy 4MeV'",
              "threshold ",
              ui->pushline_en4_good_threshold_upper->text().toDouble(),
              0.0, 8192, 1, &ok) ;
    if(ok)
    {
        ui->pushline_en4_good_threshold_upper->setText(QString("%1").arg(value));
    }
}
//***************************************************************************************************
void T4good_ger_dlg::on_push_time_threshold_lower_clicked()
{
    bool ok = false ;
    double value  = QInputDialog::getDouble
            ( this, "Type the value of the lower threshold for signal 'time'",
              "threshold ",
              ui->push_time_threshold_lower->text().toDouble() ,
              0.0, 30000, 1, &ok) ;
    if(ok)
    {
        ui->push_time_threshold_lower->setText(QString("%1").arg(value));
    }
}
//***************************************************************************************************
void T4good_ger_dlg::on_push_time_threshold_upper_clicked()
{
    bool ok = false ;
    double value  = QInputDialog::getDouble
            ( this, "Type the value of the upper threshold for signal 'time'",
              "threshold ",
              ui->push_time_threshold_upper->text().toDouble() ,
              0.0, 32000, 1, &ok) ;
    if(ok)
    {
        ui->push_time_threshold_upper->setText(QString("%1").arg(value));
    }
}
//***************************************************************************************************

void T4good_ger_dlg::on_checkBox_good_en20_clicked(bool checked)
{
    ui->pushline_en20_good_threshold->setEnabled(checked);
}
//***************************************************************************************************
void T4good_ger_dlg::on_checkBox_good_en4_clicked(bool checked)
{
    ui->pushline_en4_good_threshold->setEnabled(checked);
    ui->pushline_en4_good_threshold_upper->setEnabled(checked);
}
//***************************************************************************************************
void T4good_ger_dlg::on_checkBox_good_time_clicked(bool checked)
{
    ui->push_time_threshold_lower->setEnabled(checked);
    ui->push_time_threshold_upper->setEnabled(checked);
}
