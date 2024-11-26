#include "t4selfgate_ger.h"
#include "ui_t4selfgate_ger.h"

T4selfgate_ger::T4selfgate_ger(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4selfgate_ger)
{
    ui->setupUi(this);
}

T4selfgate_ger::~T4selfgate_ger()
{
    delete ui;
}
