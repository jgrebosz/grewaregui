/***************************************************************************
                          Tpinup.h  -  description
                             -------------------
    begin                : Tue Aug 5 2003
    copyright            : (C) 2003 by dr. Jerzy Grebosz, IFJ Krakow, Poland
    email                : jerzy.grebosz@ifj.edu.pl
 ***************************************************************************/

#ifndef TPINUP_H
#define TPINUP_H

#include <string>
#include <fstream>
using namespace std ;

/**For markers on the field of the spectrum
  *@author dr. Jerzy Grebosz, IFJ Krakow, Poland
  */
class QPainter ;
class swiatek ;
//#include <qstring.h>
////////////////////////////////////////////////////////////////////////////////////////////////////
class Tpinup
{
protected:
  double x ;
  double y ;    // coordinates on the spectrum
  string info ;
public:

  Tpinup(double xx, double yy, string inf): x(xx), y(yy), info(inf) {}
  Tpinup() { x = 0 ; y = 0 ; info = "" ;}

  ~Tpinup() {} ;
  void draw(swiatek* w, QPainter *pior, bool flag_log);
  double give_x() { return x ;}
  double give_y() { return y ;}
  void set_xy(double xx, double yy) { x = xx ; y = yy ;}
  void set_info(string inf) { info = inf.c_str(); }

  string give_info() { return info ;}
  void save_to_disk_file(ofstream & s)
  {
    s << x << "  " << y << " " << info << endl;
  }
  //---------------------------------------------------------
  void read_from_disk_file(ifstream & s);
};

#endif
