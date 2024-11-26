// definicje swiatowych wspolrzednych

#include "swiat.h"

/**************************************************************************/
void swiatek::ustaw_swiat(
  int left, int top, int right, int bottom,   // dane pikselowe
  double x0, double y1, double x1, double y0) // dane swiata,
// jak uklad kartezjanski czyli najpierw lewy dolny rog
{
  // w tym systemie nie ma viewport, zatem musimy zapamietac
  // pozcje pikselowe poczatku naszego swiata
  lewy_bok_dev = left ;
  gorny_bok_dev = top ;
  dolny_bok_dev = bottom ;



  rozm_xdev = right - left ;
  rozm_ydev = bottom - top ;

  rozm_xwor = x1 - x0 ;
  rozm_ywor = y1 - y0 ;

  if(!rozm_xdev || !rozm_ydev || !rozm_xwor || !rozm_ywor)
    return ;

  scaler_x = rozm_xdev / rozm_xwor;
  scaler_y = rozm_ydev / rozm_ywor;

  //flag_world = 1 ;
  worldLeft = x0 ;
  worldTop = y1 ;
  worldRight = x1 ;
  worldBottom = y0 ;

//   cout
//   << "w ustaw swiat worldLeft = " << worldLeft
//   << " worldRight = " << worldLeft
//   << endl;

}


