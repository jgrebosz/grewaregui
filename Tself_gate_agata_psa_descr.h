/***************************************************************************
                          Tself_gate_aga_descr.h  -  description
                             -------------------
    begin                : Mon Sep 8 2003
    copyright            : (C) 2003 by dr. Jerzy Grebosz, IFJ Krakow, Poland
    email                : jerzy.grebosz@ifj.edu.pl
 ***************************************************************************/


#ifndef TSELF_GATE_AGATA_PSA_DESCR_H
#define TSELF_GATE_AGATA_PSA_DESCR_H

#include <string>
using namespace std;

#include "Tself_gate_abstract_descr.h"


/**Text file which describes the gate on the agata crystal
  *@author dr. Jerzy Grebosz, IFJ Krakow, Poland
  */
//////////////////////////////////////////////////////////////////////////
class Tself_gate_agata_psa_descr : public Tself_gate_abstract_descr
{
public:
  string name;

  bool enable_energy_gate ;
  double energy_gate[2];


  bool enable_time_gate ;
  double time_gate[2];

  bool enable_x_gate ;
  double x_gate[2];

  bool enable_y_gate ;
  double y_gate[2];

    bool enable_z_gate ;
  double z_gate[2];


  // this is the multiplicity inside the cryostat where this crystal  belongs
  bool enable_mult_of_hits_gate ;
  double mult_of_hits_gate[2];


  Tself_gate_agata_psa_descr();
  virtual ~Tself_gate_agata_psa_descr() {} ;
  void read_definition_from(string pathed_name);
  void write_definitions(string path_only);
};

#endif
