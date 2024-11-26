/***************************************************************************
                          Tpinup.cpp  -  description
                             -------------------
    begin                : Tue Aug 5 2003
    copyright            : (C) 2003 by dr. Jerzy Grebosz, IFJ Krakow, Poland
    email                : jerzy.grebosz@ifj.edu.pl
 ***************************************************************************/
 
#include "Tpinup.h"
#include "swiat.h"
#include <qpainter.h>
#include <qrect.h>
#include <math.h>

//****************************************************************
void Tpinup::draw(swiatek * world, QPainter * pior, bool log_scale)
{
  double vert =  world->wy(log_scale ? log10(y) : y ) ;
  string ekran = info + "\0" ;

  //pior->drawText(world->wx(x), (int) vert, info.c_str() );
  QRect ramka(world->wx(x), (int) vert, 800, 500);

  // NOTE: normal pinup can not be multiline. But the "notice" - can!
  pior->drawText(ramka, Qt::AlignLeft | Qt::TextWordWrap, info.c_str() );
}
//**************************************************************************
void Tpinup::read_from_disk_file(ifstream &s)
{
    s >> x  >>  y ;
    getline(s, info);
    // remove leading spaces
    while(isspace(info[0]) )
        info.erase(0, 1);
//    cout << "Wczytana nalepka " << x << ", " << y << " : " << info
//         <<endl;
}
//****************************************************************
