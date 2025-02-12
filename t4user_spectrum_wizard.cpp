#include "t4user_spectrum_wizard.h"
#include "ui_t4user_spectrum_wizard.h"
#include <QMessageBox>
#include <QDir>
#include <QInputDialog>
#include <iostream>
#include <sstream>
#include "t4incrementer_selector.h"
#include "paths.h"
#include "t4selfgate_ger_dlg.h"
#include "Tfile_helper.h"
#include "t4selfgate_choice_dlg.h"
#include "tselfgate_type.h"
using TSG = Tselfgate_type ;
#include "t4selfgate_hec_dlg.h"
#include "Tself_gate_hec_descr.h"
#include "Tself_gate_mib_descr.h"
#include "Tself_gate_agata_psa_descr.h"
#include "t4selfgate_miniball_dlg.h"
#include "t4selfgate_geraddback_dlg.h"
#include "t4selfgate_agata_psa_dlg.h"
#include "t4selfgate_ge_crystal_galileo.h"
#include "t4selfgate_selector.h"
#include "t4selfgate_kratta_2dlg.h"
#include "Tself_gate_kratta_descr.h"
// NO! so far for paris sg dialog we use hector dialog
#include "t4selfgate_paris_dlg.h"
#include "Tself_gate_paris_descr_simple.h"

using namespace std;

string T4user_spectrum_wizard::NoSelfGate;
//**************************************************************************************************************
T4user_spectrum_wizard::T4user_spectrum_wizard(QWidget *parent) :
	QWizard(parent),
	ui(new Ui::T4user_spectrum_wizard)
{
	ui->setupUi(this);

	// To avoid writing this in Polish
	setButtonText(QWizard::NextButton, "Next  >" );
	setButtonText(QWizard::BackButton, "<  Back" );
	setButtonText(QWizard::CancelButton, "Cancel" );
	setButtonText(QWizard::FinishButton, "Finish" );


	szary = 0xcccccc;
	NoSelfGate = "No_self_gate";

	//QBrush brush_tab_x = ui->table_incr_x->


	// at first validators
	ui->lineEdit_bins->setValidator( new QIntValidator( 0, 10000, ui->lineEdit_bins));
	ui->lineEdit_first_bin->setValidator( new QDoubleValidator( -10000, +10000,  2,  ui->lineEdit_first_bin) );

	ui->lineEdit_last_bin->setValidator( new QDoubleValidator( -10000, +10000,  2,  ui->lineEdit_last_bin) );


	ui->table_incr_x->setColumnCount(3);
	//    ui->table_incr_x->setColumnHidden(2, true);
	ui->table_incr_y->setColumnCount(3);
	//    ui->table_incr_y->setColumnHidden(2, true);

	QStringList labels;
	labels << "Incrementer name" << "'Self-gate' name (if any)" << "SG type" ;
	ui->table_incr_x->setHorizontalHeaderLabels ( labels );
	ui->table_incr_x->resizeColumnsToContents ();
	ui->table_incr_y->setHorizontalHeaderLabels ( labels );
	ui->table_incr_y->resizeColumnsToContents ();


	ui->table_incr_x->setRowCount(0);
	ui->table_incr_y->setRowCount(0);

	// setting the  enabled options
	//   extended_selection_mode(false) ;

	ui->radio_always->setChecked(1) ;
	//ui->helpButton () ->hide();
	cloning = false ;
	ui->lineEdit_spectrum_name->setReadOnly(cloning);
	ui->textLabel_explanation->hide();

	ui->pushButton_create_condition->hide();  // better if it is not existing, because at first
	// the user should test new spectrum without any condition

	ui->push_common_sg->hide();     // do we really need it?

	flag_helper_y = false;

	arrow_up.load(":/images/images/arrow_up_normal.xcf") ;
	arrow_right.load(":/images/images/arrow_right_normal.xcf") ;
	//on_radioButton_x_toggled(true);
	ui->radioButton_x->setChecked(true);

    on_checkBox_rotation_flag_stateChanged(false);


}
//*************************************************************************************************************
T4user_spectrum_wizard::~T4user_spectrum_wizard()
{
	delete ui;
}
//*************************************************************************************************************
void T4user_spectrum_wizard::on_lineEdit_spectrum_name_textChanged(const QString &arg1)
{
	//    cout << "F. on_lineEdit_spectrum_name_textChanged(" << endl;

	string name = arg1.toStdString();

	int len = name.size() ;
	len = min(len, 5) ;
	if(name.substr(0, len) != string("user_").substr(0, len))
	{
		name.insert(0, "user_") ;
		ui->lineEdit_spectrum_name->setText(name.c_str()) ;
	}

	//cout << "Now the name is ->" << name << "<- " << endl ;
	if(name.size() < 5)
	{
		ui->lineEdit_spectrum_name->setText("user_") ;
		//cout << "Now is disabled " << endl ;
		//setNextEnabled ( currentPage (), false ) ;
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
		ui->lineEdit_spectrum_name->setText(name.c_str()) ;
	}

}
//**************************************************************

//**************************************************************
int T4user_spectrum_wizard::nextId() const
{
	//    cout << "F. nextId()  for the page: "
	//         << currentPage()->title().toStdString()
	//         << endl ;

	switch (currentId())
	{
		case  page__name_of_spectrum:
			{
				string name = ui->lineEdit_spectrum_name->text().toStdString() ;
				//        cout << "Now the name is ->" << name << "<- " << endl ;

				ui->textLabel_spec_name_X->setText(name.c_str() );
				ui->textLabel_spec_name_Y->setText(name.c_str() );

				if(ui->radioButton_1D->isChecked() == true)
					return page__binning_1D;
				else
					return page__binning_2D;
			}

		case page__binning_1D:
			{
				return page__x_incrementers;
			}
			//-------------------------------------
		case page__binning_2D:
			{
				//cout << "this is binnig_2D page" << endl;
				return page__x_incrementers;
			}

		case page__x_incrementers:
			return
					ui->radioButton_2D->isChecked() ?
						page__y_incrementers : page__conditon;


		case page__y_incrementers:
			if(ui->table_incr_x->rowCount() > 1 || ui->table_incr_y->rowCount() > 1
					|| has_any_ALL_incremeter()   )
			{
				return page__combinations;
			}else{

				ui->radio_always->setChecked(true);
                return page__rotation ;
			}
			// break;


		case page__combinations:
        if(ui->radioButton_1D->isChecked() == true)
            return page__conditon;
        else
            return page__rotation;

    case page__rotation:
        return page__conditon;

    case page__conditon:
			//        setFinishEnabled ( currentPage (), true) ;
			//        cout << "Enabling FinishButton" << endl;
			//        button(QWizard::FinishButton)->setEnabled(true);


            return -1;  // this is the end of wizard

		default:
			cout << "unknown id of the wizard page!!!!! " << endl;
	} // end switch

	return -1 ;  // should never happen


}

//****************************************************************
void T4user_spectrum_wizard::illegal_value( double d )
{
    showWarningMessage(
							  "Error in value of the angle",
							  QString("You typed the value %1, while the angle can not be bigger than 359.9, \nI will correct it into:  359.9")
                              .arg(d), QMessageBox::Critical);
}

//*********************************************************************
void T4user_spectrum_wizard::set_parameters( user_spectrum_description *s, bool editing)
// editing "false" - means brand new spectrum
{
	user = s ;

	if(editing) // already existing spectrum
	{
		ui->lineEdit_spectrum_name->setReadOnly(true);
		ui->push_clone->show();
		// ui->textLabel_about_name
		string nnn = string("<font size=\"+1\">Name of this spectrum:<br>")
				+ user->name_of_spectrum + "</font>" ;
		ui->textLabel_about_name->setText(nnn.c_str());
		ui->lineEdit_spectrum_name->hide();
	}
	else   // creating
	{
		ui->lineEdit_spectrum_name->setReadOnly(false);
		ui->push_clone->hide();
		button(QWizard::NextButton)->setEnabled(false); // untill good name is given
	}

	// setting current values
	ui->lineEdit_spectrum_name->setText(user->name_of_spectrum.c_str() );

	// binning
	ui->lineEdit_bins->setText(QString::number(user->bins_x) );
	ui->lineEdit_first_bin->setText(QString::number(user->beg_x) );
	ui->lineEdit_last_bin->setText(QString::number(user->end_x) );

	ui->lineEdit_bins_x2->setText(QString::number(user->bins_x) );
	ui->lineEdit_first_bin_x2->setText(QString::number(user->beg_x) );
	ui->lineEdit_last_bin_x2->setText(QString::number(user->end_x) );

	ui->lineEdit_bins_y->setText(QString::number(user->bins_y) );
	ui->lineEdit_first_bin_y->setText(QString::number(user->beg_y) );
	ui->lineEdit_last_bin_y->setText(QString::number(user->end_y) );

	ui->radioButton_1D->setChecked(user->dimmension == 1);
	ui->radioButton_2D->setChecked(user->dimmension == 2);

	ui->table_incr_x->setRowCount(0);
    for(uint i = 0 ; i <  user->incrementors_x.size() ; i++)
	{
		ui->table_incr_x->setRowCount(i+1);
		string increm_name = user->incrementors_x[i].first.c_str();
		ui->table_incr_x->setItem(i, 0, new QTableWidgetItem(increm_name.c_str()) );
		ui->table_incr_x->item(i, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);


		// in the colum of SELFGATE name
		string selfgate_name = user->incrementors_x[i].second;
		if(selfgate_name != NoSelfGate)
			selfgate_name = name_without_extension(selfgate_name);

		ui->table_incr_x->setItem(i, 1, new QTableWidgetItem(selfgate_name.c_str(),
															 Qt::ItemIsEnabled | Qt::ItemIsSelectable) );
		grey_cells(ui->table_incr_x, i);
		//

	}

	// YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY incrementer ---------------------------------
	/*   ui->table_incr_x->adjustColumn(0) ;
	ui->table_incr_x->adjustColumn(1) */;
	ui->table_incr_x->resizeColumnsToContents ();

	ui->table_incr_y->setRowCount(0);
	for(int i = 0 ; i <  (int) user->incrementors_y.size() ; i++)  {
		ui->table_incr_y->setRowCount(i+1);
		ui->table_incr_y->setItem(i, 0,
								  new QTableWidgetItem(user->incrementors_y[i].first.c_str(),
													   Qt::ItemIsEnabled | Qt::ItemIsSelectable) );

		string selfgate_name = user->incrementors_y[i].second;
		if(selfgate_name != NoSelfGate)
			selfgate_name = name_without_extension(selfgate_name);

		ui->table_incr_y->setItem(i, 1,
								  new QTableWidgetItem(selfgate_name.c_str(),
													   Qt::ItemIsEnabled | Qt::ItemIsSelectable) );

		grey_cells(ui->table_incr_y, i);
	}
	ui->table_incr_y->resizeColumnsToContents ();

	// ------------------- yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy

	ui->radio_always->setChecked(user->policy_when_increm == 0);
	ui->radio_different->setChecked(user->policy_when_increm == 1);
	ui->radio_same->setChecked(user->policy_when_increm == 2);

    ui->checkBox_rotation_flag->setChecked(user->flag_rotation);
    ui->doubleSpinBox_rotation_degree->setValue(user->rotation_angle);

	load_combo_conditions();
}
//*********************************************************************
void T4user_spectrum_wizard::grey_cells(QTableWidget * table, int row)
{
	// selfgate (if any, was just insterted in to column 1)


	//    cout << __func__ << endl;
	// in the TYPE colum of selfgates
	string increm_name = table->item(row, 0)->text().toStdString();
	Tselfgate_type typ = selfgate_type_for_this_type_of_incrementer(increm_name);

	if(typ.sg_is_not_available() == false)
	{
		//        cout << increm_name << " JEST sg " << endl;

		string typs = typ.give_type_name();

		table->setItem(row, 2, new QTableWidgetItem(typs.c_str(),
													Qt::ItemIsEnabled | Qt::ItemIsSelectable) );

		table->item(row, 1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);


	}else {         // if selfgate not possible for this  incrementer

		// so make this cell disabled
		//        cout << increm_name << " not_available sg " << endl;

		table->setItem(row, 1, new QTableWidgetItem("") );
		table->setItem(row, 2, new QTableWidgetItem("") );

		table->item(row, 1)->setFlags(Qt::ItemIsEnabled); //    Qt::ItemIsSelectable);
		table->item(row, 2)->setFlags(Qt::NoItemFlags); //Qt::ItemIsSelectable);

        table->item(row, 1)->setBackground(szary); // grey
        table->item(row, 2)->setBackground(szary); // grey
	}
}


//*********************************************************************
// this is probably call with Finish button
void T4user_spectrum_wizard::accept()
{
	//    cout << "F.T4user_spectrum_wizard::accept()" << endl;

	// copy all the settings into the structure "user"

	if(!check_xy_time_diff_selfgate_correctness() ) return;

	user->name_of_spectrum =    ui->lineEdit_spectrum_name->text().toStdString();

	// x incrementors are always ----------------------------------------------
	user->incrementors_x.clear();

	for(int i = 0 ;  i < (int) ui->table_incr_x->rowCount() ;  i++)
	{
		//user->incrementors_x.push_back(listBox_incr_x->text(i) )

		// cout << "table incr_x - row nr =" << i << " of " << ui->table_incr_x->rowCount() <<endl;
		string selfgateName = ui->table_incr_x->item(i, 1)->text().toStdString() ;
		selfgateName =
				selgate_name_with_extension(selfgateName,
											ui->table_incr_x->item(i, 0)->text().toStdString());   // incrementer name

		// cout << "while saving selfgate = " << selfgateName << endl;
		if(selfgateName[0] == '.') selfgateName.clear();
		user->incrementors_x.push_back(
					pair<string,string>(ui->table_incr_x->item(i, 0)->text().toStdString(),
										string(selfgateName.size()>2 ? selfgateName : NoSelfGate)
										) );
	}

	//------------------------------------------------------------------------
	if( ui->radioButton_1D->isChecked() )
	{
		user->bins_x= ui->lineEdit_bins->text().toInt() ;
		user->beg_x = ui->lineEdit_first_bin->text().toDouble() ;
		user->end_x = ui->lineEdit_last_bin->text().toDouble() ;

		user->dimmension =   user_spectrum_description::spec_1D    ;
	}
	else
	{
		user->bins_x= ui->lineEdit_bins_x2->text().toInt() ;
		user->beg_x = ui->lineEdit_first_bin_x2->text().toDouble() ;
		user->end_x = ui->lineEdit_last_bin_x2->text().toDouble() ;

		user->bins_y= ui->lineEdit_bins_y->text().toInt() ;
		user->beg_y = ui->lineEdit_first_bin_y->text().toDouble() ;
		user->end_y = ui->lineEdit_last_bin_y->text().toDouble() ;

		user->dimmension = user_spectrum_description::spec_2D  ;

		user->incrementors_y.clear();

		for(int i = 0 ;  i < (int) ui->table_incr_y->rowCount() ;  i++)
		{
			string selfgateName = ui->table_incr_y->item(i, 1)->text().toStdString() ;
			selfgateName = selgate_name_with_extension(selfgateName,
													   ui->table_incr_y->item(i, 0)->text().toStdString());   // incrementer name
			if(selfgateName[0] == '.') selfgateName.clear();
			user->incrementors_y.push_back(
						pair<string,string>(ui->table_incr_y->item(i, 0)->text().toStdString(),
											string(selfgateName.size()> 2 ? selfgateName : NoSelfGate)
											) );
		}
	}
	if(ui->radio_always->isChecked()) user->policy_when_increm = 0 ;
	else if(ui->radio_different->isChecked()) user->policy_when_increm = 1 ;
	else if(ui->radio_same->isChecked()) user->policy_when_increm = 2 ;

    user->flag_rotation = ui->checkBox_rotation_flag->isChecked()?true : false;
    user->rotation_angle = ui->doubleSpinBox_rotation_degree->value();

	user->condition_name = ui->comboBox_conditions->currentText().toStdString();

	check_xy_time_diff_selfgate_correctness();
	//-------------
	QDialog::accept();
}
//****************************************************************
string T4user_spectrum_wizard::short_long_name( string name)
{
	cerr << __func__ << " is this function ever used ????" << endl;
	if(name.find(".poly") != string::npos)
	{
		string::size_type pos  = name.rfind("_polygon_") ;
		if(pos == string::npos)return name ;
		return name.substr(pos+ 9);  // 9 is a size of the word "_polygon_"
	}
	else
	{
		return name;
	}
}
//********************************************************************
void T4user_spectrum_wizard::on_pushButton_add_clicked()
{
	add_clicked(ui->table_incr_x);
}
//***********************************************************************************************************
void T4user_spectrum_wizard::on_pushButton_add_y_clicked()
{
	add_clicked(ui->table_incr_y);
}
//********************************************************************************************
void T4user_spectrum_wizard::add_clicked(QTableWidget *table)
{
	T4incrementer_selector * dlg = new T4incrementer_selector ;
	dlg->set_multi_selection(true);

	if(dlg->exec()   == QDialog::Accepted )
	{
		QStringList lista = dlg->give_seleced_items() ;

		// if the chosen incrementor may have the selfgate, activate the combo
		for ( QStringList::Iterator it = lista.begin(); it != lista .end(); ++it )
		{
			int row = table->rowCount();
			// add
			table->setRowCount(row+1) ;

			table->setItem(row, 0, new QTableWidgetItem( *it )) ;
			table->setItem(row, 1, new QTableWidgetItem("")) ;

			table->item(row, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			table->item(row, 1)->setFlags(Qt::ItemIsEnabled| Qt::ItemIsSelectable);
			table->item(row, 1)->setTextAlignment(Qt::AlignHCenter);


			// in the TYPE colum of selfgates
			Tselfgate_type typ = selfgate_type_for_this_type_of_incrementer((*it).toStdString() );
			string typs = typ.give_type_name();

			table->setItem(row, 2, new QTableWidgetItem(typs.c_str(),
														Qt::ItemIsEnabled | Qt::ItemIsSelectable) );
			grey_cells(table, row);
		}
		table->resizeColumnsToContents ();
	}
}
//***********************************************************************************************************
void T4user_spectrum_wizard::on_pushButton_remove_y_clicked()
{
	remove_clicked(ui->table_incr_y);
}
//***********************************************************************************************************
void T4user_spectrum_wizard::on_pushButton_remove_x_clicked()
{
	remove_clicked(ui->table_incr_x);
}
//***********************************************************************************************************
void T4user_spectrum_wizard::remove_clicked(QTableWidget *table)
{
	// at first we look what is selected, because after removing the one row
	// all other selections are forgottten
	vector<int> rows_to_remove;

	string tekst ("you are going to erase following rows\n");
	ostringstream out;
	out << tekst;
	for(int i = 0 ; i < table->rowCount() ;  i++)
	{
		if( table->item(i,0)->isSelected())
		{
			rows_to_remove.push_back(i);
			out << "Row nr " << i+1 << "    " << table->item(i,0)-> text().toStdString() << endl;
			//            table->removeRow(i);
		}
	}
	out << "\nAre you sure?" ;

	if(QMessageBox::question ( this,"Deleting some row(s)",  out.str().c_str(),
							   QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel) == QMessageBox::Yes)
	{
		cout << " Will be removing. The list size " << rows_to_remove.size() << endl;
		for(int i =rows_to_remove.size()-1 ; i >= 0;  i--)
		{
			cout << " removing from the list pos " << i << ",  this is row " << rows_to_remove[i] << endl;
			table->removeRow(rows_to_remove[i]);
		}
		cout << "End" << endl;
	}
}


//*********************************************************************************************************************
//*********************************************************************************************************************
void T4user_spectrum_wizard::run_condition_wizard()   // never used?
{
#ifdef FINAL
	condition_wizard * dlg = new condition_wizard;

	Tcondition_description_cracow  cnd;

	dlg->set_parameters(&cnd) ;
	if ( dlg->exec() == QDialog::Accepted )
	{
		cnd.save_condition();
		//assign this condition
		user->condition_name = cnd.give_name() + user_cond_ext;


	}
	load_combo_conditions();
#endif
}
//******************************************************************
void T4user_spectrum_wizard::load_combo_conditions()
{

	QDir katalog(gpath.Qconditions()) ;
	QStringList lista_cond = katalog.entryList (QStringList( user_cond_filter.c_str() ) ) ;

	ui->comboBox_conditions->clear();
	ui->comboBox_conditions->addItem ("No_condition");
	int nr = 1 ;  // bec "No condition" is on the 0 position
	int current = 0 ;

	for ( QStringList::Iterator it = lista_cond.begin() ; it != lista_cond.end() ;  ++it, nr++ )
	{
		//    cout << "now1 =" << *it << "\n";

		QString nam = *it ;
		if(user->condition_name == nam.toStdString())
		{
			current = nr;
		}
		// removing the extension
		int i = nam.lastIndexOf(user_cond_ext.c_str() );
		nam.truncate(i);
		ui->comboBox_conditions->addItem (nam);
	}
	ui->comboBox_conditions->setCurrentIndex(current);
}
//**************************************************************************
void T4user_spectrum_wizard::called_creator_self_gate(QTableWidget *table, bool creation)
{

	// here we have to look what is in the pervious column and by that
	// we know which type of the self gate will be needed

	QStringList lst;

	// Loading the combobox contents - from the map
	int nr = 0 ;
	for(auto x  :   TSG::selfgate_types_map)               // for each loop
	{
		ostringstream s;
		s  << nr << ")  "      // this nr is important, by this later we will recognize the choice
		   << TSG::give_type_info( x.first) ;
		lst <<  s.str().c_str() ;
		++nr;
	}


	// trying to suggest which type is suitable for the currently selected row
	int row = table->currentRow();


	int typsg = 6;  // default is germanium galileo
	if(row >= 0 ){
		//    Tselfgate_type - to suggest the type of selgate
		typsg = selfgate_type_for_this_type_of_incrementer(table->item(row, 0)->text().toStdString() );
	}


    T4selfgate_selector dlg_sg;
    dlg_sg.set_parameters("There are several types of selfgates.\nSelect the desired type",
					   lst,
					   typsg, // 1,
					   "230, 230, 190, 255",
					   600);

	QString res;
    if(dlg_sg.exec() == QDialog::Accepted )
	{
        res = dlg_sg.get_selected_string() ;
	}else return;





	// user selected an item and pressed OK

	istringstream s(res.toStdString());
	int choice ;
	s >> choice ;
	switch(choice)    // first character in the text converted into number
	{
		case TSG::Types::not_available : return;

		case TSG::Types::german_crystal:
			{
				T4selfgate_ger_dlg dlg;
				Tself_gate_ger_descr desc ;
				desc.energy_time_polygon_gate = "please_click_here_to_choose_banana_gate";

				if(!creation) {
					string sg_name = select_sg_name_from_disk(Tselfgate_type(choice));
					if(sg_name.empty()) return;
					desc.read_definition_from(gpath.conditions + sg_name);
				}
				dlg.set_parameters( &desc );
				if (dlg.exec() == QDialog::Accepted )
				{
//					cout << "Dialog accepted " << endl;
					dlg.get_parameters( &desc );
					desc.write_definitions(gpath.conditions);
					if(creation)selfgate_succesfully_created(desc.name, table);
				}
				//desc.name ;
			}
			break;


		case TSG::Types::hector :
			{
                T4selfgate_hec_dlg dlg2;
				Tself_gate_hec_descr desc ;
				if(!creation) {
					string sg_name = select_sg_name_from_disk(Tselfgate_type(choice));
					if(sg_name.empty()) return;
					desc.read_definition_from(gpath.conditions + sg_name);
				}
                dlg2.set_parameters( &desc );
                if (dlg2.exec() == QDialog::Accepted )
				{
                    dlg2.get_parameters( &desc );
					desc.write_definitions(gpath.conditions);
					if(creation)selfgate_succesfully_created(desc.name, table);
				}

			}
			break;

		case TSG::Types::miniball :
			{
                T4selfgate_miniball_dlg dlg2;
				Tself_gate_mib_descr desc ;
				if(!creation) {
					string sg_name = select_sg_name_from_disk(Tselfgate_type(choice));
					if(sg_name.empty()) return;
					desc.read_definition_from(gpath.conditions + sg_name);
				}
                dlg2.set_parameters( &desc );
                if (dlg2.exec() == QDialog::Accepted ) {
                    dlg2.get_parameters( &desc );
					desc.write_definitions(gpath.conditions);
					if(creation)selfgate_succesfully_created(desc.name, table);
				}
			}
			break;




		case TSG::Types::cluster_addback   :
			{
				T4selfgate_geraddback_dlg dlg;
				Tself_gate_ger_addback_descr desc ;
				desc.energy_time_polygon_gate = "please_click_here_to_choose_banana_gate";

				if(!creation) {
					string sg_name = select_sg_name_from_disk(Tselfgate_type(choice));
					if(sg_name.empty()) return;
					desc.read_definition_from(gpath.conditions + sg_name);
				}
				dlg.set_parameters( &desc );
				if (dlg.exec() == QDialog::Accepted )
				{
					dlg.get_parameters( &desc );
					desc.write_definitions(gpath.conditions);
					if(creation)selfgate_succesfully_created(desc.name, table);
				}
			}
			break;

		case TSG::Types::agata_psa :
			{
				T4selfgate_agata_psa_dlg dlg;
				Tself_gate_agata_psa_descr  desc ;
				//desc.energy_time_polygon_gate = "please_click_here_to_choose_banana_gate";

				if(!creation) {
					string sg_name = select_sg_name_from_disk(Tselfgate_type(choice));
					if(sg_name.empty()) return;
					desc.read_definition_from(gpath.conditions + sg_name);
				}
				dlg.set_parameters( &desc );
				if (dlg.exec() == QDialog::Accepted )
				{
					dlg.get_parameters( &desc );
					desc.write_definitions(gpath.conditions);
					if(creation)selfgate_succesfully_created(desc.name, table);
				}

			}
			break;

		case TSG::Types::galileo_ge :
			{
				T4selfgate_ge_galileo dlg;
				Tself_gate_ge_galileo_descr  desc ;


				if(!creation) {
					string sg_name = select_sg_name_from_disk(Tselfgate_type(choice));
					if(sg_name.empty()) return;
					desc.read_definition_from(gpath.conditions + sg_name);
				}
				dlg.set_parameters( &desc );
				if (dlg.exec() == QDialog::Accepted )
				{
					dlg.get_parameters( &desc );
					desc.write_definitions(gpath.conditions);
					if(creation)selfgate_succesfully_created(desc.name, table);
				}

			}
			break;
		case TSG::Types::kratta :
			{
				T4selfgate_kratta_2dlg dlg;
				Tself_gate_kratta_descr desc ;
				if(!creation) {
					string sg_name = select_sg_name_from_disk(Tselfgate_type(choice));
					if(sg_name.empty()) return;
					desc.read_definition_from(gpath.conditions + sg_name);
				}
				dlg.set_parameters( &desc );
				if (dlg.exec() == QDialog::Accepted )
				{
					dlg.get_parameters( &desc );
					desc.write_definitions(gpath.conditions);
					if(creation)selfgate_succesfully_created(desc.name, table);
				}

			}
			break;

		case TSG::Types::paris :
			{
				T4selfgate_paris_dlg dlg;
				Tself_gate_paris_descr desc ;
				if(!creation) {
					string sg_name = select_sg_name_from_disk(Tselfgate_type(choice));
					if(sg_name.empty()) return;
					desc.read_definition_from(gpath.conditions + sg_name);
				}
				dlg.set_parameters( &desc ); // cheating
				if (dlg.exec() == QDialog::Accepted )
				{
					dlg.get_parameters(&desc );
					desc.write_definitions(gpath.conditions);
					if(creation)selfgate_succesfully_created(desc.name, table);
				}

			}
			break;


		default:
			QMessageBox::warning(this, "Strange...", "This is strange: Unrecognised type of SelfGate", QMessageBox::Ok);
			break;
	} // end switch


	ui->table_incr_x->clearSelection();
	ui->table_incr_y->clearSelection();
	ui->table_incr_x->selectColumn(1);
	ui->table_incr_y->selectColumn(1);
}

//***********************************************************************************************************************
bool T4user_spectrum_wizard::check_xy_time_diff_selfgate_correctness()
{
	vector<string> list_of_suspects_x;
	vector<string> list_of_suspects_y;
#ifdef FINAL
	for(int i = 0 ;  i < (int) ui->table_incr_x->rowCount() ;  i++)
	{
		//user->incrementors_x.push_back(listBox_incr_x->text(i) )
		QComboTableItem* comb_ptr = dynamic_cast<QComboTableItem*>(ui->table_incr_x->item(i, 1));

		if(!comb_ptr) continue;
		string sf_name = string( comb_ptr->currentText().toStdString());
		//cout << "X sf_name = " << sf_name << endl;
		// checking the possible use of xy difference checkbox_of_channels

		if(sf_name.find(".self_gate_ger_crystal") != string::npos)
		{
			Tself_gate_ger_descr d ;
			d.read_definition_from(gpath.conditions + sf_name);
			if(d.enable_xy_gamma_time_difference)
			{
				list_of_suspects_x.push_back(sf_name);
				//         cout << "is suspected X sf_name = " << sf_name << endl;
			}
		}
	}
	unique(list_of_suspects_x.begin(), list_of_suspects_x.end());
	sort(list_of_suspects_x.begin(), list_of_suspects_x.end());

	//   for(unsigned int i = 0 ; i < list_of_suspects_x.size() ; i++)
	//   {
	//     cout << i << " after sorting  X sf's" << list_of_suspects_x[i] << endl;
	//   }

	//========================================

	switch( ui->radioButton_1D->isChecked())
	{
		case true : // 1D spectrum
			// check all selfgates if they have opiton xy  - if yes - nonsense
			if(list_of_suspects_x.size())
			{
				//       cout <<" Error: In the 1D spectrum you should not use the option xy inside the selgates: " ;

				string zdanie  ;
				for(unsigned int i = 0 ; i < list_of_suspects_x.size() ; i++)
				{
					zdanie += list_of_suspects_x[i] + "\n";
				}

				ostringstream ccc;
				ccc << "Logical Error!: In the 1D spectrum you should not use inside a selfgates the option: \n"
					   " >Understand as time difference between two gammas'<\n"
					   "It is used in  the selfgate called:\n"  << zdanie
					<< "Go BACK and correct this error" ;

				QMessageBox::critical( this,
									   "Error in the use of selfgates ", ccc.str(),
									   QMessageBox::Ok,
									   QMessageBox::NoButton,
									   QMessageBox::NoButton);
				return false;
			}

			break;
			//--------------------------------------------- 2D--------------------------------------------------
		case false:
			// if on the x list there is such selfgate - it must be also on the Y axis)
			// and vice versa
			{
				//       cout << "Searching suspected Y sf" << endl;

				for(int i = 0 ;  i < (int) ui->table_incr_y->rowCount() ;  i++)
				{
					//user->incrementors_y.push_back(listBox_incr_y->text(i) )
					QComboTableItem* comb_ptr = dynamic_cast<QComboTableItem*>(ui->table_incr_y->item(i, 1));

					if(!comb_ptr) continue;
					string sf_name = string( comb_ptr->currentText().toStdString());
					// cout << " Y sf_name = " << sf_name << endl;
					// checking the possible use of xy difference checkbox_of_channels


					if(sf_name.find(".self_gate_ger_crystal") != string::npos)
					{
						Tself_gate_ger_descr d ;
						d.read_definition_from(gpath.conditions + sf_name);
						if(d.enable_xy_gamma_time_difference)
						{
							list_of_suspects_y.push_back(sf_name);
							//             cout << " suspected Y sf_name = " << sf_name << endl;

						}
					} // endif this is suspected selfgate
				} // end for over selfgates
				unique(list_of_suspects_y.begin(), list_of_suspects_y.end());
				sort(list_of_suspects_y.begin(), list_of_suspects_y.end());
				//       for(unsigned int i = 0 ; i < list_of_suspects_y.size(); i++)
				//       {
				//         cout << list_of_suspects_y[i] << ",  " ;
				//       }

				vector<string> result;
				result.resize(list_of_suspects_x.size() + list_of_suspects_y.size());

				set_symmetric_difference(
							list_of_suspects_x.begin(), list_of_suspects_x.end(),
							list_of_suspects_y.begin(), list_of_suspects_y.end(),
							result.begin() );

				//       cout << "\nAfter differences result size = " << result.size() << endl;
				bool flag_was_error = false;
				string zdanie ;
				for(unsigned int i = 0 ; i < result.size() ; i++)
				{
					if(result[i] == "")    continue;
					//         cout << i << " after 2D differences   " << result[i] << endl;

					flag_was_error = true;
					zdanie += result[i] + "\n" ;
				}
				if(flag_was_error)
				{
					ostringstream ccc;
					ccc << "Logical Error! The selfgates with xy time difference option  "
						   "should always apear twice - on the both lists (X anY) of incrementers.  "
						   "It is not the case for the selfgates called:\n"  << zdanie
						<< "Go back and correct this error" ;

					QMessageBox::critical( this,
										   "Error in the use of selfgates ", ccc.str(),
										   QMessageBox::Ok,
										   QMessageBox::NoButton,
										   QMessageBox::NoButton);
					return false;

				}
			}
			break;

	};

#endif
	return true;
}
//***********************************************************************************************************************
//***********************************************************************************************************************
bool T4user_spectrum_wizard::validateCurrentPage()
{
	//    cout << "F. validateCurrentPage "
	//         << currentPage()->title().toStdString()  << endl;
	switch(currentId() )
	{
        default:
		case page__name_of_spectrum:
			{
				string name = ui->lineEdit_spectrum_name->text().toStdString() ;
				//        cout << "Now the name is ->" << name << "<- " << endl ;
				if(name.size() <= 5)
				{
					//      cout << "Now enabled is disabled " << endl ;
					//setNextEnabled ( currentPage (), false ) ;

                    showWarningMessage(
											   QString("Error in the name of spectrum"),
											   QString("To go to the next page, you need to specify the name of the spectrum\n"
													   "\n(It will always start with prefix \"user_\")"),
                                               QMessageBox::Critical);
					return false;
				}
				else return true;
			}
			break;

			//--------------------
		case page__binning_1D:
			if(ui->lineEdit_last_bin->text().toDouble()  <= ui->lineEdit_first_bin->text().toDouble() )
			{
                showWarningMessage(
										  "Error in the range of the spectrum",
										  "The 'value of the right edge of the last bin' "
										  " should be BIGGER than the 'value of the left edge of the first bin'.  "
										  "\nPlease correct it",
                                          QMessageBox::Critical);
				return false;
			}

			if(ui->lineEdit_bins->text().toInt()  <= 0)
			{
                showWarningMessage(
										  "Error in the binning of the spectrum",
										  "The 'value of how many bins' should be bigger than zero. "
										  "\nPlease correct it",
                                          QMessageBox::Critical);
				return false;
			}
			return true;
			break;

		case page__binning_2D:

			// ---- X --------------
			if(ui->lineEdit_bins_x2->text().toInt()  <= 0)
			{
                showWarningMessage(
										  "Error in the X binning",
										  "The 'value of how many X bins' \nshould be bigger than zero. "
										  "\nPlease correct it",
                                          QMessageBox::Critical);
				return false;
			}

			if(ui->lineEdit_last_bin_x2->text().toDouble()  <= ui->lineEdit_first_bin_x2->text().toDouble() )
			{
                showWarningMessage(
										  "Error in the X range",
										  "The 'value of the right edge of the X last bin ' \n"
										  " should be BIGGER than \nthe 'value of the left edge of the X first bin'.  "
										  "\nPlease correct it",
                                          QMessageBox::Critical);
				return false;
			}



			//------- Y -------------
			if(ui->lineEdit_bins_y->text().toInt()  <= 0)
			{
                showWarningMessage(
										  "Error in the Y binning",
										  "The 'value of how many Y bins' should be bigger than zero. \n"
										  "Please correct it",
                                          QMessageBox::Critical);
				return false;
			}

			if(ui->lineEdit_last_bin_y->text().toDouble()  <= ui->lineEdit_first_bin_y->text().toDouble() )
			{
                showWarningMessage(
										  "Error in the Y range of the spectrum",
										  "The 'value of the right edge of the last Y bin' \n"
										  " should be BIGGER than \nthe 'value of the left edge of the first Y bin'.  "
										  "\nPlease correct it",
                                          QMessageBox::Critical);
				return false;
			}


			return true;
			break;
		case page__x_incrementers:
			if(ui->table_incr_x->rowCount() < 1)
			{
				//            QMessageBox::information( this,
				//                                  "Just to remind you",
				//                                  "You didn't specify any incrementers, so your spectrum will not be incremented"
				//                                  "\nPlease remember to return later and correct it",
				//                                  QMessageBox::Ok,
				//                                  QMessageBox::NoButton,
				//                                  QMessageBox::NoButton);
			}
			return true;
			break;

		case page__y_incrementers:
			// cout << "Validating page__y_incrementers as true!!!!" << endl;
			return true;
			break;

		case page__combinations:

			if( (ui->table_incr_x->rowCount() > 1 || ui->table_incr_y->rowCount() > 1)
					&&
					ui->radio_always->isChecked()
					)
			{
                showWarningMessage( "It can be your logical error",
									  "There is more than one possible combination of x,y incrementers\n"
									  "and you selected here option: 'Always'\n"
                                      "this may have NO PHYSICAL SENSE!\nYou were warned...");
			}
			return true;
			//break;

        case page__rotation:
        return true;

		case page__conditon:
			return true;
			//break;

	} // end switch

	cout << "This should never happen " << endl;
	return false;
}
//**************************************************************************************************************************
void T4user_spectrum_wizard::on_table_incr_x_cellDoubleClicked(int row, int column)
{
	table_incr_DoubleClicked(ui->table_incr_x, row, column);
}
//***********************************************************************************************************************
void T4user_spectrum_wizard::on_table_incr_y_cellDoubleClicked(int row, int column)
{
	table_incr_DoubleClicked(ui->table_incr_y, row, column);
}
//**************************************************************************************************************************
void T4user_spectrum_wizard::table_incr_DoubleClicked(QTableWidget * table, int row, int column)
{
	switch(column)
	{
        default:
        case 0: // column of incrementers name clicked
			{
				T4incrementer_selector dlg;
				dlg.set_multi_selection(false);

				if(dlg.exec()   == QDialog::Accepted )
				{
					QStringList lista = dlg.give_seleced_items() ;

					// if the chosen incrementor may have the selfgate, activate the combo
					if(lista.empty() == false)
					{
						table->item(row, 0)->setText( lista[0] ) ;
						table->setItem(row, 1, new QTableWidgetItem() ) ;

						table->item(row, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
						table->item(row, 1)->setTextAlignment(Qt::AlignHCenter);

						// in the TYPE colum of selfgates
						Tselfgate_type typ = selfgate_type_for_this_type_of_incrementer(lista[0].toStdString() );
						if(typ.sg_is_not_available() == false  )
						{
							table->item(row, 1)->setText(NoSelfGate.c_str());
						}

						string typs = typ.give_type_name();

						table->item(row, 2)->setText(typs.c_str());
						grey_cells(table, row);
					}
					table->resizeColumnsToContents ();
				}
			}
			break;


		case 1: //            cout << "Colum 2  SG name clicked"  << endl;
			{
				string increm_name = table->item(row,0)->text().toStdString();
				cout << "Trying to find selfgate for " << increm_name << endl;
				Tselfgate_type typ = selfgate_type_for_this_type_of_incrementer(increm_name);
//				cout << "Znaleziony typ " << typ.give_type_name() << ", filtr = "
//					 << typ.give_type_filter() << ", info = " << typ.give_type_info()
//					 << endl;
				if(typ.sg_is_not_available() )
				{
					string txt = "No self-gate is applicable\n\nfor the incrementer called \n" + increm_name;
					QMessageBox::information(this, "Not available", txt.c_str());

				}else{
					T4selfgate_choice_dlg dlg;
					dlg.set_type_of_selfgate(typ);
					if(dlg.exec())
					{
						string fname_selfgate = dlg.get_selected();
						// cout << "fname_selfgate = " << fname_selfgate << endl;
						// put the selfgate without extension
						table->item(row,1)->setText(fname_selfgate.c_str()) ;
						table->resizeColumnsToContents ();
					}
				}
			}
			break;

			// selfgate TYPE column clicked
		case 2:
			QMessageBox::information(this, "Not available", "Click on the selfgate column ");
			break;

	} // end switch
	table->item(row, column)->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}
//***********************************************************************************************************************
void T4user_spectrum_wizard::on_push_clone_clicked()
{
	cloning = true ;
	ui->lineEdit_spectrum_name->setReadOnly(!cloning);
	ui->push_clone->hide();
	ui->lineEdit_spectrum_name->show();
	ui->lineEdit_spectrum_name->setText(
				ui->lineEdit_spectrum_name->text() +"_clone");
	ui->textLabel_explanation->show();
}
//***********************************************************************************************************************
void T4user_spectrum_wizard::on_push_crea_self_x_clicked()
{
	called_creator_self_gate(ui->table_incr_x, true); // true means -  creation new selfgeate
}
//***********************************************************************************************************************
void T4user_spectrum_wizard::on_push_edit_self_x_clicked()
{
	called_creator_self_gate(ui->table_incr_x, false);   // false means - no creation, just editing old one
}
//***********************************************************************************************************************
Tselfgate_type::Types  T4user_spectrum_wizard::selfgate_type_for_this_type_of_incrementer(string name_of_incrementer)
{

	// can be old style (no info  about selgate type for this incrementer) . Then we search
	// in the incrementers name some patterns - like "cluster_crys", "hector" "miniball"


	// The new style reqires that the spy prepares the list of incrementers together with the codes
	// (enum) describing the type of selfgate (code = 0 means NO selgate can be used for this incrementer)

	// look at the name of the incrementer

	// give the code of the selfgate type
	string fname = (gpath.commands + "list_of_variables_with_selfgates.txt") ;
	// open the file
	ifstream plik( fname.c_str() );
	if(plik)
	{
		try{
			//            int kod = (int)
			Nfile_helper::spot_in_file(plik, name_of_incrementer);
			Tselfgate_type::Types   kod_enum = Tselfgate_type::not_available;
			plik >> kod_enum   ; // = static_cast<Tselfgate_type> (kod);
			return kod_enum;
		}
		catch(Tno_keyword_exception m )
		{
			// normal situation,
			return Tselfgate_type::not_available;
		}
		catch(Treading_value_exception m)
		{
			cerr << "In system file " << fname << " " << m.message << endl;
			throw ;
		}
	}
	return Tselfgate_type::not_available ; // by default
}
//***********************************************************************************************************************
string T4user_spectrum_wizard::name_without_extension(string name)
{
	string sg = name;
	if(sg.size())
	{
		size_t k = name.rfind(".");
		if(k != string::npos)
		{
			sg = name.substr(0,k);
		}
	}
	//cout << "finally selfgate without ext = " << sg << endl;
	return sg;
}
//***********************************************************************************************************************
string T4user_spectrum_wizard::selgate_name_with_extension(string sg, string incrementer)
{
	if(sg == NoSelfGate) return sg;

	Tselfgate_type typ = selfgate_type_for_this_type_of_incrementer(incrementer);
	if(typ.sg_is_not_available() ) return sg;

	string ext = typ.give_type_extension();

	size_t k = sg.rfind(ext) ;
	if(k != string::npos)   // if extension already exist
	{
		//cout << __func__ << "  ext found on position nr "<< k << " so return "<< sg << endl;
		return sg;
	}else {
		//cout << __func__ << "   "<< (sg+ext) ;
		return sg +  ext;
	}
}
//***********************************************************************************************************************
void T4user_spectrum_wizard::selfgate_succesfully_created(string sg_name, QTableWidget * /*table*/)
{
	ostringstream s;
	s << "A selfgate called: " << sg_name <<
		 "\nwas succesfuly created, and is stored on the disk.\n\n"
		 "You can apply this selfgate to a proper incrementer\n"
		 " by double clicking his 'Selfgate' column, and selecting it"
		 " from the list of all available selfgates" ;

    showWarningMessage(
							  "Success",
							  s.str().c_str(),
                              QMessageBox::Information);

}
//***********************************************************************************************************************
void T4user_spectrum_wizard::on_push_crea_self_Y_clicked()
{
	called_creator_self_gate(ui->table_incr_y, true); // create
}
//***********************************************************************************************************************
void T4user_spectrum_wizard::on_push_edit_self_y_clicked()
{
	called_creator_self_gate(ui->table_incr_y, false);   // no create, just edit exiting one
}
//***********************************************************************************************************************
void T4user_spectrum_wizard::on_push_common_sg_clicked()
{
#if 0 // NA RAZIE NIE WIDZE POTRZEBY

	QTableWidget * table = ui->table_incr_x;


	// 1. Wybor SG - (wg typu pierwszej wybranej komorki)

	// 2. Może wybrać - z tego menu, ktore pojawia sie w edycji selfgate.


	// At first we look what is selected, because after removing the one row
	// all other selections are forgottten  <-- tu nie ma tego zagrożenia
	vector<int> rows_to_place_sg;

	// To wstawiac tylko pod warunkiem ze typ SG się zgadza.




	string tekst ("You are going to insert a SelfGate name into following rows\n");
	ostringstream out;
	out << tekst;
	for(int i = 0 ; i < table->rowCount() ;  i++)
	{
		if( table->item(i,1)->isSelected())
		{
			rows_to_place_sg.push_back(i);
			out << "Row nr " << i+1 << "    " << table->item(i,0)-> text().toStdString() << endl;
			//            table->removeRow(i);
		}
	}
	out << "\nAre you sure?" ;


	if(QMessageBox::question ( this,"Including selfgate name in some row(s)",  out.str().c_str(),
							   QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel) == QMessageBox::Yes)
	{
		cout << " Will be placing. The list size " << rows_to_place_sg.size() << endl;
		for(int i =rows_to_place_sg.size()-1 ; i >= 0;  i--)
		{
			cout << " Placing from the list pos " << i << ",  this is row " << rows_to_place_sg[i] << endl;
			table->item(rows_to_place_sg[i], 1)->setText("NewSelfgate");
		}
		cout << "End" << endl;
	}
#endif //

}
//*******************************************************************************************************************
string T4user_spectrum_wizard::select_sg_name_from_disk(Tselfgate_type typ)
{
	QDir katalog(gpath.Qconditions()) ;
	QStringList filter ;
	filter <<  typ.give_type_filter().c_str();
	QStringList lista_cond = katalog.entryList(filter);

	if(lista_cond.empty())
	{
        showWarningMessage( "Warning",
							  "No selfgates of this particular type exist yet.\n"
                              "You must create it at first");
		return "";
	}

	T4selfgate_selector dlg;
	dlg.set_parameters("Choose the self-condition which you want to edit",
					   lista_cond,
					   0, // 1,
					   "220, 255, 255, 255",
					   350);

	QString sg_name;
	if(dlg.exec() == QDialog::Accepted )
	{
		sg_name = dlg.get_selected_string() ;
	}else return "";


	// user selected an item and pressed OK
    showWarningMessage( "Warning",
						  "Remember that...\n...the same 'Self-Gate' may be used by many incrementors "
						  "in many different user spectra.\nBy modifying the chosen 'Self Gate' "
						  "YOU WILL AFFECT THEM ALL.\n\n"
						  "Perhaps this is just what you want.\n\n"
						  "If not - consider cloning the self gate, (change its name)\n"
                          "and then use this new 'self gate' in your particular incrementor");

	return sg_name.toStdString();
}
//***************************************************************************************
bool T4user_spectrum_wizard::has_any_ALL_incremeter() const
{

	for(int i = 0 ;  i < (int) ui->table_incr_x->rowCount() ;  i++)
	{
		string incrName = ui->table_incr_x->item(i, 0)->text().toStdString();   // incrementer name
		if(incrName.substr(0,3) == "ALL") {
			//cout << "Found ALL incrementer :  " << incrName << endl;
			return true;
		}

	}
	for(int i = 0 ;  i < (int) ui->table_incr_y->rowCount() ;  i++)
	{
		string incrName = ui->table_incr_y->item(i, 0)->text().toStdString();   // incrementer name
		if(incrName.substr(0,3) == "ALL") {
			//cout << "Found ALL incrementer :  " << incrName << endl;
			return true;
		}

	}
	return false;
}
//**************************************************************************************************************
void T4user_spectrum_wizard::on_pushButton_256_clicked()
{
	ui->lineEdit_bins->setText("256");
	ui->lineEdit_first_bin->setText("0.0");
	ui->lineEdit_last_bin->setText("256.0");
}
//**************************************************************************************************************
void T4user_spectrum_wizard::on_pushButton_512_clicked()
{
	ui->lineEdit_bins->setText("512");
	ui->lineEdit_first_bin->setText("0.0");
	ui->lineEdit_last_bin->setText("512.0");
}
//**************************************************************************************************************
void T4user_spectrum_wizard::on_pushButton_1024_clicked()
{
	ui->lineEdit_bins->setText("1024");
	ui->lineEdit_first_bin->setText("0.0");
	ui->lineEdit_last_bin->setText("1024.0");
}

//**************************************************************************************************************
void T4user_spectrum_wizard::on_pushButton_2048_clicked()
{
	ui->lineEdit_bins->setText("2048");
	ui->lineEdit_first_bin->setText("0.0");
	ui->lineEdit_last_bin->setText("2048.0");
}
//**************************************************************************************************************
void T4user_spectrum_wizard::on_pushButton_4096_clicked()
{
	ui->lineEdit_bins->setText("4096");
	ui->lineEdit_first_bin->setText("0.0");
	ui->lineEdit_last_bin->setText("4096.0");
}

//**************************************************************************************************************
void T4user_spectrum_wizard::on_pushButton_8192_clicked()
{
	ui->lineEdit_bins->setText("8192");
	ui->lineEdit_first_bin->setText("0.0");
	ui->lineEdit_last_bin->setText("8192.0");
}
//**************************************************************************************************************
void T4user_spectrum_wizard::on_radioButton_y_toggled(bool checked)
{
	flag_helper_y = checked;
	//	cout << __func__ << " flag helper Y ma stan " << flag_helper_y << endl;

	ui->label_arrow_up->setPixmap(arrow_up) ;

}
//**************************************************************************************************************
void T4user_spectrum_wizard::on_radioButton_x_toggled(bool checked)
{
	flag_helper_y = !checked;
	//	cout << __func__ << " flag helper Y ma stan " << flag_helper_y << endl;

	ui->label_arrow_up->setPixmap(arrow_right) ;

}
//**************************************************************************************************************
void T4user_spectrum_wizard::on_pushButton_256_M_clicked()
{
	if( flag_helper_y)
	{
		ui->lineEdit_bins_y->setText("256");
		ui->lineEdit_first_bin_y ->setText("0.0");
		ui->lineEdit_last_bin_y->setText("256.0");
	}else{
		ui->lineEdit_bins_x2->setText("256");
		ui->lineEdit_first_bin_x2->setText("0.0");
		ui->lineEdit_last_bin_x2->setText("256.0");
	}
}
//**************************************************************************************************************
void T4user_spectrum_wizard::on_pushButton_512_M_clicked()
{
	if( flag_helper_y)
	{
		ui->lineEdit_bins_y->setText("512");
		ui->lineEdit_first_bin_y ->setText("0.0");
		ui->lineEdit_last_bin_y->setText("512.0");
	}else{
		ui->lineEdit_bins_x2->setText("512");
		ui->lineEdit_first_bin_x2->setText("0.0");
		ui->lineEdit_last_bin_x2->setText("512.0");
	}
}
//**************************************************************************************************************
void T4user_spectrum_wizard::on_pushButton_1024_M_clicked()
{
	if( flag_helper_y)
	{
		ui->lineEdit_bins_y->setText("512");
		ui->lineEdit_first_bin_y ->setText("0.0");
		ui->lineEdit_last_bin_y->setText("1024.0");
	}else{
		ui->lineEdit_bins_x2->setText("512");
		ui->lineEdit_first_bin_x2->setText("0.0");
		ui->lineEdit_last_bin_x2->setText("1024.0");
	}
}
//**************************************************************************************************************
void T4user_spectrum_wizard::on_pushButton_2048_M_clicked()
{
	if( flag_helper_y)
	{
		ui->lineEdit_bins_y->setText("512");
		ui->lineEdit_first_bin_y ->setText("0.0");
		ui->lineEdit_last_bin_y->setText("2048.0");
	}else{
		ui->lineEdit_bins_x2->setText("512");
		ui->lineEdit_first_bin_x2->setText("0.0");
		ui->lineEdit_last_bin_x2->setText("2048.0");
	}
}
//**************************************************************************************************************
void T4user_spectrum_wizard::on_pushButton_4096_M_clicked()
{
	if( flag_helper_y)
	{
		ui->lineEdit_bins_y->setText("512");
		ui->lineEdit_first_bin_y ->setText("0.0");
		ui->lineEdit_last_bin_y->setText("4096.0");
	}else{
		ui->lineEdit_bins_x2->setText("512");
		ui->lineEdit_first_bin_x2->setText("0.0");
		ui->lineEdit_last_bin_x2->setText("4096.0");
	}
}
//**************************************************************************************************************
void T4user_spectrum_wizard::on_pushButton_8192_M_clicked()
{
	if( flag_helper_y)
	{
		ui->lineEdit_bins_y->setText("512");
		ui->lineEdit_first_bin_y ->setText("0.0");
		ui->lineEdit_last_bin_y->setText("8192.0");
	}else{
		ui->lineEdit_bins_x2->setText("512");
		ui->lineEdit_first_bin_x2->setText("0.0");
		ui->lineEdit_last_bin_x2->setText("8192.0");
	}
}
//**************************************************************************************************************


void T4user_spectrum_wizard::on_checkBox_rotation_flag_stateChanged(int arg1)
{
    ui->doubleSpinBox_rotation_degree->setVisible(arg1);
    ui->label_degree->setVisible(arg1);
    ui->textLabel_rotation_note->setVisible(arg1);
}
