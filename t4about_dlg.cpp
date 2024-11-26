#include "t4about_dlg.h"
#include "ui_t4about_dlg.h"

#include <string>
using namespace std ;
extern bool allow_secret_part ;

//**************************************************************************
T4about_dlg::T4about_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4about_dlg)
{
    ui->setupUi(this);

    string str = string("version ") + __DATE__ + "  " + __TIME__ ;
    ui->lineEdit_version->setText(str.c_str());

//    ui->pushButton_allow->setStyleSheet("border: 2px solid #850000 background-color: rgb(85, 0, 0)");

    ui->pushButton_allow->setStyleSheet("border: 2px solid #850000; \n"
                                        "background-color:rgb(85, 0, 0);" );

}
//**************************************************************************
T4about_dlg::~T4about_dlg()
{
    delete ui;
}
//**************************************************************************
void T4about_dlg::on_pushButton_allow_clicked()
{
     allow_secret_part = true ;
//     cout << "allowed" << endl;
     ui->pushButton_allow->setText("Allowed");
}
//**************************************************************************
