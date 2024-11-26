#include "t4user_incrementer_wizard.h"
#include "ui_t4user_incrementer_wizard.h"

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

    // To avoid writing this in Polish
    setButtonText(QWizard::NextButton, "Next  >" );
    setButtonText(QWizard::BackButton, "<  Back" );
    setButtonText(QWizard::CancelButton, "Cancel" );
    setButtonText(QWizard::FinishButton, "Finish" );

    //    setTitle ("Tytul");
    ui->lineEdit_const_value_green->setText("0");
    ui->lineEdit_const_value_blue->setText("0");
    PressToChoose = "press_to_choose";
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
    ui->radioButton_blue_is_incrementer->setChecked(true);
    ui->radioButton_green_is_incrementer->setChecked(true);
    //setAppropriate(QWizard::page(5), allow_incrementers);
}
//**************************************************************************
void T4user_incrementer_wizard::set_parameters( Tincrementer_description_cracow *increm_ptr ,
                                                string spectra, string conditions, string incrementers)
{
    increm = increm_ptr ;

    string txt;
    if(spectra.size() )
    {
        txt =   "<html><head/><body><p><span style=\" font-style:italic; color:#585858;\">"
                "Note: Currently this incrementer is used by the following user defined SPECTRA:<br>" +
                spectra + "</span></p></body></html>";

    }
    ui->label_spectra_using->setText(txt.c_str() );

    txt.clear();
    if(conditions.size() )
    {
        txt =
                "<html><head/><body><p><span style=\" font-style:italic; color:#585858;\">"
                "Note: Currently this incrementer is used by the following user defined CONDITIONS:<br>" +
                conditions + "</span></p></body></html>";
    }
    ui->label_conditions_using->setText(txt.c_str() );

    txt.clear();
    if(incrementers.size() )
    {
        txt =
                "<html><head/><body><p><span style=\" font-style:italic; color:#585858;\">"
                "Note: Currently this incrementer is used by the following user defined INCREMENTERS:<br>" +
                incrementers + "</span></p></body></html>";;
    }
    ui->label_other_incrementers_using->setText(txt.c_str() );





    ui->lineEdit_incrementer_name->setText(increm->name.c_str() );

    // expression green -------------------------------------------------------
    ui->push_increm_green->setText(increm->other_incrementer_nameA.c_str());
    ui->radioButton_green_is_constant->setChecked(increm->flag_exprA_is_constant);
    ui->radioButton_green_is_incrementer->setChecked(!increm->flag_exprA_is_constant);
    ostringstream s;
    s << increm->constant_valueA;

    ui->lineEdit_const_value_green->setText(s.str().c_str() );


    // setting on the screen the blue (second) expression ---------------------

    ui->radioButton_blue_is_incrementer->setChecked(!increm->flag_exprB_is_constant);
    ui->radioButton_blue_is_constant->setChecked(increm->flag_exprB_is_constant);

    ui->push_increm_blue->setText(increm->other_incrementer_nameB.c_str());

    ostringstream s2;
    s2 << increm->constant_valueB;
    ui->lineEdit_const_value_blue->setText(s2.str().c_str() );


    ui->comboBox_operation->setCurrentIndex(increm->operation   -1); // plus, minus, times, divide

}
//**************************************************************************
int T4user_incrementer_wizard::nextId()
{
    switch (currentId())
    {
    case page__result:
    {
        //setFinishEnabled ( currentPage (), true) ;
        QString result = "zielone oper blue";
        ui->label_result_incrementer->setText(result);
        break;
    }

    default:
        break;
    } // switch
    return 0;
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

    // expression A green
    increm->other_incrementer_nameA = ui->push_increm_green->text().toStdString() ;
    increm->constant_valueA = ui->lineEdit_const_value_green->text().toDouble();
    increm->flag_exprA_is_constant =  !ui->radioButton_green_is_incrementer->isChecked();


    // expression B blue
    increm->flag_exprB_is_constant =  !ui->radioButton_blue_is_incrementer->isChecked();
    increm->other_incrementer_nameB = ui->push_increm_blue->text().toStdString() ;
    increm->constant_valueB = ui->lineEdit_const_value_blue->text().toDouble();

    increm->operation =   ui->comboBox_operation->currentIndex() + 1; // plus, minus, times, divide


    QDialog::accept() ;
}
//**************************************************************************
//***********************************************************************************************
void T4user_incrementer_wizard::on_lineEdit_incrementer_name_textChanged(const QString &arg1)
{
    string name = arg1.toStdString() ;

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
}
//*****************************************************************************************************
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
    ui->push_increm_blue->setEnabled(ui->radioButton_blue_is_incrementer->isChecked());
    ui->lineEdit_const_value_blue->setEnabled(ui->radioButton_blue_is_incrementer->isChecked());
    //    ui->lineEdit_const_value_green->setEnabled(! ui->radioButton_green_is_incrementer->isChecked());
}
//*****************************************************************
void T4user_incrementer_wizard::on_push_increm_green_clicked()
{
    choosing_expressionA_incrementer();
}
//*****************************************************************
void T4user_incrementer_wizard::on_push_increm_blue_clicked()
{
    choosing_expressionB_incrementer();
}
//*****************************************************************
bool T4user_incrementer_wizard::validateCurrentPage()
{
    //    cout << "F. validateCurrentPage "
    //         << currentPage()->title().toStdString()  << endl;

    // || name == PressToChoose

    switch(currentId() )
    {
    case     page__name_of_incrementer:
    {
        string name = ui->lineEdit_incrementer_name->text().toStdString();
        //cout << "Now the name is ->" << name << "<- " << endl ;

        if(name.size() <= 0  )
        {
            //cout << "Now enabled is disabled " << endl ;
            //setNextEnabled ( currentPage (), false ) ;

            QMessageBox::information( this,
                                      "Error in the name of incrementer",
                                      "To go to the next page, you need to specify the name of the incrementer\n",
                                      QMessageBox::Ok,
                                      QMessageBox::NoButton,
                                      QMessageBox::NoButton);
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
            ui->lineEdit_incrementer_name->setText(name.c_str()) ;
            //                QMessageBox::information( this,
            //                                          "Error in the name of incrementer",
            //                                          "In the name of the conditon you included one or more space character. \n"
            //                                          "This is not allowed, so they were changed into underscores\n",
            //                                          QMessageBox::Ok,
            //                                          QMessageBox::NoButton,
            //                                          QMessageBox::NoButton);
        }
        return true;
    }
        break;

    case page__expression_green:
        if( ! ui->radioButton_green_is_incrementer->isChecked())
        {
            green = ui->lineEdit_const_value_green->text().toStdString();
            donnor = "none";

        }else
        {
            if(ui->push_increm_green->text().toStdString() == PressToChoose)
            {
                QMessageBox::warning( this,
                                      "Error in the name of incrementer",
                                      "Please select the expresion green incrementer\n",
                                      QMessageBox::Ok,
                                      QMessageBox::NoButton,
                                      QMessageBox::NoButton);
                return false;
            }
            donnor = green = ui->push_increm_green->text().toStdString();
        }
        return true;

    case page__expression_blue:
    {
        oper = ui->comboBox_operation->currentText().toStdString() ;
        blue = ui->push_increm_blue->text().toStdString();

        increm->constant_valueB = ui->lineEdit_const_value_blue->text().toDouble();

        if( ! ui->radioButton_blue_is_incrementer->isChecked())
        {
            blue = ui->lineEdit_const_value_blue->text().toStdString();
            // division by 0?????????
            istringstream s(blue);
            double d;
            s >> d;
            if(d< 0.0000001 && d > -0.0000001 && oper == "/")
            {
                QMessageBox::warning( this,
                                      "Error in the logic of the operation",
                                      "Yoor selected the operation 'Division' by 0.\n"
                                      "It would crash the spy. \nPlease correct it",
                                      QMessageBox::Ok,
                                      QMessageBox::NoButton,
                                      QMessageBox::NoButton);
                return false;

            }



        }else
        {
            if(blue == PressToChoose)
            {
                QMessageBox::warning( this,
                                      "Error in the name of incrementer",
                                      "Please select the expresion blue incrementer\n",
                                      QMessageBox::Ok,
                                      QMessageBox::NoButton,
                                      QMessageBox::NoButton);
                return false;
            }
            if(ui->radioButton_green_is_incrementer->isChecked() == false) donnor = blue;
        }

        string total = // green + "    " + oper + "    " + blue ;

                "<font size=\"+1\"><b><i>" +

                ui->lineEdit_incrementer_name->text().toStdString()
                +
                " = <br>     <font color=\"#005500\">" + green +
                "</font>    <font color=\"#ff0000\">" +
                oper +
                "</font>   <font color=\"#0000ff\">" +
                blue +
                "</font></i></b></font>" ;


        ui->label_result_incrementer->setText(total.c_str());


        string ownership_info =
                "<html><head/><body><p><span style=\" font-size:small; font-style:italic; color:#005500;\">"
                "Your new incrementer will be 'owned' by <br/>the same detector as an incrementer: "
                +donnor+
                "<br><br>(This information is important only if you want to use this incrementer to "
                "increment a matrix) </span></p></body></html>";

        ui->textLabel_ownership->setText(ownership_info.c_str() );
        return true;
    }
        break;


        //--------------------
    default :
        return true;

    } // end switch

    cout << "This should never happen " << endl;
    return false;
}
//*******************************************************************************************************
void T4user_incrementer_wizard::on_lineEdit_incrementer_name_cursorPositionChanged(int /*arg1*/, int arg2)
{
    //cout << "cursor pos " << arg1 << " arg2 = " << arg2 << endl;
    if(arg2 < 5)   // size of prefix "user_"
        ui->lineEdit_incrementer_name->setCursorPosition(5);
}
