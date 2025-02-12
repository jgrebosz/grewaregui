#ifndef APPL_FORM_H
#define APPL_FORM_H

//#include <QtGui/QMainWindow>
#include <QMainWindow>
#include <QMdiArea>

#include <t4select_spectra.h>
#include <t4results_of_integration.h>
#include <map>
using namespace std;  //  map;

#include <QMainWindow>

class QMdiArea;
#include "tplate_spectrum.h"

namespace Ui
{
class appl_form;
}

// this below will be uncomented when all function will be fully ready
//#define FINAL

class T4important_dlg;
class T4incrementer_selector;


/////////////////////////////////////////////////////////////
class appl_form : public QMainWindow
{
    Q_OBJECT

public:
    appl_form(QWidget *parent = nullptr);
    appl_form(const appl_form&) = delete;
    appl_form& operator=(const appl_form&) = delete;
    ~appl_form();
    time_t  give_time_of_zeroing_spectrum (std::string name);
    //void pusta_funkcja() {}
    void send_to_statusbar(std::string m, int timeout);
    void warning_spy_is_in_action();

    bool is_spy_running() ;
    void restore_times_of_zeroing_from_disk() ;
    void zeroing_really_on_disk (string name)   ;
    void save_times_of_zeroing_on_disk();
    void addSubWindow(Tplate_spectrum * s, Qt::WindowFlags windowFlags) ;
    //    void addSubWindow(Tplate_spectrum * s, Qt::WindowFlags windowFlags = nullptr ) ;
    bool give_flag_draw_spectra_scales() { return flag_draw_spectra_scales ; }

    //    void on_action_fileNew_triggered();
    //    void on_action_fileNew_activated();
    void if_needed_apply_to_other_spectra(){
        if(flag_act_on_all_spectra) apply_to_other_spectra();
    }
    QMdiArea * give_workspace() { return area;}

public slots:

    void on_actionCustom_tiling_triggered();

private slots:
    void on_actionSelecting_the_spectra_triggered();

    void on_actionPath_to_the_spy_directory_triggered();
    void on_actionTile_triggered();
    void on_actionClose_all_spectra_triggered();
    void on_shift_left_action_triggered();
    void on_shift_right_action_triggered();
    void on_act_on_all_spectra_Button_toggled(bool checked);
    void on_act_on_one_spectrum_Button_toggled(bool checked);
    void on_toolButton_select_spectra_clicked();
    void on_full_spectrum_action_triggered();

    void on_expand_spectrum_action_triggered();

    void on_Yscale05_action_triggered();

    void on_Yscale2_action_triggered();

    void on_Action_undo_triggered();
    void on_toolButton_apply_to_others_clicked();

    void on_draw_scale_Action_triggered(bool checked);

    void on_parameters_Action_triggered();

    void on_log_scale_Action_triggered(bool checked);

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_filePrintAction_triggered();

    void on_actionDefine_snapshot_program_to_print_triggered();

    void on_actionCall_wizard_to_edit_the_CURRENT_user_def_spectrum_triggered();

    void on_actionN_tuple_Root_Tree_options_triggered();

    void on_addback_dlg_Action_triggered();

    void on_actionInteresting_region_of_Z_and_A_Q_triggered();

    void on_actionVerbose_mode_show_selected_range_of_events_on_the_screen_triggered();

    void on_actionDefinition_of_good_Miniball_data_triggered();

    void on_actionChange_the_binning_and_range_triggered();

    void on_actionOther_SPY_options_triggered();

    void on_actionZero_ing_all_displayed_spectra_no_questions_using_shorcut_Ctrl_Alt_Z_triggered();

    void on_actionOther_tools_for_matrix_operations_triggered();

    void on_actionOther_tools_for_1D_operations_triggered();

    void on_actionHelp_about_mouse_clicking_triggered();

    void on_petrify_Action_triggered();

    void on_actionZero_ing_ALL_SPECTRA_triggered();

    void on_actionSum_many_2D_spectra_triggered();

    void on_active_stopper_optionsAction_triggered();

    void on_incrementer_managerAction_triggered();

    void on_active_stopper_munich_Action_triggered();

    void on_actionLYCCA_settings_triggered();

    void on_actionSpy_update_quickly_without_saving_spectra_triggered();

    void on_actionAutomatic_calibration_of_germanium_detectors_triggered();

    // void on_actionMake_tar_file_with_calibatrions_and_lookup_tables_triggered();

    void on_Combo_app_group_textActivated(const QString &groupName);
    // void on_Combo_app_group_activated(const QString &groupName);
    void check_status_of_spy();
    void check_synchro_of_spy();
    void enable_some_actions ( int typ );

    void on_actionSpy_Update_pause_and_run_again_triggered();


    void on_black_white_action_triggered(bool checked);

    void on_actionExit_triggered();

    void on_Yscale_fit_action_triggered();

    void on_actionCopy_integration_markers_and_background_markers_to_other_spectra_triggered();



    void on_actionSetting_times_of_refreshing_triggered();

    void on_actionCascade_triggered();

    void on_Integrate_spectrum_action_triggered();

    void updateMenus() ;
    void updateActions() ;
    void on_matrix_projection_Action_triggered();
    void on_actionSpectrum_below_spectrum_triggered();
    void on_lupa_action_triggered();
    void on_show_hide_polygons_action_triggered(bool checked);

    void on_actionShow_full_names_of_polygon_gates_triggered(bool checked);

    void on_about_Action_triggered();

    void on_actionTip_of_the_day_triggered();

    void on_user_spec_wizard_Action_triggered();

    void on_actionCondition_Manager_triggered();

    void on_actionSelect_a_type_of_the_Doppler_correction_algorithm_triggered();

    void on_actionDefinition_of_Good_Germanium_detectors_data_triggered();

    void on_actionDefinition_of_Good_Hector_BaF_data_triggered();

    void on_actionChoose_relevant_dialog_windows_triggered();

    void on_actionSpy_Finish_quit_the_spy_triggered();

    void on_spy_pause_action_triggered();

    void on_spy_go_action_triggered();

    //    void on_actionZero_ing_a_contents_of_the_current_spectrum_triggered();

    void on_actionTile_with_system_function_triggered();

    void on_actionZero_ing_all_currently_displayed_spectra_triggered();

    void on_actionZero_ing_a_contents_of_the_current_spectrum_triggered();

    void resizeEvent ( QResizeEvent * event );
    //    void on_actionShow_full_names_of_polygon_gates_triggered();

    void on_actionGUI_options_triggered();

    void on_actionGalileo_prefered_ranges_of_spectra_triggered();

    void on_action_Good_Galileo_Germanium_data_triggered();

    void on_actionGalileo_signals_enabling_triggered();

    void on_actionEuclides_signals_enabling_triggered();

    void on_actionNeutronwall_signals_enabling_triggered();

    void on_actionGood_signals_from_KRATTA_element_triggered();

    void on_actionSystem_tile_Creation_order_0_triggered();

    void on_actionSystem_tile_Stacking_order_1_triggered();

    void on_actionSystemTile_activation_history_order_2_triggered();

    void on_actionMake_tar_file_with_calibrations_and_lookup_tables_triggered();

    void on_actionCheck_incrementers_triggered();

    void on_actionAuto_scale_to_highest_peaks_toggled(bool stan);


private:
    Ui::appl_form *ui;

protected:

    void remove_existing_spectra_windows();
    void save_names_of_spectra_currently_watched();
    void refresh_combo_groups();
    void set_current_undo_text_in_menu();
    bool should_sliders_be_enabled();
    bool it_is_user_def_spectrum();
    void set_paths();
    void apply_to_other_spectra();

    void close_document_with_name ( string name );
    void adjust_program_to_experiment_type();
    void set_visibility_of_spy_options_dialogs();
    void read_last_spectra_selection_after_crash();

    void make_Custom_tiling(int how_many_spec, int vert_rows, int horiz_columns);
    std::vector<std::string> suggest_incrementer(T4incrementer_selector *dlg, std::string filter);



    void common_galileo_signals_dialog(string input_file,
                                       string label_detector, string label_color,
                                       string label_first_threshold,
                                       bool flag_second_thr,
                                       string label_second_threshold,
                                       bool flag_geometry,
                                       bool flag_second_calibration,
                                       string first_calibration_txt,
                                       string second_calibration_txt,
                                       string header,
                                       string outfname);


    bool flag_draw_spectra_scales;
    map<string, time_t> map_of_zeroing;
    QString printer_call_line;

    T4important_dlg *  important_dlg_ptr;

    QTimer *synchro_check_timer;
    QTimer *spy_works_timer ;
    bool flag_show_filenames_of_gates;
    bool flag_show_polygons;
    T4results_of_integration * resul_ptr;
    bool flag_log_scale = 0;
    bool flag_reaction_for_scroll_slider_event;

    QMdiArea *area;

    bool flag_act_on_all_spectra;
    bool flag_display_previously_displayed_spectra = false;
    vector<string> last_spectra_selection ;

};
/////////////////////////////////////////////////////////////
extern appl_form  *appl_form_ptr;
extern QColor background_colour_of_1D_spectrum;

#endif // APPL_FORM_H
