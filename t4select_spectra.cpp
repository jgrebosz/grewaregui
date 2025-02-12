#include "t4select_spectra.h"
#include "ui_t4select_spectra.h"

#include "tplate_spectrum.h"
#include "paths.h"
#include "options.h"

//extern paths path;
extern options current_options ;

#include "appl_form.h"
extern appl_form  *appl_form_ptr;

#include<QProgressDialog>
#include<QDir>
#include <QMessageBox>
#include "spectrum_1D.h"
#include "spectrum_2D.h"
#include <QFileDialog>
#include <QInputDialog>
//#include <Q3ListBox>
#include <QMdiArea>
#include <QTableWidgetSelectionRange>
#include <QCompleter>

#include "t4sum_spectra_dialog.h"
// #include <stack>

// #define QRegExp QRegularExpression
//********************************************************************************************************
bool checkPattern(const QString& str, const QString& pattern, bool caseSensitive)
{
    QRegularExpression regex = zbuduj_regexpattern(pattern, caseSensitive);

    // Sprawdzamy dopasowanie
    QRegularExpressionMatch match = regex.match(str);
    return match.hasMatch();
}
//********************************************************************************************************
QRegularExpression zbuduj_regexpattern(const QString& pattern, bool caseSensitive)
{
    // Zamiana gwiazdki "*" na wyrażenie regularne ".*"
    QString regexPattern = pattern;
    regexPattern.replace('*', ".*");

    // Tworzymy obiekt QRegularExpression z przekształconego wzorca
    QRegularExpression::PatternOptions options =
        caseSensitive ? QRegularExpression::NoPatternOption: QRegularExpression::CaseInsensitiveOption ;
    QRegularExpression regex(regexPattern, options);

    return regex;
}
//***************************************************************************************************
#define WHERE  //  cout << __func__ << " in line " << __LINE__ << endl;

string ROWS_COLUMNS_KEYWORD ("rows_columns");

//***********************************************************************

T4select_spectra::T4select_spectra(QWidget *parent) :
    //    QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowContextHelpButtonHint | Qt::WindowCloseButtonHint),
    QDialog(parent),
    ui(new Ui::T4select_spectra)
{
    //  auto previous = windowFlags ();

    //  setWindowFlags(
    //        Qt::CustomizeWindowHint |
    //        Qt::WindowTitleHint |
    //        Qt::WindowSystemMenuHint |
    //                  Qt::WindowMinMaxButtonsHint |
    //                  Qt::WindowContextHelpButtonHint |

    //                 // Qt::WindowCloseButtonHint |
    //              previous
    //                );
    ui->setupUi(this);
    init();
    ui->Button_show_exit-> setStyleSheet( "background-color:darkGreen ;"
                                          "color:yellow");
}
//***************************************************************************
T4select_spectra::~T4select_spectra()
{
    delete ui;
}
//***************************************************************************
void T4select_spectra::init()
{
    /*
     This enum specifies what the QComboBox should do when a new string is entered by the user.
     QComboBox::NoInsertion - the string will not be inserted into the combobox.
     QComboBox::AtTop - insert the string as the first item in the combobox.
     QComboBox::AtCurrent - replace the previously selected item with the string the user has entered.
     QComboBox::AtBottom - insert the string as the last item in the combobox.
     QComboBox::AfterCurrent - insert the string after the previously selected item.
     QComboBox::BeforeCurrent - insert the string before the previously selected item.
     */
    //     ui->ComboBox_filter->setInsertionPolicy ( QComboBox::BeforeCurrent );

    ui->ComboBox_filter->completer()-> setCaseSensitivity(
                flag_case_sensitive? Qt::CaseSensitive : Qt::CaseInsensitive) ;

    ui->checkBox_case_sens->setChecked(flag_case_sensitive);

    // setting the  enabled options
    extended_selection_mode ( false ) ;

    ui->ListBox_available-> clear () ;
    ui->ListBox_chosen-> clear () ;
    ui->Check_add_overwrite->hide();
    on_ListBox_chosen_itemSelectionChanged();
    ui->checkBox_anti->setChecked(false);

    ui->lineEdit_anti_filter->setEnabled(false);
    ui->lineEdit_anti_filter->setHidden(true);

    //--------------

    ui->checkBox_enable_drag->setCheckState(Qt::Unchecked);

}
//****************************************************************************
void T4select_spectra::set_parameters ( QMdiArea  * ws )
{
    arch_ws = ws ;   // can be used by other member    functions

    // As sometimes this take very long - we show progress dialog
    int how_many_items = 100;
    QString infor ( "Some shared disk directories are very slow!\n"
                    "If this is your case - consider moving your analysis "
                    "into the /scratch.local direcory\n"
                    "Reading the directory: " );
    infor += gpath.Qspectra() + "..." ;

    QProgressDialog progress ( infor,
                               "Abort reading",
                               0,
                               how_many_items,
                               this);

    //    QProgressDialog progress("Copying files...",
    //                             "Abort Copy",
    //                             0,
    //                             numFiles,
    //                             this);

    //     progress.setWindowModality(Qt::WindowModal);


    //progress.setMinimumDuration ( 2000 ) ; // miliseconds
    //progress.setValue ( 10 );
    //qApp->processEvents();

    //--------------------------------------------------------
    // taking the spectra names from the directory - only
    // opening all spectra files from current directory
    QDir katalog ( gpath.Qspectra() ); // ("spectra") ;

    //progress.setValue ( 11 );
    //qApp->processEvents();

    how_many_items = katalog.count() ;
    //    progress.setTotalSteps ( how_many_items );


    QStringList filters;
    filters << "*.spc" <<  "*.mat";
    katalog.setNameFilters(filters);

    QStringList lista = katalog.entryList (  ) ;

    //   int tmp_ile =  lista.count();
    //   cout << "tmp_ile = " << tmp_ile << endl;


    //    QStringList lista = katalog.entryList ( "*.spc *.mat" ) ;

    int nr = 0 ;
    for ( QStringList::Iterator it = lista.begin() ; it != lista.end() ;  ++it, nr++ )
    {
        progress.setValue ( nr );
        if ( ! ( nr % 100 ) )
        {
            //                 cout << "nr = " << nr << endl;

            //qApp->processEvents();
        }
        if ( progress.wasCanceled() )
            break;
        //... list one file

        //cout << (*it).toStdString()  << "\n";
        ui->ListBox_available->addItem ( *it );
        all_spectra_names += *it ;
    }
    progress.setValue ( how_many_items );
    //------------------------------------------------------------
    if ( lista.begin() == lista.end() )
    {
        ui->ListBox_available->addItem ( "No spectra in the directory:" );
        ui->ListBox_available->addItem ( gpath.Qspectra() );
        ui->ListBox_available->addItem ( "Most probably the path to the directory of SPY was wrongly set" );
        ui->ListBox_available->addItem ( "Go to menu bar:  Preferences->Path to spy" );
    }
    refresh_defined_groups();

}
//********************************************************************************************
void T4select_spectra::select_one_pressed()
{

    // WHERE;
    QList<QListWidgetItem*>  lista = ui->ListBox_available->selectedItems();
    int nr = 0;
    for (  QList<QListWidgetItem*>::Iterator it = lista.begin() ; it != lista.end() ;  ++it, nr++ )
    {

        // cout << "selected is ..." << ui->ListBox_available->item(i)->text() << endl ;
        // ui->ListBox_chosen->addItem (ui->ListBox_available-> currentText () );
        ui->ListBox_chosen->addItem ( (*it)->text() );

        (*it)->setSelected(false);
    }


    if ( ui->ListBox_chosen->count() > 80 )
    {
        QMessageBox::warning
                ( nullptr, "A lot of chosen spectra" ,
                  QString ( "Warning: Your list of chosen spectra has %1 item now" ).arg ( ui->ListBox_chosen->count() ),
                  QMessageBox::Yes | QMessageBox::Default,
                  QMessageBox::NoButton );
    }
    add_to_history ( ui->ComboBox_filter->currentText() ) ;

}
//*********************************************************************************************
void T4select_spectra::show_chosen_spectra()
{
    // cout << "F. " << __PRETTY_FUNCTION__  << endl ;

    // 1. here we close existing windows - or not
    QList <QMdiSubWindow*> windows = arch_ws->subWindowList();

    if ( !ui->Check_add_overwrite->isChecked() )
    {
        for(int nr = 0 ; nr < windows.count() ; nr++)
        {
            Tplate_spectrum * dokument  = ( Tplate_spectrum * ) windows[nr]->widget() ;

            if ( dokument )
            {
                dokument->close() ;
                dokument->destroy() ;  // my destructor
                //delete dokument ;     <--- with this tile() will crash
            }
        }
    }
    qApp->processEvents ();

//    cout << "F. " << __PRETTY_FUNCTION__  << " line " << __LINE__<< endl ;
    // Perhaps too many spectra  ?  ---------------------------------------------------------
    if ( ui->ListBox_chosen->count() > 100 )
    {
        QString list_of_spectra= QString ( "You asked to show %1"
                                           " online SPY spectra \n"
                                           "More than 70 spectra on the screen - it is not recommended (hence generally possible)\n\n"
                                           ).arg ( ui->ListBox_chosen-> count() );

         showWarningMessage(" This would make the program very slow !!! ",
                               list_of_spectra);
        // return ;
    }

    time_t moment = time ( nullptr ) ;
    current_options.set_how_many_spectra_on_the_screen_now ( ui->ListBox_chosen->count() );

    // adding the new spectra windows ------------------------------------------------

#define INVERTED_ORDER false

#if INVERTED_ORDER
    //    cout << "Inverting the order of spectra" << endl;
    for ( int  i = (int) ui->ListBox_chosen->count()-1 ; i >= 0  ;  --i )
#else
    for ( int i = 0 ;  i < (int) ui->ListBox_chosen->count() ;  ++i )
#endif
    {

        string spectrum_filename ;
        spectrum_filename =  ui->ListBox_chosen->item(i)->text ().toStdString () ;
        QString fn =  ui->ListBox_chosen->item(i)->text () ;

        //                cout   << "spectrum name = " << spectrum_filename << endl;

        Tplate_spectrum * s = nullptr ;

        // looking for an extension
        if ( fn.contains ( ".spc", Qt::CaseInsensitive ) ) // <-- false=case insensitife
        {
            //cout << "you selected  a spectrum .spc" << endl;
            s = new spectrum_1D ( arch_ws, fn );

        }
        if ( fn.contains ( ".mat", Qt::CaseInsensitive ) )
        {
            //             cout << " you seleted  a matrix (.mat)" << endl;
            s = new spectrum_2D ( arch_ws, fn );
        }

        if (s)
        {
            appl_form_ptr->addSubWindow(s, Qt::Window);
            // the name is taken form t
            s->set_the_name ( fn );
            s->read_in_file ( ui->ListBox_chosen->item(i)->text().toStdString().c_str() , true ); // true <- this is first time, full, new markers etc.

            s->showNormal();
            //cout << "fileOpen, if(s) true, " << endl ;
        }

        // ???
        if ( time ( nullptr ) - moment > 2 ) // for longer times we put something on the screen
        {
            qApp->processEvents ();
            moment = time ( nullptr ) ; // now more often
        }
    }
//    cout << "End of F. " << __PRETTY_FUNCTION__  << endl ;
}
//********************************************************************************
void T4select_spectra::remove_highlighted()
{
    // WHERE;
    //cout << "remove_highlighted() z " << ui->ListBox_chosen-> count() << endl ;

    QList<QListWidgetItem *> list_to_remove  = ui->ListBox_chosen->selectedItems ();

    for (  int i = list_to_remove.count() -1; i >=0 ; i-- )
    {
        ui->ListBox_chosen->removeItemWidget(list_to_remove[i] );
        delete list_to_remove[i];  // seems that this instruction above does not work.
        //  ui->ListBox_chosen->takeItem(int row) ;   // this could be another opion because  removeItemWidget does not work
    }

    //    //ui->ListBox_chosen->firstItem() ;
    //    for (  int i = ui->ListBox_chosen-> count()-1; i >=0 ; i-- )
    //    {
    //        //cout << "is selected ..." << i  ;
    //        if ( ui->ListBox_chosen-> item( i )->isSelected() )
    //        {
    //            cout << "   Yes selected item nr " << i << " is ..."
    //                 << ui->ListBox_chosen->item(i)->text().toStdString() << endl ;

    //           QListWidgetItem* w =  ui->ListBox_chosen-> item(i)   ;
    //            ui->ListBox_chosen->removeItemWidget(w);
    //            cout << "now item counter = " <<     ui->ListBox_chosen-> count() << endl;
    //        }
    //        else {
    //            cout << "   NO ..." << i  << ") "
    //                 << ui->ListBox_chosen->item(i)->text().toStdString() << endl ;
    //        }
    //    }

    ui->ListBox_chosen->clearSelection ();
}
//********************************************************************************
void T4select_spectra::remove_all()
{
    ui->ListBox_chosen-> clear () ;
}
//*******************************************************************************
void T4select_spectra::define_group()
{

    // perhaps we should stop repainting screen?
    Tplate_spectrum::flag_repainting_allowed = false;
    QString selected_filter;
    QString fileName =
            QFileDialog::getSaveFileName(this,
                                         "Select the name for your group",
                                         gpath.Qgroups(),
                                         "group of spectra names  (*.group)",
                                         &selected_filter
                                         );

    Tplate_spectrum::flag_repainting_allowed = true;
    if ( !fileName.isEmpty() )
    {
        // Checking if it has  extension group
        string name = fileName.toStdString();
        string extension = ".group";
        QString fname;

        if ( name.substr ( name.size() - extension.size(), extension.size() ) != extension )
        {
            fname = fileName  + ".group" ;
        }
        else
        {
            fname = fileName ;
        }


        ofstream plik ( fname.toStdString() ) ;

        // user entered something and pressed OK
        //cout << tekscik << " will contain spectra " << endl ;

        //            ui->ListBox_chosen->firstItem() ;

        for ( int i = 0 ; i < ui->ListBox_chosen-> count() ; i++ )
        {
            //cout <<  " tresc " <<  (ui->ListBox_chosen->item(i)->text() )  << endl ;
            plik << ( ui->ListBox_chosen->item(i)->text().toStdString() ) << endl ;
        }

        // any desired display pattern?

        // Do you want any special

        int odp =  QMessageBox::question(this,
                                         "Overwrite ?",
                                         "Do you want a custom tiling of spectra?",
                                         QMessageBox::Yes,
                                         QMessageBox::No
                                         );
        switch(odp)
        {
        case QMessageBox::Yes:
        {

            // note: we ask for columns only. ROWS will be calculated - "as needed"
            int columns = 0 ;
            bool ok;
            int res = QInputDialog::getInt(this,
                                           "Define custom tiling ", "How many columns of spectra: \n (leave 0 - if no special wish)",
                                           0 , 0, 50, 1,
                                           &ok);
            if ( ok ) {
                // user entered something and pressed OK
                columns = res;

                // user entered something and pressed OK
                columns = res;


            } else {
                // user pressed Cancel for columns
            }

            if(columns){
                plik << ROWS_COLUMNS_KEYWORD << "\n 0" << " " << columns << "\n";

                QMessageBox::information( this,
                                          " Information ",
                                          "Note, that the tiling information will be used only when"
                                          " display the group by pressing its name in the Groups Combo Box",
                                          QMessageBox::Ok| QMessageBox::Default);
            }
        }
            break;

        default:
        case QMessageBox::No:

            break;
        } // end switch odp - special tiling

    }
    else
    {
        ;// user entered nothing or pressed Cancel
    }

    //cout << "Nazwa " << fileName << endl ;


    refresh_defined_groups();

    ui->Button_group->setDown ( false );
    ui->Button_group->show();
}
//********************************************************************************
void T4select_spectra::combo_groups_activated( )
{

    QString mystring =  gpath.Qgroups() + ui->Combo_groups->currentText() ;
    mystring += ".group"  ;
    ifstream plik ( mystring.toStdString() );
    char linia[500] = ""  ;
    while ( plik )
    {
        plik.getline ( linia, sizeof ( linia ) ) ;
        //QString nazwid = linia ;
        if(linia == ROWS_COLUMNS_KEYWORD)
        {
            // after this (tiling numbers), should be nothing
            return;
        }

        if ( plik )
            ui->ListBox_chosen->addItem ( linia );
    }
}
//*******************************************************************************
void T4select_spectra::show_choosen_and_exit()
{
    //cout << " T4select_spectra::show_choosen_and_exit() " << endl;
    //show_chosen_spectra() ;

    done ( QDialog::Accepted ) ;
}
//*******************************************************************************
void T4select_spectra::selected_filter()
{

    // WHERE;
    // we do not react for typing filter more often than every 2 secondes
    // (because in case of 2000 spectra it is slowing down our typing)

    //  static time_t last = time(0);
    //   // just wait 1 seconds
    //   while((time(0) - last) < 1) ;
    //   last = time(0) ;

    QString filtr ;
    //	int nr = ui->ComboBox_filter->currentIndex();
    //	cout << "AT FIRST  Current text is [" << ui->ComboBox_filter->currentText ().toStdString() << "]" << endl;
    //	cout << "           Current index is " << nr << endl;



    //	cout << "List of " << ui->ComboBox_filter->count() << "  items in history:\n" ;

    //	for ( int i = 0 ; i < ui->ComboBox_filter->count() ; i++ )
    //	{
    //		filtr = ui->ComboBox_filter-> itemText (i);
    //		cout << "History  text nr [" << i << "] is : " << filtr.toStdString() << endl;
    //	 }

    //	if(nr >-1)
    //		ui->ComboBox_filter->setCurrentIndex (nr);
    filtr = ui->ComboBox_filter->currentText ();
    //cout << "current   filter text  is now: [" << filtr.toStdString() << "]" << endl;
    ui->ListBox_available->clear();

#if  (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    QStringList found = all_spectra_names.filter (
         zbuduj_regexpattern(filtr, flag_case_sensitive)

        // QRegExp ( filtr,
        //         flag_case_sensitive?   Qt::CaseSensitive : Qt::CaseInsensitive,
        //         QRegExp::Wildcard )
        );
#else


    QStringList found = all_spectra_names.filter (
                QRegExp ( filtr,
                          flag_case_sensitive?   Qt::CaseSensitive : Qt::CaseInsensitive,
                          QRegExp::Wildcard )
                );
#endif
    //    QStringList found = all_spectra_names.filter ( QRegExp ( filtr, Qt::CaseSensitive,QRegExp::RegExp2 ) );


    // anti-filter? ----------------------------------------------

    if( ui->checkBox_anti->isChecked() &&  antifilter.isEmpty() == false)
    {
        //QStringList result;
        QStringList antifiltered;

        for (auto str: found) {
            if (str.contains( QRegularExpression ( antifilter,
                                     //   flag_case_sensitive?   Qt::CaseSensitive : Qt::CaseInsensitive
                                     QRegularExpression::CaseInsensitiveOption
                                     //,
                                       // QRegExp::Wildcard
                                    // QRegExp::DefaultWildcardConversion
                                     )))
            {    /* result += str;*/ }
            else
                antifiltered += str;
        }
        found = antifiltered;
    }
    //-------------------------------------------

    ui->ListBox_available->insertItems(0, found ) ;


}
//***********************************************************************************
void T4select_spectra::delete_group()
{
    bool ok = false;

    //    QString tekscik = QInputDialog::getText(
    //      tr( "Telescope - removing the definiton of a group of spectra" ),
    //      tr( "You are going to remove the definition of the group. \n(This does not delete any spectra"
    //    " but just the definition of a group)."
    //    "Please enter name of the group to be removed" ),
    //      QLineEdit::Normal, QString::null, &ok, this );
    //

    QDir katalog ( gpath.Qgroups() ) ; //
    QStringList filters;
    filters << QString ("*.group");
    katalog.setNameFilters(filters);

    QStringList lista_grup = katalog.entryList () ;
    QString res = QInputDialog::getItem (this,
                                         tr ( "Application name" ),
                                         tr ( "Please select an item" ),
                                         lista_grup,
                                         0,
                                         false,
                                         &ok );
    if ( ok )
    {
        // user selected an item and pressed OK
        switch ( askYesNoCancel(
                                            "Cracow  group definition remover",
                                            QString ( "You are going to delete the definition for the group \n\t %1\n"
                                                      "Are you sure \n\n" ).arg ( res )
                                            ) )
        {
        case QMessageBox::Yes:  // Yes :
            katalog.remove ( res );
            refresh_defined_groups();
            break;
        default: break;
        }
    } // end if ok


    ui->PushButton_Delete_group->setDown ( false );
    ui->PushButton_Delete_group->show();


}
//**********************************************************************
void T4select_spectra::refresh_defined_groups()
{
    //    cout << __func__ << endl;
    //---------------------------------------

    //  now, we can check defined groups
    ui->Combo_groups->clear() ;

    QDir katalog ( gpath.Qgroups() );
    QStringList filters;
    filters << QString ("*.group");
    katalog.setNameFilters(filters);

    QStringList lista_grup = katalog.entryList() ;

    for ( QStringList::Iterator it = lista_grup.begin() ; it != lista_grup.end() ;  ++it )
    {
        //cout << *it << "\n";
        // removing the extension ".group"
        QString mystring = *it ;
        int i = mystring.lastIndexOf ( ".group" );
        mystring.truncate ( i );
        ui->Combo_groups->addItem ( mystring );
    }
}
//***************************************************************************
QStringList T4select_spectra::give_last_filter()
{

    // WHERE;
    //return ui->ComboBox_filter->currentText ();

    QStringList all ;
    //for(int i = 0 ; i < ui->ComboBox_filter->count(); i++ )


    for ( int i = 0 ; i < ui->ComboBox_filter->count() ; i++ )
    {
        ui->ComboBox_filter->setCurrentIndex (i);
        all += ui->ComboBox_filter->currentText() ;
        //all += "\n" ;
    }
    return all ;
}
//************************************************************************
void T4select_spectra::set_filter ( QStringList fff )
{

    // WHERE;
    //  ui->ComboBox_filter->setCurrentText (fff);
    if ( fff.isEmpty() )
        return ;

    ui->ComboBox_filter->addItems(fff);



    //    // take first part, till the newline
    //    string all = fff.toStdString();

    //    string::size_type a = 0 ;
    //    string::size_type b = all.find ( "\n" ) ;
    //    while ( b != string::npos )
    //    {
    //        cout << "inserting to combo ->" << all.substr(a, b-a) << "<-" << endl;
    //        ui->ComboBox_filter->addItem ( all.substr ( a, b-a ).c_str() ) ;
    //        a = b+1;
    //        b = all.find ( "\n", a ) ;
    //    }
}
//***************************************************************************
vector<string> T4select_spectra::give_listbox_chosen()
{
    vector<string> parcel ;
    //ui->ListBox_available->firstItem() ;
    for ( int i = 0 ; i < ui->ListBox_chosen-> count() ; i++ )
    {
        parcel.push_back ( ui->ListBox_chosen->item(i)->text().toStdString() ) ;
    }

    return parcel;
}
//****************************************************************************
void T4select_spectra::set_listbox_chosen ( vector<string> parcel )
{
    // inserting the names of files which are recently placed on the green window
    for ( unsigned int i = 0 ; i < parcel.size() ; i++ )
    {

        // cout << "Initially inserting " << parcel[i].c_str() << endl;
        ui->ListBox_chosen->addItem ( parcel[i].c_str() );
    }
}

//*****************************************************************************
void T4select_spectra::zeroing_choosen_spectra()
{
    //vector<string> parcel ;

    QString list_of_spectra=
            QString ( "You are going to erase the contents of the %1"
                      " online SPY spectra \n"
                      "which are currently on the list 'spectra choosen to display'\n\n"
                      "ARE YOU SURE  ?" ).arg ( ui->ListBox_chosen-> count() );

    switch ( askYesNoCancel (" 'Zero-ing' online spectra, ARE YOU SURE ? ",list_of_spectra) )
    {
    default: break;

    case QMessageBox::Yes: //Yes :

        //cout << "Spectrum to zero = " <<   dokument->caption() << endl;
        string command_file = gpath.commands + "spy_spectra_for_zeroing.command";
        ofstream plik ( command_file.c_str() );
        if ( !plik )
            cout << "problem in openning the file" << command_file << endl;

        //        ui->ListBox_available->firstItem() ;

        QProgressDialog progress ( "Zeroing selected spectra...",
                                   "Abort zeroing",  0, ui->ListBox_chosen-> count(),
                                   this);
        progress.setMinimumDuration ( 5000 ) ; // miliseconds
        progress.setValue ( 0 );
        // int counter = 0;


        if ( ! appl_form_ptr->is_spy_running() )
        {
            // if we do zeroing on disk ourselves, we will need to set the time of zeroing
            appl_form_ptr->restore_times_of_zeroing_from_disk();
        }

        for (int i = 0 ; i < ui->ListBox_chosen-> count() ; i++ )
        {
            //plik << ( ui->ListBox_chosen->item(i)->text() )  << endl;
            progress.setValue ( i );

            qApp->processEvents();
            if ( progress.wasCanceled() )
                break;
            progress.setLabelText ( QString ( "Zero-ing:  " ) + ( ui->ListBox_chosen->item(i)->text() ) );

            if ( appl_form_ptr->is_spy_running() )
            {
                plik << ( ui->ListBox_chosen->item(i)->text().toStdString() )
                     << endl;  // we can ask spy to do it
            }
            else
            {
                appl_form_ptr->zeroing_really_on_disk ( ui->ListBox_chosen->item(i)->text().toStdString() ); // if spy is not running - we do it ourselves
            }


        }
        plik << "this_is_the_end_of_the_list_of_spectra_which_cracow_wants_to_zero" << endl;

        if ( !plik )
            cout << "problem in writing to file" << command_file << endl;
        plik.close();

        if ( ! appl_form_ptr->is_spy_running() )
        {
            // if we do zeroing on disk ourselves, we need to set the time of zeroing
            appl_form_ptr->save_times_of_zeroing_on_disk();
        }
    }
}
//**************************************************************************************
/** select all spectra currently filtered */
void T4select_spectra::select_all()
{
    //    ui->ListBox_available->firstItem() ;
    for (int i = 0 ; i < ui->ListBox_available-> count() ; i++ )
    {
        ui->ListBox_chosen->addItem ( ui->ListBox_available->item(i)->text() );
        ui->ListBox_available->item(i)->setSelected(false);
    }

    if ( ui->ListBox_chosen->count() > 80 )
    {
        QMessageBox::warning
                ( nullptr, "A lot of chosen spectra" ,
                  QString ( "Warning: Your list of chosen spectra has %1 item now" ).arg ( ui->ListBox_chosen->count() ),
                  QMessageBox::Yes | QMessageBox::Default,
                  QMessageBox::NoButton );
    }

    add_to_history ( ui->ComboBox_filter->currentText() ) ;
}
//*************************************************************************************
void T4select_spectra::create_the_sum_spectrum()
{

    if(ui->ListBox_chosen->count() == 0 )
    {
        showWarningMessage(
                                   "Empty list of spectra?",
                                   "The sum spectrum - is build on a set of currently chosen spectra.\n"
                                   "Now this list is empty - so no sense im making a sum");
        return ;
    }


    bool ok = false;



    QString krotka_nazwa_sumu =
            QInputDialog::getText (this,
                                   tr ( "Cracow viewer - defining a new SUM spectrum" ),
                                   tr ( "Currently selected spectra can contribute to the new SUM spectrum.\n"
                                        "NOTE: you can sum only such spectra which have the same binning and ranges\n\n"
                                        "The sum spectrum name should start with prefix '@sum_'.\n"
                                        "It will automatically have an extension '.spc'\n\n",
                                        "Please enter name of the sum spectrum" ),
                                   QLineEdit::Normal,
                                   "@sum_", // QString::null,
                                   &ok);

    QStringList  list_of_names;   // list of spectra contributing to the sum
    QStringList  list_of_factors;   // list of spectra contributing to the sum



    if ( ok && !krotka_nazwa_sumu.isEmpty() )
    {
        // if somebody forgot to give prefix
        string::size_type poz =krotka_nazwa_sumu.indexOf( "@sum_", 0 ) ;
        if ( poz == string::npos )
            krotka_nazwa_sumu.insert ( 0, "@sum_" ) ;

        QString fname = gpath.Qspectra() + krotka_nazwa_sumu ;
        poz = fname.indexOf( ".spc",0 );
        if ( poz == string::npos )
        {
            krotka_nazwa_sumu += ".spc" ;
            fname += ".spc" ;
        }
        QFile plik_check ( fname ) ;
        bool already_exists = plik_check.exists();
        bool use_from_disk = false ;
        if ( already_exists )
        {


            switch ( askQuestionWithButtons(            // +
                                                "Such sum spectrum already exists",
                                                "The sum spectrum definition with such a name was already defined\n"
                                                "Would you like to update the old definition or create the new",

                                                "Edit the previous definition",  // 1
                                                "create the NEW definition",   // 2
                                                "Cancel", 2 ) )

            {

            case 1 :
                use_from_disk = true;
                break ;

            case 2:
                use_from_disk = false;
                break ;

            default:
                return ;
            }
        }
        if ( use_from_disk )
        {
            ifstream plik ( fname.toStdString() ) ;
            string nnn, fff ;
            while ( plik )
            {
                plik >> nnn >> fff ;
                if ( !plik )
                    break;
                list_of_names += nnn.c_str()  ;
                list_of_factors += fff.c_str()  ;
            }
        }
        else
        {

            // depending of the results
            // user entered something and pressed OK
            //cout << tekscik << " will contain spectra " << endl ;

            // we will check the binnings comparing with the first spectrum on the list

            double bin_first = 0,
                    beg_first = 0,
                    end_first = 0 ;
            string spec_first ;



            //            ui->ListBox_chosen->firstItem() ;

            for (int i = 0 ; i < ui->ListBox_chosen-> count() ; i++ )
            {
                //cout <<  " tresc " <<  (ui->ListBox_chosen->item(i)->text() )  << endl ;

                string spec_name = ui->ListBox_chosen->item(i)->text().toStdString() ;

                // is it  1D spectrum ??? --------------------------
                poz = spec_name.rfind ( ".spc" );
                //cout << "poz = " << (int) poz << endl ;

                if ( poz != ( spec_name.length() - 4 ) )
                {
                   showWarningMessage(
                                            "Sum spectra creator warning",
                                            QString ( "The spectrum %1 is not a 1D spectrum with the extension '*.spc'\n"
                                                      "Please remove it from the list and start again" ).arg ( spec_name.c_str() ),
                                            QMessageBox::Critical);

                    // remove the file
                    remove
                            ( fname.toStdString().c_str() );
                    // refreshing the list with all available spectra
                    selected_filter();
                    return ;

                }


                // is it another sum spectrum ??? --------------------------
                poz = spec_name.find ( "@sum_", 0 );
                // cout << "poz = " << (int) poz << endl ;

                if ( poz != string::npos )
                {
                    showWarningMessage(
                                            "Sum spectra creator warning",
                                            QString ( "The spectrum %1 is already a sum spectrum. You can not nest the summing\n"
                                                      "Please remove it from the list and start again" ).arg ( spec_name.c_str() ),
                                            QMessageBox::Critical);
                    // remove the file
                    remove
                            ( fname.toStdString().c_str() );
                    // refreshing the list with all available spectra
                    selected_filter();
                    return ;
                }


                // checking if it is legal - the same binning, the same ranges

                if ( i == 0 )
                {
                    check_common_binnings ( spec_name, &bin_first, &beg_first, &end_first );
                    spec_first = spec_name ;
                }
                else
                {
                    double bin, beg, end ;
                    if ( check_common_binnings ( spec_name, &bin, &beg, &end ) == true )
                    {
                        if ( bin != bin_first ||  beg != beg_first ||  end != end_first )
                        {
                            showWarningMessage(
                                                    "Sum spectra creator warning",
                                                    QString ( "The spectrum %1 has not the same binning as the first one on the list\n\n"
                                                              "%2\nhas bins = %3, begin = %4, end = %5\n\n"
                                                              "%6\nhas bins = %7, begin = %8, end = %9\n\n"
                                                              "Please remove the wrong spectrum from the list and start again"
                                                              ).arg ( spec_name.c_str() )
                                                    .arg ( spec_name.c_str() ).arg ( bin ).arg ( beg ).arg ( end )
                                                    .arg ( spec_first.c_str() ).arg ( bin_first ).arg ( beg_first ).arg ( end_first )
                                                    ,
                                                    QMessageBox::Critical );
                            // remove the file
                            remove
                                    ( fname.toStdString().c_str() );
                            // refreshing the list with all available spectra
                            selected_filter();
                            return ;

                        }
                    }
                }

                //        plik <<  (ui->ListBox_chosen->item(i)->text() ) << endl ;
                list_of_names += ui->ListBox_chosen->item(i)->text() ;
                list_of_factors += "+1";

            }
        } // end if else   from disk/create new

        // now we will call the editor to give the chance to change the factors
        T4sum_spectra_dialog dlg;
        dlg.set_parameters ( &list_of_names, &list_of_factors );
        if ( dlg.exec() == QDialog::Accepted )
        {


            // now we can store the results on the disk
            ofstream plik ( fname.toStdString() ) ;
            QStringList::Iterator itfac =  list_of_factors.begin();
            for ( QStringList::Iterator it = list_of_names.begin()
                  ; it != list_of_names.end() ;  ++it, ++itfac )
            {
                plik << (*it).toStdString()  << "\t"
                     << (*itfac).toStdString() << endl;

                cout << (*it).toStdString()  << "\t"
                     << (*itfac).toStdString() << endl;
            }




            // inform about succes ==================================
            showWarningMessage(
                                       "Sum spectra creator",
                                       QString ( "SUCCES\n\nThe sum spectrum definition %1 was succesfully created.\n\n"
                                                 "Note: \n"
                                                 "Actually the sum spectrum is just a list of names of spectra\n"
                                                 "which will be summed every 10 seconds and the result will be displayed on the screen. \n"
                                                 "If you want to save such a spectrum as a real spectrum,\n"
                                                 "just choose File->SaveAs from the main menu bar).\n\n"
                                                 ).arg ( fname ));

            int how_many = ui->ListBox_chosen-> count() ;

            if ( how_many > 40 )
                showWarningMessage(
                                       "This Sum spectrum can slow down the communication with you",
                                       QString ( "The sum spectrum %1 \ndemands summing %2 spectra\n\n"
                                                 "Warning: \n"
                                                 "Summing hundreds of spectra (every 30 seconds) can dramatically slow down\n"
                                                 "the communication between you and the cracow gui \n"
                                                 "If you really need such a spectrum, consider producing it direcly in the SPY\n"

                                                 ).arg ( fname ).arg ( how_many ));
        } // end of the sum dialog accepted

        ui->ListBox_available->addItem(  krotka_nazwa_sumu);
        ui->ListBox_available->sortItems();
        //        // refreshing the list with all available spectra
        //        selected_filter();


    }
    else
    {
        ;// user entered nothing or pressed Cancel
    }

}
//*********************************************************************
bool T4select_spectra::check_common_binnings ( string spec_name,
                                               double *bin,
                                               double *beg,
                                               double *end )
{

    string desc_name = gpath.spectra + "description_of_ranges.txt";
    ifstream plik ( desc_name.c_str() ) ;
    if ( !plik )
        return false ;

    char wyraz[500] ;
    while ( 1 )
    {
        if ( !plik )
        {
            //      cout << "reading of  " << desc_name << " failed " << endl ;
            break ;
        }
        if ( plik.eof() )
        {
            //,     cout  << desc_name  << " riched the eof " << endl ;
            break ;
        }

        plik.getline ( wyraz, sizeof ( wyraz ) ) ; // because filename can have some spaces inside
        //cout << " read line: " << wyraz << endl ;
        //COTO ;

        if ( strcmp ( spec_name.c_str(), wyraz ) == 0 ) // found  ----------
        {
            plik
                    >> *bin
                    >> *beg
                    >> *end ;
            return true ;
        }
        // try without extension ----------------------

        char *wsk = strrchr ( wyraz, ' ' ) ;
        if ( wsk )
            *wsk = 0 ;
        strcat ( wyraz, ".spc" );
        //cout << " after appending is  " << wyraz << endl ;

        if ( strcmp ( spec_name.c_str(), wyraz ) == 0 ) // found
        {
            plik >> *bin  >> *beg >> *end ;
            return true ;
        }
    }
    //  cout << "Description of the spectrum " << spec_name << " was not found " << endl ;
    return false ;
}
//********************************************************************************
// extended mode is when we want to use CTRL/SHIFT keys - just like in Windows
//********************************************************************************
void T4select_spectra::extended_selection_mode ( bool extended )
{
    ui->ListBox_available->setSelectionMode (
                extended ? QAbstractItemView::ExtendedSelection :QAbstractItemView::MultiSelection) ;
    ui->ListBox_chosen  ->setSelectionMode (
                extended ? QAbstractItemView::ExtendedSelection: QAbstractItemView::MultiSelection ) ;
}
//*********************************************************************
void T4select_spectra::add_to_history ( QString s )
{


    //	WHERE;
    //	cout << "-----------------------------------------\n"
    //		 << __func__ << "    need to add  the text: " << s.toStdString() << endl;

    //	cout << " CHECKING if such a string already is on the list" << endl;

    for ( int i = 0 ; i < ui->ComboBox_filter->count(); i++ )
    {
        //        if ( i == ui->ComboBox_filter->currentIndex () ) // current is new
        //            continue ;

        if ( s == ui->ComboBox_filter->itemText(i) ){
            //			cout << " !!!! tekst " << s.toStdString() << " is already in a fliter as "
            //				 << ui->ComboBox_filter->itemText(i).toStdString() << endl;
            return ; // because it was already there
        }
    }

    // cout << "It is NOT yet on the history list\n";
    // if it is not on the list yet...

    // now, rearrange - to put always on Top

    //	for ( int i = ui->ComboBox_filter->count() -1 ; i >0 ; i-- )
    //	{
    //		ui->ComboBox_filter->itemText ( i ) = ui->ComboBox_filter->itemText ( i-1 );
    //	}


    ui->ComboBox_filter->insertItem ( 0, s) ; // -- add on the top




    //  ui->ComboBox_filter->addItem(s) ;

    //	cout << " At the end of the function here is contents of the history: \n";
    //	   for(int i = 0 ; i < ui->ComboBox_filter->count(); i++ )
    //	   {
    //		 cout << i << ") " <<  ui->ComboBox_filter->  itemText(i).toStdString() << "   " ;
    //	   }
    //	   cout << endl;

    //cout << __func__ << "--------------------------------------- END of history function \n ";
}
//*********************************************************************
void T4select_spectra::select_chosen_as_ascii()
{
    if(ui->ListBox_chosen->count() == 0 )
    {
        showWarningMessage(
                                   "Empty list of spectra?",
                                   "The list of currently chosen spectra is empty now\n"
                                   " - so no sense in saving");
        return ;
    }


    //int answ ;
    switch (
             askYesNoCancel(
                                        "Copy binary spectra into ASCII format",
                                        QString (
                                            "This option takes chosen 1D binary spectra (*.spc) \n"
                                            "and copies them converting into the two column ASCII format (*.asc)\n\n"


                                            "Note: The binary format of spy is:\n"
                                            "     1. double word  - how many bins\n"
                                            "     2. double word  - left edge of the first bin\n"
                                            "     3. double word  - right edge of the last bin\n\n"
                                            "     4. long int word - value in the bin first bin\n"
                                            "     5. long int word - value in the second bin\n"
                                            "\t   ....... and so on, till the contents of the last bin\n\n"


                                            "After this conversion you will have the new ASCII file with two columns of numbers\n"
                                            "     Left column with values of the left edge of the particular bin\n"
                                            "     Right column with the contents of the particlular bin\n\n"

                                            "The two column ASCII format allows you to read the spectrum into "
                                            "such a programs as for example Excel.\n\n\n"

                                            "The result spectra (*.asc) will be stored in the same directory:\n " + gpath.Qspectra()
                                            + "\n\nDo you want to create an ASCII version of the chosen spectra ?"
                                            ) ) )
    {
    default: break;

    case QMessageBox::Yes:  // Yes :

        bool flag_radware = false;
        auto odp =askYesNoCancel("Do you want Radware header?",
                                             QString (
                                                 "If it is a 1D spectrum you may have so called Radware header\n"
                                                 "Do you want a Radware header in 1D spectra?"
                                                 ));

        if ( odp == QMessageBox::Cancel ) // nr 2 is cancel
            return;

        if ( odp == QMessageBox::Yes)
            flag_radware = true;


        int numFiles = ui->ListBox_chosen-> count() ;

        QString mess = QString ( "Copying and converting files" );
        QProgressDialog progress (
                    mess,
                    "Abort Copying",
                    0, numFiles, this);

        progress.show();

        if ( numFiles >0 )
        {

            for (int i = 0 ; i < ui->ListBox_chosen-> count() ; i++ )
            {
                //plik << (ui->ListBox_chosen->item(i)->text() )  << endl;
                if ( ui->ListBox_chosen->item(i)->text().contains ( ".spc" ) )
                {

                    QString mess2 = QString ( "Copying and converting %1   (%2 of %3 files)" )
                            .arg ( ui->ListBox_chosen->item(i)->text() ).arg ( i ).arg ( numFiles );

                    //cout << "progres =  " << i << endl;
                    progress.setLabelText ( mess2 );
                    progress.setValue ( i );
                    qApp->processEvents();

                    if ( progress.wasCanceled() )
                        break;
                    //... copy one file
                    QString old_name = gpath.Qspectra() + ui->ListBox_chosen->item(i)->text() ;
                    QString new_name =  old_name ;
                    new_name =  new_name.replace ( ".spc", ".asc" );
                    //cout << "Saving spectrum " << new_name << endl;

                    ifstream we ( old_name.toStdString(), ios::binary );
                    if ( !we )
                    {
                         showWarningMessage(
                                                "Error while opening output file",
                                                QString ( "The spectrum file %1 can not be opened" ).arg ( old_name ),
                                                QMessageBox::Critical);
                        return ;
                    }

                    ofstream wy ( new_name.toStdString() );
                    if ( !wy )
                    {
                         showWarningMessage(
                                                "Error while opening output file",
                                                QString ( "The spectrum file %1 can not be created" ).arg ( new_name ),
                                                QMessageBox::Critical);
                        return ;
                    }

                    double bin, left, right, wid ;
                    we.read ( ( char* ) &bin, sizeof ( left ) );
                    we.read ( ( char* ) &left, sizeof ( left ) );
                    we.read ( ( char* ) &right, sizeof ( wid ) );
                    wid = ( right - left ) /bin ;
                    int value ;

                    if ( flag_radware )
                    {

                        string nazwa_no_path = new_name.toStdString();
                        string::size_type pos = nazwa_no_path.rfind ( '/' );
                        if ( pos != string::npos ) //
                        {
                            nazwa_no_path = nazwa_no_path.substr ( pos+1 );
                        }


                        wy << "SPECTRUM_NAME\t" << nazwa_no_path
                           << "\nDIMENSION\t\t\t1\nCOUNTTYPE\t\t\tfloat"
                              "\nBASE\t\t\t\t0"
                              "\nRANGE\t\t\t\t4096"
                              "\nDATA\n"  ;
                    }

                    for ( int n = 0 ; ( n < bin ) && we; n++ )
                    {
                        we.read ( ( char* ) &value, sizeof ( value ) );
                        if ( we.eof() )
                            break ;
                        wy << left << "\t" << value << "\n" ;
                        left += wid ;
                    }
                } //  end if  *.spe
            }
            progress.setValue ( numFiles );
            break ;
        } // switch
    }
}
//**********************************************************************
void T4select_spectra::create_the_overplot_spectrum()
{
    if(ui->ListBox_chosen->count() == 0 )
    {
         showWarningMessage(
                                   "Empty list of spectra?",
                                   "The overlay spectrum - is build on a set of currently chosen spectra.\n"
                                   "Now this list is empty - so no sense im making an overlay");
        return ;
    }



    bool ok = false;
    QString krotka_nazwa_overplotu = QInputDialog::getText (this,
                                                            tr ( "Cracow viewer - defining a new OVERLAY specrtrum" ),
                                                            tr ( "Currently selected spectra can contribute to the new OVERLAY spectrum.\n"
                                                                 //"NOTE: you can sum only such spectra which have the same binning and ranges\n\n"
                                                                 "The overlay spectrum name should start with prefix '@over_'.\n"
                                                                 "It will automatically have an extension '.spc'\n\n",
                                                                 "Please enter name for the overlay spectrum" ),
                                                            QLineEdit::Normal,
                                                            "@over_", // QString::null,
                                                            &ok);

    if ( ok && !krotka_nazwa_overplotu.isEmpty() )
    {
        // if somebody forgot to give prefix
        string::size_type poz =krotka_nazwa_overplotu.indexOf ( "@over_", 0 ) ;
        if ( poz == string::npos )
            krotka_nazwa_overplotu.insert ( 0, "@over_" ) ;

        QString fname = gpath.Qspectra() + krotka_nazwa_overplotu ;
        poz = fname.indexOf( ".spc",0 );
        if ( poz == string::npos )
        {
            krotka_nazwa_overplotu += ".spc" ;
            fname += ".spc" ;
        }
        ofstream plik ( fname.toStdString() ) ;

        // user entered something and pressed OK
        //cout << tekscik << " will contain spectra " << endl ;

        // we will check the binnings comparing with the first spectrum on the list

        //      double bin_first = 0,
        //          beg_first = 0,
        //          end_first = 0 ;
        string spec_first ;

        //        ui->ListBox_chosen->firstItem() ;

        for ( int i = 0 ; i < ui->ListBox_chosen-> count() ; i++ )
        {
            //cout <<  " tresc " <<  (ui->ListBox_chosen->item(i)->text() )  << endl ;
            string spec_name = ui->ListBox_chosen->item(i)->text().toStdString() ;
            // is it  1D spectrum ??? --------------------------
            poz = spec_name.rfind ( ".spc" );
            //cout << "poz = " << (int) poz << endl ;
            if ( poz != ( spec_name.length() - 4 ) )
            {
                 showWarningMessage(
                                        "Overlay spectra creator warning",
                                        QString ( "The spectrum %1 is not a 1D spectrum with the extension '*.spc'\n"
                                                  "Please remove it from the list and start again" ).arg ( spec_name.c_str() ),
                                        QMessageBox::Critical);
                // remove the file
                remove ( fname.toStdString().c_str() );
                // refreshing the list with all available spectra
                selected_filter();
                return ;
            }
            // is it another sum spectrum ??? --------------------------
            poz = spec_name.find ( "@", 0 );
            // cout << "poz = " << (int) poz << endl ;
            if ( poz != string::npos )
            {
                 showWarningMessage(
                                        "overlay spectra creator warning",
                                        QString ( "The spectrum %1 is a quasi-spectrum. You can not use @ spectra to overlay\n"
                                                  "Please remove it from the list and start again" ).arg ( spec_name.c_str() ),
                                        QMessageBox::Critical);
                // remove the file
                remove
                        ( fname.toStdString().c_str() );
                // refreshing the list with all available spectra
                selected_filter();
                return ;
            }
            // checking if it is legal - the same binning, the same ranges
#ifdef NIGDY
            if ( i == 0 )
            {
                check_common_binnings ( spec_name, &bin_first, &beg_first, &end_first );
                spec_first = spec_name ;
            }
            else
            {
                double bin, beg, end ;
                if ( check_common_binnings ( spec_name, &bin, &beg, &end ) == true )
                {
                    if ( bin != bin_first ||  beg != beg_first ||  end != end_first )
                    {
                        QMessageBox::critical ( this,
                                                "Sum spectra creator warning",
                                                QString ( "The spectrum %1 has not the same binning as the first one on the list\n\n"
                                                          "%2\nhas bins = %3, begin = %4, end = %5\n\n"
                                                          "%6\nhas bins = %7, begin = %8, end = %9\n\n"
                                                          "Please remove the wrong spectrum from the list and start again"
                                                          ).arg ( spec_name )
                                                .arg ( spec_name ).arg ( bin ).arg ( beg ).arg ( end )
                                                .arg ( spec_first ).arg ( bin_first ).arg ( beg_first ).arg ( end_first )
                                                ,
                                                QMessageBox::Ok,
                                                QMessageBox::NoButton,
                                                QMessageBox::NoButton );
                        // remove the file
                        remove
                                ( fname.toStdString().c_str() );
                        // refreshing the list with all available spectra
                        selected_filter();
                        return ;
                    }
                }
            }
#endif
            plik << ( ui->ListBox_chosen->item(i)->text().toStdString() ) << endl ;
        }

        ui->ListBox_available->addItem(  krotka_nazwa_overplotu);
        ui->ListBox_available->sortItems();
        // inform about succses
        showWarningMessage(
                                   "Overlay spectrum creator",
                                   QString ( "SUCCES\n\nThe overlay spectrum %1 was succesfully created.\n\n"
                                             "Note: \n"
                                             "Actually the overlay spectrum is just a list of names of spectra\n"
                                             "which will be overlayed every 1 minue and the result will be displayed on the screen. \n"
                                             ).arg ( fname ), QMessageBox::Information );

        int how_many = ui->ListBox_chosen-> count() ;

        if ( how_many > 40 )
             showWarningMessage(
                                   "This overlay spectrum can slow down the communication with you",
                                   QString ( "The overlay spectrum %1 \ndemands painting on the screen  %2 spectra\n\n"
                                             "Warning: \n"
                                             "overlayming hundreds of spectra (every minute) can dramatically slow down\n"
                                             "the communication between you and the cracow gui \n"
                                             ).arg ( fname ).arg ( how_many ));
        // refreshing the list with all available spectra
        // selected_filter();  <-- not needed anymore
    }
    else
    {
        ;// user entered nothing or pressed Cancel
    }
}
//******************************************************************************************************


//*****************************************************************************************************
void T4select_spectra::zero_all_available_spectra()
{
    QString list_of_spectra= "You are going to erase the contents of the ALL the   online SPY spectra'\n\n"
                             "ARE YOU SURE  ?";
    switch ( askYesNoCancel(" 'Zero-ing' online spectra, ARE YOU SURE ? ",
                                    list_of_spectra
                                   ) )
    {
    default: break;

    case QMessageBox::Yes:   // Yes :
        switch ( askYesNoCancel(  " 'Zeroing ALL analysis specta  ",
                                        "Not only those filtered spectra, but ALL spectra will be zero-ed, ARE YOU SURE ?") )
        {
        case QMessageBox::Yes:     // Yes :
        {
            // cout << "Spectrum to zero = " <<   dokument->caption() << endl;
            string command_file = gpath.commands + "spy_spectra_for_zeroing.command";

            ofstream plik ( command_file.c_str() );
            if ( !plik )
                cout << "problem in openning the file" << command_file << endl;

            // taking the spectra names from the directory - only
            // opening all spectra files from current directory
            QDir katalog ( gpath.Qspectra() ); // ("spectra") ;
            QStringList filters;
            filters <<  "*.spc" << "*.mat" ;
            katalog.setNameFilters(filters);

            QStringList lista = katalog.entryList () ;


            QProgressDialog progress ( "Zeroing All spectra on the disk...",
                                       "Abort zeroing", 0, lista.size(),
                                       this);
            progress.setMinimumDuration ( 5000 ) ; // miliseconds
            progress.setValue ( 0 );
            int counter = 0;

            if ( ! appl_form_ptr->is_spy_running() )
            {
                // if we do zeroing on disk ourselves, we will need to set the time of zeroing
                appl_form_ptr->restore_times_of_zeroing_from_disk();
            }

            for ( QStringList::Iterator it = lista.begin() ; it != lista.end() ;  ++it )
            {
                //cout << "To zero " << *it << "\n";

                qApp->processEvents();
                if ( progress.wasCanceled() )
                    break;
                progress.setValue ( ++counter );
                progress.setLabelText ( QString ( "Zero-ing:  " ) + ( *it ) );


                //plik << *it << endl ;
                if ( appl_form_ptr->is_spy_running() )
                {
                    plik << (*it).toStdString()  << endl ;   // we can ask spy to do it
                }
                else
                {
                    appl_form_ptr->zeroing_really_on_disk ( (*it).toStdString() ); // if spy is not running - we do it ourselves
                }

            }
            plik << "this_is_the_end_of_the_list_of_spectra_which_cracow_wants_to_zero" << endl;
            if ( !plik )
                cout << "problem in writing to file" << command_file << endl;
            plik.close();

            if ( ! appl_form_ptr->is_spy_running() )
            {
                // if we do zeroing on disk ourselves, we need to set the time of zeroing
                appl_form_ptr->save_times_of_zeroing_on_disk();
            }
            break;
        }

        default:
            cout << "No, or Cancel" << endl;
            break;
        }
    }
}
//**********************************************************************************************
void T4select_spectra::filter_dialog()
{
    QString filtr = ui->ComboBox_filter->currentText ();
    bool ok;
    QString text = QInputDialog::getText (this,
                                          "Fliter, without updating (use  it  when  you have a  slow  disk)", "Enter the text filter string:", QLineEdit::Normal,
                                          ui->ComboBox_filter->currentText (),
                                          &ok);
    if ( ok && !text.isEmpty() )
    {
        // user entered something and pressed OK
        ui->ComboBox_filter->addItem(text );
    }
    else
    {
        // user entered nothing or pressed Cancel
    }
}

//**********************************************************************************************************************
void T4select_spectra::add_spectra_names_currently_displayed()
{

    QList <QMdiSubWindow*> windows = arch_ws->subWindowList();

    Tplate_spectrum * dokument   = nullptr ;

    for(int nr_dok = 0 ; nr_dok < windows.count() ; nr_dok++)
    {
        dokument  = dynamic_cast< Tplate_spectrum * >( windows[nr_dok]->widget() ) ;
        if(!dokument) break;

        string name =  dokument->give_spectrum_name();
        ui->ListBox_chosen->addItem ( name.c_str()   );
    }
}
//*********************************************************************************************************************
void T4select_spectra::on_push_zero_all_all_clicked()
{
    zero_all_available_spectra();
}
//*********************************************************************************************************************
void T4select_spectra::on_checkBox2_toggled(bool checked)
{
    extended_selection_mode(checked);
}
//*********************************************************************************************************************
void T4select_spectra::on_PushButton_choose_clicked()
{
    select_one_pressed();
}
//*********************************************************************************************************************
//void T4select_spectra::on_ListBox_available_doubleClicked(QListWidget *  /*arg1*/ )
//{
//    select_one_pressed();
//}
//*********************************************************************************************************************
void T4select_spectra::on_PushButton_deselect_clicked()
{
    remove_highlighted();
}
//*********************************************************************************************************************
void T4select_spectra::on_PushButton_deselect_all_clicked()
{
    remove_all();
}
//*********************************************************************************************************************
void T4select_spectra::on_Push_B_OK_clicked()
{
    close();
}
//*********************************************************************************************************************
void T4select_spectra::on_Combo_groups_activated(int /*index*/)
{
    combo_groups_activated();
}
//*********************************************************************************************************************
void T4select_spectra::on_Button_show_exit_pressed()
{
    show_choosen_and_exit();
}
////*********************************************************************************************************************
// No signal - of this kind (anymore?)
//void T4select_spectra::on_ComboBox_filter_textChanged(const QString & /*arg1*/)
//{
//    selected_filter();
//}
//*********************************************************************************************************************
void T4select_spectra::on_PushButton_Delete_group_clicked()
{
    delete_group();
}
//*********************************************************************************************************************
void T4select_spectra::on_Button_group_pressed()
{
    define_group();
}
//*********************************************************************************************************************
void T4select_spectra::on_pushButton_zeroing_pressed()
{
    zeroing_choosen_spectra();
}
//*********************************************************************************************************************
void T4select_spectra::on_PushButton_choose_ALL_clicked()
{
    select_all();
}
//*********************************************************************************************************************
void T4select_spectra::on_pushButton_sum_clicked()
{
    create_the_sum_spectrum();
}
//*********************************************************************************************************************
void T4select_spectra::on_push_save_as_ascii_clicked()
{
    select_chosen_as_ascii();
}
//*********************************************************************************************************************
void T4select_spectra::on_pushButton_overlay_clicked()
{
    create_the_overplot_spectrum();
}
//*********************************************************************************************************************
//void T4select_spectra::on_ListBox_chosen_rightButtonClicked(QListWidget *arg1, const QPoint &arg2)
//{
//    right_mouse_on_chosen(arg1, arg2);
//}
//*********************************************************************************************************************
void T4select_spectra::on_pushButton_filter_dialog_clicked()
{
    filter_dialog();
}
//*********************************************************************************************************************
void T4select_spectra::on_Check_add_overwrite_clicked()
{
    add_spectra_names_currently_displayed();
}
//*********************************************************************************************************************
void T4select_spectra::on_ListBox_available_itemDoubleClicked(QListWidgetItem *  /*item*/ )
{
    select_one_pressed();
}
//*********************************************************************************************************************
void T4select_spectra::on_toolButton_shift_up_clicked()
{
    move_selected_chosen_up_down(true); // true means up
}
//*********************************************************************************************************************
void T4select_spectra::on_toolButton_shift_down_clicked()
{
    move_selected_chosen_up_down(false); // true means down
}
//*********************************************************************************************************************
void T4select_spectra::move_selected_chosen_up_down(bool up)
{
    // check if anything is selected
    QList<QListWidgetItem *> list_to_move  = ui->ListBox_chosen->selectedItems ();
    if(list_to_move.count() == 0)
    {
        return;
    }

    // finding the rows of items to be removed
    vector<int> nr_of_rowsToRemove ;

    for (int i = 0 ; i < ui->ListBox_chosen->count() ; i++)
    {
        if( ui->ListBox_chosen->item(i)->isSelected())
        {
            //            cout << "to be removed row =" << i << endl;
            nr_of_rowsToRemove.push_back(i);
            //            last = i;
        }
    }

    if(nr_of_rowsToRemove.empty()) return;

    // removing (taking) starting from the highest positions
    for(int i = nr_of_rowsToRemove.size()-1 ; i >= 0 ; i--  )
    {
        int pos = nr_of_rowsToRemove[i];
        if(pos <= 0) { pos = 0; }
        //if(pos > (ui->ListBox_chosen->count() - 1) ) continue;

        //        cout << i <<") removing the row " << pos << " with: "
        //             << ui->ListBox_chosen->item(pos)->text().toStdString()
        //             << endl;
        ui->ListBox_chosen->item(pos)->setSelected(false);
        ui->ListBox_chosen->takeItem(pos); // removing
    }

    // now, include and move it up by one position
    for(int i = 0 ; i < list_to_move.count() ; i++)
        //        for(int i = list_to_move.count()-1 ; i >=0 ; i--)
    {
        // calculating new row where it should be placed
        int pos = nr_of_rowsToRemove[0]+ i + ( (up)? -1 : +1);   // here is a difference between up and down

        if(pos < 0) { pos = 0 ; } //continue; // putting on raw 0 - possible
        if(pos >= (ui->ListBox_chosen->count()) )
            pos =  (ui->ListBox_chosen->count() );
        //        if(pos >= )
        ui->ListBox_chosen->insertItem(pos, list_to_move[i])  ;
        //        cout << "adding " << ui->ListBox_chosen->item(i)->text().toStdString() << endl;
        ui->ListBox_chosen->item(pos)->setSelected(true);
    }
}
//*********************************************************************************************************************

void T4select_spectra::on_ListBox_chosen_itemSelectionChanged()
{
    // if something is selected - enable up and down toolbuttons
    QList<QListWidgetItem *> list_to_move  = ui->ListBox_chosen->selectedItems ();

    bool state = !list_to_move.empty();

    ui->toolButton_shift_down->setEnabled(state);
    ui->toolButton_shift_up->setEnabled(state);

}
//*********************************************************************************************************************
void T4select_spectra::on_push_add_from_screen_clicked()
{
    add_spectra_names_currently_displayed();
}
//*********************************************************************************************************************
void T4select_spectra::on_lineEdit_anti_filter_textChanged(const QString &arg1)
{
    antifilter = arg1;
    selected_filter();
}
//*********************************************************************************************************************
void T4select_spectra::on_checkBox_anti_toggled(bool checked)
{
    ui->lineEdit_anti_filter->setEnabled(checked);
    ui->lineEdit_anti_filter->setHidden(!checked);

    selected_filter();
}
//*********************************************************************************************************************
//void T4select_spectra::xxxon_ComboBox_filter_currentTextChanged(const QString & arg1)
//{
//    cout << __func__ <<  " was empty before  T4select_spectra::on_ComboBox_filter_currentTextChanged"
//         << arg1.toStdString()  << endl;

//    //on_ComboBox_filter_textChanged(QString)
//    selected_filter();
//}
//*********************************************************************************************************************
//void T4select_spectra::xxxon_ComboBox_filter_editTextChanged(const QString &)
//{
////    cout << __func__ << " was empty before  arg1 =" << arg1.toStdString() << endl;
//}
//*********************************************************************************************************************
void T4select_spectra::on_ComboBox_filter_editTextChanged(const QString /*arg1*/)
{
    //WHERE;
    // cout << __func__ << "   arg1 =" << arg1.toStdString() << endl;
    //cout << "   then  is calling function:    selected_filnter()  " << endl;
    selected_filter();
}
//*********************************************************************************************************************
void T4select_spectra::on_ComboBox_filter_currentTextChanged([[maybe_unused]]  QString arg1)
{
    selected_filter();
}
//*********************************************************************************************************************
// void T4select_spectra::on_ComboBox_filter_activated(const QString & /*arg1*/)
// {
//     // This function is essential when we type filter "F" and "f" - they are distinguished properly.

//     //cout << "Combo activqted with " << arg1.toStdString() << endl;
// }
//*********************************************************************************************************************
void T4select_spectra::on_checkBox_case_sens_clicked(bool checked)
{
    // cout << "Case sensitive is now " << checked << endl;
    flag_case_sensitive = checked;
    ui->ComboBox_filter->completer()-> setCaseSensitivity(
                flag_case_sensitive? Qt::CaseSensitive : Qt::CaseInsensitive);
    selected_filter();
}
//*********************************************************************************************************************

void T4select_spectra::on_Button_show_exit_clicked()
{
    show_choosen_and_exit();
}
//*********************************************************************************************************************
//void T4select_spectra::on_checkBox_toggled(bool checked)
//{

//}
//*********************************************************************************************************************
void T4select_spectra::on_checkBox_enable_drag_toggled(bool checked)
{
    static QString original_style;
    if(checked){

        original_style = ui->ListBox_chosen-> 	styleSheet( );
        ui->ListBox_chosen->setStyleSheet( "background-color: rgb(0, 0, 89);\ncolor: rgb(255, 255, 255);");

        ui->ListBox_chosen->clearSelection();
        ui->ListBox_chosen->setSelectionMode(QAbstractItemView::SingleSelection);
        // ui->ListBox_chosen->setSelectionMode(QAbstractItemView::MultiSelection);

        ui->ListBox_chosen->setDragEnabled(true);
        ui->ListBox_chosen->viewport()->setAcceptDrops(true);
        ui->ListBox_chosen->setDropIndicatorShown(true);
        ui->ListBox_chosen->setDragDropMode(QAbstractItemView::InternalMove);
    }else {
        ui->ListBox_chosen->setStyleSheet( original_style);
        ui->ListBox_chosen->setSelectionMode(QAbstractItemView::MultiSelection);
        ui->ListBox_chosen->setDragEnabled(false);
        ui->ListBox_chosen->viewport()->setAcceptDrops(false);
        ui->ListBox_chosen->setDropIndicatorShown(false);

    }
}



