/***************************************************************************
                          Tincrementer_description_cracow.h  -  description
                             -------------------
    begin                : pon sie 25 2003
    copyright            : (C) 2003 by dr. Jerzy Grebosz, IFJ Krakow, Poland
    email                : jerzy.grebosz@ifj.edu.pl
 ***************************************************************************/


#ifndef Tincrementer_description_cracow_H
#define Tincrementer_description_cracow_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std ;
#include "paths.h"

/**a stand alone incrementer which can be assigned to any user defined spectrum
 
  *@author dr. Jerzy Grebosz, IFJ Krakow, Poland
  */
//////////////////////////////////////////////////////////////////////////////
class Tincrementer_description_cracow
{
  friend class T4manager_user_incrementers ;
  friend class T4user_incrementer_wizard ;
protected:

  string name ; // of this incrementer

  bool flag_exprA_is_constant;
  string other_incrementer_nameA ;
  double constant_valueA;


  int operation; // plus, minus, times, divide, mean

  bool flag_exprB_is_constant;
  string other_incrementer_nameB;
  double constant_valueB;


public:
  Tincrementer_description_cracow();
  ~Tincrementer_description_cracow();
  /** No descriptions */
  void save_incrementer();
  /** No descriptions */
  void read_incrementer();
  void set_name(string n) { name = n; }
  string give_name() { return name; }
};
///////////////////////////////////////////////////////////////////////
#endif
