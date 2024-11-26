#include "T4user_incrementer_wizard.h"
#include "ui_T4user_incrementer_wizard.h"

#include <qdir.h>
#include <qstringlist.h>
#include <qinputdialog.h>

#include "t4incrementer_selector.h"
#include <sstream>
using namespace std;

extern bool allow_incrementers ;

#include <QMessageBox>

//**************************************************************************
T4user_incrementer_wizard::T4user_incrementer_wizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::T4user_incrementer_wizard)
{
    ui->setupUi(this);
    //    setTitle ("Tytul");
}
//**************************************************************************
T4user_incrementer_wizard::~T4user_incrementer_wizard()
{
    delete ui;
}
//**************************************************************************
void T4user_incrementer_wizard::init()
{
    //helpButton () ->hide();
    ui->radioButton_increm_blue->setChecked(true);
    //setAppropriate(QWizard::page(5), allow_incrementers);
}
//**************************************************************************
void T4user_incrementer_wizard::set_parameters( Tincrementer_description_cracow *increm_ptr )
{
    increm = increm_ptr ;
    // ui->lineEdit_incrementer_name->setText(increm->name.c_str());

    ui->lineEdit_incrementer_name->setText(increm->name.c_str() );

    // expression green
    ui->push_increm_green->setText(increm->other_incrementer_nameA.c_str());

    // setting on the screen the blue (second) expression

    ui->radioButton_increm_blue->setChecked(!increm->flag_exprB_is_constant);
    ui->radioButton_value->setChecked(increm->flag_exprB_is_constant);

    ui->push_increm_blue->setText(increm->other_incrementer_nameB.c_str());

    ostringstream s;
    s << increm->constant_valueB;
    ui->lineEdit_const_value->setText(s.str().c_str() );

    ui->comboBox_operation->setCurrentIndex(increm->operation   -1); // plus, minus, times, divide

}
//**************************************************************************
void T4user_incrementer_wizard::nextId()
{


    switch (currentId())
    {
    case     page__name_of_incrementer:
    {
        string name = ui->lineEdit_incrementer_name->text().toStdString();
        //cout << "Now the name is ->" << name << "<- " << endl ;

        if(name.size() <= 0)
        {
            //cout << "Now enabled is disabled " << endl ;
            //setNextEnabled ( currentPage (), false ) ;

            QMessageBox::information( this,
                                      "Error in the name of incrementer",
                                      "To go to the next page, you need to specify the name of the incrementer\n",
                                      QMessageBox::Ok,
                                      QMessageBox::NoButton,
                                      QMessageBox::NoButton);
            return ;
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
            ui->lineEdit_incrementer_name->setText(name.c_str()) ;
            QMessageBox::information( this,
                                      "Error in the name of incrementer",
                                      "In the name of the conditon you included one or more space character. \n"
                                      "This is not allowed, so they were changed into underscores\n",
                                      QMessageBox::Ok,
                                      QMessageBox::NoButton,
                                      QMessageBox::NoButton);
        }
    }
        break;

    case page__result:
        //setFinishEnabled ( currentPage (), true) ;
        break;

    } // switch





    // general settings
    // setHelpEnabled (currentPage (), false) ;
    // helpButton () ->hide();


}
//**************************************************************************
void T4user_incrementer_wizard::accept()
{
    /*
       bool flag_exprB_is_constant;
        string other_incrementer_nameA ;
        double constant_valueB;

        int operation; // plus, minus, times, divide
        string other_incrementer_nameB ;
  */

    increm->name = ui->lineEdit_incrementer_name->text().toStdString();

    // expression A
    increm->flag_exprB_is_constant =  !ui->radioButton_increm_blue->isChecked();
    increm->other_incrementer_nameB = ui->push_increm_blue->text().toStdString() ;
    increm->constant_valueB = ui->lineEdit_const_value->text().toDouble();

    increm->operation =   ui->comboBox_operation->currentIndex() + 1; // plus, minus, times, divide

    // expression B
    increm->other_incrementer_nameA = ui->push_increm_green->text().toStdString() ;

    QDialog::accept() ;
}
//**************************************************************************
void T4user_incrementer_wizard::name_changed()
{

    string name = ui->lineEdit_incrementer_name->text().toStdString() ;

    int len = name.size() ;
    len = min(len, 5) ;
    if(name.substr(0, len) != string("user_").substr(0, len))
    {
        name.insert(0, "user_") ;
        ui->lineEdit_incrementer_name->setText(name.c_str()) ;
    }

    //cout << "Now the name is ->" << name << "<- " << endl ;
    if(name.size() < 5)
    {

        ui->lineEdit_incrementer_name->setText("user_") ;
        //cout << "Now is disabled " << endl ;
        //setNextEnabled ( currentPage (), false ) ;
    }






    //  string name = ui->lineEdit_incrementer_name->text() ;
    //
    //  int len = name.size() ;
    //  len = min(len, 5) ;
    //  if(name.substr(0, len) != string("cond_").substr(0, len))
    //  {
    //    name.insert(0, "cond_") ;
    //    ui->lineEdit_incrementer_name->setText(name) ;
    //  }
    //
    //  //cout << "Now the name is ->" << name << "<- " << endl ;
    //  if(name.size() < 5)
    //  {
    //       ui->lineEdit_incrementer_name->setText("cond_") ;
    //        //cout << "Now is disabled " << endl ;
    //  //setNextEnabled ( currentPage (), false ) ;
    //  }
}

#ifdef NIGDY
help
{

    int nr_rows =cond->item_2D_AND.size();
    table_2D_AND_gates->setNumRows (nr_rows);


    //setCurrentItem ( int i )
    QStringList list  ;
    list << "consider this line as: TRUE" << "consider this line as: FALSE" ;

    for (int j = 0; j < nr_rows; ++j )
    {

        table_2D_AND_gates->setItem(j, 0, new QComboTableItem( table_1D_AND_gates, list ));
        //QComboTableItem *ptr =
        dynamic_cast<QComboTableItem*> (table_2D_AND_gates->item(j, 0))->
                setCurrentItem ( cond->item_2D_AND[j].obligatory_in_event) ;


        table_2D_AND_gates->setText(j, 1, cond->item_2D_AND[j].var_x_name.c_str() );
        table_2D_AND_gates->setText(j, 2, cond->item_2D_AND[j].var_y_name.c_str()  );

        string pname = cond->item_2D_AND[j].polygon_name ;
        if(checkBox_short_polygon_names_AND->isChecked())
        {
            pname.erase(0, pname.rfind("_polygon_")+9);
        }
        table_2D_AND_gates->setText(j, 3, pname.c_str()  );
    }

    table_2D_AND_gates->adjustColumn(0) ;
    table_2D_AND_gates->adjustColumn(1) ;
    table_2D_AND_gates->adjustColumn(2) ;
    table_2D_AND_gates->adjustColumn(3) ;
}


QString help_message ;
switch(nr_page)
{

    case 0:   // name
        help_message =
                "If you change this name of the incrementer later, note that perhaps there are already "
                "MANY spectra which use this condtion. \n"
                "This is why Jurek decided, that the incrementer with the old name should "
                "still remain on the disk";
        break;

    case 1: case 2:  // 1D variables
        help_message =
                "Here you may define the one dimmensional conditons.\n"
                "1. Add a row to this table\n"
                "2. Choose the variable which wil be tested \n"
                "3. Set the minimum value of the gate\n"
                "4. Set the maximum value of the gate\n\n"
                "Note:\n"
                "You may double click on the item to edit it.\n"
                "You may also click and drag on the table to select (and edit) many values\n"
                "You may click on the header of the column to select all variables in the column\n\n"

                "\"always\" - means that chosen variable is ALWAYS TESTED and must have this value in every event \n"
                "\"test only WHEN legal\" - means that if this variable is TESTED ONLY IF IT HAS A LEGAL VALUE in this event\n\n "
                "For example:\n"
                " - the germanium detector energy data it is legal only when its detector has fired,\n"
                " - multiwire 'position x' is legal only when this detector has fired and if it was possible to calculate position\n"
                " from the data in current event (that means: both 'left' and 'right' signals comming from multiwire chamber \n"
                "were present in this event).\n"
                "\nIf you know when your favourite variable should be 'legal' - "
                "tell this to Jurek, he will set it so in the program\n";



        break;

    case 3: case 4:   // 2D gates
        help_message =
                "Here you may define the two dimmensional conditons.\n"
                "1. Add a row to this table\n"
                "2. Choose the variable which wil be tested as X coordinate\n"
                "3. Choose the variable which wil be tested as Y coordinate\n"
                "4. Chose the poligon which describes two dimmensional gate which both variables "
                "should pass\n\n"
                "Note: you may double click on the item to edit it\n\n"
                "Question: What to do if there is no proper polygon ?\n"
                "Answer: Close the wizard and go to the correct matrix and create the right polygon.\n\n"
                "Question: What to do, if there is no such matrix ?\n"
                "Answer: Create such a matrix as the user defined spectrum and run the SPY. \n"
                "The spy will create such a matrix and will put some data on it. \n"
                "Now you can create the polygon in the right position. \n"
                "After doing this - you may return to the incrementer wizard and \n"
                "use this polygon in the incrementer\n\n"


                "\"always\" - means that chosen variable is ALWAYS TESTED and must have this value in every event \n"
                "\"test only WHEN legal\" - means that if this variable is TESTED ONLY IF IT HAS A LEGAL VALUE in this event\n\n "
                "For example:\n"
                " - the germanium detector energy data it is legal only when its detector has fired,\n"
                " - multiwire 'position x' is legal only when this detector has fired and if it was possible to calculate position\n"
                " from the data in current event (that means: both 'left' and 'right' signals comming from multiwire chamber \n"
                "were present in this event).\n"
                "\nIf you know when your favourite variable should be 'legal' - "
                "tell this to Jurek, he will set it so in the program\n";

        break;

    default:
        help_message = "No explanations about this page";
        break;

}
if(help_message != "")
{
    QMessageBox::information ( this, "Help about setting the incrementers", help_message,
                               QMessageBox::Ok);
}
}



{

int nr_rows =cond->item_2D_OR.size();
table_2D_OR_gates->setNumRows (nr_rows);


//setCurrentItem ( int i )
QStringList list  ;
list << "consider this line as: TRUE" << "consider this line as: FALSE" ;

for (int j = 0; j < nr_rows; ++j )
{

    table_2D_OR_gates->setItem(j, 0, new QComboTableItem( table_1D_OR_gates, list ));
    //QComboTableItem *ptr =
    dynamic_cast<QComboTableItem*> (table_2D_OR_gates->item(j, 0))->
            setCurrentItem ( cond->item_2D_OR[j].obligatory_in_event) ;


    table_2D_OR_gates->setText(j, 1, cond->item_2D_OR[j].var_x_name.c_str() );
    table_2D_OR_gates->setText(j, 2, cond->item_2D_OR[j].var_y_name.c_str()  );

    string pname = cond->item_2D_OR[j].polygon_name ;
    if(checkBox_short_polygon_names_OR->isChecked())
    {
        pname.erase(0, pname.rfind("_polygon_")+9);
    }
    table_2D_OR_gates->setText(j, 3, pname.c_str()  );
}

table_2D_OR_gates->adjustColumn(0) ;
table_2D_OR_gates->adjustColumn(1) ;
table_2D_OR_gates->adjustColumn(2) ;
table_2D_OR_gates->adjustColumn(3) ;
}
#endif

//**************************************************************************
void T4user_incrementer_wizard::choosing_expressionA_incrementer()
{
    T4incrementer_selector *dlg  = new T4incrementer_selector ;
    dlg->setWindowTitle("List of available incrementers");
    if(dlg->exec() == QDialog::Accepted)
    {
        QStringList lista = dlg->give_seleced_items() ;

        // should be only one incrementer,
        // but it can be ALL,
        // or WHEN?

        for ( QStringList::Iterator it = lista.begin(); it != lista .end(); ++it )
        {
            cout << (*it).toStdString() << ":";

            /*
            bool flag_exprB_is_constant;
            string other_incrementer_nameA ;
            double constant_valueB;

            int operation; // plus, minus, times, divide
            string other_incrementer_nameB ;
      */

            increm->other_incrementer_nameA = (*it).toStdString() ;
            ui->push_increm_green->setText((*it)) ;

#ifdef NIGDY
            // check if this name contains the keyword _WHEN_
            //

            if( ((*it)).contains("_WHEN_", false))  // case unsensitive
            {
                Taddline_1D_wizard * dlg2 = new Taddline_1D_wizard;
                dlg2->set_incrementor( item.var_name.c_str() );
                if(dlg2->exec() == QDialog::Accepted)
                {
                    item.obligatory_in_event = dlg2->give_obligatory() ;
                }
                else
                {
                    raise();   // to the top of desktop
                    return ;
                }
            }
#endif // NIGDY

        }
    }

    raise();   // to the top of desktop
}
//*****************************************************************
void T4user_incrementer_wizard::choosing_expressionB_incrementer()
{
    T4incrementer_selector *dlg  = new T4incrementer_selector ;
    dlg->setWindowTitle("List of available incrementers");
    if(dlg->exec() == QDialog::Accepted)
    {
        QStringList lista = dlg->give_seleced_items() ;

        // should be only one incrementer,
        // but it can be ALL,
        // or WHEN?

        for ( QStringList::Iterator it = lista.begin(); it != lista .end(); ++it )
        {
            cout << (*it).toStdString() << ":";
            increm->other_incrementer_nameB = (*it).toStdString() ;
            ui->push_increm_blue->setText((*it)) ;

#ifdef NIGDY
            // check if this name contains the keyword _WHEN_
            //
            if( ((*it)).contains("_WHEN_", false))  // case unsensitive
            {
                Taddline_1D_wizard * dlg2 = new Taddline_1D_wizard;
                dlg2->set_incrementor( item.var_name.c_str() );
                if(dlg2->exec() == QDialog::Accepted)
                {
                    item.obligatory_in_event = dlg2->give_obligatory() ;
                }
                else
                {
                    raise();   // to the top of desktop
                    return ;
                }
            }
#endif

        }
    }

    raise();   // to the top of desktop


}
//*****************************************************************
void T4user_incrementer_wizard::radio_changed( )
{
    ui->push_increm_blue->setEnabled(ui->radioButton_increm_blue->isChecked());
    ui->lineEdit_const_value->setEnabled(ui->radioButton_increm_blue->isChecked());
    ui->lineEdit_const_value->setEnabled(! ui->radioButton_increm_blue->isChecked());
}

void T4user_incrementer_wizard::on_push_increm_green_clicked()
{
    choosing_expressionA_incrementer();
}

void T4user_incrementer_wizard::on_push_increm_blue_clicked()
{
    choosing_expressionB_incrementer();
}
