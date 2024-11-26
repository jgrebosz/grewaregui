#include "t4good_hector_dlg.h"
#include "ui_t4good_hector_dlg.h"
#include <qinputdialog.h>
#include <iostream>
#include <string>
#include "paths.h"
#include "Tfile_helper.h"


//***************************************************************************************
T4good_hector_dlg::T4good_hector_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4good_hector_dlg)
{
    ui->setupUi(this);
    init();
    ui->tabWidget->setCurrentIndex(1);
}
//***************************************************************************************
T4good_hector_dlg::~T4good_hector_dlg()
{
    delete ui;
}
//***************************************************************************************
void T4good_hector_dlg::init()
{

    on_push_default_clicked();

    // reading from the file
    string pname = gpath.options + "good_hector.txt" ;
    ifstream plik(pname.c_str());
    if(!plik)
    {
        cout << "Cant open file " << pname << endl;
        return ;

    }

    try{

        // reading
        bool flag ;

        //----------
        flag = (bool) Nfile_helper::find_in_file(plik, "good_fast_requires_threshold") ;
        ui->checkBox_good_enFAST->setChecked(flag);

        double ddd ;
        ddd =  Nfile_helper::find_in_file(plik, "fast_threshold") ;
        ui->pushline_enFAST_good_threshold->setText(QString::number(ddd));
        ui->pushline_enFAST_good_threshold->setEnabled(flag);

        //-------
        flag = (bool) Nfile_helper::find_in_file(plik, "good_slow_requires_threshold") ;
        ui->checkBox_good_enSLOW->setChecked(flag);

        ddd =  Nfile_helper::find_in_file(plik, "slow_threshold") ;
        ui->pushline_enSLOW_good_threshold->setText(QString::number(ddd));
        ui->pushline_enSLOW_good_threshold->setEnabled(flag);

        // time
        flag = (bool) Nfile_helper::find_in_file(plik, "good_time_requires_threshold") ;
        ui->checkBox_good_time->setChecked(flag);


        ddd =  Nfile_helper::find_in_file(plik, "good_time_threshold") ;
        ui->push_time_threshold->setText(QString::number(ddd));
        ui->push_time_threshold->setEnabled(flag);



        // NOT_LED
        ui->checkBox_not_led->setChecked(true);
        flag = (bool) Nfile_helper::find_in_file(plik, "good_requires_not_led") ;
        ui->checkBox_not_led->setChecked(flag);


        try
        {
            //   cout << __FILE__ << __LINE__ << endl;
            led_adc_nr = (int) Nfile_helper::find_in_file(plik, "led_adc_nr") ;
            ui->push_LED_Adc->setText(QString("ADC ") + QString::number(led_adc_nr));
            //   cout << __FILE__ << __LINE__ << endl;

            led_adc_low = (int) Nfile_helper::find_in_file(plik, "led_adc_low") ;
            ui->push_adc_low->setText(QString::number(led_adc_low));
            //   cout << __FILE__ << __LINE__ << endl;

            led_adc_high = (int) Nfile_helper::find_in_file(plik, "led_adc_high") ;
            ui->push_adc_high->setText(QString::number(led_adc_high));
            //   cout << __FILE__ << __LINE__ << endl;

            //--------
            led_tdc_nr = (int) Nfile_helper::find_in_file(plik, "led_tdc_nr") ;
            ui->push_LED_Tdc->setText(QString("TDC ") + QString::number(led_tdc_nr));
            //   cout << __FILE__ << __LINE__ << endl;

            led_tdc_high = (int) Nfile_helper::find_in_file(plik, "led_tdc_high") ;
            ui->push_tdc_high->setText(QString::number(led_tdc_high));
            //   cout << __FILE__ << __LINE__ << endl;

            led_tdc_low = (int) Nfile_helper::find_in_file(plik, "led_tdc_low") ;
            ui->push_tdc_low->setText(QString::number(led_tdc_low));
            //   cout << __FILE__ << __LINE__ << endl;

        }
        catch(Tno_keyword_exception m)
        {
            cout << "Error while reading " << pname << "\n" << m.message << endl;
        }
        catch(Treading_value_exception m)
        {
            cout << "Error while reading " << pname << "\n" << m.message << endl;
        }
        catch(...)
        {
            cout << "good_hec: Seems to be old spy, not defined LED ranges " << endl;
            // because the older versions of the program
        }

    }
    catch(Tfile_helper_exception k)
    {
        cout << "Error while reading " << pname << "\n" << k.message << endl;


    }

}
//*********************************************************************
void T4good_hector_dlg::accept()
{
    // save to the file
    // reading from the file
    string pname = gpath.options + "good_hector.txt" ;
    ofstream plik(pname.c_str());
    if(!plik)
    {


    }


    // writing
    plik
            << "// This !!!! file contains specification about good hector BaF signals\n"

               //----------
            << "\n\ngood_fast_requires_threshold\t"
            <<  ui->checkBox_good_enFAST->isChecked()
             << "\nfast_threshold\t"
             <<  ui->pushline_enFAST_good_threshold->text().toStdString()

                 //-------
              << "\n\ngood_slow_requires_threshold\t"
              <<  ui->checkBox_good_enSLOW->isChecked()

               << "\nslow_threshold\t"
               << ui->pushline_enSLOW_good_threshold->text().toStdString()

                  // time
               << "\n\ngood_time_requires_threshold\t"
               <<  ui->checkBox_good_time->isChecked()
                << "\ngood_time_threshold\t"
                <<  ui->push_time_threshold->text().toStdString()

                    // NOT_LED

                 << "\n\ngood_requires_not_led\t"
                 <<  ui->checkBox_not_led->isChecked()

                     // new LED boundaries
                  << "\n\nled_adc_nr\t " <<  led_adc_nr
                  <<"\nled_tdc_nr \t" << led_tdc_nr
                 << "\nled_adc_low\t" << led_adc_low
                 << "\nled_adc_high\t" << led_adc_high
                 << "\nled_tdc_low \t" << led_tdc_low
                 << "\nled_tdc_high\t" <<  led_tdc_high
                 << endl;


    QDialog::accept();
}
//***************************************************************************************
void T4good_hector_dlg::on_push_default_clicked()
{
    led_adc_nr  = 10 ;
    led_tdc_nr = 23 ;

    led_adc_low = 500 ;
    led_adc_high = 4000 ;
    led_tdc_low  = 500 ;
    led_tdc_high = 4000 ;

    ui->push_LED_Adc->setText(QString("ADC ") + QString::number(led_adc_nr));
    ui->push_adc_low->setText(QString::number(led_adc_low));
    ui->push_adc_high->setText(QString::number(led_adc_high));

    ui->push_LED_Tdc->setText(QString("TDC ") + QString::number(led_tdc_nr));
    ui->push_tdc_low->setText(QString::number(led_tdc_low));
    ui->push_tdc_high->setText(QString::number(led_tdc_high));

    // FAST

    ui->checkBox_good_enFAST->setChecked(false);
    ui->pushline_enFAST_good_threshold->setText("0");
    ui->pushline_enFAST_good_threshold->setEnabled(false);

    // SLOW
    ui->checkBox_good_enSLOW->setChecked(true);
    ui->pushline_enSLOW_good_threshold->setText("10");
    ui->pushline_enSLOW_good_threshold->setEnabled(true);

    // TIME
    ui->checkBox_good_time->setChecked(true);
    ui->push_time_threshold->setText("10");
    ui->push_time_threshold->setEnabled(true);

    // NOT_LED
    ui->checkBox_not_led->setChecked(true);

}
//**********************************************************************************************
void T4good_hector_dlg::on_pushline_enFAST_good_threshold_clicked()
{
    bool ok = false ;
    double value  = QInputDialog::getDouble
            ( this, "Type the value of threshold for signal 'energy FAST'",
              "threshold ",
              ui->pushline_enFAST_good_threshold->text().toDouble(),
              0.0, 20000, 1, &ok) ;
    if(ok)
    {
        ui->pushline_enFAST_good_threshold->setText(QString("%1").arg(value));
    }
}
//*********************************************************************************************
void T4good_hector_dlg::on_pushline_enSLOW_good_threshold_clicked()
{
    bool ok = false ;
    double value  = QInputDialog::getDouble
            ( this, "Type the value of threshold for signal 'energy SLOW'",
              "threshold ",
              ui->pushline_enSLOW_good_threshold->text().toDouble(),
              0.0, 4096, 1, &ok) ;
    if(ok)
    {
        ui->pushline_enSLOW_good_threshold->setText(QString("%1").arg(value));
    }
}
//*****************************************************************
void T4good_hector_dlg::on_push_time_threshold_clicked()
{
    bool ok = false ;
    double value  = QInputDialog::getDouble
            (this, "Type the value of threshold for signal 'TIME'",
             "threshold ",
             ui->push_time_threshold->text().toDouble() ,
             0.0, 4096, 1, &ok) ;
    if(ok)
    {
        ui->push_time_threshold->setText(QString("%1").arg(value));
    }
}
//********************************************************************
void T4good_hector_dlg::on_push_LED_Adc_clicked()
{
    bool ok;
    int res = QInputDialog::getInt(this,
                                       "LED signal", "Enter a number of the ADC which deliveres LED signal:",
                                       led_adc_nr , 0, 31, 1,
                                       &ok);
    if ( ok ) {
        // user entered something and pressed OK
        led_adc_nr = res;
        ui->push_LED_Adc->setText(QString("ADC ") + QString::number(led_adc_nr));
    } else {
        // user pressed Cancel
    }

}
//********************************************************************
void T4good_hector_dlg::on_push_LED_Tdc_clicked()
{
    bool ok;
    int res = QInputDialog::getInt(
                this, "LED signal", "Enter a number of the TDC which deliveres LED signal:",
                led_tdc_nr , 0, 31, 1,
                &ok);
    if ( ok ) {
        // user entered something and pressed OK
        led_tdc_nr = res;
        ui->push_LED_Tdc->setText(QString("TDC ") + QString::number(led_tdc_nr));
    } else {
        // user pressed Cancel
    }
}
//*********************************************************************
void T4good_hector_dlg::on_push_adc_low_clicked()
{
    bool ok;
    int res = QInputDialog::getInt(
                this, "LED signal", "Enter a MINIMAL value for LED signal: ON:", led_adc_low , 0, 4095, 1,
                &ok);
    if ( ok ) {
        // user entered something and pressed OK
        led_adc_low = res;
        ui->push_adc_low->setText(QString::number(led_adc_low));
    } else {
        // user pressed Cancel
    }
}
//********************************************************************
void T4good_hector_dlg::on_push_adc_high_clicked()
{
    bool ok;
    int res = QInputDialog::getInt(this,
                                       "LED signal", "Enter a MAXIMAL value for LED signal: ON:", led_adc_high , 0, 4095, 1,
                                       &ok);
    if ( ok ) {
        // user entered something and pressed OK
        led_adc_high = res;
        ui->push_adc_high->setText(QString::number(led_adc_high));
    } else {
        // user pressed Cancel
    }
}
//********************************************************************
void T4good_hector_dlg::on_push_tdc_low_clicked()
{
    bool ok;
    int res = QInputDialog::getInt(this,
                                       "LED signal", "Enter a MINIMAL value for LED signal: ON:", led_tdc_low , 0, 4095, 1,
                                       &ok);
    if ( ok ) {
        // user entered something and pressed OK
        led_tdc_low = res;
        ui->push_tdc_low->setText(QString::number(led_tdc_low));
    } else {
        // user pressed Cancel
    }
}
//********************************************************************
void T4good_hector_dlg::on_push_tdc_high_clicked()
{
    bool ok;
    int res = QInputDialog::getInt(this,
                                       "LED signal", "Enter a MAXIMAL value for LED signal: ON:", led_tdc_high , 0, 4095, 1,
                                       &ok);
    if ( ok ) {
        // user entered something and pressed OK
        led_tdc_high = res;
        ui->push_tdc_high->setText(QString::number(led_tdc_high));
    } else {
        // user pressed Cancel
    }
}


//**********************************************************************************************
void T4good_hector_dlg::on_checkBox_good_enFAST_clicked(bool checked)
{
    ui->pushline_enFAST_good_threshold->setEnabled(checked);
}
//**********************************************************************************************
void T4good_hector_dlg::on_checkBox_good_enSLOW_clicked(bool checked)
{
    ui->pushline_enSLOW_good_threshold->setEnabled(checked);
}
//**********************************************************************************************
void T4good_hector_dlg::on_checkBox_good_time_clicked(bool checked)
{
    ui->push_time_threshold->setEnabled(checked);
}
//**********************************************************************************************
