#include "t4manager_user_conditions.h"
#include "ui_t4manager_user_conditions.h"


#include <iostream>
using namespace std;
#include "Tcondition_description_cracow.h"
#include "t4user_condition_wizard.h"
//#include <qcursor.h>
//#include <qapplication.h>
#include <sstream>
#include <QMessageBox>
#include <QProgressDialog>
#include <QDir>
#include "t4pattern_defining_dlg.h"

#include "t4user_condition_wizard.h"


//#define COTO  // cerr << __FILE__ << ", line " << __LINE__ << endl;

#include "appl_form.h"
extern appl_form  *appl_form_ptr;

//*********************************************************************************************************************
T4manager_user_conditions::T4manager_user_conditions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4manager_user_conditions)
{
    ui->setupUi(this);

    ui->table->setColumnCount(3);
    QStringList labels;
    labels << "Name of the conditon" << "Tested [times]" << "True [times]" ;
    ui->table->setHorizontalHeaderLabels ( labels );
    ui->table->resizeColumnsToContents ();
    ui->table->setColumnCount(3);

    labels.clear();
    labels << "Condition name"
           << "[1D AND] 'When incr. not valid' "
           << "variables list (AND)"
           << "min"
           << "max"
           << "1D OR variable list"
           << "min"
           << "max"
           << "[2D AND] 'When X or Y incr is not valid'"
           << "variable X"
           << "variable Y"
           << "in polygon"
           << "2D OR variable X"
           << "variable Y"
           << "in polygon"
           << "AND conditions list"
           << "OR conditions list"
           << "NAND conditions list"
           << "NOR conditions list";

    ui->review->setHorizontalHeaderLabels ( labels );
    ui->review->resizeColumnsToContents ();




	// colors of left vertical label
	// does not work properly. Makes it black!
	//ui->table->verticalHeader()->setStyleSheet( "background-color:rgba(155, 144, 71, 30);");


    // from init()-------------------------------

    // bool      flag_long_gate_names = false ;

    int     nr_rows = 0;
    ui->table->setRowCount ( nr_rows );
    ui->table->setColumnCount ( 3);

    // column 0 is editable, but others - no:
		ui->table->setAlternatingRowColors(true);

    update_the_table() ;

    ui->table->resizeColumnsToContents();


    statistics_timer = new QTimer( this );
    connect( statistics_timer, SIGNAL(timeout()), this, SLOT(check_statistics()) );
    statistics_timer->start(1*1000) ;  // 1000 is 1 second

}
//*********************************************************************************************************************
T4manager_user_conditions::~T4manager_user_conditions()
{
    statistics_timer->stop( ) ;  // 1000 is 1 second
    delete  statistics_timer ;

    delete ui;
}
//*********************************************************************************************************************
void T4manager_user_conditions::on_push_create_new_clicked()
{


    T4user_condition_wizard * dlg = new T4user_condition_wizard;

    Tcondition_description_cracow  cnd;
    //cnd.set_name(res) ;
    //cout << "We are going yto edit >" << res << "<" << endl;

    dlg->set_parameters(&cnd) ;
    if ( dlg->exec() == QDialog::Accepted )
    {
        cnd.save_condition();
        update_the_table() ;
        appl_form_ptr-> warning_spy_is_in_action();

    }

    raise();   // to the top of desktop
}
//***********************************************************************
void T4manager_user_conditions::on_push_remove_selected_clicked()
{
    // here should be a loop over many regions of selecion
    // many lines of every selection

    QList<QTableWidgetSelectionRange> range = ui->table->selectedRanges() ;

    //cout << "Selection has " << (range.count()) << " ranges" << endl;


    if(range.count() == 0)
    {
        QMessageBox::warning(this,
                             "No condition selected",
                             QString("Select the condition by clicking once on its name\n"),
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);
        return ;
    }


    bool flag_yes_to_all = false;
    bool flag_any_change = false;

    for(int s = range.count()-1; s >=0 ; s--)    // can be many separated ranges on this list
    {
        //cout << "range[s].bottomRow()= " << range[s].bottomRow() << ", range[s].topRow()= " <<  range[s].topRow() << endl;
        for(int row = range[s].bottomRow() ; row >= range[s].topRow() ; row--)
        {
            //            cout << "remowing row = " << row << " while range[s].topRow() = " << range[s].topRow() << endl;

            //  unsigned row = tab->selectRow();    ro sel currentRow() ;
            if(row >= (signed int) vec_cond_descr.size() )
            {
                cout << "STRANGE --> ? " << __FILE__ << ":"<< __LINE__  << endl;
                return ;
            }

            if(is_possible_to_remove_this_condition(vec_cond_descr[row].name + user_cond_ext) == false)
            {
				cerr << "niemozliwe usuniecie tego warunku" << endl;
                return ;

            }

            int answ = QMessageBox::Cancel;

            if(flag_yes_to_all == false)
            {
                QMessageBox msgBox;
                msgBox.setText("Removing the condition definition");
                msgBox.setInformativeText( QString("You are going to remove the definition nr %1\nof the condition %2\n"
                                                   "Are you sure?").arg(row+1).arg(vec_cond_descr[row].name.c_str() ))  ;
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
                string fname = gpath.conditions + vec_cond_descr[row].name   +user_cond_ext;

                // cout << "Yes, so removing " << fname << ", name taken from the memory, (not from table)" << endl;

                // create 'removed'subdirectrory (if it does not exists) ---------------
                string remdir = gpath.Qconditions().toStdString() + "removed/" ;

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
                moving_command += remdir + vec_cond_descr[row].name + user_cond_ext;
                //cout << "Komenda " << moving_command << endl;
                int result = system(moving_command.c_str());
                if(result) cout << "Problem with moving the definition, so no backup made" << endl;

                // ----- now removing from the memory of the  computer

                // be careful, because if the filter is in the table, then the nr of the row
                // is not the numer of the condition on the list

                //cout << "In the memory this row has " << vec_cond_descr[row].name << endl;

                vec_cond_descr.erase(vec_cond_descr.begin() + row) ;

                ui->table->item(row, 0)->setSelected(false);
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


            //            cout << "na dole petli row = " << row << " while range[s].topRow() = " << range[s].topRow() << endl;


        } // for row
    } // for sel range
outside:
    if(flag_any_change)appl_form_ptr-> warning_spy_is_in_action();
    update_the_table() ;
}
//***********************************************************************
void T4manager_user_conditions::on_push_edit_selected_condition_clicked()
{
    int nr = ui->table->currentRow() ;
    if(nr >= ui->table->rowCount() || nr == -1)
    {
        QMessageBox::warning(this,
                             "No condition selected",
                             QString(nr == -1 ?
										 "No condition was selected to edit" :
                                         "Select the condition by clicking once on its name\n"),
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);
        return ;
    }
    row_is_selected();
    //cond_spec_wizard * dlg = new cond_spec_wizard ;

    string name_cond = ui->table->item(nr, 0)->text().toStdString() ;


    // user selected an item and pressed OK

    T4user_condition_wizard * dlg = new T4user_condition_wizard;

    Tcondition_description_cracow  cnd;
    cnd.set_name(name_cond) ;
    //cout << "We are going yto edit >" << name_cond << "<" << endl;
    cnd.read_condition();

    dlg->set_parameters(&cnd) ;
    if ( dlg->exec() == QDialog::Accepted )
    {
        cnd.save_condition();
        update_the_table() ;
        appl_form_ptr-> warning_spy_is_in_action();

    }
    raise();   // to the top of desktop
}

//***********************************************************************
void T4manager_user_conditions::update_the_table()
{
    // As sometimes this take very long - we show progress dialog
    int how_many_items = 100;
    QString infor("Some disks (directories) can be very slow!\nReading the directory: ");
    infor += gpath.Quser_def_spectra() + "..." ;


    //----------------------

    vec_cond_descr.clear() ;
    // ======================================================
    // read the spectra from the disk and load them to the vector
    //   vector<user_spectra> vec_cond_descr

    // taking the spectra names from the directory - only

    QDir katalog2(gpath.Qconditions() );
    QString filtr = ui->ComboBox_filter->currentText ();
    // perhaps it is just a space???????????
    filtr.remove(" ");

    QString name_filter = "*" + filtr + user_cond_filter.c_str() ;

    how_many_items = katalog2.count() ;

    QProgressDialog progress( infor,
                              "Abort reading", 0, how_many_items,
                              this);
    progress.setMinimumDuration (5000) ; // miliseconds
    progress.setValue(2);
    qApp->processEvents();

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



        Tcondition_description_cracow us ;


        string name_c = (*it).toStdString() ;
//        cout << "Trying to read condition definition " << name_c << endl;

		auto pos = name_c.rfind(".") ;
        name_c.erase(pos) ;

        us.set_name(name_c);
        us.read_condition ();


        vec_cond_descr.push_back(us) ;

        //
        // combo->insertItem(short_long_name(*it));
    }
    progress.setValue( how_many_items );


	auto nr_rows =vec_cond_descr.size();
    ui->table->setRowCount ( nr_rows );

	for (int j = 0; j < (int)nr_rows; ++j )
    {
        ui->table->setItem(j, 0, new QTableWidgetItem(vec_cond_descr[j].give_name().c_str()) )  ;
        ui->table->item(j, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        ui->table->setItem(j, 1, new QTableWidgetItem(""));
        ui->table->item(j, 1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        ui->table->setItem(j, 2, new QTableWidgetItem(""));
        ui->table->item(j, 2)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    }
    ui->table->resizeColumnsToContents();
    //-------------------------
    update_review();
}
////*********************************************************************
//void T4manager_user_conditions::table_changed( int /*row*/, int /*col*/ )
//{
//    //  //cout << "Table value changed row = " << row << ", col " << col << endl;
//    //  bool stan  ;
//    //  if(col == 0)
//    //  {
//    //    QCheckTableItem *ptr = dynamic_cast<QCheckTableItem*> (ui->table->item( row, 0));
//    //    stan = ptr->isChecked();
//    //    ui->table->item( row, 1)->setEnabled(stan) ;   // name
//    //    ui->table->item( row, 2)->setEnabled(stan) ;  // type
//    //    ui->table->item( row, 3)->setEnabled(stan) ;  // condition
//    ////    vec_cond_descr[row].enabled = stan ;
//    ////    vec_cond_descr[row].write_definitions(gpath.user_def_spectra) ;
//    //  }

//}
//**********************************************************************
void T4manager_user_conditions::on_push_How_to_clone_clicked()
{

    QMessageBox::information(this,
                             "Cloning the definition of the condition",
                             QString("If you want to clone some condition\n\n"
                                     "   1. Open it for editing (Press button: Edit Selected condition)\n\n"
                                     "   2. On the first page of the editing wizard - change the name of the condition\n\n\n"
                                     "Note: By this the original condition will not be affected, "
                                     "(its definition is already on the disk)\n"
                                     "and the new-named condition (clone) will be stored on the disk independently\n"),
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);

}
//********************************************************************************
string T4manager_user_conditions::is_name_unique( string n, int nr )
{
    // perhaps such a cloned condition already exists ?
    bool changed = false ;
    string new_name = n ;
    for(unsigned i = 0 ; i < vec_cond_descr.size() ; i++)
    {
        if(new_name == vec_cond_descr[i].give_name()
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
                             "Duplicate name of the condition",
                             QString("The name of the condition:\n\t%1\n"
                                     "was already existing in the list, so it had to be changed into:\n"
                                     "\t%2\n").arg(n.c_str())
                             .arg(new_name.c_str()),
                             QMessageBox::Ok,
                             QMessageBox::NoButton,
                             QMessageBox::NoButton);
    }
    return new_name ;
}
//******************************************************************
void T4manager_user_conditions::row_is_selected()
{
    int row_nr = ui->table->currentRow() ;
    ui->table->selectRow (row_nr ) ;
}
//******************************************************************
void T4manager_user_conditions::check_statistics()
{

    //    int nr_rows =vec_cond_descr.size();
    //ui->table->setRowCount ( nr_rows );
    int nr_rows = ui->table->rowCount ( );
    if( !nr_rows )return ;

    for (int j = 0; j < nr_rows; ++j )
    {
        if(ui->table->item(j, 0) == nullptr)
        {
            cout << "Scandal! nr of rows in the table now (after filtering) = "
                 << ui->table->rowCount() << endl;
            cout << "in function " << __func__
                 << " which should be in row " << j << endl;
            cerr << "while row j=" << j << " does not exist (is nullptr) " << endl;

        }

        if( j >= (int) vec_cond_descr.size() )
        {
            cerr << "In function " << __func__ << ": a row nr " << j
                 << " is too big for the vec_cond_descr which has the size = "
                 << vec_cond_descr.size() << endl;
            int licznik = 0 ;
            for(auto x : vec_cond_descr)
            {
                cout << licznik++ << ") -> " << x.give_name().c_str() << endl;
                ui->table->item(j, 2)->setText( "error prone?");
            }
        }else{

            // XYZ Tutaj j=42 give_name  dalo name niedostepny !!!
            ui->table->item(j, 0)->setText( vec_cond_descr[j].give_name().c_str());
            ui->table->item(j, 1)->setText( "not used now?");
            ui->table->item(j, 2)->setText( "");
        }
    }
    ui->label_statistics->setText("Statistics report not found");
    string info, cname, tested, was_true ;
    ifstream plik((gpath.conditions + "report.txt").c_str() );

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
            if(cname == ui->table->item(j, 0)->text().toStdString())
            {
                ui->table->item(j, 1)->setText( tested.c_str());
                ui->table->item(j, 2)->setText( was_true.c_str());
            }
        }
    }
    statistics_timer->start(1*1000) ;  // 1000 is 1 second
}
//    //*****************************************************************
//    void T4manager_user_conditions::destroy()  // is anybody using this?
//    {
//        statistics_timer->stop( ) ;  // 1000 is 1 second
//        delete  statistics_timer ;
//    }
//*************************************************************************************
void T4manager_user_conditions::update_review()
{

    QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );

    ui->review->setRowCount (0);
    unsigned int nr_rows =vec_cond_descr.size();
    ui->review->setRowCount ( nr_rows );


	//ui->review->setStyleSheet( "background-color: rgb(255, 255, 190);   selection-color: rgb(255, 198, 171);");



    unsigned int current_line = 0 ;
    for (unsigned int j = 0; j < nr_rows; ++j, current_line++ )
    {


        make_new_row_in_review_if_needed(current_line);

        //      cout << "New condition in review table is to be written current_line " << current_line
        //           << " while currently the table has rows = " << ui->review->rowCount()
        //           << endl;

        ui->review->item(current_line, 0)->setText(vec_cond_descr[j].name.c_str());


        //      cout << "==========Review =============preparing condition named "
        //           << vec_cond_descr[j].name << "  in the row nr " << current_line << endl;


        unsigned int extra_lines = 0 ;
        // loop over 1D AND
        //#ifdef NIGDY

        //    cout << "    item_1D_AND.size() = " << vec_cond_descr[j].item_1D_AND.size() << endl;
        for (unsigned int r = 0; r < vec_cond_descr[j].item_1D_AND.size(); ++r)
        {
            if(r)
            {
                make_new_row_in_review_if_needed(current_line+r);
                ui->review->item(current_line+r, 0)->setText( "          (cont.) ");
            }

            ui->review->item(current_line+r, 1)->setText( (vec_cond_descr[j]. item_1D_AND[r].obligatory_in_event  == 1 ?
                                                               "consider this line as: false" : " consider this line as: true"));
            ui->review->item(current_line+r, 2)->setText( (vec_cond_descr[j]. item_1D_AND[r].var_name.c_str() ));
            ui->review->item(current_line+r, 3)->setText( QString::number(vec_cond_descr[j]. item_1D_AND[r].min_value));
            ui->review->item(current_line+r, 4)->setText( QString::number(vec_cond_descr[j]. item_1D_AND[r].max_value));

            extra_lines = max(extra_lines, r) ;

        }
        // COTO ;

        //      cout << "    item_1D_OR.size() = " << vec_cond_descr[j].item_1D_OR.size() << endl;
        for (unsigned int r = 0; r < vec_cond_descr[j].item_1D_OR.size(); ++r)
        {

            //cout << "r = " << r << endl;
            if(r)
            {
                make_new_row_in_review_if_needed(current_line+r);
                if(!ui->review->item(current_line+r, 0)) ui->review->setItem(current_line+r, 0, new QTableWidgetItem);
            }
            ui->review->item(current_line+r, 5)->setText( vec_cond_descr[j]. item_1D_OR[r].var_name.c_str() );
            ui->review->item(current_line+r, 6)->setText( QString::number(vec_cond_descr[j]. item_1D_OR[r].min_value));
            ui->review->item(current_line+r, 7)->setText( QString::number(vec_cond_descr[j]. item_1D_OR[r].max_value));

            extra_lines = max(extra_lines, r) ;

        } //
        //        COTO ;
        //   cout << "    item_2D_AND.size() = " << vec_cond_descr[j].item_2D_AND.size() << endl;

        for (unsigned int r = 0; r < vec_cond_descr[j].item_2D_AND.size(); ++r)
        {
            if(r)
            {
                make_new_row_in_review_if_needed(current_line+r);
                ui->review->item(current_line+r, 0)->setText( "          (cont.) ");
            }

            ui->review->item(current_line+r, 8)->setText( (vec_cond_descr[j]. item_2D_AND[r].obligatory_in_event  == 1 ?
                                                               "consider this line as: false" : " consider this line as: true"));
            ui->review->item(current_line+r, 9)->setText( vec_cond_descr[j]. item_2D_AND[r].var_x_name.c_str()  );
            ui->review->item(current_line+r, 10)->setText( vec_cond_descr[j]. item_2D_AND[r].var_y_name.c_str()) ;
            ui->review->item(current_line+r, 11)->setText( vec_cond_descr[j]. item_2D_AND[r].polygon_name.c_str());
            extra_lines = max(extra_lines, r) ;
        }


        //-----------
        //        COTO ;
        //   cout << "    item_2D_OR.size() = " << vec_cond_descr[j].item_2D_OR.size() << endl;
        for (unsigned int r = 0; r < vec_cond_descr[j].item_2D_OR.size(); ++r)
        {

            if(r)
            {
                make_new_row_in_review_if_needed(current_line+r);
                ui->review->item(current_line+r, 0)->setText( "          (cont.) ");
            }

            //          ui->review->item()->setText(current_line+r, 1, (vec_cond_descr[j]. item_2D_OR[r].obligatory_in_event  == 1 ?
            //            "obligatory" : " consider this line as: true"));
            ui->review->item(current_line+r, 12)->setText(  vec_cond_descr[j]. item_2D_OR[r].var_x_name.c_str() );
            ui->review->item(current_line+r, 13)->setText(  vec_cond_descr[j]. item_2D_OR[r].var_y_name.c_str());
            ui->review->item(current_line+r, 14)->setText(  vec_cond_descr[j]. item_2D_OR[r].polygon_name.c_str());
            extra_lines = max(extra_lines, r) ;

        }


        //-----------
        //    COTO ;

        //   cout << "    nodes other_condition_AND.size() = " << vec_cond_descr[j].other_condition_AND.size() << endl;
        for (unsigned int r = 0; r < vec_cond_descr[j].other_condition_AND.size(); ++r)
        {
            if(r)
            {
                make_new_row_in_review_if_needed(current_line+r);
                ui->review->item(current_line+r, 0)->setText( "          (cont.) ");
            }
            ui->review->item(current_line+r, 15)->setText(  vec_cond_descr[j]. other_condition_AND[r].c_str() );
            extra_lines = max(extra_lines, r) ;
        }
        //    COTO ;
        for (unsigned int r = 0; r < vec_cond_descr[j].other_condition_OR.size(); ++r)
        {
            if(r)
            {
                make_new_row_in_review_if_needed(current_line+r);
                ui->review->item(current_line+r, 0)->setText( "          (cont.) ");
            }
            ui->review->item(current_line+r, 16)->setText( vec_cond_descr[j]. other_condition_OR[r].c_str() );
            extra_lines = max(extra_lines, r) ;
        }
        //        COTO ;
        for (unsigned int r = 0; r < vec_cond_descr[j].other_condition_NAND.size(); ++r)
        {
            if(r)
            {
                make_new_row_in_review_if_needed(current_line+r);
                ui->review->item(current_line+r, 0)->setText( "          (cont.) ");
            }
            ui->review->item(current_line+r, 17)->setText( vec_cond_descr[j]. other_condition_NAND[r].c_str() );
            extra_lines = max(extra_lines, r) ;
        }
        //        COTO ;
        for (unsigned int r = 0; r < vec_cond_descr[j].other_condition_NOR.size(); ++r)
        {
            if(r)
            {
                make_new_row_in_review_if_needed(current_line+r);
                ui->review->item(current_line+r, 0)->setText( "          (cont.) ");
            }
            ui->review->item(current_line+r, 18)->setText( vec_cond_descr[j]. other_condition_NOR[r].c_str() );
            extra_lines = max(extra_lines, r) ;
        }


        //    COTO ;
        // Finishing the conditon definition is a row of stars-----------

        //        ui->review->setRowCount(current_line+extra_lines+1);


        // one row more
        make_new_row_in_review_if_needed(current_line+ extra_lines+1);
        for(unsigned r = 0 ; r < 19 ; r++)   // <--- 19 columns of the text
        {
            ui->review->item(current_line+ extra_lines+1, r)->setBackground(QColor(220,220,220));
			ui->review->item(current_line+ extra_lines+1, r)->setText("***" );
        }

        current_line += extra_lines +1 ;

    }// end of loop over conditons
    //    ui->review->setRowCount (current_line-1);


    //------------------------------------------
    for(int row = 0 ; row < ui->review->rowCount(); row++)
        for(int column = 0 ; column < ui->review->columnCount() ; column++)
        {
            // make them all not editable !!!!!!!!!!!

            ui->review->item(row, column) ->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }
    ui->review->resizeColumnsToContents();

    QApplication::restoreOverrideCursor();
}
//*************************************************************************************
void T4manager_user_conditions::on_push_Save_review_as_file_clicked()
{

    ofstream plik((gpath.conditions + "report_conditions.txt").c_str() );

    int nr_rows = ui->review->rowCount ();

    for (int j = 0; j < nr_rows; j++ )
    {
        if(ui->review->item(j, 0) == NULL) continue ; // perhaps extra empty row?
        for(int k = 0 ; k < 19; k++)
        {
            //string co = ui->review->text(j, k).toStdString() ;
            if(ui->review->item(j, k)->text().isNull() )
                plik << "    " << " ; " ;
            else
                plik << ui->review->item(j, k)->text().toStdString() << " ; " ;

        }
        plik << "\n";

    }

    plik.close() ;

    QString wiad ;
    if(plik)
    {
        wiad = QString("Report succesfully written as \n") +
                (gpath.conditions+ "report_conditions.txt").c_str() ;
        QMessageBox::information(this,
                                 "Report about user defined conditions",
                                 wiad,
                                 QMessageBox::Ok,
                                 QMessageBox::NoButton,
                                 QMessageBox::NoButton);
    }
    else
    {
        wiad = QString("Error while writing the report \n") +
                (gpath.conditions+ "report_conditions.txt").c_str() ;

        QMessageBox::critical(this,
                              "Report about user defined  conditions",
                              wiad,
                              QMessageBox::Ok,
                              QMessageBox::NoButton,
                              QMessageBox::NoButton);
    }
    return ;

}
////*************************************************************************************
//void T4manager_user_conditions::review_double_clicked()
//{


//}
////*********************************************************************************************
//void T4manager_user_conditions::update_review2()
//{
//  update_review() ;
//}
//**********************************************************************************************
bool T4manager_user_conditions::anybody_uses_this_condition()
{
    // 1. Look into the user def SPECTRA directory, open every spectrum
    // and read the keyword condition_name
    // if this the name of this conditon, put it on the list

    // 2. Other conditons can use this condition
    return true;

}
//**************************************************************************
bool T4manager_user_conditions::is_possible_to_remove_this_condition( string condition_name )
{
    /*loop over all user def spectra available on the disk and checking if  any of them
  is using a condition with this name

  */
    cout
            << " F. T4manager_user_conditions::is_possible_to_remove_this_condition("
            << condition_name
            << ")"
            << endl;


    bool possible = true;
    string list_of_spectra_which_uses;

    // INSIDE THE SPECTRA DEFINITIONS ---------------------------------------
    // taking the spectra names from the directory - only
    QDir katalog2(gpath.Quser_def_spectra() );
    QString name_filter = user_cond_filter.c_str() ;
    // in the condition dir
    QStringList lista = katalog2.entryList (QStringList(name_filter) ) ;
    int nr = 0 ;
    for ( QStringList::Iterator it = lista.begin() ; it != lista.end() ;  ++it, nr++ )
    {
        // cout << "Trying to read spectrum  definition " << *it << endl;
        string name_c = gpath.Quser_def_spectra().toStdString() + (*it).toStdString() ;

        // Opening the definition and checking  if there is a banana name here
        ifstream plik(name_c.c_str());
        if(!plik)
        {
            cout << "Error while opening spectrum definition " << name_c << endl;
            possible = false;
            continue ; // error wile opening
        }
        //        cout << "Searching the name " <<   condition_name   << " iniside this spectrum " << name_c << endl;

        string word;
        while(!plik.eof() )
        {
            plik >> word ;
            if(!plik)
                break;

            if(word == condition_name)
            {
                // this condition is using the given polygon, so can't be erased
                list_of_spectra_which_uses += string((*it).toStdString()) + ",    " ;
                cout << "Found the name " <<   condition_name   << " inside this spectrum " << name_c << endl;
                possible = false;
                break;
            }
        } // end of while reading one condition file

    } // end of reading all conditonis

    if(!possible)
    {
        // removing from the disk file
        string mmm ;
        mmm += "The condition named\n";
        mmm +=  condition_name;
        mmm += " \ncan not be removed , because it is applied to the user defined spectra:\n";
        mmm += list_of_spectra_which_uses;
        mmm += "\n(You can delete a user_defined spectrum - using the the User Defined Spectrum Manager)";

        QMessageBox::critical( this, "Impossible to delete the condition",
                               mmm.c_str(),
                               QMessageBox::Ok | QMessageBox::Default,
                               QMessageBox::NoButton,
                               QMessageBox::NoButton);  // error

    }// end if impossible

    //========================================================
    // the conditition can be also used by other conditions, so we need to check also this
    { // local scope, because some variables are the same
        string list_of_conditions_which_uses;

        // taking the spectra names from the directory - only
        QDir katalog3(gpath.Qconditions() );
//        QString
        name_filter = user_cond_filter.c_str();
        // in the condition dir
        //QStringList
                lista = katalog3.entryList (QStringList(name_filter) ) ;
//        int
                nr = 0 ;
        for ( QStringList::Iterator it = lista.begin() ; it != lista.end() ;  ++it, nr++ )
        {
            //            cout << "Trying to read condition definition " << (*it).toStdString() << endl;
            string name_c = gpath.conditions + (*it).toStdString() ;

            // Opening the definition and checking  if there is a condition name here

            if(name_c == condition_name ) continue;

            ifstream plik(name_c.c_str());
            if(!plik)
            {
                cout << "Error while opening condition definition " << name_c << endl;
                possible = false;
                continue ; // error while opening
            }

            string word;
            while(!plik.eof() )
            {
                plik >> word ;
                if(!plik)
                    break;

                if(word == condition_name)
                {
                    // this condition is using the given polygon, so can't be erased
                    ostringstream numerek ;
                    numerek << nr +1 ;
                    list_of_conditions_which_uses += numerek.str() + (") ") + (*it).toStdString() + ",    " ;
                    //  cout << "Found the name " <<  banana_name
                    //   << " iniside this condition " << name_c << endl;
                    possible = false;
                    break;
                }
            } // end of while reading one condition file

        } // end of reading all conditonis

        if(!possible && list_of_conditions_which_uses.size() )
        {
            // removing from the disk file
            string mmm ;
            mmm += "The condition named\n";
            mmm +=  condition_name;
            mmm += " \ncan not be removed , because it us used by some other condition(s):\n";
            mmm += list_of_conditions_which_uses;
            mmm += "\n(You must delete them first )";

            QMessageBox::critical( this, "Impossible to delete the condition",
                                   mmm.c_str(),
                                   QMessageBox::Ok | QMessageBox::Default,
                                   QMessageBox::NoButton,
                                   QMessageBox::NoButton);  // error

        }// end if impossible

    } // end of local scope
    return possible;

}
//********************************************************************************************
void T4manager_user_conditions::filtered_condtions_only()
{

    /* QString filtr = ui->ComboBox_filter->currentText ();

   ListBox_available->clear();

   QStringList found = all_spectra_names.grep(QRegExp(filtr, true,true) );

   ListBox_available->insertStringList(found) ;
   */
}

//********************************************************************************************************
void T4manager_user_conditions::on_push_A_1_cloning_clicked()
{
    //    cout << "on_push_A_1_cloning_clicked()" << endl;

    class error
    {
    public:
        string title ;
        string message;
        int kind ; // 1 error, 2 warning
    };
    error e;
    //---------------
    string introduction_txt =  "This is the option to clone a condition which contains \n"
                               "at most 2 pattern of characters which had to be smartly replaced\n"
                               "Such substring will be replaced with your desired values"
                               "(i.e. for all possible cluster crystals)\n"
                               "By this action, the set of new spectra wiil be created.\n\n"
                               "The replacement will be done :\n"
                               "The replacement will be done:\n"
                               "    1. Inside  the name of the conditon, \n"
                               "and everywhere inside the definition of this conditon:\n"
                               "    2. Inside the name of any incrementer (variable) ,\n"
                               "    3. Inside the name of any other condition refered in this condition.\n";

    QMessageBox::information(this,
                             "Cloning the conditon ", introduction_txt.c_str(),

                             QMessageBox::Ok);



    bool flag_any_change = false;
    try
    {
        int nr = ui->table->currentRow() ;
        if(nr >= ui->table->rowCount() || nr == -1)
        {
            e.title = "No conditon is selected to clone";
            e.message = "Please  select the conditon to be cloned\nby clicking once on its name\n";
            e.kind = 2; // warning;
            throw e;
        }
        row_is_selected();
#if 0
        // checking if the name of the spectrum contains the substring A_1
        string pattern = "_A_1_";
        string condname = vec_cond_descr[nr].name ;
        string::size_type loc = condname.find( pattern);
        if( loc == string::npos )
        {
            e.title = "Wrong name of the conditon ";
            e.message = "The name of the selected conditon does not contain the substring \"_A_1_\" ";
            e.kind = 1; // critial
            throw e;
        }

        int result = QMessageBox::information(this,
                                              "Cloning the _A_1_ conditon ",
                                              "This is the option to clone a conditon  which contains the substring \"_A_1_\"\n "
                                              "Such substring will be replaced with _A_2_ , _A_3_, ... , _R_7_"
                                              "(i.e. for all possible cluster crystals)\n"
                                              "By this action, the new 104 conditons  wiil be created.\n\n"
                                              "The replacement will be done:\n"
                                              "    1. Inside  the name of the conditon, \n"
                                              "and everywhere inside the definition of this conditon:\n"
                                              "    2. Inside the name of any incrementer (variable) ,\n"
                                              "    3. Inside the name of any other condition refered in this condition.\n"

                                              "\n\nDo you really want to create such set of 104 clones ? ",

                                              QMessageBox::Yes,
                                              QMessageBox::No,
                                              QMessageBox::Cancel);
        if(result != QMessageBox::Yes )
        {
            return ;
        }


        //
        //cout << "OK, we can clone it !" << endl;

        //  reading the whole contents of the file
        string pathed_name = gpath.conditions + condname + user_cond_ext;

        ifstream plik(pathed_name.c_str() );
        if(!plik)
        {
            string m = "Error while opening the file :";
            m + pathed_name;

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
            cout << "No " << pattern << " found inside the file " <<  condname << endl;

            e.title = "Some nonsense ? ";
            e.message = "The  cloned conditon  definition does not use anything with the substring \"_A_0_\" "
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
        int  odp = QMessageBox::information(this, "Set of Detectors", mess.c_str(),
                                            "ABCDEFGJKLMNPQR",
                                            "ABCDGJKL",  "Cancel",
                                            0 );

        switch(odp)
        {
        case 0:
        default:
            break;

        case 1:
            markers = "ABCDGJKL" ;
            break;

        case 2:
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
                string new_filename = condname;
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
                new_filename =  gpath.conditions + new_filename +  user_cond_ext;

                // checking if it exists
                if(make_checking_if_clone_exists)
                {
                    ifstream plik_exists(new_filename.c_str());
                    if(plik_exists)
                    {
                        int odp =  QMessageBox::question(this,
                                                         "Overwrite ?",
                                                         QString(string("Condition named  \n" + new_filename + "\nalready exist. \n Overwite it?").c_str() ),
                                                         QMessageBox::Yes,
                                                         QMessageBox::YesAll,
                                                         QMessageBox::No);
                        switch(odp)
                        {
                        case QMessageBox::No: continue; break;
                        case QMessageBox::Yes: break;
                        case QMessageBox::YesAll: make_checking_if_clone_exists = false ; break;
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
                flag_any_change = true;
            } // end for  d (detectors) and for k

        update_the_table() ;

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
#else



        string condname = vec_cond_descr[nr].name ;
        // string::size_type loc ;

        string pattern1 = "_00_";
        string pattern2  = "";
        string one = "B C";
        string two = "01 02 ";

        vector<string> chain_one;   // for result
        vector<string> chain_two;
        vector<string> filenames;


        //  reading the whole contents of the template file
        string pathed_name = gpath.conditions + condname + user_cond_ext;

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



        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        T4pattern_defining_dlg dlg;
        dlg.set_parameters(condname, pattern1, pattern2, &one, &two, contents);
        if( dlg.exec() != QDialog::Accepted) return;

        dlg.get_parameters(&chain_one, &chain_two, &filenames);

        cout << "After closing the dialog for parameters there are following spectra" << endl;

        for(auto x : filenames)
        {
            cout << x << endl;

        }




        string file_contents_skeleton = dlg.find_patterns_and_make_skeleton_with_procents(contents);

        cout << "SZKIELET CONDITIION ZAWIERA: \n"<< file_contents_skeleton << endl;

        //  search all the positions of the pattern substrings and replace with  %1 %2

        // FOR loop over all crystals========================================
        bool make_checking_if_clone_exists = true;
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
		if(result != 0)   // QMessageBox::Yes )
        {
            return ;
        }

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


                string result4 =
                        dlg.make_a_clone_from_skeleton_using_kombination(file_contents_skeleton,
                                                                         chain_one[d],
                                                                         chain_two[k],
                                                                         &result_bw,
                                                                         &flag_any_change2);


                if(!flag_any_change2) continue;
                //cout << "result: " << result << endl;



                // ------------------------------------

                // save under a proper name --------------------------

                string new_filename = filenames[file_nr++]; // condname;
                //cout << "New file " << nr << ")  = " << new_filename << endl;
                // adding the path --------------
                new_filename =  gpath.conditions + new_filename +  user_cond_ext;

                // checking if it exists

                if(make_checking_if_clone_exists)
                {
                    ifstream plik_exists(new_filename.c_str());
                    if(plik_exists)
                    {
                        int odp =  QMessageBox::question(this,
                                                         "Overwrite ?",
                                                         string("Condition called \n" +
                                                                new_filename +
                                                                "\nalready exist. \n Overwite it?").c_str(),
                                                         QMessageBox::Yes,
                                                         QMessageBox::YesAll,
                                                         QMessageBox::No);
                        switch(odp)
                        {
                        default:
                        case QMessageBox::No: continue; break;
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


        update_the_table() ;
        // if(flag_any_change)appl_form_ptr-> warning_spy_is_in_action();

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


#endif

    raise();   // to the top of desktop
    if(flag_any_change)appl_form_ptr-> warning_spy_is_in_action();
}
//********************************************************************************************
void T4manager_user_conditions::on_ComboBox_filter_textChanged(const QString &  /* arg1*/ )
{
  static bool flag_working = 0;

  if(flag_working) return; // if there are many conditions - we do not want "pile-ups"

  flag_working = true;
  update_the_table();
  flag_working = false;
}
//********************************************************************************************
void T4manager_user_conditions::on_ComboBox_filter_editTextChanged(const QString &arg1)
{
	on_ComboBox_filter_textChanged(arg1 );
}
//********************************************************************************************

void T4manager_user_conditions::on_table_cellDoubleClicked(int /*row*/, int /*column*/)
{
    on_push_edit_selected_condition_clicked();
}
//********************************************************************************************
void T4manager_user_conditions::make_new_row_in_review_if_needed(int row)
{
    if(ui->review->rowCount() <= row)
    {
        ui->review->setRowCount(row+1);

    }

    for(int col = 0; col < ui->review->columnCount() ; col++)
    {
        if(ui->review->item(row, col)== NULL )
        {
            ui->review->setItem(row, col, new QTableWidgetItem("",(Qt::ItemIsEnabled | Qt::ItemIsSelectable)) );
        }
    }

}
//********************************************************************************************
void T4manager_user_conditions::on_review_cellDoubleClicked(int row, int column)
{
    //    cout << "on_review_cellDoubleClicked(int row, int column)" << endl;
    //      cout << "Dbl click on the row " << row << endl;

    column = column ; // fake

    string last_legal_name ;
    for(int i = 0 ; i <= row ; i ++)
    {
        string current_name = ui->review->item(i, 0)->text().toStdString() ;
        //    cout << "Checking name " << current_name << endl;
        if(current_name.find("(cont.)") != string::npos)
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
    for(unsigned int i = 0 ; i < vec_cond_descr.size() ; i++)
    {
        //    cout << "comparing with " << vec_cond_descr[i].name_of_spectrum << endl;
        if(vec_cond_descr[i].name ==  last_legal_name)
        {
            which = i ;
            //       cout << "remembered as spectrum nr " << i << endl ;
        }
    }
    if(which == -1)
        return ;

    //row_is_selected();

    // find such a name in the vector

    T4user_condition_wizard * dlg = new T4user_condition_wizard ;
    dlg->set_parameters(& vec_cond_descr[which] ) ; // true = editing, cloning is allowed
    if ( dlg->exec() == QDialog::Accepted )
    {
        vec_cond_descr[which].name
                = is_name_unique(vec_cond_descr[which].name, which);

        //update_the_table() ;
        vec_cond_descr[which].save_condition();
        update_the_table() ;
        appl_form_ptr-> warning_spy_is_in_action();
    }
    raise();   // to the top of desktop



}

