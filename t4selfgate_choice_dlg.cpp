#include "t4selfgate_choice_dlg.h"
#include "ui_t4selfgate_choice_dlg.h"

#include <QDir>
#include <string>
using namespace std;
#include "paths.h"

//************************************************************************************
T4selfgate_choice_dlg::T4selfgate_choice_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4selfgate_choice_dlg)
{
    ui->setupUi(this);
}
//************************************************************************************
T4selfgate_choice_dlg::~T4selfgate_choice_dlg()
{
    delete ui;
}
//***********************************************************************************
void T4selfgate_choice_dlg::set_type_of_selfgate(Tselfgate_type t)
{
// filling the list with the selfgate names

    // opening all spectra files from current directory
    QDir katalog(gpath.conditions.c_str()) ;

    string f = t.give_type_filter() ;
    QStringList filters;

    filters <<  f.c_str() ;
    katalog.setNameFilters(filters);

    QStringList lista = katalog.entryList (  ) ;

    ui->listWidget_of_selfgates->addItem("No_self_gate");
    ui->listWidget_of_selfgates->addItems(lista);
    ui->label_filter->setText(f.c_str());
}
//************************************************************************************
void T4selfgate_choice_dlg::on_buttonBox_accepted()
{
    selected_selfgate_name = ui->listWidget_of_selfgates->currentItem()->text().toStdString();
}
//************************************************************************************
