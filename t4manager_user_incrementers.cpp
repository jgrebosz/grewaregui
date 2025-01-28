#include "t4manager_user_incrementers.h"
#include "ui_t4manager_user_incrementers.h"
#include <iostream>
using namespace std;
#include "Tincrementer_description_cracow.h"
#include <qpainter.h>
#include <qcursor.h>
#include <qapplication.h>
#include <sstream>
#include <string>
#include <QMessageBox>
#include  "paths.h"
#include "t4user_incrementer_wizard.h"
#include <QProgressDialog>
#include <QDir>

#include "appl_form.h"
extern appl_form  *appl_form_ptr;

//#define COTO  // cerr << __FILE__ << ", line " << __LINE__ << endl;
//*************************************************************************************
T4manager_user_incrementers::T4manager_user_incrementers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4manager_user_incrementers)
{
    ui->setupUi(this);
    init();
}
//***********************************************************************
void T4manager_user_incrementers::init()
{

    // bool      flag_long_gate_names = false ;

    int     nr_rows = 3;
    ui->table->setRowCount( nr_rows );
    ui->table->setColumnCount( 1);

    QStringList labels;
    labels << "Name of the incrementer" ; // << "  who is using it   " << " -              - " ;
    ui->table->setHorizontalHeaderLabels ( labels );
    ui->table->resizeColumnsToContents ();
    // column 0 is editable, but others - no:
    ui->table->verticalHeader()->setStyleSheet( "background-color: rgb(255, 239, 222, 255);");

    update_the_table() ;

    ui->table->resizeColumnsToContents ();
    ui->push_A_1_clone->hide();

    //    statistics_timer = new QTimer( this );
    //    connect( statistics_timer, SIGNAL(timeout()), this, SLOT(check_stitistics()) );
    //    statistics_timer->start(1*1000) ;  // 1000 is 1 second

}


//*************************************************************************************
T4manager_user_incrementers::~T4manager_user_incrementers()
{
    //    statistics_timer->stop( ) ;  // 1000 is 1 second
    //    delete  statistics_timer ;
    delete ui;
}
//*************************************************************************************
void T4manager_user_incrementers::add_new()
{

    T4user_incrementer_wizard dlg ;

    Tincrementer_description_cracow  increm2;
    //increm.set_name(res) ;
    //cout << "We are going yto edit >" << res << "<" << endl;

    dlg.set_parameters(&increm2, "", "", "") ;
    if ( dlg.exec() == QDialog::Accepted )
    {
        increm2.save_incrementer();
        update_the_table();
        appl_form_ptr-> warning_spy_is_in_action();

    }
    raise();   // to the top of desktop
}
//***********************************************************************
//***********************************************************************
void T4manager_user_incrementers::edit_existing()
{
    int nr = ui->table->currentRow() ;
    if(nr >= ui->table->rowCount() || nr == -1)
    {
        showWarningMessage(
                             "No incrementer selected",
                             QString(nr == -1 ?
                                         "Nothing to edit" :
                                         "Select the incrementer by clicking once on its name\n"));
        return ;
    }
    row_is_selected();
    //incr_spec_wizard * dlg = new incr_spec_wizard ;

    string name_incr = ui->table->item(nr, 0)->text().toStdString();


    // user selected an item and pressed OK

    T4user_incrementer_wizard dlg;

    Tincrementer_description_cracow  increm3;
    increm3.set_name(name_incr) ;
    //cout << "We are going yto edit >" << name_incr << "<" << endl;
    increm3.read_incrementer();



    dlg.set_parameters(&increm3,
                       names_of_spectra_which_use(name_incr),
                       names_of_conditions_which_use(name_incr),
                       names_of_other_incrementers_which_use(name_incr)
                       ) ;
    if ( dlg.exec() == QDialog::Accepted )
    {
        increm3.save_incrementer();
        update_the_table() ;
        appl_form_ptr-> warning_spy_is_in_action();

    }
    raise();   // to the top of desktop
}
//***********************************************************************
void T4manager_user_incrementers::update_the_table()
{
    // As sometimes this take very long - we show progress dialog
    int how_many_items = 100;
    QString infor("Some disks (directories) can be very slow!\nReading the directory: ");
    infor += gpath.Quser_def_spectra() + "..." ;


    //----------------------

    vec_incrementer_descr.clear() ;
    // ======================================================
    // read the spectra from the disk and load them to the vector
    //   vector<user_spectra> vec_incrementer_descr

    // taking the spectra names from the directory - only

    QDir katalog2(gpath.Qincrementers() );
    QString filtr = ui->ComboBox_filter->currentText ();

    QString name_filter = "*" + filtr + "*.incr";
    how_many_items = katalog2.count() ;


    QProgressDialog progress(infor,
                             "Abort reading", 0, how_many_items, this);
    //                            "progress", true );
    progress.setMinimumDuration (5000) ; // miliseconds
    progress.setValue(2);
    //  qApp->processEvents();

    // in the polygon dir
    QStringList lista = katalog2.entryList (QStringList( name_filter)) ;

    int nr = 0 ;
    for ( QStringList::Iterator it = lista.begin() ; it != lista.end() ;  ++it, nr++ )
    {
        if(! (nr % 10))
        {
            //     cout << "nr = " << nr << endl;
            progress.setValue(nr );
            qApp->processEvents();
        }
        if ( progress.wasCanceled() )   break;



        Tincrementer_description_cracow us ;

        //cout << "Trying to read incrementer definition " << *it << endl;

        string name_c = (*it).toStdString();

        int pos = name_c.rfind(".") ;
        name_c.erase(pos) ;

        us.set_name(name_c);
        us.read_incrementer ();
        vec_incrementer_descr.push_back(us) ;

        //
        // combo->insertItem(short_long_name(*it));
    }
    progress.setValue( how_many_items );


    int nr_rows =vec_incrementer_descr.size();
    ui->table->setRowCount(nr_rows );

    for (int j = 0; j < nr_rows; ++j )
    {
        ui->table->setItem(j, 0, new QTableWidgetItem( vec_incrementer_descr[j].give_name().c_str()));
        ui->table->item(j, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }

    ui->table->resizeColumnsToContents();


    //-------------------------
    update_review();
}
//*********************************************************************
void T4manager_user_incrementers::table_changed( int /*row*/, int /*col*/ )
{
    //  //cout << "Table value changed row = " << row << ", col " << col << endl;
    //  bool stan  ;
    //  if(col == 0)
    //  {
    //    QCheckTableItem *ptr = dynamic_cast<QCheckTableItem*> (ui->table->item( row, 0));
    //    stan = ptr->isChecked();
    //    ui->table->item( row, 1)->setEnabled(stan) ;   // name
    //    ui->table->item( row, 2)->setEnabled(stan) ;  // type
    //    ui->table->item( row, 3)->setEnabled(stan) ;  // incrementer
    ////    vec_incrementer_descr[row].enabled = stan ;
    ////    vec_incrementer_descr[row].write_definitions(gpath.user_def_spectra) ;
    //  }

}
//**********************************************************************

//********************************************************************************
string T4manager_user_incrementers::is_name_unique( string n, int nr )
{
    // perhaps such a cloned incrementer already exists ?
    bool changed = false ;
    string new_name = n ;
    for(unsigned i = 0 ; i < vec_incrementer_descr.size() ; i++)
    {
        if(new_name == vec_incrementer_descr[i].give_name()
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
       showWarningMessage(
                             "Duplicate name of the incrementer",
                             QString("The name of the incrementer:\n\t%1\n"
                                     "was already existing in the list, so it had to be changed into:\n"
                                     "\t%2\n").arg(n.c_str())
                             .arg(new_name.c_str()) );
    }
    return new_name ;
}
//******************************************************************
void T4manager_user_incrementers::row_is_selected()
{
    int row_nr = ui->table->currentRow() ;
    ui->table->selectRow (row_nr ) ;
}
//******************************************************************
void T4manager_user_incrementers::check_stitistics()
{
#if 0
    int nr_rows =vec_incrementer_descr.size();
    ui->table->setRowCount( nr_rows );

    for (int j = 0; j < nr_rows; ++j )
    {
        ui->table->setItem(j, 0, new QTableWidgetItem(vec_incrementer_descr[j].give_name().c_str()) );
        ui->table->item(j, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    }
    ui->label_statistics->setText("Statistics report not found");
    string info, cname, tested, was_true ;
    ifstream plik((gpath.incrementers + "report.txt").c_str() );

    if(plik)
    {
        getline(plik, info);
        ui->label_statistics->setText(info.c_str());
    }

    while(plik)
    {
        plik >> cname >> tested >> was_true ;
        if(!plik)
            break ;
        for (int j = 0; j < nr_rows; ++j )
        {
            if(cname == ui->table->item(j, 0)->text().toStdString() )
            {
                //                ui->table->item(j, 1)->setText(tested.c_str()) ;
                //                ui->table->item(j, 2)->setText(was_true.c_str()) ;
            }
        }
    }
    statistics_timer->start(1*1000) ;  // 1000 is 1 second
#endif
}

//*************************************************************************************
void T4manager_user_incrementers::update_review()
{
    QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );

    ui->review->setRowCount(0);
    unsigned int nr_rows =vec_incrementer_descr.size();
    ui->review->setRowCount ( nr_rows );

    unsigned int current_line = 0 ;
    for (unsigned int j = 0; j < nr_rows; ++j, current_line++ )
    {
        //        COTO ;
        //    ui->review->insertRow(current_line);



        if(ui->review->rowCount() <= (int)current_line)
        {
            ui->review->setRowCount(current_line+1);
        }

        for(int col = 0; col < ui->review->columnCount() ; col++)
        {
            if(ui->review->item(current_line, col)== NULL )
            {
                ui->review->setItem(current_line, col, new QTableWidgetItem("",(Qt::ItemIsEnabled | Qt::ItemIsSelectable)) );
                ui->review->item(current_line, col)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

            }
        }

        ui->review->item(current_line, 0)->setText(vec_incrementer_descr[j].name.c_str());

        // expression A  (GREEN) #############################################################################
        if(vec_incrementer_descr[j].flag_exprA_is_constant)
        {
            //string result;
            ostringstream s;
            s  << vec_incrementer_descr[j].constant_valueA;
            ui->review->setItem(current_line, 1, new QTableWidgetItem( s.str().c_str() ));
            ui->review->item(current_line, 1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }
        else
        {
            ui->review->setItem(current_line, 1, new QTableWidgetItem(vec_incrementer_descr[j].other_incrementer_nameA.c_str()));
            ui->review->item(current_line, 1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }

        // operator ##########################################################################################
        string oper;
        switch(vec_incrementer_descr[j].operation)
        {
        case 1: oper = " + "; break;
        case 2: oper = " - "; break;
        case 3: oper = " * "; break;
        case 4: oper = " / "; break;
        case 5: oper = " mean value "; break;

        default: oper = " unrecognised "; break;
        }
        ui->review->setItem(current_line, 2, new QTableWidgetItem( oper.c_str() ) );
        ui->review->item(current_line, 2)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        // expression B  (blue) #############################################################################
        if(vec_incrementer_descr[j].flag_exprB_is_constant)
        {
            //string result;
            ostringstream s;
            s  << vec_incrementer_descr[j].constant_valueB;
            ui->review->setItem(current_line, 3, new QTableWidgetItem( s.str().c_str() ));
            ui->review->item(current_line, 3)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }
        else
        {
            ui->review->setItem(current_line, 3, new QTableWidgetItem(vec_incrementer_descr[j].other_incrementer_nameB.c_str()));
            ui->review->item(current_line, 3)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }
    }
    ui->review->setRowCount(current_line);


    //------------------------------------------

    ui->review->resizeColumnsToContents();
    QApplication::restoreOverrideCursor();
}
//*************************************************************************************
void T4manager_user_incrementers::make_report_file()
{
#ifdef NIGDY
    ofstream plik((gpath.incrementers + "report_incrementers.txt").c_str() );

    int nr_rows = ui->review->numRows ();

    for (int j = 0; j < nr_rows; j++ )
    {
        for(int k = 0 ; k < 19; k++)
        {
            //string co = ui->review->text(j, k).ascii() ;
            if(ui->review->text(j, k).isNull() )
                plik << "    " << " ; " ;
            else
                plik << ui->review->text(j, k).ascii() << " ; " ;

        }
        plik << "\n";

    }

    plik.close() ;

    QString wiad ;
    if(plik)
    {
        wiad = QString("Report succesfully written as \n") +
                (gpath.incrementers+ "report_incrementers.txt").c_str() ;
        QMessageBox::information(this,
                                 "Report about user defined incrementers",
                                 wiad,
                                 QMessageBox::Ok,
                                 QMessageBox::NoButton,
                                 QMessageBox::NoButton);
    }
    else
    {
        wiad = QString("Error while writing the report \n") +
                (gpath.incrementers+ "report_incrementers.txt").c_str() ;

        QMessageBox::critical(this,
                              "Report about user defined  incrementers",
                              wiad,
                              QMessageBox::Ok,
                              QMessageBox::NoButton,
                              QMessageBox::NoButton);
    }
    return ;
#endif
}
//*************************************************************************************
//*********************************************************************************************
void T4manager_user_incrementers::update_review2()
{
    update_review() ;
}
//**********************************************************************************************
bool T4manager_user_incrementers::anybody_uses_this_incrementer()
{
    // 1. Look into the user def SPECTRA directory, open every spectrum
    // and read the keyword incrementer_name
    // if this the name of this incriton, put it on the list

    // 2. Other incritons can use this incrementer
    return true;

}
//*******************************************************************************
string T4manager_user_incrementers::names_of_spectra_which_use(string incrementer_name)
{
    string list_of_spectra_which_uses;

    //    cout << "Checking in the user def SPECTRA --------------------------" << endl;

    // taking the spectra names from the directory - only
    QDir katalog2(gpath.Quser_def_spectra() );
    QString name_filter = user_spec_filter.c_str() ;

    // in the user def dir
    QStringList lista = katalog2.entryList (QStringList(name_filter)) ;
    //    cout << "wielkosc listy = " << lista.count() << endl;
    int nr = 0 ;
    for ( QStringList::Iterator it = lista.begin() ; it != lista.end() ;  ++it, nr++ )
    {
        string name_c = gpath.user_def_spectra + (*it).toStdString();
        //        cout << "Trying to read spectrum  definition " << name_c << endl;

        // Opening the definition and checking  if there is a banana name here
        ifstream plik(name_c.c_str());
        if(!plik)
        {
            cout << "Error while opening spectrum definition " << name_c << endl;
            list_of_spectra_which_uses += "perhaps " + (*it).toStdString() + ",    " ;
            continue ; // error while opening
        }
        //cout << "Searching the name " <<   incrementer_name   << " iniside this spectrum " << name_c << endl;

        string word;
        while(!plik.eof() )
        {
            plik >> word ;
            if(!plik)
                break;

            if(word == incrementer_name)
            {
                // this user spectrum def. is using the given incementer, so can't be erased
                list_of_spectra_which_uses += (*it).toStdString() + ",    " ;
                //                cout << "Found the name " <<   incrementer_name   << " iniside this spectrum " << name_c << endl;
                break;
            }
        } // end of while reading one incrementer file

    } // end of reading all incritonis

    return list_of_spectra_which_uses;
}
//**********************************************************************************************************
string T4manager_user_incrementers::names_of_conditions_which_use(string incrementer_name)
{
    string list_of_conditions_which_use;

    //    cout << "Checking in the user def conditions --------------------------" << endl;
    // taking the spectra names from the directory - only
    QDir katalog2(gpath.Qconditions() );
    QString name_filter = user_cond_filter.c_str();

    // in the user def dir
    QStringList lista = katalog2.entryList (QStringList(name_filter)) ;
    int nr = 0 ;
    for ( QStringList::Iterator it = lista.begin() ; it != lista.end() ;  ++it, nr++ )
    {
        // cout << "Trying to read condition  definition " << *it << endl;
        string name_c = gpath.conditions + (*it).toStdString();

        // Opening the definition and checking  if there is a incrementer name in here
        ifstream plik(name_c.c_str());
        if(!plik)
        {
            cout << "Error while opening condition definition " << name_c << endl;
            list_of_conditions_which_use += "perhaps " + (*it).toStdString() + ",    " ;
            continue ; // error while opening
        }
        //        cout << "Searching the name " <<   incrementer_name   << " inside the condition: " << name_c << endl;

        string word;
        while(!plik.eof() )
        {
            plik >> word ;
            if(!plik)
                break;

            if(word == incrementer_name)
            {
                // this condition def. is using the given incementer, so can't be erased
                list_of_conditions_which_use += (*it).toStdString() + ",    " ;
                //                cout << "Found the name " <<   incrementer_name   << " used iniside the condition" << name_c << endl;
            }
        } // end of while reading one incrementer file

    } // end of reading all conditions
    return list_of_conditions_which_use;
}
//**********************************************************************************************************
string T4manager_user_incrementers::names_of_other_incrementers_which_use(string incrementer_name)
{
    string list_of_incrementers_which_uses;  // which are using this incrementer

    string his_filename = incrementer_name;  // to avoid looking into his own definition file

    // remove extension
    auto pos = incrementer_name.find_last_of(".");
    if(pos != string::npos)
    {
        incrementer_name = incrementer_name.substr(0, pos);

    }




    // taking the spectra names from the directory - only
    QDir katalog2(gpath.Qincrementers() );
    QString name_filter = user_incr_filter.c_str();
    // in the incrementer dir
    QStringList lista = katalog2.entryList (QStringList(name_filter) ) ;
    int nr = 0 ;
    for ( QStringList::Iterator it = lista.begin() ; it != lista.end() ;  ++it, nr++ )
    {
        //cout << "Trying to read incrementer definition " << (*it).toStdString() << endl;
        string name_c = gpath.incrementers + (*it).toStdString();



        auto pos2 = name_c.find_last_of("/");
        string name_c_without_path = name_c.substr(pos2+1);


        if(name_c_without_path == his_filename) continue;  // to avoid looking into his own definition file

        // Opening the definition and checking  if there is a incrementer name here
        ifstream plik(name_c.c_str());
        if(!plik)
        {
            cout << "Error while opening incrementer definition " << name_c << endl;
            list_of_incrementers_which_uses += "perhaps " + (*it).toStdString() + ",    " ;
            continue ; // error while opening
        }

        string word;
        while(!plik.eof() )
        {

            plik >> word ;
            if(!plik)
                break;
            //                cout << "Checking file word " << word << " and comparing with " << incrementer_name << endl;
            if(word == incrementer_name)
            {
                // this incrementer is using the given incrementer, so can't be erased
                //                ostringstream numerek ;
                //                numerek << nr +1 ;
                list_of_incrementers_which_uses +=
                        // numerek.str() + (") ") +
                        (*it).toStdString() + ",    " ;
                //                    cout << "Found the name " <<  incrementer_name
                //                         << " inside this incrementer " << name_c << endl;
            }
        } // end of while reading one incrementer file

    } // end of reading all incritonis
    return list_of_incrementers_which_uses ;
}
//**********************************************************************************************************
bool T4manager_user_incrementers::is_possible_to_remove_this_incrementer( string incrementer_name )
{
    /*loop over all
     *user def spectra available on the disk
     *user conditons
     *other user def incrementers
     *to check  if  any of them is using a incrementer with this name
  */


    //    cout << " F. incrementer_manager::" << __func__ <<  "( " << incrementer_name << ")" << endl;

    bool possible = true;


    string list_of_spectra_which_uses = names_of_spectra_which_use(incrementer_name);
    if(list_of_spectra_which_uses.size() )
    {
        possible = false;

        string mmm ;
        mmm += "The incrementer named\n";
        mmm +=  incrementer_name;
        mmm += " \ncan not be removed , because it is applied to the \nuser defined spectra:\n\n";
        mmm += list_of_spectra_which_uses;
        mmm += "\n\n(You can delete a user_defined spectrum - using the the User Defined Spectrum Manager)";

        showWarningMessage("Impossible to delete the incrementer",
                               mmm.c_str(),
                               QMessageBox::Critical);  // error

    }// end if impossible

    //========================================================
    // In the conditions


    string list_of_conditions_which_use = names_of_conditions_which_use(incrementer_name);
    if(list_of_conditions_which_use.size() )
    {
        possible = false;
        // removing from the disk file
        string mmm ;
        mmm += "The incrementer named\n";
        mmm +=  incrementer_name;
        mmm += " \ncan not be removed , because it is applied in the \nuser conditions:\n\n";
        mmm += list_of_conditions_which_use;
        mmm += "\n\n(You can delete or modify a condition - using the the User Defined Condition Manager)";

        showWarningMessage("Impossible to delete the incrementer",
                               mmm.c_str(),
                               QMessageBox::Critical);  // error

    }// end if impossible



    // =======================================================
    // the incrementer can be also used by other incrementers, so we need to check also this.


    string list_of_incrementers_which_uses =  // which are using this incrementer
            names_of_other_incrementers_which_use(incrementer_name);
    if(list_of_incrementers_which_uses.size() )
    {
        possible = false;
        // removing from the disk file
        string mmm ;
        mmm += "The incrementer named\n";
        mmm +=  incrementer_name;
        mmm += " \ncan not be removed , because it us used by some \nother incrementer(s):\n\n";
        mmm += list_of_incrementers_which_uses;
        mmm += "\n\n(You must change this first )";

        showWarningMessage( "Impossible to delete the incrementer",
                               mmm.c_str(),
                               QMessageBox::Critical);  // error

    }// end if impossible

    return possible;

}
//********************************************************************************************
void T4manager_user_incrementers::filtered_condtions_only()
{
    /* QString filtr = ui->ComboBox_filter->currentText ();

   ListBox_available->clear();

   QStringList found = all_spectra_names.grep(QRegExp(filtr, true,true) );

   ListBox_available->insertStringList(found) ;
   */
}
//********************************************************************************************************
void T4manager_user_incrementers::cloning_A_1_()
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
    bool flag_any_changes = false;

    try
    {
        int nr = ui->table->currentRow() ;
        if(nr >= ui->table->rowCount() || nr == -1)
        {
            e.title = "No incrementer selected";
            e.message = nr == -1 ? "Nothing to edit" : "Select the incrementer by clicking once on its name\n";
            e.kind = 2; //warning;
            throw e;

            /*    QMessageBox::warning(this,
                               "No spectrum selected",
                               QString(nr == -1 ? "Nothing to edit" : "Select the spectrum by clicking once on its name\n"),
                               QMessageBox::Ok,
                               QMessageBox::NoButton,
                               QMessageBox::NoButton);
          return ; */

        }
        row_is_selected();

        // checking if the name of the spectrum contains the substring A_1
        string pattern = "_A_1_";
        string incrname = vec_incrementer_descr[nr].name ;
        string::size_type loc = incrname.find( pattern);
        if( loc == string::npos )
        {
            e.title = "Wrong name of the incrementer ";
            e.message = "The name of the selected incrementer does not contain the substring \"_A_1_\" ";
            e.kind = 1; // critial
            throw e;

            /*    QMessageBox::critical(this,
                                "Wrong name of the spectrum",
                                "The name of the selected spectrum does not contain the substring \"_A_1_\" "
                                ,
                                QMessageBox::Ok,
                                QMessageBox::NoButton,
                                QMessageBox::NoButton);
          return ;
      */
        }

        auto  result = askYesNoCancel(
                                              "Cloning the _A_1_ incrementer ",
                                              "This is the option to clone a incrementer  which contains the substring \"_A_1_\"\n "
                                              "Such substring will be replaced with _A_2_ , _A_3_, ... , _R_7_"
                                              "(i.e. for all possible cluster crystals)\n"
                                              "By this action, the new 104 incrementers  wiil be created.\n\n"
                                              "The replacement will be done:\n"
                                              "    1. Inside  the name of the incrementer, \n"
                                              "and everywhere inside the definiotion of this incrementer:\n"
                                              "    2. Inside the name of any incrementer (variable) ,\n"
                                              "    3. Inside the name of any other incrementer refered in this incrementer.\n"

                                              "\n\nDo you really want to create such set of 104 clones ? ");
        if(result != QMessageBox::Yes )
        {
            return ;
        }


        //
        //cout << "OK, we can clone it !" << endl;

        //  reading the whole contents of the file
        string pathed_name = gpath.incrementers + incrname +user_incr_ext;

        ifstream plik(pathed_name.c_str() );
        if(!plik)
        {
            string m = "Error while opening the file :";
            m += pathed_name;

            e.title = "Can't open a file ";
            e.message = m ;
            e.kind = 1; // critial
            throw e;
        }
        // read in the whole contents
        string contents;
        string one_line;
        while(plik)
        {
            getline(plik, one_line);  // delimiter (\n) is not added to the string
            // so we add it
            contents += one_line;
            contents += '\n';
        }

        //  search all the positions of the substrings A_0
        vector<string::size_type> position;

        for(string::size_type i = 0 ; ; i++)
        {
            loc = contents.find(pattern, i);
            if( loc == string::npos )
                break;
            position.push_back(loc);
            i = loc;
        }
        if(!position.size())
        {
            cout << "No " << pattern << " found inside the file " <<  incrname << endl;

            e.title = "Some nonsense ? ";
            e.message = "The  cloned incrementer  definition does not use anything with the substring \"_A_0_\" "
                        "\nPerhaps you are doing some nonsense...";
            e.kind = 2; // critial
            throw e;

        }
        // FOR loop over all crystals========================================
        bool make_checking_if_clone_exists = true;

        //----------------------
        string markers =
                "ABCDEFGJKLMNPQR"  ; // : "ABCDGJKL" ;

        string mess = "Which set of detectors do you have?";
        int  odp = askQuestionWithButtons( "Set of Detectors", mess.c_str(),            //+
                                            "ABCDEFGJKLMNPQR",
                                            "ABCDGJKL",
                                            "Cancel",
                                            1 );

        switch(odp)
        {
        case 1:
        default:
            break;

        case 2:
            markers = "ABCDGJKL" ;
            break;

        case 3:
            raise();
            return;
            break;
        }


        //----------------------



        for(unsigned int d = 0 ; d < markers.size() ; d++)
            for(unsigned int k= 1 ; k<= 7 ; k++)
            {
                if(d == 0 && k == 1) continue;

                //      cout << "d = " << d << " , k = " << k << endl;
                ostringstream s;
                s << "_" << markers[d] << "_" << k << "_" ;
                string new_pattern = s.str();
                // For loop over all patterns inside
                string output_contents = contents;
                for(unsigned int i = 0 ; i < position.size() ; i++)
                {
                    output_contents.replace(position[i],  new_pattern.size(), new_pattern);
                }
                // save -------------- -----------------------------
                string new_filename = incrname;
                // change all the patterns in the new filename

                do
                {
                    loc = new_filename.find(pattern);
                    if(loc != string::npos)
                    {
                        new_filename.replace(loc,  new_pattern.size(), new_pattern);
                    }
                    //      cout << "Currently new_filename = " << new_filename << endl;
                }
                while(loc != string::npos);
                //         cout << "New file = " << new_filename << endl;
                new_filename =  gpath.incrementers + new_filename +  ".incr";

                // checking if it exists
                if(make_checking_if_clone_exists)
                {
                    ifstream plik_exists(new_filename.c_str());
                    if(plik_exists)
                    {
                        string pyt = "Condition named  \n" + new_filename + "\nalready exist. \n Overwite it?";
                        int odp2 = askQuestionWithButtons(      // +
                                                          "Overwrite ?", pyt.c_str(),
                                                          "Yes",
                                                          "Yes to all",
                                                          "No", 3);
                        switch(odp2)
                        {
                        case 1:  break;   // QMessageBox::Yes:
                        case 2: make_checking_if_clone_exists = false ; break; // QMessageBox::YesAll:

                        default:
                        case 3: continue; break; // QMessageBox::No:
                        }
                    } // if exists
                } // if make checking

                ofstream plikB(new_filename.c_str());
                if(!plikB)
                {
                    e.message= string ("Can't open the file for writing: ") +  new_filename ;
                    e.title = "Error while opening the file";
                    e.kind = 1; // critial
                    throw e;
                }
                plikB << output_contents ;
                if(!plikB)
                {
                    e.message= string ("Can't write the file ") +  new_filename ;
                    e.title = "Error while writing the file";
                    e.kind = 1; // critial
                    throw e;
                }
                plikB.close();
                flag_any_changes = true;
            } // end for  d (detectors) and for k

        update_the_table() ;

    }// end of try
    catch(error m)
    {
        showWarningMessage(
                             m.title.c_str() ,
                             m.message.c_str());
        return;
    }
    if(flag_any_changes)appl_form_ptr-> warning_spy_is_in_action();
    raise();   // to the top of desktop
}
//***************************************************************************************
void T4manager_user_incrementers::on_push_create_new_clicked()
{
    add_new();
}
//***************************************************************************************
void T4manager_user_incrementers::on_push_edit_selected_clicked()
{
    edit_existing();
}
//***************************************************************************************
void T4manager_user_incrementers::on_table_cellDoubleClicked(int /*row*/, int /*column*/ )
{
    edit_existing();
}
//***************************************************************************************
void T4manager_user_incrementers::on_review_cellDoubleClicked(int row, int /*column*/ )
{
    //int row = ui->review->currentRow() ;
    if(row >= ui->review->rowCount())
    {
        showWarningMessage(
                             "No incrementer selected",
                             QString("Select the spectrum by clicking once on its name\n") );
        return ;
    }

    cout << "Dbl click on the row " << row << endl;
    string last_legal_name ;
    for(int i = 0 ; i <= row ; i ++)
    {
        string current_name = ui->review->item(i, 0)->text().toStdString();
        cout << "Checking name " << current_name << endl;
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
    for(unsigned i = 0 ; i < vec_incrementer_descr.size() ; i++)
    {
        //    cout << "comparing with " << vec_incrementer_descr[i].name_of_spectrum << endl;
        if(vec_incrementer_descr[i].name ==  last_legal_name)
        {
            which = i ;
            //       cout << "remembered as spectrum nr " << i << endl ;
        }
    }
    if(which == -1)
        return ;

    // row_is_selected();

    // find such a name in the vector

    T4user_incrementer_wizard dlg;


    dlg.set_parameters(& vec_incrementer_descr[which], "", "", "" ) ; // true = editing, cloning is allowed
    if ( dlg.exec() == QDialog::Accepted )
    {
        vec_incrementer_descr[which].name
                = is_name_unique(vec_incrementer_descr[which].name, which);

        //update_the_table() ;
        vec_incrementer_descr[which].save_incrementer();
        update_the_table() ;
        appl_form_ptr-> warning_spy_is_in_action();
    }
    raise();   // to the top of desktop

}
//*******************************************************************************************
void T4manager_user_incrementers::on_push_remove_selected_clicked()
{

    // here should be a loop over many regions of selecion
    // many lines of every selection

    QList<QTableWidgetSelectionRange> range = ui->table->selectedRanges() ;

    //cout << "Selection has " << (range.count()) << " ranges" << endl;


    if(range.count() == 0)
    {
        showWarningMessage(
                             "No condition selected",
                             QString("Select the condition by clicking once on its name\n") );
        return ;
    }


    bool flag_yes_to_all = false;
    bool flag_any_changes = false;

    for(int s = range.count()-1; s >=0 ; s--)    // can be many separated ranges on this list
    {
        //cout << "range[s].bottomRow()= " << range[s].bottomRow() << ", range[s].topRow()= " <<  range[s].topRow() << endl;
        for(int row = range[s].bottomRow() ; row >= range[s].topRow() ; row--)
        {
            //            cout << "remowing row = " << row << " while range[s].topRow() = " << range[s].topRow() << endl;

            //  unsigned row = tab->selectRow();    ro sel currentRow() ;
            if(row >= (signed int) vec_incrementer_descr.size() )
            {
                cout << "STRANGE --> ? " << __FILE__ << ":"<< __LINE__  << endl;
                return ;
            }

            if(is_possible_to_remove_this_incrementer(vec_incrementer_descr[row].name + ".incr") == false)
            {
                cerr << "Impossible to remove incrementer called" << vec_incrementer_descr[row].name  << endl;
                return ;
            }

            int answ = QMessageBox::Cancel;

            if(flag_yes_to_all == false)
            {
                QMessageBox msgBox;
                msgBox.setText("Removing the user incrementer definition");
                msgBox.setInformativeText( QString("You are going to remove the definition nr %1\nof the user incrementer %2\n"
                                                   "Are you sure?").arg(row+1).arg(vec_incrementer_descr[row].name.c_str() ))  ;
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::YesToAll |  QMessageBox::No | QMessageBox::Cancel);
                msgBox.setDefaultButton(QMessageBox::No);
                answ = msgBox.exec();

            }

            if(answ == QMessageBox::YesToAll) flag_yes_to_all = true;

            if(answ == QMessageBox::Yes|| answ == QMessageBox::YesToAll  || flag_yes_to_all )  /*Yes, Yes_to_all */
            {
                // here checking if there is any user def spectrum which uses this condition
                //..........

                // --------------------------------------
                // remove from the directory
                string fname = gpath.incrementers + vec_incrementer_descr[row].name   +".incr";

                //                cout << "Yes, so removing " << fname << ", name taken from the memory, (not from table)" << endl;

                // create 'removed'subdirectrory (if it does not exists) ---------------
                string remdir = gpath.conditions + "removed/" ;

                QDir dir(remdir.c_str() );
                if(dir.exists() == false)
                {
                    dir.mkdir(remdir.c_str());
                }
                else {
                    // cout << "subdir already exists " << endl;     // TODO:
                }

                // --------- now the moving
                string moving_command = "mv " + fname + " ";
                moving_command += remdir + vec_incrementer_descr[row].name + ".incr";
                //cout << "Komenda " << moving_command << endl;
                int result = system(moving_command.c_str());
                if(result) cout << "Problem with moving the definition, so no backup made" << endl;

                // ----- now removing from the memory of the  computer

                // be careful, because if the filter is in the table, then the nr of the row
                // is not the numer of the condition on the list

                //cout << "In the memory this row has " << vec_cond_descr[row].name << endl;

                vec_incrementer_descr.erase(vec_incrementer_descr.begin() + row) ;

                ui->table->item(row, 0)->setSelected(false);
                flag_any_changes = true;

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


            //            cout << "na dole petli row = " << row << " while range[s].topRow() = " << range[s].topRow() << endl;


        } // for row
    } // for sel range
outside:
    if(flag_any_changes)      appl_form_ptr-> warning_spy_is_in_action();
    update_the_table() ;


}
//**************************************************************************************************
void T4manager_user_incrementers::on_push_clone_clicked()
{


    showWarningMessage(
                             "Cloning the definition of the incrementer",
                             QString("If you want to clone some incrementer\n\n"
                                     "   1. Open it for editing (Press button: Edit Selected incrementer)\n\n"
                                     "   2. On the first page of the editing wizard - change the name of the incrementer\n\n\n"
                                     "Note: By this the original incrementer will not be affected, "
                                     "(its definition is already on the disk)\n"
                                     "and the new-named incrementer (clone) will be stored on the disk independently\n") );


}
//*******************************************************************************************
void T4manager_user_incrementers::on_ComboBox_filter_editTextChanged(const QString &  /*arg1*/ )
{
    update_the_table();
}
//*******************************************************************************************
void T4manager_user_incrementers::on_push_log_clicked()
{
    make_report_file();
}
//********************************************************************************************
