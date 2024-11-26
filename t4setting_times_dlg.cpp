#include "t4setting_times_dlg.h"
#include "ui_t4setting_times_dlg.h"

T4setting_times_dlg::T4setting_times_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4setting_times_dlg)
{
    ui->setupUi(this);
}

T4setting_times_dlg::~T4setting_times_dlg()
{
    delete ui;
}
//**************************************************************************************
//**************************************************************************************
void T4setting_times_dlg::give_times( int * time_1D, int *time_2D, int * time_sum, bool * flag )
{
    *time_1D = ui->spinBox_update_1D->value();
    *time_2D = ui->spinBox_update_2D->value();
    *time_sum = ui->spinBox_update_sum->value();
    *flag = ui->checkBox_ask_about_big_matrices->isChecked();
}
//**************************************************************************************
void T4setting_times_dlg::set_times( int time_1D, int time_2D, int time_sum, bool flag )
{
    ui->spinBox_update_1D->setValue(time_1D);
    ui->spinBox_update_2D->setValue(time_2D);
    ui->spinBox_update_sum->setValue(time_sum);
    ui->checkBox_ask_about_big_matrices->setChecked(flag);
}
//**************************************************************************************

void T4setting_times_dlg::on_push_default_values_clicked()
{
    ui->spinBox_update_1D->setValue(20);
    ui->spinBox_update_2D->setValue(30);
    ui->spinBox_update_sum->setValue(60);
    ui->checkBox_ask_about_big_matrices->setChecked(true);
}
