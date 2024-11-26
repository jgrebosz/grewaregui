#if 0



#include "t4checking_incrementers_dlg.h"
#include "ui_t4checking_incrementers_dlg.h"

#include <QDir>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "paths.h"
#include "t4incrementer_selector.h"

using namespace std;

extern Tpaths path;
//***********************************************************************************************************************
T4checking_incrementers_dlg::T4checking_incrementers_dlg(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::T4checking_incrementers_dlg)
{
	ui->setupUi(this);
	ui->progressBar->setRange(0, 100);
	ui->progressBar->setValue(0);

    ui->text_raport->setAcceptRichText(true);

	nieb_pocz = "<span style=\"  color:#0000ff;\">";
	czerw_pocz = "<span style=\"   color:#ff0000;\">";
	ziel_pocz	= 	"<span style=\"  color:#006f00;\">";;
	kolor_kon = "</span>";

	string komunikat = nieb_pocz + "  Results will be displayed here" + kolor_kon ;
    ui->text_raport->setText(komunikat.c_str() );
	ui->label_working->setVisible(false);

	ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Acknowledged");
	ui->buttonBox->button(QDialogButtonBox::Cancel)->setHidden(true);
	ui->checkBox_ignore_noinc->setChecked(true);
	flag_ignore_noinc = true;

}
//***********************************************************************************************************************
T4checking_incrementers_dlg::~T4checking_incrementers_dlg()
{
	delete ui;
}
//***********************************************************************************************************************
void T4checking_incrementers_dlg::on_pushButton_start_clicked()
{

	ui->pushButton_start->setEnabled(false);
	// flag_stop_checking = false;
	auto save_tekst = ui->pushButton_start->text();
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	ui->pushButton_start->setText("Working...");
	ui->label_working->setVisible(true);

    ui->text_raport->setText(" ");
    ui->text_raport->update();
	// make a list of all bild-in spectra

	QDir katalog ( path.Qspectra() ); // ("spectra");

	ifstream dfile(path.spectra + "descriptions.txt");
	if(!dfile)
	{
		cout << "error in creating descr_file file" << endl;
	}

	// for progress bar
	dfile.seekg(0, ios_base::end); // go to the last word
	auto last_char  = dfile.tellg();
	ui->progressBar->setRange(0, last_char);
	dfile.seekg(0);
	ui->progressBar->setValue(0);
	// ---------------------------------------------------------------

	vector<string> spectra_names_vector;
	//------------------------------------------------------------

	ostringstream report;

	string word;
	string sname;
	int dim = 0;
	int nr = 0;
	int error_nr = 0;
	T4incrementer_selector * dlg = new T4incrementer_selector ;

	// loop over every spectrum -------------------------
	while(dfile >> word)    // =========================================
	{
		if(word == "version_with_incrementers") continue;
		sname = word;
		string txt = sname + "\n";

		nr++;
		auto current_byte  = dfile.tellg();

		auto tmp = to_string(nr) + ") " + sname;

        ui->text_raport->append(tmp.c_str());
		ui->progressBar->setValue(current_byte);
		ui->progressBar->update()	;

		// if(nr > 50) break;   // temporarily
		if(flag_first_errors && error_nr >100) {
			report << "\n\nThese are first 100 errors found. Now, start correcting the spy source code, then run it, and come back here again to repeat the check.";
			break;   // was: 100
		}

		if(!(nr %10) ) update();



		// check 1D, 2D
		if(sname.substr(sname.size() - 4) == ".spc") dim = 1;
		else if(sname.substr(sname.size() - 4) == ".mat") dim = 2;
		else {
			{
				cout << "error - I can't recognize the dimmension of " << sname << endl;
				exit(1);
			}
		}
		// incrementer information starts after second  '{'
		getline(dfile, word, '{');
		getline(dfile, word, '{');

		string inkrem_set;
		getline(dfile, inkrem_set, '}');

		// cout << "now inkrem_set is " << inkrem_set << endl;

		// skip user defined
		if(sname.substr(0,5) == "user_")
			continue; // ??
		if(flag_ignore_fan_spectra)
		{
			// cout << "checking for fun sname = " << sname << endl;
				if( sname.find("_fan") != string::npos)
					continue;
		}


		// After':'  add a space, because it should not be 'sticked' to a following incrementer
		string::size_type where = 0 ;
		while(1)
		{
			where = inkrem_set.find(":", where+1);
			if(where != string::npos)
			{
				inkrem_set.insert(where+1, " ");  // adding space after
			}
			else break;
		}

		//----------------------------------------------------------------------------------------------------------------------------

		istringstream s(inkrem_set);
		[[maybe_unused]] bool flag_x = false;

		bool flag_y = false;
		bool flag_info = false;
		bool flag_error_entry = false;
		ostringstream prepared;
		bool flag_No_such_incrementer_defined = false;

		while(s >> word)    // scaning incrementers description ----------------------------------------------
		{
			// analyse the set of incrementers
			// skip "X:"
			if(word == "X:") { flag_x = true; continue;}

			// skip "Y:"
			if(word == "Y:") {flag_y = true; continue;}

			// skip "INFO:"
			if(word == "INFO:") {
				flag_info = true;
				// getline(s, info_txt, '}');    // we skip everything what is after?
				getline(s, word, '}');    // we skip everything what is after?
				break; // continue;
			}

			// skip words which do not have '_' inside
			//			if(word.find("_") == string::npos){
			//				//strange_words += wyraz + "\n";
			//				continue;
			//			}

			// Remove coma at the end (or period).
			where = word.find(',');
			if(where != string::npos)
				word.erase(where);


			// If we are here, it may be an incrementer

			// check if such a incrementer exists
			//			if(wyraz == "kratta_multiplicity")
			//			{
			//				cout << "Warning!" << endl;
			//			}

			dlg->set_filter(word);
			auto list = dlg->give_filtered_items();

			if(list.size() == 1)
			{
				// cout << "Good, this incrementer " << wyraz << " was found as official" << endl;
				// if yes, continue to check another word
			}
			else if(list.contains(word.c_str(), Qt::CaseInsensitive))
			{
				// that can be more than one incrementer found by the filter and put on the list,
				// but if one of them is exactly equall to the filter - it means that this is a good incrementer
			}
			else   // else situation of error. Such a word is NOT on the list of incrementers
			{
				if(word == "No_such_incrementer_defined" )
				{
					// cout  << word << " warning! " << endl;
					flag_No_such_incrementer_defined = true;
					if (flag_ignore_noinc) {
						getline(s, word, '}');    // we skip everything what is after?
						break;
					}
				}


				++error_nr;
				// if no, write a report,
				flag_error_entry = true;
				prepared << "\ninforms that it has an "
						 << (flag_y ? "Y " : "X ")
						 << " incrementer called : \n\t["
						 << czerw_pocz
						 << word
						 << kolor_kon
						 << "]\nwhich is not recognized as the official one\n";


				// then we suggest a sensible correction of the incrementer description in the Spy
				if(flag_No_such_incrementer_defined == false){
					vector<string> suggestion = suggest_incrementer(dlg, word);

					if(suggestion.size())
					{
						prepared << string(40, '.') << "\nPerhaps you mean:\n";
						for(auto x : suggestion)
							prepared << "\t" << x << "\n";
					}
				}
			}

		} // end while (for scaning inrementers txt) -------------------------------------------------------------------------

		if(flag_No_such_incrementer_defined && flag_ignore_noinc)
			continue;
		if (flag_error_entry) {

			report << string(60, '=')
				   << "\n" << nr << " ) Spectrum called "
				   << nieb_pocz  + sname + kolor_kon
				   << prepared.str()
				   << "\n"
				   << string(50, '-')
				   << "\nJust to compare. Full description of incrementers was: \n" <<  inkrem_set << endl;
		}
	} // end loop for scaning every spectrum =========================================


	string ttt = nieb_pocz + " RESULTS"  + kolor_kon + "<br><br>";
    //ui->text_raport->append( ttt.c_str());

	string reptxt = ttt + to_string(error_nr) + "  errors found \n" + report.str();

	// change every \n into <br> ------------------------------------
	string what {"\n"};
	string to {"<br>"};
	while(1)
	{
		auto pos = reptxt.find(what);
		if(pos == string::npos) break;
		reptxt.replace(pos, what.size(), to);
	};

    //	ui->text_raport->append(reptxt.c_str() );
    //	ui->text_raport->append("###########################################\n\r");


	string rt = R"(<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd">
				<html><head><meta name="qrichtext" content="1" /><style type="text/css">
				p, li { white-space: pre-wrap; }
				</style></head><body style=" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;">
				<p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">)";
	rt += reptxt;
    ui->text_raport->setText(rt.c_str() );

	// put scrolling to the top!!!!!!!

	delete dlg;

	ui->pushButton_start->setEnabled(true);
	ui->pushButton_start->setText(save_tekst);
	ui->label_working->setVisible(false);
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);

}
//*******************************************************************************************************************
std::vector<string> T4checking_incrementers_dlg::suggest_incrementer(T4incrementer_selector * dlg, string filter)
{
	string wyraz = filter;
	vector<string> candidates_smart;

	if(wyraz.size() > 2){

		dlg->set_filter(wyraz);
		QStringList list ; // = dlg->give_filtered_items();  // fake, just to have auto

		int p = 0;
		for(p =  filter.size() ; p > 0 ; --p)
		{
			wyraz = "*" + filter.substr(0, p) + "*";
			dlg->set_filter(wyraz);
			list = dlg->give_filtered_items();
			//		cout << i << " Checking filter: [" <<wyraz << "] -> gives counts:"
			//			 << list.count() << endl;
			if(list.count() > 0)
				break;
		}



		vector<string> candidates_general;

		if(wyraz.size() > 2)
		{

			candidates_general.push_back( "\nGENERAL SUGGESTION   -  ( using filter: " +wyraz + ")");


			for( auto it = list.begin(); it != list.end(); ++it)
			{
				candidates_general.push_back( (*it).toStdString());
			}

		}else {
			candidates_general.push_back("Too short filter " + wyraz);
		}

		//-----------------------------------------------------------------------------------------------------------------------------------
		string smart_filter = smart_suggestion(filter, p);
		smart_filter = wyraz + smart_filter;

		dlg->set_filter(smart_filter);
		list = dlg->give_filtered_items();


		candidates_smart.push_back( "\nSMART SUGGESTION    -  ( using filter: " +smart_filter + ")");
		if(!list.count() || list.count() > 15)
			candidates_smart.push_back("? ? ?");
		else {
			candidates_smart.push_back(ziel_pocz);
			for( auto it = list.begin(); it != list.end(); ++it)
			{
				candidates_smart.push_back( (*it).toStdString());
			}
			candidates_smart.push_back(kolor_kon);
		}

		// adding gereral candidates to the and of smart candidates--------------------------------------
		for(auto x : candidates_general)
		{
			candidates_smart.push_back(x);
		}

		// all candidates
		//	for(auto x : candidates_smart)
		//		cout << x << endl;
	}else{
		cout << "filter " << filter << " to short for suggestions" << endl;
	}
	return candidates_smart;

}
//********************************************************************************************************************************
string T4checking_incrementers_dlg::smart_suggestion(string incrementer, int pocz)
{	
	// cout << "\nLooking a incrementer: " << incrementer << endl;
	// checking following strings

	vector<string> keywords
	{
		"energy", "time", "raw", "cal", "fired", "good",
		"gamma", "doppler", "theta", "phi",
		"when", "multiplic", "fan" ,
		"stamp",   // timestamp
		"amplitude",
		"slow",
		"fast",
		"compton", "No_such_incrementer_defined"
	};
	vector<pair<int, string>> position;

	for(int i = 0 ; i < (int) keywords.size() ; ++i)
	{

		//auto pos = filter.find(keywords[i]);
		QString word = incrementer.c_str();
		QString what = keywords[i].c_str();
		int where = word.indexOf(what, pocz, Qt::CaseInsensitive);
		if(where > -1)
		{
			position.push_back(pair(where, keywords[i]) );
		}
	}

	// sorting vector of positions according to first in pair
	sort(position.begin(), position.end() );

	string new_filter;
	for(auto x : position)
	{
		// cout << x.first << ", --> " << x.second << endl;
		new_filter += "*" + x.second ;
	}

	//cout << "new filter = " << new_filter << endl;
	return new_filter;
}
//***********************************************************************************************************************
void T4checking_incrementers_dlg::on_checkBox_first_errors_clicked(bool checked)
{
	flag_first_errors = checked;
}
//***********************************************************************************************************************
void T4checking_incrementers_dlg::on_buttonBox_rejected()
{
	cout << __PRETTY_FUNCTION__ << endl;
}
//***********************************************************************************************************************
void T4checking_incrementers_dlg::on_checkBox_ignore_noinc_toggled(bool checked)
{
	flag_ignore_noinc = checked;
}

void T4checking_incrementers_dlg::on_checkBox_ignore_fan_spectra_toggled(bool checked)
{
	flag_ignore_fan_spectra = checked;
}


#endif
