#include "Tpaths.h"
#include <fstream>
#include <iostream>
#include <qdir.h>


Tpaths path ;
//***********************************************************
bool Tpaths::read_from_disk()
{

  ifstream plik("paths.cnf");
  if(plik)
    {
      plik >> spy_dir ;
    }
  else{
      cout << "Error while reading the file: paths.cnf" << endl;
    }
  check_for_slash_in_spy_dir() ;
  old_or_new_structure_of_directories();


  if(is_it_legal())
    return true;
  else return false;
}
//***********************************************************
void Tpaths::save_to_disk()   // this is a main function after choosing from GUI
{
  check_for_slash_in_spy_dir() ;
  is_it_legal();

  ofstream plik("paths.cnf");
  if(plik)
    {
      plik << spy_dir << endl  ;
    }
  else {
      cout << "Error while saving the file: paths.cnf" << endl;
    }
}

//************************************************************
bool Tpaths::is_it_legal()
{
  // we test here if spectra directory exist here
  QDir dir;

  if( dir.exists(Qspectra()) )
    {
      old_or_new_structure_of_directories();
      return true;
    }
  else return false;
}
//***********************************************************
void Tpaths::check_for_slash_in_spy_dir()
{
  if(spy_dir[spy_dir.length() -1] != '/' )
    {
      //cout << "No slash on the end : "<< spectra << endl ;
      spy_dir += "/" ;
      //cout << "after correction: "<< spectra << endl ;
    }
  //--------------------- for polygons -------------

}
//**************************************************************
 bool Tpaths::old_or_new_structure_of_directories()
 {
     check_for_slash_in_spy_dir() ;
   // checking if it is an old/new style of directories

   // we test here if spectra directory exist here
   QDir dir;

   if( dir.exists(QString( (spy_dir +"system").c_str()) ))
     {
       // New style

              spectra      = spy_dir + "spectra/";   // OK2
              gates_1D      = spy_dir + "system/gates/";     // ok2
              polygons      = spy_dir + "system/polygons/";    // OK2
              commands      = spy_dir + "system/commands/";   // OK2
              groups        = spy_dir + "groups/";   // OK2
              mbs_settings  = spy_dir + "system/mbs_settings/"; // ok2
              options       = spy_dir + "options/";   // ok2
              user_def_spectra  = spy_dir + "system/definitions_user_spectra/";  //ok2
              autom         = spy_dir + "system/automatic/"; // OK2
              calibration   = spy_dir + "calibration/"; // ok2
              conditions    = spy_dir + "system/conditions/";   // ok
              my_binnings    = spy_dir + "my_binnings/";
              incrementers = spy_dir + "system/incrementers_user_def/";

              if( dir.exists(QString( (spy_dir +"experiment_setup/").c_str()) ))
                  calibration   = spy_dir + "experiment_setup/";

              return false;

  }

       // otherwise old style
       //--------------------
       spectra      = spy_dir + "spectra/";
       gates_1D      = spy_dir + "gates/";
       polygons      = spy_dir + "polygons/";
       commands      = spy_dir + "commands/";
       groups        = spy_dir + "groups/";
       mbs_settings  = spy_dir + "mbs_settings/";
       options       = spy_dir + "options/";
       user_def_spectra  = spy_dir + "definitions_user_spectra/";
       autom         = spy_dir + "automatic/";
       calibration   = spy_dir + "calibration/";
       conditions    = spy_dir + "conditions/";
       my_binnings    = spy_dir + "my_binnings/";
       incrementers = spy_dir + "incrementers_user_def/";

       return true;
 }
