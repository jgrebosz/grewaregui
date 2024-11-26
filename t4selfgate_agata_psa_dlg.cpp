#include "t4selfgate_agata_psa_dlg.h"
#include "ui_t4selfgate_agata_psa_dlg.h"

#include <QMessageBox>

//************************************************************************************
T4selfgate_agata_psa_dlg::T4selfgate_agata_psa_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4selfgate_agata_psa_dlg)
{
    ui->setupUi(this);

    //this_is_germanium = false ; //true ; // for false - means this is Hector selfgate
    update_checked_unchecked();

    //  ui->checkBox_mult->hide() ; //setVisible(false);
    //  ui->lineEdit_mult_low->hide() ; //setVisible(false);
    //  ui->lineEdit_mult_high->hide() ;  //setVisible(false);

}
//************************************************************************************
T4selfgate_agata_psa_dlg::~T4selfgate_agata_psa_dlg()
{
    delete ui;
}
//************************************************************************************
void T4selfgate_agata_psa_dlg::on_checkBox_energy_clicked()
{
    update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_agata_psa_dlg::on_checkBox_time_clicked()
{
    update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_agata_psa_dlg::on_checkBox_x_clicked()
{
    update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_agata_psa_dlg::on_checkBox_y_clicked()
{
    update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_agata_psa_dlg::on_checkBox_z_clicked()
{
    update_checked_unchecked();
}
//************************************************************************************
void T4selfgate_agata_psa_dlg::on_checkBox_mult_clicked()
{
    update_checked_unchecked();
}
//****************************************************************
void T4selfgate_agata_psa_dlg::update_checked_unchecked()
{

    ui->lineEdit_energy_low->setEnabled(ui->checkBox_energy->isChecked());
    ui->lineEdit_energy_high->setEnabled(ui->checkBox_energy->isChecked());

    // original time
    ui->lineEdit_time_low->setEnabled(ui->checkBox_time->isChecked());
    ui->lineEdit_time_high->setEnabled(ui->checkBox_time->isChecked());

    //-------------
    ui->lineEdit_x_low->setEnabled(ui->checkBox_x->isChecked());
    ui->lineEdit_x_high->setEnabled(ui->checkBox_x->isChecked());

    ui->lineEdit_y_low->setEnabled(ui->checkBox_y->isChecked());
    ui->lineEdit_y_high->setEnabled(ui->checkBox_y->isChecked());

    ui->lineEdit_z_low->setEnabled(ui->checkBox_z->isChecked());
    ui->lineEdit_z_high->setEnabled(ui->checkBox_z->isChecked());

    // this is mutliplicity inside one cluster --------------------------
    ui->lineEdit_mult_low->setEnabled(ui->checkBox_mult->isChecked());
    ui->lineEdit_mult_high->setEnabled(ui->checkBox_mult->isChecked());

}
//********************************************************************
void T4selfgate_agata_psa_dlg::set_parameters( const Tself_gate_agata_psa_descr *d )
{

    string nnn = d->name;
    string ext = (d->sg_type).give_type_extension() ;
    string::size_type i = nnn.find(ext);
    if(i != string::npos)
    {
        nnn.erase(i) ;    // do not show extension in the edit field
    }

    ui->lineEdit_name->setText(nnn.c_str());
    ui->checkBox_energy->setChecked(d->enable_energy_gate) ;
    ui->lineEdit_energy_low->setText(QString::number(d-> energy_gate[0]));
    ui->lineEdit_energy_high->setText(QString::number(d-> energy_gate[1]));

    ui->checkBox_time->setChecked(d-> enable_time_gate) ;
    ui->lineEdit_time_low->setText(QString::number(d-> time_gate[0]));
    ui->lineEdit_time_high->setText(QString::number(d-> time_gate[1]));


    ui->checkBox_x->setChecked(d->enable_x_gate) ;
    ui->lineEdit_x_low->setText(QString::number(d-> x_gate[0]));
    ui->lineEdit_x_high->setText(QString::number(d->x_gate[1]));

    ui->checkBox_y->setChecked(d->enable_y_gate) ;
    ui->lineEdit_y_low->setText(QString::number(d-> y_gate[0]));
    ui->lineEdit_y_high->setText(QString::number(d->y_gate[1]));

    ui->checkBox_z->setChecked(d->enable_z_gate) ;
    ui->lineEdit_z_low->setText(QString::number(d-> z_gate[0]));
    ui->lineEdit_z_high->setText(QString::number(d->z_gate[1]));


    // this is the multiplicity inside the cluster where this crystal belongs
    ui->checkBox_mult->setChecked(d-> enable_mult_of_hits_gate);
    ui->lineEdit_mult_low->setText(QString::number(d->mult_of_hits_gate[0]));
    ui->lineEdit_mult_high->setText(QString::number(d->mult_of_hits_gate[1]));

}
//*******************************************************************
void T4selfgate_agata_psa_dlg::get_parameters( Tself_gate_agata_psa_descr * d )
{
    string s = d->name = ui->lineEdit_name->text().toStdString();
    // if there is no extension, add it
    string ext = (d->sg_type).give_type_extension() ;
    string::size_type i = s.find(ext);
    if(i == string::npos)
    {
        d->name += ext;
    }

    d->enable_energy_gate = ui->checkBox_energy->isChecked() ;
    d-> energy_gate[0] = ui->lineEdit_energy_low->text().toDouble();
    d-> energy_gate[1] = ui->lineEdit_energy_high->text().toDouble();


    d-> enable_time_gate = ui->checkBox_time->isChecked() ;
    d-> time_gate[0] = ui->lineEdit_time_low->text().toDouble();
    d-> time_gate[1] = ui->lineEdit_time_high->text().toDouble();

    //----------
    d->enable_x_gate = ui->checkBox_x->isChecked() ;
    d-> x_gate[0] = ui->lineEdit_x_low->text().toDouble();
    d-> x_gate[1] = ui->lineEdit_x_high->text().toDouble();
    //----------
    d->enable_y_gate = ui->checkBox_y->isChecked() ;
    d-> y_gate[0] = ui->lineEdit_y_low->text().toDouble();
    d-> y_gate[1] = ui->lineEdit_y_high->text().toDouble();
    //----------
    d->enable_z_gate = ui->checkBox_z->isChecked() ;
    d-> z_gate[0] = ui->lineEdit_z_low->text().toDouble();
    d-> z_gate[1] = ui->lineEdit_z_high->text().toDouble();

    // this is the multiplicity inside the cluster where this crystal belongs
    d-> enable_mult_of_hits_gate = ui->checkBox_mult->isChecked();
    d->mult_of_hits_gate[0] = ui->lineEdit_mult_low->text().toDouble();
    d->mult_of_hits_gate[1] = ui->lineEdit_mult_high->text().toDouble();

}
//*******************************************************************
void T4selfgate_agata_psa_dlg::accept()
{
    if(ui->lineEdit_name->text() == "" )
    {

        QMessageBox::critical( this, "The name is missing",
                               "Please specify the (unique) name of this 'self-gate' ",
                               QMessageBox::Ok | QMessageBox::Default,
                               QMessageBox::NoButton,
                               QMessageBox::NoButton);  // error
        return ;
    }
    QDialog::accept();

}
//************************************************************************************
