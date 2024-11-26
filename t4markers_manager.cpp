#include "t4markers_manager.h"
#include "ui_t4markers_manager.h"

#include "QInputDialog"
#include "QMessageBox"

#include <sstream>
using namespace std;
//***********************************************************************************************
t4markers_manager::t4markers_manager(QWidget *parent,
                                     std::vector<std::vector<typ_x>>  & set_of_markers__,
                                     spectrum_1D *widmo
                                     ):
    QDialog(parent),
    ui(new Ui::t4markers_manager),
    widmo_ptr(widmo),
    set_of_markers(set_of_markers__)

{
    ui->setupUi(this);
    refresh();
}
//*******************************************************************************
t4markers_manager::~t4markers_manager()
{
    delete ui;
}
//*******************************************************************************
void t4markers_manager::refresh()
{
    ui->table->clearContents();

    ui->table->setShowGrid(true);
    ui->table->setColumnCount(2);

    int nr_rows = set_of_markers.size();
    ui->table->setRowCount( nr_rows );


    QStringList labels;
    labels << "  " << "Markers positions" ;
    ui->table->setHorizontalHeaderLabels ( labels );

    labels.clear();
    //    labels << "Set Nr" ;
    //     ui->table->setVerticalHeaderLabels ( labels );


    for (int j = 0; j < nr_rows; ++j )
    {
        labels << "Set Nr" ;
        ui->table->setVerticalHeaderLabels ( labels );
        ui->table->setItem ( j, 0,
                             new QTableWidgetItem (
                                 to_string(j).c_str(),
                                 Qt::ItemIsEnabled )
                             );

        string tresc ;
        ostringstream s(tresc);
        int nr = 0 ;
        for(auto n : set_of_markers[j])
        {

            if(nr == 0 ) s << "Peak: ";
            if(nr == 1 ) s << " - ";
            if(nr == 2 ) s << "   Background: ";
            s << n << " ";
            nr++;
        }

        s << "\n";
        // tresc

        ui->table->setItem ( j, 1,
                             new QTableWidgetItem (
                                 s.str().c_str(),
                                 Qt::ItemIsEnabled )
                             );


    }
    ui->table->resizeColumnsToContents ();
    //----------------- disaple/enable buttons

    bool enab = true;
    //    int nr = ui->table->currentRow();
    //cout << "Current row = " << nr << endl;

    //    auto lista = ui->table->selectedItems() ;
    //    if(lista.size() ){
    //        cout << "Jest wybrany rzad " << endl;
    //        enab = true;
    //    }
    //    else{
    //        cout << "Pusta selekcja" << endl;
    //        enab = false;
    //    }


    ui->pushButton_paste->setEnabled(enab);
    ui->pushButton_remove->setEnabled(enab);

}
//*******************************************************************************
void t4markers_manager::on_pushButton_paste_clicked()
{
    int curr_row = ui->table->currentRow();

//    cout
//         << " curr row = " << curr_row
//         << endl;

    uint how_many = set_of_markers.size() -1;
    bool ok;
    int i = QInputDialog::getInt(this, tr("Paste set"),
                                 tr("Which set number should be pasted: "),
                                 curr_row, 0, how_many, 1, &ok);
    if (ok){
        widmo_ptr->paste_integration_and_bgr_markers(i);

    }
}
//*******************************************************************************
void t4markers_manager::on_pushButton_remove_clicked()
{
    // just try

    int curr_row = ui->table->currentRow();

//    cout << " curr row = " << curr_row << endl;

    // which nr of set
    uint how_many = set_of_markers.size() -1;

    bool ok;
    int i = QInputDialog::getInt(this, tr("Remove set"),
                                 tr("Which set number should be removed: "),
                                 curr_row, 0, how_many, 1, &ok);
    if (ok){
        //           cout << "Sukces, przyjete = " << i << endl;
        if(set_of_markers.size())
        {
            auto iter= set_of_markers.begin() +i;
            set_of_markers.erase(iter );

            refresh();
        }
        //           for(uint el = 0;  el < spectrum_1D::set_of_markers.size() ; el++)
        //           {
        //               for(uint m = 0;  m < spectrum_1D::set_of_markers[el].size() ; m++)
        //               {
        //                   cout << spectrum_1D::set_of_markers[el][m] << "  ";
        //               }
        //               cout << endl;

        //           }
    }


    spectrum_1D::store_all_marker_on_a_disk();
}
//*******************************************************************************
void t4markers_manager::on_pushButton_insert_clicked()
{

    // Take markers from current spectrum (just take and show)

    string txt = widmo_ptr->take_integration_and_bgr_markers();

    // present it and ask if this is what you want to include
    string msg = "The current markers are: \n" + txt ;
    msg += "\n\nDo you want to inser them to this list?";
    int result = QMessageBox::information
            (this, "Inserting markers" , msg.c_str(),
             "Yes", "No", "Cancel");

    if(result == 0)  // yes
    {
       // include and save it on a disk
       widmo_ptr->remember_integration_and_bgr_markers();
    }


    refresh();

}
