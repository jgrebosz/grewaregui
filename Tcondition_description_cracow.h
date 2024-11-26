/***************************************************************************
                          Tcondition_description_cracow.h  -  description
                             -------------------
    begin                : pon sie 25 2003
    copyright            : (C) 2003 by dr. Jerzy Grebosz, IFJ Krakow, Poland
    email                : jerzy.grebosz@ifj.edu.pl
 ***************************************************************************/


#ifndef Tcondition_description_cracow_H
#define Tcondition_description_cracow_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std ;
#include "paths.h"

/**a stand alone condition which can be assigned to any user defined spectrum

  *@author dr. Jerzy Grebosz, IFJ Krakow, Poland
  */
//////////////////////////////////////////////////////////////////////////////
class Tcondition_description_cracow
{
    friend class T4manager_user_spectra ;
    friend class T4user_condition_wizard ;
    friend class T4manager_user_conditions;
protected:

  struct Titem_1D
  {
      bool obligatory_in_event ;
      string var_name ;
      double min_value, max_value ;
  };
  struct Titem_2D
  {
      bool obligatory_in_event ;
      string var_x_name ;
      string var_y_name ;
      string polygon_name;
  };


  string name ;
  vector <Titem_1D> item_1D_AND;
  vector <Titem_2D> item_2D_AND;
  vector <Titem_1D> item_1D_OR;
  vector <Titem_2D> item_2D_OR;
  vector <string> other_condition_AND;
  vector <string> other_condition_OR;
  vector <string> other_condition_NAND;
  vector <string> other_condition_NOR;

public:
	Tcondition_description_cracow();
	~Tcondition_description_cracow();
  /** No descriptions */
  void save_condition();
  /** No descriptions */
  void read_condition();
  void set_name(string n) { name = n; }
  string give_name() { return name; }
};
///////////////////////////////////////////////////////////////////////
#endif
