/***************************************************************************
                          Tself_gate_ger_descr.cpp  -  description
                             -------------------
    begin                : Mon Sep 8 2003
    copyright            : (C) 2003 by dr. Jerzy Grebosz, IFJ Krakow, Poland
    email                : jerzy.grebosz@ifj.edu.pl
   
    ======================================================
    This file may be copied from the cracow directrly 
     to the spy directory, but remember
    to change the extension from *.cpp into *.cxx
    ======================================================
    
 ***************************************************************************/

#include "Tself_gate_ger_descr.h"
#include "Tfile_helper.h"
#include <iostream>

#define fif(x,y) Nfile_helper::find_in_file((x),(y))

//********************************************************************
Tself_gate_ger_descr::Tself_gate_ger_descr()
{


  sg_type = Tselfgate_type::german_crystal; //  germanium_crystal ; // this is germanium
  enable_en4gate = false ;
  en4_gate[0] = 0 ;
  en4_gate[1] = 4096 ;

  enable_en20gate = false;
  en20_gate[0] = 0 ;
  en20_gate[1] = 20000 ;

  enable_time_gate = false ;
  time_gate[0] = 0;
  time_gate[1] = 8192;

  enable_xy_gamma_time_difference = false; // Marek Pfutzner

  enable_energy_time_polygon_gate = false;
  energy_time_polygon_gate = "";
  which_time = which_time_for_banana::time_cal;

  //-------------------

  enable_LR_time_gate = false ;
  LR_time_gate[0] = 0;
  LR_time_gate[1] = 8192;

  enable_SR_time_gate = false ;
  SR_time_gate[0] = 0;
  SR_time_gate[1] = 8192;

  //---------------    ????????????? degrees
  enable_geom_theta_gate = false ;
  geom_theta_gate[0] = 0;
  geom_theta_gate[1] = 360;

  enable_geom_phi_gate = false ;
  geom_phi_gate[0] = 0;
  geom_phi_gate[1] = 360;

  // gamma - particle angle ---> Radians ???????????
  enable_gp_theta_gate = false ;
  gp_theta_gate[0] = 0;
  gp_theta_gate[1] = 6.28;

  enable_gp_phi_gate = false ;
  gp_phi_gate[0] = 0;
  gp_phi_gate[1] = 6.28;

  // this is the multiplicity inside the cluster where this crystal belongs
  enable_mult_in_cluster_gate = false;
  mult_in_cluster_gate[0] = 0;
  mult_in_cluster_gate[1] = 7;

  // this is the multiplicity inside the cluster where this crystal belongs
  enable_AC_in_cluster_gate = false;
  AC_in_cluster_gate[0] = 0;
  AC_in_cluster_gate[1] = 0;

}
//**********************************************************************

//*********************************************************************
void Tself_gate_ger_descr::read_definition_from(string pathed_name)
{

  string file_name = pathed_name ;
  // gpath.user_def_spectra + name_of_spectrum + ".self_gate_ger_crystal" ;
  ifstream plik(file_name.c_str());
  if(!plik)
  {
        throw runtime_error( " Can't open the file "  + file_name +  " for reading selfgate");
  }

  try
  {
    //type =
      // I decided not to read this. It is already fixed
      // any errors will be automaticly fixed here
//      Nfile_helper::spot_in_file(plik, "type_of_self_gate") ;
//      plik >> sg_type;

    Nfile_helper::spot_in_file(plik, "name");
    plik >> name;   // will be without the path and extension

    //SPY NEEDS the extension to recognize the title
    //    unsigned int i = name.find( ".self_gate_ger_crystal"); // != string::npos)
    //    if(i == string::npos)
    //    {
    //      name += ".self_gate_ger_crystal" ;
    //    }

    enable_en4gate = (bool) Nfile_helper::find_in_file(plik, "enable_en4gate");

    en4_gate[0] = fif(plik, "en4_gate_low");
    en4_gate[1] = fif(plik, "en4_gate_high");

    enable_en20gate = (bool) fif(plik, "enable_en20gate");

    en20_gate[0] = fif(plik, "en20_gate_low");
    en20_gate[1] = fif(plik, "en20_gate_high");

    enable_time_gate = (bool) fif(plik, "enable_time_gate");

    time_gate[0] = fif(plik, "time_gate_low");
    time_gate[1] = fif(plik, "time_gate_high");

    //---------------
    enable_geom_theta_gate = (bool) fif(plik, "enable_geom_theta_gate");

    geom_theta_gate[0] = fif(plik, "geom_theta_gate_low");
    geom_theta_gate[1] = fif(plik, "geom_theta_gate_high");

    enable_geom_phi_gate = (bool) fif(plik, "enable_geom_phi_gate");

    geom_phi_gate[0] = fif(plik, "geom_phi_gate_low");
    geom_phi_gate[1] = fif(plik, "geom_phi_gate_high");

    //---------------
    enable_gp_theta_gate = (bool) fif(plik, "enable_gp_theta_gate");

    gp_theta_gate[0] = fif(plik, "gp_theta_gate_low");
    gp_theta_gate[1] = fif(plik, "gp_theta_gate_high");


    enable_gp_phi_gate = (bool) fif(plik, "enable_gp_phi_gate");

    gp_phi_gate[0] = fif(plik, "gp_phi_gate_low");
    gp_phi_gate[1] = fif(plik, "gp_phi_gate_high");

    //------------
    enable_mult_in_cluster_gate = (bool) fif(plik, "enable_mult_in_cluster_gate");

    mult_in_cluster_gate[0] = fif(plik, "mult_in_cluster_gate_low");
    mult_in_cluster_gate[1] = fif(plik, "mult_in_cluster_gate_high");

    try
    {  // old versions of the program didn't have this
      // this is the multiplicity inside the cluster where this crystal belongs
      enable_AC_in_cluster_gate = (bool) fif(plik, "enable_AC_in_cluster_gate");
      AC_in_cluster_gate[0] = fif(plik, "AC_in_cluster_gate_low");
      AC_in_cluster_gate[1] = fif(plik, "AC_in_cluster_gate_high");


      enable_LR_time_gate = (bool) fif(plik, "enable_LR_time_gate");
      LR_time_gate[0] = fif(plik, "LR_time_gate_low");
      LR_time_gate[1] = fif(plik, "LR_time_gate_high");

      enable_SR_time_gate = (bool) fif(plik, "enable_SR_time_gate");
      SR_time_gate[0] = fif(plik, "SR_time_gate_low");
      SR_time_gate[1] = fif(plik, "SR_time_gate_high");


    }
    catch(...)
    {
      enable_AC_in_cluster_gate = false;
      AC_in_cluster_gate[0] = 0;
      AC_in_cluster_gate[1] = 0;

      enable_LR_time_gate = false ;
      LR_time_gate[0] = 0;
      LR_time_gate[1] = 8192;

      enable_SR_time_gate = false;
      SR_time_gate[0] = 0;
      SR_time_gate[1] = 8192;
    }
    // xy - is also a new invention -------------
    try
    {
      enable_xy_gamma_time_difference = (bool) fif(plik, "enable_xy_gamma_time_difference" );
    }
    catch(...)
    {
      cout << "No infor in the selfgate file about enabeling/disabling  time diference, so I assume -> False " << endl;
      enable_xy_gamma_time_difference = false ;
    }
    // for banana  on energy vs time
    try
    {
      enable_energy_time_polygon_gate  = (bool) fif(plik, "enable_energy_time_polygon_gate" );

      int what = (int ) fif(plik, "which_time" );
      switch(what)
      {
      default:
      case time_cal: which_time = which_time_for_banana::time_cal; break;
      case LR_time_cal: which_time = which_time_for_banana::LR_time_cal; break;
      case SR_time_cal: which_time = which_time_for_banana::SR_time_cal; break;
      }

      Nfile_helper::spot_in_file(plik, "energy_time_polygon_gate" );
      plik >> energy_time_polygon_gate;
    }
    catch(...)
    {
      enable_energy_time_polygon_gate  = false;
      which_time = which_time_for_banana::time_cal;
      energy_time_polygon_gate = "no_polygon";
    }



  }
  catch(Tfile_helper_exception &m)
  {
    cout << "Error during reading the file "
    << file_name << " : "
    << m.message << endl;
  }
  //   cout << "Success" << endl;
}
//*************************************************************************
void Tself_gate_ger_descr::write_definitions(string path_only)
{
  string file_name = path_only + name;

  //  if(file_name.find(".self_gate_ger_crystal") == string::npos)
  //  {
  //    file_name += ".self_gate_ger_crystal" ;
  //  }

  ofstream plik(file_name.c_str());
  if(!plik)
  {
    cout << "Can't open the file " <<file_name
    << " for writing " << endl;
    return ;
  }

  plik
  << "// This is a definiton of the 'self_gate' for germanium crystal\n"
  << "// comments are marked using two slashes: // comment \n\n"
  << "\n//-----------------------------------------------------\n\n"

  << "type_of_self_gate\t\t" << sg_type << "\t\t// 1= Ger crystal, 2= Hec BaF\n"
  << "name\t\t " << name
  //<< ".self_gate_ger_crystal"
  << "\n\n"


  << "enable_en4gate\t\t" << enable_en4gate
  << "\ten4_gate_low\t"  << en4_gate[0]
  << "\ten4_gate_high\t" << en4_gate[1] << "\t// eneryg 4MeV gate\n"

  << "enable_en20gate\t\t" << enable_en20gate
  << "\ten20_gate_low\t"  << en20_gate[0]
  << "\ten20_gate_high\t" << en20_gate[1] << "\t// eneryg 20MeV gate\n"

  << "enable_time_gate\t\t" << enable_time_gate
  << "\ttime_gate_low\t"  << time_gate[0]
  << "\ttime_gate_high\t" << time_gate[1]
  << "\t// time gate\n"

  << "\tenable_xy_gamma_time_difference\t\t" << enable_xy_gamma_time_difference
  //----------
  << "\nenable_energy_time_polygon_gate\t\t"
  <<       enable_energy_time_polygon_gate
  << "\nwhich_time\t\t" << ((int) which_time)
  << "\nenergy_time_polygon_gate\t\t"      << energy_time_polygon_gate


  //-----------------------------
  << "\nenable_LR_time_gate\t\t" << enable_LR_time_gate
  << "\tLR_time_gate_low\t"  << LR_time_gate[0]
  << "\tLR_time_gate_high\t" << LR_time_gate[1] << "\t// Long Range time gate\n"

  << "enable_SR_time_gate\t\t" << enable_SR_time_gate
  << "\tSR_time_gate_low\t"  << SR_time_gate[0]
  << "\tSR_time_gate_high\t" << SR_time_gate[1] << "\t// Short Range time gate\n"


  //--------------
  << "\nenable_geom_theta_gate\t\t" << enable_geom_theta_gate
  << "\tgeom_theta_gate_low\t"  << geom_theta_gate[0]
  << "\tgeom_theta_gate_high\t" << geom_theta_gate[1] << "\t// geom_theta gate\n"

  << "enable_geom_phi_gate\t\t" << enable_geom_phi_gate
  << "\tgeom_phi_gate_low\t"  << geom_phi_gate[0]
  << "\tgeom_phi_gate_high\t" << geom_phi_gate[1] << "\t// geom_phi gate\n"

  //--------------
  << "enable_gp_theta_gate\t\t" << enable_gp_theta_gate
  << "\tgp_theta_gate_low\t"  << gp_theta_gate[0]
  << "\tgp_theta_gate_high\t" << gp_theta_gate[1] << "\t// gamma - scattered particle\n"

  << "enable_gp_phi_gate\t\t" << enable_gp_phi_gate
  << "\tgp_phi_gate_low\t"  << gp_phi_gate[0]
  << "\tgp_phi_gate_high\t" << gp_phi_gate[1] << "\t// gamma - scattered particle\n"


  << "enable_mult_in_cluster_gate\t\t" << enable_mult_in_cluster_gate
  << "\n\tmult_in_cluster_gate_low\t"  << mult_in_cluster_gate[0]
  << "\n\tmult_in_cluster_gate_high\t" << mult_in_cluster_gate[1] << "\t// multiplicity\n"


  << "enable_AC_in_cluster_gate\t\t" << enable_AC_in_cluster_gate
  << "\n\tAC_in_cluster_gate_low\t"  << AC_in_cluster_gate[0]
  << "\n\tAC_in_cluster_gate_high\t" << AC_in_cluster_gate[1] << "\t// AC energy\n"

  << "\n//-----------------------------------------------------\n\n"

  << endl;


}
//*********************************************************************


