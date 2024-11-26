#include "t4welcome.h"
#include "ui_t4welcome.h"

#include "paths.h"
//extern paths path ;
#include <QMessageBox>
//#include <cstdlib>
using namespace std;
extern int general_font_size ;
//************************************************************************************
T4welcome::T4welcome(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::T4welcome)
{
	ui->setupUi(this);
	load_tips();

	ui->textLabel_path->setStyleSheet("color : red");
	ui->textLabel_path->setText(
				QString("   %1").arg(gpath.Qspectra() )) ;

	//ui->textEdit_tip->setBackgroundColor(QColor(193, 193, 193)) ;
	ui->textEdit_tip->setPlainText(give_tip().c_str());
}
//************************************************************************************
T4welcome::~T4welcome()
{
	delete ui;
}
//************************************************************************************

void T4welcome::load_tips()
{
	//tip.push_back("Jurek Grebosz wish you a nice day ");
	tip.push_back("You can define interactively a SUM spectrum. \n"
				  "To make this, look for a button in the \"select spectra\" dialog.\n"
				  "A sum spectrum is not a normal spectrum. It is just a list of spectra "
				  "which are summed every 30 seconds and the result is shown on the screen\n"
				  "You can save such a 'result' on the disk as a normal ASCII spectrum.\n"
				  "Note: You can sum only the spectra which have the same binning.\n\n"
				  "Actually you can choose how often summing is performed\n"
				  "by setting corresponding time value in the dialog window:\n"
				  "     Preferences->Setting times of refreshing\n"
				  );

	tip.push_back("If you delete the spectra, you actually only ask the SPY to delete them.\n"
				  "The spy program does not make this at once, you need to wait a few seconds "
				  "to see the result on the screen."
				  );


	tip.push_back("Spy saves every 10 seconds the spectra which you just observe on the screen.\n"
				  "All spectra are saved by the spy if you press \"pause\" icon "
				  "or \"update\" icon or you finish the program."
				  );

	tip.push_back("While watching 1D or 2D spectra, you can call a context menu by clicking"
				  " the right button of the mouse on the spectrum."
				  );

	tip.push_back("Next to the horizontal slider there are two toggle icons which change the mode "
				  "of your operations on the spectra. If the left one is active - all actions as shifting the "
				  "spectrum, scaling down etc. are applied only to the one, selected spectrum. If the other "
				  "icon is active - these changes are applied to all the spectra currently displayed on the screen."
				  );


	tip.push_back("You can integrate peaks in all currently displayed spectra if the icon "
				  "'apply changes to all spectra windows' is active ");

	tip.push_back("You can copy you integration markers and background markers "
				  "to the other 1D spectra currently visible on the screen. "
				  "To make this, use the option Spectrum->Copy integration markers...");

	tip.push_back("You can define polygon gates on any 2D spectrum. "
				  "The name which you give to your gate it is modified to make it unique. "
				  "You can see the real, modified name of such gate if you select the option:\n"
				  "Matrix->Show full names of polygon gates"
				  );

	tip.push_back("If you look at some set of spectra very often you can define a 'group'."
				  "Such a group is just a list on names of choosen spectra. The real advantage of creating "
				  "the group is that you can recall the members of such a group by just one click "
				  "on the main aplication window ('GROUP NAMES' combo box).\n "
				  "You can define a group inside the 'select spectra dialog'.");

	tip.push_back("How to delete ALL your spectra? \n"
				  "Go to the 'select spectra dialog', select all your spectra into the list 'choosen spectra' "
				  "and then click the button:\n    'Send request to the spy to zero choosen spectra'.");

	tip.push_back("You can decide how often to refresh the spectra on the screen\n"
				  "by setting time values in the dialog window:\n"
				  "     Preferences->Setting times of refreshing\n"
				  "There are separate values for refreshing \n     - normal spectra, \n     - 2D spectra (matrices)\n"
				  "     - the SUM spectra.\n"
				  "Note: By this option you decide just how often the spectrua are read from the disk "
				  "(but not how often the SPY produces them).");

	tip.push_back("The fastes way to operate the scale of the spectrum are the keys:\n"
				  "    Y   - adjust the scale to the maximal peak\n"
				  "    +   - to increase spectrum\n"
				  "    -   - to decrease spectrum\n\n"
				  "use the Numerical Keypad + or -, it is easiest to touch\n"
				  "!!! NEW:    You can also use the mouse wheel");

	tip.push_back("To see the full spectrum press the key 'A' (all)\n"
				  "this is for 1D or 2D spectrum");

	tip.push_back("To magnify the part of the 2D spectrum (matrix) press F2 and then start "
				  "dragging a mouse across the favourite region - you will see a rubberband\n\n"
				  "Another way to 'zoom in' is to make double click in the interesting region of the matrix.\n"
				  "To zoom out - just double click while pressing the SHIFT key");

	tip.push_back(
				"If you prefer another binning of some spectrum - you can change it manually.\n"
				"1. Go to the subdirectory 'my_binnings'\n"
				"2. Create the file with the name exactly the same as your spectrum, but replace "
				"the extension with the extension '.binning'\n"
				"3. Edit this file. In case of 1D spectrum put there 3 numbers:\n"
				"   a)  bin   - how many bins you would like to have\n"
				"   b)  begin - left edge of the first bin of your spectrum\n"
				"   c)  end   - righ edge of the last bin of your spectrum\n"
				"If your spectum is 2D (matrix) you have to provide 2 sets of these numbers "
				"(one describing the binning of the X axis, the other for the Y axis)\n"
				"While editing such file - you can use comments.    // comment \n"
				"NOTE: \n"
				" Changing the binning is a very drastic change. The spectra are created by"
				" the spy in the moment when it starts. So if you make your wish about new binning - "
				" spy will read your wish only when you start it once more. "
				" (It is not sufficient to make SPY pause/continue)"
				);

	tip.push_back(
				"In the select spectra dialog - you have two possible modes "
				"of selecting items (spectra) from the list.\n"
				"1. In the 'default mode' every spectrum name clicked - makes it highlighted (selected).\n"
				"   You can select many spectra names - just by clicking on them\n\n"

				"2. The 'Ctrl/Shift mode' is known to all, who use MS Windows:\n"
				"Every spectrum name clicked - makes it selected. But the previously cliked are deselected.\n\n"
				"If you would like to select more than one name (sure!) - you have to click the next names "
				"while pressing Ctrl key.\n"

				"If you would like to select long list of spectra - you have to click the first spectrum name "
				"then you go to the last and click on it while pressing the Shift key");


	tip.push_back(
				"The filter of the names - in the \"select spectra dialog\" has its history list.\n"
				"On this list you can find all previously used filter strings.\n"
				"(The filter string enters the history list - if you use it "
				"to select any spectrum).");

	tip.push_back(
				"If you want to move one vertex of the polygon gate - just click it and drug.\n"
				"If you want to move whole polygon (all vertices) - press Shift key, click one vertex"
				" and drag it - you will see that all other verices are moving as well.");

	tip.push_back(
				"Format of the spy spectra:\n"
				" 2D spectra (matrices) are written as the binary numbers\n"
				" 1D spectra are also written in binary format, but there are two extra numbers"
				" in front :\n"
				"      -  the first number (double) describes the left edge of the frist bin,\n"
				"      -  second number (double) describes the width of the bin.\n"
				" After this you have all (long int) values of your bins." );


	tip.push_back(
				"You may see constantly updated report  about matching of the events\n"
				"(how many of which combinations were possible)\n"
				"Go to dialog window:\n\n"
				"   Spy analysis -> Parameters of subevent matching\n\n"
				"and watch the 'tab':\n"
				"  Report from current (last) matching.\n"
				"The report is updated every 10 seconds \n" );

	tip.push_back(
				"While watching 1D spectrum you may use a mouse WHEEL."
				"this acts on the scale of a counts number\n" );
	tip.push_back(
				"New option. You can move more than one vertex of the polygon gate. \n"
				"1) Select some of them by Click+Shift (or by marking a region with a mouse)\n"
				"2) Press Ctrl+Shift - and start dragging a mouse. The selected vertices will start moving" );

	tip.push_back(
				"New option. You can select many vertices of the polygon gate just by clicking and dragging over them \n"
				"To do this:\n"
				"Left click a mouse in one corner of the chosen region and dragg it. You will see a rubberband. \n"
				"Release a button of the mouse.\n"
				"All vertices which are inside the region will be selected\n"
				"If you do this while pressing Shift key, the selection will be added to the already existing selection\n");


	tip.push_back( "New option: Now you can make a new user-defined spectrum based on the build-in spectrum\n"
				   "This option is available as a button in the user-defined spectra manager)");


	tip.push_back( "New Tool for Spy programmer: checking if description of incrementers in the build-in spectra are true\n"
				   "Menu Bar --> Tools");

	tip.push_back( "New option: There is an option for making a castom tiling of the screen\n"
				   "Menu Bar -> Window -> Custom tiling");

	tip.push_back( "New: In the Spectra selection dialog you can drag spectra names on the 'green' list of chosen spectra.\n"
				   "By this you can rearrange the order of displaying them on the screen");


	// is anybody really reading this?

}

//**************************************************************************
string T4welcome::give_tip()
{
	static vector<int> seen_tips ;

    int nr ;
	do
	{
		//          cout << "Tip size = " <<tip.size()
		//               << " You have seen already " << seen_tips.size() << " tips "<< endl;
		if(tip.size() == seen_tips.size())
		{
			seen_tips.clear() ;
			return "Congratulations - You have seen already all currently prepared tips.\n"
				   "Since now I will repeat them." ;
		}

        nr = rand() %tip.size() ;
		bool look_once_more = false ;

		for(unsigned i = 0 ; i < seen_tips.size() ; i++)
		{
			if(nr == seen_tips[i])
			{
				look_once_more = true ;
				//                    cout << "You know already tip nr " << nr << endl;
				break ;
			}
		}
		if(look_once_more) continue ;
		// it was not
		else
		{
			//               cout << "      You dint' see yet the tip nr " << nr << endl;
			seen_tips.push_back(nr);
			break ;
		}
	}while(1);

	return tip[nr] ;

}
//***********************************************************************************
void T4welcome::on_push_next_tip_clicked()
{
	ui->textEdit_tip->setPlainText(give_tip().c_str());
}
//************************************************************************************
void T4welcome::on_push_begin_clicked()
{

	QString tekst = QString("To see your spectra, go to the   Edit->Selecting spectra\n"
							"or   Choose the group name from the 'Group names' combo box"
							"\nNote: Currently in 'preferences' the path to the directory with your spectra is defined as:\n"
							"%1").arg(gpath.Qspectra() ) ;

    QMessageBox::information ( this, "How to begin", tekst, "OK", nullptr, nullptr);
}
