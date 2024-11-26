#ifndef TSELF_GATE_GER_ADDBACK_DESCR_H
#define TSELF_GATE_GER_ADDBACK_DESCR_H

/***************************************************************************
                          Tself_gate_ger_addback_descr.h  -  description
                             -------------------
    begin                : Mon Sep 8 2003
    copyright            : (C) 2003 by dr. Jerzy Grebosz, IFJ Krakow, Poland
    email                : jerzy.grebosz@ifj.edu.pl
 ***************************************************************************/

#include <string>
using namespace std;

#include "Tself_gate_abstract_descr.h"


//////////////////////////////////////////////////////////////////////////
class Tself_gate_ger_addback_descr : public Tself_gate_abstract_descr
{
public:


  bool enable_en4gate ;
  double en4_gate[2];


  bool enable_time_gate ;
  double time_gate[2];

  bool enable_LR_time_gate ;
  double LR_time_gate[2];
  
  bool enable_SR_time_gate ;
  double SR_time_gate[2];
  
  // ---------------- for polygon gate on energy vs time (one of  three times)
  bool  enable_energy_time_polygon_gate;
  string energy_time_polygon_gate;
  
  enum which_time_for_banana { time_cal, LR_time_cal, SR_time_cal } ;
  which_time_for_banana    which_time;


  bool enable_geom_theta_gate ;
  double geom_theta_gate[2];

  bool enable_geom_phi_gate ;
  double geom_phi_gate[2];

// secondary

  bool enable_geom_theta_gate_secondary ;
  double geom_theta_gate_secondary[2];

  bool enable_geom_phi_gate_secondary ;
  double geom_phi_gate_secondary[2];

  // this is the multiplicity inside the cluster where this crystal belongs
  bool enable_mult_in_cluster_gate ;
  double mult_in_cluster_gate[2];

  // this is the AC inside the cluster where this crystal belongs
  bool enable_AC_in_cluster_gate ;
  double AC_in_cluster_gate[2];

  Tself_gate_ger_addback_descr();
  virtual ~Tself_gate_ger_addback_descr() {} ;
  void read_definition_from(string pathed_name);
  void write_definitions(string path_only);
};

#endif
