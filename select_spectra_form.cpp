/**************************************************************** ************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/
//#include "select_spectra_form.h"
#include "ui_select_spectra_form.h"


#include <qprogressdialog.h>

#include "paths.h"
#include "options.h"

extern paths path ;
#include "appl_form.h"
extern appl_form  *appl_form_ptr;
extern options current_options ;

namespace Ui {


//***************************************************************************
void Ui_Select_spectra_form::init()
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
    ComboBox_filter->setInsertionPolicy ( QComboBox::BeforeCurrent );

    // setting the  enabled options
    extended_selection_mode ( false ) ;

    ListBox_available-> clear () ;
    ListBox_chosen-> clear () ;
    Check_add_overwrite->hide();
}
//****************************************************************************
void Select_spectra_form::set_parameters ( QMdiArea  * ws )
{
    arch_ws = ws ;   // can be used by other member    functions

    // As sometimes this take very long - we show progress dialog
    int how_many_items = 100;
    QString infor ( "Some GSI directories are very slow!\n"
                    "If this is your case - consider moving your analysis "
                    "into the /scratch.local direcory\n"
                    "Reading the directory: " );
    infor += path.Qspectra() + "..." ;
    QProgressDialog progress ( infor,
                               "Abort reading", how_many_items,
                               this, "progress", TRUE );
    progress.setMinimumDuration ( 2000 ) ; // miliseconds
    progress.setProgress ( 10 );
    qApp->processEvents();

    //--------------------------------------------------------
    // taking the spectra names from the directory - only
    // opening all spectra files from current directory
    QDir katalog ( path.Qspectra() ); // ("spectra") ;

    progress.setProgress ( 11 );
    qApp->processEvents();

    how_many_items = katalog.count() ;
    progress.setTotalSteps ( how_many_items );

    QStringList lista = katalog.entryList ( "*.spc *.mat" ) ;

    int nr = 0 ;
    for ( QStringList::Iterator it = lista.begin() ; it != lista.end() ;  ++it, nr++ )
    {
        if ( ! ( nr % 100 ) )
        {
            //     cout << "nr = " << nr << endl;
            progress.setProgress ( nr );
            qApp->processEvents();
        }
        if ( progress.wasCancelled() )
            break;
        //... list one file

        //cout << *it << "\n";
        ListBox_available->insertItem ( *it );
        all_spectra_names += *it ;
    }
    progress.setProgress ( how_many_items );
    //------------------------------------------------------------
    if ( lista.begin() == lista.end() )
    {
        ListBox_available->insertItem ( "No spectra in the directory:" );
        ListBox_available->insertItem ( path.Qspectra() );
        ListBox_available->insertItem ( "Most probably the path to the directory of SPY was wrongly set" );
        ListBox_available->insertItem ( "Go to menu bar:  Preferences->Path to spy" );
    }
    refresh_defined_groups();

}
//********************************************************************************************
void Select_spectra_form::select_one_pressed()
{
    ListBox_available->firstItem() ;
    for ( unsigned int i = 0 ; i < ListBox_available-> count() ; i++ )
    {
        if ( ListBox_available-> isSelected ( i ) )
        {
            // cout << "selected is ..." << ListBox_available->text(i) << endl ;
            // ListBox_chosen->insertItem (ListBox_available-> currentText () );
            ListBox_chosen->insertItem ( ListBox_available->text ( i ) );

            ListBox_available-> setSelected ( i, false );
        }
    }

    if ( ListBox_chosen->count() > 80 )
    {
        QMessageBox::warning
        ( 0, "A lot of chosen spectra" ,
          QString ( "Warning: Your list of chosen spectra has %1 item now" ).arg ( ListBox_chosen->count() ),
          QMessageBox::Yes | QMessageBox::Default,
          QMessageBox::NoButton );
    }
    add_to_history ( ComboBox_filter->currentText() ) ;

}
//*********************************************************************************************
void Select_spectra_form::show_chosen_spectra()
{

    //  cout << "F. show_chosen_spectra()" << endl ;

    // here we could close existing windows - or not

    QWidgetList windows = arch_ws->windowList();
    spectrum_widget * dokument  = ( spectrum_widget * ) windows.first() ;
    if ( !Check_add_overwrite->isChecked() )
    {
        while ( dokument )
        {
            //windows.->
            dokument->close() ;
            dokument->destroy() ;  // my destructor
            //delete dokument ;     <--- with this tile() will crash
            dokument  = ( spectrum_widget * ) windows.next() ;
            if ( !dokument )
                break ;
        }
    }
    qApp->processEvents ();


    if ( ListBox_chosen->count() > 100 )
    {
        QString list_of_spectra= QString ( "You asked to show %1"
                                           " online SPY spectra \n"
                                           "More than 70 spectra on the screen - it is not recommended (hence generally possible)\n\n"
                                         ).arg ( ListBox_chosen-> count() );

        QMessageBox::warning ( this, " This would make the program very slow !!! ",
                               list_of_spectra,
                               QMessageBox::Ok | QMessageBox::Default,
                               QMessageBox::NoButton,
                               QMessageBox::NoButton );
        // return ;

    }

    time_t moment = time ( 0 ) ;

    current_options.set_how_many_spectra_on_the_screen_now ( ListBox_chosen->count() );

    // adding the new spectra windows
    for ( int i = 0 ;  i < ( int ) ListBox_chosen->count() ;  i++ )
    {

        char spectrum_filename[250] ;
        strcpy ( spectrum_filename, ListBox_chosen->text ( i ) );
        QString fn =  ListBox_chosen->text ( i ) ;

        // spectrum_widget * s = new spectrum_widget(arch_ws, spectrum_filename );

        spectrum_widget * s = 0 ;

        // looking for an extension
        if ( fn.contains ( ".spc", FALSE ) ) // <-- FALSE=case insensitife
        {
            //cout << "you selected  a spectrum .spc" << endl;
            s = new spectrum_1D ( arch_ws, fn );
        }
        if ( fn.contains ( ".mat", FALSE ) )
        {
            // cout << " you seleted  a matrix (.mat)" << endl;
            s = new spectrum_2D ( arch_ws, fn );
        }

        if ( s )
        {
            // the name is taken form t
            s->set_the_name ( fn );
            s->read_in_file ( ListBox_chosen->text ( i ) , true ); // true <- this is first time, full, new markers etc.

            s->showNormal();
            //cout << "fileOpen, if(s) true, " << endl ;
        }
        //
        if ( time ( 0 ) - moment > 2 ) // for longer times we put something on the screen
        {
            qApp->processEvents ();
            moment = time ( 0 ) ; // now more often
        }

    }
    //qApp->processEvents ();
    arch_ws->tile() ;


}
//********************************************************************************
void Select_spectra_form::remove_highlighted()
{
    //cout << "remove_highlighted() z " << ListBox_chosen-> count() << endl ;


    ListBox_chosen->firstItem() ;
    for ( unsigned int i = 0 ; i < ListBox_chosen-> count() ; i++ )
    {
        //cout << "is selected ..." << i  ;
        if ( ListBox_chosen-> isSelected ( i ) )
        {
            //cout << "   Yes selected is ..." << ListBox_available->text(i) << endl ;

            ListBox_chosen->removeItem ( i );
            i-- ;
        }
        //else cout << "   NO ..." << i << ListBox_available->text(i) << endl ;
    }
    ListBox_chosen->clearSelection ();
}
//********************************************************************************
void Select_spectra_form::remove_all()
{
    ListBox_chosen-> clear () ;
}
//*******************************************************************************
void Select_spectra_form::define_group()
{

    //  bool ok = FALSE;



#ifdef OLD_GROUPS

    QString tekscik = QInputDialog::getText (
                          tr ( "Cracow viewer - defining a new group of spectra" ),
                          tr ( "Please enter name of the new group" ),
                          QLineEdit::Normal, QString::null, &ok, this );



    if ( ok && !tekscik.isEmpty() )
    {
        QString fname = path.Qgroups() + tekscik + ".group" ;
        ofstream plik ( fname ) ;

        // user entered something and pressed OK
        //cout << tekscik << " will contain spectra " << endl ;

        ListBox_chosen->firstItem() ;

        for ( unsigned int i = 0 ; i < ListBox_chosen-> count() ; i++ )
        {
            //cout <<  " tresc " <<  (ListBox_chosen->text(i) )  << endl ;
            plik << ( ListBox_chosen->text ( i ) ) << endl ;
        }
        refresh_defined_groups();

    }
    else
    {
        ;// user entered nothing or pressed Cancel
    }
#else
    // dialog to choose the name and the format

    QFileDialog* fd = new QFileDialog ( this, "Select the name for your group", TRUE );
    fd->setMode ( QFileDialog::AnyFile );
    fd->setFilter ( "group of spectra names  (*.group)" );
    fd->setDir ( path.Qgroups() );

    QString fileName;
    if ( fd->exec() == QDialog::Accepted )
    {
        QString fileName = fd->selectedFile();

        if ( !fileName.isEmpty() )
        {
            // Checking if it has  extension group
            string name = fileName.ascii();
            string extension = ".group";

            QString fname;
            ;

            if ( name.substr ( name.size() - extension.size(), extension.size() ) != extension )
            {
                fname = fileName  + ".group" ;
            }
            else
            {
                fname = fileName ;
            }


            ofstream plik ( fname ) ;

            // user entered something and pressed OK
            //cout << tekscik << " will contain spectra " << endl ;

            ListBox_chosen->firstItem() ;

            for ( unsigned int i = 0 ; i < ListBox_chosen-> count() ; i++ )
            {
                //cout <<  " tresc " <<  (ListBox_chosen->text(i) )  << endl ;
                plik << ( ListBox_chosen->text ( i ) ) << endl ;
            }
            refresh_defined_groups();

        }
        else
        {
            ;// user entered nothing or pressed Cancel
        }


        //cout << "Nazwa " << fileName << endl ;
    }


#endif

    //Button_group->show();
    Button_group->setDown ( false );
    Button_group->show();
}
//********************************************************************************
void Select_spectra_form::combo_groups_activated( )
{

    QString string =  path.Qgroups() + Combo_groups->currentText() ;
    string += ".group"  ;
    ifstream plik ( string );
    char linia[500] = ""  ;
    while ( plik )
    {
        plik.getline ( linia, sizeof ( linia ) ) ;
        //QString nazwid = linia ;

        if ( plik )
            ListBox_chosen->insertItem ( linia );
    }
}
//*******************************************************************************
void Select_spectra_form::show_choosen_and_exit()
{
    //cout << " Select_spectra_form::show_choosen_and_exit() " << endl;

    //show_chosen_spectra() ;
    done ( QDialog::Accepted ) ;
}
//*******************************************************************************
void Select_spectra_form::selected_filter()
{

    // we do not react for typing filter more often than every 2 secondes
    // (because in case of 2000 spectra it is slowing down our typing)

    //  static time_t last = time(0);
    //   // just wait 1 seconds
    //   while((time(0) - last) < 1) ;
    //   last = time(0) ;


    QString filtr = ComboBox_filter->currentText ();

    ListBox_available->clear();

    QStringList found = all_spectra_names.grep ( QRegExp ( filtr, TRUE,true ) );

    ListBox_available->insertStringList ( found ) ;

#ifdef NIGDY
    // taking the spectra names from the directory - only
    // opening all spectra files from current directory
    QDir katalog ( path.spectra ); // ("spectra") ;
    QString name_filter = "*"
                          // + filtr + "*.asc  *"
                          + filtr + "*.spc  *"
                          + filtr + "*.mat" ; // clever filetering with '*' and '?'

    QStringList lista = katalog.entryList ( name_filter ) ;

    for ( QStringList::Iterator it = lista.begin() ; it != lista.end() ;  ++it )
    {
        //cout << *it << "\n";
        ListBox_available->insertItem ( *it );
    }

#endif // NIGDY
}
//***********************************************************************************
void Select_spectra_form::delete_group()
{
    bool ok = FALSE;

    //    QString tekscik = QInputDialog::getText(
    //      tr( "Telescope - removing the definiton of a group of spectra" ),
    //      tr( "You are going to remove the definition of the group. \n(This does not delete any spectra"
    //    " but just the definition of a group)."
    //    "Please enter name of the group to be removed" ),
    //      QLineEdit::Normal, QString::null, &ok, this );
    //

    QDir katalog ( path.Qgroups() ) ; //
    QStringList lista_grup = katalog.entryList ( "*.group" ) ;

    //  QStringList lst;
    //  lst << "First" << "Second" << "Third" << "Fourth" << "Fifth";
    //  bool ok = FALSE;

    QString res = QInputDialog::getItem (
                      tr ( "Application name" ),
                      tr ( "Please select an item" ), lista_grup, 0, FALSE, &ok, this );
    if ( ok )
    {
        // user selected an item and pressed OK
        switch ( QMessageBox::information ( this,
                                            "Cracow  group definition remover",
                                            QString ( "You are going to delete the definition for the group \n\t %1\n"
                                                    "Are you sure \n\n" ).arg ( res ),
                                            "Yes","No", "Cancel", 1 ) )
        {
        case 0:  // Yes :
            katalog.remove ( res, FALSE );
            refresh_defined_groups();
            break;
        }
    } // end if ok


    PushButton_Delete_group->setDown ( false );
    PushButton_Delete_group->show();

}
//**********************************************************************
void Select_spectra_form::refresh_defined_groups()
{

    //---------------------------------------
    //  now, we can check defined groups

    Combo_groups->clear() ;

    QDir katalog ( path.Qgroups() ); //("spectra") ;
    QStringList lista_grup = katalog.entryList ( "*.group" ) ;

    for ( QStringList::Iterator it = lista_grup.begin() ; it != lista_grup.end() ;  ++it )
    {
        //cout << *it << "\n";
        // removing the extension ".group"
        QString string = *it ;
        int i = string.findRev ( ".group" );
        string.truncate ( i );
        Combo_groups->insertItem ( string );
    }
}
//***************************************************************************
QString Select_spectra_form::give_last_filter()
{
    //return ComboBox_filter->currentText ();

    QString all ;
    //for(int i = 0 ; i < ComboBox_filter->count(); i++ )


#ifdef Never

    for ( int i = ComboBox_filter->count() - 1 ; i >= 0 ; i-- )
    {
        all += ComboBox_filter->text ( i ) ;
        all += "\n" ;
    }
#else
    for ( int i = 0 ; i < ComboBox_filter->count() ; i++ )
    {
        all += ComboBox_filter->text ( i ) ;
        all += "\n" ;
    }

#endif

    return all ;
}
//************************************************************************
void Select_spectra_form::set_filter ( QString fff )
{
    //  ComboBox_filter->setCurrentText (fff);
    if ( fff.isEmpty() )
        return ;


    // take first part, till the newline
    string all = fff.ascii() ;

    string::size_type a = 0 ;
    string::size_type b = all.find ( "\n" ) ;
    while ( b != string::npos )
    {
        //cout << "insetring to combo ->" << all.substr(a, b-a) << "<-" << endl;
        ComboBox_filter->insertItem ( all.substr ( a, b-a ).c_str() ) ;
        a = b+1;
        b = all.find ( "\n", a ) ;
    }
}
//***************************************************************************
vector<string> Select_spectra_form::give_listbox_chosen()
{
    vector<string> parcel ;
    ListBox_available->firstItem() ;
    for ( unsigned int i = 0 ; i < ListBox_chosen-> count() ; i++ )
    {
        parcel.push_back ( ListBox_chosen->text ( i ).ascii() ) ;
    }

    return parcel;
}
//****************************************************************************
void Select_spectra_form::set_listbox_chosen ( vector<string> parcel )
{
    // inserting the names of files which are recently placed on the green window
    for ( unsigned int i = 0 ; i < parcel.size() ; i++ )
    {

        // cout << "Initially inserting " << parcel[i].c_str() << endl;
        ListBox_chosen->insertItem ( parcel[i].c_str() );
    }
}

//*****************************************************************************
void Select_spectra_form::zeroing_choosen_spectra()
{
    //vector<string> parcel ;

    QString list_of_spectra= QString ( "You are going to erase the contents of the %1"
                                       " online SPY spectra \n"
                                       "which are currently on the list 'spectra choosen to display'\n\n"
                                       "ARE YOU SURE  ?" ).arg ( ListBox_chosen-> count() );

    switch ( QMessageBox::warning ( this, " 'Zero-ing' online spectra, ARE YOU SURE ? ",
                                    list_of_spectra,
                                    "Yes","No","Cancel", 1 ) )
    {
    case 0: //Yes :

        //cout << "Spectrum to zero = " <<   dokument->caption() << endl;
        string command_file = path.commands + "spy_spectra_for_zeroing.command";
        ofstream plik ( command_file.c_str() );
        if ( !plik )
            cout << "problem in openning the file" << command_file << endl;

        ListBox_available->firstItem() ;

        QProgressDialog progress ( "Zeroing selected spectra...",
                                   "Abort zeroing",  ListBox_chosen-> count(),
                                   this, "progress", TRUE );
        progress.setMinimumDuration ( 5000 ) ; // miliseconds
        progress.setProgress ( 0 );
        int counter = 0;


        if ( ! appl_form_ptr->is_spy_running() )
        {
            // if we do zeroing on disk ourselves, we will need to set the time of zeroing
            appl_form_ptr->restore_times_of_zeroing_from_disk();
        }

        for ( unsigned int i = 0 ; i < ListBox_chosen-> count() ; i++ )
        {
            //plik << ( ListBox_chosen->text ( i ) )  << endl;
            progress.setProgress ( i );

            qApp->processEvents();
            if ( progress.wasCancelled() )
                break;
            progress.setLabelText ( QString ( "Zero-ing:  " ) + ( ListBox_chosen->text ( i ) ) );

            if ( appl_form_ptr->is_spy_running() )
            {
                plik << ( ListBox_chosen->text ( i ) )  << endl;  // we can ask spy to do it
            }
            else
            {
                appl_form_ptr->zeroing_really_on_disk ( ListBox_chosen->text ( i ) ); // if spy is not running - we do it ourselves
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
void Select_spectra_form::select_all()
{

    ListBox_available->firstItem() ;
    for ( unsigned int i = 0 ; i < ListBox_available-> count() ; i++ )
    {
        ListBox_chosen->insertItem ( ListBox_available->text ( i ) );
    }

    if ( ListBox_chosen->count() > 80 )
    {
        QMessageBox::warning
        ( 0, "A lot of chosen spectra" ,
          QString ( "Warning: Your list of chosen spectra has %1 item now" ).arg ( ListBox_chosen->count() ),
          QMessageBox::Yes | QMessageBox::Default,
          QMessageBox::NoButton );
    }

    add_to_history ( ComboBox_filter->currentText() ) ;
}
//*************************************************************************************
void Select_spectra_form::create_the_sum_spectrum()
{

    bool ok = FALSE;
    QString tekscik = QInputDialog::getText (
                          tr ( "Cracow viewer - defining a new SUM specrtrum" ),
                          tr ( "Currently selected spectra can contribute to the new SUM spectrum.\n"
                               "NOTE: you can sum only such spectra which have the same binning and ranges\n\n"
                               "The sum spectrum name should start with prefix '@sum_'.\n"
                               "It will automatically have an extension '.spc'\n\n",
                               "Please enter name of the sum spectrum" ),
                          QLineEdit::Normal,
                          "@sum_", // QString::null,
                          &ok, this );


    QStringList  list_of_names;   // list of spectra contributing to the sum
    QStringList  list_of_factors;   // list of spectra contributing to the sum



    if ( ok && !tekscik.isEmpty() )
    {
        // if somebody forgot to give prefix
        string::size_type poz =tekscik.find ( "@sum_", 0 ) ;
        if ( poz == string::npos )
            tekscik.insert ( 0, "@sum_" ) ;

        QString fname = path.Qspectra() + tekscik ;
        poz = fname.find ( ".spc",0 );
        if ( poz == string::npos )
            fname += ".spc" ;

        QFile plik_check ( fname ) ;
        bool already_exists = plik_check.exists();
        bool use_from_disk = false ;
        if ( already_exists )
        {


            switch ( QMessageBox::information ( this,
                                                "Such sum spectrum already exists",
                                                "The sum spectrum definition with such a name was already defined\n"
                                                "Would you like to update the old definition or create the new",
                                                //"( Yes => all, No => only the one selected)",
                                                "Edit the previous definition",  // QMessageBox::Yes | QMessageBox::Default,
                                                "create the NEW definition",   // QMessageBox::No,
                                                "Cancel", 1 ) )

            {
                //case QMessageBox::Yes :
            case 0 :
                use_from_disk = true;
                break ;
                //case QMessageBox::No :
            case 1:
                use_from_disk = false;
                break ;

            default:
                return ;
            }
        }
        if ( use_from_disk )
        {
            ifstream plik ( fname ) ;
            string nnn, fff ;
            while ( plik )
            {
                plik >> nnn >> fff ;
                if ( !plik )
                    break;
                list_of_names += nnn ;
                list_of_factors += fff ;
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



            ListBox_chosen->firstItem() ;

            for ( unsigned int i = 0 ; i < ListBox_chosen-> count() ; i++ )
            {
                //cout <<  " tresc " <<  (ListBox_chosen->text(i) )  << endl ;

                string spec_name = ListBox_chosen->text ( i ).ascii() ;

                // is it  1D spectrum ??? --------------------------
                poz = spec_name.rfind ( ".spc" );
                //cout << "poz = " << (int) poz << endl ;

                if ( poz != ( spec_name.length() - 4 ) )
                {
                    QMessageBox::critical ( this,
                                            "Sum spectra creator warning",
                                            QString ( "The spectrum %1 is not a 1D spectrum with the extension '*.spc'\n"
                                                      "Please remove it from the list and start again" ).arg ( spec_name.c_str() ),
                                            QMessageBox::Ok,
                                            QMessageBox::NoButton,
                                            QMessageBox::NoButton );

                    // remove the file
                    remove
                    ( fname.ascii() );
                    // refreshing the list with all available spectra
                    selected_filter();
                    return ;

                }


                // is it another sum spectrum ??? --------------------------
                poz = spec_name.find ( "@sum_", 0 );
                // cout << "poz = " << (int) poz << endl ;

                if ( poz != string::npos )
                {
                    QMessageBox::critical ( this,
                                            "Sum spectra creator warning",
                                            QString ( "The spectrum %1 is already a sum spectrum. You can not nest the summing\n"
                                                      "Please remove it from the list and start again" ).arg ( spec_name.c_str() ),
                                            QMessageBox::Yes,
                                            QMessageBox::NoButton,
                                            QMessageBox::NoButton );
                    // remove the file
                    remove
                    ( fname.ascii() );
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
                            QMessageBox::critical ( this,
                                                    "Sum spectra creator warning",
                                                    QString ( "The spectrum %1 has not the same binning as the first one on the list\n\n"
                                                              "%2\nhas bins = %3, begin = %4, end = %5\n\n"
                                                              "%6\nhas bins = %7, begin = %8, end = %9\n\n"
                                                              "Please remove the wrong spectrum from the list and start again"
                                                            ).arg ( spec_name.c_str() )
                                                    .arg ( spec_name.c_str() ).arg ( bin ).arg ( beg ).arg ( end )
                                                    .arg ( spec_first.c_str() ).arg ( bin_first ).arg ( beg_first ).arg ( end_first )
                                                    ,
                                                    QMessageBox::Ok,
                                                    QMessageBox::NoButton,
                                                    QMessageBox::NoButton );
                            // remove the file
                            remove
                            ( fname.ascii() );
                            // refreshing the list with all available spectra
                            selected_filter();
                            return ;

                        }
                    }
                }

                //        plik <<  (ListBox_chosen->text(i) ) << endl ;
                list_of_names += ListBox_chosen->text ( i ) ;
                list_of_factors += "+1";

            }
        } // end if else   from disk/create new

        // now we will call the editor to give the chance to change the factors
        Tsum_spectra_dialog dlg;
        dlg.set_parameters ( &list_of_names, &list_of_factors );
        if ( dlg.exec() == QDialog::Accepted )
        {

            // now we can store the results on the disk
            ofstream plik ( fname ) ;
            QStringList::Iterator itfac =  list_of_factors.begin();
            for ( QStringList::Iterator it = list_of_names.begin() ; it != list_of_names.end() ;  ++it, ++itfac )
            {
                plik << ( *it ) << "\t" << ( *itfac ) << endl;
            }




            // inform about succses ==================================
            QMessageBox::information ( this,
                                       "Sum spectra creator",
                                       QString ( "The sum spectrum %1 was succesfully created.\n\n"
                                                 "Note: \n"
                                                 "Actually the sum spectrum is just a list of names of spectra\n"
                                                 "which will be summed every 10 seconds and the result will be displayed on the screen. \n"
                                                 "If you want to save such a spectrum as a real spectrum,\n"
                                                 "just choose File->SaveAs from the main menu bar).\n\n"
                                               ).arg ( fname ),
                                       QMessageBox::Ok,
                                       QMessageBox::NoButton,
                                       QMessageBox::NoButton );

            int how_many = ListBox_chosen-> count() ;

            if ( how_many > 40 )
                QMessageBox::warning ( this,
                                       "This Sum spectrum can slow down the communication with you",
                                       QString ( "The sum spectrum %1 \ndemands summing %2 spectra\n\n"
                                                 "Warning: \n"
                                                 "Summing hundreds of spectra (every 30 seconds) can dramatically slow down\n"
                                                 "the communication between you and the cracow gui \n"
                                                 "If you really need such a spectrum, consider producing it direcly in the SPY\n"

                                               ).arg ( fname ).arg ( how_many ),
                                       QMessageBox::Ok,
                                       QMessageBox::NoButton,
                                       QMessageBox::NoButton );
        } // end of the sum dialog accepted

        // refreshing the list with all available spectra
        selected_filter();


    }
    else
    {
        ;// user entered nothing or pressed Cancel
    }

}
//*********************************************************************
/** No descriptions */
bool Select_spectra_form::check_common_binnings ( string spec_name,
        double *bin,
        double *beg,
        double *end )
{

    string desc_name = path.spectra + "description_of_ranges.txt";
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
    //  cout << "Finally description of the spectrum " << spec_name << " was not found " << endl ;
    return false ;
}
//********************************************************************************
// extended mode is when we want to use CTRL/SHIFT keys - just like in Windows
//********************************************************************************
void Select_spectra_form::extended_selection_mode ( bool extended )
{
    ListBox_available->setSelectionMode ( extended ? QListBox::Extended : QListBox::Multi ) ;
    ListBox_chosen  ->setSelectionMode ( extended ? QListBox::Extended : QListBox::Multi ) ;
}
//*********************************************************************
void Select_spectra_form::add_to_history ( QString s )
{

    // checking if such a string already is on the list`
    for ( int i = 0 ; i < ComboBox_filter->count(); i++ )
    {
        if ( i == ComboBox_filter->currentItem () ) // current is new
            continue ;
        if ( s == ComboBox_filter->text ( i ) )
            return ; // because it was already there
    }
    // if it is not on the list yet...

    // ComboBox_filter->insertItem(s + "*") ; <--this was for debugging
    // ComboBox_filter->insertItem(s + "*") ;

    // now, rearrange - to put always on Top

    for ( int i = ComboBox_filter->count() -1 ; i >0 ; i-- )
    {
        ComboBox_filter->text ( i ) = ComboBox_filter->text ( i-1 );
    }
    ComboBox_filter->insertItem ( s, 0 ) ;


    //  ComboBox_filter->insertItem(s) ;

    // contents of the history
    //   for(int i = 0 ; i < ComboBox_filter->count(); i++ )
    //   {
    //     cout << i << ") " <<  ComboBox_filter->text(i) << "   " ;
    //   }
    //   cout << endl;

}
//*********************************************************************
void Select_spectra_form::select_chosen_as_ascii()
{

    int answ ;
    switch ( answ = QMessageBox::information ( this,
                    "Copy binary spectra into ASCII format",
                    QString (
                        "This option takes chosen 1D binary spectra (*.spc) \n"
                        "and copies them converting into the two column ASCII format (*.asc)\n\n"


                        "Note: The binary format of spy is:\n"
                        "\t\t\t1. double word  - how many bins\n"
                        "\t\t\t2. double word  - left edge of the first bin\n"
                        "\t\t\t3. double word  - right edge of the last bin\n\n"
                        "\t\t\t4. long int word - value in the bin first bin\n"
                        "\t\t\t5. long int word - value in the second bin\n"
                        "\t\t\t   ....... and so on, till the contents of the last bin\n\n"


                        "After this conversion you will have the new ASCII file with two columns of numbers\n"
                        "\tLeft column with values of the left edge of the particular bin\n"
                        "\tRight column with the contents of the particlular bin\n\n"

                        "The two column ASCII format allows you to read the spectrum into\n"
                        "such a programs as for example Excel.\n\n\n"

                        "The result spectra (*.asc) will be stored in the same directory:\n " + path.Qspectra()
                        + "\n\nDo you want to create an ASCII version of the chosen spectra ?"
                    ),

                    "Yes","No","Cancel", 1 ) )
    {
    case 0:// Yes :

        bool flag_radware = false;
        int odp = QMessageBox::information ( this, "Do you want Radware header?",
                                             QString (
                                                     "If it is a 1D spectrum you may have so called Radware header\n"
                                                     "Do you want a Radware header in 1D spectra?"
                                             ),
                                             "Yes","No","Cancel", 1 );

        if ( odp == 2 ) // nr 2 is cancel
            return;

        if ( odp == 0 ) // nr. 0 is: QMessageBox::Yes
            flag_radware = true;


        int numFiles = ListBox_chosen-> count() ;

        QString mess = QString ( "Copying and converting files" );
        QProgressDialog progress ( mess, "Abort Copying", numFiles, this, "progress", TRUE );

        progress.show();

        if ( numFiles >0 )
        {

            for ( unsigned int i = 0 ; i < ListBox_chosen-> count() ; i++ )
            {
                //plik << (ListBox_chosen->text(i) )  << endl;
                if ( ListBox_chosen->text ( i ).contains ( ".spc" ) )
                {

                    QString mess2 = QString ( "Copying and converting %1   (%2 of %3 files)" )
                                    .arg ( ListBox_chosen->text ( i ) ).arg ( i ).arg ( numFiles );

                    //cout << "progres =  " << i << endl;
                    progress.setLabelText ( mess2 );
                    progress.setProgress ( i );
                    qApp->processEvents();

                    if ( progress.wasCancelled() )
                        break;
                    //... copy one file
                    QString old_name = path.Qspectra() + ListBox_chosen->text ( i ) ;
                    QString new_name =  old_name ;
                    new_name =  new_name.replace ( ".spc", ".asc" );
                    //cout << "Saving spectrum " << new_name << endl;

                    ifstream we ( old_name.ascii(), ios::binary );
                    if ( !we )
                    {
                        QMessageBox::critical ( this,
                                                "Error while opening output file",
                                                QString ( "The spectrum file %1 can not be opened" ).arg ( old_name ),
                                                QMessageBox::Ok,
                                                QMessageBox::NoButton,
                                                QMessageBox::NoButton );
                        return ;
                    }

                    ofstream wy ( new_name.ascii() );
                    if ( !wy )
                    {
                        QMessageBox::critical ( this,
                                                "Error while opening output file",
                                                QString ( "The spectrum file %1 can not be created" ).arg ( new_name ),
                                                QMessageBox::Ok,
                                                QMessageBox::NoButton,
                                                QMessageBox::NoButton );
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

                        string nazwa_no_path = new_name.ascii();
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

                    for ( int i = 0 ; ( i < bin ) && we; i++ )
                    {
                        we.read ( ( char* ) &value, sizeof ( value ) );
                        if ( we.eof() )
                            break ;
                        wy << left << "\t" << value << "\n" ;
                        left += wid ;
                    }
                } //  end if  *.spe
            }
            progress.setProgress ( numFiles );
            break ;
        } // switch
    }
}
//**********************************************************************
void Select_spectra_form::create_the_overplot_spectrum()
{
    bool ok = FALSE;
    QString tekscik = QInputDialog::getText (
                          tr ( "Cracow viewer - defining a new OVERLAY specrtrum" ),
                          tr ( "Currently selected spectra can contribute to the new OVERLAY spectrum.\n"
                               //"NOTE: you can sum only such spectra which have the same binning and ranges\n\n"
                               "The overlay spectrum name should start with prefix '@over_'.\n"
                               "It will automatically have an extension '.spc'\n\n",
                               "Please enter name for the overlay spectrum" ),
                          QLineEdit::Normal,
                          "@over_", // QString::null,
                          &ok, this );

    if ( ok && !tekscik.isEmpty() )
    {
        // if somebody forgot to give prefix
        string::size_type poz =tekscik.find ( "@over_", 0 ) ;
        if ( poz == string::npos )
            tekscik.insert ( 0, "@over_" ) ;

        QString fname = path.Qspectra() + tekscik ;
        poz = fname.find ( ".spc",0 );
        if ( poz == string::npos )
            fname += ".spc" ;

        ofstream plik ( fname ) ;

        // user entered something and pressed OK
        //cout << tekscik << " will contain spectra " << endl ;

        // we will check the binnings comparing with the first spectrum on the list

        //      double bin_first = 0,
        //          beg_first = 0,
        //          end_first = 0 ;
        string spec_first ;

        ListBox_chosen->firstItem() ;

        for ( unsigned int i = 0 ; i < ListBox_chosen-> count() ; i++ )
        {
            //cout <<  " tresc " <<  (ListBox_chosen->text(i) )  << endl ;
            string spec_name = ListBox_chosen->text ( i ).ascii() ;
            // is it  1D spectrum ??? --------------------------
            poz = spec_name.rfind ( ".spc" );
            //cout << "poz = " << (int) poz << endl ;
            if ( poz != ( spec_name.length() - 4 ) )
            {
                QMessageBox::critical ( this,
                                        "Overlay spectra creator warning",
                                        QString ( "The spectrum %1 is not a 1D spectrum with the extension '*.spc'\n"
                                                  "Please remove it from the list and start again" ).arg ( spec_name.c_str() ),
                                        QMessageBox::Ok,
                                        QMessageBox::NoButton,
                                        QMessageBox::NoButton );
                // remove the file
                remove ( fname.ascii() );
                // refreshing the list with all available spectra
                selected_filter();
                return ;
            }
            // is it another sum spectrum ??? --------------------------
            poz = spec_name.find ( "@", 0 );
            // cout << "poz = " << (int) poz << endl ;
            if ( poz != string::npos )
            {
                QMessageBox::critical ( this,
                                        "overlay spectra creator warning",
                                        QString ( "The spectrum %1 is a quasi-spectrum. You can not use @ spectra to overlay\n"
                                                  "Please remove it from the list and start again" ).arg ( spec_name.c_str() ),
                                        QMessageBox::Yes,
                                        QMessageBox::NoButton,
                                        QMessageBox::NoButton );
                // remove the file
                remove
                ( fname.ascii() );
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
                        ( fname.ascii() );
                        // refreshing the list with all available spectra
                        selected_filter();
                        return ;
                    }
                }
            }
#endif
            plik << ( ListBox_chosen->text ( i ) ) << endl ;
        }

        // inform about succses
        QMessageBox::information ( this,
                                   "Overlay spectrum creator",
                                   QString ( "The overlay spectrum %1 was succesfully created.\n\n"
                                             "Note: \n"
                                             "Actually the overlay spectrum is just a list of names of spectra\n"
                                             "which will be overlayed every 1 minue and the result will be displayed on the screen. \n"
                                           ).arg ( fname ),
                                   QMessageBox::Ok,
                                   QMessageBox::NoButton,
                                   QMessageBox::NoButton );

        int how_many = ListBox_chosen-> count() ;

        if ( how_many > 40 )
            QMessageBox::warning ( this,
                                   "This overlay spectrum can slow down the communication with you",
                                   QString ( "The overlay spectrum %1 \ndemands painting on the screen  %2 spectra\n\n"
                                             "Warning: \n"
                                             "overlayming hundreds of spectra (every minute) can dramatically slow down\n"
                                             "the communication between you and the cracow gui \n"
                                           ).arg ( fname ).arg ( how_many ),
                                   QMessageBox::Ok,
                                   QMessageBox::NoButton,
                                   QMessageBox::NoButton );
        // refreshing the list with all available spectra
        selected_filter();
    }
    else
    {
        ;// user entered nothing or pressed Cancel
    }
}
//******************************************************************************************************
void Select_spectra_form::right_mouse_on_chosen ( QListBoxItem * itemek, const QPoint & e )
{
    //  cout << "F.right_mouse_on_chosen() " << endl ;
    if ( itemek == 0 )
    {
        //    cout << "BUT 0 " << endl ;
        return ; // nothing or more than one selected
    }
    unsigned int nr   = ListBox_chosen-> index ( itemek );
    QString one_selected  = ListBox_chosen->text ( nr ) ;

    // context menu -------------
    QPopupMenu *contekst_Menu = new QPopupMenu;

    contekst_Menu->insertItem ( "Move this item UP", 1 );
    contekst_Menu->insertItem ( "Move this item DOWN", 2 );

    contekst_Menu->setItemEnabled ( 1, ( nr != 0 ) ) ; // delete
    contekst_Menu->setItemEnabled ( 2, ( nr != ListBox_chosen->count() -1 ) ) ; // delete

    //int result = contekst_Menu->  exec(mapToGlobal(QPoint(e.x() ,e.y()   )) );
    int result = contekst_Menu->  exec ( QPoint ( e.x() ,e.y() ) );
    bool up = false ;
    switch ( result )
    {
    case 1:    // ----------------------set marker
        up = true ;
        break ;

    case 2:   // ---------------------- erase all markers
        up = false ;
        break ;

    default:
        return ;
    }
    // moving up
    // what is selected
    // only one may be selected
    ListBox_chosen->removeItem ( nr );
    ListBox_chosen->insertItem ( one_selected, nr+ ( up?  -1: 1 ) );
    //ListBox_chosen->clearSelection ();
}
//*****************************************************************************************************
void Select_spectra_form::zero_all_available_spectra()
{
    QString list_of_spectra= "You are going to erase the contents of the ALL the   online SPY spectra'\n\n"
                             "ARE YOU SURE  ?";
    switch ( QMessageBox::warning ( this, " 'Zero-ing' online spectra, ARE YOU SURE ? ",
                                    list_of_spectra,
                                    "Yes","No","Cancel", 1 ) )
    {
    case 0:   // Yes :
        switch ( QMessageBox::warning ( this, " 'Zeroing ALL analysis specta  ",
                                        "Not only those filtered spectra, but ALL spectra will be zero-ed, ARE YOU SURE ?",
                                        "Yes","No", "Cancel", 1 ) )
        {
        case 0:   // Yes :
        {
            // cout << "Spectrum to zero = " <<   dokument->caption() << endl;
            string command_file = path.commands + "spy_spectra_for_zeroing.command";

            ofstream plik ( command_file.c_str() );
            if ( !plik )
                cout << "problem in openning the file" << command_file << endl;

            // taking the spectra names from the directory - only
            // opening all spectra files from current directory
            QDir katalog ( path.Qspectra() ); // ("spectra") ;

            QStringList lista = katalog.entryList ( "*.spc *.mat" ) ;


            QProgressDialog progress ( "Zeroing All spectra on the disk...",
                                       "Abort zeroing", lista.size(),
                                       this, "progress", TRUE );
            progress.setMinimumDuration ( 5000 ) ; // miliseconds
            progress.setProgress ( 0 );
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
                if ( progress.wasCancelled() )
                    break;
                progress.setProgress ( ++counter );
                progress.setLabelText ( QString ( "Zero-ing:  " ) + ( *it ) );


                //plik << *it << endl ;
                if ( appl_form_ptr->is_spy_running() )
                {
                    plik << *it << endl ;   // we can ask spy to do it
                }
                else
                {
                    appl_form_ptr->zeroing_really_on_disk ( *it ); // if spy is not running - we do it ourselves
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
void Select_spectra_form::filter_dialog()
{
    QString filtr = ComboBox_filter->currentText ();
    bool ok;
    QString text = QInputDialog::getText (
                       "Fliter, without updating (use  it  when  you have a  slow  disk)", "Enter the text filter string:", QLineEdit::Normal,
                       ComboBox_filter->currentText (),
                       &ok, this );
    if ( ok && !text.isEmpty() )
    {
        // user entered something and pressed OK
        ComboBox_filter->setCurrentText ( text );
    }
    else
    {
        // user entered nothing or pressed Cancel
    }
}

//**********************************************************************************************************************
void Select_spectra_form::add_spectra_names_currently_displayed()
{

    QWidgetList windows = arch_ws->windowList();
    spectrum_widget * dokument  = ( spectrum_widget * ) windows.first() ;
    while ( dokument )
    {
        string name =  dokument->give_spectrum_name();
        ListBox_chosen->insertItem ( name );

        dokument  = ( spectrum_widget * ) windows.next() ;
        if ( !dokument )
            break ;
    }

}

};
