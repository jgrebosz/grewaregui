/***************************************************************************
                          Tself_gate_agata_psa_descr.cxx  -  description
                             -------------------
    begin                : Mon Sep 8 2003
    copyright            : (C) 2003 by dr. Jerzy Grebosz, IFJ Krakow, Poland
    email                : jerzy.grebosz@ifj.edu.pl
 ***************************************************************************/

#include "Tself_gate_agata_psa_descr.h"
#include "Tfile_helper.h"
#include <iostream>

#define fif(x,y) Nfile_helper::find_in_file((x),(y))

//********************************************************************
Tself_gate_agata_psa_descr::Tself_gate_agata_psa_descr()
{
    sg_type =  Tselfgate_type::agata_psa ;    // this hit in the segmented detector
    name = "type-name-here";

    enable_energy_gate = false ;
    energy_gate[0] = 0 ;
    energy_gate[1] = 8192 ;


    enable_time_gate = false;
    time_gate[0] = 0;
    time_gate[1] = 8192;

    enable_x_gate = false;
    x_gate[0] = -100;
    x_gate[1] = +100;

    enable_y_gate = false;
    y_gate[0] = -100;
    y_gate[1] = +100;

    enable_z_gate = false;
    z_gate[0] = -100;
    z_gate[1] = +100;

    // this is the multiplicity inside the cryostat where this crystal  belongs
    enable_mult_of_hits_gate = false;
    mult_of_hits_gate[0] = 0 ;
    mult_of_hits_gate[1] = 100;

}
//**********************************************************************

//*********************************************************************
void Tself_gate_agata_psa_descr::read_definition_from(string pathed_name)
{
    string file_name = pathed_name ;
    // gpath.user_def_spectra + name_of_spectrum + ".self_gate_aga_crystal" ;
    ifstream plik(file_name.c_str());
    if (!plik)
    {
        throw runtime_error( " Can't open the file "  + file_name +  " for reading selfgate");
    }

    try
    {
        Nfile_helper::spot_in_file(plik, "type_of_self_gate") ;
        plik >> sg_type;
        Nfile_helper::spot_in_file(plik, "name");
        plik >> name;   // will be without the path and extension

        //SPY NEEDS the extension to recognize the title
//    unsigned int i = name.find( ".self_gate_aga_crystal"); // != string::npos)
//    if(i == string::npos)
//    {
//      name += ".self_gate_aga_crystal" ;
//    }

        enable_energy_gate = (bool) Nfile_helper::find_in_file(plik, "enable_energy_gate");

        energy_gate[0] = fif(plik, "energy_gate_low");
        energy_gate[1] = fif(plik, "energy_gate_high");


        enable_time_gate = (bool) fif(plik, "enable_time_gate");

        time_gate[0] = fif(plik, "time_gate_low");
        time_gate[1] = fif(plik, "time_gate_high");

        //---------------
        enable_x_gate = (bool) fif(plik, "enable_x_gate");

        x_gate[0] = fif(plik, "x_gate_low");
        x_gate[1] = fif(plik, "x_gate_high");

        //---------------
        enable_y_gate = (bool) fif(plik, "enable_y_gate");

        y_gate[0] = fif(plik, "y_gate_low");
        y_gate[1] = fif(plik, "y_gate_high");
        //---------------
        enable_z_gate = (bool) fif(plik, "enable_z_gate");

        z_gate[0] = fif(plik, "z_gate_low");
        z_gate[1] = fif(plik, "z_gate_high");
	
        //------------	
	  // this is the multiplicity inside the cryostat where this crystal  belongs

        enable_mult_of_hits_gate = (bool) fif(plik, "enable_mult_of_hits_gate");

        mult_of_hits_gate[0] = fif(plik, "mult_of_hits_gate_low");
        mult_of_hits_gate[1] = fif(plik, "mult_of_hits_gate_high");
    }
    catch (Tfile_helper_exception &m)
    {
        cout << "Error during reading the file "
             << file_name << " : "
             << m.message << endl;
    }
}
//*************************************************************************
void Tself_gate_agata_psa_descr::write_definitions(string path_only)
{
    string file_name = path_only + name;

//  if(file_name.find(".self_gate_aga_crystal") == string::npos)
//  {
//    file_name += ".self_gate_aga_crystal" ;
//  }

    ofstream plik(file_name.c_str());
    if (!plik)
    {
        cout << "Can't open the file " <<file_name
             << " for writing " << endl;
        return ;
    }

    plik
    << "// This is a definiton of the 'self_gate' for germanium agata crystal\n"
    << "// comments are marked using two slashes: // comment \n\n"
    << "\n//-----------------------------------------------------\n\n"

    << "type_of_self_gate\t\t" << sg_type << "\t\t// 1= Ger crystal, 2= Hec BaF\n"
    << "name\t\t " << name
    //<< ".self_gate_aga_crystal"
    << "\n\n"


    << "enable_energy_gate\t\t" << enable_energy_gate
    << "\tenergy_gate_low\t"  << energy_gate[0]
    << "\tenergy_gate_high\t" << energy_gate[1] << "\t// energy of the hit\n"


    << "enable_time_gate\t\t" << enable_time_gate
    << "\ttime_gate_low\t"  << time_gate[0]
    << "\ttime_gate_high\t" << time_gate[1] << "\t// time gate\n"

//--------------
    << "enable_x_gate\t\t" << enable_x_gate
    << "\tx_gate_low\t"  << x_gate[0]
    << "\tx_gate_high\t" << x_gate[1] << "\t\n"


    << "enable_y_gate\t\t" << enable_y_gate
    << "\ty_gate_low\t"  << y_gate[0]
    << "\ty_gate_high\t" << y_gate[1] << "\t\n"

    << "enable_z_gate\t\t" << enable_z_gate
    << "\tz_gate_low\t"  << z_gate[0]
    << "\tz_gate_high\t" << z_gate[1] << "\t\n"



    << "enable_mult_of_hits_gate\t\t" << enable_mult_of_hits_gate
    << "\tmult_of_hits_gate_low\t"  << mult_of_hits_gate[0]
    << "\tmult_of_hits_gate_high\t" << mult_of_hits_gate[1] << "\t// multiplicity\n"



    << "\n//-----------------------------------------------------\n\n"

    << endl;


}
//*********************************************************************


