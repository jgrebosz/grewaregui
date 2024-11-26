#include "t4incrementer_selector.h"
#include "ui_t4incrementer_selector.h"
#include "paths.h"
#include <fstream>
#include <QMessageBox>
#include <QDir>
#include <iostream>
using namespace std;


QString T4incrementer_selector::last_filter;
//*********************************************************************************************
T4incrementer_selector::T4incrementer_selector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4incrementer_selector)
{
    ui->setupUi(this);

    //cout << "init()" << endl;

    ui->listBox_filtered->clear();
    ui->lineEdit_filter->setText(last_filter);
    ui->lineEdit_filter->selectAll ();


    QString nam=  gpath.Qcommands() + "list_of_variables.txt" ;

    ifstream plik(nam.toStdString().c_str());
    if(!plik)
    {
        QString mess = "I can not open the file "
                + nam
                + "to read the list of possible incrementors. "
                "Are you sure that the path to the spy spectra is correct ?";

        QMessageBox::critical( this, " List of possible variables not found",
                               mess,
                               QMessageBox::Ok | QMessageBox::Default,
                               QMessageBox::NoButton,
                               QMessageBox::NoButton);  // error
        return ;
    }

    string one_name ;
    while(plik)
    {
        plik >> one_name ;
        if(plik)
        {
            list_of_all_increm += one_name.c_str() ;
            ui->listBox_filtered->addItem( one_name.c_str()) ;
        }
    };
    add_all_user_incrementers_to_the_list() ;
    //**  put all on the list
    filter_changed();
}
//***********************************************************************************************
T4incrementer_selector::~T4incrementer_selector()
{
    delete ui;
}
//***********************************************************************************************
void T4incrementer_selector::filter_changed()
{
    //cout << "filter_changed()" << endl;

    QString filter = ui->lineEdit_filter->text();
    last_filter = filter;
    // separate the * parts

    QStringList found = list_of_all_increm.filter(
//                QRegExp(filter, Qt::CaseInsensitive)
                QRegExp ( filter, Qt::CaseInsensitive,QRegExp::Wildcard )
                );

    ui->listBox_filtered->clear();
    ui->listBox_filtered->addItems(found) ;

}
//**********************************************************************************************
QStringList T4incrementer_selector::give_seleced_items()
{
    QStringList chosen ;

    //    ui->listBox_filtered->  firstItem() ;

    for(int i = 0 ; i < ui->listBox_filtered-> count() ; i++)
    {
        if(ui->listBox_filtered->item(i)->isSelected())
        {
            chosen += ui->listBox_filtered->item(i)->text();
        }
    }
    return chosen ;
}
//**********************************************************************************************
QStringList T4incrementer_selector::give_filtered_items()
{
	QStringList chosen ;

	//    ui->listBox_filtered->  firstItem() ;

	for(int i = 0 ; i < ui->listBox_filtered-> count() ; i++)
	{
			chosen += ui->listBox_filtered->item(i)->text();
	}
	return chosen ;
}
//**********************************************************************************************
void T4incrementer_selector::set_multi_selection( bool multi )
{
    ui->listBox_filtered->setSelectionMode(multi ?
                                               QAbstractItemView::MultiSelection :
                                               QAbstractItemView::SingleSelection) ;

    ui->textLabel_one_more->setText(multi ?  "Select one or more items" :
                                             "Select only one item");
}
//*********************************************************************************************
void T4incrementer_selector::change_title( QString str )
{
    setWindowTitle(str);
}
//************************************************************************************
void T4incrementer_selector::make_incremeter_ALL_impossible()
{
    //cout << "make_incremeter_ALL_impossible()" << endl;

    // removing he ALL* incrementors
    for ( QStringList::Iterator it = list_of_all_increm.begin() ; it != list_of_all_increm.end() ;  ++it )
    {
        //cout << *it << "\n";
        QString corected_string = *it ;
        if(corected_string.lastIndexOf( "ALL_") != -1)  // found
        {
            *it =  QString( "NOT POSSIBLE NOW ->"  + corected_string);
            //cout << "Corrected as "  << *it << endl;
        }
    } // end for
    ui->textLabel_one_more->setText("Select, some incrementer, but do not use so called 'ALL...'");
    filter_changed();
}
//**************************************************************************************************
void T4incrementer_selector::add_all_user_incrementers_to_the_list()
{
    QDir katalog2(gpath.Qincrementers() );

    const string user_incr_ext2 { ".incr" };
    const string user_incr_filter2 { "*" + user_incr_ext2 };


    QString name_filter =   user_incr_filter2.c_str();
    //how_many_items = katalog2.count() ;
    QStringList lista = katalog2.entryList (QStringList( name_filter)) ;

    for ( auto x : lista )
    {
        string name_c = x.toStdString();
        int pos = name_c.rfind(".") ;
        name_c.erase(pos) ;
//        cout << "Increment to be added: " << name_c << endl;
        list_of_all_increm += name_c.c_str() ;
    }
    list_of_all_increm.removeDuplicates();
    list_of_all_increm.sort();

}
//************************************************************************************
//void T4incrementer_selector::on_buttonOk_clicked()
//{

//}
//************************************************************************************
void T4incrementer_selector::on_lineEdit_filter_textChanged(const QString &filter)
{
    last_filter = filter;
    QStringList found = list_of_all_increm.filter(QRegExp(filter, Qt::CaseInsensitive, QRegExp::Wildcard)  );

    ui->listBox_filtered->clear();
    ui->listBox_filtered->addItems(found) ;
}
