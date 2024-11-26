#ifndef box_of_counts_h
#define box_of_counts_h


#include <QtGlobal>

#include <qwidget.h>
#include <qpainter.h>
#include "swiat.h"

class Tplate_spectrum ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// this is a class responsible for drawing the Y scale of the spectrum (counts)
class box_of_counts :  public QWidget, public swiatek
{
    
    Tplate_spectrum *parent ;

    // current values of the counts range
    typ_x minY ;
    typ_x maxY;

protected:
    void mouseMoveEvent ( QMouseEvent * event ) ;
    void mousePressEvent ( QMouseEvent * e );
    void mouseReleaseEvent( QMouseEvent * e );

	int last_dragged_y ;  // for dragging the scale
    bool flag_draw_rainbow_scale; // for 2D matrices raibow legend (Z-scale)
    void drawing_rainbow_background_scale(QPainter * piorko);
  void draw_for_1D(QPainter * piorko);
  void draw_for_2D(QPainter * piorko);

public:

    box_of_counts(
//            QWidget *a_parent= 0, bool flag_rainbow, string name = "");
    QWidget *a_parent, bool flag_rainbow, string name);
    // parent is the widget where this box is drawn,
    // (boss is the object which sends the order what to draw
public:
    

    /** really drawing function */
    void paintEvent( QPaintEvent * )
    {
        QPainter piorko_ekranowe (this);
        draw_all(&piorko_ekranowe);
    }
    //-------------------
    void draw_all(QPainter * pior);
    
    void change_region_of_counts(typ_x a, typ_x b)
    {
        minY = a;
        maxY = b;
    }
    

public slots:
    void wheelEvent ( QWheelEvent * e );
};

#endif // box_of_counts_h
