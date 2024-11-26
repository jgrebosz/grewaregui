/***************************************************************************
                          Tself_gate_ger_addback_descr.cpp  -  description
                             -------------------
    begin                : 27 Dec 2005
    copyright            : (C) 2005 by dr. Jerzy Grebosz, IFJ Krakow, Poland
    email                : jerzy.grebosz@ifj.edu.pl
 ***************************************************************************/

#include "Tself_gate_ger_addback_descr.h"
#include "Tfile_helper.h"
#include <iostream>

#define fif(x,y) Nfile_helper::find_in_file((x),(y))

//********************************************************************
Tself_gate_ger_addback_descr::Tself_gate_ger_addback_descr()
{
    sg_type = Tselfgate_type::cluster_addback; // this is germanium
    enable_en4gate = false ;
    en4_gate[0] = 0 ;
    en4_gate[1] = 4096 ;

    enable_time_gate = false ;
    time_gate[0] = 0;
    time_gate[1] = 8192;

    enable_LR_time_gate = false ;
    LR_time_gate[0] = 0;
    LR_time_gate[1] = 8192;

    enable_SR_time_gate = false ;
    SR_time_gate[0] = 0;
    SR_time_gate[1] = 8192;


    enable_energy_time_polygon_gate = false;
    energy_time_polygon_gate = "";
    which_time = time_cal;


    //---------------    ????????????? degrees
    enable_geom_theta_gate = false ;
    geom_theta_gate[0] = 0;
    geom_theta_gate[1] = 360;

    enable_geom_phi_gate = false ;
    geom_phi_gate[0] = 0;
    geom_phi_gate[1] = 360;

    //---secondary (degrees)
    enable_geom_theta_gate_secondary = false ;
    geom_theta_gate_secondary[0] = 0;
    geom_theta_gate_secondary[1] = 360;

    enable_geom_phi_gate_secondary = false ;
    geom_phi_gate_secondary[0] = 0;
    geom_phi_gate_secondary[1] = 360;

    // this is the multiplicity inside the cluster where this crystal belongs
    enable_mult_in_cluster_gate = false;
    mult_in_cluster_gate[0] = 0;
    mult_in_cluster_gate[1] = 7;

    // this is the multiplicity inside the cluster where this crystal belongs
    enable_AC_in_cluster_gate = false;
    AC_in_cluster_gate[0] = 0;
    AC_in_cluster_gate[1] = 0;

}
//*********************************************************************
//*********************************************************************
void Tself_gate_ger_addback_descr::read_definition_from(string pathed_name)
{

    enable_energy_time_polygon_gate = "click_here";
    string file_name = pathed_name ;
    // gpath.user_def_spectra + name_of_spectrum + ".self_gate_ger_crystal" ;
    ifstream plik(file_name.c_str());
    if(!plik)
    {
       throw runtime_error( " Can't open the file "  + file_name +  " for reading selfgate");
    }

    try
    {
        // I decided not to read this. It is already fixed
        // any errors will be automaticly fixed here
        //        Nfile_helper::spot_in_file(plik, "type_of_self_gate") ;
        //        plik >> sg_type;


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



        enable_time_gate = (bool) fif(plik, "enable_time_gate");

        time_gate[0] = fif(plik, "time_gate_low");
        time_gate[1] = fif(plik, "time_gate_high");

        //--------Primary -------
        enable_geom_theta_gate = (bool) fif(plik, "enable_geom_theta_gate");

        geom_theta_gate[0] = fif(plik, "geom_theta_gate_low");
        geom_theta_gate[1] = fif(plik, "geom_theta_gate_high");


        enable_geom_phi_gate = (bool) fif(plik, "enable_geom_phi_gate");

        geom_phi_gate[0] = fif(plik, "geom_phi_gate_low");
        geom_phi_gate[1] = fif(plik, "geom_phi_gate_high");

        //--------secondary ------- is inside the try


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

            // SECONDARY ===========
            enable_geom_theta_gate_secondary = (bool) fif(plik, "enable_geom_theta_gate_secondary");
            geom_theta_gate_secondary[0] = fif(plik, "geom_theta_gate_low_secondary");
            geom_theta_gate_secondary[1] = fif(plik, "geom_theta_gate_high_secondary");


            enable_geom_phi_gate_secondary = (bool) fif(plik, "enable_geom_phi_gate_secondary");
            geom_phi_gate_secondary[0] = fif(plik, "geom_phi_gate_low_secondary");
            geom_phi_gate_secondary[1] = fif(plik, "geom_phi_gate_high_secondary");

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


            enable_geom_theta_gate_secondary = false;
            geom_theta_gate_secondary[0] = 0;
            geom_theta_gate_secondary[1] = 360;

            enable_geom_phi_gate_secondary = false;
            geom_phi_gate_secondary[0] = 0;
            geom_phi_gate_secondary[1] = 366;

        }
        // for banana  on energy vs time
        try
        {
            enable_energy_time_polygon_gate  = (bool) fif(plik, "enable_energy_time_polygon_gate" );

            int what = (int ) fif(plik, "which_time" );
            switch(what)
            {
            default:
                case time_cal: which_time = time_cal; break;
                case LR_time_cal: which_time = LR_time_cal; break;
                case SR_time_cal: which_time = SR_time_cal; break;
            }

            Nfile_helper::spot_in_file(plik, "energy_time_polygon_gate" );
            plik >> energy_time_polygon_gate;
        }
        catch(...)
        {
            enable_energy_time_polygon_gate  = false;
            which_time = time_cal;
            energy_time_polygon_gate = "no_polygon";
        }

    }
    catch(Tfile_helper_exception &m)
    {
        cout << "2Error during reading the file "
             << file_name << " : "
             << m.message << endl;
    }
    plik.close();
}
//*************************************************************************
void Tself_gate_ger_addback_descr::write_definitions(string path_only)
{
    string file_name = path_only + name;


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

            << "type_of_self_gate\t\t" << sg_type << "\t\t// 1= Ger crystal, 2= Hec BaF, 3 miniball, 4 Clust addback\n"
            << "name\t\t " << name
               //<< ".self_gate_ger_crystal"
            << "\n\n"


            << "enable_en4gate\t\t" << enable_en4gate
            << "\ten4_gate_low\t"  << en4_gate[0]
            << "\ten4_gate_high\t" << en4_gate[1] << "\t// eneryg 4MeV gate\n"

            << "enable_time_gate\t\t" << enable_time_gate
            << "\ttime_gate_low\t"  << time_gate[0]
            << "\ttime_gate_high\t" << time_gate[1] << "\t// time gate\n"

               //----------
            << "enable_LR_time_gate\t\t" << enable_LR_time_gate
            << "\tLR_time_gate_low\t"  << LR_time_gate[0]
            << "\tLR_time_gate_high\t" << LR_time_gate[1] << "\t// Long Range time gate\n"

            << "enable_SR_time_gate\t\t" << enable_SR_time_gate
            << "\tSR_time_gate_low\t"  << SR_time_gate[0]
            << "\tSR_time_gate_high\t" << SR_time_gate[1] << "\t// Short Range time gate\n"

               //----------
            << "\nenable_energy_time_polygon_gate\t\t"
            <<       enable_energy_time_polygon_gate
                  << "\nwhich_time\t\t" << ((int) which_time)
                  << "\nenergy_time_polygon_gate\t\t"      << energy_time_polygon_gate
                  << "     xxxxxxxxxxxxxxxxxxxxxxxxxxxx "  // <--- this is to be sure, that an empty field is not written (that would mess the rest)




                     //--------------
                  << "\nenable_geom_theta_gate\t\t" << enable_geom_theta_gate
                  << "\n\tgeom_theta_gate_low\t"  << geom_theta_gate[0]
                  << "\n\tgeom_theta_gate_high\t" << geom_theta_gate[1]
                  << "\t// geom_theta gate\n"

                  << "enable_geom_phi_gate\t\t" << enable_geom_phi_gate
                  << "\n\tgeom_phi_gate_low\t"  << geom_phi_gate[0]
                  << "\n\tgeom_phi_gate_high\t" << geom_phi_gate[1] << "\t// geom_phi gate\n"

                     //--------------Secondary
                  << "\nenable_geom_theta_gate_secondary\t\t" << enable_geom_theta_gate_secondary
                  << "\n\tgeom_theta_gate_low_secondary\t"  << geom_theta_gate_secondary[0]
                  << "\n\tgeom_theta_gate_high_secondary\t" << geom_theta_gate_secondary[1]
                  << "\t// geom_theta gate_secondary\n"

                  << "enable_geom_phi_gate_secondary\t\t" << enable_geom_phi_gate_secondary
                  << "\n\tgeom_phi_gate_low_secondary\t"  << geom_phi_gate_secondary[0]
                  << "\n\tgeom_phi_gate_high_secondary\t" << geom_phi_gate_secondary[1] << "\t// geom_phi gate_secondary\n"

                     //--------------

                  << "enable_mult_in_cluster_gate\t\t" << enable_mult_in_cluster_gate
                  << "\n\tmult_in_cluster_gate_low\t"  << mult_in_cluster_gate[0]
                  << "\n\tmult_in_cluster_gate_high\t" << mult_in_cluster_gate[1] << "\t// multiplicity\n"


                  << "enable_AC_in_cluster_gate\t\t" << enable_AC_in_cluster_gate
                  << "\n\tAC_in_cluster_gate_low\t"  << AC_in_cluster_gate[0]
                  << "\n\tAC_in_cluster_gate_high\t" << AC_in_cluster_gate[1] << "\t// AC energy\n"

                  << "\n//-----------------------------------------------------\n\n"

                  << endl;
    plik.close();

}
//*********************************************************************


