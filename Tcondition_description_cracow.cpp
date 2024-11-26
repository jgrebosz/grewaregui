/***************************************************************************
                          Tcondition_description_cracow.cpp  -  description
                             -------------------
    begin                : pon sie 25 2003
    copyright            : (C) 2003 by dr. Jerzy Grebosz, IFJ Krakow, Poland
    email                : jerzy.grebosz@ifj.edu.pl
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "Tcondition_description_cracow.h"
#include "Tfile_helper.h"
#include <qmessagebox.h>
#include <unistd.h>   // for unink
//**************************************************************
Tcondition_description_cracow::Tcondition_description_cracow()
{

}
//**************************************************************
Tcondition_description_cracow::~Tcondition_description_cracow()
{

}
//**************************************************************
/** No descriptions */
void Tcondition_description_cracow::read_condition()
{
    string pat_name = gpath.conditions + name ;
    if(pat_name.find(user_cond_ext) == string::npos)
    {
        pat_name += user_cond_ext ;
    }
    ifstream plik(pat_name.c_str() );
    if(!plik)
    {
        // cant open file warning
        QMessageBox::warning
                (nullptr, "File cant be opened" ,
                 QString("Error while opening file: ") + pat_name.c_str(),
                 QMessageBox::Yes | QMessageBox::Default,
                 QMessageBox::NoButton );
        return ;
    }

    try{

        Nfile_helper::spot_in_file(plik, "1D_AND_gates");
        string klamra ;
        plik >> zjedz >> klamra ; // "{"


        Titem_1D  item_1D_tmp;

        do
        {

            plik >> zjedz >> item_1D_tmp.var_name;
            if(item_1D_tmp.var_name == "}" || !plik ) break ;
            plik >> zjedz >> item_1D_tmp.obligatory_in_event ;
            plik >> zjedz >> item_1D_tmp.min_value;
            plik >> zjedz >> item_1D_tmp.max_value;

            item_1D_AND.push_back(item_1D_tmp) ;
        } while(plik);



        Nfile_helper::spot_in_file(plik, "1D_OR_gates");

        plik >> zjedz >> klamra ; // "{"



        do
        {

            plik >> zjedz >> item_1D_tmp.var_name;
            if(item_1D_tmp.var_name == "}" || !plik ) break ;
            plik >> zjedz >> item_1D_tmp.obligatory_in_event ;
            plik >> zjedz >> item_1D_tmp.min_value;
            plik >> zjedz >> item_1D_tmp.max_value;

            item_1D_OR.push_back(item_1D_tmp) ;
        } while(plik);




        Nfile_helper::spot_in_file(plik, "2D_AND_gates");
        plik >> zjedz >> klamra ; // "{"

        Titem_2D  item_2D_tmp;
        do
        {
            plik >> zjedz >> item_2D_tmp.var_x_name;
            if(item_2D_tmp.var_x_name == "}" || !plik ) break ;
            plik >> zjedz >> item_2D_tmp.var_y_name;
            plik >> zjedz >> item_2D_tmp.obligatory_in_event;
            plik >> zjedz >> item_2D_tmp.polygon_name;

            item_2D_AND.push_back(item_2D_tmp) ;
        } while(plik);



        Nfile_helper::spot_in_file(plik, "2D_OR_gates");
        plik >> zjedz >> klamra ; // "{"

        do
        {
            plik >> zjedz >> item_2D_tmp.var_x_name;
            if(item_2D_tmp.var_x_name == "}" || !plik ) break ;
            plik >> zjedz >> item_2D_tmp.var_y_name;
            plik >> zjedz >> item_2D_tmp.obligatory_in_event;
            plik >> zjedz >> item_2D_tmp.polygon_name;

            item_2D_OR.push_back(item_2D_tmp) ;
        } while(plik);


        string name2 ;

        Nfile_helper::spot_in_file(plik, "other_condition_AND");
        plik >> zjedz >> klamra ; // "{"
        do
        {
            plik >> zjedz >> name2;
            if(name2 == "}" || !plik ) break ;
            other_condition_AND.push_back(name2);
        }while(plik);

        Nfile_helper::spot_in_file(plik, "other_condition_OR");
        plik >> zjedz >> klamra ; // "{"
        do
        {
            plik >> zjedz >> name2;
            if(name2 == "}" || !plik ) break ;
            other_condition_OR.push_back(name2);
        }while(plik);

        Nfile_helper::spot_in_file(plik, "other_condition_NAND");
        plik >> zjedz >> klamra ; // "{"
        do
        {
            plik >> zjedz >> name2;
            if(name2 == "}" || !plik ) break ;
            other_condition_NAND.push_back(name2);
        }while(plik);

        Nfile_helper::spot_in_file(plik, "other_condition_NOR");
        plik >> zjedz >> klamra ; // "{"
        do
        {
            plik >> zjedz >> name2;
            if(name2 == "}" || !plik ) break ;
            other_condition_NOR.push_back(name2);
        }while(plik);

    }


    catch(Tfile_helper_exception &m)
    {
        cout << "Error during reading the file "
         << pat_name << " : "
         << m.message << endl;
        unlink(pat_name.c_str());
    }

}
//**************************************************************
/** No descriptions */
void Tcondition_description_cracow::save_condition()
{
    string pat_name = gpath.conditions + name ;
    if(pat_name.find(user_cond_ext) == string::npos)
    {
        pat_name += user_cond_ext ;
    }
    ofstream plik(pat_name.c_str() );
    if(!plik)
    {
        // cant open file
    }

    plik
            << name << "\t\t// condition name \n"
    << "1D_AND_gates\t // variable name \t obligatory_in_event \tminimal value   \tmaxmimal value\n{\n"     ;


    for(unsigned i = 0 ; i < item_1D_AND.size() ; i++)
    {
        plik << item_1D_AND[i].var_name  << "\t\t\t"
         << item_1D_AND[i].obligatory_in_event << "\t"
         << item_1D_AND[i].min_value << "\t"
         << item_1D_AND[i].max_value << "\n";
    }
    plik << "\n}\n\n";


    plik  << "1D_OR_gates\t // variable name \t obligatory_in_event \tminimal value   \tmaxmimal value\n{\n"     ;


    for(unsigned i = 0 ; i < item_1D_OR.size() ; i++)
    {
        plik << item_1D_OR[i].var_name  << "\t\t\t"
         << item_1D_OR[i].obligatory_in_event << "\t"
         << item_1D_OR[i].min_value << "\t"
         << item_1D_OR[i].max_value << "\n";
    }


    plik << "\n}\n\n"
        << "2D_AND_gates\n{\n"     ;

    for(unsigned i = 0 ; i < item_2D_AND.size() ; i++)
    {
        plik
                << item_2D_AND[i].var_x_name << "\t\t// var_x_name\n"
      << item_2D_AND[i].var_y_name << "\t\t// var_y_name\n"
      << item_2D_AND[i].obligatory_in_event << "\t\t// obligatory_in_event (1=obligatory, 0= if_present)\n"
      << item_2D_AND[i].polygon_name << "\t\t// polygon_name\n\n" ;
    }
    plik << "}\n\n" ;


    plik << "2D_OR_gates\n{\n"     ;

    for(unsigned i = 0 ; i < item_2D_OR.size() ; i++)
    {
        plik
                << item_2D_OR[i].var_x_name << "\t\t// var_x_name\n"
      << item_2D_OR[i].var_y_name << "\t\t// var_y_name\n"
      << item_2D_OR[i].obligatory_in_event << "\t\t// obligatory_in_event (1=obligatory, 0= if_present)\n"
      << item_2D_OR[i].polygon_name << "\t\t// polygon_name\n\n" ;
    }
    plik << "}\n\n" ;



    //-----------------------

    plik << "\n\nother_condition_AND\n{\n"     ;

    for(unsigned i = 0 ; i < other_condition_AND.size() ; i++)
    {
        plik
                << other_condition_AND[i] << "\t\t// other condition name\n";
    }
    plik << "}\n\n" ;

    //-------------
    plik << "\n\nother_condition_OR\n{\n"     ;

    for(unsigned i = 0 ; i < other_condition_OR.size() ; i++)
    {
        plik
                << other_condition_OR[i] << "\t\t// other condition name\n";
    }
    plik << "}\n\n" ;


    //-------------

    plik << "\n\nother_condition_NAND\n{\n"     ;

    for(unsigned i = 0 ; i < other_condition_NAND.size() ; i++)
    {
        plik
                << other_condition_NAND[i] << "\t\t// other condition name\n";
    }
    plik << "}\n\n" ;


    //-------------

    plik << "\n\nother_condition_NOR\n{\n"     ;

    for(unsigned i = 0 ; i < other_condition_NOR.size() ; i++)
    {
        plik
                << other_condition_NOR[i] << "\t\t// other condition name\n";
    }
    plik << "}\n\n" ;



}
//******************************************************************
