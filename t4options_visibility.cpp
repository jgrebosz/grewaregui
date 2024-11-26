#include "t4options_visibility.h"
#include "ui_t4options_visibility.h"

#include "paths.h"

#include <iostream>
#include <fstream>

#include <QAction>
using namespace  std;


//***********************************************************************************************
T4options_visibility::T4options_visibility(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4options_visibility)
{
    ui->setupUi(this);

    QStringList labels;
    labels << "Visible? " << "Name of the dialog window" ;
    ui->table->setHorizontalHeaderLabels ( labels );
    ui->table->resizeColumnsToContents ();

    ui->groupBox->hide();
    ui->tabWidget->setCurrentIndex(0);
}
//***********************************************************************************************
T4options_visibility::~T4options_visibility()
{
    delete ui;
}
//***********************************************************************************************
// if the number of experiment were read from the disk, it would go here, as a paramenter

void T4options_visibility::set_parameters(std::vector<QAction *> vec, int /*nr_experiment*/)
{
    //        std::cout << "Bedzie rzedow" << vec.size() << endl;
    vec_of_actions = vec;
    ui->table->setRowCount(vec.size());

    for (unsigned int row = 0; row < vec.size(); ++row )
    {
        auto stan = vec[row]->isVisible();
        auto name = vec[row]->text() ;

        ui->table->setItem( row, 0, new QTableWidgetItem(Qt::ItemIsUserCheckable) );  // safe !
        ui->table->item( row, 0)->setCheckState( stan ? Qt::Checked : Qt::Unchecked);
        //       ui->table->item( row, 0)->setCheckState(Qt::Checked);

        ui->table->setItem ( row, 1, new QTableWidgetItem (name));
        //        ui->table->setItem ( row, 1, new QTableWidgetItem ("rzad" , Qt::ItemIsEnabled ) );
        ui->table->item( row, 1)->setFlags(stan ? (Qt::ItemIsEnabled | Qt::ItemIsSelectable): (Qt::NoItemFlags| Qt::ItemIsSelectable) ) ;   // name
    }
}
//***********************************************************************************************
void T4options_visibility::on_buttonBox_accepted()
{
    // saving on the disk the selected, and giving the application - results
    //    cout << "Accepted " << endl;
    // zapis na dysku
    // read from file
    string pat_name = gpath.options + "visible.options" ;

    ofstream plik ( pat_name.c_str() );
    if ( !plik )
    {
        cout << "Can't open file: " << pat_name  << endl;
        return ;
    }

    for (int row = 0; row < ui->table->rowCount(); ++row )
    {
        bool stan = ui->table->item( row, 0)->checkState();

        //        if(!stan)
//        cout << "option  " << vec_of_actions[row]->text().toStdString()
//             << " is " << stan
//             << endl;
        vec_of_actions[row]->setVisible(stan);
        plik
                //                << vec_of_actions[row]->text().toStdString()
                << vec_of_actions[row]->objectName().toStdString()
                << "\n"
                << stan
                << endl;
    }
}
//***********************************************************************************************
void T4options_visibility::refresh_table()
{
//    cout << __func__ << endl;
    for (int row = 0; row < ui->table->rowCount(); ++row )
    {
        bool stan = ui->table->item( row, 0)->checkState();
//        cout << "    stan = " << stan << endl;
        ui->table->item( row, 1)->setFlags(stan ?
                                               (Qt::ItemIsEnabled | Qt::ItemIsSelectable):
                                               (Qt::NoItemFlags| Qt::ItemIsSelectable) ) ;
    }

}
//***********************************************************************************************
void T4options_visibility::on_table_cellClicked(int /*row*/, int column)
{
    if(column == 0) refresh_table();
}
//***********************************************************************************************
void T4options_visibility::on_radioButton_exotic_clicked(bool /*checked*/)
{
//    cout << "somebody clicked on "<< __func__  << endl;
}
