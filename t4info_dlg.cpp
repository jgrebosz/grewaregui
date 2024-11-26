#include "t4info_dlg.h"
#include "ui_t4info_dlg.h"

//**************************************************************************
T4info_dlg::T4info_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4info_dlg)
{
    ui->setupUi(this);
}
//**************************************************************************
T4info_dlg::~T4info_dlg()
{
    delete ui;
}
//**************************************************************************
void T4info_dlg::set_parameter( QString message)
{
  ui->textEdit->setText(message);

}
//**************************************************************************
void T4info_dlg::set_label(QString etykieta)
{
 ui->textLabel_mess->setText (etykieta);
}
