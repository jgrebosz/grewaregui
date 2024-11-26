/***************************************************************************
                          Tself_gate_abstract_descr.h  -  description
                             -------------------
    begin                : Thu Sep 11 2003
    copyright            : (C) 2003 by dr. Jerzy Grebosz, IFJ Krakow, Poland
    email                : jerzy.grebosz@ifj.edu.pl

This file is modified in cracow directory, so it should be copied
to the spy directory (not oposite)

***************************************************************************/
#ifndef TSELF_GATE_ABSTRACT_DESCR_H
#define TSELF_GATE_ABSTRACT_DESCR_H

#include <string>
using namespace std;

#include <stdexcept>
#include "tselfgate_type.h"

/**Abstract for selfgates crystal, cate, hector, etc
  *@author dr. Jerzy Grebosz, IFJ Krakow, Poland
  */

//**********************************************************************************
class Tself_gate_abstract_descr {
public:
    string name;
public:


    Tselfgate_type sg_type;

    Tself_gate_abstract_descr();
    virtual ~Tself_gate_abstract_descr(){}

    virtual void read_definition_from(string pathed_name) = 0 ;
    virtual void write_definitions(string path_only) = 0 ;
    virtual void this_is_x_incrementer()   {};
    virtual void this_is_y_incrementer() {} ;
    virtual bool not_for_1Dspectrum() { return false ; }
    string give_name() { return name; }

    static
    Tself_gate_abstract_descr * create_descr_for_sg_file(string sg_name);

};

#endif
