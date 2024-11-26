#include "appl_form.h"
#include "ui_appl_form.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
using namespace std;

#include <unistd.h>
#include <stdio.h>


#include <QTimer>
#include <QDir>
#include <QMessageBox>
#include <QInputDialog>
#include <QProgressDialog>
#include <QString>

#include "t4results_of_integration.h"
#include "t4select_spectra.h"
#include "tplate_spectrum.h"
#include "spectrum_1D.h"
#include "spectrum_2D.h"
#include "paths.h"
#include "options.h"
extern Tpaths gpath;
extern options current_options;
#include "t4setting_paths_dlg.h"
#include "t4size_spectrum_dlg.h"
#include "Tfile_helper.h"
#include "t4setting_times_dlg.h"
#include "t4binning_change_dlg.h"
#include "t4merge_2d_spectra_dlg.h"
#include "t4about_dlg.h"
#include "t4welcome.h"
#include "t4info_dlg.h"
#include "t4auto_calib_wizard.h"
#include "t4manager_user_spectra.h"
#include "t4manager_user_conditions.h"
#include "t4important_dlg.h"
#include "t4user_spectrum_wizard.h"
#include "t4collect_ntuple.h"
#include "t4addback_dlg.h"
#include "t4zet_aoq_ranges.h"
#include "t4verbose_dlg.h"
#include "t4good_minibal.h"
#include "t4doppler_dlg.h"
#include "t4other_spy_options.h"
#include "t4good_galileo_ge_dlg.h"
#include "t4good_ger_dlg.h"
#include "t4good_hector_dlg.h"
#include "t4good_kratta_dlg.h"
#include "t4lycca_parameters.h"
#include "t4active_stopper_dlg.h"
#include "t4manager_user_incrementers.h"
#include "t4options_visibility.h"
#include "t4update_info_dlg.h"
#include "tgui_options.h"
#include "t4interesting_ranges_galileo.h"
#include "tgalileo_signals_dlg.h"
#include "Tfile_line_det_cal_lookup.h"
#include "t4tiling_marix_dlg.h"
#include "t4incrementer_selector.h"
#include "t4checking_incrementers_dlg.h"

// #undef COTO
// #define COTO

// extern
appl_form  *appl_form_ptr;

T4results_of_integration * integr_resul_ptr;
bool allow_secret_part ;

const string user_spec_ext { ".user_definition" };
const string user_spec_filter { "*" + user_spec_ext };

const string user_cond_ext { ".cnd" };
const string user_cond_filter { "*" + user_cond_ext };

const string user_incr_ext { ".incr" };
const string user_incr_filter { "*" + user_incr_ext };

extern string ROWS_COLUMNS_KEYWORD;

int general_font_size = 8;
QColor background_colour_of_1D_spectrum = QColor(0, 0, 60);

extern bool global_flag_after_crash;
// enum below is defined in spectrum_D.h
// enum refresh_big_matrices_policy { not_decided, skip_one =1, skip_all, refresh_one, refresh_all};
refresh_big_matrices_policy policy {not_decided};
//********************************************************************
appl_form::appl_form ( QWidget *parent_w )
    : QMainWindow ( parent_w ), ui ( new Ui::appl_form )
{
    // error "incorrect sRGB profile - comes out of the fact of using some *.png file
    // which was made in the wrong colour profile.
    // No harm at all, but would be nice to stop this warning

    ui->setupUi ( this );

    //=============== init () ===========
    appl_form_ptr = this;
    area = ui->mdiArea;


    //  QToolBar * firstBarSecondLine = ui->toolBar_2;
    //  ui->mainToolBar->insertToolbarBreak( firstBarSecondLine );


    // This below is defining slots in case if user selected
    // another spectrum window to work.

    // so far we do not change anything in menus

    connect ( ui->mdiArea, SIGNAL ( subWindowActivated ( QMdiSubWindow* ) ),
              this, SLOT ( updateMenus() ) );

    // but we update actions icons in toolbox, bec. some actions are for 1D, some other for 2D
    connect ( ui->mdiArea, SIGNAL ( subWindowActivated ( QMdiSubWindow* ) ),
              this, SLOT ( updateActions() ) );


    // chnage font for everybody
    QFont sansFont_ogolny ( "Sans",general_font_size);   // was : 9
    //QFont sansFont_ogolny ( "Sans", 12 );
    //    setFont ( sansFont_ogolny );
    QApplication::setFont(sansFont_ogolny);

    flag_reaction_for_scroll_slider_event = false;

    //flag_act_on_all_spectra = false ;
    flag_act_on_all_spectra = true;
    flag_log_scale = false ;
    flag_show_polygons = true ;
    flag_show_filenames_of_gates = false ;
    printer_call_line = "gimp &" ;
    printer_call_line = current_options.give_snapshot_program().c_str();



    if(gpath.read_from_disk() == false)
    {

        show() ;
        QMessageBox::warning
                (this, "Where is spy working directory?",
                 "Directory \n\n     " + gpath.Qspy_dir() +
                 "\n\n is probably NOT a legal spy directory \n"
                 " (as it does not have a subdirectory 'spectra' )\n\n"
                 "The next dialong window will help you to define location of spy",
                 QMessageBox::Ok,
                 QMessageBox::NoButton,
                 QMessageBox::NoButton
                 );

        //cout << "wrong directory tree - no spectrua subdirectory" << endl;
        set_paths();


    }
    // cout << "Current s directory = " << gpath.spy_dir << endl;
    try{
        ifstream option (gpath.options + "gui_options.options");
        if(option)
        {

            int tmp = static_cast<int>(  FH::find_in_file(option, "background_colour_of_1D_spectrum"));
            background_colour_of_1D_spectrum.setRed(tmp);
            option >> zjedz >> tmp ;
            background_colour_of_1D_spectrum.setGreen(tmp);
            option >> zjedz >> tmp ;
            background_colour_of_1D_spectrum.setBlue(tmp);
            general_font_size = FH::find_in_file(option, "general_font_size");

        }
    }catch(...)
    {
        //   cout << "File gui_options.options - does not exist" << endl;
    }

    // group combo box initializing done after reading paths !
    refresh_combo_groups();

    should_sliders_be_enabled();

    important_dlg_ptr  = new T4important_dlg ;

    // dialog for results of integration etc, must be one for all the spectra widgets
    integr_resul_ptr = resul_ptr = new T4results_of_integration ;

    spy_works_timer = new QTimer ( this );
    connect ( spy_works_timer, SIGNAL ( timeout() ),
              this, SLOT ( check_status_of_spy() ) );

    spy_works_timer->start ( 1 * 1000 ) ; // 1000 is 1 second

    ui->spy_go_action->setEnabled ( false );
    ui->spy_pause_action->setEnabled ( true );


    // timer "watchdog" of spy errors (in GSI - "synchronisation lost" )
    synchro_check_timer = new QTimer ( this );
    connect ( synchro_check_timer, SIGNAL ( timeout() ),
              this, SLOT ( check_synchro_of_spy() ) );

    synchro_check_timer ->start ( 1 * 1000 ) ; // 1000 is 1 second


    // allow_secret_part = false ;   // secret part is untested pard
    allow_secret_part = true ;

    ui->incrementer_managerAction->setEnabled ( allow_secret_part );
    ui->actionRead_and_install_the_frs_calibration_factors_from_an_external_file->setEnabled(false);

    //  conditon_edit_Action->setEnabled(allow_secret_part);

    enable_some_actions ( 0 ); // at init - everything disabled


    flag_draw_spectra_scales = true;
    // press it really once more
    ui->draw_scale_Action->setChecked( flag_draw_spectra_scales ); //setOn ( flag_draw_spectra_scales );




    adjust_program_to_experiment_type();

    show() ;

    read_last_spectra_selection_after_crash();


    if(global_flag_after_crash)    {
        //        QMessageBox::information
        //                (this, "After crash recovery",
        //                 "Don't worry, your selection of last spectra - is restored",
        //                 "OK");
        on_actionSelecting_the_spectra_triggered();
    }else{

#if 0  // FINAL    // works, but now I prefer it blocked
        T4welcome *dlg = new T4welcome ;
        dlg->exec();
        delete dlg ;
#endif
    }
    set_visibility_of_spy_options_dialogs();

    spectrum_1D::read_from_disk_sets_of_markers();
}
//********************************************************************
appl_form::~appl_form()
{
    delete ui;
}
//********************************************************************
void appl_form::on_actionSelecting_the_spectra_triggered()
{

    //    cout << "F. " << __PRETTY_FUNCTION__ << endl ;

    //    static QString last_filter = "" ;
    static QStringList  last_filters;


    // static vector<string> last_spectra_selection ; <--- moved to be a member data

    // to have only one copy of the dialog on the screen...
    ui->actionSelecting_the_spectra->setEnabled ( false );

    T4select_spectra * dlg  = new T4select_spectra ( this);
    dlg->set_parameters ( area );
    dlg->set_filter ( last_filters );
    dlg->set_listbox_chosen ( last_spectra_selection ) ;

    dlg ->show();

    if ( dlg->exec() == QDialog::Accepted )
    {
        //                cout << "Dialog accepted " << endl;
        //dlg->set_parameters ( ws );   // <---------------------- dlaczego po raz drugi?
        QApplication::setOverrideCursor ( QCursor ( Qt::WaitCursor ) );
        //         COTO;

        remove_existing_spectra_windows();
        dlg->show_chosen_spectra();

#define MY_TILE_AFTER_SELECT true

#if MY_TILE_AFTER_SELECT
        on_actionTile_triggered(); // my version
#else
        area->tileSubWindows();
#endif

        should_sliders_be_enabled();
        QApplication::restoreOverrideCursor();

    }

    last_filters = dlg->give_last_filter();
    //     cout << "after closing the window - reported last filter as = " << last_filter.toStdString() << endl;
    last_spectra_selection = dlg->give_listbox_chosen() ;
    //cout << "after exec()  the select spectra form" << endl;
    delete dlg;
    dlg = nullptr; // null;

    // now conect all spectra
    QList <QMdiSubWindow*>  windows = area->subWindowList();

    if ( ! windows.empty() )
    {

        Tplate_spectrum  * dokument ;

        for ( int nr_dok = 0 ; nr_dok < windows.count() ; nr_dok++ )
        {

            //             cout << "nr " << nr_dok << " of " << windows.count() << endl;
            dokument  = static_cast<Tplate_spectrum *> ( windows[nr_dok]->widget()) ;
            if(dokument == nullptr) continue; // to prevent segment violation

            // the real action of this loop
            connect (
                        dokument, SIGNAL ( spec_update_scrolls_and_sliders() ),
                        this, SLOT ( appl_update_scrolls_and_sliders() )
                        );

            //             cout << "******** before connecting in selected combo groups"
            //                  << dokument->caption().toStdString() << endl ;

            connect (
                        dokument, SIGNAL ( enable_selected_actions ( int ) ),
                        this, SLOT ( enable_some_actions ( int ) )
                        );
            // dokument  = ( Tplate_spectrum * ) windows[nr_dok++];  // .next() ;
            //  dokument->setFocus();
        }

        // we must know if the current document is 1D or 2D spectrum

        if ( area->currentSubWindow() )
        {
            dokument  = static_cast<Tplate_spectrum *> ( area-> currentSubWindow()->widget())  ;
            if(dokument != nullptr) { // to prevent segment violation

                //             int dim = ( dynamic_cast<spectrum_1D*> ( dokument ) == 0 ) ? 2 : 1 ;
                //             dokument->give_dimension();

                //     cout   << ", dimmension of spectrum " << dim << endl;
                enable_some_actions ( dokument->give_dimension() );
            }
        }
        else enable_some_actions ( 0 );

        save_names_of_spectra_currently_watched();
        refresh_combo_groups();
    }

    ui->actionSelecting_the_spectra->setEnabled ( true );
    set_current_undo_text_in_menu();
    //    cout << "End of F. " << __PRETTY_FUNCTION__ << endl ;


}

//********************************************************************************************
void appl_form::remove_existing_spectra_windows()
{
    //    cout << __PRETTY_FUNCTION__ << endl;
    QList <QMdiSubWindow*> windows = area->subWindowList()  ;
    for ( int i = 0 ; i < windows.count()  ; i++ )
    {
        auto doc  = static_cast<Tplate_spectrum *> ( windows[i]->widget()) ;
        if(doc == nullptr) continue;

        //        cout << "Destroying window nr " << i << endl;
        doc->close() ;
        doc->destroy();
        if ( doc->isHidden() )
            delete doc;

    }
    //    cout << __PRETTY_FUNCTION__ << " line= " << __LINE__ << endl;
    area->closeAllSubWindows();
    //    cout << __PRETTY_FUNCTION__ << " line= " << __LINE__ << endl;
    qApp->processEvents();
    should_sliders_be_enabled();
    save_names_of_spectra_currently_watched();
    //    cout << __PRETTY_FUNCTION__ << " line= " << __LINE__ << endl;
    enable_some_actions ( 0 );

    // policy of handling huge 2D matces - reset here after finishing working
    // on putting spectra on the screen
    policy = not_decided;

    //    cout << "End of function " << __PRETTY_FUNCTION__ << endl;
}
//*************************************************************************
void appl_form::enable_some_actions ( int dimension )
{
    //     cout << "Appl form:: enable_some_actions, arg=" << typ << endl;

    ui->incrementer_managerAction->setEnabled ( allow_secret_part );
    //  conditon_edit_Action->setEnabled(allow_secret_part);


    ui->actionSelecting_the_spectra->setEnabled(true);
    qApp->processEvents();
    ui->actionCall_wizard_to_edit_the_CURRENT_user_def_spectrum->setEnabled( it_is_user_def_spectrum());
    //ui->user_spec_wizard_Action-> setEnabled ( it_is_user_def_spectrum() );
    switch ( dimension )
    {
    default:   // newJG
    case  1: // 1D spectra
        //cout << "Case 1 selected - 1D " << endl;
        ui->expand_spectrum_action->setEnabled ( true );
        ui->current_spectrum_Action->setEnabled ( true );
        ui->all_spectra_Action->setEnabled ( true );
        ui->Integrate_spectrum_action->setEnabled ( true ); // <-----------always enabled
        ui->show_hide_polygons_action->setEnabled ( false );
        ui->lupa_action->setEnabled ( false );
        ui->parameters_Action->setEnabled ( true );
        ui->matrix_projection_Action->setEnabled ( false );
        ui->petrify_Action->setEnabled ( true );

        // all true, when anykind of spectrum on screan
        ui->full_spectrum_action->setEnabled ( true );
        ui->shift_left_action->setEnabled ( true );
        ui->shift_right_action->setEnabled ( true );
        ui->Yscale05_action->setEnabled ( true );
        ui->Yscale2_action->setEnabled ( true );
        ui->Yscale_fit_action->setEnabled ( true );
        ui->black_white_action->setEnabled ( true );
        ui->filePrintAction->setEnabled ( true );
        ui->log_scale_Action->setEnabled ( true );
        ui->Action_undo->setEnabled ( true );
        ui->draw_scale_Action->setEnabled ( true );
        ui->apply_to_others_Action->setEnabled ( true );
        ui->toolButton_apply_to_others->setEnabled ( true );
        // 			zero_everything_Action->setEnabled ( true );
        ui->toolButton_apply_to_others->setEnabled ( true );
        ui->actionShow_full_names_of_polygon_gates->setEnabled(false);
        ui->actionChange_the_binning_and_range->setEnabled(true);
        ui->actionCustom_tiling->setEnabled(true);


        break;

    case 2:
        //cout << "Case 2 selected - 2D " << endl;
        ui->expand_spectrum_action->setEnabled ( false );

        ui->current_spectrum_Action->setEnabled ( true );
        ui->all_spectra_Action->setEnabled ( true );

        ui->show_hide_polygons_action->setEnabled ( true );
        ui->lupa_action->setEnabled ( true );
        ui->parameters_Action->setEnabled ( true );
        ui->matrix_projection_Action->setEnabled ( true );
        ui->petrify_Action->setEnabled ( false );
        // all true, when anykind of spectrum on screan
        ui->full_spectrum_action->setEnabled ( true );
        ui->shift_left_action->setEnabled ( true );
        ui->shift_right_action->setEnabled ( true );
        ui->Yscale05_action->setEnabled ( true );
        ui->Yscale2_action->setEnabled ( true );
        ui->Yscale_fit_action->setEnabled ( true );
        ui->black_white_action->setEnabled ( true );
        ui->filePrintAction->setEnabled ( true );
        ui->log_scale_Action->setEnabled ( true );
        ui->Integrate_spectrum_action->setEnabled ( true ); // <---- always enabled
        ui->Action_undo->setEnabled ( false );
        ui->draw_scale_Action->setEnabled ( true );
        ui->apply_to_others_Action->setEnabled ( true );
        ui->toolButton_apply_to_others->setEnabled ( true );
        ui->toolButton_apply_to_others->setEnabled ( true );
        ui->actionShow_full_names_of_polygon_gates->setEnabled(true);
        ui->actionChange_the_binning_and_range->setEnabled(true);
        ui->actionCustom_tiling->setEnabled(true);

        //zero_everything_Action->setEnabled ( true );
        break;

    case 0: // nothing on the screen

        ui->expand_spectrum_action->setEnabled ( false );
        ui->current_spectrum_Action->setEnabled ( false );
        ui->all_spectra_Action->setEnabled ( false );
        ui->Integrate_spectrum_action->setEnabled ( false );
        ui->show_hide_polygons_action->setEnabled ( false );
        ui->lupa_action->setEnabled ( false );
        ui->parameters_Action->setEnabled ( false );
        ui->matrix_projection_Action->setEnabled ( false ) ;
        ui->petrify_Action->setEnabled ( false );

        ui->full_spectrum_action->setEnabled ( false );
        ui->shift_left_action->setEnabled ( false );
        ui->shift_right_action->setEnabled ( false );
        ui->Yscale05_action->setEnabled ( false );
        ui->Yscale2_action->setEnabled ( false );
        ui->Yscale_fit_action->setEnabled ( false );
        ui->black_white_action->setEnabled ( false );
        ui->filePrintAction->setEnabled ( true );
        ui->log_scale_Action->setEnabled ( false );
        ui->Action_undo->setEnabled ( false );
        ui->draw_scale_Action->setEnabled ( false );
        ui->apply_to_others_Action->setEnabled ( false );
        ui->toolButton_apply_to_others->setEnabled ( false );
        ui->actionShow_full_names_of_polygon_gates->setEnabled(false);
        ui->actionChange_the_binning_and_range->setEnabled(false);
        ui->actionCustom_tiling->setEnabled(false);

        //zero_everything_Action->setEnabled ( false );
    }
    //set_current_undo_text_in_menu();???????

}
//*************************************************************************
//***************************************************************************************
void appl_form::save_names_of_spectra_currently_watched()
{
    // it is good to tell the spy which spectra we are currently watching

    string command_file = gpath.commands + "spectra_currently_watched.names";
    ofstream plik ( command_file.c_str() );
    if ( !plik )
    {
        cout << "problem in openning the file" << command_file << endl;
    }

    // now conect all spectra

    QList <QMdiSubWindow*> windows = area->subWindowList();
    for ( int nr = 0 ; nr < windows.count() ; nr++ )
    {
        Tplate_spectrum * dokument  = dynamic_cast<Tplate_spectrum *> ( windows[nr]->widget() );
        if ( !dokument )
        {
            // cout << "bzdurny dokument dla nr =" << nr << endl;
            continue;
        }

        // byproduct
        if ( dokument)   // ( dokument->isVisible() )
        {
            string spec_name = dokument->give_spectrum_name();
            if ( spec_name.find ( "@sum_", 0 ) != 0 )  // not starting the name, so the normal situation
            {
                plik << dokument->give_spectrum_name() << endl;
                //cout  << dokument->give_spectrum_name() << endl;
            }
            else
            {
                plik << dokument->give_spectrum_name() << endl;
                //                // this is a sum spectrum, so include all real spectra
                //                string naz = gpath.spectra + spec_name ;
                //                ifstream plik_sum ( naz.c_str() );
                //                string nnn ;
                //                while ( plik_sum )
                //                {
                //                    plik_sum >> nnn ;
                //                    plik << nnn << endl ;
                //                }
                //                plik_sum.close() ;
            }
        }
        if ( !plik )
        {
            cout << "problem in writing to file" << command_file << endl;
        }
    }

    plik.close();


}
//**************************************************************************************************
void appl_form::refresh_combo_groups()
{

    QDir katalog ( gpath.groups.c_str() ) ;
    QStringList lista_grup = katalog.entryList ( QStringList("*.group")  ) ;

    ui->Combo_app_group->clear();
    ui->Combo_app_group->addItem ( "Group names:" );  // insertItem ( "Group names:" );
    for ( QStringList::Iterator it = lista_grup.begin() ; it != lista_grup.end() ;  ++it )
    {
        //cout << *it << "\n";
        // removing the extension ".group"
        QString anystring = *it ;
        int i = anystring.lastIndexOf(".group") ;   // .findRev ( ".group" );
        anystring.truncate ( i );
        ui->Combo_app_group->addItem ( anystring );
    }
}
//****************************************************************************
void appl_form::set_current_undo_text_in_menu()
{
    // not making undo, just providing a simple explanation which is shown
    // in the main menu.


    // finding the selected spectrum
    if ( ! area->currentSubWindow() ) return;

    QList <QMdiSubWindow*> windows = area->subWindowList();
    if ( windows.empty() ) return;

    Tplate_spectrum * doc = dynamic_cast< Tplate_spectrum *> ( area-> currentSubWindow()->widget() );
    if ( doc /*&& doc->hasFocus()*/ )
    {
        QString descr_text = QString ( "Undo: " ) +  doc->give_undo_text ( 0 ).c_str();
        ui->Action_undo->setText(descr_text);//    setMenuText ( descr_text );
        ui->Action_undo->setToolTip ( descr_text );
        ui->Action_undo->setEnabled ( true ) ;

        //cout << "Setting the undo menu into " << descr_text.toStdString() << endl;
    }
    else
    {
        ui->Action_undo->setText ( "Undo: Not available now" );
        ui->Action_undo->setEnabled ( false ) ;
    }
}
//********************************************************************************************
bool appl_form::should_sliders_be_enabled()
{
    bool some_spectra_displayed ;

    QList <QMdiSubWindow*> windows = area->subWindowList();
    if ( windows.empty() )
    {
        some_spectra_displayed = false ;
    }
    else
    {
        some_spectra_displayed = true ;
    }
    ui->ScrollBar_poziomy_widma->setEnabled ( some_spectra_displayed );
    ui->ScrollBar_pionowy_widma->setEnabled ( some_spectra_displayed );

    //  set_current_undo_text_in_menu();   <-- why was this?
    return some_spectra_displayed;
}

//**************************************************************************************
//******************************************************************************
bool appl_form::it_is_user_def_spectrum()
{
    //cout << " appl_form::it_is_user_def_spectrum() --------------------------"<< endl;

    qApp->processEvents();

    QList <QMdiSubWindow*> windows = area->subWindowList();
    if(windows.count() < 1) return false;
    if(area-> currentSubWindow() == nullptr) return false;
    //COTO;
    // finding the selected spectrum
    Tplate_spectrum * doc = static_cast<Tplate_spectrum *> ( area-> currentSubWindow()->widget())  ;
    //  COTO;
    if(!doc) return false;
    //    cout << "Current window is " <<  doc->give_spectrum_name() << endl;

    string naz = doc->give_spectrum_name();
    //cout << naz << endl;

    if ( naz.find ( "user_" ) != string::npos )
    {
        //cout << "Spectrum " << naz << " is user def !!!" << endl ;
        return true ;
    }
    //    cout << "Spectrum " << " is NOT us def !!!" << endl ;
    return false ;

}
//******************************************************************************
void appl_form::addSubWindow ( Tplate_spectrum * s, Qt::WindowFlags windowFlags )
{
    ui->mdiArea->addSubWindow ( s, windowFlags );
    //     cout << "Dodane okno, teraz jest ich "
    //          << ui->mdiArea->subWindowList().count() << endl;
}

//********************************************************************
//********************************************************************
//********************************************************************
//void appl_form::on_action_fileNew_triggered()
//{

//}
////********************************************************************
//void appl_form::on_action_fileNew_activated()
//{

//}
//********************************************************************
void appl_form::send_to_statusbar ( std::string m, int timeout )
{
    ui->statusBar->showMessage ( m.c_str(), timeout );
}
//********************************************************************
void appl_form::warning_spy_is_in_action()
{
    //   "!!! New parameters are not applied yet !!!",
    //     "The spy is currently analysing the data\n"
    //     "Your new settings will be applied when you start it again or just \n"
    //     "press the UPDATE icon ",

    //#if FINAL
    if ( ui->spy_pause_action->isEnabled() )
    {
        T4update_info_dlg dlg;
        if ( dlg.exec() == QDialog::Accepted )
        {
            //  cout << "Now will be update ..." << endl;
            //            spy_update();
            on_actionSpy_Update_pause_and_run_again_triggered();
        }
    }
    //#endif
}
//********************************************************************
time_t  appl_form::give_time_of_zeroing_spectrum ( std::string name )
{

    restore_times_of_zeroing_from_disk();

    time_t kiedy = 0;
    map<string, time_t> ::iterator pos = map_of_zeroing.find ( name ) ;
    if ( pos != map_of_zeroing.end() )
    {
        kiedy = pos->second;
    }
    // cout << "Returning the time of zeroing" << endl;
    return kiedy;
}
//***********************************************************************
void appl_form::on_actionPath_to_the_spy_directory_triggered()
{
    set_paths();
}
//***********************************************************************
void appl_form::set_paths()
{
    T4setting_paths_dlg dlg;

    dlg.set_spy_path ( gpath.spy_dir );
    if ( dlg.exec() == QDialog::Accepted )
    {
        gpath.set_spy_dir(dlg.get_spy_path());
        //      cout << "Before saving on disk" << dlg.get_spy_path()
        //           << ", in the same time path = " << gpath.spy_dir
        //           << endl;
        gpath.save_to_disk() ;
        //      cout << "After saving on disk the path = " << gpath.spy_dir  << endl;
    }
}
//***********************************************************************
void appl_form::on_actionTile_with_system_function_triggered()
{

    //     QList <QMdiSubWindow*> windows = ws->subWindowList()  ;
    //     ws->

    // ws->setActivationOrder(QMdiArea::StackingOrder);
    area->setActivationOrder(QMdiArea::CreationOrder);  // does not work as expected

    area->tileSubWindows();
    //    QList <QMdiSubWindow*> windows = ws->subWindowList()  ;

    //    cout << "System tile"
    //         << " width " << width()
    //         << " ws->height() = " << ws->height()
    //         << endl;

    //    for ( int i = 0 ; i < windows.count()  ; i++ )
    //    {
    //        //        auto wsk = (windows[i] );
    //        //
    //        cout << " x: " << windows[i]->x()
    //        << " y: " << windows[i]->y()
    //        << " width: " << windows[i]->width()
    //        << " height: " << windows[i]->height()
    //                << endl;
    //    }

}
//***********************************************************************
void appl_form::on_actionTile_triggered()
{
#if 0
    ws->tileSubWindows();   // the official one - gives wrong (opposite) order
#else

    // bez tego  poniżej  dotychczasowewybrane  stare widmo  czasem robi sie bez ramki.
    on_actionTile_with_system_function_triggered();

    // cout << __func__ << ", not a system one!" << endl;
    // MY versions, should be (?) better?
    //// one spectrum is below the other

    QList <QMdiSubWindow*> windows = area->subWindowList()  ;

    if(windows.empty() ) return;

    int how_many_spec = windows.count();
    int pion = sqrt(how_many_spec );
    int poziom = pion;
    if(how_many_spec == 3)
    {
        pion = 2;
        poziom = 2;
    }

    while (how_many_spec - (pion * poziom) > 0)
    {
        ++poziom;
    }

    int empty_place = 0; // (pion * poziom) - how_many_spec;

    //difference = abs(how_many_spec - (pion * poziom)) ;


    int vertical_distance = (area->height() - 2) / pion;
    int horizontal_distance = (area->width() - 2) / poziom;

    //    vertical_distance -= 1;
    //    horizontal_distance -=1;


    int counter = 0 ;
    int extra = empty_place ;
    for ( int y = 0 ; y < pion ; ++y)
    {
        for(int x = 0 ; x < poziom ; ++x, ++counter, --empty_place)
        {
            if(counter >= how_many_spec) break;
            if(y == 1 && x ==0) x+= extra;

            int wys = (empty_place > 0) ? (2* vertical_distance) : vertical_distance;
            windows[counter]->setGeometry(x*horizontal_distance,
                                          vertical_distance * y,
                                          horizontal_distance,
                                          wys
                                          );
            //            cout << "spectrum nr " << counter << ", name =" ;


            //                         cout <<  windows[counter]->windowTitle().toStdString()  ;

            //                 cout << ", x = " << (x*horizontal_distance)
            //                 << ", y = " << (y*vertical_distance) << endl;
        }
        //        windows[i]->update();
    }

    //    cout << "Moje tile"
    //         << " width " << width()
    //         << " ws->height() = " << ws->height()
    //         << endl;

    //        for ( int i = 0 ; i < windows.count()  ; i++ )
    //        {
    //            //        auto wsk = (windows[i] );
    //            //wsk->resize( 400,100);
    //            cout << " x: " << windows[i]->x()
    //            << " y: " << windows[i]->y()
    //            << " width: " << windows[i]->width()
    //            << " height: " << windows[i]->height()
    //                    << endl;
    //        }
#endif
}
//********************************************************************
void appl_form::on_actionCascade_triggered()
{
    area->cascadeSubWindows();

    // cascade  makes it very narrow (vertically). So we correct
    QList <QMdiSubWindow*> windows = area->subWindowList()  ;

    int how_many_spectra = windows.count();
    if(how_many_spectra < 1) return;

    int how_many_columns = 1;
    int spec_width = 200;
    int spec_hight = 100;

    int vertical_distance = (area->height() - spec_hight)/ (how_many_spectra);

    while(vertical_distance < 50){
        ++how_many_columns;
        vertical_distance = (area->height() - spec_hight) * how_many_columns / (how_many_spectra );
    }

    int horizontal_distance = 10;

    int column_distance = (area->width() - spec_width) / how_many_columns ;
    if(spec_width < (column_distance/2) ) spec_width = column_distance / 2 ;

    int spectra_in_column = how_many_spectra / how_many_columns;
    ++spectra_in_column; // just to be sure

    for ( int i = 0 ; i < how_many_spectra ; ++i )
    {
        int y = i % spectra_in_column;
        int column = (i / spectra_in_column);

        if(windows[i] == nullptr) continue;

        windows[i]->setGeometry(
                    i * horizontal_distance + (column *column_distance),
                    y * vertical_distance,
                    spec_width,
                    spec_hight );
    }
}
//***********************************************************************
void appl_form::on_actionSpectrum_below_spectrum_triggered()
{
    // one spectrum is below the other
    QList <QMdiSubWindow*> windows = area->subWindowList()  ;

    if(windows.empty() ) return;

    int vertical_distance = (area->height() ) / windows.count();
    for ( int i = 0 ; i < windows.count()  ; i++ )
    {
        if(windows[i] == nullptr) continue;
        //        auto wsk = (windows[i] );
        //wsk->resize( 400,100);
        windows[i]->setGeometry(0, vertical_distance * i, area->width()-1, vertical_distance);
        //        windows[i]->update();
    }
}
//********************************************************************
void appl_form::on_actionClose_all_spectra_triggered()
{
    remove_existing_spectra_windows();
}
//*************************************************************************
void appl_form::on_shift_left_action_triggered()
{
    Tplate_spectrum * doc = ( Tplate_spectrum * ) area-> currentSubWindow()->widget();
    if ( !flag_act_on_all_spectra )
    {
        if ( doc )
            doc->shift_left ( 1 );
    }
    else
    {
        //apply_to_all_spectra();
        QList <QMdiSubWindow*> windows = area->subWindowList()  ;
        for ( int i = 0 ; i < windows.count()  ; i++ )
        {
            doc  = ( Tplate_spectrum * ) windows[i]->widget() ;
            if(!doc) continue;

            doc->shift_left ( 1 );
        }
    }

    set_current_undo_text_in_menu();
}
//*************************************************************************
void appl_form::on_shift_right_action_triggered()
{
    Tplate_spectrum * doc = ( Tplate_spectrum * ) area-> currentSubWindow()->widget();

    if ( flag_act_on_all_spectra )
    {
        //apply_to_other_spectra();
        QList <QMdiSubWindow*> windows = area->subWindowList()  ;
        for ( int i = 0 ; i < windows.count()  ; i++ )
        {
            doc  = ( Tplate_spectrum * ) windows[i]->widget() ;
            if(!doc) continue;
            doc->shift_right ( 1 );
        }
    }
    else  if ( doc )
        doc->shift_right ( 1 );
    set_current_undo_text_in_menu();
}
//**********************************************************************************************
void appl_form::on_act_on_all_spectra_Button_toggled ( bool checked )
{
    //cout << "F. ALL ---> stan = " << checked ;
    flag_act_on_all_spectra = checked ;
    ui->act_on_one_spectrum_Button->setChecked ( !checked );
    //     cout << "F. ALL   flag_act_on_all = " << flag_act_on_all_spectra << endl ;

}
//**********************************************************************************************
void appl_form::on_act_on_one_spectrum_Button_toggled ( bool checked )
{

    //    cout << " F. ONE  ---> stan = " << checked ;
    flag_act_on_all_spectra = !checked;
    //     ui->act_on_one_spectrum_Button->setChecked(checked);
    ui->act_on_all_spectra_Button->setChecked ( !checked );
    //      cout << "F ONE  now flag_act_on_all_spectra is = " << flag_act_on_all_spectra << endl ;
}
//**********************************************************************************************
void appl_form::on_toolButton_select_spectra_clicked()
{
    on_actionSelecting_the_spectra_triggered();
}
//**********************************************************************************************
void appl_form::on_full_spectrum_action_triggered()
{

#if 0
    cout << "appl_form::on_full_spectrum_action_triggered() - there are subwindows "
         << ws->subWindowList().count()
         << ", active has address " << ws->activeSubWindow()
         << ", current has address " << ws->currentSubWindow()
         << endl ;

    if ( ws-> currentSubWindow() == NULL )
    {
        cerr << "Alarm "<< endl;
        if ( ws->subWindowList().count() )
        {
            QList <QMdiSubWindow*> windows = ws->subWindowList()  ;
            ws->setActiveSubWindow ( windows[0] ) ;  //   subWindowList().  count()

            cout << "2222 appl_form::on_full_spectrum_action_triggered() - there are subwindows "
                 << ws->subWindowList().count()
                 << ", active has address " << ws->activeSubWindow()
                 << ", current has address " << ws->currentSubWindow()
                 << endl ;

        }
        //return;
    }
    //    if( ws-> currentSubWindow()->widget() == 0 )
    //        return ;

#endif




    Tplate_spectrum * doc = ( Tplate_spectrum * ) area-> currentSubWindow()->widget();
    if ( flag_act_on_all_spectra )
    {

        //apply_to_other_spectra();
        QList <QMdiSubWindow*> windows = area->subWindowList()  ;
        for ( int i = 0 ; i < windows.count()  ; i++ )
        {
            doc  = static_cast<Tplate_spectrum *> ( windows[i]->widget()) ;
            if(!doc) continue;
            doc->full ();
        }
    }
    else  if ( doc )
        doc->full();  // only one

    set_current_undo_text_in_menu();
}
//**********************************************************************************************
void appl_form::on_expand_spectrum_action_triggered()
{
    // COTO;
    // finding the selected spectrum and asking fora a expand
    Tplate_spectrum * doc = ( Tplate_spectrum * ) area-> currentSubWindow()->widget();
    if ( doc )
        doc->expand();

    // pozostale tez przesuwam
    if ( flag_act_on_all_spectra )
        apply_to_other_spectra();
    set_current_undo_text_in_menu();
}
//**********************************************************************************************
//************************************************************************
// this function can apply display settings to the other (not current spectra)
void appl_form::apply_to_other_spectra()
{
    //     COTO;
    //     cout << "F. Apply to other spectra " << endl ;

    qApp->processEvents(); // in case if some dialog was opened and curent subwindow
    // is not active

    //     void *ptr = ws-> currentSubWindow();
    //     if ( !ptr )
    //     {
    //         cout << "no active sub Window now !!!!!!!!!!!!!!" << endl;
    //         // return;
    //
    //         // this may happen if you opened some dialog window
    //         // so focus did't return yet... (see function  appl_form::on_parameters_Action_triggered)
    //     }


    Tplate_spectrum * doc = ( Tplate_spectrum * ) area-> currentSubWindow()->widget();


    //  flag_reaction_for_scroll_slider_event = true;

    if ( doc )
    {
        //doc->   scroller_vertical_moved( value_bottom );
        typ_x min_x = 0 ;
        typ_x max_x = 4096 ;
        typ_x min_yy = 0 ;
        typ_x max_yy = 100 ;


        spectrum_descr sd  ;
        doc->give_parameters ( &min_x, &max_yy,  &max_x, &min_yy, &sd );
        //         COTO;

        // here we could check if the origingal was 1D or 2d
        spectrum_1D *orig_1D = dynamic_cast<spectrum_1D *> ( doc ) ;

        // we do not use sd results , but why ?


        // now apply this to all spectra on the list

        QList <QMdiSubWindow*> windows     = area->subWindowList()  ;
        for ( int i = 0 ; i < windows.count() ; i++ )
        {
            //cout << "spectrum window nr = " << i << endl;
            Tplate_spectrum * dokument  = dynamic_cast<Tplate_spectrum*> ( windows[i]->widget() );
            if(!dokument) continue;

            spectrum_1D *curr_1D = dynamic_cast<spectrum_1D *> ( dokument ) ;
            if ( doc != dokument )
            {
                //                cout << "orig = " << orig_1D << " curr = " << curr_1D << endl ;
                if (
                        ( orig_1D  && curr_1D )  // both are 1D
                        ||                                     //  OR
                        ( orig_1D == nullptr && curr_1D == nullptr ) // both are 2D
                        )
                {
                    //                    cout << "this is the same type of spectrum" << endl;
                    dokument-> set_parameters ( min_x, max_yy,  max_x, min_yy );
                }
                else
                {
                    //                    cout << "this is different  type of spectrum"<< endl;

                }

                dokument-> log_linear ( flag_log_scale );
                //dokument-> draw_all_on_screen();
                dokument->show_polygons ( flag_show_polygons );
                dokument->show_disk_names_of_gates ( flag_show_filenames_of_gates );
            } // if
        } // for
    }
}
//*************************************************************************************
void appl_form::on_Yscale05_action_triggered()
{

    Tplate_spectrum * doc = ( Tplate_spectrum * ) area-> currentSubWindow()->widget();
    if ( flag_act_on_all_spectra )
    {

        //apply_to_other_spectra();
        QList <QMdiSubWindow*> windows = area->subWindowList()  ;
        for ( int i = 0 ; i < windows.count()  ; i++ )
        {
            doc  = ( Tplate_spectrum * ) windows[i]->widget() ;
            if(!doc) continue;
            doc->scaleY_by_05 ();
        }
    }
    else  if ( doc )
        doc->scaleY_by_05();  // only one

    set_current_undo_text_in_menu();

}
//*************************************************************************************
void appl_form::on_Yscale2_action_triggered()
{
    Tplate_spectrum * doc = ( Tplate_spectrum * ) area-> currentSubWindow()->widget();
    if ( flag_act_on_all_spectra )
    {

        //apply_to_other_spectra();
        QList <QMdiSubWindow*> windows = area->subWindowList()  ;
        for ( int i = 0 ; i < windows.count()  ; i++ )
        {
            doc  = ( Tplate_spectrum * ) windows[i]->widget() ;
            if(!doc) continue;
            doc->scaleY_by_2 ();
        }
    }
    else  if ( doc )
        doc->scaleY_by_2();  // only one

    set_current_undo_text_in_menu();

}
//*************************************************************************
void appl_form::on_Action_undo_triggered()
{

    //  cout << "somebody pressed undo" << endl ;
    // COTO;

    //     if ( ! ws->currentSubWindow() ) return ;

    Tplate_spectrum * doc = ( Tplate_spectrum * ) area-> currentSubWindow()->widget();
    flag_reaction_for_scroll_slider_event = false;
    if ( doc )
        doc->undo() ;    //  1 /10-th of the screen
    // pozostale tez przesuwam
    // if(flag_act_on_all_spectra ) apply_to_other_spectra();

}
//*************************************************************************
void appl_form::on_toolButton_apply_to_others_clicked()
{
    apply_to_other_spectra();
}
//********************************************************************************
void appl_form::on_draw_scale_Action_triggered ( bool stan )
{
    flag_draw_spectra_scales = stan ;


    //     if ( ws->currentSubWindow() == NULL ) return ;


    Tplate_spectrum * doc = ( Tplate_spectrum * ) area-> currentSubWindow()->widget();
    if ( doc )
        doc->draw_scale_or_not ( stan );


    if ( flag_act_on_all_spectra )
    {
        //apply_to_other_spectra();
        QList <QMdiSubWindow*> windows = area->subWindowList()  ;
        for ( int i = 0 ; i < windows.count()  ; i++ )
        {
            doc  = ( Tplate_spectrum * ) windows[i]->widget() ;
            if(!doc) continue;
            doc->draw_scale_or_not ( stan );
        }
    }
}
//*************************************************************************************
void appl_form::on_parameters_Action_triggered()
{
    // COTO;

    Tplate_spectrum * doc = ( Tplate_spectrum * ) ( area-> currentSubWindow()->widget() );

    //  flag_reaction_for_scroll_slider_event = true;
    if ( doc )
    {
        //doc->   scroller_vertical_moved( value_bottom );
        typ_x min_x = 0 ;
        typ_x max_x = 4096 ;
        typ_x min_yy = 0 ;
        typ_x max_yy = 100 ;

        spectrum_descr sd ;
        doc->give_parameters ( &min_x, &max_yy,  &max_x, &min_yy, &sd );
        //        COTO;

        T4size_spectrum_dlg *dlg  =  new  T4size_spectrum_dlg();

        vector<typ_x> tab;
        tab.push_back ( min_x );
        tab.push_back ( max_x );
        tab.push_back ( min_yy );
        tab.push_back ( max_yy );

        int dim = ( dynamic_cast<spectrum_1D*> ( doc ) == nullptr ) ? 2 : 1 ;

        dlg->set_parameters ( tab, sd, dim ) ;


        if ( dlg->exec() == QDialog::Accepted )
        {
            // cout << "parameters were accepted  " << endl ;

            vector<typ_x> result = dlg->give_parameters() ; // the result is a pointer to an array
            // created in this function using the operator new
            //            COTO;

            min_x = result [0];
            max_x = result [1] ;
            min_yy = result [2] ;
            max_yy = result [3] ;

            //            cout << "After the execution "
            //            << " (min_x = " << min_x
            //            << " - max_x = " << max_x
            //            << ")   (min_yy = " << min_yy
            //            << " - max_yy = " << max_yy
            //            << ")"
            //            << endl ;

            // now apply this to spectrum view

            doc->set_parameters ( min_x, max_yy,  max_x, min_yy );


            if ( flag_act_on_all_spectra )
            {
                apply_to_other_spectra();
            }
            set_current_undo_text_in_menu();
        }
        delete dlg;

    }

}
//********************************************************************************************
void appl_form::on_log_scale_Action_triggered ( bool stan )
{

    // cout << "appl_form::on_log_scale_Action_triggered ( bool stan =)" << stan << endl;

    //     if ( ws->currentSubWindow() == NULL ) return ;

    flag_log_scale = stan ;
    // press it really once more
    ui->log_scale_Action->setChecked( stan );

    Tplate_spectrum * doc = ( Tplate_spectrum * ) ( area-> currentSubWindow()->widget() );
    if ( doc )
        doc->log_linear ( stan );

    // pozostale tez
    if ( flag_act_on_all_spectra )
    {
        //apply_to_other_spectra();
        QList <QMdiSubWindow*> windows = area->subWindowList()  ;
        for ( int i = 0 ; i < windows.count()  ; i++ )
        {
            doc  = ( Tplate_spectrum * ) windows[i]->widget() ;
            if(!doc) continue;
            doc->log_linear ( stan );
        }
    }

}
//********************************************************************************************
void appl_form::on_actionSave_triggered()
{
    // which is selected now?
    // this can be a document which is a 1D spectrum, 2D matrix !!!!

    Tplate_spectrum * doc = ( Tplate_spectrum * ) ( area-> currentSubWindow()->widget() );
    if ( doc )
        doc->save();
}
//********************************************************************************************
void appl_form::on_actionSave_as_triggered()
{
    // which is selected now?
    // this can be a document which is a 1D spectrum, 2D matrix !!!!
#ifdef NIGDY



#else

    if(!area-> currentSubWindow() ) return;
    Tplate_spectrum * doc = ( Tplate_spectrum * ) ( area-> currentSubWindow()->widget() );

    if(!doc) return;

    bool flag_all = false ;
    if ( flag_act_on_all_spectra )
    {
        switch ( QMessageBox::information ( this,
                                            " Saving As - of many spectra ?",
                                            "Currently you are in mode 'work on many spectra'.\nDo you want to save  all "
                                            "displayed spectra?  \n",
                                            //"( Yes => all, No => only the one selected)",
                                            "ALL displayed spectra",  // QMessageBox::Yes | QMessageBox::Default,
                                            "only one, SELECTED spectrum",   // QMessageBox::No,
                                            "Cancel", 1 ) )

        {

        case 0 : // Yes
            flag_all = true;
            break ;

        case 1:   // No
            flag_all = false;
            break ;

        default:
            return ;
        }

    }

    if ( !flag_all )
    {
        if ( doc )
            doc->save_as ( "" );
    }
    else  // on all
    {

        // here we can ask for a prefix

        bool ok;
        QString text = QInputDialog::getText ( this,
                                               "Choosing the prefix for spectra names",


                                               "You are going to save many files. "
                                               "All of them will be saved in an original cracow binary format \n\n"
                                               "Their new names will be constructed as follows:"
                                               "       prefix+old_name\n"
                                               "\n"
                                               //"(if you give prefix empty - you will be asked for all particular spectrum file name)"
                                               "Enter the text for prefix: ",
                                               QLineEdit::Normal,
                                               "prefix_",
                                               &ok );
        if ( ok && !text.isEmpty() )
        {
            QList <QMdiSubWindow*> windows = area->subWindowList()  ;
            for ( int i = 0 ; i < windows.count()  ; i++ )
            {

                doc  = ( Tplate_spectrum * ) windows[i]->widget() ;
                if(!doc) continue;
                if ( doc->isVisible() )
                    doc->save_as ( text.toStdString() );
            }

        } // end if ok
    }

#endif

}
//********************************************************************************************
void appl_form::on_filePrintAction_triggered()
{
    // black_white(true);
    int result = system ( printer_call_line.toStdString().c_str() );
    if ( result != 0 )
        cout << "Error in executing system  command"
             << printer_call_line.toStdString() << endl;

}
//********************************************************************************************
void appl_form::on_actionDefine_snapshot_program_to_print_triggered()
{

    bool ok;
    QInputDialog dlg;
    // dlg.setCancelButtonText("CAAAAAAancel");
    //	dlg.setCancelButtonText(tr("ABC") );
    //	dlg.setOkButtonText("DOBRZE");

    //	QLocale lokal(QLocale::English);
    //	dlg.setLocale(lokal);
    //	QInputDialog::setCancelButtonText("AAA");

    QString text = dlg.getText (this,
                                "Choose the snapshoot program",
                                "Specify here some Linux system program, which you choose for making a snapshot of the screen.\n"
                                "It mybye a `gimp` or a `spectacle`, or a 'ksnapshot' or any other.\n\n"
                                "Note: if you are using greware while being logged (by ssh) from other computer\n"
                                "some snapshot programs may behave strange: \n"
                                "Instead making snapshot of your screen,\n"
                                "they will make a snapshot of a screen of the original computer screen.\n"
                                "So if you work by ssh - try to choose another program.\n\n\n"
                                "Enter name of the snapshot program:  ",
                                QLineEdit::Normal,
                                printer_call_line,
                                &ok );
    if ( ok && !text.isEmpty() )
    {
        // user entered something and pressed OK
        printer_call_line  = text ;
        if ( printer_call_line.lastIndexOf ("&") == -1)  //    find ( "&" ) == -1 )
            printer_call_line  += "   &" ;
        current_options.set_snapshot ( printer_call_line.toStdString() );
    }
    else
    {
        // user entered nothing or pressed Cancel
    }
}
//********************************************************************************************
void appl_form::on_actionCall_wizard_to_edit_the_CURRENT_user_def_spectrum_triggered()
{


    // finding the selected spectrum
    if ( !it_is_user_def_spectrum() )
    {
        QMessageBox::warning ( this,
                               "No spectrum selected",
                               QString ( "Current spectrum is not the user defined" ),
                               QMessageBox::Ok,
                               QMessageBox::NoButton,
                               QMessageBox::NoButton );
        return ;
    }

    //read the definition of such a spectrum into the temporary object

    user_spectrum_description us ;


    if(!area-> currentSubWindow() ) return;
    Tplate_spectrum * doc = ( Tplate_spectrum * ) ( area-> currentSubWindow()->widget() );

    if(!doc) return;

    string naz = doc->give_spectrum_name();

    naz.erase ( naz.rfind ( '.' ) );
    naz += user_spec_ext ;
    if ( us.read_from ( gpath.user_def_spectra + naz ) == false )
    {
        QMessageBox::warning ( this,
                               "Error while reading the definition",
                               QString ( "Most probably the definition of such user spectrum does not exist anymore \n\n(%1)\n"
                                         "This may happen if you are watching a spectrum which was stored on the disk long time ago, and its definition is already deleted"
                                         "\nTry to edit it using the 'user spectra manager' (There you can find all currently known definitions)" )
                               .arg ( ( gpath.user_def_spectra + naz ).c_str() ),
                               QMessageBox::Ok,
                               QMessageBox::NoButton,
                               QMessageBox::NoButton );
        return ;
    }

    T4user_spectrum_wizard  dlg;
    dlg.set_parameters ( & us, true ) ; // true = editing, cloning is allowed
    if ( dlg.exec() == QDialog::Accepted )
    {
        us.write_definitions ( gpath.user_def_spectra );
        warning_spy_is_in_action();
    }

}

//********************************************************************************************
void appl_form::on_actionN_tuple_Root_Tree_options_triggered()
{
    T4collect_ntuple dlg ;
    dlg.exec() ;
}
//********************************************************************************************
void appl_form::on_addback_dlg_Action_triggered()
{
    T4addback_dlg dlg;
    dlg.exec() ;
}
//********************************************************************************************

void appl_form::on_actionInteresting_region_of_Z_and_A_Q_triggered()
{
    T4zet_aoq_ranges dlg;
    dlg.exec() ;
}
//********************************************************************************************

void appl_form::on_actionVerbose_mode_show_selected_range_of_events_on_the_screen_triggered()
{
    T4verbose_dlg dlg;
    dlg.exec() ;
}
//********************************************************************************************
void appl_form::on_actionDefinition_of_good_Miniball_data_triggered()
{
    T4good_minibal dlg ;
    dlg.exec() ;
}
//********************************************************************************************
void appl_form::on_actionDefinition_of_Good_Germanium_detectors_data_triggered()
{
    T4good_ger_dlg   dlg;
    dlg.exec() ;
}
//********************************************************************************************
void appl_form::on_actionChange_the_binning_and_range_triggered()
{
    //COTO;

    //cout << "Apply to other spectra " << endl ;

    if( area-> currentSubWindow() == nullptr) return;

    Tplate_spectrum * doc = ( Tplate_spectrum * ) area-> currentSubWindow()->widget();
    //  flag_reaction_for_scroll_slider_event = true;

    if ( doc )
    {
        // if this the user definied spectrum - we will reject
        string sname = doc->give_spectrum_name();   //    (caption() );
        if ( sname.substr ( 0, 4 ) == "user" )
        {
            QMessageBox::critical ( this, "Do it in a different way",
                                    "To change the binning of the user defined spectrum, please use:\n\n"
                                    "  'Edit -> Call wizard to edit CURRENT user def. spectrum' ",
                                    QMessageBox::Ok | QMessageBox::Default,
                                    QMessageBox::NoButton,
                                    QMessageBox::NoButton ); // error
            return;
        }

        //doc->   scroller_vertical_moved( value_bottom );
        typ_x min_x = 0 ;
        typ_x max_x = 4096 ;
        typ_x min_yy = 0 ;
        typ_x max_yy = 100 ;

        spectrum_descr sd ;
        doc->give_parameters ( &min_x, &max_yy,  &max_x, &min_yy, &sd );
        //        COTO;

        T4binning_change_dlg dlg;
        int dim = doc->give_dimension();
        dlg.set_parameters ( doc->give_spectrum_name()  , sd, dim ) ;
        dlg.exec();
    } // if doc

    // set_current_undo_text_in_menu();
}
//********************************************************************************************
void appl_form::on_actionOther_SPY_options_triggered()
{
    T4other_spy_options  * dlg = new T4other_spy_options ;
    dlg->exec() ;
    delete dlg;
}
//********************************************************************************************

void appl_form::on_actionZero_ing_all_displayed_spectra_no_questions_using_shorcut_Ctrl_Alt_Z_triggered()
{
    Tplate_spectrum * dokument = ( Tplate_spectrum * ) ( area-> currentSubWindow()->widget() );
    //      cout << "Spectrum to zero = " <<   dokument->caption().toStdString() << endl;
    //string command_file = gpath.commands + "spy_spectra_for_zeroing.command";
    ostringstream robot;
    QList <QMdiSubWindow*> windows = area->subWindowList()  ;
    for ( int i = 0 ; i < windows.count()  ; i++ )
    {
        dokument  = ( Tplate_spectrum * ) windows[i]->widget() ;
        if(!dokument) continue;
        if ( dokument->isVisible() )
        {
            robot << ( dokument->give_spectrum_name()  ) << endl ;
            dokument->zero_spectrum();
            //cout << "Ask to zero " << dokument->caption().toStdString() << endl;
            dokument-> re_read_counter_after_zeroing ( 15 );
        }

    }
    // robot << ends ;

    string command_file = gpath.commands + "spy_spectra_for_zeroing.command";
    ofstream plik ( command_file.c_str() );
    if ( !plik )
        cout << "problem in openning the file" << command_file << endl;
    plik << robot.str() << endl ;
    plik << "this_is_the_end_of_the_list_of_spectra_which_cracow_wants_to_zero" << endl;
    if ( !plik )
        cout << "problem in writing to file" << command_file << endl;
    plik.close();

}
//********************************************************************************************
void appl_form::on_actionOther_tools_for_1D_operations_triggered()
{
    QMessageBox::information ( this, "Use context sensitive menu",
                               "To have other tools for working with the 1D spectra\n"
                               " - Setting background markers\n - Creating default gate,\n"
                               " - Putting markers in the precise channel\n"
                               " - Rebining  on screen\n - Putting Tags  on the spectrum\n"
                               "please right click on the spectrum picture - you will see the context sensitive popup menu",
                               QMessageBox::Ok, QMessageBox::NoButton , QMessageBox::NoButton );
}
//********************************************************************************************
void appl_form::on_actionOther_tools_for_matrix_operations_triggered()
{
    QMessageBox::information ( this, "Use context sensitive menu",
                               "To have other tools for working with matrices\n"
                               " - Polygon gates creating, cloning, modyfing vertices, etc\n"
                               " - Integrating the contents of the polygon  gates\n"
                               "please right click on the spectrum picture - you will see the context sensitive popup menu",
                               QMessageBox::Ok, QMessageBox::NoButton , QMessageBox::NoButton );
}
//********************************************************************************************
void appl_form::on_actionHelp_about_mouse_clicking_triggered()
{
    string clicking_explanations =
            //"MOUSE CLICKING during SPECTRA (1D, 2D) DISPLAY\n\n"

            "============ In 1D spectrum: ============\n\n"

            "Single click   -  set a yellow marking line\n"
            //"Shift + Single click   -  correct the last \"single click marker\"\n"
            "Double  click   -  expand the region marked with two marking lines\n"
            "Shift  double click   -  Zoom OUT the region\n"
            "Right click - popup menu for background markers, etc.\n\n"
            "NEW: if you use a mouse ROLL on any scale (channels, or counts) you expand/compress the scale\n"
            "NEW: if you press a mouse and DRAG - on any scale (channels, or counts) you shift the scale\n"
            "\n"

            "============ In 2D spectrum: ============\n\n"
            "Double  click   -  zoom IN the region around the clicked place\n"
            "Shift double  click   -  zoom OUT the region around the clicked place\n"
            "Right click - popup menu for bananaa gates,  background markers, etc.\n\n"
            "NEW: if you use a mouse ROLL on any scale (horizontal or vertical) you expand/compress the scale\n"
            "NEW: if you press a mouse and DRAG - on any scale (horizontal or vertical) you shift the scale\n"
            "\n"


            "--- clicking on a polygon -----\n"
            "Single click  on the vertex of  polygon  gate - selects the vertex of  polygon,\n"
            "Shift single click  on the vertex of  polygon  gate -selects additional vertex of this polygon\n"
            "Click vertex and drag - moves the vertex of polygon\n"
            "Ctrl + Click on vertex (and drag) - moves a whole polygon\n"
            "Ctrl + Shift + Click on vertex (and drag) - moves selected vertices only\n\n"
            "(Remember -  in a popup menu (right click) you have an UNDO for  moving vertices)\n";

    T4info_dlg * dlg = new T4info_dlg ;
    dlg->set_parameter ( clicking_explanations.c_str() );
    dlg->set_label("<p align=\"center\"><font color=\"#ff0000\" size=\"+4\"><b>'Mouse Clicking' during spectra (1D, 2D) display</b></font></p>" );
    dlg->exec();
    delete dlg;
    //statusBar()->message("1234");
}
//********************************************************************************
void appl_form::close_document_with_name ( string name )
{
    // here we could close existing windows - or not
    QList <QMdiSubWindow*> windows = area->subWindowList()  ;
    for ( int i = 0 ; i < windows.count()  ; i++ )
    {
        Tplate_spectrum * dokument  = ( Tplate_spectrum * ) windows[i]->widget() ;
        if(!dokument) continue;

        //        cout << "Destroying widow nr " << i << endl;
        if ( name == dokument->give_spectrum_name() )
        {
            area->removeSubWindow(windows[i]);
            dokument->close() ;
            dokument->destroy();
            delete dokument ;

            break;
        }
    }
    should_sliders_be_enabled();
    save_names_of_spectra_currently_watched();
    //enable_some_actions(0);
    qApp->processEvents();
    //COTO;

}
//***********************************************************************************

void appl_form::on_petrify_Action_triggered()
{
    // cout << "F. appl_form::petrify_1D_spectrum()" << endl;
    //spectrum_widget * doc = ( spectrum_widget * ) ws-> activeWindow();
    Tplate_spectrum * doc = ( Tplate_spectrum * ) ( area-> currentSubWindow()->widget() );
    bool flag_all = false ;
    if ( flag_act_on_all_spectra )
    {
        switch ( QMessageBox::information ( this,
                                            " Freezing shadows  of  many spectra ?",
                                            "Currently you are in mode 'work on many spectra'.\nDo you want to freeze shadows of all "
                                            "displayed spectra?  \n",
                                            //"( Yes => all, No => only the one selected)",
                                            "ALL displayed spectra",  // QMessageBox::Yes | QMessageBox::Default,
                                            "only one, SELECTED spectrum",   // QMessageBox::No,
                                            "Cancel", 1 ) )

        {
        case 0 : // Yes
            flag_all = true;
            break ;
        case 1:   // No
            flag_all = false;
            break ;
        default:
            return ;
        }
    }

    if ( flag_all == false )
    {
        if ( doc )
            doc->freezing_photo_of_spectrum();  // work on only one
    }
    else     //apply_to_all_spectra();
    {

        QList <QMdiSubWindow*> windows = area->subWindowList()  ;
        for ( int i = 0 ; i < windows.count()  ; i++ )
        {

            doc  = ( Tplate_spectrum * ) windows[i]->widget() ;
            doc->freezing_photo_of_spectrum ( );
        }
    }

}
//********************************************************************************************
void appl_form::on_actionZero_ing_ALL_SPECTRA_triggered()
{
    QString list_of_spectra = "You are going to erase the contents of the ALL the   online SPY spectra'\n\n"
                              "ARE YOU SURE  ?";

    switch ( QMessageBox::warning ( this, " 'Zero-ing' online spectra, ARE YOU SURE ? ",
                                    list_of_spectra,
                                    "Yes", "No", "Cancel", 1 ) )
    {
    case 0:   // Yes :

        switch ( QMessageBox::warning ( this, " 'Zeroing ALL analysis specta  ",
                                        "Not only those shown spectra shown on the screen, but ALL spectra will be zero-ed, ARE YOU SURE ?",
                                        "Yes", "No", "Cancel", 1 ) )
        {
        case 0:   // Yes :
        {
            // cout << "Spectrum to zero = " <<   dokument->caption() << endl;
            string command_file = gpath.commands + "spy_spectra_for_zeroing.command";

            ofstream plik ( command_file.c_str() );
            if ( !plik )
                cout << "problem in openning the file" << command_file << endl;

            // taking the spectra names from the directory - only
            // opening all spectra files from current directory
            QDir katalog ( gpath.Qspectra() );   // ("spectra") ;

            QStringList filter;
            filter << "*.spc"<<  "*.mat" ;
            QStringList lista = katalog.entryList ( filter) ;

            // cout << "sciezka to " <<gpath.Qspectra().toStdString() << endl;


            //                    QProgressDialog progress ( "Zeroing All spectra on the disk...",
            //                                               "Abort zeroing", 0, lista.size(),
            //                                               this );
            //                    progress.setMinimumDuration ( 5000 ) ; // miliseconds
            //                    progress.setValue ( 0 );
            int counter = 0;

            if ( ! is_spy_running() )
            {
                // if we do zeroing on disk ourselves, we will need to set the time of zeroing
                restore_times_of_zeroing_from_disk();
            }

            for ( QStringList::Iterator it = lista.begin() ; it != lista.end() ;  ++it )
            {
                //                        qApp->processEvents();
                //                        if ( progress.wasCanceled() )
                //                            break;
                ++counter;

                //                        progress.setValue (counter );

                //                         if( (counter % 10 ) == 0)
                //                        {
                //                             progress.setValue (counter );
                //                             progress.setLabelText ( QString ( "Zero-ing:  " ) + ( *it ) );
                //                         }


                //cout << "To zero " << it->toStdString() << "\n";
                if ( ui->actionZero_ing_all_currently_displayed_spectra->isEnabled() )
                {
                    plik << ( *it ).toStdString() << endl ; // we can ask spy to do it
                }
                else
                {
                    zeroing_really_on_disk ( ( *it ).toStdString() ); // if spy is not running - we do it ourselves
                }
            }

            plik << "this_is_the_end_of_the_list_of_spectra_which_cracow_wants_to_zero" << endl;
            if ( !plik )
                cout << "problem in writing to file" << command_file << endl;
            plik.close();


            if ( ! is_spy_running() )
            {
                // if we do zeroing on disk ourselves, we need to set the time of zeroing
                save_times_of_zeroing_on_disk();
            }

            QList <QMdiSubWindow*> windows = area->subWindowList()  ;
            for ( int i = 0 ; i < windows.count()  ; i++ )
            {
                auto dokument  = ( Tplate_spectrum * ) windows[i]->widget() ;
                if ( dokument->isVisible() )
                {
                    //robot << ( dokument->give_spectrum_name()  ) << endl ;
                    dokument->zero_spectrum();
                    //cout << "Ask to zero " << dokument->caption().toStdString() << endl;
                    dokument-> re_read_counter_after_zeroing ( 15 );
                }

            }

            break;
        }

        default:
            cout << "No, or Cancel" << endl;
            break;
        }
    default:
        break;
    }
}
//********************************************************************************************
void appl_form::on_actionSum_many_2D_spectra_triggered()
{
    // cout << "Merging selected from menu bar " << endl;
    T4merge_2d_spectra_dlg  * dlg = new T4merge_2d_spectra_dlg;
    dlg->exec() ;
    delete dlg;
}
//********************************************************************************************
void appl_form::on_active_stopper_optionsAction_triggered()
{
    T4active_stopper_dlg  * dlg = new T4active_stopper_dlg;
    int result = dlg->exec() ;
    delete dlg;
    if ( result == QDialog::Accepted )
        warning_spy_is_in_action();
}
//********************************************************************************************
void appl_form::on_incrementer_managerAction_triggered()
{
    T4manager_user_incrementers  *dlg = new T4manager_user_incrementers;
    int result = dlg->exec(); // everthing is done in the member funcitons
    delete dlg;
    if ( result == QDialog::Accepted )
        warning_spy_is_in_action();
}
//********************************************************************************************
void appl_form::on_active_stopper_munich_Action_triggered()
{
#ifdef FINAL
    Tactive_stopper_munich_dlg  * dlg = new Tactive_stopper_munich_dlg;
    int result = dlg->exec() ;
    delete dlg;
    if ( result == QDialog::Accepted )
        warning_spy_is_in_action();
#endif
}
//********************************************************************************************
//*******************************************************************************
void appl_form::adjust_program_to_experiment_type()
{
#ifdef NIGDY
    ifstream plik ( ( gpath.options + "/experiment_version.dat" ).c_str() );
    if ( !plik )
    {
        cout << "Can't open file: options/experiment_version.dat"    << endl;
        return ;
    }
#define RISING_STOPPED_BEAM_CAMPAIGN     1000
#define RISING_FAST_BEAM_CAMPAIGN        1100
#define G_FACTOR_OCTOBER_2005           1200
#define RISING_ACTIVE_STOPPER_BEAM_CAMPAIGN     1300
#define RISING_ACTIVE_STOPPER_100TIN                       1400
#define RISING_ACTIVE_STOPPER_APRIL_2008            1500
#define RISING_ACTIVE_STOPPER_JULY_2008            1600
#define RISING_LYCCA_JUNE_2010            1700
    try
    {
        int v;
        plik >> v;

        cout << "Experiment recognised as id=" << v << endl;

        switch ( v )
        {
        case RISING_LYCCA_JUNE_2010:
            cout << "Experiment recognised as " << v << endl;

            ui->actionMatching_the_MBS_subevents->setVisible ( false );
            ui->active_stopper_munich_Action->setVisible ( false ); // disable munich
            ui->active_stopper_optionsAction->setVisible ( false ); // disable active stopper
            ui->actionDefinition_of_Good_CATE_data->setVisible ( false );
            ui->actionDefinition_of_good_Miniball_data->setVisible ( false );
            ui->actionRead_and_install_the_frs_calibration_factors_from_an_external_file->setVisible ( false );
            //ui->show_trigger_14_Action->setVisible ( false );
            break;

        default:
            cout << "Nothing predefined, so all options are anabled" << endl;
            break;
        }

    }
    catch ( ... )
    {

    }

    // chwilowo $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

    //    ui->actionMatching_the_MBS_subevents->setVisible ( true );
    //    ui->active_stopper_munich_Action->setVisible ( true ); // disable munich
    //    ui->active_stopper_optionsAction->setVisible ( true ); // disable active stopper
    //    ui->actionDefinition_of_Good_CATE_data->setVisible ( true );
    //    ui->actionDefinition_of_good_Miniball_data->setVisible ( true );
    //    ui->actionRead_and_install_the_frs_calibration_factors_from_an_external_file->setVisible ( true );
    //    ui->active_stopper_optionsAction->setVisible ( true ); // disable active stopper

#endif   // false
}
//********************************************************************************************
void appl_form::on_actionLYCCA_settings_triggered()
{
    T4lycca_parameters * dlg = new T4lycca_parameters ;
    int result = dlg->exec() ;
    delete dlg;
    if ( result == QDialog::Accepted )
        warning_spy_is_in_action();
}
//********************************************************************************************
void appl_form::on_actionSpy_update_quickly_without_saving_spectra_triggered()
{
    string command_file = gpath.commands ;
    command_file += "spy_pause.command";
    ofstream plik ( command_file.c_str() );
    plik << "quick   "  << endl ;
    plik.close();

    command_file = gpath.commands ;
    command_file += "spy_continue.command";
    plik.open ( command_file.c_str() );

    plik.close();

    spy_works_timer->start ( 1 * 1000 ) ;
    ui->actionSpy_Update_pause_and_run_again->setEnabled ( false );
    ui->actionSpy_update_quickly_without_saving_spectra->setEnabled ( false );
    ui->spy_pause_action->setEnabled ( false );
}
//********************************************************************************************
void appl_form::on_actionAutomatic_calibration_of_germanium_detectors_triggered()
{
    T4auto_calib_wizard dlg;
    int result = dlg.exec() ;
    if ( result == QDialog::Accepted )
        warning_spy_is_in_action();
}
//********************************************************************************************
void appl_form::on_actionMake_tar_file_with_calibrations_and_lookup_tables_triggered()
{
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    //     printf ( "Current date and time are: %s", asctime (timeinfo) );
    string date ;
    ostringstream s ( date ) ;
    s
            << "20" << setw ( 2 ) << setfill ( '0' ) << timeinfo->tm_year + ( 1900 - 2000 ) << "_"
            << setw ( 2 ) << ( timeinfo->tm_mon + 1 ) << "_"
            << setw ( 2 ) << timeinfo->tm_mday << "_"
            << setw ( 2 ) << timeinfo->tm_hour << "-"
            << setw ( 2 ) << timeinfo->tm_min << "-"
            << setw ( 2 ) << timeinfo->tm_sec ;
    string godzina = s.str() ;
    string zip_filename = "calibration_and_lookuptable_files_" + godzina + ".gz";
    //   cout   << "-->" << zip_filename << "<---"  << "-->" << godzina << "<---"
    //   << endl;
    string message = "You are going to create the tar  archive file \n"
                     "containing all the calibration files and lookup_table files found in directories:\n\t" +
            gpath.calibration + "*  \n\t" +
            gpath.mbs_settings + "* \n\n" +
            "The file will be placed in directory:  " + gpath.spy_dir +
            "\nand called \n\t"  + zip_filename  + "\n\nDo you want this? \n\n";
    int odp = QMessageBox::information ( this,
                                         "Archiving the essential files",
                                         QString ( message.c_str() ),
                                         "Yes",
                                         "No",
                                         "Cancel",
                                         1 ); // default is No
    switch ( odp )
    {
    default:
        break;
    case 0:   // Yes
        string command_line = "cd " + gpath.spy_dir + "\n "
                // + "pwd \n"
                +	"tar -czvf "  + zip_filename  + "   " +
                gpath.calibration + "*  " +
                gpath.mbs_settings + "*" ;

        cout << "command was: " << command_line << endl;
        int resp = system ( command_line.c_str() );
        if(resp < 0)
        {
            cerr <<"something went wrong, while making a archiving command" << endl;
        }

        message = "The file \n   " + zip_filename + " \nhas been saved in a directory\n   " + gpath.spy_dir ;
        QMessageBox::information ( this,
                                   "Archiving the essential files",
                                   QString ( message.c_str() ),
                                   QMessageBox::Ok,
                                   QMessageBox::NoButton);
    }
}
//********************************************************************************************
//********************************************************************************************
void appl_form::set_visibility_of_spy_options_dialogs()
{
    //    cout << "appl_form::" << __func__ << endl;
    // initially - everything? NO

    ui-> actionMatching_the_MBS_subevents->setVisible (0);
    ui->actionSelect_a_type_of_the_Doppler_correction_algorithm->setVisible (1);
    ui->addback_dlg_Action->setVisible (0);
    ui->actionInteresting_region_of_Z_and_A_Q->setVisible (0);
    ui->actionGalileo_prefered_ranges_of_spectra->setVisible (1);
    ui->active_stopper_munich_Action->setVisible (0);
    ui->active_stopper_optionsAction->setVisible (0);
    ui->actionLYCCA_settings->setVisible (0);
    ui->actionDefinition_of_Good_Germanium_detectors_data->setVisible (1);
    ui->actionDefinition_of_Good_CATE_data->setVisible (0);
    ui->actionDefinition_of_Good_Hector_BaF_data->setVisible (0);
    ui->actionDefinition_of_good_Miniball_data->setVisible (0);
    ui->actionGalileo_signals_enabling->setVisible (0);

    // read from file
    string pat_name = gpath.options + "visible.options" ;
    // reading from disk
    ifstream file ( pat_name.c_str() );
    if ( !file ) return;
    //cout << __func__ << "   " << __LINE__ << endl;
    string action_name;
    bool stan = false;
    while(file)
    {
        file >> action_name >> stan ;
        //        cout << action_name << " stan = " << stan << endl;
        if(!file) break;

        //        cout << "Searching action called " << action_name << endl;
        for(int i = 0 ; i < ui->menuSpy_optioons->actions().count() ; ++i)
        {
            //            cout << i << ") is it Menu Bar action called"
            //                 << ui->menuSpy_optioons->actions().at(i)->objectName().toStdString()
            //                 << endl;

            if(ui->menuSpy_optioons->actions().at(i)->objectName().toStdString()  == action_name)
            {
                ui->menuSpy_optioons->actions().at(i)->setVisible(stan);

                //                cout << "Window "
                //                     << ui->menuSpy_optioons->actions().at(i)->text().toStdString()
                //                     << " visible? = "
                //                     << stan << endl;
            }
        }

    }
    return;
}
//*********************************************************************************************
void appl_form::read_last_spectra_selection_after_crash()
{
    string command_file = gpath.commands +
            "spectra_currently_watched.names";
    ifstream plik ( command_file.c_str() );
    if ( !plik )
    {
        //cout << "problem in openning the file: " << command_file << endl;
    }
    string name;
    while(         plik >> name)
    {
        last_spectra_selection.push_back(name);
    }

    //    for(auto x : last_spectra_selection)
    //    {
    //        cout << x << endl;
    //    }
}


//*********************************************************************************************
void appl_form::zeroing_really_on_disk ( string name )
{
    if ( name.empty() ) return;
    if ( name[0] == '@' ) return; // overplot and sum spectra

    std::size_t found = name.find ( ".spc" );
    if ( found != std::string::npos )   // if it is 1D spectrum
    {
        //cout << "spectrum_1D::zero_spectrum()   - zeroing 1D spectrum " << name << endl;
        fstream plik ( ( gpath.spectra + name ).c_str(), ios::binary | ios_base::in | ios_base::out );
        if ( !plik )
        {
            cout << "Can't open file " << ( gpath.spectra + name ) << " for zeroing "<< endl;
            return;
        }
        plik.seekg ( 0 );
        // specif  - definicja

        double bins = -1;
        double left_edge = -1;
        double right_edge = +1;

        plik.read ( ( char* ) &bins, sizeof ( bins ) );
        plik.read ( ( char* ) &left_edge, sizeof ( left_edge ) );
        plik.read ( ( char* ) &right_edge, sizeof ( right_edge ) );

        int binint= static_cast<int> ( bins );
        // positioning for writing from this point
        long pos = plik.tellg();
        plik.seekp ( pos );

        int *table = new int[binint] ;  //
        memset ( table, 0, sizeof ( int ) * binint ); // zeroing memory
        //         for ( int i = 0 ; i < binint ; i++ ) table[i] = 0 ;
        plik.write ( ( char* ) table, sizeof ( int ) * binint ); // just bin zeros
        plik.close() ;
        delete [] table;

        map_of_zeroing[name] = time ( nullptr ) ; // now


    }

    // 2D spectrum ========================================================
    found = name.find ( ".mat" );
    if ( found != std::string::npos )
    {
        //cout << "zeroing 2D spectrum " << name << endl;
        fstream plik ( ( gpath.spectra + name ).c_str(), ios::binary | ios_base::in | ios_base::out );
        if ( !plik )
        {
            cout << "Can't open file " << ( gpath.spectra + name ) << " for zeroing "<< endl;
            return;
        }

        plik.seekg ( 0 );
        // specif  - definicja

        double bins = -1;
        double left_edge = -1;
        double right_edge = +1;

        plik.read ( ( char* ) &bins, sizeof ( bins ) );
        plik.read ( ( char* ) &left_edge, sizeof ( left_edge ) );
        plik.read ( ( char* ) &right_edge, sizeof ( right_edge ) );

        double bins_y = -1;
        double left_edge_y = -1;
        double right_edge_y = +1;

        plik.read ( ( char* ) &bins_y, sizeof ( bins_y ) );
        plik.read ( ( char* ) &left_edge_y, sizeof ( left_edge_y ) );
        plik.read ( ( char* ) &right_edge_y, sizeof ( right_edge_y ) );

        int binint= static_cast<int> ( bins );

        int factor = sizeof(short);  // if binint is negative, so the cell is not short, but int
        if ( binint < 0 ) {
            binint *= -1;
            factor = sizeof(int);
        }
        int binint_y= static_cast<int> ( bins_y );

        // positioning
        long pos = plik.tellg();
        plik.seekp ( pos );

        int bytes = binint * binint_y* factor;
        char *table = new char[bytes] ;  //
        memset ( table, 0, bytes); // zeroing memory
        //for ( int i = 0 ; i < binint ; i++ ) table[i] = i ;
        plik.write ( table, bytes ); // just bin zeros
        plik.close() ;
        delete [] table;
        map_of_zeroing[name] = time ( nullptr ) ; // now
    } // endl of 2D
}
//*********************************************************************************************
bool appl_form::is_spy_running()
{
    return ( ui->actionZero_ing_all_currently_displayed_spectra->isEnabled() );
}
//***************************************************************************************
//**************************************************************************************
void appl_form::restore_times_of_zeroing_from_disk()
{
    //     cout << "F. restore_times_of_zeroing_from_disk() " << ((int)time(0)) << endl;
    map_of_zeroing.clear();
    ifstream plik ( gpath.spectra + "zeroing_times.bin", ios::binary );
    if ( !plik )
    {
        cout << "Can't open file  for restoring zeroing times " << endl;
        return;
    }

    string nazwa_wid;
    time_t when = 0 ;
    const int max_length_of_name = 200;

    while ( plik )   // for(unsigned int i = 0 ;/* i < map_of_zeroing.size()*/ ; i++)
    {
        char tablica[max_length_of_name] = { 0 } ;  // name can be max for ex. 200 bytes long
        plik.read ( ( char* ) tablica, sizeof ( tablica ) );
        nazwa_wid = tablica;

        plik.read ( ( char* ) &when, sizeof ( when ) );
        if ( !plik )
            break;
        map_of_zeroing[nazwa_wid] = when ;
    }
    //  cout << "Size of zeroing map is " << map_of_zeroing.size() << endl;
}
//***************************************************************************************
void appl_form::save_times_of_zeroing_on_disk()
{
    //cout << "F. save_times_of_zeroing_on_disk() " << endl;
    ofstream plik ( gpath.spectra + "zeroing_times.bin", ios_base::binary );
    if ( !plik )
    {
        cout << "Can't open file  for storing zeroing times " << endl;
        return;
    }

    string nazwa_wid;
    time_t when = 0 ;
    const unsigned int max_length_of_name = 200;

    map<string, time_t >::iterator it =
            map_of_zeroing.begin();

    for ( ; it != map_of_zeroing.end() ; it++ )
    {
        nazwa_wid = it->first ;
        if ( nazwa_wid.size() > max_length_of_name )
            continue;

        char tablica[max_length_of_name] = { 0 } ;  // name can be max for ex. 200 bytes long
        nazwa_wid.copy ( tablica, nazwa_wid.size() );
        plik.write ( ( char* ) tablica, sizeof ( tablica ) );

        when = it->second ;
        plik.write ( ( char* ) &when, sizeof ( when ) );
        if ( !plik )
            break;
    }
}
//****************************************************************************************
void appl_form::on_Combo_app_group_activated ( const QString & groupName )
{
    ui->Combo_app_group->setEnabled ( false );
    QApplication::setOverrideCursor ( QCursor ( Qt::WaitCursor ) );

    // remove all previous
    remove_existing_spectra_windows();

    QString gstring =  ui->Combo_app_group->currentText() ;
    gstring = groupName + ".group"  ;
    //  cout << "Reading the contents of " << gstring << endl ;
    string path_groupname = gpath.groups + gstring.toStdString() ;
    ifstream plik ( path_groupname.c_str() );
    char linia[500] = ""  ;

    int rows = 0;
    int columns = 0;
    int how_many_spec = 0;
    while ( plik )
    {
        plik.getline ( linia, sizeof ( linia ) ) ;
        if ( linia[0] == '\0' )
            break ;
        QString nazwa = linia ;
        //        cout << "Trying to open ***" << nazwa.toStdString() << "***" << endl ;
        // spectrum_widget * s = new spectrum_widget(ws, nazwa );


        if(nazwa.toStdString() == ROWS_COLUMNS_KEYWORD){
            plik >> rows >> columns ;
            // cout << "wczytane row-columns" << endl;
            continue;
        }
        QString pname = gpath.spectra.c_str() + nazwa ;
        QFile fff ( pname ) ;
        if ( !fff.exists() )
            continue ;

        Tplate_spectrum * s = nullptr ;
        // looking for an extension
        if ( nazwa.contains ( ".spc", Qt::CaseInsensitive ) )    // <-- false=case insensitife
        {
            //cout << "you selected  a spectrum .asc" << endl;
            s = new spectrum_1D ( area, nazwa );
        }
        if ( nazwa.contains ( ".mat", Qt::CaseInsensitive ) )
        {
            //cout << " you seleted  a matrix (.mat)" << endl;
            s = ( Tplate_spectrum * ) ( new spectrum_2D ( area, nazwa ) );
        }



        if ( s )
        {
            //            QMdiSubWindow *subWindow =
            area->addSubWindow ( s, Qt::Window );
            // now conect all spectra
            connect (
                        s, SIGNAL ( spec_update_scrolls_and_sliders() ),
                        this, SLOT ( appl_update_scrolls_and_sliders() )
                        );

            //cout << "before connecting in selected combo groups" << endl ;
            connect (
                        s, SIGNAL ( enable_selected_actions ( int ) ),
                        this, SLOT ( enable_some_actions ( int ) )
                        );

            // the name is taken form t
            s->set_the_name ( nazwa );
            s->read_in_file ( nazwa.toStdString().c_str(), true );   // true <- this is first time, full, new markers etc.);

            s->showNormal();
            ++how_many_spec;
            //cout << "fileOpen, if(s) true, " << endl ;
        }
        else
        {
            cerr << "creation of 2Dspectrum failed" << endl;

        }

    }

    if(rows == 0 && columns == 0){
        on_actionTile_triggered(); // ws->tileSubWindows() ;
    }else {
        // cout << "Pokazanie rows = " << rows << ", columns = " << columns << endl;
        make_Custom_tiling(how_many_spec, rows, columns);
    }



    save_names_of_spectra_currently_watched();

    QApplication::restoreOverrideCursor();
    //     COTO;
    ui->Combo_app_group->setEnabled ( true );
    should_sliders_be_enabled();

}
//***************************************************************************
void appl_form::check_status_of_spy()
{
    string command_file = gpath.commands + "spy_works.command";

    ifstream plik ( command_file.c_str() );
    if ( !plik )
    {
        ui->spy_go_action->setEnabled ( false );
        ui->spy_pause_action->setEnabled ( false );
        ui->actionSpy_Update_pause_and_run_again->setEnabled ( false );
        ui->actionSpy_update_quickly_without_saving_spectra->setEnabled ( false );
        ui->actionSpy_Finish_quit_the_spy->setEnabled ( false );

        ui->actionZero_ing_all_currently_displayed_spectra->setEnabled ( false );
        ui->actionZero_ing_a_contents_of_the_current_spectrum->setEnabled ( false );
        ui->actionZero_ing_all_displayed_spectra_no_questions_using_shorcut_Ctrl_Alt_Z->setEnabled ( false );
        //zero_everything_Action->setEnabled ( false );
    }
    else
    {
        // check more precisely
        string str;
        plik >> str ;
        if ( str == "paused" )
        {
            ui->spy_go_action->setEnabled ( true );
            ui->spy_pause_action->setEnabled ( false );
            ui->actionSpy_Update_pause_and_run_again->setEnabled ( false );
            ui->actionSpy_update_quickly_without_saving_spectra->setEnabled ( false );
            ui->actionSpy_Finish_quit_the_spy->setEnabled ( false );
        }
        else if ( str == "running" )
        {
            ui->spy_go_action->setEnabled ( false );
            ui->spy_pause_action->setEnabled ( true );
            ui->actionSpy_Update_pause_and_run_again->setEnabled ( true );
            ui->actionSpy_update_quickly_without_saving_spectra->setEnabled ( true );
            ui->actionSpy_Finish_quit_the_spy->setEnabled ( true );
        }
        else // nobody knows
        {
            ui->spy_go_action->setEnabled ( true );
            ui->spy_pause_action->setEnabled ( true );
            ui->actionSpy_Update_pause_and_run_again->setEnabled ( false );
            ui->actionSpy_update_quickly_without_saving_spectra ->setEnabled ( false );

        }
        ui->actionZero_ing_all_currently_displayed_spectra->setEnabled ( true );
        ui->actionZero_ing_a_contents_of_the_current_spectrum->setEnabled ( true );
        ui->actionZero_ing_all_displayed_spectra_no_questions_using_shorcut_Ctrl_Alt_Z->setEnabled ( true );
        // 		ui->zero_everything_Action->setEnabled ( true );
    }

    plik.close();

    // ------------------- info
    // information about the current nr of events
    string info_file = gpath.commands + "spy_events.info";
    ifstream plikinfo ( info_file.c_str() );
    string info ;
    time_t info_time = 0 ;
    if ( plikinfo )
    {
        getline ( plikinfo, info );
        plikinfo >> info_time ;
        // cout << info << ",   seconds read " << info_time << endl;

    }
    if ( plikinfo )
    {
        ui->lineEdit_spy_info->setText ( info.c_str() );
    }
    else
        ui->lineEdit_spy_info->setText ( " --- " );


    plikinfo.close();

    if ( time ( nullptr ) - info_time > 60 )
    {
        // this is too old info ?
        //    string last_message = "UNKNOWN STATE OF SPY (Last message: \"" + info  +"\")";
        string last_message = "!!!Unknown state of spy!!! (Last message was: \"" + info  + "\")";
        ui->lineEdit_spy_info->setText ( last_message.c_str() );
        ui->spy_go_action->setEnabled ( false );
        ui->spy_pause_action->setEnabled ( false );
        ui->actionSpy_Update_pause_and_run_again->setEnabled ( false );
        ui->actionSpy_update_quickly_without_saving_spectra ->setEnabled ( false );
        ui->actionSpy_Finish_quit_the_spy->setEnabled ( false );

        ui->actionZero_ing_all_currently_displayed_spectra->setEnabled ( false );
        ui->actionZero_ing_a_contents_of_the_current_spectrum->setEnabled ( false );
        ui->actionZero_ing_all_displayed_spectra_no_questions_using_shorcut_Ctrl_Alt_Z->setEnabled ( false );
    }

    // set_current_undo_text_in_menu();
}
//**************************************************************************************
void appl_form::on_actionSpy_Update_pause_and_run_again_triggered()
{
    string command_file = gpath.commands ;

    //  command_file += "spy_pause.command";
    command_file += "spy_update.command";

    ofstream plik ( command_file.c_str() );
    plik.close();

    //  // wait some seconds
    //  sleep(3);
    //  command_file = gpath.commands ;
    //  command_file += "spy_continue.command";
    //  plik.open ( command_file.c_str() );
    //  plik.close();

    spy_works_timer->start ( 1 * 1000 ) ;
    ui->actionSpy_Update_pause_and_run_again->setEnabled ( false );
    ui->actionSpy_update_quickly_without_saving_spectra ->setEnabled ( false );
    ui->spy_pause_action->setEnabled ( false );
}
//****************************************************************************
void appl_form::check_synchro_of_spy()     // kind of watchdog
{
    string command_file = gpath.commands + "synchronisation_lost.txt";
    ifstream plik ( command_file.c_str() );
    if ( !plik )
    {
        return;  // OK situation
    }


    if ( important_dlg_ptr->may_I_alarm_now() )
    {
        static bool flag_alarm_on_screen = false ;

        if ( flag_alarm_on_screen )
        {
            important_dlg_ptr->show() ;
            important_dlg_ptr->raise() ;
            return ;  // one is eough
        }

        flag_alarm_on_screen = 1 ;
        string smessage;
        string wyraz ;

        string tytul ;
        getline ( plik, tytul );

        while ( getline ( plik, wyraz ) )
        {
            smessage += wyraz ;
            smessage += "\n" ;
            //cout << "Read:" << wyraz  << " so far " << smessage << endl;
        }

        plik.close();

        // call the dialog
        important_dlg_ptr->set_header ( tytul.c_str() );
        important_dlg_ptr->set_parameter ( smessage.c_str(), command_file.c_str(), &flag_alarm_on_screen );
        important_dlg_ptr->show() ;
        important_dlg_ptr->raise() ;   // to the top

    }
    //remove file is in the dialog

}
//************************************************************************************************************
/// This is a fuction which recalculates the image of scrollers. It does not change the picture
/// what is made by other functions (scroll moved, scroll next line, etc.
/// after these previous functions - this one is changing the position of the scroller itself.

void appl_form::appl_update_scrolls_and_sliders()
{
    //    cout << "++++++++++++ F. appl_form::appl_update_scrolls_and_sliders() " << endl;
    //    //------------------------------------------------
    static time_t last = 0 ;
    // to prevent updating more than every second while opening many files
    time_t now =  time(nullptr) ;

    if(now - last == 0) return ;  // the same second
    last  = now ;
    //--------------------------------------------------
    static int busy = 0 ;
    //    if(busy){
    //        for(int i = 0 ; i < 1000 ; i++) sin(sin(i)) ;
    //        return;
    //    }

    busy++;

    if ( !should_sliders_be_enabled() ) return ;  // no need for empty screen

    //cout << "=== appl_update scrolls - begins " << endl ;

    // at first ask the current spectrum for its display parameters
    /*-------*/
    //  COTO ;


    if ( ! area->currentSubWindow() ) return ;   // this is needed

    Tplate_spectrum * doc = ( Tplate_spectrum * ) ( area-> currentSubWindow()->widget() );
    if ( doc
         &&
         flag_reaction_for_scroll_slider_event == false
         )
    {

        //cout << "flag_reaction_for_scroll_slider_event..  Trying to react ..." << endl ;

        //ScrollBar_poziomy_widma->setVisible(true);
        // we ask for parameters of window
        typ_x min_x = 0 ;
        typ_x max_x = 4096 ;
        typ_x min_yy = 0 ;
        typ_x max_yy = 100 ;

        //---------------------------------------------
        spectrum_descr sd;
        doc->give_parameters ( &min_x, &max_yy, &max_x, &min_yy,  &sd );
        typ_x spectrum_beg_x  = sd.beg ;
        typ_x spectrum_end_x  = sd.end ;
        typ_x spectrum_beg_y  = sd.beg_y ;
        typ_x spectrum_end_y  = sd.end_y ;


        //             cout << "--------------(f. update),  after give parameters min_x=" << min_x
        //                 << " max_x " << max_x
        //                 <<  " spectrum_beg_x " << spectrum_beg_x
        //                 << " spectrum_end_x " << spectrum_end_x << endl;


        //---------------------------------
        // to enable working with spectra 0.1 - 0.7 (root spectra)
        // we make such a trick - multiplying by mnoznik.
        // later results of scrollers will be divided


        min_x *= scrollbar_poziomy_mnoznik ;   // const defined in swiat.h  = 1000
        max_x *= scrollbar_poziomy_mnoznik ;
        spectrum_beg_x *= scrollbar_poziomy_mnoznik ;
        spectrum_end_x *= scrollbar_poziomy_mnoznik ;

        //        cout << "(f. update),  after monoznik  " << scrollbar_poziomy_mnoznik << "    ,   min_x=" << min_x
        //                    << " max_x " << max_x
        //                    <<  " spectrum_beg " << spectrum_beg_x
        //                    << " spectrum_end " << spectrum_end_x
        //                    << endl;


        //==================================================================
        // then update horizontal scrollbar---------------H SCR-------------
        ui->ScrollBar_poziomy_widma->setEnabled ( true );
        //        COTO;
        //Sets the current scroll bar value. See the "value" property for details.
        //cout << "Update sliders, scroll poz" << endl ;

        ui->ScrollBar_poziomy_widma->setPageStep ( ( int ) ( max_x - min_x ) ) ;
        //cout << "Update sliders, scroll poz" << endl ;
        ui->ScrollBar_poziomy_widma->setSingleStep ( ( int ) ( max_x - min_x ) / 10 ) ;
        //        COTO;

        //              cout << "Update sliders, setRange("  << spectrum_beg
        //            <<  ", "  << (spectrum_end - (max_x - min_x))
        //            << " becse end = " << spectrum_end
        //            << " (max_x - min_x) = "  << (max_x - min_x)
        //            << endl ;
        //
        //        COTO;
        typ_x till = spectrum_end_x - ( max_x - min_x ) ;
        if ( till == spectrum_beg_x )
            till = till + 1;
        ui->ScrollBar_poziomy_widma->setRange ( ( int ) spectrum_beg_x, ( int ) till ) ;
        //                cout
        //                    << "Update sliders, scrolls range beg: " << spectrum_beg_x << " till = "
        //                    << till
        //                    <<  endl ;
        //
        ui->ScrollBar_poziomy_widma->setValue ( ( int ) min_x );   // it seems that it should be the last of 3 above...
        //cout << "Update sliders, scrolls 5" << endl ;

        /*---
          cout << "        w appl_update ["
            << min_x
            << "-"
            << max_x
            << "]"
            << " page step = "
            //<< max_x - min_x
            << (ui->ScrollBar_poziomy_widma->pageStep ( ) )
            << endl ;
          ---*/

        //---------


        //=======================================================
        // then update vertical scrollbar ---------------------- V SCR ----------
        //=======================================================

        //         int cur_max = doc->giveCurrentMaxCounts() ;
        //         cout << "cur_max = " << cur_max << endl;

        //             cout << "Update vertical scrollbar value ="
        //                 << (cur_max - (max_yy - min_yy))
        //                 << "     czyli cur_max - (max_yy - min_yy), gdzie "
        //                 << " "
        //                 << cur_max
        //                 << " - ("
        //                 << max_yy
        //                 << " - "
        //                 << min_yy << ") "
        //                 << endl ;


        // this below is impotrtant for 2D spectra
        //---------------------------------
        // to enable working with spectra 0.1 - 0.7 (root spectra)
        // we make such a trick - multiplying by mnoznik.
        // later results of scrollers will be divided


        // sometimes in 1D spectra we can view higher than maximal peak
        // so we change
        if ( spectrum_end_y < max_yy )
        {
            spectrum_end_y = max_yy;
        }
        min_yy *= scrollbar_poziomy_mnoznik ;   // const defined in swiat.h  = 1000
        max_yy *= scrollbar_poziomy_mnoznik ;
        spectrum_beg_y *= scrollbar_poziomy_mnoznik ;
        spectrum_end_y *= scrollbar_poziomy_mnoznik ;

        //        COTO;
        //         cout << "Po mnozniku  FULL  spectrum_beg_y = " << spectrum_beg_y << ", spectrum_end_y= " << spectrum_end_y
        //              << ",  while visible is (max_yy - min_yy)= " << max_yy << " - " << min_yy << " = "  <<  (max_yy - min_yy)
        //              << endl ;

        // when we see on the picture higher than the highest peak - ther is on slider
        if (
                max_yy <= spectrum_end_y
                &&
                min_yy >= spectrum_beg_y
                )
        {
            typ_x visible_part =  max_yy - min_yy ;
            typ_x till2 = - ( spectrum_beg_y ) - ( visible_part ) ;

            // NOTE:
            // range had to ber from [ bottom of the spectrum(0)  ]
            //                            to     [the top of the spectrum - (visible part)]
            // how long is the slider - we define with the    setPageStep()

            // but now, here it is upside down (this scroller is vertical)


            ui->ScrollBar_pionowy_widma->setRange ( ( int ) - spectrum_end_y, ( int ) ( till2 ) ) ;
            //             cout << "Update sliders, scrolls 7, range " << -spectrum_end_y << " till " << till  << endl ;

            //
            ui->ScrollBar_pionowy_widma->setPageStep ( ( int ) visible_part ) ;
            //             cout << "Update sliders, scrolls 8" << endl ;
            //ui->ScrollBar_pionowy_widma->setValue (min_yy);

            //             cout << "In the new image of scrollbar   value set now is " << - max_yy << endl ;
            ui->ScrollBar_pionowy_widma->setValue ( ( int )   - max_yy );
            //             cout << "Update sliders, scrolls 9" << endl ;


            //----
            //             cout
            //                 << "*** update scrv  range 0-" << cur_max
            //                 << " page step =" << (max_yy - min_yy)
            //                 << " value =" << min_yy
            //                 << endl ;
            //--------
        }
        else
        {
            // picture is bigger - higher  than specturm
            ui->ScrollBar_pionowy_widma->setRange ( 0, 0 ) ;
            ui->ScrollBar_pionowy_widma->setValue ( 0 );
        }
        //        COTO;
    }
    else
    {
        // making all slider disabled
    }
    //    COTO;
    /*----------*/
    //     cout << "++++++++++++++++++++++++++++++++ appl_update scrolls - ends " << endl ;
    flag_reaction_for_scroll_slider_event = false;
    //    COTO;
    // set_current_undo_text_in_menu();  ???????????????

    busy--;
}
//***********************************************************************
#ifdef NIGDY
void appl_form::ScrollBar_poziomy_widma_nextPage()
{

    //    COTO;
    Tplate_spectrum * doc = ( Tplate_spectrum * ) ( ws-> currentSubWindow()->widget() );
    cout << "ScrollBar_poziomy_widma_nextPage  " << endl ;
    flag_reaction_for_scroll_slider_event = true;
    if ( doc )
        doc->shift_right ( 1 );

    // pozostale tez przesuwam
    if ( flag_act_on_all_spectra )
        apply_to_other_spectra();

    // cout << " handler next Page, po shift right " << endl ;
    set_current_undo_text_in_menu();
}

//***********************************************************************
void appl_form::ScrollBar_poziomy_widma_prevPage()    // handler of the signal
{
#ifdef NIGDY
    cout << "appl_form::ScrollBar_poziomy_widma_prevPage() " << endl;
    //    COTO;
    Tplate_spectrum * doc = ( Tplate_spectrum * ) ( ws-> currentSubWindow()->widget() );
    flag_reaction_for_scroll_slider_event = true;
    if ( doc )
        doc->shift_left ( 1 );
    // pozostale tez przesuwam
    if ( flag_act_on_all_spectra )
        apply_to_other_spectra();
    set_current_undo_text_in_menu();
#endif
}
//***********************************************************************
void appl_form::ScrollBar_poziomy_widma_prevLine()
{
#ifdef NIGDY
    cout << "appl_form::ScrollBar_poziomy_widma_prevLine() " << endl;
    //    COTO;
    Tplate_spectrum * doc = ( Tplate_spectrum * ) ( ws-> currentSubWindow()->widget() );
    flag_reaction_for_scroll_slider_event = true;
    if ( doc )
        doc->shift_left ( 10 );
    // pozostale tez przesuwam
    if ( flag_act_on_all_spectra )
        apply_to_other_spectra();

    //cout << "line left" << endl ;
    set_current_undo_text_in_menu();
#endif
}
//***********************************************************************
void appl_form::ScrollBar_poziomy_widma_nextLine()
{
#ifdef NIGDY
    cout << "appl_form::ScrollBar_poziomy_widma_nextLine() " << endl;
    //    COTO;
    Tplate_spectrum * doc = ( Tplate_spectrum * ) ( ws-> currentSubWindow()->widget() );
    flag_reaction_for_scroll_slider_event = true;
    if ( doc )
        doc->shift_right ( 10 );
    // pozostale tez przesuwam
    if ( flag_act_on_all_spectra )
        apply_to_other_spectra();

    //   cout << "line Right" << endl ;
    set_current_undo_text_in_menu();
#endif
}
//***********************************************************************
void appl_form::ScrollBar_pionowy_widma_sliderMoved ( int value_bottom )
{

}
#endif
//************************************************************************
//************************************************************************
// Slideres and scrollbars
//************************************************************************
void appl_form::on_ScrollBar_poziomy_widma_sliderMoved ( int left_edge )
{
    static int working_now = 0 ;
    if(working_now > 0)
    {
        //cout << "rejected scroll horiz = level = " << working_now << endl;
        return;
    }
    working_now++;
    // parametr zmienia sie od 0 4096 (w obie strony)
    Tplate_spectrum * doc = ( Tplate_spectrum * ) ( area-> currentSubWindow()->widget() );
    flag_reaction_for_scroll_slider_event = true;
    if ( doc )
        doc->  scrollbar_horizontal_moved ( left_edge );
    /*--------*/

    // pozostale tez przesuwam
    if ( flag_act_on_all_spectra )
        apply_to_other_spectra();
    //cout << "poziomy moved, wartosc parametru left edge= " << left_edge << endl ;
    set_current_undo_text_in_menu();
    working_now--;

}
//****************************************************************************************
void appl_form::on_ScrollBar_pionowy_widma_sliderMoved ( int value_bottom )
{
    //     cout << " ScrollBar_pionowy_widma_sliderMoved (  "
    //     << " value_bottom= " << value_bottom << ")"<< endl ;
    //    COTO;
    /*- -*/
    Tplate_spectrum * doc = ( Tplate_spectrum * ) ( area-> currentSubWindow()->widget() );
    flag_reaction_for_scroll_slider_event = true;
    if ( doc )
        doc->   scroller_vertical_moved ( value_bottom );

    // pozostale tez przesuwam
    if ( flag_act_on_all_spectra )
        apply_to_other_spectra();

    set_current_undo_text_in_menu();
}
//****************************************************************************************
void appl_form::on_black_white_action_triggered ( bool checked )
{
    //   QList <QMdiSubWindow*> windows = ws->subWindowList()  ;
    auto windows = area->subWindowList()  ;
    for ( int nr_dok = 0 ; nr_dok < windows.count() ; nr_dok++ )
    {
        //cout << "nr " << nr_dok << " of " << windows.count() << endl;
        Tplate_spectrum * dokument  = ( Tplate_spectrum * ) windows[nr_dok]->widget() ;
        dokument-> black_white_spectrum ( checked );
    }
}
//****************************************************************************************

void appl_form::on_actionExit_triggered()
{
    // cout << "finishing the program in natural way" << endl ;
    exit ( 0 ) ;
}
//****************************************************************************************
void appl_form::on_Yscale_fit_action_triggered()
{
    Tplate_spectrum * doc = static_cast<Tplate_spectrum *> ( area-> currentSubWindow()->widget())  ;
    if ( flag_act_on_all_spectra )
    {
        //apply_to_other_spectra();
        QList <QMdiSubWindow*> windows = area->subWindowList()  ;
        for ( int i = 0 ; i < windows.count()  ; i++ )
        {
            doc  = ( Tplate_spectrum * ) windows[i]->widget() ;
            doc->scale_to_maxY ();
        }
    }
    else  if ( doc )
        doc->scale_to_maxY();  // only one

    set_current_undo_text_in_menu();
}
//****************************************************************************************
void appl_form::on_actionCopy_integration_markers_and_background_markers_to_other_spectra_triggered()
{
    //apply_to_other_spectra();
    QList <QMdiSubWindow*> windows = area->subWindowList()  ;
    if(windows.count() < 1) return;

    Tplate_spectrum * doc_act = ( Tplate_spectrum * ) area-> currentSubWindow()->widget();
    for ( int i = 0 ; i < windows.count()  ; i++ )
    {
        auto doc  = static_cast<Tplate_spectrum *> ( windows[i]->widget()) ;
        if ( doc == doc_act ) continue;

        doc->copy_markers ( doc_act );
    }
    set_current_undo_text_in_menu();
}
//****************************************************************************************
void appl_form::on_actionSetting_times_of_refreshing_triggered()
{
    T4setting_times_dlg  *dlg = new T4setting_times_dlg ;

    dlg->set_times (
                current_options.give_user_refreshing_time_1D(),
                current_options.give_user_refreshing_time_2D(),
                current_options.give_refreshing_time_sum_1D(),
                current_options.give_preference_asking_about_refreshing_big_matrices()
                );

    if ( dlg->exec() == QDialog::Accepted )
    {
        // take the current parameters
        int t1, t2, t3 ;
        bool flag_big_matrices;
        dlg->give_times ( &t1, &t2, &t3, &flag_big_matrices );
        current_options.set_refreshing_time_1D ( t1 );
        current_options.set_refreshing_time_2D ( t2 );
        current_options.set_refreshing_time_sum_1D ( t3 );
        current_options.set_flag_ask_about_refreshing_big_matrices ( flag_big_matrices );


        // change in all existing spectra
        QList <QMdiSubWindow*> windows = area->subWindowList()  ;
        for ( int i = 0 ; i < windows.count()  ; i++ )
        {
            auto doc  = static_cast<Tplate_spectrum *> ( windows[i]->widget()) ;
            if(!doc) continue;
            doc->set_new_refreshing_times ( );
        }

    }
    delete dlg ;
}
//*************************************************************************
void appl_form::on_Integrate_spectrum_action_triggered()
{
    Tplate_spectrum * doc = ( Tplate_spectrum * ) area-> currentSubWindow()->widget();
    bool flag_all = false ;

    auto how_many_spectra_are_on_the_screen = area->subWindowList().count()  ;

    if ( flag_act_on_all_spectra && (how_many_spectra_are_on_the_screen > 1))
    {
        switch ( QMessageBox::information ( this,
                                            " Integration of many spectra ?",
                                            "Currently you are in mode 'work on many spectra'.\nDo you want to integrate all "
                                            "displayed spectra?  \n",
                                            //"( Yes => all, No => only the one selected)",
                                            "ALL displayed spectra",  // QMessageBox::Yes | QMessageBox::Default,
                                            "only one, SELECTED spectrum",   // QMessageBox::No,
                                            "Cancel", 1 ) )
        {
        case 0 : // Yes
            flag_all = true;
            break ;

        case 1:   // No
            flag_all = false;
            break ;

        default:
            return ;
        } // switch
    } // if

    if ( !flag_all )
    {
        if ( doc )
            doc->integrate ( resul_ptr );
    }
    else  // on all
    {
        // apply_to_other_spectra();
        QList <QMdiSubWindow*> windows = area->subWindowList()  ;
        for ( int i = 0 ; i < windows.count()  ; i++ )
        {
            auto doc2  = static_cast<Tplate_spectrum *> ( windows[i]->widget()) ;
            if(!doc2) continue;
            doc2->integrate ( resul_ptr );
        }
    }
    set_current_undo_text_in_menu();
}
//*************************************************************************
void appl_form::updateMenus()
{
    // cout << "slot- updateMenus " << endl;
}
//*************************************************************************
void appl_form::updateActions()   // this is a SLOT called by window manager
{
    //     cout << "slot- updateActions " << endl;
    if ( ! area->currentSubWindow() )
    {
        enable_some_actions ( 0 );
        return;
    }
    // some actions are 1D specific, others 2D specific
    auto sp = dynamic_cast<Tplate_spectrum *> ( area-> currentSubWindow()->widget() );
    if(sp)
        enable_some_actions ( sp->give_dimension() ) ;
}
//*************************************************************************
void appl_form::on_matrix_projection_Action_triggered()
{
    //    COTO;
    Tplate_spectrum * doc = ( Tplate_spectrum * ) area-> currentSubWindow()->widget();
    if ( doc )
    {
        int x_axis = 0 ;
        switch ( QMessageBox::information ( this,
                                            " Projection of the fragment of the selected matrix",
                                            QString (
                                                "!!! NEW: if one polygon is selcted - you can use it here as a gate !!!\n\n"
                                                "You are going to make the projection of the VISIBLE FRAGMENT "
                                                "of currently selected matrix\n"
                                                "%1\n Please specify on which axis"
                                                ).arg ( doc->give_spectrum_name().c_str() ) ,

                                            "Projection on X axis",  // QMessageBox::Yes | QMessageBox::Default,
                                            "Projection on Y axis",   // QMessageBox::No,
                                            "Cancel", 2 ) )
        {
        case 0 :
            x_axis = 1 ;
            break ;

        case 1:
            x_axis = 0 ;
            break ;

        default:
            return ;
        }

        QString name_of_projection = doc->projection ( x_axis ).c_str();   // only one
        string proj_nam =   name_of_projection.toStdString();
        //        cerr << "Proj_nam = " << proj_nam
        //             << "and name_of_projection = " << name_of_projection.toStdString() << endl;

        Tplate_spectrum * s = nullptr ;
        // looking for an extension
        if ( name_of_projection.lastIndexOf( ".spc", -1, Qt::CaseInsensitive ) > -1)
        {
            string info = proj_nam;
            // if name_of_projection contains anything after .spc = this is a comment to display on the picture of the spectrum
            string::size_type pos =  proj_nam.rfind ( ".spc" );
            // remove from the file name everything what is after
            if ( pos != string::npos )
                proj_nam.erase ( pos + 4 ) ;  // 4 characters are in ".spc"
            name_of_projection =   proj_nam.c_str() ;


            // if such a projection already exists on the screen - we should remove it, because anyway
            // the data will be overwrittend - but the info - NOT, so this can be missleading?
            close_document_with_name ( proj_nam );


            //cout << "you selected  a spectrum " << endl;
            s = new spectrum_1D ( area, name_of_projection );
            auto win =  area->addSubWindow ( s, Qt::Window );
            // now conect all spectra
            connect (
                        s, SIGNAL ( spec_update_scrolls_and_sliders() ),
                        this, SLOT ( appl_update_scrolls_and_sliders() )
                        );

            //cout << "before connecting in selected combo groups" << endl ;
            connect (
                        s, SIGNAL ( enable_selected_actions ( int ) ),
                        this, SLOT ( enable_some_actions ( int ) )
                        );

            // the name is taken form t
            s->set_the_name ( name_of_projection );
            s->read_in_file ( name_of_projection.toStdString().c_str(), true );   // true <- this is first time, full, new markers etc.);


            s->showNormal();

            //-----------------------------
            // Now we can put the tag on the spectrum
            // if name_of_projection contains anything after .spc = this is a comment to display on the picture of the spectrum
            pos =  info.rfind ( ".spc" );
            // remove from the file name everything what is after
            if ( pos != string::npos )
                info = info.substr ( pos + 4 ) ;  // 4 characters are in ".spc"

            dynamic_cast<spectrum_1D *> ( s )->nalepka_notice.set_info ( info ) ;
            //cout << "Info = " << info << endl;
            //cout << "fileOpen, if(s) true, " << endl ;

            //
            QPoint p = doc->pos();
            win->setGeometry(p.x() +10, p.y() + 10, 380, 150);

        }
        // ................
    }
}
//*****************************************************************************************
void appl_form::on_lupa_action_triggered()
{
    Tplate_spectrum * doc = ( Tplate_spectrum * ) area-> currentSubWindow()->widget();

    // change cursor

    //QApplication::setOverrideCursor( QCursor(Qt::PointingHandCursor) );

    //cout << "coursor changed ? " << endl ;
    if ( doc )
        doc->lupa_button_pressed();

    set_current_undo_text_in_menu();
}
//*****************************************************************************************
void appl_form::on_show_hide_polygons_action_triggered(bool checked)
{
    flag_show_polygons = checked ;
    Tplate_spectrum * doc = ( Tplate_spectrum * ) area-> currentSubWindow()->widget();
    if ( doc )
        doc->show_polygons ( checked );

    // pozostale tez
    // if(flag_act_on_all_spectra ) apply_to_other_spectra();
    if ( flag_act_on_all_spectra )
    {
        //         //apply_to_other_spectra();
        //         QWidgetList windows = ws->windowList();
        //         doc  = ( spectrum_widget * ) windows.first() ;
        //         while ( doc )
        //         {
        //             doc->show_polygons ( toggle );
        //             doc = ( spectrum_widget * ) windows.next() ;
        //         }


        // change in all existing spectra
        QList <QMdiSubWindow*> windows = area->subWindowList()  ;
        for ( int i = 0 ; i < windows.count()  ; i++ )
        {
            auto doc2  = static_cast<Tplate_spectrum *> ( windows[i]->widget()) ;
            if(!doc2) continue;
            doc2->show_polygons ( checked );
        }



    }
}
//*****************************************************************************************
void appl_form::on_actionShow_full_names_of_polygon_gates_triggered(bool checked)
{
    flag_show_filenames_of_gates = checked ;
    if(area-> currentSubWindow() == nullptr) return;
    //  cout << "fullnames = " << t << endl ;
    Tplate_spectrum * doc = ( Tplate_spectrum * ) area-> currentSubWindow()->widget();
    if ( doc )
        doc->show_disk_names_of_gates ( flag_show_filenames_of_gates );

    apply_to_other_spectra();  // always
}
//*****************************************************************************************
void appl_form::on_about_Action_triggered()
{
    T4about_dlg dlg ;
    dlg.exec() ;
}
//*****************************************************************************************
void appl_form::on_actionTip_of_the_day_triggered()
{
    T4welcome dlg  ;
    dlg.exec();
}
//*****************************************************************************************
void appl_form::on_user_spec_wizard_Action_triggered()
{
#ifdef FINAL
    QMessageBox::information ( this, "EXPERIMENTAL",
                               "This part of software is a UNIQUE, NEW, FEATURE invented by Jurek Grebosz\n\n"
                               "\n\n\nIf you use this software not only to monitor your experiment, \nbut also for the 'physics' data analysis\n"
                               "please do not forget to cite this software in your papers.\n\n"
                               " 'Cracow Package'\n"
                               "J. Grebosz, Comp. Phys. Comm. 176   251 (2007)\n",
                               QMessageBox::Ok | QMessageBox::Default,
                               QMessageBox::NoButton,
                               QMessageBox::NoButton );
#endif // FINAL
    T4manager_user_spectra dlg;
    //    int result =
    dlg.exec(); // everthing is done in the member funcitons

    //    if ( result == QDialog::Accepted )
    //        warning_spy_is_in_action();   // no need here, it is done already inside manager

}
//*******************************************************************************************************
void appl_form::on_actionCondition_Manager_triggered()
{
    T4manager_user_conditions  dlg ;
    //    int result =
    dlg.exec(); // everthing is done in the member funcitons
}
//*******************************************************************************************************
void appl_form::on_actionSelect_a_type_of_the_Doppler_correction_algorithm_triggered()
{
    T4doppler_dlg  dlg;
    dlg.exec(); // everthing is done in the member functions
    warning_spy_is_in_action() ;
}
//*******************************************************************************************************
void appl_form::on_actionDefinition_of_Good_Hector_BaF_data_triggered()
{
    T4good_hector_dlg  dlg;
    dlg.exec() ;
}
//*******************************************************************************************************
void appl_form::on_actionChoose_relevant_dialog_windows_triggered()
{
    // making list of available dialogs
    vector<QAction *> vec =
    {
        ui-> actionMatching_the_MBS_subevents,
        ui->actionSelect_a_type_of_the_Doppler_correction_algorithm,
        ui->addback_dlg_Action,
        ui->actionInteresting_region_of_Z_and_A_Q,
        ui->actionGalileo_prefered_ranges_of_spectra,
        ui->active_stopper_munich_Action,
        ui->active_stopper_optionsAction,
        ui->actionLYCCA_settings,
        ui->actionDefinition_of_Good_Germanium_detectors_data,
        ui->actionDefinition_of_Good_CATE_data,
        ui->actionDefinition_of_Good_Hector_BaF_data,
        ui->actionDefinition_of_good_Miniball_data,
        ui->actionGalileo_signals_enabling,
        ui->actionGood_signals_from_KRATTA_element,
        ui->action_Good_Galileo_Germanium_data,
        ui->actionEuclides_signals_enabling,
        ui->actionNeutronwall_signals_enabling
    };

    T4options_visibility dlg;
    dlg.set_parameters(vec, 2);
    dlg.exec();
}
//*******************************************************************************************************
void appl_form::on_actionSpy_Finish_quit_the_spy_triggered()
{
    int odp = QMessageBox::information ( this,
                                         "Sending the request to the SPY",
                                         QString ( "You are going to finish the work of the SPY program\n"
                                                   "Are you sure \n\n" ),
                                         "Yes",
                                         "No",
                                         "Brutal YES - no saving spectra",
                                         //"Cancel",
                                         1 ); // default is No

    switch ( odp )
    {
    default: // No?
        break;

    case 0:   // Yes
    case 2:  // Yes, brutal
        string command_file = gpath.commands + "spy_finish.command";
        ofstream plik ( command_file.c_str() );
        if ( !plik )
        {
            cout << "problem in openning the file" << command_file << endl;
        }
        if ( odp == 0 )
        {
            plik << "finish" << endl;
        }
        else if ( odp == 2 )
        {
            plik << "brutalfinish" << endl;
        }

        if ( !plik )
        {
            cout << "problem in writing to file" << command_file << endl;
        }

        plik.close();

        // disable the action: continue
        ui->spy_go_action->setEnabled ( false );
        ui->spy_pause_action->setEnabled ( false );
        ui->actionSpy_Update_pause_and_run_again->setEnabled ( false );                     // spy_update_Action
        ui->actionSpy_update_quickly_without_saving_spectra->setEnabled ( false );          // spy_update_quick_Action
        spy_works_timer->start ( 1 * 1000 ) ;
        break;
    }

}
//*************************************************************************************
void appl_form::on_spy_pause_action_triggered()
{
    string command_file = gpath.gates_1D ;
    string::size_type pos = command_file.rfind ( "gates/" );

    command_file.replace ( pos, 6, "commands/spy_pause.command" );


    ofstream plik ( command_file.c_str() );
    if ( !plik )
        cout << "problem in openning the file" << command_file << endl;
    plik << "pause" << endl;
    if ( !plik )
        cout << "problem in writing to file" << command_file << endl;

    plik.close();

    // enable the action: continue
    ui->spy_go_action->setEnabled ( true );
    // disable action: pause
    ui->spy_pause_action->setEnabled ( false );
    ui->actionSpy_Update_pause_and_run_again->setEnabled ( false );                     // spy_update_Action
    ui->actionSpy_update_quickly_without_saving_spectra->setEnabled ( false );          // spy_update_quick_Action
    ui->actionSpy_Finish_quit_the_spy->setEnabled ( false );
    spy_works_timer->start ( 1 * 1000 ) ;
}
//***************************************************************************
void appl_form::on_spy_go_action_triggered()        // void appl_form::spy_continue_analysis()
{
    string command_file = gpath.commands + "spy_continue.command";

    ofstream plik ( command_file.c_str() );
    if ( !plik )
        cout << "problem in openning the file" << command_file << endl;
    plik << "continue" << endl;
    if ( !plik )
        cout << "problem in writing to file" << command_file << endl;

    plik.close();

    // disable the action: continue
    ui->spy_go_action->setEnabled ( false );
    // enable action: pause
    ui->spy_pause_action->setEnabled ( true );
    ui->actionSpy_Update_pause_and_run_again->setEnabled ( true );                     // spy_update_Action
    ui->actionSpy_update_quickly_without_saving_spectra->setEnabled (true );          // spy_update_quick_Action
    spy_works_timer->start ( 1 * 1000 ) ;
}
//*****************************************************************************************************
void appl_form::on_actionZero_ing_all_currently_displayed_spectra_triggered()
{
    QString list_of_spectra= "You are going to erase the contents of "
                             "\nall currently displayed  online SPY spectra \n"
                             "ARE YOU SURE?";
    switch ( QMessageBox::warning ( this, " 'Zero-ing' online spectra, ARE YOU SURE? ",
                                    list_of_spectra,
                                    "Yes",
                                    "No",
                                    "Cancel",
                                    2 ) )  // Default
    {
    default: // No?
        break;
    case 0:  // Yes :
        //            Tplate_spectrum * doc = ( Tplate_spectrum * ) ws-> currentSubWindow()->widget();
        ostringstream robot;

        QList <QMdiSubWindow*> windows = area->subWindowList()  ;
        for ( int i = 0 ; i < windows.count()  ; i++ )
        {
            auto dokument  = static_cast<Tplate_spectrum *> ( windows[i]->widget()) ;
            if(!dokument) continue;

            if ( dokument->isVisible() )
            {
                robot << ( dokument->give_spectrum_name() ) << endl ;
                dokument->zero_spectrum();
                // cout << "Ask to zero " << dokument->caption() << endl;

                dokument-> re_read_counter_after_zeroing ( 15 );
            }
        }

        string command_file = gpath.commands + "spy_spectra_for_zeroing.command";
        ofstream plik ( command_file.c_str() );
        if ( !plik )
            cout << "problem in openning the file" << command_file << endl;
        plik << robot.str() << endl ;
        plik << "this_is_the_end_of_the_list_of_spectra_which_cracow_wants_to_zero" << endl;

        if ( !plik )
            cout << "problem in writing to file" << command_file << endl;
        plik.close();
    }
    send_to_statusbar("Tip: You can \"Zero all visible spectra\" without any questions - by pressing Ctrl+Alt+Z",
                      15000 );
}
//**********************************************************************************************************
void appl_form::on_actionZero_ing_a_contents_of_the_current_spectrum_triggered()
{
    Tplate_spectrum * doc = ( Tplate_spectrum * ) area-> currentSubWindow()->widget();
    if ( doc )
    {
        cout << "Spectrum to zero = " <<  doc->give_spectrum_name() << endl;
        QString list_of_spectra= QString ( "You are going to erase the contents of the"
                                           " online SPY spectrum\n %1\n"
                                           "ARE YOU SURE?" ).arg ( doc->give_spectrum_name().c_str() );

        int odp = QMessageBox::warning ( this, " 'Zero-ing' online spectra, ARE YOU SURE? ",
                                         list_of_spectra,
                                         "Yes",
                                         "No",
                                         "Cancel",
                                         2  // Default
                                         );
        switch ( odp )
        {
        case 0:    // Yes
        {
            string command_file = gpath.commands + "spy_spectra_for_zeroing.command";
            ofstream plik ( command_file.c_str() );
            //      if(!plik) cout << "problem in openning the file" << command_file << endl;

            if ( doc->isVisible() )
            {
                plik << ( doc->give_spectrum_name() ) << endl;
                doc->zero_spectrum();
                doc->re_read_counter_after_zeroing ( 15 );
            }
            plik << "this_is_the_end_of_the_list_of_spectra_which_cracow_wants_to_zero" << endl;
            if ( !plik )
                cout << "problem in writing to file" << command_file << endl;
            plik.close();
        }
        default:
            break;

        }
    }
}
//***************************************************************************************************************
void appl_form::resizeEvent(QResizeEvent *   /*event*/ )
{

    //cout << "tHIS WAS RESIZE EVENT" << endl;
    on_actionTile_triggered();
}
//***************************************************************************************************************
void appl_form::on_actionGUI_options_triggered()
{
    Tgui_options  dlg;
    dlg.set_font_size(general_font_size);
    dlg.background_for_1D_spectra(background_colour_of_1D_spectrum);   //  = QColor(0, 0, 60);
    // read in the current options
    if(dlg.exec() == QDialog::Accepted)
    {
        // cout << "dialog is accepted" << endl;
        // save options on disk
        background_colour_of_1D_spectrum = dlg.give_background_for_1D_spectra();
        general_font_size = dlg.give_font_size();
        QFont sansFont_ogolny ( "Sans",general_font_size);   // was : 9
        QApplication::setFont(sansFont_ogolny);
        ofstream option (gpath.options + "gui_options.options");
        if(option)
        {
            option << "background_colour_of_1D_spectrum  " << background_colour_of_1D_spectrum.red()
                   << "  "  << background_colour_of_1D_spectrum.green()
                   << "  "  << background_colour_of_1D_spectrum.blue() << endl;
            option << "general_font_size   " << general_font_size << endl;
        }

        // repainting spectra.
        QList <QMdiSubWindow*> windows = area->subWindowList()  ;
        for ( int i = 0 ; i < windows.count()  ; i++ )
        {
            auto doc  = ( Tplate_spectrum * ) windows[i]->widget() ;
            if(!doc) continue;
            doc->shift_right ( 0 );   // <-- shifting nothing, but repainting
        }
    }
}
//***************************************************************************************************************
void appl_form::on_actionGalileo_prefered_ranges_of_spectra_triggered()
{
    T4interesting_ranges_galileo dlg;
    dlg.exec() ;
}
//***************************************************************************************************************
void appl_form::on_action_Good_Galileo_Germanium_data_triggered()
{
    T4good_galileo_ge_dlg dlg;
    dlg.exec();
}
//***************************************************************************************************************
void appl_form::on_actionGalileo_signals_enabling_triggered()
{

    string outfname = gpath.calibration + "galil_look_new.txt";
    string fname = gpath.calibration + "galileo_lookup_table_and_calibration.txt";
    string header =
            "//  channel  channel_enable   name_of_the_detector  Alpha_thresholds theta(deg)  phi(deg) domain id";

    common_galileo_signals_dialog(fname, "Galileo germanium detectors",
                                  "color: rgb(0, 255, 234);\nbackground-color: rgb(0, 17, 255);", // blue
                                  "Compton threshold",
                                  false, // second threshold
                                  "",
                                  true, // flag_geometry
                                  false, // no second calibration
                                  "None", "None",  // labels for calibration factors
                                  header,
                                  outfname); // output filename

}
//**************************************************************************************************

void appl_form::on_actionEuclides_signals_enabling_triggered()
{
    string outfname = gpath.calibration + "euclides_look_new.txt";
    string fname = gpath.calibration + "euclides_lookup_table_and_calibration.txt";

    string header =
            "//chanel channel_enable detector_name alpha_low_thrs alpha_up_thrs "
            "theta phi "
            "cal_order_sub0 calibr_factors_sub0 "
            "cal_order_sub1 calibr_factors_sub1\n" ;

    //---------
    common_galileo_signals_dialog(fname,
                                  "Euclides detectors",
                                  "color: rgb(255, 255, 255);\nbackground-color: rgb(180, 0, 180);",
                                  "Alpha low threshold:",
                                  true, // second threshold
                                  "Alpha up threshold:.",
                                  true, // flag_geometry
                                  true, // second calibration needed (dE, E)
                                  "Delta E calibration",
                                  "E calibraion",
                                  header, // header line above data
                                  outfname ); // output filename
}
//**************************************************************************************************
void appl_form::common_galileo_signals_dialog(string input_file,
                                              string label_detector,
                                              string label_color,
                                              string label_first_threshold,
                                              bool flag_second_thr,
                                              string label_second_threshold,
                                              bool flag_geometry,
                                              bool flag_second_calibration,
                                              string first_calibration_txt,
                                              string second_calibration_txt,
                                              string header,
                                              string outfname)
{

    ifstream infile (input_file);
    if(!infile) {
        cerr << "Can't open input file" << input_file<< endl;
        string mess = "Can't open calibration file: " + input_file ;
        QMessageBox::warning ( this,
                               " Can't open file",
                               mess.c_str(),
                               "OK",
                               "",
                               "Cancel",
                               2  // Default
                               );

        return;
    }
    vector<Tfile_line_det_cal_lookup> full_file;
    try
    {
        while(infile)
        {
            // in case of the galileo germanium detectors
            Tfile_line_det_cal_lookup linijka { flag_second_thr, // no second threshold
                        flag_geometry,// geometry present
                        flag_second_calibration} ;
            linijka.read_in(infile);
            // cout << linijka << endl;
            full_file.push_back(linijka);
        }

        infile.close();
        TGalileo_signals_dlg dlg(full_file);
        dlg.setup(label_detector,
                  label_color,
                  label_first_threshold,
                  label_second_threshold,

                  first_calibration_txt,
                  second_calibration_txt
                  );

        bool flag_really_replace = false;

        if(dlg.exec() == QDialog::Accepted)
        {
            // saving all new file
            ofstream sav(outfname, ios::trunc);

            //#define sav cout
            if(!sav) { cerr << "Can't open input file" << outfname<< endl; return;}

            // saving header
            string top {
                "// This is a Lookup table and calibration file for galileo detectors\n"
                "//===================================================================\n"
                "//\n"
                "// Note:\n"
                "//    1. Everything after two slashes (//) till the end of the line is\n"
                "//       treated as a COMMENT\n"
                "//    2. Empty lines, newlines, spaces, tabulators are ignored\n"
                "//    3. Keywords (names of the parameters) can be typed lowercase\n"
                "//       or uppercase, as you wish.\n"
                "//\n"
                "//       Advice: for your own sake, use rather uppercase 'L' instead\n"
                "//       of lowercase 'l'\n"
                "//       (because 'l' is too similar to '1')\n"
                "//    4. If a channel is disabled (0) then we don't read anything else\n"
                "//       after so the empty_?? are just here to make it easier to read\n"
                "//====================================================================\n"
                "// Channel channel_enable name_of_detector  ComptonThres  theta phi cal_order calibr_factors\n"};

            sav << top << header << endl;

            for(auto linia : full_file)
            {
                linia.save_line(sav, this, flag_really_replace);
            }
            sav.close();
            // Here we are if the user said YES, or the re was no problem
            // renaming file into the old name

            // UNCOMMENT THIS LINE BELOW IF YOU REALLY WANT REPLACE THE OLD LUT/CAL FILE
            // if(flag_really_replace) system( ("mv " + outfname + " " + fname).c_str() ); // renaming
        }

    } // end try
    catch(Texcept_read obj)
    {

        string mess = "Error while reading a file: \n" + input_file +
                "\n\nThe error is in a line dedicated to the signal: "
                + obj.channel + " ...,  \nerror while reading item: "
                + obj.item  ;

        QMessageBox::warning ( this,
                               " Error while reading calibration file",
                               mess.c_str(),
                               "OK",
                               "",
                               "Cancel",
                               2  // Default
                               );
        return;
    }
}
//*****************************************************************************************************
void appl_form::on_actionNeutronwall_signals_enabling_triggered()
{
    string outfname = gpath.calibration + "neutronwall_look_new.txt";
    string fname = gpath.calibration + "neutronwall_lookup_table_and_calibration.txt";

    string header =
            "//  channel channel_enable(1)/disable(0)     name_of_the_detector threshold calibration_order calibr_factors";

    //---------
    common_galileo_signals_dialog(fname,
                                  "Neutron detectors",
                                  "color: rgb(255, 255, 0);\nbackground-color: rgb(190, 0, 0);",
                                  "Threshold:",
                                  false , // second threshold
                                  "None",
                                  true, // flag_geometry
                                  false, // second calibration needed (dE, E)
                                  "E calibration",
                                  "None",
                                  header, // header line above data
                                  outfname ); // output filename
}
//**************************************************************************************************

void appl_form::on_actionGood_signals_from_KRATTA_element_triggered()
{
    T4good_kratta_dlg  dlg ;
    dlg.exec() ;
}
//**************************************************************************************************
void appl_form::on_actionSystem_tile_Creation_order_0_triggered()
{
    area->setActivationOrder(QMdiArea::CreationOrder);  // 0

    area->tileSubWindows();
}
//**************************************************************************************************
void appl_form::on_actionSystem_tile_Stacking_order_1_triggered()
{
    area->setActivationOrder(QMdiArea::StackingOrder);  // 1
    area->tileSubWindows();
}
//**************************************************************************************************
void appl_form::on_actionSystemTile_activation_history_order_2_triggered()
{
    // this seam to work, but I do not understand the proprer rules.
    // anyway, this system tile in not good for spectra. When there are 8 on a screen
    // it tiles them not  2x4 but in some akward way
    area->setActivationOrder(QMdiArea::ActivationHistoryOrder);  // 2
    area->tileSubWindows();
}
//***********************************************************************************************************
void appl_form::on_actionCustom_tiling_triggered()
{
    // cout << "my custom tiling " << endl;

    // Defining matrix
    static int vert_rows = 2;
    static int horiz_columns = 8;

    QList <QMdiSubWindow*> windows = area->subWindowList()  ;

    if(windows.empty() ) return;

    int how_many_spec = windows.count();
    [[maybe_unused]] int pion = sqrt(how_many_spec );
    // int poziom = pion;

    auto *dlg = new T4tiling_marix_dlg ;
    dlg->setup(how_many_spec, horiz_columns, vert_rows);
    if(dlg->exec() == QDialog::Accepted)
    {
        // taking proper values
        dlg->take_results(horiz_columns, vert_rows);   // we take by references
        delete dlg ;

    }else {
        delete dlg;
        return;
    }

    make_Custom_tiling(how_many_spec, vert_rows, horiz_columns);

}
//***********************************************************************************************************
void appl_form::make_Custom_tiling(int how_many_spec, int vert_rows, int horiz_columns)
{

    //cout << PRETTY_FUNCTION << endl;
    int poziom = horiz_columns;
    int pion = vert_rows;
    if(how_many_spec == 3)
    {
        pion = 2;
        poziom = 2;
    }

    while (how_many_spec - (pion * poziom) > 0)
    {
        //++poziom;
        ++pion;
    }

    int empty_place = 0; // (pion * poziom) - how_many_spec;

    //difference = abs(how_many_spec - (pion * poziom)) ;


    double vertical_distance = 1.0 *(area->height() - 2) / pion;
    double horizontal_distance = 1.0* (area->width() - 2) / poziom;

    //    vertical_distance -= 1;
    //    horizontal_distance -=1;

    QList <QMdiSubWindow*> windows = area->subWindowList()  ;
    if(windows.empty() ) return;

    int counter = 0 ;
    int extra = empty_place ;
    for ( int y = 0 ; y < pion ; ++y)
    {
        for(int x = 0 ; x < poziom ; ++x, ++counter, --empty_place)
        {
            if(counter >= how_many_spec) break;
            if(y == 1 && x ==0) x+= extra;

            int wys = (empty_place > 0) ? (2* vertical_distance) : vertical_distance;
            windows[counter]->setGeometry(x*horizontal_distance,
                                          vertical_distance * y,
                                          horizontal_distance,
                                          wys
                                          );
            //            cout << "spectrum nr " << counter << ", name =" ;


            //                         cout <<  windows[counter]->windowTitle().toStdString()  ;

            //                 cout << ", x = " << (x*horizontal_distance)
            //                 << ", y = " << (y*vertical_distance) << endl;
        }
        //        windows[i]->update();
    }

}
//***********************************************************************************************************************
void appl_form::on_actionCheck_incrementers_triggered()
{
#if 0

    t4checking_incrementers_dlg  * dlgc = new t4checking_incrementers_dlg;
    dlgc->exec();

    delete dlgc;

#endif



#if 1


    // make a list of all bild-in spectra

    QDir katalog ( gpath.Qspectra() ); // ("spectra") ;

    ifstream dfile(gpath.spectra + "descriptions.txt");
    if(!dfile)
    {
        cout << "error in creating descr_file file" << endl;
    }

    //    cout << "opened file : " << gpath.spectra + "descriptions.txt" << endl;
    vector<string> spectra_names_vector;
    //------------------------------------------------------------

    // open the result text file
    ofstream report(gpath.spy_dir + "list_of_spectra_with_wrong_incrementers.txt");
    if(!report)
    {
        cout << "error in creating report file" << endl;
    }
    bool flag_was_error = false;
    string wyraz;
    string sname;
    [[maybe_unused]] int dim = 0;
    int nr = 0;
    T4incrementer_selector * dlg = new T4incrementer_selector ;

    // loop over every spectrum -------------------------
    cout << "Obecnie wyraz  =" << wyraz << endl;
    while(dfile >> wyraz)
    {
        if(wyraz == "version_with_incrementers") continue;
        sname = wyraz;
        cout << nr<< ") Checking spectrum called: " << sname << endl;
        nr++;
        // check 1D, 2D
        if(sname.substr(sname.size() - 4) == ".spc") dim = 1;
        else if(sname.substr(sname.size() - 4) == ".mat") dim = 2;
        else {
            {
                cout << "error - I cant recognize the dimmension of a spectrum " << sname << endl;
                exit(1);
            }
        }
        // incrementer information starts after second  '{'
        getline(dfile, wyraz, '{');
        getline(dfile, wyraz, '{');

        string inkrem_set;
        getline(dfile, inkrem_set, '}');

        //       cout << "now inkrem_set is " << inkrem_set << endl;

        // skip user defined
        if(sname.substr(0,5) == "user_")
            continue; // ??

        //         cout << "a following wyraz is " << wyraz << endl;

        istringstream s(inkrem_set);
        [[maybe_unused]] bool flag_x = false;

        bool flag_y = false;
        [[maybe_unused]] bool flag_info = false;
        bool flag_error_entry = false;
        ostringstream prepared;

        while(s >> wyraz)
        {
            // analyse the set of incrementers
            // skip "X:"
            if(wyraz == "X:") { flag_x = true; continue;}

            // skip "Y:"
            if(wyraz == "Y:") {flag_y = true; continue;}

            // skip "INFO:"
            if(wyraz == "INFO:") {
                flag_info = true;
                // getline(s, info_txt, "}");
                continue;
            }

            // skip words which do not have '_' inside
            if(wyraz.find("_") == string::npos){
                //strange_words += wyraz + "\n";
                continue;
            }

            // so it may be an incrementer

            // check if such a incrementer exists
            dlg->set_filter(wyraz);
            auto list = dlg->give_filtered_items();

            if(list.size() == 1)
            {
                //                 cout << "Good, this incrementer " << wyraz << " was found as official" << endl;
                // if yes, check another
            }
            else
            {
                // if no, write a report,
                flag_error_entry = true;
                flag_was_error  = true ;
                prepared << "\ninforms that it has an "
                         << (flag_y ? "Y " : "X ")
                         << " incrementer called : \n\t" << wyraz
                         << "\nwhich is not recognized as the official one\n";

                // then filter inteligently and suggest sensible
                // correction in the spy

                //                vector<string> suggestion = suggest_incrementer(dlg, wyraz);
                //                if(suggestion.size())
                //                {
                //                    prepared << string(40, '.') << "\nPerhaps you mean:\n" ;
                //                    for(auto x : suggestion)
                //                        prepared << "\t" << x << "\n";
                //                    prepared << string(40, '.');
                //                }
            }

        } // end scaning inrementers txt
        if (flag_error_entry) {

            report << string(60, '=')
                   << "\n" << nr << " ) Spectrum called " << sname
                   << prepared.str()
                   << string(50, '-')
                   << "\n\nJust to help you to make decision. In a description of incrementers we see: \n" <<  inkrem_set << endl;
        }
    } // end scaning of this spectrum
    delete dlg;

    string info;
    if(flag_was_error)
    {


        //        cout //<< "End of f. " << __PRETTY_FUNCTION__
        info =     "There were some errors. Check the file\n "
                + gpath.spy_dir + "list_of_spectra_with_wrong_incrementers.txt" ;
    }else {
        info= "Everything is  OK, no errors found";
    }

    QMessageBox::information ( this, "Result of checking",
                              QString(info.c_str()),
                               QMessageBox::Ok, QMessageBox::NoButton , QMessageBox::NoButton );

}
//*******************************************************************************************************************
std::vector<string> appl_form::suggest_incrementer(T4incrementer_selector * dlg, string filter)
{
    string wyraz = filter;
    dlg->set_filter(wyraz);
    QStringList list ; // = dlg->give_filtered_items();  // fake, just to have auto


    for(int i =  filter.size() ; i > 0 ; --i)
    {
        wyraz = "*" + filter.substr(0, i) + "*";
        dlg->set_filter(wyraz);
        list = dlg->give_filtered_items();
        //		cout << i << " Checking filter: [" <<wyraz << "] -> gives counts:"
        //			 << list.count() << endl;
        if(list.count() > 0)
            break;
    }


    vector<string> candidates;

    for( auto it = list.begin(); it != list.end(); ++it)
    {
        candidates.push_back( (*it).toStdString());
    }

    //	cout << "Candidates for substitution:\n";
    //	for(auto x : candidates)
    //		cout << x << endl;

    return candidates;

#endif // 0

}


void appl_form::on_actionAuto_scale_to_highest_peaks_toggled(bool stan)
{
    // changing a flag  flag_auto_maximum
    // cout << __PRETTY_FUNCTION__ << " stan =" << stan << endl;

    //     if ( ws->currentSubWindow() == NULL ) return ;

    // flag_auto_maximum = stan ;
    // press it really once more
    // ui->log_scale_Action->setChecked( stan );

    Tplate_spectrum * doc = ( Tplate_spectrum * ) ( area-> currentSubWindow()->widget() );
    if ( doc )
        doc->auto_scale(stan );

    // pozostale tez
    if ( flag_act_on_all_spectra )
    {
        //apply_to_other_spectra();
        QList <QMdiSubWindow*> windows = area->subWindowList()  ;
        for ( int i = 0 ; i < windows.count()  ; i++ )
        {
            doc  = ( Tplate_spectrum * ) windows[i]->widget() ;
            if(!doc) continue;
            doc->auto_scale(stan );
        }
    }

}
