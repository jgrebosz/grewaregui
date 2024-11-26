#include "t4sum_spectra_dialog.h"
#include "ui_t4sum_spectra_dialog.h"

#include <QInputDialog>
#include <QMessageBox>

//**********************************************************************************
T4sum_spectra_dialog::T4sum_spectra_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4sum_spectra_dialog)
{
    ui->setupUi(this);
}
//**********************************************************************************
T4sum_spectra_dialog::~T4sum_spectra_dialog()
{
    delete ui;
}
//**********************************************************************************
void T4sum_spectra_dialog::set_parameters( QStringList *ptr_names, QStringList *ptr_factors )
{
    names = ptr_names ;
    factors = ptr_factors;
    int nr = 0 ;
    ui->tableWidget1->setRowCount(0);
    QStringList::Iterator itfac =  factors->begin();
    for ( QStringList::Iterator it = names->begin() ; it != names->end() ;  ++it, nr++, ++itfac )
    {
        //cout << *it << "\n";
        if(nr+1 > ui->tableWidget1->rowCount())
        {
            ui->tableWidget1->insertRow(nr);
        }
        ui->tableWidget1->setItem(nr, 0, new QTableWidgetItem(*it));
        ui->tableWidget1->item(nr, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        ui->tableWidget1->setItem(nr, 1, new QTableWidgetItem(*itfac) );
        ui->tableWidget1->item(nr, 1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }

    ui->tableWidget1->resizeColumnsToContents();
}
//**********************************************************************************
void T4sum_spectra_dialog::on_pushButton_remove_row_clicked()
{

    // which row is selected
    int nr = ui->tableWidget1->currentRow();

    // are you sure?

    if(QMessageBox::warning(this,
                            "Removing the spectrum from the table",      // tile
                            QString(                                                            // message
                                                                                                "You are going to remove the row nr %1\n which is: %2  factor %3\n"
                                                                                                "Are you sure?").arg(nr+1).
                            arg(ui->tableWidget1->item(nr, 0)->text() ).
                            arg(ui->tableWidget1->item(nr, 1)->text() )
                            ,
                            QMessageBox::Yes,
                            QMessageBox::No,
                            QMessageBox::Cancel)
            == QMessageBox::Yes)
    {

        // remove it

        ui->tableWidget1->removeRow(nr);
    }

}
//**********************************************************************************

#if 0
void T4sum_spectra_dialog::on_table1_doubleClicked(int row, int col, int button, const QPoint &   /*mousePos*/ )
{
    // which row is selected
    int nr = row ; // ui->tableWidget1->currentRow();
    col = col ; // fake
    button = button ; // fake


    // Take the text from the table


    double old_value = ui->tableWidget1->item(nr, 1)->text().toDouble() ;

    // display the dialog
    bool ok;
    double res = QInputDialog::getDouble(this , "Adding factor",
                                         QString("Enter a new value of the factor for spectrum %1").arg(ui->tableWidget1->item(nr, 0)->text() ),
                                         old_value, -10000.5,
                                         10000.5, 3, &ok);
    if ( ok ) {
        // user entered something and pressed OK
        // store this back into the table
        QString new_text;
        new_text.setNum(res);
        ui->tableWidget1->item(nr, 1)->setText(new_text);
    } else {
        // user pressed Cancel
    }
}
#endif // 0
//*********************************************************************************
void T4sum_spectra_dialog::on_tableWidget1_cellDoubleClicked(int row, int column)
{
    // which row is selected
    //    int row = rowx ; // ui->tableWidget1->currentRow();
    //    col = col ; // fake
    //button = button ; // fake


    // Take the text from the table
    if(column == 0) return;

    double old_value =  ui->tableWidget1->item(row, 1)->text().toDouble() ;

    // display the dialog
    bool ok = true;
    double res = QInputDialog::getDouble(this , "Adding factor",
                                         QString("Enter a new value of the factor for spectrum %1").arg(ui->tableWidget1->item(row, 0)->text() ),
                                         old_value, -10000.5,
                                         10000.5  ,
                                         8, &ok);
    if ( ok ) {
        // user entered something and pressed OK
        // store this back into the table
        QString new_text = QString("%1").arg(res);
        ui->tableWidget1->item(row, 1)->setText(new_text);
        (*factors)[row] = new_text ;
    } else {
        // user pressed Cancel
    }
}
