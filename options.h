#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <string>
using namespace std;

class options
{
    int refreshing_time_1D_spectra ;
    int refreshing_time_2D_spectra ;
    int refreshing_time_sum_1D_spectra ;
    string snapshot_program ;
    // if we have many spectra to display, we enhace the time period of refreshing
    int how_many_spectra_on_the_screen_now;
    bool flag_preference_asking_about_refreshing_big_matrices;

public:
    options()  // constructor
    {
        refreshing_time_1D_spectra  = 10;
        refreshing_time_2D_spectra  = 30;
        refreshing_time_sum_1D_spectra = 60;
        snapshot_program = "gimp  &" ;
        how_many_spectra_on_the_screen_now = 0 ;
        flag_preference_asking_about_refreshing_big_matrices = false;
        restore_options();
    }
    ~options()  // destructor
    {
        save_options();
    }

    //**********************************************
    void save_options();
    //*********************************************
    void restore_options();

    /**
     * this function makes the refreshing time longer - depending on how many other spectra are on the screen
     * @return
     */
    int give_realistic_refreshing_time_1D()
    {
        return refreshing_time_1D_spectra  + int (how_many_spectra_on_the_screen_now * 0.2);
    }

    /**
     * This function gives the time defined by user. It is used only for showing the parameter
     * on the dialog window
     * @param value
     */
    int give_user_refreshing_time_1D()
    {
        return refreshing_time_1D_spectra;
    }

    int give_realistic_refreshing_time_2D()
    {
        return refreshing_time_2D_spectra + int (how_many_spectra_on_the_screen_now * 0.2);
    }

    int give_user_refreshing_time_2D()
    {
        return refreshing_time_2D_spectra ;
    }

    int give_refreshing_time_sum_1D()
    {
        return refreshing_time_sum_1D_spectra ;
    }
    string give_snapshot_program()
    {
        return snapshot_program ;
    }

    bool give_preference_asking_about_refreshing_big_matrices()
    {
        return flag_preference_asking_about_refreshing_big_matrices ;
    }

    void set_refreshing_time_1D(int value);
    void set_refreshing_time_2D(int value);
    void set_refreshing_time_sum_1D(int value);
    void set_flag_ask_about_refreshing_big_matrices(bool f);

    void set_snapshot(string s)
    {
        snapshot_program = s ;
        save_options();
    }
    void set_how_many_spectra_on_the_screen_now(int n)
    {
        how_many_spectra_on_the_screen_now = n;
    }

};

//extern int flag_draw_spectra_scales;
#endif
