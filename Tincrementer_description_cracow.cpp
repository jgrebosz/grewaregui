/***************************************************************************
                          Tincrementer_description_cracow.cpp  -  description
                             -------------------
    begin                :
    copyright            : (C) 2003 by dr. Jerzy Grebosz, IFJ Krakow, Poland
    email                : jerzy.grebosz@ifj.edu.pl
 ***************************************************************************/

#include "Tincrementer_description_cracow.h"
#include "Tfile_helper.h"
#include <qmessagebox.h>
#include <unistd.h>   // for unink
//**************************************************************
Tincrementer_description_cracow::Tincrementer_description_cracow()
{
    //default values
    flag_exprA_is_constant = false;
    other_incrementer_nameA = "press_to_choose" ;
    constant_valueA = 1;

    operation = 1; // plus, minus, times, divide

    flag_exprB_is_constant = false;
    other_incrementer_nameB = "press_to_choose" ;
    constant_valueB = 1;

}
//**************************************************************
Tincrementer_description_cracow::~Tincrementer_description_cracow()
{}
//**************************************************************
/** No descriptions */
void Tincrementer_description_cracow::read_incrementer()
{

    string pat_name = gpath.incrementers + name ;
    if(pat_name.find(user_incr_ext) == string::npos)
    {
        pat_name += user_incr_ext ;
    }
    ifstream plik(pat_name.c_str() );
    if(!plik)
    {
        // cant open file warning
        QMessageBox::warning
                (0, "File cant be opened" ,
                 QString("Error while opening file: ") + pat_name.c_str(),
                 QMessageBox::Yes | QMessageBox::Default,
                 QMessageBox::NoButton );
        return ;
    }

    //default values
    flag_exprA_is_constant = false;
    other_incrementer_nameA = "press_to_choose" ;
    constant_valueA = 1;

    operation = 1; // plus, minus, times, divide

    other_incrementer_nameB = "press_to_choose" ;
    flag_exprB_is_constant = false;
    constant_valueB = 1;





    try{

        FH::spot_in_file(plik, "$user_incrementer_name");
        plik >>  name;

        flag_exprA_is_constant = FH::find_in_file(plik, "$flag_exprA_is_constant");

        FH::spot_in_file(plik, "$other_incrementer_nameA");
        plik >> other_incrementer_nameA ;

        constant_valueA = FH::find_in_file(plik, "$constant_valueA");

        operation = FH::find_in_file(plik, "$operation");

        constant_valueB = FH::find_in_file(plik, "$constant_valueB");

        flag_exprB_is_constant = FH::find_in_file(plik, "$flag_exprB_is_constant");

        FH::spot_in_file(plik, "$other_incrementer_nameB");
        plik >> other_incrementer_nameB;

    }
    catch(Tno_keyword_exception &m)
    {
        cout << "Error during reading the file "
             << pat_name << "\n" << m.message

             << "\n so most probably it is an OLD format of incrementer"
             << endl;

        FH::repair_the_stream(plik);
        plik.seekg(0);

          plik >> zjedz >> name
          >> zjedz >> other_incrementer_nameA
          >> zjedz >> operation
          >> zjedz >> flag_exprB_is_constant
          >> zjedz >> other_incrementer_nameB
          >> zjedz >> constant_valueB;

          if(!plik) {
              cerr << "The format is even not good as 'old' " << endl;
              throw;
          }
    }
    catch(Treading_value_exception &m)
    {
        cout << "Error during reading the file "
             << pat_name << "\n" << m.message << endl;
        throw;
    }

}
//**************************************************************
/** No descriptions */
void Tincrementer_description_cracow::save_incrementer()
{
    string pat_name = gpath.incrementers + name ;
    if(pat_name.find(user_incr_ext) == string::npos)
    {
        pat_name += user_incr_ext;
    }
    ofstream plik(pat_name.c_str() );
    if(!plik)
    {
        // cant open file
    }

    plik << "\n$user_incrementer_name\t"<< name
         << "\n$flag_exprA_is_constant\t" << flag_exprA_is_constant
         << "\n$other_incrementer_nameA\t" << other_incrementer_nameA
         << "\n$constant_valueA\t" << constant_valueA

         << "\n\n$operation\t"<< operation
         << "\n\n$flag_exprB_is_constant\t" << flag_exprB_is_constant
         << "\n$other_incrementer_nameB\t" << other_incrementer_nameB
         << "\n$constant_valueB\t" << constant_valueB
            << endl;

}
//******************************************************************
