#include "box_of_matrix.h"
#include "tplate_spectrum.h"
#include "spectrum_2D.h"
#include <qstring.h>
#include <math.h>
#include <qpixmap.h>
#include <qcursor.h>
#include <qapplication.h>
#include <qbitmap.h>
#include <iostream>

//#undef COTO
//#define COTO

class spectrum_2D ;

vector<QColor> box_of_matrix::paleta ;  // static
#include "appl_form.h"
extern appl_form  *appl_form_ptr;
//*******************************************************************
box_of_matrix::box_of_matrix (
        //        double factor_left,
        //                               double factor_top,
        //                               double factor_right,
        //                               double factor_bottom,
        QWidget *a_parent, string name ) : QWidget ( a_parent) , swiatek()
{
    setWindowTitle(name.c_str() );
    setMouseTracking ( true );

    // remembering the factors
    //    f_left = factor_left;
    //    f_top = factor_top;
    //    f_right = factor_right;
    //    f_bottom = factor_bottom;

    minZthreshold = 1 ; // threshold to display z axisa

    parent = dynamic_cast< spectrum_2D* >( a_parent) ;
    // parent = ( spectrum_2D* ) a_parent ;

    matrix_ptr = nullptr ;
    printer = nullptr ;

    // // constructing a frame responsible for this scale
    // setPaletteBackgroundColor ( parent->give_flag_black_white() ?
    //               Qt::white
    //               :
    //               QColor (0, 0,40) );  // dark blue
    // setPaletteBackgroundColor ( QColor (0, 0,85) );  // <- blue marine

    prepare_paleta() ;

    ( QRect ( 50,50, 5, 5 ) ) ;

    flag_was_mouse_click = false ;
    flag_mouse_just_pressed = false;
    x_distance_to_vertex = 0;
    y_distance_to_vertex = 0;
    pixel_tolerance = 10;
    flag_banana_creating_mode = false;

    x_mouse_click = 0;
    y_mouse_click = 0;
    pressed_button_state = Qt::NoButton ;

    //--------------------------
    //setBackgroundColor( transparentColor );
    rubberband_on = false;
    flag_lupa_mode = false ;
    current_cursor =  is_usual ;
    flag_move_whole_polygon = false ;
    flag_move_one_vertex_of_polygon = false;
    flag_move_selected_vertices_of_polygon = false;

    // make also bitmaps for lupa cursor

    construct_lupa_cursor_bitmaps();

    // setFocusPolicy ( QWidget::WheelFocus) ; // - like StrongFocus plus the widget accepts focus by using the mouse wheel.

    // setFocus() ; // < -----  this instruction   made mdi worked wrongly with 2D specta (currentSubWindow was null)

    flag_crosshair_cursor = false;

    rubberBand_lupa = nullptr ;
}
//*********************************************************************
void box_of_matrix::prepare_paleta()
{
    // creating the rainbow palete

    if ( paleta.empty() ) // paleta is static, so we need to do it only once
    {
        // zero is black
        paleta.push_back ( QColor ( 0, 0, 0 ) );
        int cursor_red = 0 ;
        int cursor_green = 0;
        int cursor_blue = 255 ;

#define cccc    //  cout << "Now rgb will be " << cursor_red << " " << cursor_green << " " << cursor_blue << endl;
        for ( ; cursor_green < 256 ;  cursor_green += 4 )
        {
            cccc ;
            paleta.push_back ( QColor ( cursor_red, cursor_green, cursor_blue ) ); //  from blue to cyjan
        }
        paleta[0] = QColor ( 0 , 0, 127 ); //  half stron blue

        // first colour corrected to dark bule

        cursor_green = 255 ; // bec we left the loop when it was too high

        for ( ; cursor_blue >= 0 ; cursor_blue -=4 )
        {
            cccc;
            paleta.push_back ( QColor ( cursor_red, cursor_green, cursor_blue ) ); // cyjan to green
        }

        cursor_blue = 0 ;

        for ( ; cursor_red < 256 ; cursor_red += 4 )
        {
            cccc;
            paleta.push_back ( QColor ( cursor_red, cursor_green, cursor_blue ) ); // green to yellow
        }
        cursor_red = 255 ;


        for ( ; cursor_green >= 0 ; cursor_green -=4 )
        {
            cccc ;
            paleta.push_back ( QColor ( cursor_red, cursor_green, cursor_blue ) ); // yellow to red
        }

        //cout << "rozmiar palety = " << paleta.size() ;


    } // endif
}
//***********************************************************************
void box_of_matrix::paintEvent ( QPaintEvent * )
{
    // too often refreshes (3 times are in the beginning)
    // so here we install the cloct tu measure how long time was the last referesh

    //	static int counter;
    //	cout << __PRETTY_FUNCTION__ << " update box_matrix  " << (counter++) << endl;

    QPainter piorko_ekranowe ( this );
    draw_all ( &piorko_ekranowe );

}
//*****************************************************************************************
void box_of_matrix::draw_all ( QPainter * pior )
{
    // cout << __PRETTY_FUNCTION__ << endl;

    static int now_working;

    if ( now_working > 2 )
    {
        //cout << " should be rejected box_of_matrix::paintEvent" << endl;
        //        return ;

    }
    now_working++;
    //cout << "box_of_matrix::paintEvent, level " << now_working << endl;

    flag_screen_just_repainted = true;
    //asking for a geometry of the parent widget
    //    QSize size_of_window = parent->size() ;

    // matrix_widget *p = dynamic_cast<spectrum_widget*> parent ;
    spectrum_2D *p = ( spectrum_2D* ) parent ;


#if 0
    resize ( ( int ) ( ( f_right-f_left ) * size_of_window.width() ),
             ( int ) ( ( f_bottom-f_top ) * size_of_window.height() ) ); // restore size
    move ( ( int ) ( f_left * size_of_window.width() ),
           ( int ) ( f_top * size_of_window.height() ) );     // restore position
#endif

    QRect r = rect() ;

    //bool
    black_white = parent->give_flag_black_white() ;
    //cout << "przed rysowaniem" << endl;
    if(p->is_flag_error_while_reading() ) return;

    QPainter &piorko = *pior ;

    QBrush pedzel( black_white? Qt::white : QColor(0,0, 0), Qt::SolidPattern);
    piorko.fillRect(QRect(QPoint(0, 0), r.size()), pedzel   );   //?????????????????????

    // do we have to prepare the new pixmap ?
    if ( r.size() != pix.size()  || make_new_pixmap )
    {
        //        cout << "sizes of rectangle and pixmap are different, so we must draw all really "
        //                << " r.width = " << r.width() << ", r.hight=" << r.height()
        //             << ", pixmap pix.with" << pix.width() << ", pix.hight=" << pix.height()
        //             << endl;

        make_new_pixmap = false ;  // for future, in case if we escape out of loop


        // YES so we will make pixmap again
        if ( r.size() != pix.size() )
        {
            //            cout << "Resizing pixmap...." << flush ;
            //            pix = pix.copy(QRect(QPoint(0, 0), r.size()));  // qT3: pix.resize( r.size() );

            //            QPixmap tmp(r.size());
            pix =  QPixmap(r.size());


            //            cout << "Finished "
            //                    << " r.width = " << r.width() << ", r.hight=" << r.height()
            //                 << ", pixmap pix.with" << pix.width() << ", pix.hight=" << pix.height()
            //                 << endl;
        }

        QFont sansFont ( "Helvetica [Cronyx]" );

        //int rozm =(int) ( 0.1 * QMIN( r.width(), r.height()  ) )  ; // adjusting the size of the font to the size of the widget
        int rozm = 20    ;
        sansFont.setPixelSize ( rozm );

        sansFont.setStyleHint ( QFont::SansSerif );
        piorko.setFont ( sansFont );
        piorko.setPen ( black_white ? Qt::black :Qt::white); // prepering the pen

#if 0
        QString napis = QString ( "wait..." ) ;
        piorko.drawText ( r.left() + ( r.right() -r.left() ) /2,
                          r.top() + ( r.bottom() - r.top() ) /2 ,  napis );
#endif

        piorko.end();   // end of normal painting on the screen
        piorko.begin ( &pix ); // start to paint on the pixmap

        //cout << "Filling black...." << flush ;
        pix.fill ( black_white ?  Qt::white: Qt::black ); // so - no data, we do not have to draw anything

        // drawing a sunken frame --------------------------
        piorko.setPen ( black_white ? Qt::black : Qt::darkGray );
        piorko.drawLine ( r.bottomLeft(), r.topLeft() );
        piorko.drawLine ( r.topLeft(), r.topRight() );
        piorko.setPen ( black_white ? Qt::black :Qt::white);
        piorko.drawLine ( r.topRight(), r.bottomRight() );
        piorko.drawLine ( r.bottomRight(), r.bottomLeft() );

        // drawing a contents -----------------------------

        //         cout << "in matrix before World setting  x "
        //             <<  minX << "-"
        //             << maxX
        //             <<  "  Y= " << minY
        //             << "-" <<  maxY
        //             << endl ;

        piorko.setPen ( black_white ? Qt::black :Qt::white); // prepering the pen


        // Note: if we have to draw channels  0-56 (inclusive), so the the last channel will start at 56 and finish at 56.9999
        // so the real range of picture should be  min_y : max_y+waga_y
        ustaw_swiat ( r.left(), r.top(), r.right(), r.bottom(), // preparing the coordinates
                      minX,
                      maxY , // + specif->waga_y,
                      maxX, //  + specif->waga,
                      minY );

        //        cout << "w box_matr dol = " << minY << ", gora " << (maxY+specif->waga_y)
        //          << ", ( bo maxY = " <<  maxY << " +specif->waga_y" << specif->waga_y  << endl ;

        //QFont sansFont("Helvetica [Cronyx]");

        //int rozm =(int) ( 0.1 * QMIN( r.width(), r.height()  ) )  ; // adjusting the size of the font to the size of the widget
        //int rozm = 20    ;
        //sansFont.setPixelSize(rozm);

        //sansFont.setStyleHint( QFont::SansSerif );
        piorko.setFont ( sansFont );

        vector<int> & widmo =  *matrix_ptr ;

        //            cout << "in matrix draw_all,    Specif -------\n"
        //             << "specif.beg=" << specif->beg
        //         << " specif.end=" << specif->end
        //         << " specif.bin= " << specif->bin
        //         << " specif.waga=" <<  specif->waga
        //         << "\nspecif.beg_y=" << specif->beg_y
        //         << " specif.end_y=" << specif->end_y
        //         << " specif.bin_y= " << specif->bin_y
        //         << " specif.waga_y=" <<  specif->waga_y << endl;

        int first_drawn_channel = ( int ) ( ( minX - specif->beg ) / specif->waga )  ;
        int last_drawn_channel  = ( int ) ( ( maxX - specif->beg ) / specif->waga ) ;

        int first_drawn_row = ( int ) ( ( minY - specif->beg_y ) / specif->waga_y )  ;
        int last_drawn_row  = ( int ) ( ( maxY - specif->beg_y ) / specif->waga_y ) ;


        // by sliders etc, somebody might to draw something out of matrix
        first_drawn_channel = max ( first_drawn_channel, 0 ) ;
        last_drawn_channel  =min ( last_drawn_channel, ( int ) specif->bin-1 ) ;
        first_drawn_row = max ( first_drawn_row, 0 ) ;
        last_drawn_row  =min ( last_drawn_row, ( int ) specif->bin_y-1 ) ;

        //        cout << "first_drawn_row = " << first_drawn_row
        //         << "last_drawn_row =" <<  last_drawn_row  << endl;

        double waga = specif->waga ;
        double beg = specif->beg ;
        double row_length = specif->bin ;

        double waga_y = specif->waga_y ;
        double beg_y = specif->beg_y ;


        //int max_counts_in_matrix = p->give_max_counts() - (minZ-1);
        int maxZ = p->give_max_counts();
        unsigned int diffZ = maxZ - minZthreshold;  // if zero?
        if ( diffZ < 1 ) diffZ = 1 ;

        //   cout << "---------------------before loop drawing a range x = "
        //    << first_drawn_channel << ", " << last_drawn_channel
        //    << ", range y = " << first_drawn_row << ", " << last_drawn_row
        //    << " while width= " << r.width()
        //    << endl ;
        //


        double log_helper = 0 ;
        if ( p->give_flag_log_scale() && diffZ )
        {

            log_helper = 255.0 / log10 ( ( double ) diffZ );
        }

        // if the picture covers big range - we can save time by drawing only points #########
        // if the picture magnifies - we draw squares as pixels#########


        // normal drawing with the coulored RECTANGLE PIXELS ################################
        if ( r.width() >= ( last_drawn_channel - first_drawn_channel )
             ||
             r.height() >= ( last_drawn_row - first_drawn_row )
             )
        {

            // cout << "This is a rectangle drawing  "<< endl;
            // to make it faster
            int szer = relx ( waga ) +1; // +1 to avoid holes in case is 255 matrix is printed on 280 pixels
            int wys = rely ( waga_y ) +1 ;
            //cout << " wys = "<< wys  << " while waga_y = " << waga_y
            // << " szer = "<< szer  << " while waga = " << waga
            // << endl;



            // loops goes on real channels !----------------------------------------
            for ( int row = first_drawn_row ; row <= last_drawn_row; row ++ )
            {

                int row_calculated = wy ( ( row ) * waga_y + beg_y ) ;

                for ( int i = first_drawn_channel ; i <= last_drawn_channel ; i ++ )
                {
                    //      cout << "Darwing pixel [" << row << "][" << i << "]" << endl ;
                    /// drawing a box
                    /// int color = ( !(i%2)) ? i : 0 ;

                    int color = widmo[ ( int ) ( row * row_length ) + i ] - ( minZthreshold-1 ) ;

                    //                         cout << "size if widmo " << widmo.size()
                    //                           << ", adressed " << (row * row_length) + i << endl;

                    if ( color > 0 ) // no color = we do not draw anything
                    {
                        //color++ ; // this trick is to draw weakest points

                        //           (in color dirrerent than empty space)
                        if ( p->give_flag_log_scale() )
                        {
                            if ( color ) // do not make logarithm of zero , just draw color 0 (black)
                            {
                                //cout << "this is a log scale, color is " << color << endl;
                                //double tmp =  log10(color) ;
                                //cout << "logharitm of this is  is " << tmp << endl;
                                // moze byc dzielenie przez zero
                                //if(color != 1)
                                color = static_cast<int> ( log10 ( ( double ) color ) * log_helper )  ;

                                //cout << "so finally ---------in the palete, this will be " << color << endl;
                            }// end of if color

                        }
                        else
                        {
                            // this is NOT a LOG scale, classical linear scale
                            if ( diffZ== 0 )
                            {
                                cout << "divide by zero in colours 2D !" << endl ;
                            }
                            else
                            {
                                //if(color != 1)
                                color = color * 255 / diffZ ;
                            }
                        }

                        //cout << "color = " << color ;

                        if ( color <= 0 ) color = 1 ; // to draw even weakest points !

                        QColor  paleta_helper =paleta[min ( color, 255 ) ]  ;
                        piorko.setBrush ( paleta_helper ) ;

                        //piorko.setBrush(paleta[min(color, 255) ] ) ;

                        piorko.fillRect (
                                    wx ( ( i ) * waga + beg ), row_calculated,
                                    szer,
                                    -wys ,
                                    piorko.brush() ) ;
                        /*----
                        if(row == last_drawn_row)
                        {
                        piorko.drawRect (wx( (i)* waga + beg  ), row_calculated,
                            szer/2, -wys/2 );
                        }
                        piorko.setPen(white ) ;
                        piorko.drawPoint (wx( (i)* waga + beg  ),  wy( (row)* waga_y + beg_y)) ;
                        //--
                        if(row == i)
                        {
                        piorko.drawRect (wx( (i)* waga + beg  ), row_calculated,
                            szer/2, -wys/2 );

                        piorko.setPen(white ) ;
                        if(!(i%10)) piorko.setPen(red ) ;
                        piorko.drawPoint(wx( (i)* waga + beg  ), row_calculated);

                        }
                        -----------------*/
                    }// end if color
                }
            }
            //cout << "----------------------------skonczona petla rysujaca" << endl;


        }
        else   // ############# fast simple points instead of rectangle pixels ###########################
        {
            //cout << "Pixel drawing only " << endl;

            for ( int row = first_drawn_row ; row <= last_drawn_row ; row ++ )
            {

                int row_calculated = wy ( ( row ) * waga_y + beg_y ) ;
                for ( int i = first_drawn_channel ; i <= last_drawn_channel ; i ++ )
                {

                    //                    cout << "Darwing pixel [" << row << "][" << i << "]";   //  << endl ;
                    //                    cout << "colour comes from widmo["<< (( int ) ( row * row_length ) + i) << "]"
                    //                         << ", while widmo.size = " << widmo.size() << endl;

                    int color = widmo[ ( int ) ( row * row_length ) + i ] - ( minZthreshold-1 );
                    if ( color > 0 )
                    {
                        //color++ ; // this trick is to draw weakest points
                        //           (in color dirrerent than empty space)

                        if ( p->give_flag_log_scale() )
                        {
                            //cout << "this is a log scale, color is " << color << endl;
                            //double tmp =  log10(color) ;
                            //cout << "logharitm of this is  is " << tmp << endl;
                            // moze byc dzielenie przez zero
                            color = static_cast<int> ( log10 ( ( double ) color ) * log_helper )  ;

                            //cout << "so finally ---------in the palete, this will be " << color << endl;

                        }
                        else
                        {
                            // not log, classical
                            if ( !diffZ ) cout << "dividing by zero in: colours 2D" << endl ;
                            color = color * 255 / diffZ ;
                        }


                        if ( color <= 0 )
                        {
                            color = 1 ; // to draw even weakest points !
                        }
                        piorko.setPen ( paleta[min ( color, 255 ) ] )  ;
                        //piorko.setPen ( paleta[color%256] ) ;
                        int gdzie = wx ( ( i ) * waga + beg )  ;
                        piorko.drawPoint ( gdzie,  row_calculated );
                        //piorko.moveTo( gdzie,  row_calculated);
                        //piorko.lineTo(  (gdzie+1),  row_calculated);
                    }
                }

            }

        }// ############################################################################

    } // end of "do we have to prepare pixmap "
    else
    {
        //cout << "sizes of rectangle and pixmap are equall, so just pin it up" << endl ;
    }
    piorko.end();
    // cout << "pin up of the bitmap rysowaniem" << endl;
    piorko.begin ( this );
    piorko.drawPixmap ( r.topLeft(), pix );

    //    cout << "Drawing in a range " << minX << "-" << maxX
    //      << ", waga " << waga
    //      << " specif bins= " << specif->bin
    //       <<  ", specif->beg" << specif->beg
    //      << ", specif->end=" << specif->end
    //      << ", first_drawn_channel=" <<  first_drawn_channel
    //      << ", last_drawn_channel=" <<   last_drawn_channel
    //      << endl ;


    //    static int counter ;
    //    cout << "Refreshing the specttrum " << p->give_spectrum_name() << "  " << counter ++ << endl;
    auto font_size = prepare_font_size();

    draw_pinup_infos ( piorko, font_size );
    draw_bananas ( piorko );

    // RUBBER -------------

    if(flag_move_whole_polygon || flag_move_one_vertex_of_polygon || flag_move_selected_vertices_of_polygon)
    {
        // erase old shape

        // draw new shape
        piorko.drawPath(shape_moving_vertex);

    }
    else{
        if(shape_moving_vertex.isEmpty() == false)
            shape_moving_vertex = QPainterPath();

    }

    if(flag_crosshair_cursor)
    {
        piorko.setPen ( black_white ? Qt::black :Qt::cyan); // prepering the pen
        piorko.drawPath(path_crosshair);
    }
    else
    {
        path_crosshair = QPainterPath();
    }

    // end of RUBBER -----------------------------------------------------------------------


    //    if ( flag_banana_creating_mode ) // while user clicks verices of the new banana
    //        {
    //            // draw new banana linear
    //            draw_lines_of_new_banana();
    //    }


    // this below you may see while clicking the right button of the mouse
    // cout << "Integrtion of this part of the picture is : " << integral << endl;
    now_working--;

}
//***********************************************************************
void box_of_matrix::draw_pinup_infos ( QPainter & piorko, int font_size )
{
    // pinup infos ====================
    // Pinup with the general  info  (about the gates required (dark yellow))



    piorko.setPen ( QPen ( black_white ? Qt::black : Qt::darkYellow, 2, Qt::SolidLine ) ); // preparing the pen

    QFont sansFont ( "Sans", font_size );
    piorko.setFont ( sansFont );

    // calculating the position of notice
    double x_notice = minX + 0.03 * ( maxX- minX ) ;
    double y_notice = maxY - ( 0.1* ( maxY - minY ) ) ;

    parent->nalepka_notice.set_xy ( x_notice,  y_notice );
    piorko.setFont ( sansFont);
    parent->nalepka_notice.draw ( this, &piorko, false );


    // -----------
    if ( minZthreshold > 1 ) // this is a rebining factor
    {
        piorko.setPen ( QPen ( black_white ? Qt::black : Qt::red, 2, Qt::SolidLine ) ); // preparing the pen
        QString inf = QString ( "%1 counts thresh." ).arg ( minZthreshold ) ;
        parent->z_threshold_notice.set_info ( inf.toStdString() ) ;
        // calculating the position of notice
        double x_notice2 = minX + 0.8 * ( maxX- minX ) ;
        double y_notice2 = maxY - ( 0.15* ( maxY - minY ) ) ;
        parent->z_threshold_notice.set_xy ( x_notice2,  y_notice2 );

        //sansFont.setPixelSize(12);
        //  piorko.setFont(sansFont);
        parent->z_threshold_notice.draw ( this, &piorko, false ); // never care about log scale
    }

    //----------- other - user definied notices -----------------
    piorko.setPen ( QPen ( black_white ? Qt::black :  Qt::cyan, 2, Qt::SolidLine ) ); // preparing the pen

    sansFont.setPixelSize ( font_size);  // was: 15
    piorko.setFont ( sansFont );

    for ( unsigned int i = 0 ; i < parent->nalepka.size() ; i ++ ) // drawing all infos
    {
        parent->nalepka[i].draw ( this, &piorko, false );           // never care about log scale
    }

}
//***********************************************************************
void box_of_matrix::draw_bananas ( QPainter & piorko )
{

    // drawing bananas --------------------------------------------------------

    if ( parent->give_flag_show_polygons() )
    {
        vector<polygon_gate>&  banana = parent->give_vector_of_polygons();
        for ( unsigned int i = 0 ; i < banana.size() ; i++ )
        {

            //      cout << "we are going to draw banana called " <<  banana[i].name  << endl; // this is a name

            polygon_type::iterator it ;
            [[__maybe_unused__]]  int nr = 0 ;

            for ( it = banana[i].polygon.begin() ; it != banana[i].polygon.end() ;  nr++ )
            {
                // here we are jumping through coordinates
                //        cout << "This is the vertex nr " << nr
                //          << " [ "
                //          << (*it).x   // x
                //          << ", "
                //          << (*it).y
                //          << "]"
                //          << endl ;
                //
                int xx = wx ( ( * ( it ) ).x ) ;
                int yy = wy ( ( * ( it ) ).y ) ;
                QPainterPath skok;
                skok.moveTo  ( xx, yy ) ;         // x1 y1

                // drawing a rect here
                piorko.setPen ( QPen ( black_white ? Qt::black : Qt::white, 1, Qt::SolidLine ) ); // preparing the pen
                piorko.drawRect ( xx -4, yy -4 , 8, 8 );
                if ( ( * ( it ) ).selected )
                {
                    // mark it as filled rectangle
                    //piorko.setBrush(white ) ;
                    piorko.fillRect ( xx -4, yy -4 , 8, 8 , QBrush (Qt::white) );
                    piorko.setPen ( QPen ( Qt::red, 1, Qt::SolidLine ) ); // preparing the pen
                    piorko.drawRect ( xx -4, yy -4 , 8, 8 );


                }

                it++ ;
                if ( ( it ) != banana[i].polygon.end() )
                {
                    piorko.setPen ( QPen ( black_white ?Qt::white: Qt::white, 3, Qt::DashLine ) );
                    skok.lineTo ( wx ( ( * ( it ) ).x )   ,    wy ( ( * ( it ) ).y ) ) ;   //x2 y2
                    piorko.drawPath(skok);

                    // same in red, width of line is 1 (narrow)
                    piorko.setPen ( QPen ( black_white ? Qt::black : Qt::red,   1, Qt::DashLine ) );
                    piorko.drawPath(skok);

                }
                else
                {
                    // this is drawing the last vertex so join to the first
                    piorko.setPen ( QPen ( black_white ?Qt::white: Qt::white, 3, Qt::DotLine ) );

                    QPainterPath skokfin;
                    skokfin.moveTo ( xx, yy ) ;
                    skokfin.lineTo ( wx ( ( *banana[i].polygon.begin() ).x ),
                                     wy ( ( *banana[i].polygon.begin() ).y ) ) ;         //x2 y2
                    piorko.drawPath(skokfin);

                    // same in red, but narrow
                    piorko.setPen ( QPen ( black_white ? Qt::black : Qt::red, 1, Qt::DotLine ) );
                    piorko.drawPath(skokfin);


                    // writing the name, but without the full path  ---------------
                    string::size_type pos = banana[i].screen_name.rfind ( "/" ) ;
                    string nnn ;
                    if ( pos != string::npos )
                    {
                        nnn = banana[i].screen_name.substr ( pos+1 ) ;
                        //cout << "name cleaned " << endl ;

                    }
                    else nnn =banana[i].screen_name ;

                    if ( parent ->give_flag_show_disk_names_of_gates() )
                    {
                        //cout << "bedzie nazwa dyskowa " << endl ;
                        nnn = banana[i].disk_name ;
                    }
                    else
                    {
                        //cout << "bedzie nazwa ekranowa " << endl ;
                    }
                    //-------------------------
                    piorko.drawText ( wx ( ( *banana[i].polygon.begin() ).x )  +5    ,  // +5 is to move away from rectangle
                                      wy ( ( *banana[i].polygon.begin() ).y ),
                                      ( nnn.c_str() )
                                      );

                }

            } // end of for iterator

            // drawing banana


        } // end of drawing all bananas
    }// endof if flag polygons
    if ( flag_banana_creating_mode )
    {
        draw_lines_of_new_banana();  // in case if we are just drawing
    }

}

//***********************************************************************
void box_of_matrix::set_spectrum_pointer ( vector<int> * ptr, spectrum_descr * d )
{
    matrix_ptr = ptr ;
    specif = d ;
}
//***********************************************************************
void   box_of_matrix::mousePressEvent ( QMouseEvent * e )
{
    // we set the values
    flag_was_mouse_click = true ;
    flag_mouse_just_pressed = true ;


    x_mouse_click = typ_x ( pix2worX (
        #if  (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
        e->position().x()
#else
        e->x()
#endif
        ) );


    //y_mouse_click = int (pix2worY(e->y() ) ) ;


    y_mouse_click = typ_x ( pix2worY ( e->
#if  (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
                                   position().
#endif
                                   y() ) ) ;

    pressed_button_state = e->button() ;

    //     cout << "In mousePressEvent,   Pixl=" << e->x()
    //      << "  means channel =" <<  pix2worX(e->x() )
    //      << endl ;


    // if this is a double click - make zoom quickly

    if ( e->type() ==QEvent::MouseButtonDblClick ) //------------------------------------------------
    {

        if ( flag_banana_creating_mode )
        {
            cout << "  - This was doubleclick for finishing banana" << endl ;
            finish_drawing_new_banana();
            flag_mouse_just_pressed = false;
            e->accept();
            update();
            return;
        }
        //cout << " Instant  zoom " << endl ;


#if 1   // My SUSE behaves strange recognising wrongly DOUBLE CLICK
        // so, for now - I blocked this

        QPoint p1 = e->pos();
        typ_x   x1 = typ_x ( pix2worX ( p1.x() ) );
        typ_x   y1 = typ_x ( pix2worY ( p1.y() ) ) ;

        // ranges of current picture are  minX, maxX, minY, maxY ;
        double magn_factor = 2 ;

        // if shift was pressed also, UNzoom
        if ( e->modifiers ()  & Qt::ShiftModifier  )
        {
            //   cout << "shift was also pressed " << endl ;
            magn_factor = 0.5 ;
        }


        typ_x  new_width = fabs ( maxX - minX ) / magn_factor ;
        typ_x  new_height = fabs ( maxY - minY ) / magn_factor ;

        // if this are legal values (fitting into the matrix) will be decided by function expand

        parent->expand ( ( x1 - new_width /2 )  ,
                         ( y1 + new_height / 2 ) ,
                         ( x1 + new_width / 2 ),
                         ( y1 - new_height /2 ) ) ;

        e->accept ();
#endif // 0


    } // endof double click

    else{     // NOT Doulble click ---------------------------------------------------------------------------------
        //        cout << "  - single click " << endl;
    }

    // if it is right click - send it to the parent (because it is the popup menu) --------------------
    if ( e->button() == Qt::RightButton )    // -----------
    {
        //         cout << " this is right button pressed " << endl;
        shape_moving_vertex = QPainterPath();
        e->ignore (); // <-- send it to the parent
        return;
    }

    // Just normal left button click -------------------------------------------------------------------------------

    if ( flag_lupa_mode )
    {
        appl_form_ptr->send_to_statusbar(
                    "Tip: To ZOOM IN a matrix - Double click, to ZOOM OUT a matrix -   Shift+Double Click",
                    15*1000 ); // 15 seconds on the screen

        QApplication::restoreOverrideCursor();
        setCursor ( QCursor ( Qt::SizeFDiagCursor ) ) ;

        // for rubberband purposes
        starting_pt = e->pos();
        current_end_pt = starting_pt;
        previous_end_pt = starting_pt;

        rubberband_on = true;
        setMouseTracking ( true );


        //origin_lupa = e->pos();

        // library rubberband class
        if (!rubberBand_lupa)
            rubberBand_lupa = new QRubberBand(QRubberBand::Rectangle, this);

        QPalette pal;
        pal.setBrush(QPalette::Highlight, QBrush(Qt::red));
        rubberBand_lupa->setPalette(pal);

        rubberBand_lupa->setGeometry(QRect(starting_pt, QSize()));
        rubberBand_lupa->show();
    }

    // perhaps user clicks new verices of the creating banana  --------------------
    else if ( flag_banana_creating_mode )
    {
        new_banana_points.push_back ( e->pos() );
        // draw new banana linear - can be called from paintEvent (draw_all function)
        // not from here !  draw_lines_of_new_banana();
        parent->flag_repaint_spectrum_box = true;
        update();
    }

    else   // NON LUPA, NON new vertices of new banana -----------------------------------------
    {

        // this can be dragging the nearest vertex of the polygon gate
        if ( e->button() & Qt::RightButton )
        {
            rubberband_on = false;
            e->ignore ();   // so it will go to the parent
        }
        else
        {

            // cout << "mousePressEvent( QMouseEvent * e ) " << endl;
            // select_nearest_vertex ( e );

            flag_move_one_vertex_of_polygon = false;
            flag_move_selected_vertices_of_polygon = false;
            flag_move_one_vertex_of_polygon = false;

            // for moving
            if ( give_nearest_vertex ( e ) ) // if true
            {

                if (e->modifiers ()  & Qt::ControlModifier)
                {
                    if(	e->modifiers ()  & Qt::ShiftModifier )
                    {
                        // select_nearest_vertex ( e );
                        setCursor ( QCursor ( Qt::SizeAllCursor ) ) ;
                        flag_move_selected_vertices_of_polygon = true;
                    }
                    else // no addtional shift
                    {
                        // select_nearest_vertex (e);
                        flag_move_whole_polygon = true ;
                        setCursor ( QCursor ( Qt::PointingHandCursor ) ) ;
                    }
                }
                else   // no controll modifier -------------------------
                {
                    // just normal selection of vertex
                    select_nearest_vertex ( e );
                    appl_form_ptr->send_to_statusbar("Tip: To move the whole polygon, use Shift+Click and drag it, only selected vertices - Ctrl+shift", 15000 );
                    setCursor ( QCursor ( Qt::PointingHandCursor ) ) ;
                    flag_move_one_vertex_of_polygon = true;


                }


                // for rubberband purposes
                starting_pt = e->pos();
                current_end_pt = starting_pt;
                previous_end_pt = starting_pt;
                //         rubberband_on = true;
                //         setMouseTracking( true );


            } // end of found nearest vertex
            else
            {
                flag_mouse_just_pressed = false;
                //				cout << "pressed in a neutral place " << endl;

                // nowe
                starting_pt = e->pos();
                current_end_pt = starting_pt;
                previous_end_pt = starting_pt;
                flag_marking_selected_vertices_region_mode = true;

                if(e->modifiers ()  & Qt::ShiftModifier)
                    flag_deselect_all_vertices_outside = false;
                else {
                    flag_deselect_all_vertices_outside = true;
                }
                if (!rubberBand_lupa)
                {
                    // cout << " need to make new rubberband for marking vertices" << endl;
                    rubberBand_lupa = new QRubberBand(QRubberBand::Rectangle, this);
                }
                QPalette pal;
                pal.setBrush(QPalette::Highlight, QBrush(Qt::red));
                rubberBand_lupa->setPalette(pal);

                rubberBand_lupa->setGeometry(QRect(starting_pt, QSize()));
                rubberBand_lupa->show();
            }

            // show_moving_flags("mousePress event: ");
            update();
        }
    }

}
//***********************************************************************

//***********************************************************************
//***************************************************************************
void  box_of_matrix::mouseMoveEvent ( QMouseEvent * e )
{
    //         cout << "Mouse move event dla widgeta box_matrix" <<  endl ;

    {
        // showing the position in the "Tip" area
        double real_x = pix2worX ( e->pos().x() );
        double real_y = pix2worY ( e->pos().y() );
        int contents =    parent->give_value_of_pixel ( real_x,real_y );

        if ( contents >=0 )
        {
            //             QString wydruk ;
            //             wydruk=
            appl_form_ptr->send_to_statusbar(
                        QString ( "Matrix  X: %1  Y: %2  cnt: %3    %4" ).
                        arg ( real_x, 10 )
                        .arg ( real_y, 10 )
                        .arg ( contents,10 )
                        .arg( parent->give_spectrum_name().c_str() ).toStdString(),
                        10*1000 ); //  seconds on the screen
        }
    }

    if ( flag_banana_creating_mode )
    {
        e->accept();
        return;
    }

    if ( flag_mouse_just_pressed )
    {
        //     cout << " this is just the  first move after pressing " << endl;
        flag_mouse_just_pressed = false;
        rubberband_on = true;
        setMouseTracking ( true );
    }

    if ( flag_lupa_mode ) // LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
    {
        if ( rubberband_on ) // RUBBER already activated by the first click
        {

            if(rubberBand_lupa)
                rubberBand_lupa->setGeometry(QRect(starting_pt, e->pos()).normalized());

            current_end_pt = e->pos();

            previous_end_pt = current_end_pt;

        }
        else //  RUBBER not yet on screen, we are just positioning cursor to start lupa
        {
            // THIS WORKS !!!!!!!!!!!!!!
            // kursory lupy z + lub - sa wazne do momentu wejscia w rubberband mode

            if ( e->modifiers ()  & Qt::ShiftModifier )
            {
                // to not to change cursor into itself
                if ( current_cursor != is_lupa_minus )
                {
                    // cout << " kursor lupa z minusem " << endl ;
                    setCursor ( QCursor ( lupa_minus, lupa_minus_maska ) ) ;
                    current_cursor = is_lupa_minus ;
                }
            }
            else
            {
                //cout << " kursor lupa z plusem " << endl ;
                if ( current_cursor != is_lupa_plus ) // to not to change cursor into itself
                {
                    setCursor ( QCursor ( lupa_plus, lupa_plus_maska ) ) ;
                    current_cursor = is_lupa_plus ;
                }
            } // end of if shift

        } // end of else NOT rubberband RRRRRRRRRRRRRRRRRRRRR
    } // end of lupa mode LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL

    // Perhaps it is just marking the region of selected vertices of polygon?
    if(flag_marking_selected_vertices_region_mode)
    {

        if(rubberBand_lupa)
            rubberBand_lupa->setGeometry(QRect(starting_pt, e->pos()).normalized());
        current_end_pt = e->pos();
        previous_end_pt = current_end_pt;
    }

    //-----------------------------------
    if ( flag_crosshair_cursor )
    {
        crosshair_handler ( e );
    }

    //else // normal situation  of moving the vertex

    if ( ! flag_lupa_mode ) moving_polygon_vertex_handler ( e );

    // end of work
    flag_screen_just_repainted = false;


    //cout << "box_of_matrix:: end of mouse move" << endl;
    e->accept();
    update();
}
//************************************************************************
void  box_of_matrix::mouseReleaseEvent ( QMouseEvent* )
{
    // COTO;
    //    rubberBand->hide();
    // determine selection, for example using QRect::intersects()
    // and QRect::contains().


    //   cout << "Mouse release event dla widgeta box_matrix" << endl ;
    flag_mouse_just_pressed = false;
    flag_crosshair_mode_just_started = true;

    if ( flag_lupa_mode )
    {
        rubberBand_lupa->hide();
        // determine selection, for example using QRect::intersects()
        // and QRect::contains().

        //        if ( rubberband_on )
        //        {
        //            QPainter p ( this );
        //            p.eraseRect ( rect() );
        //        }
        rubberband_on = false;
        flag_lupa_mode = false ;
        //crosshair_mode ( false );
        //setMouseTracking ( false );


        // here make a reaction for zooming or for moving a vertex of the polygon

        // now we can return the coordinates of all the points
        typ_x   x1 = typ_x ( pix2worX ( starting_pt.x() ) );
        typ_x   y1 = typ_x ( pix2worY ( starting_pt.y() ) ) ;

        typ_x   x2 = typ_x ( pix2worX ( previous_end_pt.x() ) );
        typ_x   y2 = typ_x ( pix2worY ( previous_end_pt.y() ) ) ;

        // p1 and p3 have the coordinates of the magnifying region

        // if somebody just clicked there, so that the region would be too small
        // so we mage general magnification aroung this point
        make_new_pixmap = true ;
        if ( ( starting_pt - current_end_pt ).isNull() )
        {
            //      cout << " nearly Null zoom " << endl ;
            // ranges of current picture are  minX, maxX, minY, maxY ;
            double magn_factor = 2 ;
            if ( current_cursor != is_lupa_plus ) magn_factor = 0.5 ;
            typ_x  new_width = fabs ( maxX - minX ) / magn_factor ;
            typ_x  new_height = fabs ( maxY - minY ) / magn_factor ;

            // if this are legal values (fitting into the matrix) will be decided by function expand

            parent->expand ( ( x1 - new_width /2 )  ,
                             ( y1 + new_height / 2 ) ,
                             ( x1 + new_width / 2 ),
                             ( y1 - new_height /2 ) ) ;
        }
        else // non nearly null zoom
        {
            //            cout << "new points are x1,2 = "
            //              << x1 << " " << x2
            //              << " y1,2 = " <<  y1  <<  ", "
            //              <<  y2 << endl;
            parent->expand ( min ( x1, x2 ) ,
                             max ( y1, y2 ) ,
                             max ( x1, x2 ) ,
                             min ( y1, y2 ) ) ;

        }
        make_new_pixmap = true ;
        unsetCursor ();
        //setMouseTracking ( false ) ;
        current_cursor = is_usual ;
        //update() ;
    }
    else if ( flag_banana_creating_mode )
    {}
    else
    {
        // not lupa, not creating new banana - so perhaps it was moving the vertex ----------------

        if ( rubberband_on && ( flag_move_whole_polygon || flag_move_one_vertex_of_polygon  || flag_move_selected_vertices_of_polygon) )
        {
            //       cout << "This is rubberband " << endl;
            //            QPainter p ( this );
            //            p.eraseRect ( rect() );

            // if ( flag_crosshair_cursor == false )
            rubberband_on = false;
            //setMouseTracking ( false );

            // here make a reaction for zooming or for moving a vertex of the polygon

            // now we can return the coordinates of all the points
            typ_x   x1 = typ_x ( pix2worX ( starting_pt.x() ) );
            typ_x   y1 = typ_x ( pix2worY ( starting_pt.y() ) ) ;

            // p1 and p3 have the coordinates of the magnifying region

            // if somebody just clicked there, so that the region would be too small
            // so we mage general magnification aroung this point
            //      cout << "starting point was x1,2 = "
            //        << x1 << " " << y1
            //        << " new is  = " <<  x2  <<  ", "
            //        <<  y2 << endl;

            vector<polygon_gate>&  banana = parent->give_vector_of_polygons();

            if ( nr_of_moved_polygon >= banana.size() )
            {
                //             cout << "Illegar nr of polygon, in "
                // 				     << __FILE__ << " line = " << __LINE__ << endl;
                return;
            }
            // for undo purposes ----------------------------
            undo_banana_list.push_front ( banana[nr_of_moved_polygon] ) ;
            if ( undo_banana_list.size() > 25 )
            {
                undo_banana_list.pop_back() ;
            }
            //------------------------------------------------

            if ( flag_move_whole_polygon )
            {
                typ_x   x2 = typ_x ( pix2worX ( previous_end_pt.x()  /*- x_distance_to_vertex*/ ) );
                typ_x   y2 = typ_x ( pix2worY ( previous_end_pt.y()  /*- y_distance_to_vertex*/ ) ) ;

                typ_x xdiff  = x2 - x1  ;  // not pixels ! just world
                typ_x ydiff  = y2 - y1  ;

                //    cout << "Moved by differnc  " << xdiff << " " << ydiff << endl ;
                //                COTO;
                it_of_moving_vertex = banana[nr_of_moved_polygon].polygon.begin() ;
                //                COTO;

                for ( ; it_of_moving_vertex !=banana[nr_of_moved_polygon].polygon.end() ;  it_of_moving_vertex++ )
                {
                    it_of_moving_vertex->x  += xdiff ;
                    it_of_moving_vertex->y  += ydiff ;
                }

                flag_move_whole_polygon = false ;
            }
            else if(flag_move_selected_vertices_of_polygon)
            {
                typ_x   x2 = typ_x ( pix2worX ( previous_end_pt.x()  /*- x_distance_to_vertex*/ ) );
                typ_x   y2 = typ_x ( pix2worY ( previous_end_pt.y()  /*- y_distance_to_vertex*/ ) ) ;

                typ_x xdiff  = x2 - x1  ;  // not pixels ! just world
                typ_x ydiff  = y2 - y1  ;

                //    cout << "Moved by differnc  " << xdiff << " " << ydiff << endl ;
                //                COTO;
                it_of_moving_vertex = banana[nr_of_moved_polygon].polygon.begin() ;
                //                COTO;

                for ( ; it_of_moving_vertex !=banana[nr_of_moved_polygon].polygon.end() ;  it_of_moving_vertex++ )
                {
                    if(it_of_moving_vertex->selected )
                    {
                        it_of_moving_vertex->x  += xdiff ;
                        it_of_moving_vertex->y  += ydiff ;
                    }
                }

                flag_move_selected_vertices_of_polygon = false ;
            }
            else if ( flag_move_one_vertex_of_polygon ) //  normal, just one vertex is moved ---------------
            {
                //         cout << " Normal moving while release " << endl;
                typ_x   x2 = typ_x ( pix2worX ( previous_end_pt.x()  - x_distance_to_vertex ) );
                typ_x   y2 = typ_x ( pix2worY ( previous_end_pt.y()  - y_distance_to_vertex ) ) ;

                // if it was creating the new gate, so  the nothing sensible is in it_of_moving_vertex
                if ( flag_it_of_moving_vertex_legal ) // non NULL
                {
                    it_of_moving_vertex->x = x2 ;
                    it_of_moving_vertex->y = y2;;
                }
                flag_move_one_vertex_of_polygon = false;
            }

            banana[nr_of_moved_polygon].save_to_disk() ;
            flag_move_one_vertex_of_polygon = false;
            flag_move_selected_vertices_of_polygon = false;
            flag_move_one_vertex_of_polygon = false;
        } // end of rubberball

        if(flag_marking_selected_vertices_region_mode)
        {

            // cout << "now we can make selection of vertices, which are inside the rubberband" << endl;
            rubberBand_lupa->hide();

            select_all_vertices_in_rubberband_region();
            flag_marking_selected_vertices_region_mode = false;
            flag_deselect_all_vertices_outside = false;
        }

        unsetCursor ();
        current_cursor = is_usual ;
        //        update() ;

    }
    setMouseTracking ( true );
    update();
    //COTO;
}
//*****************************************************************************
void box_of_matrix::keyPressEvent ( QKeyEvent * e )
{
    // state()   Returns the keyboard modifier flags that existed immediately before the event occurred.
    //The returned value is ShiftButton, ControlButton, AltButton and MetaButton OR'ed together.

    //  cout << "Keypressed event in box of matrix  =" << hex << e->stateAfter () << endl ;

    if ( flag_lupa_mode )
    {
        //cout << "(we are in lupa mode)  " << endl ;
        if ( e->modifiers ()  & Qt::ShiftModifier)
        {

            // cout << " kursor lupa z minusem " << endl ;
            setCursor ( QCursor ( lupa_minus, lupa_minus_maska ) ) ;
            current_cursor = is_lupa_minus ;
            //cout << "                     zmiana na kursor lupa z minusem " << endl ;
            //update() ;
        }

        // if it was escape so we finish lupa mode
        if ( e->key ()  == Qt::Key_Escape )
        {
            flag_lupa_mode = false ;

            unsetCursor ();

            //setMouseTracking ( false ) ;
            current_cursor = is_usual ;
        }
    }

    //impotant to leave it for others
    else e->ignore() ;
}
//*****************************************************************************
void box_of_matrix::keyReleaseEvent ( QKeyEvent * e )
{
    //cout << "Key Release event in box of matrix  =" << hex << e->stateAfter () << endl ;

    if ( flag_lupa_mode )
    {
        //cout << "(we are in lupa mode)  state orowany = " << hex <<  (e->stateAfter ()  & Qt::ShiftButton ) << endl ;
        if ( ( e->modifiers ()  & Qt::ShiftModifier )  == 0 )
        {
            //cout << " bedzie zmiana na kursor lupa z plusem " << endl ;

            setCursor ( QCursor ( lupa_plus, lupa_plus_maska ) ) ;
            current_cursor = is_lupa_plus ;
            //cout << "                       zrobilem zmienae " << endl ;
            //update() ;

        }
    }
    else e->ignore() ; //impotant to leave it for others

}
//**************************************************************************
void box_of_matrix::force_new_pixmap(bool b)
{
    make_new_pixmap = b ;
    parent->flag_repaint_spectrum_box = 1;
    parent->flag_repaint_channels_box = 1;
    parent->flag_repaint_counts_box = 1;
    parent->flag_repaint_bscale_box = 1;
    parent->update();
}
//**************************************************************************
void box_of_matrix::construct_lupa_cursor_bitmaps()
{

    //cout << "constructing bitmaps for lupa coursor " << endl ;

    constexpr int sizemap = 32;
    constexpr int line_width = 3;
    constexpr int inner_line_width = 1;

    lupa_plus = QBitmap ( sizemap, sizemap); //QBitmap ( 32, 32);
    lupa_plus.clear();

    // lupa_plus_maska =  lupa_plus ;
    lupa_minus = lupa_minus_maska = lupa_plus_maska = lupa_plus ;

    //################################################################
    draw_lupa ( lupa_plus, Qt::color1, line_width, '+' ); // to bedzie mialo kolor czarny
    //------ zeby miec srodek kursora bialy a obwodke czarno

    draw_lupa ( lupa_plus, Qt::color0, inner_line_width, '+' ); // teraz zerujemy srodek, bedzie bialy

    // drawing mask --------------------------------
    draw_lupa ( lupa_plus_maska, Qt::color1, line_width, '+' );


    // the same for minus cursor #############################################
    draw_lupa ( lupa_minus, Qt::color1, line_width, '-' );

    //------ zeby miec srodek kursora bialy a obwodke czarno
    draw_lupa ( lupa_minus, Qt::color0, 1, '-' );

    // drawing mask --------------------------------
    draw_lupa ( lupa_minus_maska, Qt::color1, line_width, '-' );

}
//******************************************************************************
void box_of_matrix::draw_lupa ( QBitmap &bitmapa, QColor color, int width, char plus_minus )
{
    // dla narysowania plus
    int scale = bitmapa.size().rheight()    /32;
    const int os_pionowa = (14*scale) ;
    const int os_pozioma = (14*scale) ;
    const int dlug_ramion = 7*scale ; // beter when is impair
    width *= scale;

    QPoint
            p1 ( os_pozioma -dlug_ramion , os_pionowa ),
            p2 ( os_pozioma + dlug_ramion , os_pionowa ) ; // minus poziomy

    QPoint
            p3 ( os_pozioma, os_pionowa - dlug_ramion ),
            p4 ( os_pozioma, os_pionowa + dlug_ramion ) ; // pion plusa

    QPainter p ( &bitmapa );
    p.setPen ( QPen ( color, 2*width ) );

    p.drawLine ( 24*scale, 24*scale, 32*scale, 32*scale );// raczka_lupy

    p.setPen ( QPen ( color, width ) );

    p.drawArc ( (3*scale), (3*scale), 23*scale, 23*scale, 0, 360*160 ) ; // obręcz lupy

    // narysowanie plusa w lupie
    p.drawLine ( p1, p2 );
    if ( plus_minus == '+' ) p.drawLine ( p3, p4 );

}
// //*****************************************************************************
// void box_of_matrix::wheelEvent ( QWheelEvent * e )
// {
//     // this probably does not work now in linux debian
//     // NO: (had to be activated by the administrator!!!)
//
//     //e->accept() ;
//     cout << "this is a wheel event in box of matrix" << endl ;
//
//     e->ignore() ;  // so it will go to the parent
//
//     /*=======================box_of_matrix::
//       This event handler, for event e, can be reimplemented in a subclass to receive wheel events for the widget.
//       If you reimplement this handler, it is very important that you ignore() the event if you do not handle it, so that the widget's parent can interpret it.
//       The default implementation ignores the event.
//       See also QWheelEvent::ignore(), QWheelEvent::accept(), event(), and QWheelEvent.
//       =================*/
// }

//****************************************************************************
void  box_of_matrix::select_nearest_vertex ( QMouseEvent *e )
{
    //   cout << "select_nearest_vertex(QMouseEvent *e )" << endl;
    if ( parent->give_flag_show_polygons() )
    {
        bool deselect_other_vertices = ! ( e->modifiers ()  & Qt::ShiftModifier ) ;

        vector<polygon_gate>&  banana = parent->give_vector_of_polygons();
        //     cout << "Click was at x = " << e->x() << " y= " << e->y() << endl ;
        //vector<polygon_gate>&  banana = parent->give_vector_of_polygons();
        for ( unsigned int i = 0 ; i < banana.size() ; i++ )
        {
            //   cout << "we are going to search in banana called "
            //        <<  banana[i].screen_name  << endl; // this is a name


            polygon_type::iterator it ;
            [[__maybe_unused__]]  int nr = 0 ;
            for ( it = banana[i].polygon.begin() ; it != banana[i].polygon.end() ;  nr++, it++ )
            {
                // here we are jumping through coordinates
                //        cout << "This is the vertex nr " << nr
                //          << " [ "
                //          << (*it).x   // x
                //          << ", "
                //          << (*it).y
                //          << "]"
                //          << endl ;

                int xx = wx ( ( * ( it ) ).x ) ;
                int yy = wy ( ( * ( it ) ).y ) ;

                // cout << " in at pixels xx=" << xx << ", yy= " << yy << endl ;
                //looking in pixels if this the place of clicking
                if ( abs ( e->
#if  (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
                        position().
#endif
                        x() - xx ) < pixel_tolerance
                     &&
                     abs ( e->
#if  (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
                           position().
#endif
                           y() - yy )   <  pixel_tolerance
                     )
                {
                    //           cout << "nearest Vertex found !!! banana " << banana[i].screen_name
                    //           << ", vertex nr " << nr << endl ;
                    ( * ( it ) ).selected = ! ( ( * ( it ) ).selected )  ; // flip-flop
                    //update() ;
                    if ( deselect_other_vertices == false ) break ; // we select only one vetrex, even if there is another below it
                }
                else
                {
                    if ( deselect_other_vertices ) it->selected  = false ;
                }

            } // end of for interator

            // drawin banana


        } // end of drawing all bananas
    }// endof if flag polygons

}
//***************************************************************************
double box_of_matrix::prepare_font_size()
{
    return rect().width() / 80.0;
}
//****************************************************************************
// this funkction finds the vertex where we clicked. As ther result we need to have
// iterator to the vertex in this banana

//polygon_type::iterator

bool
box_of_matrix::give_nearest_vertex ( QMouseEvent *e )
{

    //   cout << "box_of_matrix::give_nearest_vertex(QMouseEvent *e)" << endl;

    flag_it_of_moving_vertex_legal = false;

    if ( parent->give_flag_show_polygons() )
    {
        vector<polygon_gate>&  banana = parent->give_vector_of_polygons();
        //     cout << "Click was at x = " << e->x() << " y= " << e->y() << endl ;
        //vector<polygon_gate>&  banana = parent->give_vector_of_polygons();
        for ( unsigned int i = 0 ; i < banana.size() ; i++ )
        {
            // cout << "we are going to search in banana called " <<
            //        banana[i].screen_name  << endl; // this is a name


            polygon_type::iterator it ;
            [[__maybe_unused__]] int nr = 0 ;
            for ( it = banana[i].polygon.begin() ; it != banana[i].polygon.end() ;  nr++, it++ )
            {
                // here we are jumping through coordinates
                //        cout << "This is the vertex nr " << nr
                //          << " [ "
                //          << (*it).x   // x
                //          << ", "
                //          << (*it).y
                //          << "]"
                //          << endl ;

                int xx = wx ( ( * ( it ) ).x ) ;
                int yy = wy ( ( * ( it ) ).y ) ;

                //        cout << " in at pixels xx=" << xx << ", yy= " << yy << endl ;
                // looking in pixels if this the place of clicking
                if ( abs ( e->
#if  (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
                                   position().
#endif
                        x() - xx ) < pixel_tolerance
                     &&
                     abs ( e->
#if  (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
                           position().
#endif
                           y() - yy )   < pixel_tolerance
                     )
                {
                    x_distance_to_vertex = e->
#if  (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
                                           position().
#endif
                                           x() - xx;
                    y_distance_to_vertex = e->
#if  (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
                                           position().
#endif
                                           y() - yy;

                    //           cout << "GIVE Vertex found !!! banana "
                    //           << banana[i].screen_name
                    //           << ", vertex nr " << nr << endl ;

                    //(*(it) ).selected = !( (*(it) ).selected)  ;   // flip-flop
                    it_of_moving_vertex = it ;

                    // now we need two neighbours
                    // let look for the next vertex --------------
                    it++ ;
                    if ( it == banana[i].polygon.end() )
                    {
                        it = banana[i].polygon.begin() ;
                    }

                    p_next = QPoint ( wx ( ( * ( it ) ).x ), wy ( ( * ( it ) ).y ) ) ;
                    //     cout << "next vertex is " <<  (*(it) ).x <<  " " << (*(it) ).y << endl ;


                    // ----------------------------------
                    it = it_of_moving_vertex ;
                    //now, lets look for the previous------------

                    if ( it == banana[i].polygon.begin() )
                    {
                        it = banana[i].polygon.end() ; // <--- this is after end
                        //      cout << "it is begin, so we jumed to the end   "  << endl ;
                    }
                    it-- ;   // in both cases (normal and begin)

                    p_previous = QPoint ( wx ( ( * ( it ) ).x ), wy ( ( * ( it ) ).y ) ) ; // normal point
                    //     cout << "Previous vertex is " <<  (*(it) ).x <<  " " << (*(it) ).y << endl ;


                    store_points_of_polygon ( i );
                    // return it_of_moving_vertex  ;
                    flag_it_of_moving_vertex_legal = true;
                    return true ; // success  ;
                }
                else
                {
                    x_distance_to_vertex = 0;
                    y_distance_to_vertex = 0;

                }


            } // end of for interator

            // drawin banana


        } // end of drawing all bananas
    }// endof if flag polygons
    return false ; // null pointer
}
//**********************************************************************************
void box_of_matrix::store_points_of_polygon ( int i )
{
    nr_of_moved_polygon = i ;
    vector<polygon_gate>&  banana = parent->give_vector_of_polygons();
    stored_polygon.clear() ;
    [[__maybe_unused__]] int nr = 0 ;
    polygon_type::iterator it ;
    for ( it = banana[i].polygon.begin() ; it != banana[i].polygon.end() ;  nr++, it++ )
    {

        // here we are jumping through coordinates
        //  cout << "This is the vertex nr " << nr
        //   << " [ "
        //   << (*it).x   // x
        //   << ", "
        //   << (*it).y
        //   << "]"
        //   << endl ;

        int xx = wx ( ( * ( it ) ).x ) ;
        int yy = wy ( ( * ( it ) ).y ) ;
        stored_polygon.push_back ( QPoint ( xx,yy ) );
    }
}
//*******************************************************************************
void box_of_matrix::select_all_vertices_in_rubberband_region()
{

    // one corner of rubberband region
    typ_x   x1 = typ_x ( pix2worX ( starting_pt.x() ) );
    typ_x   y1 = typ_x ( pix2worY ( starting_pt.y() ) ) ;

    // another
    typ_x   x2 = typ_x ( pix2worX ( previous_end_pt.x()  /*- x_distance_to_vertex*/ ) );
    typ_x   y2 = typ_x ( pix2worY ( previous_end_pt.y()  /*- y_distance_to_vertex*/ ) ) ;


    auto x_left = min(x1, x2);
    auto x_right  = max(x1, x2);

    auto y_down = min(y1, y2);
    auto y_up  = max(y1, y2);


    // loop over all polygons
    for(auto & banan : parent->give_vector_of_polygons() )
    {
        // loop over all vertices of particular polygon
        for(auto & vertex : banan.polygon )
        {
            // if vertex is inside this rectangular - make it as selected
            if(vertex.x >= x_left && vertex.x <= x_right
                    &&
                    vertex.y >= y_down && vertex.y <= y_up)
            {

                // this action may be dependent on Ctrl or Shif key in the moment
                // of starting rubberband

                // 1. No special keys pressed
                // This could be selecting only this, which are inside
                // and all other should be deselected
                vertex.selected = true;

                // 2. Shift - make all chosen vertices selected

                // 3. When
                // This is changing the current state of the vertex
                // vertex.selected = !vertex.selected;
                // this
            }
            else{ // not in the rubberband region
                if(flag_deselect_all_vertices_outside)
                    vertex.selected = false;
            }
        } // end loop over vertices of polygon

    } // end of loop over polygons
    update();
}
/********************************************************************************/
void box_of_matrix::release_reservations()
{
    //     pix.resize ( 0,0 );

    pix = pix.copy(QRect(0, 0, 0, 0) );
}
//*********************************************************************************

void box_of_matrix::make_undo_banana()
{
    //queue<polygon_gate>
    if ( undo_banana_list.size() > 0 )
    {
        undo_banana_list.front().save_to_disk()   ;
        undo_banana_list.pop_front();
    }
}
//**************************************************************************************
void  box_of_matrix::draw_lines_of_new_banana()
{
    if ( new_banana_points.empty() ) return;

    QPainter p ( this );
    p.setPen ( QPen ( Qt::red, 1 ) );


    QPainterPath skok;
    skok.moveTo ( new_banana_points[0] ) ;
    skok.lineTo ( new_banana_points[0] + QPoint ( +5, +5 ) );
    skok.lineTo ( new_banana_points[0] + QPoint ( -5, -5 ) );
    skok.moveTo ( new_banana_points[0] + QPoint ( -5, +5 ) ) ;
    skok.lineTo ( new_banana_points[0] + QPoint ( +5, -5 ) );

    for ( unsigned int i = 0 ; i < new_banana_points.size() ; i++ )
    {
        skok.lineTo ( new_banana_points[i] ) ;
    }

    p.drawPath(skok);

    p.drawText ( new_banana_points[0].x()    ,
            new_banana_points[0].y(),
            "DOUBLE CLICK TO FINISH" );
    update();

}
//***************************************************************************************
void box_of_matrix::finish_drawing_new_banana()
{

    polygon_type fence ;
    double xxx, yyy ;


    for ( unsigned int i = 0 ; i < new_banana_points.size() ; i++ )
    {
        int ptx = new_banana_points[i].x();
        xxx =  typ_x ( pix2worX ( ptx ) );
        yyy =  pix2worY ( new_banana_points[i].y() );
        fence.push_back ( vertex ( xxx, yyy ) ) ;
    }

    polygon_gate b ( screen_name_for_new_banana,
                     disk_name_for_new_banana,
                     fence );

    //cout << "Before pushing to vector " << screen_name << endl;
    parent->banana.push_back ( b );
    unsigned int last = parent->banana.size() -1;
    parent->banana[last].save_to_disk();

    flag_banana_creating_mode = false;
    new_banana_points.clear();
    //update();
}
//***************************************************************************************
void box_of_matrix::crosshair_handler ( QMouseEvent* e )
{
    //if ( rubberband_on )
    {
        current_crosshair_end_pt = e->pos();
        //QPainter p ( this );

        //p.setRasterOp ( Qt::XorROP );
        //        p.setCompositionMode(QPainter::CompositionMode_Xor);

        if ( flag_crosshair_mode_just_started || flag_screen_just_repainted )
        {
            previous_crosshair_end_pt = current_end_pt;
            // and no need to erasing previous
        }
        else
        {
#if 0
            // at first Erase last drawn rubberband:
            p.setPen ( QPen ( Qt::white, 1 ) );
            p.drawLine ( wx ( minX ),previous_crosshair_end_pt.y(),
                         wx ( maxX ),previous_crosshair_end_pt.y() );
            p.drawLine ( previous_crosshair_end_pt.x(), wy ( minY ),
                         previous_crosshair_end_pt.x(), wy ( maxY ) );
#endif
        }
        QPainterPath skok;
        // now Draw the new one:
        //        p.setPen ( QPen ( Qt::white, 1 ) );
        //        p.drawLine ( wx ( minX ),current_crosshair_end_pt.y(),
        //                     wx ( maxX ),current_crosshair_end_pt.y() );
        //        p.drawLine ( current_crosshair_end_pt.x(), wy ( minY ),
        //                     current_crosshair_end_pt.x(), wy ( maxY ) );

        skok.moveTo(QPoint( wx ( minX ),current_crosshair_end_pt.y()));
        skok.lineTo(wx ( maxX ),current_crosshair_end_pt.y());
        skok.moveTo(current_crosshair_end_pt.x(), wy ( minY ));
        skok.lineTo(current_crosshair_end_pt.x(), wy ( maxY ));
        path_crosshair = skok;

        previous_crosshair_end_pt = current_crosshair_end_pt;

        //         p.setRasterOp ( Qt::CopyROP );
        //        p.setCompositionMode(QPainter::CompositionMode_SourceOver);

        flag_crosshair_mode_just_started = false;
    } // end if rubberband_on
    update();


}
//************************************************************************
void box_of_matrix::moving_polygon_vertex_handler ( QMouseEvent* e )
{
    //static int i = 0 ;
    //	cout << "normal situation  of moving the vertex "
    //		 // << i++
    //		 << " rubberband is " << rubberband_on << endl;

    // ----------------- perhaps we want to move the vertex which is there ----

    //show_moving_flags("moving handler: ");


    if ( rubberband_on )   // rubberband for moving
    {

        //        cout << "normal situation  of moving the vertex " << i++ << " rubberband is " << rubberband_on << endl;
        current_end_pt = e->pos();
        //        QPainter p ( this );
        QPoint pd ( x_distance_to_vertex, y_distance_to_vertex );


        //         p.setRasterOp ( Qt::XorROP );
        //        p.setCompositionMode(QPainter::CompositionMode_Xor);

        if ( flag_screen_just_repainted )
        {
            previous_end_pt = current_end_pt;
            // and no need to erasing previous
        }
#if 0
        else
        {

            // ErasING last drawn rubberband: @@@@@@@@@@@@@
            p.setPen ( QPen ( Qt::white, 1 ) );
            if ( flag_move_whole_polygon )
            {


                if ( stored_polygon.size() > 1 )
                {
                    QPainterPath skok;
                    skok.moveTo ( stored_polygon[0]  + ( previous_end_pt - starting_pt /*- pd*/ ) );

                    for ( unsigned int i = 0 ; i < stored_polygon.size() ; i++ )
                    {
                        skok.lineTo ( stored_polygon[i] + ( previous_end_pt - starting_pt /*+ pd*/ ) );
                    }
                    skok.lineTo ( stored_polygon[0]  + ( previous_end_pt - starting_pt /*+ pd*/ ) );
                    p.drawPath(skok);
                }


            }
            else if ( flag_move_one_vertex_of_polygon ) // so moving only one point
            {
                p.drawLine ( p_previous, previous_end_pt - pd );
                p.drawLine ( previous_end_pt - pd, p_next );
            }
        }

        // Draw the new position one: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        p.setPen ( QPen ( Qt::white, 1 ) );
#endif
        if ( flag_move_whole_polygon )
        {
            if ( stored_polygon.size() > 1 )
            {

                QPainterPath skok;
                skok.moveTo ( stored_polygon[0] + ( current_end_pt-starting_pt /*- pd*/ ) );
                for ( unsigned int i = 0 ; i < stored_polygon.size() ; i++ )
                {
                    skok.lineTo ( stored_polygon[i] + ( current_end_pt - starting_pt /*+ pd*/ ) );
                }
                skok.lineTo ( stored_polygon[0] + ( current_end_pt - starting_pt /*+ pd*/ ) );

                //p.drawPath(skok);
                shape_moving_vertex = skok;


            }
        }
        else
            if ( flag_move_selected_vertices_of_polygon )   // ssssssssssssssssssssssssssssssssssssssssssssssssssss
            {
                // cout << "moving selected the vertices " << endl;
                // vector<pair<QPoint, string> > p; for debugging
                if ( stored_polygon.size() > 1 )
                {

                    QPainterPath skok;
                    auto diff = current_end_pt-starting_pt;

                    // Command which stores the coordinates of shape into Path called skok
                    // all vertices should be stored there, some of them as LineTo (selected)
                    // and some as moveTo (not selected)

                    //skok.moveTo ( stored_polygon[0] + ( diff ) );   // first point  will be moving  ? not needed

                    auto modulo = stored_polygon.size() ;

                    //					auto print_point = [] (QPoint pt) {    // for debugging
                    //						cout << " " << " [" << pt.x() << ", " << pt.y() << "]" ;
                    //					};

                    // loop over all points of desired polygon
                    for (int i = 0 ; i < stored_polygon.size() ; ++i )   // ppppppppppppppppppppppppppp
                    {
                        if(parent->banana[nr_of_moved_polygon].is_vertex_selected(i) == false)
                        {
                            continue;
                        }

                        // round-robin of vertices
                        // + modulo is a trick to avoid negative values
                        auto nr_prev = (i-1+modulo) % modulo;
                        auto nr_curr = i;
                        auto nr_next = (i+1+modulo) % modulo;

                        //						cout << " nr_prev =" << nr_prev << "becaue (i-1+modulo) = " << (i-1+modulo)
                        //							 << " nr_curr =" << nr_curr
                        //							 << " nr_next =" << nr_next
                        //							 << " while modulo = " << modulo << endl;


                        auto prev_pt = stored_polygon[nr_prev];
                        auto curr_pt = stored_polygon[nr_curr];
                        auto next_pt = stored_polygon[nr_next];


                        // just positioning the pen
                        if(parent->banana[nr_of_moved_polygon].is_vertex_selected(nr_prev)){
                            skok.moveTo(prev_pt+diff);   // move to line from previous vertex to here
                        }else{
                            skok.moveTo(prev_pt);   // move to line from previous vertex to here
                        }

                        // true drawing (from previous to current)
                        if(parent->banana[nr_of_moved_polygon].is_vertex_selected(nr_curr))
                        {
                            skok.lineTo( curr_pt+diff );  // line from previous vertex to current if current is selected (to his new position)
                        }
                        else {
                            skok.lineTo( curr_pt);  // line from previous vertex to current - if it is not selectd (his old, normal position)
                        }

                        // true drawing (from current to next)
                        if(parent->banana[nr_of_moved_polygon].is_vertex_selected(nr_next))
                        {
                            skok.lineTo( next_pt+diff );    // line from current to next vertex (to his new position )
                        }
                        else{
                            skok.lineTo( next_pt);    // line from current to next vertex which is normal, fixed position
                        }
                    }   // pppppppppppppppppppppppppppppppppp

                    // skok.lineTo ( stored_polygon[0] +diff );

                    //p.drawPath(skok);   // <-- this will be done in paintEvent
                    shape_moving_vertex = skok;

                    // draft
                    //					int i = 0;
                    //					for(auto x : p)
                    //					{
                    //						cout << " (" << i++ << ") ";
                    //						print_point(x);
                    //					}
                    //					cout << endl;

                }
            }      // ssssssssssssssssssssssssssssssssssssssssssssssssssss
            else if ( flag_move_one_vertex_of_polygon )
            {
                //p.drawLine ( p_previous, current_end_pt - pd );
                //p.drawLine ( current_end_pt - pd, p_next );

                QPainterPath tmp;
                tmp.moveTo(p_previous);   // move to line from previous vertex to here
                tmp.lineTo( current_end_pt);  // line from previous vertex to current
                tmp.lineTo( p_next );    // line from current to next vertex
                shape_moving_vertex = tmp;

            }

        previous_end_pt = current_end_pt;
        //         p.setRasterOp ( Qt::CopyROP );
        //p.setCompositionMode(QPainter::CompositionMode_SourceOver);

    }

}
