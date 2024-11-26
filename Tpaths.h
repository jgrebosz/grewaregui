#ifndef TPATHS_H
#define TPATHS_H
#include <string>
using namespace std ;
#include <qstring.h>
#include <cstdlib>

class Tpaths
{
public:
    string spectra ;
    string polygons ;
    string gates_1D ;
    string commands ;
    string groups ;
    string mbs_settings ;
    string spy_dir ;
    string options ;
    string user_def_spectra ;
    string autom ;
    string calibration ;
    string conditions ;
    string my_binnings;
    string incrementers;

    Tpaths()
    {
        spy_dir = "./" ;
    }
    // checks for ending slash
    void check_for_slash_in_spy_dir() ;

    bool read_from_disk() ;
    void save_to_disk() ;
    QString Qspectra() { return spectra.c_str(); }
    QString Qpolygons() { return polygons.c_str(); }
    QString Qgates_1D() { return gates_1D.c_str(); }
    QString Qcommands() { return commands.c_str(); }
    QString Qgroups() { return groups.c_str(); }
    QString Qmbs_settings() { return mbs_settings.c_str(); }
    QString Qspy_dir() { return spy_dir.c_str(); }
    QString Qoptions() { return options.c_str(); }
    QString Quser_def_spectra() { return user_def_spectra.c_str(); }
    QString Qautom() { return autom.c_str(); }
    QString Qcalibration() { return calibration.c_str(); }
    QString Qconditions() { return conditions.c_str(); }
    QString Qincrementers() { return incrementers.c_str(); }

    // checking if the current path points to the legal
    // tree of directories

    bool is_it_legal();
    void set_spy_dir(string p) {
        spy_dir = p;
        old_or_new_structure_of_directories();
        is_it_legal();

    }

    bool old_or_new_structure_of_directories();

};

extern Tpaths path ;


extern const string user_spec_ext ;
extern const string user_spec_filter ;
extern const string user_cond_ext ;
extern const string user_cond_filter ;
extern const string user_incr_ext ;
extern const string user_incr_filter ;
#endif //TPATHS_H
