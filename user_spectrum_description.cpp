/***************************************************************************
                          user_spectrum_description.cpp  -  description
                             -------------------
    begin                : Fri Jul 25 2003
    copyright            : (C) 2003 by Dr. Jerzy Grebosz, IFJ Krakow, Poland
    email                : jerzy.grebosz@ifj.edu.pl
***************************************************************************/

#include "user_spectrum_description.h"
#include "paths.h"
#include <iostream>
#include <fstream>
#include "Tfile_helper.h"


//*************************************************************************
user_spectrum_description::user_spectrum_description()
{

  name_of_spectrum = "user_" ;
  enabled = true ;
  
  dimmension= spec_1D;
  bins_x = 8192 ;
  beg_x = 0;
  end_x = 8192;
  
  bins_y = 256 ;
  beg_y = 0;
  end_y = 8192;
  policy_when_increm  = 0 ; // always   

  flag_rotation = false;
  rotation_angle = 0.0;

  condition_name = "No_condition" ;
  
}
//**************************************************************************
user_spectrum_description::~user_spectrum_description()
{
	
}
//**************************************************************************
/** read the definitons from the disk */
bool user_spectrum_description::read_from(string pathed_name)
{

  string file_name = pathed_name ;
    // gpath.user_def_spectra + name_of_spectrum + user_spec_ext ;
  ifstream plik(file_name.c_str());
  if(!plik)
  {
      cout << "Can't open the file "  << file_name
            << " for reading " << endl;
      return false;
  }

  try
  {

      Nfile_helper::spot_in_file(plik, "spectrum_name");
      plik >> name_of_spectrum ;   // will be without the path and extension
      
      enabled = (int)Nfile_helper::find_in_file(plik, "enabled");

      dimmension = (type_of_data) Nfile_helper::find_in_file(plik, "dimmension");
      
      bins_x = (int) Nfile_helper::find_in_file(plik, "bins_x");
      beg_x = Nfile_helper::find_in_file(plik, "beg_x");
      end_x = Nfile_helper::find_in_file(plik, "end_x");

      bins_y = (int) Nfile_helper::find_in_file(plik, "bins_y");
      beg_y = Nfile_helper::find_in_file(plik, "beg_y");
      end_y = Nfile_helper::find_in_file(plik, "end_y");


     
      // reading X incrementors-------------------------
      Nfile_helper::spot_in_file(plik, "incrementors_x");
      string klamra ;
      plik >> zjedz >> klamra ; // "{";
      string wyraz, self_gate ;

      do
      {
        plik >> wyraz >> self_gate;
        if(wyraz == "}" || !plik ) break ;
        incrementors_x.push_back( pair<string,string>(wyraz, self_gate)) ;
      } while(plik);

      // reading Y incrementors------------------------

      Nfile_helper::spot_in_file(plik, "incrementors_y");
      plik >> zjedz >> klamra ; // "{";

      do
      {
        plik >> wyraz >> self_gate;
        if(wyraz == "}" || !plik ) break ;
        incrementors_y.push_back(pair<string,string>(wyraz, self_gate)) ;
      }while(plik);

     policy_when_increm  =  (int) Nfile_helper::find_in_file(plik, "policy_when_increment");
     
     Nfile_helper::spot_in_file(plik, "condition_name");
     plik >> condition_name ;
     //cout << "Read condition name = " << condition_name << endl ;


     // new feature, may not be present in old definitions
     try {
          flag_rotation = Nfile_helper::find_in_file(plik, "flag_rotation");
          rotation_angle = Nfile_helper::find_in_file(plik, "rotation_angle");

     }
     catch (Tfile_helper_exception &m)
     {
         flag_rotation = false;
         rotation_angle = 0.0;
     }


     return true ;
     
  }
  catch(Tfile_helper_exception &m)
  {
    cout << "Error during reading the file "
         << file_name << " : "
         << m.message << endl;
    return false ;
  }
}
//**************************************************************************
/** save the definition on the disk */
void user_spectrum_description::write_definitions(string path_only)
{
  string file_name = path_only + name_of_spectrum + user_spec_ext ;

  ofstream plik(file_name.c_str());
  if(!plik)
    {
      cout << "Can't open the file " <<file_name
          << " for writing " << endl;
      return ;
    }

  plik
    << "// This is a definiton of the conditional spectrum\n"
    << "// enables are 0 or 1 which means false or true\n"
    << "// comments are marked using two slashes: // comment \n\n"
    << "spectrum_name\t\t " << name_of_spectrum << "\n"
    << "enabled\t\t" << enabled << "\n\n"


    << "dimmension\t\t" << int(dimmension)
    << "\n//-----------------------------------------------------\n\n"

    << "bins_x\t\t " << bins_x <<  "\n"
    << "beg_x\t\t " << beg_x <<  "\t\t// left edge of the first bin\n"
    << "end_x\t\t " << end_x <<  "\t\t// right edge of the last bin\n\n"
    << "bins_y\t\t " << bins_y <<  "\n"
    << "beg_y\t\t " << beg_y <<  "\t\t// left edge of the first bin\n"
    << "end_y\t\t " << end_y <<  "\t\t// right edge of the last bin\n\n"

    << endl;


    plik << "\nincrementors_x                // and their self_gates\n{\n";
    for(unsigned i = 0 ; i < incrementors_x.size() ; i++)
    {
          plik << incrementors_x[i].first << "\t"
            << incrementors_x[i].second   << "\n" ;
    }
    plik << "}\n";

     plik << "\nincrementors_y               // and their self_gates\n{\n";
    for(unsigned i = 0 ; i < incrementors_y.size() ; i++)
    {
          plik <<  incrementors_y[i].first << "\t" << incrementors_y[i].second << "\n" ;
    }
    plik << "}\n\n";

    
    plik << "policy_when_increment\t" 
          << policy_when_increm    
          << "\t// 0= always, 1=only when from DIFFERENT detector, 2 = only when from SAME detectror"
          << "\n" ;


    plik << "flag_rotation " << flag_rotation << "\n";
    plik << "rotation_angle " << rotation_angle << "\n";
    
    if(condition_name.find(user_cond_ext) == string::npos)
    {
      condition_name += user_cond_ext;
    }
    plik  << "\ncondition_name\t\t" << condition_name << endl ;
    
    
          
          

}
//*********************************************************************
/** No descriptions */
void user_spectrum_description::set_name(string s)
{
  name_of_spectrum = s ;
}
//********************************************************************
/** To know is somebody just changed some parameters so,
that we must re-create it (changed dimmension or binning) */
bool user_spectrum_description::are_parameters_identical(const user_spectrum_description & u) const
{
  if(dimmension != u.dimmension) return false ;
  if(dimmension == spec_1D)
  {
    return
    (
    bins_x == u.bins_x
    &&
    beg_x == u.beg_x
    &&
    end_x == u.end_x) ;
  }
  else
  {
    return
    (
    bins_x == u.bins_x
    &&
    beg_x == u.beg_x
    &&
    end_x == u.end_x
    &&
    bins_y == u.bins_y
    &&
    beg_y == u.beg_y
    &&
    end_y == u.end_y
    ) ;
  }
  
}
//*********************************************************************
string user_spectrum_description::get_selfgates() 
{
	string odp, co ;
	 for(unsigned i = 0 ; i < incrementors_x.size() ; i++)
    {
        co = incrementors_x[i].second ;
        if(co  != "No_self_gate")
        {
          if(co.rfind(".") != string::npos)
          {
            co.erase(co.rfind(".") );
          }
          odp +=  co + ", " ;
        }
    }

	 for(unsigned i = 0 ; i < incrementors_y.size() ; i++)
    {
        co =incrementors_y[i].second ;
        if(co  != "No_self_gate")
        {
            if(co.rfind(".") != string::npos)
            {
              co.erase(co.rfind(".") );
            }
            odp +=  co + ", " ;
        }
    }
	return odp ;
}
//*********************************************************************


