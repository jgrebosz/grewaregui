#include "t4update_info_dlg.h"
#include "ui_t4update_info_dlg.h"

T4update_info_dlg::T4update_info_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4update_info_dlg)
{
    ui->setupUi(this);
}

T4update_info_dlg::~T4update_info_dlg()
{
    delete ui;
}

void T4update_info_dlg::on_push_update_clicked()
{
    accept();
}

void T4update_info_dlg::on_push_later_clicked()
{
    close();
}
