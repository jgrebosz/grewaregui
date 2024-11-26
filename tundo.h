/***************************************************************************
                          tundo.h  -  description
                             -------------------
    begin                : Tue Jul 22 2003
    copyright            : (C) 2003 by Dr. Jerzy Grebosz, IFJ Krakow, Poland
    email                : jerzy.grebosz@ifj.edu.pl
 ***************************************************************************/

#ifndef TUNDO_H
#define TUNDO_H


#include <vector>
#include <list>
using namespace std;

#include "gate_1D.h"
#include "Tpinup.h"




/**Undo for any spectra operation
  *@author Dr. Jerzy Grebosz, IFJ Krakow, Poland
  */
////////////////////////////////////////////////////////////////////////////
class Tundo {
public: 

	int min_counts ;
	int max_counts;
	typ_x min_x;
	typ_x max_x;

	double bgr_slope ;
	double bgr_offset;
  /**  */
  int rebin_factor;  // for rebin on screen option

	typ_x marker_older;
	typ_x marker_younger;

	typ_x recent_bgr_marker ;
	vector<typ_x> backgr_markers;

	bool flag_bgr_valid ;

	vector<gate_1D> my_gates_1D;
	string undo_description_text ;
	vector <Tpinup> nalepka ;

  /** stores all information inside */



                    
};
////////////////////////////////////////////////////////////////////////////
#endif
