#include "t4manager_user_spectra.h"
#include "ui_t4manager_user_spectra.h"

#include "t4user_spectrum_wizard.h"
#include "user_spectrum_description.h"
#include "paths.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>


#include <QProgressDialog>
#include <QDir>
#include <QMessageBox>
#include "t4pattern_defining_dlg.h"
#include "t4incrementer_selector.h"

#include "appl_form.h"
extern appl_form  *appl_form_ptr;

//*************************************************************************************************
T4manager_user_spectra::T4manager_user_spectra(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4manager_user_spectra)
{
    ui->setupUi(this);

    int     nr_rows = 0;
    ui->table->setRowCount( nr_rows );
    ui->table->setColumnCount(5);

    QStringList labels;
    labels << "Active" << "Name of the spectrum" << "Type" << "Condition assigned" << "Selfgates" ;
    ui->table->setHorizontalHeaderLabels ( labels );
    ui->table->resizeColumnsToContents ();

    labels.clear();
    labels << "Spectrum name" << "Dim"
           << "X bins" << "X beg" << "X end"
           << "Y bins" << "Y beg" << "Y end"
           << "X incrementers" << "its selfgate"
           << "Y incrementers" << "its selfgate"
           << "assigned condition" ;

    ui->review->setHorizontalHeaderLabels ( labels );
    ui->review->resizeColumnsToContents ();


    //ui->review->hide();
    //ui->review->setEnabled(false);
    //push_log->setEnabled(false);

    update_the_table() ;
    update_review();

}
//****************************************************************************************************

T4manager_user_spectra::~T4manager_user_spectra()
{
    delete ui;
}
//****************************************************************************************************

void T4manager_user_spectra::on_push_create_new_clicked()
{
    //cond_spec_wizard * dlg = new cond_spec_wizard ;
    T4user_spectrum_wizard * dlg = new T4user_spectrum_wizard ;
    user_spectrum_description us;

    dlg->set_parameters(&us, false) ;  // false means no editing, just creating
    if ( dlg->exec() == QDialog::Accepted )
    {

        // if it was succesfull - put into a vector
        vec_spectra.push_back(us) ;
        // display all current contents of the
        int last = vec_spectra.size() - 1 ;
        vec_spectra[last].name_of_spectrum
                = is_name_unique(vec_spectra[last].name_of_spectrum, last );
        vec_spectra[last].write_definitions(gpath.user_def_spectra);
        update_the_table() ;
        appl_form_ptr-> warning_spy_is_in_action();
    }
    raise();   // to the top of desktop
    // if not - delete it
}

//***********************************************************************
void T4manager_user_spectra::update_the_table()
{

    // As sometimes this take very long - we show progress dialog
    int how_many_items = 100;
    QString infor("Some disks (directories) can be very slow!\nReading the directory: ");
    infor += gpath.Quser_def_spectra() + "..." ;

    QProgressDialog progress( infor,
                              "Abort reading", 0, how_many_items,
                              this);
    progress.setMinimumDuration (5000) ; // miliseconds
    progress.setValue(2);
    qApp->processEvents();

    //----------------------

    vec_spectra.clear() ;
    // read the spectra from the disk and load them to the vector
    //   vector<user_spectra> vec_spectra

    // taking the spectra names from the directory - only

    QDir katalog2(gpath.Quser_def_spectra() );

    QString filtr = ui->ComboBox_filter->currentText ();
    QString name_filter = "*" + filtr + ("*" + user_spec_ext).c_str() ;

    progress.setValue(2);
    qApp->processEvents();
    how_many_items = katalog2.count() ;
    progress.setMaximum(how_many_items);

    // in the polygon dir
    QStringList lista = katalog2.entryList (QStringList(name_filter) ) ;

    int nr = 0 ;
    for ( QStringList::Iterator it = lista.begin() ; it != lista.end() ;  ++it, nr++ )
    {
        if(! (nr % 10))
        {
            //     cout << "nr = " << nr << endl;
            progress.setValue( nr );
            qApp->processEvents();
        }
        if ( progress.wasCanceled() )
            break;


        user_spectrum_description us ;

        // cout << "Trying to read spectrum definition " << *it << endl;

        us.name_of_spectrum = (*it).toStdString();
        int pos = us.name_of_spectrum.rfind(".") ;
        us.name_of_spectrum.erase(pos) ;

        string name_to_compare = us.name_of_spectrum ; // to trace the possible error

        us.read_from(gpath.user_def_spectra + (*it).toStdString() );
        if(us.name_of_spectrum != name_to_compare)
        {
            cout << "Error - the spectrum name inside the file is different from the file_name ("
                 << name_to_compare << " while inside is "
                 << us.name_of_spectrum << ") fixing this error...\n" ;
            // This may happen when somebody renamed the spectrum
            // using the operating system mv command.

            // fixing this error
            us.name_of_spectrum = name_to_compare ; // correct it properly
            us.write_definitions(gpath.user_def_spectra); // and save to the disk
        }

        vec_spectra.push_back(us) ;

        //
        // combo->insertItem(short_long_name(*it));
    }
    progress.setValue( how_many_items );

    int nr_rows =vec_spectra.size();

    ui->table->clearContents();
    ui->table->setRowCount( nr_rows );
    ui->table->setColumnCount(5);


    for (int j = 0; j < nr_rows; ++j )
    {
        //      ui->table->setItem( j, 0, new QCheckTableItem( table, "" ));  // safe !
        ui->table->setItem( j, 0, new QTableWidgetItem(Qt::ItemIsUserCheckable) );  // safe !

        ui->table->setItem ( j, 1,
                             new QTableWidgetItem (
                                 vec_spectra[j].name_of_spectrum.c_str(),
                                 Qt::ItemIsEnabled )
                             );


        ui->table->setItem(j, 2,
                           new QTableWidgetItem (vec_spectra[j].dimmension == 1 ? "1D": "2D"));

        string nam_no_ext = vec_spectra[j].condition_name.substr(0,vec_spectra[j].condition_name.rfind("."));
        ui->table->setItem(j, 3,
                           new QTableWidgetItem (nam_no_ext.c_str() ));

        // here should be the loop over all used selfgates (on X and Y incrementors
        //ui->table->setText(j, 4, "inf about slef-gates not implemented yet");
        ui->table->setItem(j, 4,
                           new QTableWidgetItem (
                               vec_spectra[j].get_selfgates().c_str(), Qt::NoItemFlags )  );


        // check/uncheck
        ui->table->item(j, 0)->   setCheckState(  vec_spectra[j].enabled ? Qt::Checked : Qt::Unchecked);

        table_changed(j, 0) ; // to enable/disable columns "spec name" and "type"

    }
    ui->table->resizeColumnsToContents ();
    update_review();
}
//****************************************************************************************************

//****************************************************************************************************

void T4manager_user_spectra::table_changed( int row, int col )
{
    if(row >= ui->table->rowCount() ) return;
    //    cout << "Table value changed row = " << row << ", col " << col << endl;
    bool stan  ;
    if(col == 0)
    {
        stan =( (ui->table->item( row, 0)->checkState () ) == Qt::Checked );
        ui->table->item( row, 1)->setFlags(stan ? (Qt::ItemIsEnabled | Qt::ItemIsSelectable): (Qt::NoItemFlags| Qt::ItemIsSelectable) ) ;   // name
        ui->table->item( row, 2)->setFlags(stan ? (Qt::ItemIsEnabled | Qt::ItemIsSelectable): (Qt::NoItemFlags| Qt::ItemIsSelectable) ) ;    // type
        ui->table->item( row, 3)->setFlags(stan ? (Qt::ItemIsEnabled | Qt::ItemIsSelectable): (Qt::NoItemFlags| Qt::ItemIsSelectable) ) ;    // condition
        ui->table->item( row, 4)->setFlags(stan ? (Qt::ItemIsEnabled | Qt::ItemIsSelectable): (Qt::NoItemFlags| Qt::ItemIsSelectable) ) ;   ;
        vec_spectra[row].enabled = stan ;
        vec_spectra[row].write_definitions(gpath.user_def_spectra) ;
    }

}
//****************************************************************************************************

void T4manager_user_spectra::update_review()
{

    QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
    ui->review->hide();
    // ui->review->setUpdatesEnabled( false );


    int nr_rows =vec_spectra.size();
    //ui->review->setRowCount( nr_rows );
    ui->review->clearContents();

    int linijka = 0 ;
    for (int j = 0; j < nr_rows; ++j, linijka++ )
    {
        ui->review->setRowCount(linijka+1);
        ui->review->setItem(linijka, 0,
                            new QTableWidgetItem( vec_spectra[j].name_of_spectrum.c_str()));

        //        cout << "spectrum name = " <<  vec_spectra[j].name_of_spectrum.c_str() << endl;


        ui->review->setItem(linijka, 1,
                            new QTableWidgetItem(vec_spectra[j].dimmension == 1 ? "1D": "2D"));

        // binning
        ui->review->setItem(linijka, 2,
                            new QTableWidgetItem(QString("%1").arg(vec_spectra[j].give_bins_x())));
        ui->review->setItem(linijka, 3,
                            new QTableWidgetItem(QString("%1").arg(vec_spectra[j].give_beg_x())));
        ui->review->setItem(linijka, 4,
                            new QTableWidgetItem(QString("%1").arg(vec_spectra[j].give_end_x())));

        if(vec_spectra[j].dimmension != 1)
        {
            ui->review->setItem(linijka, 5, new QTableWidgetItem(QString("%1").arg(vec_spectra[j].give_bins_y() ))) ;
            ui->review->setItem(linijka, 6, new QTableWidgetItem(QString("%1").arg(vec_spectra[j].give_beg_y()  )));
            ui->review->setItem(linijka, 7, new QTableWidgetItem(QString("%1").arg(vec_spectra[j].give_end_y()  )));
        }
        else
        {
            ui->review->setItem(linijka, 5, new QTableWidgetItem("-") );
            ui->review->setItem(linijka, 6, new QTableWidgetItem("-") );
            ui->review->setItem(linijka, 7, new QTableWidgetItem("-") );
        }
        // General condition
        string nam_no_ext = vec_spectra[j].condition_name.substr(0,vec_spectra[j].condition_name.rfind("."));

        ui->review->setItem(linijka, 12, new QTableWidgetItem(nam_no_ext.c_str() ) );


        // here should be the loop over all used selfgates (on X and Y incrementors

        vector< pair<string, string> > & incrementors_x   =  vec_spectra[j].give_x_incr();
        vector< pair<string, string> > & incrementors_y   =  vec_spectra[j].give_y_incr();

        unsigned k = 0;  // extra rows in table, needed for multiple incrementors
        for(k = 0 ; k < max(incrementors_x.size(), incrementors_y.size()) ; k++, linijka++)
        {
            //            cout << "now we will make many incrementers and selfgates k = " << k << endl;

            if(k >0)
            {
                ui->review->setRowCount(linijka+1);  // for k = 0 we do not need to create new
                //                cout << "Adding (continuation) row of table. Now is " << linijka << endl;
                ui->review->setItem(linijka, 0, new QTableWidgetItem("          (cont.) ") );
            }

            //-------------X ----------------
            string sg = (k < incrementors_x.size()) ?
                        incrementors_x[k].second.substr(0,incrementors_x[k].second.rfind("."))
                      : " --- " ;

            ui->review->setItem(linijka, 8, new QTableWidgetItem ((k < incrementors_x.size()) ? incrementors_x[k].first.c_str() : " --- " ));
            ui->review->setItem(linijka, 9, new QTableWidgetItem(sg.c_str() ) ) ;
            //----------- Y ---------------

            ui->review->setItem(linijka, 10, new QTableWidgetItem ((k < incrementors_y.size()) ? incrementors_y[k].first.c_str() : " --- " ));

            sg = (k < incrementors_y.size()) ?
                        incrementors_y[k].second.substr(0,incrementors_y[k].second.rfind("."))
                      : " --- " ;

            ui->review->setItem(linijka, 11, new QTableWidgetItem(sg.c_str()) ) ;

            // just enabling
            for(int c = 0 ; c < ui->review->columnCount(); c++)
            {
                // cout << "trying to enable item row=" << linijka << ", column =" << c << endl;
                if(ui->review->item(linijka, c) )
                {
                    ui->review->item(linijka, c)->setFlags( Qt::ItemIsEnabled);
                }
            }

        }
        if(k) linijka-- ;
    }


    ui->review->resizeColumnsToContents ();
    ui->review->show() ;
    QApplication::restoreOverrideCursor();
}
//********************************************************************************
//********************************************************************************
string T4manager_user_spectra::is_name_unique( string n, int nr )
{
    // perhaps such a cloned spectrum already exists ?
    bool changed = false ;
    string new_name = n ;
    for(unsigned i = 0 ; i < vec_spectra.size() ; i++)
    {
        if(new_name == vec_spectra[i].name_of_spectrum
                &&
                i != (unsigned)nr )
        {
            new_name += "A" ;
            i = 0 ; // starting again
            changed = true ;
        }
    }
    if(changed)
    {
        QMessageBox::warning(this,
                             "Duplicate name of the spectrum",
                             QString("The name of the spectrum:\n\t%1\n"
                                     "was already existing in the list, so it had to be changed into:\n"
                                     "\t%2\n").arg(n.c_str())
                             .arg(new_name.c_str() ),
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);
    }
    return new_name ;
}

//********************************************************************************
void T4manager_user_spectra::on_table_cellClicked(int row, int column)
{
    table_changed(row, column);
}
//************************************************************************************
void T4manager_user_spectra::on_table_cellDoubleClicked(int /*row*/ , int column)
{
    // here, editing spectrum
    //    cout << "Dobule click at " << row << ", " << column << endl;
    if(column == 1) on_push_edit_selected_clicked();
}
//************************************************************************************
void T4manager_user_spectra::on_push_remove_selected_released()
{

    // here should be a loop over many regions of selecion
    // many lines of every selection

    QList<QTableWidgetSelectionRange> range = ui->table->selectedRanges() ;

    //cout << "Selection has " << (range.count()) << " ranges" << endl;


    if(range.count() == 0)
    {
        QMessageBox::warning(this,
                             "No spectrum name selected",
                             QString("Select the spectrum by clicking once on its name\n"),
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);
        return ;
    }

    bool flag_any_change = false;
    bool flag_yes_to_all = false;

    for(int s = range.count()-1; s >=0 ; s--)    // can be many separated ranges on this list
    {
        cout << "range[s].bottomRow()= " << range[s].bottomRow() << ", range[s].topRow()= " <<  range[s].topRow() << endl;
        for(int row = range[s].bottomRow() ; row >= range[s].topRow() ; row--)
        {
            cout << "remowing row = " << row << " while range[s].topRow() = " << range[s].topRow() << endl;

            //  unsigned row = tab->selectRow();    ro sel currentRow() ;
            if(row >= (signed int) vec_spectra.size() )
            {
                cout << "STRANGE --> row >= (signed int) vec_spectra.size()  ? " << __FILE__ << ":"<< __LINE__  << endl;
                return ;
            }

            int answ = QMessageBox::Cancel;

            if(flag_yes_to_all == false)
            {
                QMessageBox msgBox;
                msgBox.setText("Removing the spectrum definition");
                msgBox.setInformativeText( QString("You are going to remove the definition nr %1\nof the spectrum %2\n"
                                                   "Are you sure?").arg(row+1).arg(vec_spectra[row].name_of_spectrum.c_str() ) ) ;
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::YesToAll |  QMessageBox::No | QMessageBox::Cancel);
                msgBox.setDefaultButton(QMessageBox::No);
                answ = msgBox.exec();
            }

            if(answ == QMessageBox::YesToAll) flag_yes_to_all = true;

            if(answ == QMessageBox::Yes || answ == QMessageBox::YesToAll || flag_yes_to_all )  /*Yes, Yes_to_all */
            {
                // remove from the directory
                string fname = gpath.Quser_def_spectra().toStdString() + vec_spectra[row].name_of_spectrum + user_spec_ext;

                // cout << "Yes, so removing " << fname << ", name taken from the memory, (not from table)" << endl;

                // create 'removed'subdirectrory (if it does not exists) ---------------
                string remdir = gpath.Quser_def_spectra().toStdString() + "removed/" ;

                QDir dir(remdir.c_str() );
                if(dir.exists() == false)
                {
                    dir.mkdir(remdir.c_str());
                }
                else {
                    // cout << "subdir already exists " << endl;
                }

                // --------- now the moving
                string moving_command = "mv " + fname + " ";
                moving_command += remdir + vec_spectra[row].name_of_spectrum + user_spec_ext;
                //cout << "Komenda " << moving_command << endl;
                int result = system(moving_command.c_str());
                if(result) cout << "Problem with moving the definition, so no backup made" << endl;

                // ----- now removing from the memory of the  computer

                //cout << "In the memory this row has " << vec_spectra[row].name << endl;
                vec_spectra.erase(vec_spectra.begin() + row) ;

                ui->table->item(row, 1)->setSelected(false);
                flag_any_change = true;

            }
            else if(answ == QMessageBox::No)
            {
                // just ignore
                ui->table->item(row, 0)->setSelected(false);

            }
            else if(answ == QMessageBox::Cancel)
            {
                goto outside;
            }
            else {
                cout << "Should never be here "<< __FILE__ << ":" << __LINE__ << endl;
            }

        } // for row
    } // for sel range
outside:
    if(flag_any_change)appl_form_ptr-> warning_spy_is_in_action();
    update_the_table() ;
}
//****************************************************************************************************

void T4manager_user_spectra::row_is_selected()
{
    int row_nr = ui->table->currentRow() ;
    ui->table->selectRow (row_nr);
}
//****************************************************************************************************

void T4manager_user_spectra::on_push_edit_selected_clicked()
{

    int nr = ui->table->currentRow() ;
    if(nr >= ui->table->rowCount() || nr == -1)
    {
        QMessageBox::warning(this,
                             "No spectrum selected",
                             QString(nr == -1 ? "Nothing to edit" : "Select the spectrum by clicking once on its name\n"),
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);
        return ;
    }
    row_is_selected();
    //cond_spec_wizard * dlg = new cond_spec_wizard ;

    T4user_spectrum_wizard * dlg = new T4user_spectrum_wizard ;
    dlg->set_parameters(& vec_spectra[nr], true) ; // true = editing, cloning is allowed
    if ( dlg->exec() == QDialog::Accepted )
    {
        vec_spectra[nr].name_of_spectrum
                = is_name_unique(vec_spectra[nr].name_of_spectrum, nr);

        vec_spectra[nr].write_definitions(gpath.user_def_spectra);
        update_the_table() ;
        appl_form_ptr-> warning_spy_is_in_action();
    }
    raise();   // to the top of desktop

}
//*****************************************************************************************
void T4manager_user_spectra::on_push_clone_clicked()
{

    QMessageBox::information(this,
                             "Cloning the definition of the spectrum",
                             QString("If you want to clone some spectrum\n\n"
                                     "   1. Open it for editing (Press button: Edit Selected Spectrum)\n\n"
                                     "   2. On the first page of the editing wizard - change the name of this spectrum\n\n"
                                     "   3. Go to the last page of the wizard - and press Finish\n\n\n"
                                     "Note: By this you created a new spectrum definition on the disk "
                                     "while the original spectrum is not touched"
                                     //"and the new-named spectrum (clone) will be stored on the disk independently\n"
                                     ),
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);
}
//*****************************************************************************************
void T4manager_user_spectra::create_condition()
{
    /*
  T4condition_wizard * dlg = new T4condition_wizard;
  Tcondition_description_cracow  cnd;

  dlg->set_parameters(&cnd) ;
  if ( dlg->exec() == QDialog::Accepted )
  {
    cnd.save_condition();
  }
*/
}
//***********************************************************************
void T4manager_user_spectra::edit_condition()
{
    /*
  condition_wizard * dlg = new condition_wizard;
  dlg->exec();
  */
}
//**********************************************************************
void T4manager_user_spectra::make_report_file()
{

    ofstream plik((gpath.user_def_spectra+ "report_spec.txt").c_str() );

    int nr_rows = ui->review->rowCount();

    for (int j = 0; j < nr_rows; j++ )
    {
        for(int k = 0 ; k < 13; k++)
        {
            //string co = ui->review->text(j, k).ascii() ;
            if( ui->review->item(j, k)->text().isNull() )
                plik << "    " << " ; " ;
            else
                plik << (ui->review->item(j, k)->text().toStdString() ) << " ; " ;
        }
        plik << "\n";

    }

    plik.close() ;

    QString wiad ;
    if(plik)
    {
        wiad = QString("Report succesfully written as \n") +
                (gpath.user_def_spectra+ "report_spec.txt").c_str() ;
        QMessageBox::information(this,
                                 "Report about user defined spectra",
                                 wiad,
                                 QMessageBox::Ok,
                                 QMessageBox::NoButton,
                                 QMessageBox::NoButton);
    }
    else
    {
        wiad = QString("Error while writing the report \n") +
                (gpath.user_def_spectra+ "report_spec.txt").c_str() ;

        QMessageBox::critical(this,
                              "Report about user defined spectra",
                              wiad,
                              QMessageBox::Ok,
                              QMessageBox::NoButton,
                              QMessageBox::NoButton);
    }
    return ;

}
//**********************************************************************************
void T4manager_user_spectra::on_review_cellDoubleClicked(int /*row*/, int /*column*/ )
{
    review_double_clicked();
}
//********************************************************************************
void T4manager_user_spectra::review_double_clicked()
{
    int nrc = ui->review->currentRow() ;
    if(nrc >= ui->review->rowCount())
    {
        QMessageBox::warning(this,
                             "No spectrum selected",
                             QString("Select the spectrum by clicking once on its name\n"),
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);
        return ;
    }

    cout << "Dbl click on the row of review" << nrc << endl;
    string last_legal_name ;
    for(int i = 0 ; i <= nrc ; i ++)
    {
        string current_name = ui->review->item(i, 0)->text().toStdString() ;
        //    cout << "Checking name " << current_name << endl;
        if(current_name.find("(continuation)") != string::npos)
        {
            //cout << "cont " << endl;
        }
        else
        {
            //cout << " so far  legal was " << last_legal_name << endl;
            last_legal_name = current_name;
        }
    }
    //  cout << "this means the spectrum name " <<  last_legal_name << endl;
    // find this name in the vector
    int which = -1 ;
    for(unsigned i = 0 ; i < vec_spectra.size() ; i++)
    {
        //    cout << "comparing with " << vec_spectra[i].name_of_spectrum << endl;
        if(vec_spectra[i].name_of_spectrum ==  last_legal_name)
        {
            which = i ;
            //       cout << "remembered as spectrum nr " << i << endl ;
        }
    }
    if(which == -1)
        return ;

    //row_is_selected();

    // find such a name in the vector

    T4user_spectrum_wizard * dlg = new T4user_spectrum_wizard ;
    dlg->set_parameters(& vec_spectra[which], true) ; // true = editing, cloning is allowed
    if ( dlg->exec() == QDialog::Accepted )
    {
        vec_spectra[which].name_of_spectrum
                = is_name_unique(vec_spectra[which].name_of_spectrum, which);


        vec_spectra[which].write_definitions(gpath.user_def_spectra);
        update_the_table() ;
        appl_form_ptr-> warning_spy_is_in_action();
    }

}
//****************************************************************************************************
void T4manager_user_spectra::on_ComboBox_filter_textChanged(const QString & /*arg1*/)
{
    static bool flag_working = false ;
    if(flag_working)return;
    flag_working = true;
    update_the_table() ;
    flag_working = false;
    ui->ComboBox_filter->setFocus();
}
//****************************************************************************************************
void T4manager_user_spectra::on_ComboBox_filter_editTextChanged(const QString &arg1)
{
    on_ComboBox_filter_textChanged(arg1);
}
//****************************************************************************************************
void T4manager_user_spectra::on_push_A_1_clone_clicked()
{
    class error
    {
    public:
        string title ;
        string message;
        int kind ; // 1 error, 2 warning
    };
    error e;
    //---------------

    string introduction_txt =  "This is the option to clone a spectrum which contains \n"
                               "at most 2 pattern of characters which had to be smartly replaced\n"
                               "Such substring will be replaced with your desired values"
                               "(i.e. for all possible cluster crystals)\n"
                               "By this action, the set of new spectra wiil be created.\n\n"
                               "The replacement will be done :\n"
                               "    1. Inside  the name of the spectrum,\n"
                               "    2. Inside the name of any incrementer,\n"
                               "    3. Inside the name of any used selfgates (not very useful!)\n"
                               "    4. Inside the name of condition assigned to this spectrum_1D.\n" ;


    bool flag_any_change = false;
    try
    {

        int nr = ui->table->currentRow() ;
        if(nr >= ui->table->rowCount() || nr == -1)
        {
            e.title = "No spectrum selected";
            e.message = "\n\n\nSelect some spectrum name containing patters to be cloned '"
                        "`\n\n\n\nINFO:\n" + introduction_txt;
            ;
            e.kind = 2; //warning;
            throw e;

        }
        row_is_selected();

        // checking if the name of the spectrum contains the pattern (substring A_1)

        string specname = vec_spectra[nr].name_of_spectrum;
#if 0
        if(specname.empty() ||  row == -1)
        {
            e.title = "No spectrum is selected now";
            e.message = "\n\n\nPlease select a spectrum which you want to clone\n\n"
                    +
                    introduction_txt;
            e.kind = 1; // critcial
            throw e;
        }

#endif

        //  reading the whole contents of the template file
        string pathed_name = gpath.user_def_spectra + specname + user_spec_ext;
        //string pathed_name =  specname;
        ifstream plik(pathed_name.c_str() );
        if(!plik)
        {
            string m = "Error while opening the file :";
            m += pathed_name;

            e.title = "Can't open a file ";
            e.message = m ;
            e.kind = 1; // critical
            throw e;
        }

        // read in the whole contents - to replace strings in the memory
        string contents;
        string one_line;
        while(plik)
        {
            getline(plik, one_line);  // delimiter (\n) is not added to the string
            // so we add it
            contents += one_line;
            contents += '\n';
        }




        // string::size_type loc ;
        string pattern1 = "_00_";
        string pattern2  = "";
        string one = "B C";
        string two = "01 02 ";

        vector<string> chain_one;   // for result
        vector<string> chain_two;
        vector<string> filenames;


        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        T4pattern_defining_dlg dlg;
        dlg.set_parameters(specname, pattern1, pattern2, &one, &two, contents);
        if( dlg.exec() != QDialog::Accepted) return;

        dlg.get_parameters(&chain_one, &chain_two, &filenames);

        cout << "After closing the dialog for parameters there are following spectra" << endl;

        for(auto x : filenames)
        {
            cout << x << endl;

        }



        string file_contents_skeleton = dlg.find_patterns_and_make_skeleton_with_procents(contents);

        //        cout << "SZKIELET WIDMA ZAWIERA: \n"<< file_contents_skeleton << endl;

        //  search all the positions of the pattern substrings and replace with  %1 %2

        // FOR loop over all crystals========================================
        // ready to create the new spectra

        string txt = /*introduction_txt + */
                "\n\nDo you really want to create such set of clones ? ";

        for(auto x : filenames) {  txt += x + "   "; }
        int result = QMessageBox::information(this,
                                              "Cloning the spectra",
                                              txt.c_str(),
                                              "Yes",
                                              "No",
                                              "Cancel");
        if(result != 0)  // QMessageBox::Yes
        {
            return ;
        }
#if 1
        bool flag_pattern2_in_use = (file_contents_skeleton.find("%2") != string::npos);
        int file_nr = 0 ;
        for(unsigned int d = 0 ; d < chain_one.size() ; d++)
            for(unsigned int k= 0 ; k < chain_two.size() ; k++)
            {
                // if(d == 0 && k == 1) continue; // to skip the first one - which is a wzor

                // changing the contents ---------------


                bool flag_any_change2 = false;
                string result_bw;

                //                cout << "chain one size =" << chain_one.size();
                //                cout << ", chain two size =" << chain_two.size();
                //                cout << ", d = " << d ;
                //                cout << ", k = " << k  << endl;


                //                cout << "cloning skeleton for arguments: [" << chain_one[d] << "] [" << chain_two[k]
                //                     << "], pattern2 = [" << pattern2 << "]"<< endl;

                if(  (k > 0 && !flag_pattern2_in_use) ) continue;


                string result2 =
                        dlg.make_a_clone_from_skeleton_using_kombination(file_contents_skeleton,
                                                                         chain_one[d],
                                                                         chain_two[k],
                                                                         &result_bw,
                                                                         &flag_any_change2);


                if(!flag_any_change2) continue;
                //cout << "result: " << result << endl;



                // ------------------------------------

                // save under a proper name --------------------------

                string new_filename = filenames[file_nr++]; // specname;
                //cout << "New file " << nr << ")  = " << new_filename << endl;
                // adding the path --------------
                new_filename =  gpath.user_def_spectra + new_filename +  user_spec_ext;

                // checking if it exists
                static bool make_checking_if_clone_exists = true;
                if(make_checking_if_clone_exists)
                {
                    ifstream plik_exists(new_filename.c_str());
                    if(plik_exists)
                    {
                        int odp =  QMessageBox::question(this,
                                                         "Overwrite ?",
                                                         string("Spectrum called \n" +
                                                                new_filename +
                                                                "\nalready exist. \n Overwite it?").c_str(),
                                                         QMessageBox::Yes,
                                                         QMessageBox::YesAll,
                                                         QMessageBox::No);
                        switch(odp)
                        {
                        default:
                        case QMessageBox::No: continue; // break;
                        case QMessageBox::Yes: break;
                        case QMessageBox::YesAll: make_checking_if_clone_exists = false ; break;
                        }
                    } // if exists
                } // if make checking
                cerr << "saving file " << new_filename << endl;

                ofstream plikB(new_filename.c_str());
                if(!plikB)
                {
                    e.message= string ("Can't open the file for writing: ") +  new_filename ;
                    e.title = "Error while opening the file";
                    e.kind = 1; // critial
                    throw e;
                }
                plikB << result_bw;
                if(!plikB)
                {
                    e.message= string ("Can't write the file ") +  new_filename ;
                    e.title = "Error while writing the file";
                    e.kind = 1; // critial
                    throw e;
                }
                plikB.close();
                flag_any_change2 = true;
            } // end for  d (detectors) and for k
#endif

        update_the_table() ;
        if(flag_any_change)appl_form_ptr-> warning_spy_is_in_action();

    }// end of try
    catch(error m)
    {
        QMessageBox::warning(this,
                             m.title.c_str(),
                             m.message.c_str(),
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);
        return;
    }
    raise();   // to the top of desktop
}
//****************************************************************************************************
void T4manager_user_spectra::on_push_log_clicked()
{
    make_report_file();
}
//****************************************************************************************************
void T4manager_user_spectra::on_pushButton_clone_default_clicked()
{
    // find if there is any default spectrum selected
    //	ws
    auto area = appl_form_ptr->give_workspace();

    Tplate_spectrum * dokument = nullptr;
    string sname;
    int dim = 0;

    if ( area->currentSubWindow() )
    {
        dokument  = ( Tplate_spectrum * ) area->currentSubWindow()->widget() ;
        if(dokument != nullptr) { // to prevent segment violation
            sname = dokument->give_spectrum_name();
            dim = dokument->give_dimension() ;
            // cout   << "spectrum =" << sname << " dimension " << dim << endl;
        }
    }
    else {
        // error message, that no spectrum is selected now
        QMessageBox::information( this,
                                  "No spectrum selected",
                                  "No spectrum is currently displayed on screen",
                                  QMessageBox::Ok ,
                                  QMessageBox::NoButton,
                                  QMessageBox::NoButton);
        return; // no selected spectrum
    }

    // if a name starts with 'user',
    if(sname.substr(0,5) == "user_")
    {
        // it is a user def spectrum, not a build-in spectrum
        string mess {"The spectrum which is currently chosen on screen: \n\t"};
        mess += sname;
        mess += "\nis a user-definded spectrum (not a build-in spectrum).\n"
                "For cloning the user-defined spectra, please use a regular cloning procedure (available in the spectrum manager)";

        QMessageBox::information( this,
                                  "Wrong spectrum",
                                  mess.c_str(),
                                  QMessageBox::Ok ,
                                  QMessageBox::NoButton,
                                  QMessageBox::NoButton);

        return;
    }

    // info asking if user want to clone this build-in spectrum
    string mess {"Procedure of creating an user defined spectrum\n"
                 "based on the standard (build-in) spectrum begins. \n"
                 "Now you need to answer some questions\n\n"
                 "Currently, on the screen, there is a selected spectrum called:\n     "};
    mess += sname;
    mess += "\n\nDo you want to create a new user-defined spectrum which is similar to this build-in spectrum?\n";

    auto answ = QMessageBox::information( this,
                                          "Procedure begins... ",
                                          mess.c_str(),
                                          "Yes", "No", "Cancel"
                                          /*"QMessageBox::Yes ,
                                                                                QMessageBox::No,
                                                                                QMessageBox::Cancel*/
                                          );

    if(answ != 0)
        return; // if not

    sname = "user_" + sname;


    // making temporary spectrum with data from the build-in spectrum
    // depending if it is 1D or 2D

    user_spectrum_description spe;
    spe.set_name(sname);
    spe.dimmension = (dim == 1) ?
                user_spectrum_description::spec_1D :
                user_spectrum_description::spec_2D;

    auto specif = dokument->give_specif();
    spe.bins_x = specif.bin;
    spe.beg_x = specif.beg;
    spe.end_x = specif.end;

    spe.bins_y = (int) specif.bin_y;
    spe.beg_y = specif.beg_y;
    spe.end_y  = specif.end_y;

    string zestaw = dokument->give_incrementers();
    // cout << "zestaw inkrementorow = " << zestaw << endl;

    // if 1D all are X incrementers
    string inkr;

    // if there is "X:", skip it
    auto where = zestaw.find("X:");
    if(where != string::npos )
    {
        zestaw = zestaw.substr(where + 2); // 2 characters of "X:"
    }
    else{
        cout << "No 'X:' found in the spectrum desription. It is recomended to correct this" << endl;
    }

    // Note: it is possible, that zestaw contains not a name of incremeter, but some more
    // description (in case of fan spectra).

    string info_explanation;
    istringstream plik (zestaw);
    while(plik >> inkr){
        if(inkr == "Y:") break;

        // perhaps find "INFO:" and skip everything after this
        auto where2 = inkr.find("INFO:");
        if(where2 != string::npos)
        {
            // read all the rest into
            while(getline(plik, info_explanation))
            {
                info_explanation += "\n";
            }
            continue;
        }

        // the incrementer usually has some '_' characters in its name. If it doesnt - skip it
        where2 = inkr.find("_");
        if(where2 == string::npos)
            continue;

        //cout << "x incrementer: " << inkr << endl;

        inkr = correct_to_the_official_incrementer(sname, inkr);
        if(inkr.empty()) continue;
        if(inkr == "nomore") break;

        pair<string, string> entry (inkr, T4user_spectrum_wizard::NoSelfGate);
        spe.incrementors_x.push_back(entry);
    }

    string txt ;
    if(spe.incrementors_x.size())
    { txt +="List of X incrementers:";
        for(auto x : spe.incrementors_x)
        {
            txt += ("\n\t" + x.first);
        }
    }
    else {
        txt += "No X incrementors found in the description of the build-in spectrum";
    }


    // ------------2D spectra ----------------------------------------------------------------------------------
    if(dim == 2)
    {
        while(plik >> inkr)
        {
            if(inkr == "Y:") continue;
            cout << "y incrementer: " << inkr << endl;
            inkr = correct_to_the_official_incrementer(sname, inkr);
            if(inkr.empty()) continue;
            if(inkr == "nomore") break;
            pair<string, string> entry (inkr, T4user_spectrum_wizard::NoSelfGate);
            spe.incrementors_y.push_back(entry);
        }


        if(spe.incrementors_y.size())
        {
            txt += "\n\n\nList of Y incrementers:";
            for(auto y : spe.incrementors_y)
            {
                txt += ("\n\t" + y.first);
            }
        }
        else {
            txt += "No Y incrementors found in the description of the build-in spectrum";
        }

    }

    bool flag_X_incrementers = spe.incrementors_x.size() ? true : false;
    bool flag_Y_incrementers = spe.incrementors_y.size() ? true : false;
    vector <string> buttons_txt;

    if(!flag_X_incrementers && !flag_Y_incrementers)
    {
        buttons_txt.push_back("Ok, I will set right incrementers manually, during an editing process");
    }
    else if (flag_X_incrementers && !flag_Y_incrementers)
    {
        buttons_txt.push_back("Accept  them");
        buttons_txt.push_back("Do not use any, I will do it manually");
    }
    //	else if (!flag_X_incrementers && flag_Y_incrementers )    // means 2D
    //	{
    //		buttons_txt.push_back("Accept  them");
    //		buttons_txt.push_back("Do not use any, I will do it manually");
    //		buttons_txt.push_back("Forget X, accept Y incrementers ");
    //		buttons_txt.push_back("Accept only Y incrementers ");
    //	}
    else { // X, Y exits
        buttons_txt.push_back("Accept all these incrementers");
        buttons_txt.push_back("Do not use any, I will do it manually");
        buttons_txt.push_back("Accept X incrementers, reject Y ");
        buttons_txt.push_back("Accept Y incrementers, reject X ");
    }

    txt += 	"\n\nPlease decide what to do";

    QMessageBox  mb(this);
    mb.setText(txt.c_str());

    mb.addButton(buttons_txt[0].c_str(), QMessageBox::YesRole);   // accept all
    if(buttons_txt.size() > 1)
        mb.addButton(buttons_txt[1].c_str(), QMessageBox::NoRole );   // remove all

    if(dim == 2){
        mb.addButton(buttons_txt[2].c_str(), QMessageBox::NoRole );   // remove y, accept X
        mb.addButton(buttons_txt[3].c_str(), QMessageBox::NoRole );
    }
    auto odp = mb.exec();

    switch(odp)
    {
    default:
    case 0: // all
        break;
    case 1: // manually
        spe.incrementors_y.clear();
        spe.incrementors_x.clear();
        break;
    case 2: // only X,
        if(dim == 2) spe.incrementors_y.clear(); // X are accepted, Y -no
        break;
    case 3:
        if(dim == 2) spe.incrementors_x.clear();  // Y accepted, X - no
        break;
    }

    // ============== Check POLICY  ========
    // for collective 'ALL...' incrementers or many incrementers in case of 2D

    if(dim == 2)
    {
        bool flag_some_ALL = false;
        for(auto x : spe.incrementors_x)
            if(x.first.find("ALL_") != string::npos) flag_some_ALL = true;
        for(auto y : spe.incrementors_y)
            if(y.first.find("ALL_") != string::npos) flag_some_ALL = true;


        QMessageBox  mb2(this);
        mb2.setWindowTitle("Which policy");

        auto nr_x = spe.incrementors_x.size();
        auto nr_y = spe.incrementors_y.size();

        // decide what to do if it is more than 1+1 incrementers,
        // or if one of them is "ALL..."
        if(flag_some_ALL || nr_x > 1 || nr_y >1)
        {

            string txt2 ;
            string xi {"X incrementers"};
            string yi {"Y incrementers"};

            ostringstream my_string;
            my_string << "Here are the lists of your incrementers\n"
                 << xi<< '\n';
            //			plik << setfill('-');
            for(uint i = 0 ; i < nr_x ; ++i)
            {
                my_string << "\t" << spe.incrementors_x[i].first << '\n';
            }

            my_string	 << "\n"<< yi << '\n';
            for(uint i = 0 ; i < nr_y ; ++i){

                my_string << "\t" << spe.incrementors_y[i].first << '\n';
            }

            my_string << '\n';

            txt2 = my_string.str();

            txt2 += "\nNOTE:\n";
            if(nr_x > 1) txt2  += "\t X list has more than 1 item\n";
            if(nr_y > 1) txt2  += "\t Y list has more than 1 item\n";
            if(flag_some_ALL) txt2 += "\tSome incrementer is of a type 'ALL.. (which is a list of many incrementors)' \n" ;

            txt2 += "So you must decide which combinations of X Y incrementers are going"
                   "to be used\n"
                   "There are 3 possiblities:\n"
                   "   - Every x-y combination,\n"
                   "   - Only when X & Y incrementers are from different detectors    (usefull in case 'gamma vs gamma` matrix)\n"
                   "   - Only when X & Y incrementers are from a same detector    (usefull in case 'energy vs time' matrix)\n\n";

            mb2.setText(QString(txt2.c_str() ));

            mb2.addButton("Every X-Y combination", QMessageBox::YesRole);
            mb2.addButton("When X & Y are from the different detector", QMessageBox::NoRole );
            mb2.addButton("When X & Y are from the same detector", QMessageBox::NoRole );

            auto response = mb2.exec();
            switch(response){
            default:
            case 0 :spe.policy_when_increm = 0 ;    // 0 = always
                break;
            case 1 :spe.policy_when_increm = 1;    // 1 => only when from differnet detector
                break;
            case 2 :spe.policy_when_increm = 0 ;   // 2 => only when from the same detector
                break;
            }

        }else spe.policy_when_increm = 0 ;    // 0 = always
    }


    QMessageBox::information( this,
                              "Finished the procedure of creating spectrum ",
                              "The procedure of creating a user defined spectrum (which should be like a chosen standard spectrum)"
                              "has just succesfully finished. \n\n"
                              "Now look carefully at this definition and check if this is what you want",

                              QMessageBox::Ok ,
                              QMessageBox::NoButton,
                              QMessageBox::NoButton);

    // Call edit wizard for this spectrum -----------------------------------------
    //  -- opening the editor
    T4user_spectrum_wizard * dlg = new T4user_spectrum_wizard ;
    dlg->set_parameters(& spe, true) ; // true = editing, cloning is allowed

    if ( dlg->exec() == QDialog::Accepted )
    {
        spe.name_of_spectrum
                = is_name_unique(spe.name_of_spectrum, 9999);

        spe.write_definitions(gpath.user_def_spectra);
        update_the_table() ;
        appl_form_ptr-> warning_spy_is_in_action();
    }
    raise();   // to the top of desktop
}
//***********************************************************************************
string T4manager_user_spectra::correct_to_the_official_incrementer(string specname, string name)
{
    T4incrementer_selector * dlg = new T4incrementer_selector ;

    dlg->set_filter(name);
    auto list = dlg->give_filtered_items();
    delete dlg;


    if(list.size() == 1)
    {
        //		cout << "Good, this iIncrementer was found as official" << endl;
        return name;
    }

    QMessageBox  mb(this);
    mb.setWindowTitle("Incrementer with this name does not exist");
    mb.setText(
                QString(
                    "System build-in spectrum: \n\t%1\n"
                    "informs that it is using incremeter called\n\t%2\n"
                    "However, there is no official inrementer with this name.\n\n"
                    "You must decide what to do: \n"
                    "\t - you may skip this illegal incrementer\n"
                    "or\n"
                    "\t - you may choose some other incrementer from the official list\n\n").arg(specname.c_str()).arg(name.c_str())
                );

    mb.addButton("Choose another from the list", QMessageBox::YesRole);
    mb.addButton("Skip it", QMessageBox::NoRole );
    mb.addButton("Skip it, and all next", QMessageBox::NoRole );

    auto odp = mb.exec();
    if(odp == 1) return ""; // skipping
    if(odp == 2) return "nomore"; // skip and quit

    dlg->set_multi_selection(false);
    dlg->set_filter("");

    if(dlg->exec()   == QDialog::Accepted )
    {
        QStringList lista = dlg->give_filtered_items();
        if(lista.count() == 1){
            auto it = lista.begin();
            name = (*it).toStdString();
            return name;
        }
        return "";
    }
    else
        return "";

}
//***************************************************************************************************
