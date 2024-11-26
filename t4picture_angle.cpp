#include "t4picture_angle.h"
#include "ui_t4picture_angle.h"

T4picture_angle::T4picture_angle(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4picture_angle)
{
    ui->setupUi(this);
}

T4picture_angle::~T4picture_angle()
{
    delete ui;
}

void T4picture_angle::on_buttonOk_clicked()
{
    accept();
}
