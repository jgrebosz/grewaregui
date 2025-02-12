#ifndef box_of_matrix_h
#define box_of_matrix_h


#include <qwidget.h>
#include <qpainter.h>
#include <qcolor.h>
#include <QMenu>
#include <QRubberBand>
#include <QPainterPath>
#include <qbitmap.h>
#include <vector>
#include <list>
//#include <queue>  // for undo banana


#include "swiat.h"

#include "polygon_gate.h"


class spectrum_widget ;
class spectrum_2D ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// this is a class responsible for drawing the box withe the  matrix
class box_of_matrix :  public QWidget, public swiatek
{
    Q_OBJECT

public:
    static vector<QColor> paleta ;
private:



    // QWidget *parent ;
    spectrum_2D   *parent ;
    vector<int> *matrix_ptr ;   // source of data (spectrum)
    QPainter * printer ;


    // current parameters for drawing
    typ_x  minX;
    typ_x  maxY;
    typ_x  maxX;
    typ_x  minY ;

    int minZthreshold; // threshold for displaying z axis

    // pointer to the data in the spectrum widget
    spectrum_descr * specif ;            // nr of bins, np. 1024
    // begining of spectrum np. 0.32
    // end of spectrum np. 14.33


    bool flag_was_mouse_click;
    bool flag_mouse_just_pressed;
    bool flag_screen_just_repainted; // for refreshing during lupa mode
    //bool flag_banana_just_deleted;


    bool flag_banana_creating_mode;  // while user clicks vertices of the new banana
    vector<QPoint> new_banana_points;


    bool flag_marking_selected_vertices_region_mode = false;  // while user makes rubberband to select vertices
    bool flag_deselect_all_vertices_outside = false;


    // we remember how far was the place of clicking from the center of the vertex
    // then during moving - we can correct the movement
    int x_distance_to_vertex;
    int y_distance_to_vertex;

    int pixel_tolerance; // how far the user can click from the verttex, to still  select it

    typ_x x_mouse_click;
    typ_x y_mouse_click;

    Qt::MouseButtons pressed_button_state ;

    double real_x;
    double real_y;
    string real_spectrum_name ;

    typ_x marker[2] ;   // integration and expansion markers

    QPixmap pix;   // pixmap for making picture of matrix
    bool make_new_pixmap ;  // to force making general change (for expample after expanding)


    // 	for rubberband ;
    QColor c;
    QPoint starting_pt;
    QPoint current_end_pt;
    QPoint previous_end_pt;

    //crosshair_mode

    bool rubberband_on;
    //--------------
    bool flag_lupa_mode ;

    bool flag_crosshair_cursor;
    bool flag_crosshair_mode_just_started;
    //QPoint starting_crosshair_pt;
    QPoint current_crosshair_end_pt;
    QPoint previous_crosshair_end_pt;

    QBitmap lupa_plus ;
    QBitmap lupa_plus_maska ;
    QBitmap lupa_minus ;
    QBitmap lupa_minus_maska ;


    enum typ_of_cursor { is_usual, is_lupa_plus, is_lupa_minus };
    typ_of_cursor current_cursor ;
public:

    box_of_matrix (
        // fators telling the relative positions of this widget inside the parent widget
        // for example 0,    0, 1,    1   -- would mean full surface of the parent
        //                     0.5, 0, 1, 0.5 -- would mean upper right square
        //                                                                                   |_X|
        //                                                                                   |__|
        //        double factor_left,
        //        double factor_top,
        //        double factor_right,
        //        double factor_bottom,
        QWidget *a_parent= nullptr,
        string name = "" );
    // parent is the widget wher this box is drawn,
    // (boss is the object which sends the order what to draw

    ~box_of_matrix ()
    {
        delete rubberBand_lupa;
    }
    list<polygon_gate> undo_banana_list ;



    bool  is_crosshair_mode()
    {
        return flag_crosshair_cursor;
    }
    void crosshair_mode ( bool on )
    {
        flag_crosshair_cursor = on;
        //rubberband_on = on;
        flag_crosshair_mode_just_started = on;
    }
    void crosshair_handler ( QMouseEvent *e ) ;
    void moving_polygon_vertex_handler ( QMouseEvent *e );

    void make_undo_banana() ;
    auto how_many_undo_banana_possible()
    {
        return undo_banana_list.size() ;
    }
    //    void set_printing_mode ( QPainter * druk )
    //    {	printer = druk ;
    //    }
    void inform_that_banana_just_deletd()
    {
        flag_move_one_vertex_of_polygon = false;
        flag_move_whole_polygon = false;
        flag_move_selected_vertices_of_polygon = false;
        rubberband_on = false ; // probably not needed, but usually stays ON
    }
    void make_rubberband_off() { rubberband_on = false ; }

    string disk_name_for_new_banana;
    string screen_name_for_new_banana;

    void drawing_new_banana_mode ( string screen_name, string disk_name )
    {
        flag_banana_creating_mode = true;  // while user clicks vertices of the new banana
        new_banana_points.clear();
        disk_name_for_new_banana = disk_name;
        screen_name_for_new_banana = screen_name;
    }

    void draw_lines_of_new_banana();
    void finish_drawing_new_banana();

    //void draw(int fromY, int toY);    // drawing the box with the scale
    //  void draw(int minX, int maxY, int maxX, int minY);
    void paintEvent ( QPaintEvent * );

    void draw_all ( QPainter * piorko );
    void draw_pinup_infos (QPainter & piorko , int font_size);
    void draw_bananas ( QPainter & piorko );



    void set_spectrum_pointer ( vector<int> * ptr, spectrum_descr * desc );

    void 	change_region_of_spectrum ( typ_x a, typ_x b, typ_x cc, typ_x d )
    {

        minX = a;
        maxY = b;
        maxX = cc;
        minY = d;
        //cout << "max Y changed into =" << maxY << endl;
    }


    //----------------------------------
    void mousePressEvent ( QMouseEvent * e );

    // for rubberband
    void mouseMoveEvent ( QMouseEvent* e );
    void mouseReleaseEvent ( QMouseEvent* e );

    // to react on shift pressed during zoom mode
    void keyPressEvent ( QKeyEvent * e ) ;
    void keyReleaseEvent ( QKeyEvent * e );

    void wheelEvent ( QWheelEvent * /*e*/ ) {}

    //***********************************************************************
    void  take_position_from_last_mouse()
    {
        // we make it easer: just take the last mouse position
        // cout << "przeliczenie pozycji na numer kanalu : ? " ;

        x_mouse_click =real_x;
        y_mouse_click = real_y;

        // cout << "last mouse positon is = " << x_mouse_click
        //      << ", y = " << y_mouse_click
        //      << " at spectrum name " << real_spectrum_name
        //      << endl;

    }
    // below there are not slots

    bool was_mouse_click()
    {
        return  flag_was_mouse_click;
    }
    void clear_flag_was_mouse_click()
    {
        flag_was_mouse_click = false;
    }

    typ_x give_x_mouse_click()
    {
        return  x_mouse_click;
    }
    typ_x give_y_mouse_click()
    {
        return  y_mouse_click;
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

    void	enter_lupa_mode()
    {
        flag_lupa_mode = true ;
        setMouseTracking ( true ) ;
        rubberband_on = false;
        //crosshair_mode(false);
    }
    void prepare_paleta() ;
    void construct_lupa_cursor_bitmaps();

    bool is_rubberband_on()
    {
        return rubberband_on;
    }
    //     void (bool state)
    //     {
    //         rubberband_on = state;
    //     }
    /** No descriptions */
    void set_z_threshold ( int value ){ minZthreshold = value;  }

    /** No descriptions */
    int give_z_threshold()    { return minZthreshold; }
    /** No descriptions */
    void release_reservations();




private:
    void draw_lupa ( QBitmap &bitmapa, QColor color, int width, char plus_minus );
    void  select_nearest_vertex ( QMouseEvent *e );
    double prepare_font_size();


    // this function finds the vertex where we clicked. As ther result we need to have
    // 1) nr_of_banana where it was   ??  (we return it by argument !!!)
    // iterator to the vertex in this banana

    // polygon_type::iterator
    bool give_nearest_vertex ( QMouseEvent *e );
    polygon_type::iterator   it_of_moving_vertex ;
    bool flag_it_of_moving_vertex_legal ;  // if it was not succesful to find...

    // for making the rubberband in moving vertes, we want to store neighbours of the moved vertex
    QPoint p_previous, p_next ;
    bool flag_move_whole_polygon ;
    bool flag_move_one_vertex_of_polygon ;
    bool flag_move_selected_vertices_of_polygon ;

    void store_points_of_polygon ( int i );
    vector<QPoint> stored_polygon ;
    unsigned int nr_of_moved_polygon  ;   // which poligon in the spectrum is just chosen to be moven, deleted


    bool black_white; // colour of background

    //////////////////
    //QPoint origin_lupa;
    QRubberBand *rubberBand_lupa ;

    QPainterPath shape_moving_vertex;
    QPainterPath path_crosshair;

    void show_moving_flags(std::string txt )
    {
        cout << txt
             << " whole polygon = " << flag_move_whole_polygon
             << ", one vertex = " << flag_move_one_vertex_of_polygon
             <<  ", only selected vertices " << flag_move_selected_vertices_of_polygon
             << endl;

    }

    void select_all_vertices_in_rubberband_region();

public slots:
    void showMatrixContextMenu(QPoint pos);
};

#endif // box_of_matrix_h
