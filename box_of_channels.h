#ifndef box_of_channels_h
#define box_of_channels_h


#include <qwidget.h>
#include <qpainter.h>
#include "swiat.h"

class Tplate_spectrum ;


// this is a class responsible for drawing  the Y scale of the spectrum (counts)
class box_of_channels :  public QWidget,  public swiatek
{
//    double f_left;	// factor_left
//    double f_top; 	//factor_top,
//    double f_right;	//factor_right,
//    double f_bottom;	// factor_bottom,

    //  QWidget
    Tplate_spectrum *parent ;

    typ_x minX;
    typ_x maxX;

protected:
    void mouseMoveEvent ( QMouseEvent * event ) ;
     void mousePressEvent ( QMouseEvent * e );
     void mouseReleaseEvent( QMouseEvent * e );

     typ_x last_dragged_x ;  // for dragging the scale


public:

    box_of_channels(
            // factors telling the relative positions of this widget inside the parent widget
            // for example 0,    0, 1,    1   -- would mean full surface of the parent
            //             0.5,  0, 1,  0.5   -- would mean upper right square
            //                                                                                   |_X|
            //                                                                                   |__|
//            double factor_left,
//            double factor_top,
//            double factor_right,
//            double factor_bottom,
            QWidget *a_parent,

            string name = "");


    // parent is the widget wher this box is drawn,
    // (boss is the object which sends the order what to draw


public:

    //  void set_printing_mode(QPrinter * druk){printer = druk ;}

    //void draw(int fromY, int toY);    // drawing the box with the scale

    void paintEvent( QPaintEvent * )
    {
        QPainter piorko_ekranowe (this);
        draw_all(&piorko_ekranowe);
    }

    //-------------------
    void draw_all(QPainter * pior);


    void 	change_region_of_channels(typ_x a, typ_x b)
    {
        minX = a;
        maxX = b;
    }

    //-------------------------------------------------
    // new factors, when the widget was resized, and the aspect is such, that we would like
    // it to be a bit different, so the world have to be recalculated.
//    void new_factors(double factor_left,
//                     double factor_top,
//                     double factor_right,
//                     double factor_bottom
//                     )
//    {
//        // remembering the factors
//        f_left	= factor_left;
//        f_top = factor_top;
//        f_right = factor_right;
//        f_bottom = factor_bottom;
//        //    cout << "box_of_channels:: New factors are left" << f_left << ", right = " << f_right
//        //         << ", f_bottom = " << f_bottom << ", top = " << f_top << endl;
//        //    repaint();
//    }


    //  void update()
    //  {

    //      cout << "box_of_channels:: update is working"
    //           << int(time(NULL)) << endl;
    //      repaint();
    //  }

    int ilo_cyfrowe_liczby(typ_x  roznica);
public slots:
    void wheelEvent ( QWheelEvent * e );

};

#endif // box_of_channels_h
