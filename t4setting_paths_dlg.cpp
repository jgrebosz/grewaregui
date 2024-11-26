#include "t4setting_paths_dlg.h"
#include "ui_t4setting_paths_dlg.h"

#include <string>
using namespace std;
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#include <tplate_spectrum.h>
//*********************************************************************
T4setting_paths_dlg::T4setting_paths_dlg(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::T4setting_paths_dlg)
{
  ui->setupUi(this);
}
//*********************************************************************
T4setting_paths_dlg::~T4setting_paths_dlg()
{
  delete ui;
}
//*********************************************************************
void T4setting_paths_dlg::set_spy_path( string sp )
{
  ui->lineEdit1_spy_path->setText(sp.c_str());
}
//*********************************************************************

string T4setting_paths_dlg::get_spy_path()
{
  //    cout << "getting the spy path - as "
  //         << ui->lineEdit1_spy_path->text().toStdString()<< endl;
  return ui->lineEdit1_spy_path->text().toStdString();
}
//*********************************************************************
void T4setting_paths_dlg::browse_dir()
{


  Tplate_spectrum::flag_repainting_allowed = false;
  QString s = QFileDialog::getExistingDirectory(this,
                                                "Where is your spy working directory?",
                                                ui->lineEdit1_spy_path->text()
                                                );

  Tplate_spectrum::flag_repainting_allowed = true;
  if(s != QString())
    {
      ui->lineEdit1_spy_path->setText(s);
    }

  // warning if the spectra subdirectory does not exist

  QDir dir;

  if(dir.exists(s+"/spectra") == false)
    {
      QMessageBox::warning
          (this, "Perhaps your mistatke?",
           "This directory is probably not a legal spy directory \n"
           " (as it does not have a subdirectory 'spectra' )",
           QMessageBox::Ok,
           QMessageBox::NoButton,
           QMessageBox::NoButton
           );

    }


}
//************************************************************************************************
void T4setting_paths_dlg::on_pushButton_browse_spec_clicked()
{
  browse_dir();
}
//************************************************************************************************
void T4setting_paths_dlg::on_buttonOk_clicked()
{
  /*cout << "In the momemnt of OK getting the spy path - is: "
         << ui->lineEdit1_spy_path->text().toStdString()<< endl;*/
}
