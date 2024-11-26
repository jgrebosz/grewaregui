#include "options.h"
#include <fstream>
#include <iostream>
using namespace std ;
#include "Tfile_helper.h"

options current_options ;
//*******************************************************************
void options::save_options()
{
    ofstream plik("options.cracow");

    plik << "refreshing_time_1D_spectra\t" << refreshing_time_1D_spectra
         << "\nrefreshing_time_2D_spectra\t" << refreshing_time_2D_spectra
         << "\nrefreshing_time_sum_1D_spectra\t" << refreshing_time_sum_1D_spectra
         << "\nsnapshot_program " << snapshot_program
         << "\nflag_ask_about_refreshing_big_matrices "
         << flag_preference_asking_about_refreshing_big_matrices
         << endl;


    plik.close() ;
}
//*********************************************
void options::restore_options()
{
    ifstream plik("options.cracow", ios::binary);
    if(!plik)
        return ;
    try
    {
        refreshing_time_1D_spectra = (int) Nfile_helper::find_in_file(plik, "refreshing_time_1D_spectra");
        refreshing_time_2D_spectra = (int) Nfile_helper::find_in_file(plik, "refreshing_time_2D_spectra");
        refreshing_time_sum_1D_spectra = (int) Nfile_helper::find_in_file(plik, "refreshing_time_sum_1D_spectra");
        Nfile_helper::spot_in_file(plik, "snapshot_program");
        getline(plik, snapshot_program);
        flag_preference_asking_about_refreshing_big_matrices = (bool) Nfile_helper::find_in_file
                (plik, "flag_ask_about_refreshing_big_matrices");

        plik.close() ;
    }
    catch(Tfile_helper_exception &k)
    {
        cout << "Error while reading options file \"options.cracow\" "
             << k.message << endl;
    }
}
//*********************************************
void options::set_refreshing_time_1D(int value)
{
    refreshing_time_1D_spectra = value ;
    save_options();
}

//*********************************************
void options::set_refreshing_time_2D(int value)
{
    refreshing_time_2D_spectra = value  ;
    save_options();
}
//*********************************************
void options::set_refreshing_time_sum_1D(int value)
{
    refreshing_time_sum_1D_spectra  = value ;
    save_options();
}
//*********************************************
void options::set_flag_ask_about_refreshing_big_matrices(bool f)
{
    flag_preference_asking_about_refreshing_big_matrices = f;
    save_options();
}
