#include "t4merge_2d_spectra_dlg.h"

// #define QRegExp QRegularExpression
#include "ui_t4merge_2d_spectra_dlg.h"
// #include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
#include "paths.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <cmath>
#include "tplate_spectrum.h"
#include "appl_form.h"

extern appl_form  *appl_form_ptr;
//************************************************************************************************
T4merge_2d_spectra_dlg::T4merge_2d_spectra_dlg ( QWidget *parent ) :
    QDialog ( parent ),
    ui ( new Ui::T4merge_2d_spectra_dlg )
{
    ui->setupUi ( this );

    ui->table1->setRowCount ( 0 );
    ui->table1->setColumnCount ( 2 ) ; // 2);
    QStringList labels;
    labels << "Spectrum name" << "Factor" ;
    ui->table1->setHorizontalHeaderLabels ( labels );
    ui->table1->resizeColumnsToContents ();

    //ui->table1->horizontalHeader()->  setResizeMode(QHeaderView::Stretch);
    //ui->table1->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    //ui->table1->setSizePolicy ( QSizePolicy::Expanding,QSizePolicy::Expanding );

    restore_periodic_list_from_disk( );
}
//************************************************************************************************
T4merge_2d_spectra_dlg::~T4merge_2d_spectra_dlg()
{
    delete ui;
}
//*******************************************************************************************************
void T4merge_2d_spectra_dlg::on_pushButton_remove_row_released()
{

    // which row is selected
    int nr = ui->table1->currentRow();
    if (nr < 0) return;

    // are you sure?

    if (askYesNoCancel(
                                "Removing the spectrum from the table",
                                QString (
                                    "You are going to remove the row nr %1\n which is: %2  factor %3\n"
                                    "Are you sure?" ).arg ( nr+1 ).
                                arg ( ui->table1->item ( nr, 0 )->text() ).
                                arg ( ui->table1->item ( nr, 1 )->text() )
                               )
         == QMessageBox::Yes )
    {
        // remove it
        ui->table1->removeRow ( nr );
    }
}

//**************************************************************************************************************
void T4merge_2d_spectra_dlg::on_table1_cellDoubleClicked ( int row, int column )
{


    if ( column == 0 )    return;  // we do not edit the matrix name
    // Take the text from the table
    double old_value = ui->table1->item ( row, 1 )->text().toDouble() ;
    // display the dialog
    bool ok;
    double res = QInputDialog::getDouble ( this, "Adding factor",
                                           QString ( "Enter a new value of the factor for spectrum %1" ).arg ( ui->table1->item ( row, 0 )->text()
                                                                                                               ),
                                           old_value, -10000,
                                           10000, 8, &ok);
    if ( ok )
    {
        // user entered something and pressed OK store this back into the table
        QString new_text;
        new_text.setNum ( res );
        //cout << "Adding text" << new_text.toStdString() << " to column " << column  << endl;
        ui->table1->item ( row, column )->setText ( new_text );
    }
    else
    {
        // user pressed Cancel
    }
}
//**************************************************************************************************************
void T4merge_2d_spectra_dlg::on_buttonOk_clicked()   // this is a button EXECUTE
{
    string spec_name;
    double factor;
    bool flag_ok = true;

    vector<int> input_spectrum;
    vector<int>  sum_spectrum;


#define OLD false


#if OLD

    try
    {

        for ( int nr = 0; nr < ui->table1->rowCount() ;  nr++ )
        {
            spec_name = ui->table1->item ( nr, 0 )->text().toStdString();
            factor = ui->table1->item ( nr, 1 )->text().toDouble ( &flag_ok );
            if ( !flag_ok )
            {
                ostringstream s;
                s << "Error while converting the factor from the row nr " << nr+1 << " into a  value";
                throw s.str();
            }




            if( read_in_file ( spec_name, input_spectrum ) == false)
            {
                cout << "Error while reading input spectrum, do something" << endl;
                return;
            }
            if ( nr == 0 )
            {
                binning_sum = binning;
                // if ( binning.bin < 0 ) cout <<"negative  first_binx = " << binning.bin << endl;
                //  length = abs((int)( binx * biny)); // abs is because of a trick with longlong contents of a pixel

                cout << "First spectrum has length " << input_spectrum.size()  << endl;
                sum_spectrum.resize( input_spectrum.size()  );
            }
            else // if this is not the frist, but some next spectrum
            {
                //         cout << "Next spectrum has " << endl;
                // compare binning
                if ( fabs ( binning.bin ) != fabs ( binning_sum.bin )   ||
                     binning.beg != binning_sum.beg     ||
                     binning.end != binning_sum.end     ||
                     binning.bin_y != binning_sum.bin_y   ||
                     binning.beg_y != binning_sum.beg_y     ||
                     binning.end_y != binning_sum.end_y )
                {
                    ostringstream s;
                    s << "Impossible. You may add only the 2D spectra with the same binning. \nThe binning of the spectrum named "
                      << spec_name  << " is:\n"
                      << " X bins= " <<  abs ( binning.bin )
                      << ", X begin= " <<  binning.beg
                      << ", X end = " <<  binning.end
                      << ", Y bins= " <<  binning.bin_y
                      << ", Y begin= " <<  binning.beg_y
                      << ", Y end = " <<  binning.end_y
                      << "\n is wrong, because the first spectum on  the list had the following binning:\n"
                      << " X bins= " <<  abs ( binning_sum.bin )
                      << ", X begin= " <<  binning_sum.beg
                      << ", X end = " <<  binning_sum.end
                      << ", Y bins= " <<  binning_sum.bin_y
                      << ", Y begin= " <<  binning_sum.beg_y
                      << ", Y end = " <<  binning_sum.end_y ;
                    throw s.str();
                }
            }

            //       cout  << "Following binning "
            //           << spec_name  << " is:\n"
            //           << " X bins= " <<  binx
            //           << ", X begin= " <<  begx
            //           << ", X end = " <<  endx
            //           << ", Y bins= " <<  biny
            //           << ", Y begin= " <<  begy
            //           << ", Y end = " <<  endy << endl;

            // adding

            for (unsigned int i = 0 ; i < input_spectrum.size()  ; i++ )
            {
                sum_spectrum[i] += static_cast<int> ( input_spectrum[i] * factor );
            }
        }// end of for loop over all rows of the table

        // asking for a output name

        QString filter;
        Tplate_spectrum::flag_repainting_allowed = false;
        QString fileName = QFileDialog::getSaveFileName ( this,
                                                          tr ( "Type the name of the output spectrum" ),
                                                          gpath.Qspectra(),
                                                          tr ( "2D spectrum from spy-Greware  (*.mat)" ),
                                                          &filter
                                                          );
        Tplate_spectrum::flag_repainting_allowed = true;

#ifdef NIGDY

        QFileDialog* fd = new QFileDialog ( gpath.spectra, "*.mat", this,
                                            "Type the name of the output spectrum",
                                            true );




        fd->setMode ( QFileDialog::AnyFile );
        fd->setFilter ( "2D spectrum from spy-Greware  (*.mat)" );
        QString fileName;
        if ( fd->exec() == QDialog::Accepted )
#endif
            if ( ! fileName.isEmpty() )
            {
                // saving the result

                ofstream out ( fileName.toStdString(), ios::binary );
                if ( !out )
                {
                    throw string ( "Error while opening the result file " ) + fileName.toStdString()+ " for writing " ;
                }
                if(binning_sum.bin > 0) binning_sum.bin = -binning_sum.bin;  // save it negative, bec it is 32 bit word
                
                out.write ( ( char* ) &binning_sum.bin, sizeof ( binning_sum.bin ) );
                out.write ( ( char* ) &binning_sum.beg, sizeof ( binning_sum.beg ) );
                out.write ( ( char* ) &binning_sum.end, sizeof ( binning_sum.end ) );
                out.write ( ( char* ) &binning_sum.bin_y, sizeof ( binning_sum.bin_y ) );
                out.write ( ( char* ) &binning_sum.beg_y, sizeof ( binning_sum.beg_y ) );
                out.write ( ( char* ) &binning_sum.end_y, sizeof ( binning_sum.end_y ) );

                out.write ( ( char* ) sum_spectrum.data(), sizeof ( sum_spectrum[0] ) * sum_spectrum.size()  );
                if ( !out )
                {
                    throw string ( "Error while writing the spectrum " ) + fileName.toStdString()  ;
                }
                out.close();
                sum_spectrum.clear();
                input_spectrum.clear();

                QMessageBox::information ( this,
                                           "Merging the 2D spectra",
                                           QString("Success. Now you may watch this matrix \n%1\nas all other Greware matrices").arg(fileName),
                                           QMessageBox::Ok | QMessageBox::Default,
                                           QMessageBox::NoButton,
                                           QMessageBox::NoButton ); // no error

                QDialog::accept();

            }

    } // endtry
    catch ( string &s )
    {

        QMessageBox::critical ( this,
                                "Merging the 2D spectra",
                                s.c_str(),

                                QMessageBox::Ok | QMessageBox::Default,
                                QMessageBox::NoButton,
                                QMessageBox::NoButton ); // error

    }

#else   // not OLD
    try
    {

        for ( int nr = 0; nr < ui->table1->rowCount() ;  nr++ )
        {
            spec_name = ui->table1->item ( nr, 0 )->text().toStdString();
            factor = ui->table1->item ( nr, 1 )->text().toDouble ( &flag_ok );
            if ( !flag_ok )
            {
                ostringstream s;
                s << "Error while converting the factor from the row nr " << nr+1 << " into a  value";
                throw s.str();
            }


            bool success = calculations(nr, spec_name, factor, sum_spectrum);
            if(!success) break; // break out of loop

        }// end of for loop over all rows of the table

        // asking for a output name

        QString filter;
        Tplate_spectrum::flag_repainting_allowed = false;
        QString fileName = QFileDialog::getSaveFileName ( this,
                                                          tr ( "Type the name of the output spectrum" ),
                                                          gpath.Qspectra(),
                                                          tr ( "2D spectrum from spy-Greware  (*.mat)" ),
                                                          &filter
                                                          );
        Tplate_spectrum::flag_repainting_allowed = true;

#ifdef NIGDY

        QFileDialog* fd = new QFileDialog ( gpath.spectra, "*.mat", this,
                                            "Type the name of the output spectrum",
                                            true );




        fd->setMode ( QFileDialog::AnyFile );
        fd->setFilter ( "2D spectrum from spy-Greware  (*.mat)" );
        QString fileName;
        if ( fd->exec() == QDialog::Accepted )
#endif
            if ( ! fileName.isEmpty() )
            {
                // saving the result

                ofstream out ( fileName.toStdString(), ios::binary );

                if(!save_result2D_spectrum(fileName.toStdString(), sum_spectrum))
                {
                    throw string("Error while saving ");
                }
                showWarningMessage(
                                           "Merging the 2D spectra",
                                           QString("Success. Now you may watch this matrix \n%1\nas all other Greware matrices").arg(fileName)
                                            ); // no error

                QDialog::accept();

            }

    } // endtry
    catch ( string &s )
    {

        showWarningMessage(
                                "Merging the 2D spectra",
                                s.c_str(),
                                QMessageBox::Critical); // error

    }

#endif // not OLD
}
//**********************************************************************
void T4merge_2d_spectra_dlg::on_button_add_clicked()   // add_another line with a spectrum_name
{

    //  cout << "T4merge_2d_spectra_dlg::add_spectrum_name()" << endl;
    // dialog to choose the name and the format

#define OLD false
#if OLD == true
    QString fileName = "nazwa";

    Tplate_spectrum::flag_repainting_allowed = false;
    QString filter;
    fileName = QFileDialog::getOpenFileName( this,
                                             tr ( "Select a 2D spectrum file to be merged" ),
                                             gpath.Qspectra(),
                                             tr ( "names of Greware matrices names  (*.mat)" ),
                                             &filter
                                             );
    Tplate_spectrum::flag_repainting_allowed = true;

    if ( fileName.isEmpty() == false )
    {
        int nr = ui->table1->rowCount();
        ui->table1->setRowCount ( nr+1 );

        QTableWidgetItem *newItem = new QTableWidgetItem ( fileName );
        newItem->setFlags ( Qt::ItemIsEnabled );
        ui->table1->setItem ( nr, 0, newItem );

        QTableWidgetItem *newItem2 = new QTableWidgetItem ( "1" ); //  addinf factor 1
        newItem2->setFlags ( Qt::ItemIsEnabled );
        ui->table1->setItem ( nr, 1, newItem2 );

        ui->table1->resizeColumnsToContents ();
    }
#else
    QStringList list;
    QString onefileName = "nazwa";

    Tplate_spectrum::flag_repainting_allowed = false;
    QString filter;
    list = QFileDialog::getOpenFileNames( this,
                                          tr ( "Select a 2D spectrum file to be merged" ),
                                          gpath.Qspectra(),
                                          tr ( "names of Greware matrices names  (*.mat)" ),
                                          &filter
                                          );
    Tplate_spectrum::flag_repainting_allowed = true;

    QStringList::Iterator it = list.begin();
    while(it != list.end()) {
        onefileName = (*it);
        ++it;


        if ( onefileName.isEmpty() == false )
        {
            int nr = ui->table1->rowCount();
            ui->table1->setRowCount ( nr+1 );

            QTableWidgetItem *newItem = new QTableWidgetItem ( onefileName );
            newItem->setFlags ( Qt::ItemIsEnabled );
            ui->table1->setItem ( nr, 0, newItem );

            QTableWidgetItem *newItem2 = new QTableWidgetItem ( "1" ); //  addinf factor 1
            newItem2->setFlags ( Qt::ItemIsEnabled );
            ui->table1->setItem ( nr, 1, newItem2 );

            ui->table1->resizeColumnsToContents ();
        }
    }
#endif
}
//************************************************************************************************************************
// this code is almost literally cut and paste from the class spectrum_2D
bool T4merge_2d_spectra_dlg::read_in_file ( string fname, vector< int >& spectrum_table )
{

    bool too_short = true ;
    // loop to repeat 'too-short' read (when spy writes this matrix just now
    for ( int n_try = 0 ; n_try < 6 && too_short ; n_try++ ) //
    {

        //     cout << "Function: Read in file, parameter " << name << endl ;
        spectrum_table.clear() ;
        // reading from the disk file  ASCII
        string path_filename = /*gpath.spectra +*/  fname ;
        ifstream plik ( path_filename.c_str(), ios::binary );

        if ( plik )
        {
            [[__maybe_unused__]]  int ile = 0 ;

            // at first read binningsa
            double tab[6] = {0};   // note, all values, also bin, bin_y are double (not  int)
            plik.read ( ( char* ) tab, sizeof ( tab ) );

            if ( !plik )
            {
                //            cout << "Error while reading the binning data " << endl;
                continue;
            }
            // NOTE:      the trick is that when the binning.bin value is given as "negative" - this means, that the matrix
            //             is not 16 bit word (short int)
            //             but 32 bit word (normal int)

            bool flag_this_is_32bit_cell = tab[0] < 0 ?   true:false;

            binning.bin = fabs (  tab[0] ) ;
            binning.beg = tab[1];
            binning.end = tab[2];



            // ------ Y
            binning.bin_y =  tab[3];
            binning.beg_y = tab[4];
            binning.end_y = tab[5];



            
            //             cout << "binning -------111111111111111111111111111111111111111111111111\n" ;
            //             for ( int i = 0 ; i < 6 ; i ++ )/
            //                 cout << " tab[" << i << "] = " << tab[i] ;
            //
            //             cout << "\n binning.bin= " << binning.bin
            //                  << " binning.beg=" << binning.beg
            //                  << " binning.end=" << binning.end
            //                  << " binning.waga=" <<  binning.waga
            //
            //
            //                  << "\nbinning.beg_y=" << binning.beg_y
            //                  << " binning.end_y=" << binning.end_y
            //                  << " binning.bin_y= " << binning.bin_y
            //                  << " binning.waga_y=" <<  binning.waga_y << endl;


            if ( flag_this_is_32bit_cell == false )   // standard situation 16 bit
            {
                short int short_value ;
                do
                {
                    plik.read ( ( char* ) &short_value, sizeof ( short_value ) ) ; // for simulaion it is commente
                    ile++ ;
                    if ( !plik )
                    {
                        //cout << "error while reading ile=" << ile << endl;
                        break ;
                    }
                    spectrum_table.push_back ( short_value ) ;
                }
                while ( plik ) ;
            }
            else   // 32 bit situation, modern
            {
                int int_value ;
                do
                {
                    plik.read ( ( char* ) &int_value, sizeof ( int_value ) ) ; // for simulation it is commented
                    ile++ ;
                    if ( !plik )
                    {
                        //cout << "error while reading ile=" << ile << endl;
                        break ;
                    }
                    spectrum_table.push_back ( int_value ) ;
                }
                while ( plik ) ;
            }

            /*
                         cout << "sizeof(int) = " << sizeof(int)
                         << "  sizeof(short int) = " << sizeof(short int)
                         << "  sizeof(long) = " << sizeof(long)
                         << endl;

                        // now
                        cout << " read  spectrum , ile= "<< ile << endl ;

                              cout << "spectrum_table.size = " << spectrum_table.size()
                                    << ", sqrt = " <<sqrt(spectrum_table.size()) << endl ;
                                       */

            if ( spectrum_table.size() == ( unsigned ) ( binning.bin * binning.bin_y ) )
            {
                too_short = false ; // <--- no more tries
            }
            else
            {
                //cout << "problem with reading 2D spec, another try.. " << n_try << endl;
                continue; // another try
            }
            //   binning.bin = (int) (sqrt( (double)spectrum_table.size()));
            //   binning.bin_y =  (int) sqrt( (double)spectrum_table.size());

            // find the real descriptions of the x axis (for example -200, 200)

            //   if(! find_x_description(name))
            //     {
            //
            //          if(timer_refresh)
            //          {
            //            timer_refresh->stop();
            //            delete timer_refresh ;
            //            timer_refresh = 0 ;
            //          }
            //
            //    // warning
            //    string mess =  "Matrix:   \n\t" ;
            //    mess += name ;
            //          mess += "\n is not anymore known !\n\n"
            //          "Possible reasons:\n"
            //          "1. Probbably just now you started the new analysis (spy) and this matrix is not"
            //          " yet created,\n"
            //          "   - if so: wait some time and recall the matrix again\n\n"
            //          "2. The current analysis (spy) is not collecting such a matrix anymore\n"
            //          "   - if so: forget this matrix, delete it from spectra directory\n\n\n"
            //
            //          "More detailed explanations:\n\n"
            //          "Matrix needs to know the parameters of X and Y axis (range and bining)\n"
            //          "This informations should be in the file:   descrption_of_ranges.txt\n"
            //    "In the description file created by the last _last_ analysis run,\n"
            //    "there is no mention about your matrix\n" ;
            //
            //    QMessageBox::information(this, "Information not found", mess );
            //
            //    binning.beg = 0 ;
            //    binning.end =binning. bin -1  ;       //   333 ;
            //    // vertical axis
            //    binning.beg_y = 0 ;
            //    binning.end_y = binning.bin_y -1  ;       //   333 ;
            //     }


        }
        else   // if ( ! plik)   // error while opening the file
        {
            //             if ( timer_refresh )
            //             {
            //                 timer_refresh->stop(); // 20 seconds
            //                 delete timer_refresh ;
            //                 timer_refresh = 0 ;
            //             }
            //cout << " Error while opening a spectrum " << path_filename << endl ;
            string mess =  "Matrix \n";
            mess += fname ;
            mess += "\nwas not found \n" ;
            QMessageBox::information ( this, "Error while opening file", mess.c_str() );

            spectrum_table.push_back ( 0 ) ;
            binning.beg = 0 ;
            binning.end = 1 ;
            binning. bin = 1  ;       //   333 ;
            // vertical axis
            binning.beg_y = 0 ;
            binning.end_y = 1 ;
            binning.bin_y = 1  ;       //   333 ;

            too_short = false ; // <--- no more tries

        }

    } // end of for (too short read)

    return  !too_short;
}
//###################################################################################
void T4merge_2d_spectra_dlg::on_pushButton_restore_the_list_clicked()
{
    QString filter;
    Tplate_spectrum::flag_repainting_allowed = false;
    QString fileName = QFileDialog::getOpenFileName ( this,
                                                      tr ( "Select a name for this list to be saved" ),
                                                      gpath.Quser_def_spectra(),
                                                      tr ( "name of recepit file name  (*.2dsum_txt)" ),
                                                      &filter
                                                      );
    Tplate_spectrum::flag_repainting_allowed = true;


    if ( fileName.isEmpty() == false )
    {
        //        if(fileName.endsWith(".2dsum_txt") == false)
        //        {
        //            fileName += ".2dsum_txt";
        //        }

        ifstream plik(fileName.toStdString());
        if(!plik)
        {
            throw string ( "Error while opening the file for a recepit of the sum" ) + fileName.toStdString()+ " for writing " ;
        }

        ui->table1->clearContents();
        ui->table1->setRowCount(0);

        while(true)
        {
            string spec_name;
            double factor;
            plik >> spec_name;
            plik >> factor;
            if(!plik) break;


            int nr = ui->table1->rowCount();
            ui->table1->setRowCount ( nr+1 );

            QTableWidgetItem *newItem = new QTableWidgetItem ( spec_name.c_str() );
            newItem->setFlags ( Qt::ItemIsEnabled );
            ui->table1->setItem ( nr, 0, newItem );


            QTableWidgetItem *newItem2 =
                    new QTableWidgetItem ( to_string(factor).c_str() );
            newItem2->setFlags ( Qt::ItemIsEnabled );
            ui->table1->setItem ( nr, 1, newItem2 );

            ui->table1->resizeColumnsToContents ();

        }// end while
    }// endif
}
//###################################################################################

void T4merge_2d_spectra_dlg::on_pushButton_save_the_list_clicked()
{
    QString filter;
    Tplate_spectrum::flag_repainting_allowed = false;
    QString fileName = QFileDialog::getSaveFileName ( this,
                                                      tr ( "Select a name for this list to be saved" ),
                                                      gpath.Quser_def_spectra(),
                                                      tr ( "name of recepit file name  (*.2dsum_txt)" ),
                                                      &filter
                                                      );
    Tplate_spectrum::flag_repainting_allowed = true;


    if ( fileName.isEmpty() == false )
    {
        if(fileName.endsWith(".2dsum_txt") == false)
        {
            fileName += ".2dsum_txt";
        }

        ofstream plik(fileName.toStdString());
        if(!plik)
        {
            throw string ( "Error while opening the file for a recepit of the sum" ) + fileName.toStdString()+ " for writing " ;
        }
        for ( int nr = 0; nr < ui->table1->rowCount() ;  nr++ )
        {
            string spec_name = ui->table1->item ( nr, 0 )->text().toStdString();
            bool flag_ok;
            double factor = ui->table1->item ( nr, 1 )->text().toDouble ( &flag_ok );
            if ( !flag_ok )
            {
                ostringstream s;
                s << "Error while converting the factor from the row nr " << nr+1 << " into a  value";
                throw s.str();
            }

            plik << spec_name << "  " << factor << endl;
        }

    }
}
//###################################################################################

void T4merge_2d_spectra_dlg::on_pushButton_add_to_automate_list_clicked()
{
    QString filter;
    Tplate_spectrum::flag_repainting_allowed = false;
    QString fileName = QFileDialog::getOpenFileName ( this,
                                                      tr ( "Select a name for this list to be saved" ),
                                                      gpath.Quser_def_spectra(),
                                                      tr ( "name of recepit file name  (*.2dsum_txt)" ),
                                                      &filter
                                                      );
    Tplate_spectrum::flag_repainting_allowed = true;

    if ( fileName.isEmpty() == false )
    {
        //        if(fileName.endsWith(".2dsum_txt") == false)
        //        {
        //            fileName += ".2dsum_txt";
        //        }

        //        ifstream plik(fileName.toStdString());
        //        if(!plik)
        //        {
        //            throw string ( "Error while opening the file for a recepit of the sum" ) + fileName.toStdString()+ " for writing " ;
        //        }


        ui->listWidget_add_periodically->addItem(fileName);
        save_periodic_list_to_disk( );
    }// endif
}
//###################################################################################
void T4merge_2d_spectra_dlg::on_pushButton_remove_from_automate_list_clicked()
{
    int how_many =  ui->listWidget_add_periodically->count();

    cout << "there are nr of rows = " << how_many << endl;

    for(int i = 0 ; i < how_many ;++i)
    {
        string tmp = ui->listWidget_add_periodically->item(i)->text().toStdString();
        cout << tmp << endl;
    }

    int current_row = ui->listWidget_add_periodically->currentRow();
    cout << "   current row = " << current_row << endl;
    if(current_row < 0) return;

    if(current_row > ui->listWidget_add_periodically->count() ) return;

    //    ui->listWidget_add_periodically->removeItemWidget(widg);  // THIS DOES NOT WORK
    // so it is suggested to take it from the list and delete it manually
    auto widg = ui->listWidget_add_periodically->takeItem(current_row);

    cout << "trying to remove item = " << widg->text().toStdString() << endl;

    delete widg;
    repaint();

    how_many =  ui->listWidget_add_periodically->count();
    cout << "After removing  nr of rows = " << how_many << endl;

    for(int i = 0 ; i < how_many ;++i)
    {
        string tmp = ui->listWidget_add_periodically->item(i)->text().toStdString();
        cout << tmp << endl;
    }
    save_periodic_list_to_disk( );

}
//************************************************************************************
bool T4merge_2d_spectra_dlg::save_periodic_list_to_disk( )
{
    int how_many =  ui->listWidget_add_periodically->count();
    //cout << "there are nr of rows = " << how_many << endl;

    // open the text file
    string pat_name = gpath.commands + periodic_list_file.toStdString();
    ofstream file(pat_name);
    if(!file){
        cout << "Error while opening periodic summing list file " << endl;
        return false; // perhaps it was never created yet
    }

    // save the list

    for(int i = 0 ; i < how_many ;++i)
    {
        string tmp = ui->listWidget_add_periodically->item(i)->text().toStdString();
        // cout << tmp << endl;
        file << tmp << endl;
    }
    file.close();
    return true;
}
//************************************************************************************
bool T4merge_2d_spectra_dlg::restore_periodic_list_from_disk( )
{
    // should be done always after starting greware

    // open the text file
    string pat_name = gpath.commands + periodic_list_file.toStdString();
    ifstream file(pat_name);
    if(!file){
        cout << "Error while opening periodic summing list file "<< endl;
        return false; // perhaps it was never created yet
    }

    // read the contents of the file
    for( ;  ; )
    {
        string name;
        file >> name;
        if(!file) break;
        ui->listWidget_add_periodically->addItem(name.c_str() );
        // cout << tmp << endl;
    }
    file.close();
    return true;
}
//************************************************************************************
bool T4merge_2d_spectra_dlg::execute_periodic_summings()
{
    // 0. Open the periodic_list_file
    string pat_name = gpath.commands + periodic_list_file.toStdString();
    ifstream file(pat_name);
    if(!file){
        cout << "Error while opening periodic summing list file "<< endl;
        return false; // perhaps it was never created yet
    }

    vector<string>   periodic_list;
    string   result_spectra;

    // 1. read int the periodic list of summings into one vector
    // read the contents of the file
    for( ;  ;)
    {
        string name;
        file >> name;
        if(!file) break;

        periodic_list.push_back(name);
        // cout << tmp << endl;
    }
    file.close();


    // scan through it ----------------------
    for( auto sum_list : periodic_list)
    {
        // open this sum_list
        ifstream list_file(sum_list);
        if(!list_file){
            cout << "Error while opening the file " << sum_list << endl;
            exit(2);
        }

        string output_spectrum = sum_list ;
        // removing a path, and replacing with "spectra"
        string::size_type kon_sciezki = output_spectrum.rfind('/');
        if(kon_sciezki != string::npos)
        {
            output_spectrum.erase(0, kon_sciezki+1);
        }
        output_spectrum = gpath.spectra + output_spectrum;


        string::size_type kropka =  output_spectrum.rfind(".");
        if(kropka != string::npos)
        {    output_spectrum.erase(kropka);}

        output_spectrum += ".mat";

        result_spectra += output_spectrum + "\n";

        vector<int> sum_spectrum;
        for(int f = 0 ; ; ++f){
            // read the particular file name
            // read his factor of summing
            string spec_name;
            double factor;
            list_file >> spec_name >> factor ;
            if(!list_file)
                break;
            cout   << "Spectrum " << spec_name << " should be added with a factor "
                   << factor << endl;

            // runn the procedure of summing
            calculations(f, spec_name, factor, sum_spectrum);

        }
        // save the result under the name sum_list.mat
        cout << "saving the result matrix as a file: " << output_spectrum << endl;
        save_result2D_spectrum(output_spectrum, sum_spectrum);
    }
    cout << "Finished with all periodic summing" << endl;

    string txt = "Successfully created the following 2D sum spectra:\n" + result_spectra +
            "\nNow you may watch them as all usual greware matrices ";
    showWarningMessage(
                               "Merging the 2D spectra",
                               QString(txt.c_str()), QMessageBox::Information ); // no error

    QDialog::accept();

    return true;
}
//##########################################################################################
void T4merge_2d_spectra_dlg::on_pushButton_exec_periodic_manually_clicked()
{
    execute_periodic_summings();
}
//##########################################################################################
// add single matrix to...
// arguments  nr_of_spectrum
// - if this is a first spectrum of summing, we resize the result spectrum
// - spec_name name of the input spectrum
// - its factor
// - the result spectrum where sthe summing should be done

bool T4merge_2d_spectra_dlg::calculations(int nr_of_spectrum, string spec_name, double factor, vector<int> &sum_spectrum)
{
    //    string spec_name;
    //    double factor;
    //    bool flag_ok = true;

    vector<int> input_spectrum;
    //    vector<int>  sum_spectrum;


    try{

        if( read_in_file ( spec_name, input_spectrum ) == false)
        {
            cout << "Error while reading input spectrum, do something" << endl;
            return false;
        }

        if ( nr_of_spectrum == 0 )
        {
            binning_sum = binning;
            // if ( binning.bin < 0 ) cout <<"negative  first_binx = " << binning.bin << endl;
            //  length = abs((int)( binx * biny)); // abs is because of a trick with longlong contents of a pixel

            cout << "First spectrum has length " << input_spectrum.size()  << endl;
            sum_spectrum.resize( input_spectrum.size()  );
        }
        else // if this is not the frist, but some next spectrum
        {
            //         cout << "Next spectrum has " << endl;
            // compare binning
            if ( fabs ( binning.bin ) != fabs ( binning_sum.bin )   ||
                 binning.beg != binning_sum.beg     ||
                 binning.end != binning_sum.end     ||
                 binning.bin_y != binning_sum.bin_y   ||
                 binning.beg_y != binning_sum.beg_y     ||
                 binning.end_y != binning_sum.end_y )
            {
                ostringstream s;
                s << "Impossible. You may add only the 2D spectra with the same binning. \nThe binning of the spectrum named "
                  << spec_name  << " is:\n"
                  << " X bins= " <<  abs ( binning.bin )
                  << ", X begin= " <<  binning.beg
                  << ", X end = " <<  binning.end
                  << ", Y bins= " <<  binning.bin_y
                  << ", Y begin= " <<  binning.beg_y
                  << ", Y end = " <<  binning.end_y
                  << "\n is wrong, because the first spectum on  the list had the following binning:\n"
                  << " X bins= " <<  abs ( binning_sum.bin )
                  << ", X begin= " <<  binning_sum.beg
                  << ", X end = " <<  binning_sum.end
                  << ", Y bins= " <<  binning_sum.bin_y
                  << ", Y begin= " <<  binning_sum.beg_y
                  << ", Y end = " <<  binning_sum.end_y ;
                throw s.str();
            }
        }

        //       cout  << "Following binning "
        //           << spec_name  << " is:\n"
        //           << " X bins= " <<  binx
        //           << ", X begin= " <<  begx
        //           << ", X end = " <<  endx
        //           << ", Y bins= " <<  biny
        //           << ", Y begin= " <<  begy
        //           << ", Y end = " <<  endy << endl;

        // adding

        for (unsigned int i = 0 ; i < input_spectrum.size()  ; i++ )
        {
            sum_spectrum[i] += static_cast<int> ( input_spectrum[i] * factor );
        }

        return true;


    } // end try
    catch ( string &s )
    {

        showWarningMessage(
                                "Merging the 2D spectra",
                                s.c_str(),
                                QMessageBox::Critical); // error

        return false;
    }


}
//*******************************************************************************************
bool T4merge_2d_spectra_dlg::save_result2D_spectrum(string fileName, vector<int> & sum_spectrum)
{
    if ( ! fileName.empty() )
    {
        // saving the result

        ofstream out ( fileName, ios::binary );
        if ( !out )
        {
            throw string ( "Error while opening the result file " ) + fileName+ " for writing " ;
        }
        if(binning_sum.bin > 0) binning_sum.bin = -binning_sum.bin;  // save it negative, bec it is 32 bit word

        out.write ( ( char* ) &binning_sum.bin, sizeof ( binning_sum.bin ) );
        out.write ( ( char* ) &binning_sum.beg, sizeof ( binning_sum.beg ) );
        out.write ( ( char* ) &binning_sum.end, sizeof ( binning_sum.end ) );
        out.write ( ( char* ) &binning_sum.bin_y, sizeof ( binning_sum.bin_y ) );
        out.write ( ( char* ) &binning_sum.beg_y, sizeof ( binning_sum.beg_y ) );
        out.write ( ( char* ) &binning_sum.end_y, sizeof ( binning_sum.end_y ) );

        out.write ( ( char* ) sum_spectrum.data(), sizeof ( sum_spectrum[0] ) * sum_spectrum.size()  );
        if ( !out )
        {
            throw string ( "Error while writing the spectrum " ) + fileName  ;
        }
        out.close();
        sum_spectrum.clear();


        return true;
    }
    return false;
}
//******************************************************************************************
void T4merge_2d_spectra_dlg::on_pushButton_screen_spectra_clicked()
{


    QList <QMdiSubWindow*> windows = appl_form_ptr->give_workspace()->subWindowList();

    Tplate_spectrum * dokument   = NULL ;

    for(int nr_dok = 0 ; nr_dok < windows.count() ; nr_dok++)
    {
        dokument  = dynamic_cast< Tplate_spectrum * >( windows[nr_dok]->widget() ) ;
        if(!dokument) break;

        string pathed_name {  gpath.spectra + dokument->give_spectrum_name().c_str() };
        QString onefileName {  pathed_name.c_str() };

        //ui->ListBox_chosen->addItem ( name.c_str()   );


        if ( onefileName.isEmpty() == false )
        {
            int nr = ui->table1->rowCount();
            ui->table1->setRowCount ( nr+1 );

            QTableWidgetItem *newItem = new QTableWidgetItem ( onefileName );
            newItem->setFlags ( Qt::ItemIsEnabled );
            ui->table1->setItem ( nr, 0, newItem );

            QTableWidgetItem *newItem2 = new QTableWidgetItem ( "1" ); //  addinf factor 1
            newItem2->setFlags ( Qt::ItemIsEnabled );
            ui->table1->setItem ( nr, 1, newItem2 );

            ui->table1->resizeColumnsToContents ();
        }
    }



}
