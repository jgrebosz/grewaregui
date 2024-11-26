/***************************************************************************
                          Tself_gate_mib_descr.h  -  description
                             -------------------
    begin                : Mon Sep 8 2003
    copyright            : (C) 2003 by dr. Jerzy Grebosz, IFJ Krakow, Poland
    email                : jerzy.grebosz@ifj.edu.pl
 ***************************************************************************/


#ifndef TSELF_GATE_MIB_DESCR_H
#define TSELF_GATE_MIB_DESCR_H

#include <string>
using namespace std;

#include "Tself_gate_abstract_descr.h"


/**Text file which describes the gate on the mibmanium crystal
  *@author dr. Jerzy Grebosz, IFJ Krakow, Poland
  */
//////////////////////////////////////////////////////////////////////////
class Tself_gate_mib_descr : public Tself_gate_abstract_descr
{
public:

  bool enable_core_energy_gate ;
  double core_energy_gate[2];


  bool enable_time_gate ;
  double time_gate[2];

  bool enable_geom_theta_gate ;
  double geom_theta_gate[2];

  bool enable_geom_phi_gate ;
  double geom_phi_gate[2];

  // gamma - particle angle
  bool enable_gp_theta_gate ;
  double gp_theta_gate[2];

  bool enable_gp_phi_gate ;
  double gp_phi_gate[2];

  // this is the multiplicity inside the cryostat where this crystal  belongs
  bool enable_mult_in_cryostat_gate ;
  double mult_in_cryostat_gate[2];


  Tself_gate_mib_descr();
  virtual ~Tself_gate_mib_descr() {} ;
  void read_definition_from(string pathed_name);
  void write_definitions(string path_only);
};

#endif
