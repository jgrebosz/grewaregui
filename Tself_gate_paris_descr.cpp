/***************************************************************************
                          Tself_gate_paris_descr.cpp  -  description
                             -------------------
    begin                : Mon Sep 8 2003
    copyright            : (C) 2003 by dr. Jerzy Grebosz, IFJ Krakow, Poland
    email                : jerzy.grebosz@ifj.edu.pl
 ***************************************************************************/

#include "Tself_gate_paris_descr_simple.h"
#include "Tfile_helper.h"
#include <iostream>

#define fif(x,y) Nfile_helper::find_in_file((x),(y))

//********************************************************************
Tself_gate_paris_descr::Tself_gate_paris_descr()
{

    sg_type =  Tselfgate_type::paris ; // this is hector
    enable_energy_fast_gate = false ;
    en_fast_gate[0] = 0 ;
    en_fast_gate[1] = 4096 ;

    enable_energy_slow_gate = false;
    en_slow_gate[0] = 0 ;
    en_slow_gate[1] = 20000 ;

    enable_time_gate = false ;
    time_gate[0] = 0;
    time_gate[1] = 8192;

    // ---------------- for polygon gate on energy vs time (one of  three times)
    enable_fast_vs_slow_polygon_gate = false;
    name_fast_vs_slow_polygon_gate = "no_polygon";


    //---------------    ????????????? degrees
    enable_geom_theta_gate = false ;
    geom_theta_gate[0] = 0;
    geom_theta_gate[1] = 360;

    enable_geom_phi_gate = false ;
    geom_phi_gate[0] = 0;
    geom_phi_gate[1] = 360;

    // gamma - particle angle    Radians !!!!!!!!!!!!!
    enable_gp_theta_gate = false ;
    gp_theta_gate[0] = 0;
    gp_theta_gate[1] = 6.28;

    enable_gp_phi_gate = false ;
    gp_phi_gate[0] = 0;
    gp_phi_gate[1] = 6.28;


}
//**********************************************************************

//*********************************************************************
void Tself_gate_paris_descr::read_definition_from(string pathed_name)
{


    string file_name = pathed_name ;
    // gpath.user_def_spectra + name_of_spectrum + ".self_gate_paris_crystal" ;
    ifstream plik(file_name.c_str());
    if(!plik)
    {
          throw runtime_error( " Can't open the file "  + file_name +  " for reading selfgate");
    }

    try
    {
        // I decided not to read this. It is already fixed
        // any errors will be automaticly fixed here

        // Nfile_helper::spot_in_file(plik, "type_of_self_gate") ;
        // plik >> sg_type;


        Nfile_helper::spot_in_file(plik, "name");
        plik >> name;   // will be without the path and extension

        //SPY NEEDS the extension to recognize the title
        //    unsigned int i = name.find( ".self_gate_paris_crystal"); // != string::npos)
        //    if(i == string::npos)
        //    {
        //      name += ".self_gate_paris_crystal" ;
        //    }

        enable_energy_fast_gate = (bool) Nfile_helper::find_in_file(plik, "enable_energy_fast_gate");

        en_fast_gate[0] = fif(plik, "en_fast_gate_low");
        en_fast_gate[1] = fif(plik, "en_fast_gate_high");


        enable_energy_slow_gate = (bool) fif(plik, "enable_energy_slow_gate");

        en_slow_gate[0] = fif(plik, "en_slow_gate_low");
        en_slow_gate[1] = fif(plik, "en_slow_gate_high");


        enable_time_gate = (bool) fif(plik, "enable_time_gate");

        time_gate[0] = fif(plik, "time_gate_low");
        time_gate[1] = fif(plik, "time_gate_high");


        try    // because this may not exist in old selfgates
        {
            enable_fast_vs_slow_polygon_gate = fif(plik, "enable_fast_vs_slow_polygon_gate");
            // for banana  on energy vs time
            try
            {
                if(enable_fast_vs_slow_polygon_gate)
                {
                    FH::spot_in_file(plik, "name_fast_vs_slow_polygon_gate");
                    plik >> name_fast_vs_slow_polygon_gate;
                    if(name_fast_vs_slow_polygon_gate == "no_polygon")
                    {
                        throw "nic";
                    }
//                    // reading the polygon
//                    if(!read_banana(name_fast_vs_slow_polygon_gate,   &polygon))
//                    {
//                        cout << "During Reading-in the self gate named "
//                             << name
//                             << "\n  [B] Impossible to read polygon gate: " << name_fast_vs_slow_polygon_gate
//                             << "\nMost probably it does not exist (anymore?)"
//                             << endl;
//                        exit(1);
//                    }
                }
            }
            catch(...)
            {
                enable_fast_vs_slow_polygon_gate  = false;
                name_fast_vs_slow_polygon_gate = "no_polygon";
                FH::repair_the_stream(plik);
            }
        }
        catch(Tno_keyword_exception & ex)
        {
            // defaults are used
            enable_fast_vs_slow_polygon_gate = false;
            name_fast_vs_slow_polygon_gate = "no_polygon";
            FH::repair_the_stream(plik);
        }




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

    }
    catch(Tfile_helper_exception &m)
    {
        cout << "Error during reading the file "
             << file_name << " : "
             << m.message << endl;
    }
}
//*************************************************************************
void Tself_gate_paris_descr::write_definitions(string path_only)
{
    string file_name = path_only + name;

    //  if(file_name.find(".self_gate_paris_crystal") == string::npos)
    //  {
    //    file_name += ".self_gate_paris_crystal" ;
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
               //<< ".self_gate_paris_crystal"
            << "\n\n"


            << "enable_energy_fast_gate\t\t" << enable_energy_fast_gate
            << "\ten_fast_gate_low\t"  << en_fast_gate[0]
            << "\ten_fast_gate_high\t" << en_fast_gate[1] << "\t// energy FAST gate\n"

            << "enable_energy_slow_gate\t\t" << enable_energy_slow_gate
            << "\ten_slow_gate_low\t"  << en_slow_gate[0]
            << "\ten_slow_gate_high\t" << en_slow_gate[1] << "\t// eneryg SLOW gate\n"

            << "enable_time_gate\t\t" << enable_time_gate
            << "\ttime_gate_low\t"  << time_gate[0]
            << "\ttime_gate_high\t" << time_gate[1] << "\t// time gate\n"

            << "enable_fast_vs_slow_polygon_gate\t" << enable_fast_vs_slow_polygon_gate  << "\n"
            << "\tname_fast_vs_slow_polygon_gate\t" << name_fast_vs_slow_polygon_gate  << "\n"


               //--------------
            << "enable_geom_theta_gate\t\t" << enable_geom_theta_gate
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




            << "\n//-----------------------------------------------------\n\n"

            << endl;


}
//*********************************************************************


