#include "t4verbose_dlg.h"
#include "ui_t4verbose_dlg.h"
using namespace std;
//#include <qvalidator.h>
#include "paths.h"
#include "Tfile_helper.h"
#include <iostream>
#include <QMessageBox>
#include <QDoubleValidator>

//**********************************************************************************************
T4verbose_dlg::T4verbose_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4verbose_dlg)
{
    ui->setupUi(this);
    init();
}
//**********************************************************************************************
T4verbose_dlg::~T4verbose_dlg()
{
    delete ui;
}
//**********************************************************************************************
void T4verbose_dlg::accept()
{
    //cout << "The dialog was accepted" << endl;
    ofstream plik(( gpath.commands + "verbose_mode.command").c_str());

    plik
            << "verbose_mode_enabled\t"
            << (ui->checkBox_enable_verbose->isChecked() ? 1 : 0)
            << "\nverbose_begins_with_event_nr\t"
            << ui->lineEdit_start_event->text().toStdString()
            << "\nverbose_how_many_events\t"
            << ui->lineEdit2_how_many_events->text().toStdString()
            << endl ;

    plik.close() ;

    QDialog::accept() ;   // obligatory
}
//**********************************************************************************************
void T4verbose_dlg::show_help()
{

    QMessageBox::information( this, "Verbose mode help",
                              "In this dialog window where you can \n"
                              "\ta) Enable/Disable this system \n"

                              "\tb) choose when to start showing events \n"
                              "\tc) choose how many events will be shown \n\n"

                              "For example you can ask to show event \n"
                              "starting from the event nr 6000000 and you would like to see 72 events. \n\n"

                              "if you work offline analysing many files in one run of the spy - \n"
                              "the number of the \"starting event\" is related to the first event of the first file.\n\n\n"

                          #if 0
                              "Remember: \n"
                              "This what you read from the tape/file, there are   \n"
                              "not events but subevents. \n"
                              "Inside the program there is something like \"Event builder\". \n"
                              "(which makes the matching GER, FRS, HEC, DGF subevents to create  the event).\n"
                              "So in the dialog window you are asked for number of 'events', not for nr of 'subevents'. \n\n"

                              "On the other hand - if you work in the mode: NO MATCHING \n"
                              "the event is the same as the subevent (because all subevents are treated \n"
                              "independently as the separate events) \n\n"

                              "One more remark: "
                          #endif
                              "If you work in the mode \"Create N-tuple\" \n"
                              "the \"verbose mode\" also will say if the particular event is \n"
                              "good enough to be included in the ntuple. If \"not\" - it says why not.\n"
                              "(for example because the mw41_x was not OK)."
                              );

}
//**********************************************************************************************
void T4verbose_dlg::init()
{
    // cout << "init the dialog " << endl;
    ui->checkBox_enable_verbose->setChecked(0);
    ui->lineEdit_start_event->setText("0");
    ui->lineEdit2_how_many_events->setText("0");

    string nazwa = gpath.commands + "verbose_mode.command" ;
    ifstream plik(nazwa.c_str() );
    if(plik)
    {
        try
        {
            int value = (int) Nfile_helper::find_in_file(plik,
                                                         "verbose_begins_with_event_nr" );

            ui->lineEdit_start_event->setValidator( new QIntValidator( ui->lineEdit_start_event ));
            ui->lineEdit_start_event->setText(QString::number(value));

            value =  (int) Nfile_helper::find_in_file(plik,
                                                      "verbose_how_many_events" );
            ui->lineEdit2_how_many_events->setValidator( new QIntValidator( ui->lineEdit_start_event ));
            ui->lineEdit2_how_many_events->setText(QString::number(value));

            value =  (int) Nfile_helper::find_in_file(plik, "verbose_mode_enabled");
            ui->checkBox_enable_verbose->setChecked( (value ==1) ? true : false);
        }
        catch(Tfile_helper_exception &m)
        {
            cout << "Error while trying to read the file " << nazwa
                 << "\n Reason is: " << m.message << endl;
            // continue, bec this is not so important
            exit(20);
        }
    }
    // depending of the state of enable
    ui->groupBox_verbose_parameters->
            setEnabled( ui->checkBox_enable_verbose->isChecked() );

}
//**********************************************************************************************
void T4verbose_dlg::on_checkBox_enable_verbose_clicked(bool checked)
{
    ui->groupBox_verbose_parameters-> setEnabled( checked);
}
//**********************************************************************************************
void T4verbose_dlg::on_buttonHelp_clicked()
{
    show_help();
}
//**********************************************************************************************
void T4verbose_dlg::on_buttonCancel_clicked()
{
    QDialog::reject();
}
