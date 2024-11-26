#include "t4good_kratta_dlg.h"
#include "ui_t4good_kratta_dlg.h"
#include <qinputdialog.h>
#include <string>
#include "paths.h"
#include <iostream>
#include <fstream>
#include "Tfile_helper.h"

T4good_kratta_dlg::T4good_kratta_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4good_kratta_dlg)
{
    ui->setupUi(this);

    // set the intial conditions
    ui->pushButton_pd0_low->setEnabled(false);
    ui->pushButton_pd0_high->setEnabled(false);
    ui->pushButton_pd1_low->setEnabled(false);
    ui->pushButton_pd1_high->setEnabled(false);
    ui->pushButton_pd2_low->setEnabled(false);
    ui->pushButton_pd2_high->setEnabled(false);

    ui->pushButton_pd0_low->setText("1");
    ui->pushButton_pd0_high->setText("1500");
    ui->pushButton_pd1_low->setText("1");
    ui->pushButton_pd1_high->setText("1500");
    ui->pushButton_pd2_low->setText("1");
    ui->pushButton_pd2_high->setText("1500");


    // read the file
    // reading from the file
    string pname = gpath.options + "good_kratta.txt" ;
    ifstream plik(pname.c_str());
    if(!plik)
    {
        cout << "Cant open file " << pname << endl;
        return ;

    }

    try{

        // reading
        bool flag {};
        double ddd {};

        //----------
        flag = (bool) Nfile_helper::find_in_file(plik, "good_pd0_time_cal_requires_threshold") ;
        ui->checkBox_pd0->setChecked(flag);

        ddd =  Nfile_helper::find_in_file(plik, "pd0_time_cal_lower_threshold") ;
        ui->pushButton_pd0_low->setText(QString::number(ddd));

        ddd =  Nfile_helper::find_in_file(plik, "pd0_time_cal_upper_threshold") ;
        ui->pushButton_pd0_high->setText(QString::number(ddd));

        //-------
        flag = (bool) Nfile_helper::find_in_file(plik, "good_pd1_time_cal_requires_threshold") ;
        ui->checkBox_pd1->setChecked(flag);

        ddd =  Nfile_helper::find_in_file(plik, "pd1_time_cal_lower_threshold") ;
        ui->pushButton_pd1_low->setText(QString::number(ddd));

        ddd =  Nfile_helper::find_in_file(plik, "pd1_time_cal_upper_threshold") ;
        ui->pushButton_pd1_high->setText(QString::number(ddd));

        //-------
        flag = (bool) Nfile_helper::find_in_file(plik, "good_pd2_time_cal_requires_threshold") ;
        ui->checkBox_pd2->setChecked(flag);

        ddd =  Nfile_helper::find_in_file(plik, "pd2_time_cal_lower_threshold") ;
        ui->pushButton_pd2_low->setText(QString::number(ddd));

        ddd =  Nfile_helper::find_in_file(plik, "pd2_time_cal_upper_threshold") ;
        ui->pushButton_pd2_high->setText(QString::number(ddd));

        //-------
    }
    catch(Tfile_helper_exception k)
    {
        cout << "Error while reading " << pname << "\n" << k.message << endl;
    }

}
//*******************************************************************************
T4good_kratta_dlg::~T4good_kratta_dlg()
{
    delete ui;
}
//----- pd0 -----------------------------
//*******************************************************************************
void T4good_kratta_dlg::on_checkBox_pd0_toggled(bool checked)
{

    ui->pushButton_pd0_low->setEnabled(checked);
    ui->pushButton_pd0_high->setEnabled(checked);
}
//*******************************************************************************
void T4good_kratta_dlg::on_pushButton_pd0_low_clicked()
{
    bool ok = false ;
    double value  = QInputDialog::getDouble
            ( this, "Type the value of the lower threshold for signal 'pd0_time_cal'",
              "lower threshold = ",
              ui->pushButton_pd0_low->text().toDouble() ,
              0.0, 3000, 1, &ok) ;
    if(ok)
    {
        ui->pushButton_pd0_low->setText(QString("%1").arg(value));
    }
}
//*******************************************************************************
void T4good_kratta_dlg::on_pushButton_pd0_high_clicked()
{
    bool ok = false ;
    double value  = QInputDialog::getDouble
            ( this, "Type the value of the upper threshold for signal 'pd0_time_cal'",
              "upper threshold = ",
              ui->pushButton_pd0_high->text().toDouble() ,
              0.0, 3000, 1, &ok) ;
    if(ok)
    {
        ui->pushButton_pd0_high->setText(QString("%1").arg(value));
    }
}

//------ pd1 ---------------------
//*******************************************************************************
void T4good_kratta_dlg::on_checkBox_pd1_toggled(bool checked)
{
    ui->pushButton_pd1_low->setEnabled(checked);
    ui->pushButton_pd1_high->setEnabled(checked);

}
//*******************************************************************************
void T4good_kratta_dlg::on_pushButton_pd1_low_clicked()
{
    bool ok = false ;
    double value  = QInputDialog::getDouble
            ( this, "Type the value of the lower threshold for signal 'pd1_time_cal'",
              "lower threshold = ",
              ui->pushButton_pd1_low->text().toDouble() ,
              0.0, 3000, 1, &ok) ;
    if(ok)
    {
        ui->pushButton_pd1_low->setText(QString("%1").arg(value));
    }
}
//*******************************************************************************
void T4good_kratta_dlg::on_pushButton_pd1_high_clicked()
{
    bool ok = false ;
    double value  = QInputDialog::getDouble
            ( this, "Type the value of the upper threshold for signal 'pd1_time_cal'",
              "upper threshold = ",
              ui->pushButton_pd1_high->text().toDouble() ,
              0.0, 3000, 1, &ok) ;
    if(ok)
    {
        ui->pushButton_pd1_high->setText(QString("%1").arg(value));
    }
}
//------ pd2 ---------------------
//*******************************************************************************
void T4good_kratta_dlg::on_checkBox_pd2_toggled(bool checked)
{
    ui->pushButton_pd2_low->setEnabled(checked);
    ui->pushButton_pd2_high->setEnabled(checked);
}
//*******************************************************************************
void T4good_kratta_dlg::on_pushButton_pd2_low_clicked()
{
    bool ok = false ;
    double value  = QInputDialog::getDouble
            ( this, "Type the value of the lower threshold for signal 'pd2_time_cal'",
              "lower threshold = ",
              ui->pushButton_pd2_low->text().toDouble() ,
              0.0, 3000, 1, &ok) ;
    if(ok)
    {
        ui->pushButton_pd2_low->setText(QString("%1").arg(value));
    }
}
//*******************************************************************************
void T4good_kratta_dlg::on_pushButton_pd2_high_clicked()
{
    bool ok = false ;
    double value  = QInputDialog::getDouble
            ( this, "Type the value of the upper threshold for signal 'pd2_time_cal'",
              "upper threshold = ",
              ui->pushButton_pd2_high->text().toDouble() ,
              0.0, 3000, 1, &ok) ;
    if(ok)
    {
        ui->pushButton_pd2_high->setText(QString("%1").arg(value));
    }
}
//-------------------------------------------------------------------
void T4good_kratta_dlg::on_buttonBox_accepted()
{
    // saving into the file
    // save to the file
    // reading from the file
    string pname = gpath.options + "good_kratta.txt" ;
    ofstream plik(pname.c_str());
    if(!plik)
    {


    }

    // writing
    plik
            << "// This !!!! file contains specification about good KRATTA signals\n"

               //----------
            << "\ngood_pd0_time_cal_requires_threshold\t"
            <<  ui->checkBox_pd0->isChecked()
             << "\npd0_time_cal_lower_threshold\t"
             <<  ui->pushButton_pd0_low->text().toStdString()
              << "\npd0_time_cal_upper_threshold\t"
              <<  ui->pushButton_pd0_high->text().toStdString()

               << "\ngood_pd1_time_cal_requires_threshold\t"
               <<  ui->checkBox_pd1->isChecked()
                << "\npd1_time_cal_lower_threshold\t"
                <<  ui->pushButton_pd1_low->text().toStdString()
                 << "\npd1_time_cal_upper_threshold\t"
                 <<  ui->pushButton_pd1_high->text().toStdString()

                  << "\ngood_pd2_time_cal_requires_threshold\t"
                  <<  ui->checkBox_pd2->isChecked()
                   << "\npd2_time_cal_lower_threshold\t"
                   <<  ui->pushButton_pd2_low->text().toStdString()
                    << "\npd2_time_cal_upper_threshold\t"
                    <<  ui->pushButton_pd2_high->text().toStdString()

                     << endl;


    QDialog::accept();

}
