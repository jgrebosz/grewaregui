#include "t4user_condition_wizard.h"
#include "ui_t4user_condition_wizard.h"

#include <QDir>
#include <QStringList>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>

#include "t4incrementer_selector.h"
#include "Tcondition_description_cracow.h"
#include "t4addline_1d_wizard.h"

//********************************************************************************************
T4user_condition_wizard::T4user_condition_wizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::T4user_condition_wizard)
{
    ui->setupUi(this);

    // To avoid writing in Polish
    setButtonText(QWizard::NextButton, "Next  >" );
    setButtonText(QWizard::BackButton, "<  Back" );
    setButtonText(QWizard::CancelButton, "Cancel" );
    setButtonText(QWizard::FinishButton, "Finish" );

    // I made it here, below, manually because the designer is buggy in this helpRequest case
    setOption(HaveHelpButton, true);
    connect(this, SIGNAL(helpRequested()), this, SLOT(help()));
    setButtonText(QWizard::HelpButton, "Help" );

    // ---------------------------------------------------------------------------------------------------------------
    QStringList labels;
    labels << "What to do if the incrementer is NOT VALID\nfor particular event"
           << "variable (incrementer)" << "minimum" <<"maximum" ;
    ui->table_1D_AND_gates->setHorizontalHeaderLabels ( labels );
    ui->table_1D_AND_gates->resizeColumnsToContents ();

    ui->table_1D_OR_gates->setColumnCount(4);
    ui->table_1D_OR_gates->setHorizontalHeaderLabels ( labels );
    ui->table_1D_OR_gates->resizeColumnsToContents ();

    // colors of left label
    ui->table_1D_OR_gates->verticalHeader()->setStyleSheet( "background-color:rgb(230,255, 230, 255); ");
    ui->table_1D_AND_gates->verticalHeader()->setStyleSheet( "background-color: rgb(255, 230, 230, 255);");
    ui->table_2D_OR_gates->verticalHeader()->setStyleSheet( "background-color:rgb(230,255, 230, 255); ");
    ui->table_2D_AND_gates->verticalHeader()->setStyleSheet( "background-color: rgb(255, 230, 230, 255);");

    labels.clear();
    labels << "What to do\nif X or Y incrementer is NOT VALID\nin particular event" << "X incrementer\nname" << "Y incrementer\nname" <<"polygon (banana)\nname" ;
    ui->table_2D_AND_gates->setHorizontalHeaderLabels ( labels );
    ui->table_2D_AND_gates->resizeColumnsToContents ();

    ui->table_2D_OR_gates->setHorizontalHeaderLabels ( labels );
    ui->table_2D_OR_gates->resizeColumnsToContents ( );


    consider_true =  "It's optional, so consider this gate as true";
    consider_false = "It's obligatory, so consider this gate as false";

    // from  init() -----------------------------------------------

    ui->table_1D_OR_gates->hideColumn (0); // obligatory/when legal has no difference in case of OR
    ui->table_2D_OR_gates->hideColumn (0); // obligatory/when legal has no difference in case of OR


    user_wants_1D_OR = false;
    user_wants_1D_AND = false;
    user_wants_2D_OR = true;
    user_wants_2D_AND = false;
    user_wants_external_nodes = true;

    ui->checkBox_OR_gates->setChecked(user_wants_1D_OR);
    ui->checkBox_AND_gates->setChecked(user_wants_1D_AND);
    ui->checkBox_OR_bananas->setChecked(user_wants_2D_OR);
    ui->checkBox_AND_bananas->setChecked(user_wants_2D_AND);
}
//***************************************************************************************************************
T4user_condition_wizard::~T4user_condition_wizard()
{
    delete ui;
}
//***************************************************************************************************************
//***************************************************************************************************************
void T4user_condition_wizard::set_parameters( Tcondition_description_cracow *cond_ptr )
{
    cond = cond_ptr ;
    ui->lineEdit_condition_name->setText(cond->name.c_str());

    // table 1 D
    redisplay_table_1D_AND();
    redisplay_table_1D_OR();
    redisplay_table_2D_AND();
    redisplay_table_2D_OR();
    redisplay_tables_nodes();

    // check the contents. If some page contains already any information - show this page
    // even if it is not by default

    if(ui->table_1D_OR_gates->rowCount())  user_wants_1D_OR = true;
    if(ui->table_1D_AND_gates->rowCount())  user_wants_1D_AND = true;
    if(ui->table_2D_OR_gates->rowCount())  user_wants_2D_OR = true;
    if(ui->table_2D_AND_gates->rowCount())  user_wants_2D_AND = true;

    ui->checkBox_OR_gates->setChecked(user_wants_1D_OR);
    ui->checkBox_AND_gates->setChecked(user_wants_1D_AND);
    ui->checkBox_OR_bananas->setChecked(user_wants_2D_OR);
    ui->checkBox_AND_bananas->setChecked(user_wants_2D_AND);

}
//***************************************************************************************************************
int T4user_condition_wizard::nextId () const
{
    //  cout << "F. nextId "      << currentPage()->title().toStdString()  << endl;
    switch (currentId())
    {
    default:
    case  page__name:

        return page__5possibilities;
        // break;

    case page__5possibilities:
        if(user_wants_1D_OR) return page__1D_OR;
        if(user_wants_1D_AND) return page__1D_AND;
        if(user_wants_2D_OR) return page__2D_OR;
        if(user_wants_2D_AND) return page__2D_AND;
        if(user_wants_external_nodes) return page__external_nodes;
        else return page__finish;
        //        break;

    case page__1D_OR:
        if(user_wants_1D_AND) return page__1D_AND;
        if(user_wants_2D_OR) return page__2D_OR;
        if(user_wants_2D_AND) return page__2D_AND;
        if(user_wants_external_nodes) return page__external_nodes;
        else return page__finish;
        // break;

    case page__1D_AND:
        if(user_wants_2D_OR) return page__2D_OR;
        if(user_wants_2D_AND) return page__2D_AND;
        if(user_wants_external_nodes) return page__external_nodes;
        else return page__finish;
        // break;

    case page__2D_OR:

        if(user_wants_2D_AND) return page__2D_AND;
        if(user_wants_external_nodes) return page__external_nodes;
        else return page__finish;
        // break;

    case page__2D_AND:
        if(user_wants_external_nodes) return page__external_nodes;
        else return page__finish;
        // break;

    case page__external_nodes:
        return page__finish;

    case page__finish:
        return -1;

    } // switch
    return -1;
}
//***************************************************************************************************************
void T4user_condition_wizard::accept()
{
    cond->name = ui->lineEdit_condition_name->text().toStdString();
    QDialog::accept() ;
}
//***************************************************************************************************************
void T4user_condition_wizard::name_changed()
{
}
//***************************************************************************************************************
void T4user_condition_wizard::redisplay_table_1D_AND()
{
    int nr_rows =cond->item_1D_AND.size();
    ui->table_1D_AND_gates->setRowCount (nr_rows);

    // creating row widgets
    int nr_columns = ui->table_1D_AND_gates->columnCount ();
    for(int r = 0 ; r < nr_rows ; r++)
        for(int c = 0 ; c < nr_columns ; c++)
        {
            ui->table_1D_AND_gates->setItem(r,c, new QTableWidgetItem("") );
            ui->table_1D_AND_gates->item(r, c)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }

    QStringList list  ;
    list << "consider this line as: true" << "consider this line as: false" ;

    for (int j = 0; j < nr_rows; ++j )
    {
        ui->table_1D_AND_gates->setItem(j, 0, new QTableWidgetItem ( cond->item_1D_AND[j].obligatory_in_event?
                                                                       consider_false:
                                                                       consider_true) );

        ui->table_1D_AND_gates->item(j, 0)->setCheckState(cond->item_1D_AND[j].obligatory_in_event?
                                                              Qt::Checked : Qt::Unchecked);
        //      setCurrentItem ( cond->item_1D_AND[j].obligatory_in_event) ;  // <-- zaleznie od tego

        ui->table_1D_AND_gates->item(j, 1) -> setText(cond->item_1D_AND[j].var_name.c_str() );
        ui->table_1D_AND_gates->item(j, 2) -> setText(QString("!%1").setNum(cond->item_1D_AND[j].min_value)) ;
        ui->table_1D_AND_gates->item(j, 3) -> setText(QString("@%1").setNum(cond->item_1D_AND[j].max_value)) ;
    }
    ui->table_1D_AND_gates->resizeColumnsToContents();
}
//***************************************************************************************************************
void T4user_condition_wizard::redisplay_table_2D_AND()
{
    int nr_rows =cond->item_2D_AND.size();
    ui->table_2D_AND_gates->setRowCount (nr_rows);

    // creating row widgets
    int nr_columns = ui->table_2D_AND_gates->columnCount ();
    for(int r = 0 ; r < nr_rows ; r++)
        for(int c = 0 ; c < nr_columns ; c++)
        {
            ui->table_2D_AND_gates->setItem(r,c, new QTableWidgetItem("") );
            ui->table_2D_AND_gates->item(r, c)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }
    //--------------------

    for (int j = 0; j < nr_rows; ++j )
    {
        ui->table_2D_AND_gates->setItem(j, 0, new QTableWidgetItem ( cond->item_2D_AND[j].obligatory_in_event?
                                                                       consider_false:
                                                                       consider_true) );

        ui->table_2D_AND_gates->item(j, 0)->setCheckState(cond->item_2D_AND[j].obligatory_in_event?
                                                              Qt::Checked : Qt::Unchecked);

        ui->table_2D_AND_gates->item(j, 1)->setText( cond->item_2D_AND[j].var_x_name.c_str() );
        ui->table_2D_AND_gates->item(j, 2)->setText( cond->item_2D_AND[j].var_y_name.c_str()  );

        string pname = cond->item_2D_AND[j].polygon_name ;
        if(ui->checkBox_short_polygon_names_AND->isChecked())
        {
            if(pname != double_click_here)
                pname.erase(0, pname.rfind("_polygon_")+9);
        }
        ui->table_2D_AND_gates->item(j, 3)->setText( pname.c_str()  );
    }

    ui->table_2D_AND_gates->resizeColumnsToContents();

}
//***************************************************************************************************************
void T4user_condition_wizard::change_var_x_AND(int row)
{
    // selected row
    //unsigned row = ui->table_2D_AND_gates->currentRow() ;
    if(row > (signed int ) cond->item_2D_AND.size() ) return ;

    T4incrementer_selector *dlg  = new T4incrementer_selector ;
    dlg->setWindowTitle("List of variables on which you may set gates");
    dlg->set_multi_selection(false);
    dlg->make_incremeter_ALL_impossible();

    if(dlg->exec() == QDialog::Accepted)
    {
        // NOTE : here we demand only one incrementor !!!
        QStringList lista = dlg->give_seleced_items() ;
        dlg->setWindowTitle("List of variables on which you may set gates");
        for ( QStringList::Iterator it = lista.begin(); it != lista .end(); ++it )
        {
            //cout << *it << ":";
            cond->item_2D_AND[row].var_x_name = (*it).toStdString() ;
        }
        redisplay_table_2D_AND();
    }
    raise();   // to the top of desktop
}
//***************************************************************************************************************
void T4user_condition_wizard::change_var_y_AND(unsigned int row)
{
    // selected row
    //unsigned row = ui->table_2D_AND_gates->currentRow() ;
    if(row > cond->item_2D_AND.size() ) return ;

    T4incrementer_selector *dlg  = new T4incrementer_selector ;
    dlg->setWindowTitle("List of variables on which you may set gates");
    dlg->set_multi_selection(false);
    dlg->make_incremeter_ALL_impossible();

    if(dlg->exec() == QDialog::Accepted)
    {
        // NOTE : here we demand only one incrementor !!!
        QStringList lista = dlg->give_seleced_items() ;
        for ( QStringList::Iterator it = lista.begin(); it != lista .end(); ++it )
        {
            //cout << *it << ":";
            cond->item_2D_AND[row].var_y_name = (*it).toStdString() ;
        }
        redisplay_table_2D_AND();
    }
    raise();   // to the top of desktop
}
//***************************************************************************************************************
void T4user_condition_wizard::change_polygon_AND(unsigned int row)
{
    //  unsigned row = ui->table_2D_AND_gates->currentRow() ;
    if(row > cond->item_2D_AND.size() ) return ;


    QDir katalog(gpath.Qpolygons()) ;  //
    QStringList lst = katalog.entryList ( QStringList("*.poly") ) ;
    //QStringList lst;
    //lst << "First" << "Second" << "Third" << "Fourth" << "Fifth";

    bool ok;
    QString res = QInputDialog::getItem(this,
                                        "Avaliable polygon gates", "Select a polygon:", lst, 1, true, &ok);
    if ( ok ) {
        // user selected an item and pressed OK
        cond->item_2D_AND[row].polygon_name = res.toStdString() ;
        redisplay_table_2D_AND();
    } else {
        // user pressed Cancel
    }
}
//***************************************************************************************************************
void T4user_condition_wizard::help()
{
    //    cout << "T4user_condition_wizard::help()" << endl;

    QString not_valid =
        "What to do if the incrementer is not valid?\n"
        "\"it is obligatory\" - means that chosen variable is ALWAYS TESTED and must have this value in every event \n"
        "\"it is optional\" - means that if this variable is TESTED ONLY IF IT HAS A LEGAL VALUE in this event\n\n "
        "For example:\n"
        " - the germanium detector energy data it is legal only when its detector has fired,\n"
        " - multiwire chamber (ppac) 'position x' is legal only when this detector has fired and if it was possible to calculate position\n"
        " from the data in current event (that means: both 'left' and 'right' signals comming from multiwire chamber \n"
        "were present in this event).\n"
        "\nIf you know when your favourite variable should be 'legal' - "
        "tell this to Jurek, he will set its defaults  in the program\n";

    QString just_incrementer =                 "Here you may define the one dimmensional conditons.\n"
                               "1. Add a row to this table\n"
                               "2. Choose the variable which wil be tested \n"
                               "3. Set the minimum value of the gate\n"
                               "4. Set the maximum value of the gate\n\n"
                               "Note:\n"
                               "You may double click on the item to edit it.\n"
                               "You may also click and drag on the table to select (and edit) many values\n"
                               "You may click on the header of the column to select all variables in the column\n\n";

    QString help_message ;

    switch (currentId())
    {
    case  page__name:
        help_message =
            "Note, that If you change this name of the condition later - perhaps there are already "
            "MANY spectra which use this condtion. \n"
            "This is why Jurek decided, that the condition with the old name should "
            "still remain on the disk";
        break;

    case page__1D_OR :
        help_message = just_incrementer ;
        break;

    case page__1D_AND:
        help_message = just_incrementer + not_valid ;
        break;

    case page__2D_OR:   // 2D gates
        help_message = just_incrementer ;
        break;
    case page__2D_AND:
        help_message = just_incrementer + not_valid ;
        break;

    case   page__5possibilities:
        help_message =
            "Your condition can be a combination of 5 different parts.\n"
            "If you do not need some of them, you can say it here, so the wizard will not bother you "
            "by showing corresponding pages";
        break;

    case   page__external_nodes :
        help_message =
            "Here in your condition you can use some other, already exiting conditions\n"
            "This is really a good idea: make simple conditions and then combine them into one  more complicated.\n"
            "By this you can easily debug any logical errors";
        break;

    case   page__finish:
        help_message = "explanations about finish";
        break;


    default:
        help_message = "No explanations about this page";
        break;

    } // end switch

    if(help_message != "")
    {
        QMessageBox::information ( this, "Help about setting the conditions", help_message,
                                 QMessageBox::Ok);
    }
}
//***************************************************************************************************************
void T4user_condition_wizard::table_1D_AND_value_changed()
{
    unsigned row = ui->table_1D_AND_gates->currentRow() ;
    if(row >= cond->item_1D_AND.size() ) return ;

    unsigned col = ui->table_1D_AND_gates->currentColumn() ;
    if(col == 0 )
    {
        ui->table_1D_AND_gates->resizeColumnToContents(0) ;
    }
}
//***************************************************************************************************************
void T4user_condition_wizard::table_2D_AND_value_changed()
{
    cout << "T4user_condition_wizard::table_2D_AND_value_changed()" << endl;

    unsigned row = ui->table_2D_AND_gates->currentRow() ;
    if(row >= cond->item_2D_AND.size() ) return ;

    unsigned column = ui->table_2D_AND_gates->currentColumn() ;
    if(column == 0 )
    {
        // change the text in
        ui->table_2D_AND_gates->item(row, column)->setText(
            (ui->table_2D_AND_gates->item(row, column)->checkState() == Qt::Checked) ?
                consider_false :consider_true); // ???????
        ui->table_2D_AND_gates->item(row, column)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        ui->table_2D_AND_gates->resizeColumnsToContents();
    }
}
//***************************************************************************************************************
void T4user_condition_wizard::on_pushButton_add_1D_OR_clicked()
{
    //void T4user_condition_wizard::add_1D_OR_item()

    //  int nr_rows = ui->table_1D_OR_gates->rowCount () ;
    //  ui->table_1D_OR_gates->setRowCount (++nr_rows);

    T4incrementer_selector *dlg  = new T4incrementer_selector ;
    dlg->setWindowTitle("List of variables on which you may set gates");
    if(dlg->exec() == QDialog::Accepted)
    {

        QStringList lista = dlg->give_seleced_items() ;
        for ( QStringList::Iterator it = lista.begin(); it != lista .end(); ++it )
        {
            //cout << *it << ":";
            Tcondition_description_cracow::Titem_1D  item ;
            item.var_name = (*it).toStdString() ;
            item.obligatory_in_event = false ;
            item.min_value = 0 ;
            item.max_value = 8192 ;

            cond->item_1D_OR.push_back(item) ;
        }
    }
    redisplay_table_1D_OR();
    raise();   // to the top of desktop
}
//***************************************************************************************************************
void T4user_condition_wizard::redisplay_table_1D_OR()
{
    int nr_rows =cond->item_1D_OR.size();

    ui->table_1D_OR_gates->setRowCount (nr_rows);

    int nr_columns = ui->table_1D_OR_gates->columnCount ();
    for(int r = 0 ; r < nr_rows ; r++)
        for(int c = 0 ; c < nr_columns ; c++)
        {
            ui->table_1D_OR_gates->setItem(r,c, new QTableWidgetItem("") );
            ui->table_1D_OR_gates->item(r, c)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }

    //setCurrentItem ( int i )
    QStringList list  ;
    list << "consider this line as: true" << "consider this line as: false" ;

    for (int j = 0; j < nr_rows; ++j )
    {

#ifdef FINAL
            // self-gate - listed next to  the incrementer
        ui->table_1D_OR_gates->setItem(j, 0, new QComboTableItem( ui->table_1D_OR_gates, list ));
        //QComboTableItem *ptr =
        dynamic_cast<QComboTableItem*> (ui->table_1D_OR_gates->item(j, 0))->
            setCurrentItem ( cond->item_1D_OR[j].obligatory_in_event) ;
#endif
        ui->table_1D_OR_gates->item(j, 1)-> setText(cond->item_1D_OR[j].var_name.c_str() );
        ui->table_1D_OR_gates->item(j, 2)->setText( QString("!%1").setNum(cond->item_1D_OR[j].min_value)) ;
        ui->table_1D_OR_gates->item(j, 3)->setText( QString("@%1").setNum(cond->item_1D_OR[j].max_value)) ;
    }

    ui->table_1D_OR_gates->resizeColumnsToContents();

}
//***************************************************************************************************************
void T4user_condition_wizard::table_1D_OR_row_is_selected()
{
    int row_nr = ui->table_1D_OR_gates->currentRow() ;
    ui->table_1D_OR_gates->selectRow (row_nr ) ;
}
//***************************************************************************************************************
void T4user_condition_wizard::redisplay_table_2D_OR()
{
    int nr_rows =cond->item_2D_OR.size();
    ui->table_2D_OR_gates->setRowCount (nr_rows);
    int nr_columns = ui->table_2D_OR_gates->columnCount ();

    for(int r = 0 ; r < nr_rows ; r++)
        for(int c = 0 ; c < nr_columns ; c++)
        {
            ui->table_2D_OR_gates->setItem(r,c, new QTableWidgetItem("") );
            ui->table_2D_OR_gates->item(r, c)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }

    //setCurrentItem ( int i )
    QStringList list  ;
    list << "consider this line as: true" << "consider this line as: false" ;

    for (int j = 0; j < nr_rows; ++j )
    {
#ifdef FINAL

        // self-gate - listed next to  the incrementer
        ui->table_2D_OR_gates->setItem(j, 0, new QComboTableItem( ui->table_1D_OR_gates, list ));
        //QComboTableItem *ptr =
        dynamic_cast<QComboTableItem*> (ui->table_2D_OR_gates->item(j, 0))->
            setCurrentItem ( cond->item_2D_OR[j].obligatory_in_event) ;

#endif

        ui->table_2D_OR_gates->item(j, 1)-> setText(cond->item_2D_OR[j].var_x_name.c_str() );
        ui->table_2D_OR_gates->item(j, 2)-> setText(cond->item_2D_OR[j].var_y_name.c_str()  );

        string pname = cond->item_2D_OR[j].polygon_name ;
        if(ui->checkBox_short_polygon_names_OR->isChecked())
        {
            if(pname != double_click_here)
                pname.erase(0, pname.rfind("_polygon_")+9);
        }
        ui->table_2D_OR_gates->item(j, 3)-> setText(pname.c_str()  );
    }
    ui->table_2D_OR_gates->resizeColumnsToContents();
}

//***************************************************************************************************************
void T4user_condition_wizard::change_var_x_OR(unsigned int row_nr)
{
    // selected row

    if(row_nr > cond->item_2D_OR.size() ) return ;

    T4incrementer_selector *dlg  = new T4incrementer_selector ;
    dlg->setWindowTitle("List of variables on which you may set gates");
    dlg->set_multi_selection(false);
    dlg->make_incremeter_ALL_impossible();

    if(dlg->exec() == QDialog::Accepted)
    {

        // NOTE : here we demand only one incrementor !!!
        QStringList lista = dlg->give_seleced_items() ;
        for ( QStringList::Iterator it = lista.begin(); it != lista .end(); ++it )
        {
            //cout << *it << ":";

            cond->item_2D_OR[row_nr].var_x_name = (*it).toStdString() ;
        }
        redisplay_table_2D_OR();
    }
    raise();   // to the top of desktop
}
//***************************************************************************************************************
void T4user_condition_wizard::change_var_y_OR(unsigned int row)
{
    if(row > cond->item_2D_OR.size() ) return ;

    T4incrementer_selector *dlg  = new T4incrementer_selector ;
    dlg->setWindowTitle("List of variables on which you may set gates");
    dlg->set_multi_selection(false);
    dlg->make_incremeter_ALL_impossible();

    if(dlg->exec() == QDialog::Accepted)
    {

        // NOTE : here we demand only one incrementor !!!
        QStringList lista = dlg->give_seleced_items() ;
        for ( QStringList::Iterator it = lista.begin(); it != lista .end(); ++it )
        {
            //cout << *it << ":";
            cond->item_2D_OR[row].var_y_name = (*it).toStdString() ;
        }
        redisplay_table_2D_OR();
    }
    raise();   // to the top of desktop
}
//***************************************************************************************************************
void T4user_condition_wizard::change_polygon_OR(unsigned int row)
{
    //  unsigned row = ui->table_2D_OR_gates->currentRow() ;
    if(row > cond->item_2D_OR.size() ) return ;


    QDir katalog(gpath.Qpolygons()) ;  //
    QStringList lst = katalog.entryList (QStringList("*.poly") ) ;
    //QStringList lst;
    //lst << "First" << "Second" << "Third" << "Fourth" << "Fifth";

    bool ok;
    QString res = QInputDialog::getItem(this,
                                        "Avaliable polygon gates", "Select a polygon:", lst, 1, true, &ok);
    if ( ok ) {
        // user selected an item and pressed OK
        cond->item_2D_OR[row].polygon_name = res.toStdString() ;
        redisplay_table_2D_OR();
    } else {
        // user pressed Cancel
    }
}
//***************************************************************************************************************
//***************************************************************************************************************
void T4user_condition_wizard::table_1D_OR_value_changed()
{
    unsigned row = ui->table_1D_OR_gates->currentRow() ;
    if(row >= cond->item_1D_OR.size() ) return ;

    unsigned col = ui->table_1D_OR_gates->currentColumn() ;
    if(col == 0 )
    {
        cond->item_1D_OR[row].obligatory_in_event =
            (ui->table_1D_OR_gates->item(row, col)->checkState() == Qt::Checked)?
                true: false ;
        //      cout << "value in row" << row << "changed" << endl;
    }
}
//***************************************************************************************************************
void T4user_condition_wizard::table_2D_OR_value_changed()
{
    unsigned row = ui->table_2D_OR_gates->currentRow() ;
    if(row >= cond->item_2D_OR.size() ) return ;

    unsigned col = ui->table_2D_OR_gates->currentColumn() ;
    if(col == 0 )
    {
        //      cond->item_2D_OR[row].obligatory_in_event =
        //          dynamic_cast<QComboTableItem*> (ui->table_2D_OR_gates->item(row, col))->currentItem () ;

        cond->item_2D_OR[row].obligatory_in_event =
            (ui->table_2D_OR_gates->item(row, col)->checkState() == Qt::Checked)?
                true: false ;
    }
}
//***************************************************************************************************************
void T4user_condition_wizard::redisplay_tables_nodes()
{
    int nr_rows =cond->other_condition_AND.size();
    ui->table_other_conditions_AND->setRowCount (nr_rows);

    for (int i = 0; i < nr_rows; ++i )
    {
        if(! ui->table_other_conditions_AND->item(i, 0) )ui->table_other_conditions_AND->setItem(i, 0, new QTableWidgetItem);
        ui->table_other_conditions_AND->item(i, 0)->setText( cond->other_condition_AND[i].c_str()  );
        ui->table_other_conditions_AND->item(i, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }
    ui->table_other_conditions_AND->resizeColumnToContents(0) ;

    nr_rows =cond->other_condition_OR.size();
    ui->table_other_conditions_OR->setRowCount (nr_rows);

    for (int i = 0; i < nr_rows; ++i )
    {
        if(! ui->table_other_conditions_OR->item(i, 0) )ui->table_other_conditions_OR->setItem(i, 0, new QTableWidgetItem);

        ui->table_other_conditions_OR->item(i, 0)->setText( cond->other_condition_OR[i].c_str()  );
    }
    ui->table_other_conditions_OR->resizeColumnToContents(0) ;


    nr_rows =cond->other_condition_NAND.size();
    ui->table_other_conditions_NAND->setRowCount (nr_rows);

    for (int i = 0; i < nr_rows; ++i )
    {
        if(! ui->table_other_conditions_NAND->item(i, 0) )ui->table_other_conditions_NAND->setItem(i, 0, new QTableWidgetItem);
        ui->table_other_conditions_NAND->item(i, 0)->setText( cond->other_condition_NAND[i].c_str()  );
    }
    ui->table_other_conditions_NAND->resizeColumnToContents(0) ;

    nr_rows =cond->other_condition_NOR.size();
    ui->table_other_conditions_NOR->setRowCount (nr_rows);

    for (int i = 0; i < nr_rows; ++i )
    {
        if(! ui->table_other_conditions_NOR->item(i, 0) )ui->table_other_conditions_NOR->setItem(i, 0, new QTableWidgetItem);
        ui->table_other_conditions_NOR->item(i, 0)->setText( cond->other_condition_NOR[i].c_str()  );
    }
    ui->table_other_conditions_NOR->resizeColumnToContents(0) ;

    // nodes part (external conditon logic)
    ui->table_other_conditions_AND->resizeColumnsToContents ( );
    ui->table_other_conditions_OR->resizeColumnsToContents ( );
    ui->table_other_conditions_NAND->resizeColumnsToContents ( );
    ui->table_other_conditions_NOR->resizeColumnsToContents ( );
}
//***************************************************************************************************************
void T4user_condition_wizard::remove_other_cond_name_from_table( QTableWidget *tab, string tab_name,
                                                                vector<string> &cond_names)
{
    QList<QTableWidgetSelectionRange> range = tab->selectedRanges() ;

    //    cout << "Selection has " << range.count() << " ranges" << endl;

    for(int s = range.count()-1; s >=0 ; s--)    // can be many separated ranges on this list
    {
        cout << "range[s].bottomRow()= " << range[s].bottomRow() << ", range[s].topRow()= " <<  range[s].topRow() << endl;
        for(int row = range[s].bottomRow() ; row >= range[s].topRow() ; row--)
        {
            //  unsigned row = tab->selectRow();    ro sel currentRow() ;
            if((unsigned int) row >= cond_names.size() ) return ;

            QString mess = QString("You are going to remove the condition nr %1\n%2\n"
                                   "from the table :  %3\n"
                                   "\nAre you sure ?")
                               .arg(row+1).arg(cond_names[row].c_str()).arg(tab_name.c_str());


            switch(askYesNoCancel( "Removing the other condition nam, ARE YOU SURE ? ",
                                   mess))
            {
            case QMessageBox::Yes:  // Yes :
                // user selected an item and pressed OK
                cond_names.erase(cond_names.begin() + row);
                redisplay_tables_nodes();
                break ;
            default: break;
            } // switch
        } // for row
    } // for sel
}
//***************************************************************************************************************
void T4user_condition_wizard::on_pushButton_remove_1D_OR_clicked()
{

    remove_selected_rows_from_some_1Dtable(ui->table_1D_OR_gates, cond->item_1D_OR, "Wyjasnienie");
    redisplay_table_1D_OR();
}
//****************************************************************************************
void T4user_condition_wizard::remove_selected_rows_from_some_1Dtable(
    QTableWidget *tab,
    vector<Tcondition_description_cracow::Titem_1D> &wekt,
    string /* wyjasn*/ )
{

    QList<QTableWidgetSelectionRange> range = tab->selectedRanges() ;

    cout << "Selection has " << range.count() << " ranges" << endl;

    for(int s = range.count()-1; s >=0 ; s--)    // can be many separated ranges on this list
    {
        cout << "range[s].bottomRow()= " << range[s].bottomRow() << ", range[s].topRow()= " <<  range[s].topRow() << endl;
        for(int row = range[s].bottomRow() ; row >= range[s].topRow() ; row--)
        {
            //  unsigned row = tab->selectRow();    ro sel currentRow() ;
            if((unsigned int) row >= wekt.size() ) return ;

            QString mess = QString("You are going to remove the 1D gate  nr %1\n\n"
                                   "when incrementor not valid::  %2\n"
                                   "variable:  %3\n"
                                   "min value: %4\n"
                                   "max value: %5\n"
                                   "\nAre you sure ?")
                               .arg(row+1)
                               .arg(wekt[row].obligatory_in_event? consider_false : consider_true)
                               .arg(wekt[row].var_name.c_str())
                               .arg(wekt[row].min_value)
                               .arg(wekt[row].max_value) ;

            switch( askYesNoCancel( "ARE YOU SURE ? ", mess))
            {
            case QMessageBox::Yes:      // Yes :
                // user selected an item and pressed OK
                wekt.erase(wekt.begin() + row);
                // redisplay_tables_nodes();
                break ;
            default: break;
            } // switch
        } // for row
    } // for sel
}
//***************************************************************************************************************
void T4user_condition_wizard::remove_selected_rows_from_some_2Dtable(
    QTableWidget *tab,
    vector<Tcondition_description_cracow::Titem_2D> &wekt,
    string /*wyjasn*/)
{

    QList<QTableWidgetSelectionRange> range = tab->selectedRanges() ;

    cout << "Selection has " << range.count() << " ranges" << endl;

    for(int s = range.count()-1; s >=0 ; s--)    // can be many separated ranges on this list
    {
        cout << "range[s].bottomRow()= " << range[s].bottomRow() << ", range[s].topRow()= " <<  range[s].topRow() << endl;
        for(unsigned int row = range[s].bottomRow() ; row >= (unsigned int) range[s].topRow() ; row--)
        {
            //  unsigned row = tab->selectRow();    ro sel currentRow() ;
            if(row >= wekt.size() ) return ;

            QString mess = QString("You are going to remove the 2D gate  nr %1\n\n"
                                   "when incrementor not valid  : %2\n"
                                   "variable X: %3\n"
                                   "variable Y: %4\n"
                                   "polygon   : %5\n\n"
                                   "Are you sure ?")
                               .arg(row+1)
                               .arg(wekt[row].obligatory_in_event? consider_false : consider_true)
                               .arg(wekt[row].var_x_name.c_str())
                               .arg(wekt[row].var_y_name.c_str())
                               .arg(wekt[row].polygon_name.c_str()) ;


            switch(askYesNoCancel( "ARE YOU SURE ? ",
                                         mess))
            {
            case QMessageBox::Yes: // Yes :
                // user selected an item and pressed OK
                wekt.erase(wekt.begin() + row);
                // redisplay_tables_nodes();
                break ;
            default: break;
            } // switch
        } // for row
    } // for sel
}
//***************************************************************************************************************
void T4user_condition_wizard::on_pushButton_1D_OR_set_many_cells_clicked()
{
    bool ok;
    //double value = ui->table_1D_OR_gates->text(r, c).toDouble(&ok);

    double res = QInputDialog::getDouble(this ,
                                         "Minimum value of the gate", "Enter a decimal number:", 0,
                                         -2147483647,
                                         2147483647,
                                         6,
                                         &ok);

    if ( ok )
    {
        // user entered something and pressed OK
        for(int r = 0 ; r < ui->table_1D_OR_gates->rowCount() ; r++)
        {
            for(int c = 2 ; c < ui->table_1D_OR_gates->columnCount() ; c++)
            {
                if( ui->table_1D_OR_gates->item(r, c )-> isSelected()  )
                {
                    if(c == 2) cond->item_1D_OR[r].min_value = res ;
                    if(c == 3) cond->item_1D_OR[r].max_value = res ;
                }
            } // for c
        } // end for r
        redisplay_table_1D_OR();
    } // endif
}
//***************************************************************************************************************
void T4user_condition_wizard::on_pushButton_add_1D_AND_clicked()
{

    //  int nr_rows = ui->table_1D_AND_gates->rowCount () ;
    //  ui->table_1D_AND_gates->setRowCount (++nr_rows);

    T4incrementer_selector *dlg  = new T4incrementer_selector ;
    dlg->setWindowTitle("List of variables on which you may set gates");
    if(dlg->exec() == QDialog::Accepted)
    {

        QStringList lista = dlg->give_seleced_items() ;
        for ( QStringList::Iterator it = lista.begin(); it != lista .end(); ++it )
        {
            //cout << *it << ":";
            Tcondition_description_cracow::Titem_1D  item ;
            item.var_name = (*it).toStdString() ;
            item.obligatory_in_event = true;
            item.min_value = 0 ;
            item.max_value = 8192 ;

            // check if this name contains the keyword _WHEN_
            //
            if( ((*it)).contains("_WHEN_", Qt::CaseInsensitive))
            {
                T4addline_1d_wizard * dlg2 = new T4addline_1d_wizard;
                dlg2->set_incrementor( item.var_name.c_str() );
                if(dlg2->exec() == QDialog::Accepted)
                {
                    item.obligatory_in_event = dlg2->give_obligatory() ;

                }
                else
                {
                    redisplay_table_1D_AND();
                    raise();   // to the top of desktop
                    return ;
                }
            } // if
            cond->item_1D_AND.push_back(item) ;
        }
    }
    redisplay_table_1D_AND();
    raise();   // to the top of desktop
}
//***************************************************************************************************************
void T4user_condition_wizard::on_pushButton_remove_1D_AND_clicked()
{

    remove_selected_rows_from_some_1Dtable(ui->table_1D_AND_gates, cond->item_1D_AND, "Wyjasnienie");
    redisplay_table_1D_AND();
}
//***************************************************************************************************************
void T4user_condition_wizard::on_pushButton_1D_AND_set_many_cells_clicked()
{
    bool ok = false;
    //double value = ui->table_1D_AND_gates->text(r, c).toDouble(&ok);

    double res = QInputDialog::getDouble( this,
                                         "Minimum value of the gate", "Enter a decimal number:", 0,
                                         -2147483647,
                                         2147483647,
                                         6,
                                         &ok);

    if ( ok )
    {
        // user entered something and pressed OK
        for(int r = 0 ; r < ui->table_1D_AND_gates->rowCount() ; r++)
        {
            for(int c = 2 ; c < ui->table_1D_AND_gates->columnCount() ; c++)
            {
                if( ui->table_1D_AND_gates-> item (r, c )->isSelected())
                {
                    if(c == 2) cond->item_1D_AND[r].min_value = res ;
                    if(c == 3) cond->item_1D_AND[r].max_value = res ;
                }
            }
        }
        redisplay_table_1D_AND();
    }
}
//***************************************************************************************************************
void T4user_condition_wizard::on_table_1D_AND_gates_cellClicked(int row, int column)
{
    //  cout << "------------ F.  T4user_condition_wizard::on_table_1D_AND_gates_cellClicked row=" << row
    //       << ", column= " << column << endl;
    if(column == 0)
    {
        // change the text in
        ui->table_1D_AND_gates->item(row, column)->setText(
            (ui->table_1D_AND_gates->item(row, column)->checkState() == Qt::Checked) ?
                consider_false :consider_true); // ???????

        ui->table_1D_AND_gates->resizeColumnsToContents();
    }
}
//***************************************************************************************************************
void T4user_condition_wizard::on_pushButton_add_2D_OR_item_clicked()
{
    //  int nr_rows = ui->table_2D_OR_gates->rowCount () ;
    //  ui->table_2D_OR_gates->setRowCount (++nr_rows);
    Tcondition_description_cracow::Titem_2D  item ;
    item.var_x_name = double_click_here ;
    item.var_y_name = double_click_here ;
    item.polygon_name = double_click_here ;
    item.obligatory_in_event = false ;

    cond->item_2D_OR.push_back(item) ;

    redisplay_table_2D_OR();
    // after this above !, not before
    ui->table_2D_OR_gates->selectRow( cond->item_2D_OR.size() ) ;
}
//***************************************************************************************************************
void T4user_condition_wizard::on_pushButton_remove_2D_OR_row_clicked()
{
    remove_selected_rows_from_some_2Dtable(ui->table_2D_OR_gates, cond->item_2D_OR, "Wyjasnienie");
    redisplay_table_2D_OR();
}
//***************************************************************************************************************
void T4user_condition_wizard::on_checkBox_short_polygon_names_OR_clicked(bool /*checked*/ )
{
    redisplay_table_2D_OR();
}
//***************************************************************************************************************
void T4user_condition_wizard::on_table_2D_OR_gates_cellDoubleClicked(int row, int column)
{
    switch(column)
    {
    default:
    case 0:
        //change_var_x();
        break;

    case 1:
        change_var_x_OR(row);
        break;

    case 2:
        change_var_y_OR(row);
        break;

    case 3:
        change_polygon_OR(row);
        break;
    }// end switch

    //redisplay_table_1D();
}
//***************************************************************************************************************
void T4user_condition_wizard::on_pushButton_add_2D_AND_item_clicked()
{
    //  int nr_rows = ui->table_2D_AND_gates->rowCount () ;
    //  ui->table_2D_AND_gates->setRowCount (++nr_rows);
    Tcondition_description_cracow::Titem_2D  item ;
    item.var_x_name = double_click_here ;
    item.var_y_name = double_click_here ;
    item.polygon_name = double_click_here ;
    item.obligatory_in_event = true ;

    cond->item_2D_AND.push_back(item) ;

    redisplay_table_2D_AND();
    ui->table_2D_AND_gates->selectRow( cond->item_2D_AND.size() ) ;
}
//***************************************************************************************************************
void T4user_condition_wizard::on_pushButton_remove_2D_AND_row_clicked()
{
    remove_selected_rows_from_some_2Dtable(ui->table_2D_AND_gates, cond->item_2D_AND, "Wyjasnienie");
    redisplay_table_2D_AND();
}
//***************************************************************************************************************
void T4user_condition_wizard::on_checkBox_short_polygon_names_AND_clicked(bool /*checked*/)
{
    redisplay_table_2D_AND();
}
//***************************************************************************************************************
void T4user_condition_wizard::on_table_2D_AND_gates_cellDoubleClicked(int row, int column)
{
    switch(column)
    {
    default:
    case 0:
        //change_var_x();
        break;

    case 1:
        change_var_x_AND(row);
        break;

    case 2:
        change_var_y_AND(row);
        break;

    case 3:
        change_polygon_AND(row);
        break;
    }// end switch

    //redisplay_table_1D();
}
//***************************************************************************************************************
void T4user_condition_wizard::on_table_2D_AND_gates_cellClicked(int row, int column)
{
    //cout << "F. T4user_condition_wizard::on_table_2D_AND_gates_cellClicked" << endl;
    if(column == 0)
    {
        // change the text in
        ui->table_2D_AND_gates->item(row, column)->setText(
            (ui->table_2D_AND_gates->item(row, column)->checkState() == Qt::Checked) ?
                consider_false :consider_true); // ???????

        ui->table_2D_AND_gates->resizeColumnsToContents();
    }
}
//***************************************************************************************************************
void T4user_condition_wizard::on_table_other_conditions_AND_cellClicked(int /*row*/, int /*column*/)
{
    cout << "F. T4user_condition_wizard::on_table_other_conditions_AND_cellClicked" << endl;
}
//***************************************************************************************************************
void T4user_condition_wizard::add_external_condition_to_the_nodes_table(vector <string> &wekt, [[maybe_unused]] string explanation)
{
    // cout << "F. "  << __func__ << endl;

    // open dialog with the existing conditions
    //cout << "edycja cond selected " << endl;
    QDir katalog(gpath.Qconditions() ) ;
    QStringList lista_cond = katalog.entryList ( QStringList(user_cond_filter.c_str()) ) ;




    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("conditions (*.cnd)"));
    dialog.setFileMode(QFileDialog::ExistingFiles);
    QStringList fileNames;
    if (dialog.exec())
        fileNames = dialog.selectedFiles();
    //-------------------------------

    //bool ok;
    // QString selected_name = QInputDialog::getItem(this,
    //                                                  "Adding external condtiion name",
    //                                                  explanation.c_str(), // "Select an existing condition:",
    //                                                  lista_cond,
    //                                                  0,
    //                                                  false, &ok
    //                                                  );

    // user selected some item and pressed OK
    if ( fileNames.count() > 0)
    {
        for(QString selected_name : fileNames)
        {
            //            cout << "name = " <<
            //                    selected_name.toStdString()
            //                 << endl;
            int where = selected_name.lastIndexOf('/', -1, Qt::CaseSensitive);
            selected_name.remove(0, where+1);
            //            cout << "where = " << where
            //                 << " name = "
            //                 << selected_name.toStdString()
            //                 << endl;

            wekt.push_back(selected_name.toStdString() ) ;
            redisplay_tables_nodes();
        }
    }
}
//*********************************************************************************************************
void T4user_condition_wizard::on_toolButton_nodes_AND_add_clicked()
{
    add_external_condition_to_the_nodes_table(cond->other_condition_AND,
                                              "Select an external condtiion to be placed on the AND list");
}
//*********************************************************************************************************
void T4user_condition_wizard::on_toolButton_nodes_OR_add_clicked()
{
    add_external_condition_to_the_nodes_table(cond->other_condition_OR,
                                              "Select an external condtiion to be placed on the OR list");
}
//*********************************************************************************************************

void T4user_condition_wizard::on_toolButton_nodes_NAND_add_clicked()
{
    add_external_condition_to_the_nodes_table(cond->other_condition_NAND,
                                              "Select an external condtiion to be placed on the NAND list");
}
//*********************************************************************************************************

void T4user_condition_wizard::on_toolButton_nodes_NOR_add_clicked()
{
    add_external_condition_to_the_nodes_table(cond->other_condition_NOR,
                                              "Select an external condtiion to be placed on the NOR list");
}
//***************************************************************************************************************
//***************************************************************************************************************
void T4user_condition_wizard::on_toolButton_nodes_AND_remove_clicked()
{
    remove_other_cond_name_from_table( ui->table_other_conditions_AND, "AND",
                                      cond->other_condition_AND);
}
//***************************************************************************************************************
void T4user_condition_wizard::on_toolButton_nodes_OR_remove_clicked()
{
    remove_other_cond_name_from_table( ui->table_other_conditions_OR, "OR",
                                      cond->other_condition_OR);
}
//***************************************************************************************************************
void T4user_condition_wizard::on_toolButton_nodes_NAND_remove_clicked()
{
    remove_other_cond_name_from_table( ui->table_other_conditions_NAND, "NAND",
                                      cond->other_condition_NAND);
}
//***************************************************************************************************************
void T4user_condition_wizard::on_toolButton_nodes_NOR_remove_clicked()
{
    remove_other_cond_name_from_table( ui->table_other_conditions_NOR, "NOR",
                                      cond->other_condition_NOR);
}
//***************************************************************************************************************
bool T4user_condition_wizard::validateCurrentPage()
{
    switch(currentId() )
    {
    default:
    case page__name:
    {

        string name = ui->lineEdit_condition_name->text().toStdString() ;
        //cout << "Now the name is ->" << name << "<- " << endl ;

        if(name.size() <= 0)
        {
            //cout << "Now enabled is disabled " << endl ;
            //setNextEnabled ( currentPage (), false ) ;

            showWarningMessage(
                                     "Error in the name of condition",
                                     "To go to the next page, you need to specify the name of the condition\n",
                                     QMessageBox::Critical );
            return false;
        }

        // if the condtion name contains the space, we replace it with an underscore
        bool flag_space_was_changed = false;
        while(1)
        {
            string::size_type pos = name.find (" ",0) ;
            if(pos == string::npos) break ; // no spaces
            name[pos] = char('_') ; // replace the space with the underscore

            flag_space_was_changed = true;
        }
        if(flag_space_was_changed)
        {
            ui->lineEdit_condition_name->setText(name.c_str()) ;
            showWarningMessage(
                                     "Error in the name of condition",
                                     "In the name of the conditon you included one or more space character. \n"
                                     "This is not allowed, so they were changed into underscores\n",
                                     QMessageBox::Critical);
        }
        return true;
    } // endof case

    case page__5possibilities:
        user_wants_1D_OR = ui->checkBox_OR_gates->checkState();
        user_wants_1D_AND = ui->checkBox_AND_gates->checkState();
        user_wants_2D_OR = ui->checkBox_OR_bananas->checkState();
        user_wants_2D_AND = ui->checkBox_AND_bananas->checkState();
        break;
    case page__2D_OR:
        return check_double_click_here(ui->table_2D_OR_gates);
        break;
    case page__2D_AND:
        return check_double_click_here(ui->table_2D_AND_gates);
        break;

    } // switch

    // anything else
    return true;   // true means: allow to go to the next page
}

//***************************************************************************************************************
void T4user_condition_wizard::on_table_1D_OR_gates_cellDoubleClicked(int row, int column)
{
    //    int row =  ui->table_1D_OR_gates-> currentRow() ;
    //    int column = ui->table_1D_OR_gates-> currentColumn() ;

    switch(column)
    {
    default:
    case 0:
    {

    }
    break;
    case 1:
    {
        T4incrementer_selector *dlg  = new T4incrementer_selector ;
        dlg->setWindowTitle("List of variables on which you may set gates");
        if(dlg->exec() == QDialog::Accepted)
        {

            QStringList lista = dlg->give_seleced_items() ;
            for ( QStringList::Iterator it = lista.begin(); it != lista .end(); ++it )
            {
                //cout << *it << ":";
                Tcondition_description_cracow::Titem_1D  item ;
                cond->item_1D_OR[row].var_name = (*it).toStdString() ;
            }
        }
    }
    break;
    case 2:
    {
        bool ok;
        double value = ui->table_1D_OR_gates->item(row, column)->text().toDouble(&ok);

        double res = QInputDialog::getDouble( this ,
                                             "Minimum value of the gate", "Enter a decimal number:", value,
                                             -2147483647,
                                             2147483647,
                                             6,
                                             &ok);
        if ( ok ) {
            // user entered something and pressed OK
            cond->item_1D_OR[row].min_value = res ;
        }
    }
    break;

    case 3:
    {
        bool ok;
        double value = ui->table_1D_OR_gates->item(row,column)->text().toDouble(&ok);

        double res = QInputDialog::getDouble( this ,
                                             "Maximum value of the gate", "Enter a decimal number:", value,
                                             -2147483647,
                                             2147483647,
                                             6,
                                             &ok);
        if ( ok ) {
            // user entered something and pressed OK
            cond->item_1D_OR[row].max_value = res ;
        }
    }
    break;
    }// end switch

    redisplay_table_1D_OR();
    raise();   // to the top of desktop


}
//***************************************************************************************************************
void T4user_condition_wizard::on_table_1D_AND_gates_cellDoubleClicked(int row, int column)
{
    //  int row =  ui->table_1D_AND_gates-> currentRow() ;
    //  int column = ui->table_1D_AND_gates-> currentColumn() ;

    switch(column)
    {
    default:
    case 0:
    {
        //       cout << "Double click on 'when to test'..." << endl;
        // cond->item_1D_AND[r].obligatory_in_event =
        // dynamic_cast<QComboTableItem*> (ui->table_1D_AND_gates->item(r, c))->currentItem();
    }
    break;

    case 1:
    {
        T4incrementer_selector *dlg  = new T4incrementer_selector ;
        dlg->setWindowTitle("List of variables on which you may set gates");
        if(dlg->exec() == QDialog::Accepted)
        {

            QStringList lista = dlg->give_seleced_items() ;
            for ( QStringList::Iterator it = lista.begin(); it != lista .end(); ++it )
            {
                //cout << *it << ":";
                Tcondition_description_cracow::Titem_1D  item ;

                // check if this name contains the keyword _WHEN_
                //
                if( ((*it)).contains("_WHEN_", Qt::CaseInsensitive))  // case unsensitive
                {
                    T4addline_1d_wizard * dlg2 = new T4addline_1d_wizard;
                    dlg2->set_incrementor(*it);
                    if(dlg2->exec() == QDialog::Accepted)
                    {
                        cond->item_1D_AND[row].obligatory_in_event = dlg2->give_obligatory() ;
                        cond->item_1D_AND[row].var_name = (*it).toStdString() ;
                    }
                    else   // canceled
                    {
                        redisplay_table_1D_AND();
                        raise();   // to the top of desktop
                        return ;
                    }
                }
                else // now keyword WHEN
                {
                    cond->item_1D_AND[row].var_name = (*it).toStdString() ;
                }

            }
        }
    }
    break;
    case 2:
    {
        bool ok;
        double value = ui->table_1D_AND_gates->item(row, column)->text().toDouble(&ok);

        double res = QInputDialog::getDouble(this,
                                             "Minimum value of the gate", "Enter a decimal number:", value,
                                             -2147483647,
                                             2147483647,
                                             6,
                                             &ok);
        if ( ok ) {
            // user entered something and pressed OK
            cond->item_1D_AND[row].min_value = res ;
        }
    }
    break;

    case 3:
    {
        bool ok;
        double value = ui->table_1D_AND_gates->item(row,column)->text().toDouble(&ok);

        double res = QInputDialog::getDouble(this ,
                                             "Maximum value of the gate", "Enter a decimal number:", value,
                                             -2147483647,
                                             2147483647,
                                             6,
                                             &ok);
        if ( ok ) {
            // user entered something and pressed OK
            cond->item_1D_AND[row].max_value = res ;
        }
    }
    break;
    }// end switch

    if(column != 0) redisplay_table_1D_AND();
    raise();   // to the top of desktop
}
//***************************************************************************************************************
void T4user_condition_wizard::on_checkBox_OR_gates_clicked(bool checked)
{
    if(!checked)
    {
        if(ui->table_1D_OR_gates->rowCount())
        {
            // You can not disable this page, because now its table has some contents
            ui->checkBox_OR_gates->setChecked(true);
            showWarningMessage(
                                 "No spectrum selected",
                                 QString ( "You can not disable this page, because now its table has already some contents" )
                                 );
        }
    }
}
//***************************************************************************************************************
void T4user_condition_wizard::on_checkBox_AND_gates_clicked(bool checked)
{
    if(!checked)
    {
        if(ui->table_1D_AND_gates->rowCount())
        {
            // You can not disable this page, because now its table has some contents
            ui->checkBox_AND_gates->setChecked(true);
            showWarningMessage(
                                 "No spectrum selected",
                                 QString ( "You can not disable this page, because now its table has already some contents" ));
        }
    }
}
//***************************************************************************************************************
void T4user_condition_wizard::on_checkBox_OR_bananas_clicked(bool checked)
{
    if(!checked)
    {
        if(ui->table_2D_OR_gates->rowCount())
        {
            // You can not disable this page, because now its table has some contents
            ui->checkBox_OR_bananas->setChecked(true);
           showWarningMessage(
                                 "No spectrum selected",
                                 QString ( "You can not disable this page, because now its table has already some contents" )
                                 );
        }
    }
}
//***************************************************************************************************************
void T4user_condition_wizard::on_checkBox_AND_bananas_clicked(bool checked)
{
    if(!checked)
    {
        if(ui->table_2D_AND_gates->rowCount())
        {
            // You can not disable this page, because now its table has some contents
            ui->checkBox_AND_bananas->setChecked(true);
            showWarningMessage(
                                 "No spectrum selected",
                                 QString ( "You can not disable this page, because now its table has already some contents" ));
        }
    }
}
//***************************************************************************************************************
void T4user_condition_wizard::on_checkBox_external_conditions_clicked(bool checked)
{
    if(!checked)
    {
        if(ui->table_other_conditions_AND->rowCount() ||
            ui->table_other_conditions_OR->rowCount() ||
            ui->table_other_conditions_NAND->rowCount() ||
            ui->table_other_conditions_NOR->rowCount()
            )
        {
            // You can not disable this page, because now its table has some contents
            ui->checkBox_external_conditions->setChecked(true);
            showWarningMessage(
                                 "No spectrum selected",
                                 QString ( "You can not disable this page, because now some of its tables have already some contents" ) );
            return;
        }
    }
    user_wants_external_nodes = checked;
}
//***************************************************************************************************************
// checking if the user forgot putting something in a column marked as "double click here"
int T4user_condition_wizard::check_double_click_here(QTableWidget *table_ptr )
{

    bool flag_ok = true;
    string info;
    vector<int> row_to_remove;
    vector<Tcondition_description_cracow::Titem_2D>  *cond_ptr = nullptr;

    if(table_ptr == ui->table_2D_OR_gates){
        cond_ptr = &cond->item_2D_OR;
    }
    else if(table_ptr == ui->table_2D_AND_gates){
        cond_ptr = &cond->item_2D_AND;
    }
    else return true ;

    // table_2D_OR_gates

    int nr_rows =cond_ptr->size();
    table_ptr->setRowCount (nr_rows);
    int nr_columns = table_ptr->columnCount ();

    for(int r = 0 ; r < nr_rows ; r++)
        for(int c = 0 ; c < nr_columns ; c++)
        {
            // table_ptr->setItem(r,c, new QTableWidgetItem("") );
            if ( table_ptr->item(r, c)->text() == double_click_here.c_str())
            {
                info += "In the table  in a raw = " + to_string( r+1)  +
                        ", (column = " + to_string( c) + ")\n\tyou left a text -> " + double_click_here + "\n";
                flag_ok = false;
                row_to_remove.push_back(r);
                break; // if there are more in this row, they will be removed also
            }

        }

    if(flag_ok == false)
    {
        info += "\n\n Should I remove these 'unfinished' entries:";
        if( QMessageBox::question(this, "What to do", info.c_str(),
                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel) == QMessageBox::Yes)
        {
            for(auto x : row_to_remove)
            {
                table_ptr->removeRow(x);
                cond_ptr->erase(cond_ptr->begin() + x);
            }
            return false; // stay on this page, to see that it was removed
        }
    }
    else
        return true; // go to the next

    return true; // not error situation
}
//***************************************************************************************************************
