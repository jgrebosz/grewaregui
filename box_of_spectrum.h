#ifndef box_of_spectrum_h
#define box_of_spectrum_h

//#include <qworkspace.h>
#include <qevent.h>
#include <qcursor.h>
#include <qapplication.h>

#include <qwidget.h>
#include <qpainter.h>
#include <QMenu>


#include <QGridLayout>
#include <QShortcut>
#include <QKeySequence>

#include <vector>



#include "swiat.h"



//class spectrum_widget ;
class spectrum_1D ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// this is a class responsible for drawing the box with  the spectrum
class box_of_spectrum :  public QWidget, public swiatek
{
    Q_OBJECT

    // geometrical position of this box in the parent widget
    //    double f_left;	// factor_left
    //    double f_top; 	//factor_top,
    //    double f_right;	//factor_right,
    //    double f_bottom;	// factor_bottom,

        // QWidget *parent ;
        spectrum_1D   *parent ;  // <-- here we now that is is only 1D spectrum
    // spectrum_widget* parent ;

    vector<int> *spectrum_ptr ;
    QPainter * printer ;

    // current parameters for drawing
    typ_x minX;
    typ_x maxX;
    int maxY;
    int minY ;

    // pointer to the data in the spectrum widget
    spectrum_descr * specif ;            // nr of bins, np. 1024
    // begining of spectrum np. 0.32
    // end of spectrum np. 14.33


    bool flag_was_mouse_click;
    typ_x x_mouse_click;
    int y_mouse_click;
    Qt::MouseButtons pressed_button_state ;
    //    QColor backgr_color = QColor(0, 0, 60); // now global

    typ_x marker[2] ;   // integration and expansion markers


    QPixmap pix;   // pixmap for making picture of matrix
    bool make_new_pixmap ;  // to force making general change (for expample after expanding)

    double real_x;
    double real_y;
    string real_spectrum_name ;

public:

    box_of_spectrum (
        // factors telling the relative positions of this widget inside the parent widget
        // for example 0,    0, 1,    1   -- would mean full surface of the parent
        //                     0.5, 0, 1, 0.5 -- would mean upper right square
        //                                                                                   |_X|
        //                                                                                   |__|
        //        double factor_left,
        //        double factor_top,
        //        double factor_right,
        //        double factor_bottom,
        QWidget *a_parent,
        //..= 0,
        string name = "" );
    // parent is the widget wher this box is drawn,
    // (boss is the object which sends the order what to draw
public:

    void set_printing_mode ( QPainter * druk )
    {
        printer = druk ;
    }


    //void draw(int fromY, int toY);    // drawing the box with the scale
    //  void draw(int minX, int maxY, int maxX, int minY);
    void paintEvent ( QPaintEvent * );

    void draw_all ( QPainter * piorko );

    void draw_integration_markers ( QPainter & piorko );
    void draw_background_markers ( QPainter & piorko );
    void draw_1Dgates ( QPainter & piorko );
    void draw_pinup_notices (QPainter & piorko , int fontsize);


    void set_spectrum_pointer ( vector<int> * ptr, spectrum_descr * desc );

    void 	change_region_of_spectrum ( typ_x a, typ_x b, typ_x c, typ_x d )
    {
        minX = a;
        maxY = b;
        maxX = c;
        minY = d;
    }


    //----------------------------------
    void mousePressEvent ( QMouseEvent * e );
    void mouseMoveEvent ( QMouseEvent* e );
    void  take_position_from_mouse() ;

    bool was_mouse_click()
    {
        return  flag_was_mouse_click;
    }
    void clear_flag_was_mouse_click()
    {
        flag_was_mouse_click = false;
    }
    //--------------
    typ_x give_x_mouse_click()
    {
        return  x_mouse_click;
    }

    void set_x_mouse_click(typ_x x)
    {
          x_mouse_click = x;
    }
    //-------------------
    typ_x give_y_mouse_click()
    {
        return  y_mouse_click;
    }

   void set_y_mouse_click(typ_x y)
    {
        y_mouse_click = y;
    }

    //-- markers
    void erase_markers()
    {
        marker[0] = 0 ;
        marker[1] = 0 ;
    }
    void set_markers ( typ_x k, typ_x m )
    {
        marker[0] = k ;
        marker[1] = m ;
    }


    // new factors, when the widget was resized, and the aspect is such, that we would like
    // it to be a bit different, so the world have to be recalculated.
    //    void new_factors ( double factor_left,
    //                       double factor_top,
    //                       double factor_right,
    //                       double factor_bottom
    //                     )
    //    {
    //        // remembering the factors
    //        f_left	= factor_left;
    //        f_top = factor_top;
    //        f_right = factor_right;
    //        f_bottom = factor_bottom;
    //    }

    void force_new_pixmap ( bool b );
    /** No descriptions */
    void set_overlay_mode ( bool b );

public slots:
    void ctxMenu(const QPoint &);
    void test_slot() { cout << "obsluga tej opcji " << endl; } ;


protected: // Protected attributes
    /**  */
    bool  overlay_mode;
    bool black_white ;
    int krot; // rebinning factor


public slots:
    void showSpectrumContextMenu(QPoint pos);

protected:
};


#endif // box_of_spectrum_h
