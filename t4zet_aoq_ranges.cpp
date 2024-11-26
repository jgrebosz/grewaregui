#include "t4zet_aoq_ranges.h"
#include "ui_t4zet_aoq_ranges.h"
#include <fstream>
#include <iostream>
using namespace std;
#include "paths.h"
#include "Tfile_helper.h"
#include <QMessageBox>
#include <QDoubleValidator>

//***************************************************************************************************
T4zet_aoq_ranges::T4zet_aoq_ranges(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4zet_aoq_ranges)
{
    ui->setupUi(this);


    ui->lineEdit_zet_min->setText("0");
    ui->lineEdit_zet_max->setText("100");

    ui->lineEdit_aoq_min->setText("0");
    ui->lineEdit_aoq_max->setText("6");

    // try to open the file with this option
    ifstream plik(( gpath.options + "zet_aoq_ranges.option").c_str());
    if(!plik)
    {
        cout << "Can't open zet_aoq_ranges.option file" << endl;
        return ;
    }

    try{
        double value =  Nfile_helper::find_in_file(plik, "z_minimum") ;
        ui->lineEdit_zet_min->setValidator( new QDoubleValidator( 0, 120, 3,   ui->lineEdit_zet_min) );
        ui->lineEdit_zet_min->setText(QString::number(value, 'f', 1));

        value =  Nfile_helper::find_in_file(plik, "z_maximum") ;
        ui->lineEdit_zet_max->setValidator( new QDoubleValidator( 0, 120, 3,   ui->lineEdit_zet_max) );
        ui->lineEdit_zet_max->setText(QString::number(value, 'f', 1));


        value =  Nfile_helper::find_in_file(plik, "aoq_minimum") ;
        ui->lineEdit_aoq_min->setValidator( new QDoubleValidator( 0, 120, 3,   ui->lineEdit_aoq_min) );
        ui->lineEdit_aoq_min->setText(QString::number(value, 'f', 1));

        value =  Nfile_helper::find_in_file(plik, "aoq_maximum") ;
        ui->lineEdit_aoq_max->setValidator( new QDoubleValidator( 0, 120, 3,   ui->lineEdit_aoq_max) );
        ui->lineEdit_aoq_max->setText(QString::number(value, 'f', 1));

    }
    catch(...)
    {
        cout << "Exception while reading addback option file " << endl;
    }

}
//***************************************************************************************************
T4zet_aoq_ranges::~T4zet_aoq_ranges()
{
    delete ui;
}
//***************************************************************************************************
void T4zet_aoq_ranges::accept()
{
    ofstream plik(( gpath.options + "zet_aoq_ranges.option").c_str());

    plik
            << "z_minimum\t" << ui->lineEdit_zet_min->text().toStdString()
            << "\nz_maximum\t" << ui->lineEdit_zet_max->text().toStdString()
            << "\naoq_minimum\t" << ui->lineEdit_aoq_min->text().toStdString()
            << "\naoq_maximum\t" << ui->lineEdit_aoq_max->text().toStdString()
            << endl ;
    plik.close() ;

    QDialog::accept();
    QMessageBox::information ( this, "Note",
                               "Hint:\nYou just made the settings about the new ranges of Z and A over Q\n"
                               "The SPY will obey yor wish next time you start it\n"
                               "(as the SPY creates corresponding spectra only at the begining of his work).\n\n\n"
                               "The last contents of the spectra (which will have now different range)\n"
                               "will be erased.\n"
                               "All other spectra will not be changed" ,
                               QMessageBox::Ok);
}
//***************************************************************************************************


