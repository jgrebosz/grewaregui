#include "spectrum_2D.h"
#include "box_of_spectrum.h"
#include "box_of_channels.h"
#include "box_of_counts.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

//#include <math.h>
//#include <iostream>
#include <iomanip>

#include <sstream>
#include <stdlib.h>

#include <algorithm>
#include <qinputdialog.h>


#include "paths.h"
#include "options.h"
#include "Tfile_helper.h"

#include <QTimer>
#include <QMenu>

extern Tpaths path;
extern options current_options ;
#undef COTO
#define COTO
#include "t4results_of_integration.h"
#include <QMessageBox>
#include <QFileDialog>

//using namespace Ui;
// static
polygon_gate * spectrum_2D::clipboard_polygon ; // = polygon_gate("none", "none", 0, 0, 1, 1);
bool spectrum_2D::flag_polygon_in_clipboard;

extern T4results_of_integration * integr_resul_ptr;

//T4results_of_integration  na_probe;

#include "appl_form.h"

extern appl_form  *appl_form_ptr;

//static
bool spectrum_2D::flag_ask_if_refresh_the_huge_matrices = true;

extern refresh_big_matrices_policy  policy;
////***********************************************************************
//istream & zjedz ( istream & plik )
//{
//    char c = 'x' ;
//    do
//    {
//        plik >> c  ;
//        if ( (c == '/' && ( plik.peek() == '/')) || c == '#' )
//        {
//            std::string schowek;
//            getline ( plik, schowek );
//        }
//        else
//        {
//            plik.putback ( c ) ;
//            return plik ;
//        }
//    }
//    while ( plik ) ;
//    return plik ;
//}
//*********************************************************************
#if 0
void QWidget::setMouseTracking ( bool enable )
{

    void QWidget::mouseMoveEvent ( QMouseEvent * e ) [virtual protected] ;
    const QPoint & QMouseEvent::pos () const ;
}
#endif

void spectrum_2D::mouseMoveEvent ( QMouseEvent * e )
{

    appl_form_ptr->send_to_statusbar(
        QString ( "Matrix name:    %1" )
            .arg(
                give_spectrum_name().c_str()).toStdString(),

        10*1000 ); //  seconds on the screen
    e->ignore() ;
}

//*********************************************************************
void spectrum_2D::paintEvent ( QPaintEvent * )
{

    //    static int just_now_i_work = 0;


    //QPainter paint(this) ;

    //draw_all(paint) ;
    //     cout << "spectrum_2D::PaintEvent  of spectrum" << windowTitle().toStdString() << endl ;
    //if(just_now_i_work == 0)
    //{
    //        just_now_i_work++ ;
    //if(just_now_i_work < 2)
    if(flag_repainting_allowed)    paint_all_on_screen();

    //    }else
    //    {
    //       cout << "spectrum_2D::paintEvent, level of nesting  " << just_now_i_work << endl;
    //    }
    //cout << "End of PaintEvent " << windowTitle() << endl ;
    // just_now_i_work--;
}
//********************************************************************
void spectrum_2D::init()
{
    //void QWidget::

    // so no matter if it is virtual - it will be called anyway.
    /*---
      cout << " this is init() of spectrum_2D, RTTI =  "
      << typeid(*this).name()
      << endl ;
      --*/

    setMouseTracking ( true );
    // por. chapter "calling the virtual function form the constructor of the base class"
    // in my Symfonia

    dimension = 2 ;   // 2D spectrum

    //    flaga_this_is_printing = false ;
    flag_log_scale = false ;
    flag_show_polygons = true ;
    flag_show_disknames_of_polygons = false ;
    flag_draw_scales    =    appl_form_ptr->give_flag_draw_spectra_scales() ;

    flag_he_wants_to_suffer_so_do_not_ask_more = false;
    more_than_one_polygon_selected = false;
    total_vertices_selected = 0 ;
    min_z_threshold = 0;


    prefix_of_polygon_names = "_polygon_" ;
    // we can recalculate when we already know the bin_x and bin_y - because
    // the matrix does not have to be square

    re_read_counter = 0 ;



    specif.bin = 1 ;
    specif.bin_y = 1 ;




    b_counts = new box_of_counts (this, false, "scaleY" );  // false-> no rainbow
    b_channels = new box_of_channels (this, "scaleX" );
    b_matrix = new box_of_matrix (this, "matrix" );
    b_scale =  new box_of_counts (this, true, "scaleZ" );   // true -> with rainbow scale


    layout = new QGridLayout;
    layout->addWidget(b_counts, 0,0);
    layout->addWidget(b_matrix, 0,1);
    layout->addWidget(b_channels, 1,1);
    layout->addWidget(b_scale, 0,2);

    set_layout_normal() ;
    recalculate_my_geometry() ;    // will be again after reading the file

    b_matrix->set_spectrum_pointer ( &spectrum_table, & specif );

    // spectrum_length = spectrum_table.size() ;    // can be shorter, depending what is it
    // min_channel = min_counts_on_map = 0 ;

    min_x = min_counts_on_map = 0 ;

//max_channel = spectrum_length-1 ;
//max_counts_on_map = 250 ;


#ifdef NIGDY
    // now we can imitate a matrix
    for ( int w = 0 ; w < spectrum_length ; w++ )
    {
        for ( int i = 0 ; i < spectrum_length ; i++ )
        {
            //spectrum_table[i] = (int)(( 50 * sin(0.01* i)) +( 5 * sin(0.003*i))  +100 );
            //spectrum_table[i] = (int)(  (-200* (i /1024)) + (-50* (i /100)) + i   );
            spectrum_table[ ( w * spectrum_length )  + i] = ( int ) ( i );
        }
    }


    // reading from the disk file
    ifstream plik ( "WIDMO.DAT", ios::binary );
    if ( plik )
    {
        for ( int i = 0 ; i < 8192 ; i++ )
        {
            //        plik >> spectrum_table[i] ;
            long int tmp ;
            plik.read ( &tmp, sizeof ( tmp ) );
            spectrum_table[i] = tmp ;
            if ( !plik )
            {
                spectrum_length = i ;
                //cout << " read  spectrum, channels  " << i << endl ;

                break ;
            }
        }
    }
    else
    {
        cout << " error while opening a spectrum WIDMO.DAT" << endl ;
    }

#endif // NIGDY

    marker_older = marker_younger = 0;   // integration and expansion markers

    bgr_slope = 0;
    bgr_offset = 0 ;
    flag_bgr_valid = false ;
    recent_bgr_marker = -999999 ;  // nothing have been set yet



#ifdef NIGDY
    // -- setting the first banana -- for testing reasons -------
    // polygon gates
    vertex v[4] =
    {
        vertex ( 10, 10 ), vertex ( 10, 60 ), vertex ( 60, 60 ) , vertex ( 60, 10 )
} ;
polygon_type plotek ;
for ( int i = 0 ; i < 4 ; i++ )
{
    plotek.push_back ( v[i] ) ;
}
polygon_gate b ( string ( "Z=80.polyg" ), plotek );
banana.push_back ( b );
#endif //NIGDY

if(name_of_spectrum.find("monitor") != string::npos)
{
    flag_auto_maximum = true;
}


//cout << "before finding polygons " << endl ;
find_polygons_with_names_as_this_marix();
//cout << "after finding" << endl ;

find_notice_in_description();
read_tags_from_disk();
// ---------- timer to refresh the spectrum from the disk ------
timer_refresh = new QTimer ( this );
connect ( timer_refresh, SIGNAL ( timeout() ),
        this, SLOT ( re_read_spectrum() ) );
timer_refresh->start ( 1000 * current_options.give_realistic_refreshing_time_2D() ); // 60 seconds

}
////**************************************************************************
void spectrum_2D::resizeEvent ( QResizeEvent * )
{

    //    //        cout << "spectrum_2D::resizeEvent" << endl;
    //    // perhaps here we have to recalculate geometry of all the boxes
    //    recalculate_my_geometry();

    //    // here we have to send a new walues
    //    b_counts->new_factors ( Ax, 1 - Dy, Bx, 1 - Cy );
    ////    b_channels->new_factors ( Cx, 1 - By, Dx, 1 - Ay );
    //    b_matrix->new_factors ( Cx, 1 - Dy, Dx, 1 - Cy );

    recalculate_my_geometry();

    b_counts->update();
    b_channels->update();
    b_matrix->update();
    b_scale ->update();

    //    update();


}
//***************************************************************************
void spectrum_2D::destroy()     // destructor
{

    // since now it is a vector, we do not neeed to destroy it manually
    // delete [] spectrum_table;
    spectrum_table.clear() ;

    //  delete b_counts;
    //  delete b_channels;
    if ( b_matrix )
    {
        b_matrix->release_reservations();
    }

    if ( timer_refresh )
    {
        timer_refresh->stop(); // 20 seconds
        delete timer_refresh ;
        timer_refresh = nullptr ;
    }
    //cout << "Destroy of spectrum 2D " << endl;
    save_tags_to_disk();

    delete b_channels;
    delete b_counts;
    delete b_matrix;
    b_channels 	= nullptr;
    b_counts = nullptr;
    b_matrix = nullptr;

}
//****************************************************************************
void spectrum_2D::recalculate_my_geometry()
{

    QSize s = size() ;
    // calculating the standard boarder
    //   const double pixels5 = 5.0 ;

    //bool flag_impossible_to_draw_scales = false;

    if(s.height() < 80 ||  (flag_draw_scales == false) )
    {
        set_layout_without_scale();
    }else{
        set_layout_normal();
    }




//    if ( s.height() < 80 || flag_draw_scales == false)
//    {
//        flag_impossible_to_draw_scales = true;
//    }

//    double pixels5 =   0;
//    if ( flag_draw_scales  &&  !flag_impossible_to_draw_scales )
//    {
//        pixels5 = 5;
//    }

#if 0

    //    //   const double pixels5 =  flag_draw_scales? 5.0 :   0;
    //    double boarderX = pixels5  / s.width()  ;

    //    // calculating the geometry of all the boxes
    //    Ax = boarderX;    // left edge of the scale of counts
    //    Bx = Ax + 5 * boarderX  ; // (30.0 / s.width());  //  5 digits of text  ;   // right edge of the scale of counts

    //    // OLD Cx = Bx + boarderX ; // left edge of the spectrum filed
    //    Cx = Bx ; // NEW   // left edge of the spectrum filed

    //    Dx = 1 - boarderX; // right edge of the spectrum field

    //    // vertical geometry
    //    double boarderY = pixels5 / s.height()  ;

    //    //double aspect = ((double)s.height()) / s.width()   ;

    //    Ay = boarderY ; // bottom edge of channels
    //    By = Ay + 5 * ( boarderY ) ; // 2* wys_fontu (10 pix) ;





    // to not to make channel scale (and font ) too high , when it is narrow widget


    /*---------
      cout      << " original       Ay=" << Ay << "  By=" << By
      << "  By-Ay" << By-Ay
      //<< "  Dx=" << Dx
      //<< "  Cx=" << Cx
      //<< "  Dx-Cx" << Dx-Cx
      << " aspect = " << aspect
      <<   endl ;
      ----*/

    //#define OLD_VERSION 1
    //#ifdef OLD_VERSION

    enum version  { free_aspect, square, fixed_aspect } ;
    version version_now = free_aspect ;
    //version version_now = fixed_aspect ;

    switch ( version_now )
    {
    case square :
        if ( s.width()   < 250 )
        {
            By = By / 1.5  ;
            //    cout << " recalculated Ay=" << Ay << " By=" << By
            //   << " this aspect = " << aspect
            // << endl ;
        }

        // Cy = By + boarderY ; // OLD. bottom edge of spectrum box
        Cy = By ; // NEW. bottom edge of spectrum box
        Dy = 1 - boarderY ;


        // to make it square
        /*---
          cout << " before squaring"
          << "  Dx=" << Dx
          << "  Dy=" << Dy
          << " s.width()=" <<  s.width()
          << endl ;
          ------------*/

        if ( s.height()  > s.width() )
        {
            // this is a portrait-------------
            // calculate how many pixels in horizontal can be maximum
            double pix_horizontal = s.width() * ( Dx - Cx ) ;
            // now recalculate these pixels into position in vertical;
            double part = pix_horizontal / s.height()  ;
            Dy = Cy + part ;

        }
        else                                           // this is a landscape------------
        {
            // calculate how many pixels in vertical
            double pix_vertical = s.height() * ( Dy - Cy ) ;
            // now recalculate these pixels into position in horizontal;
            double part = pix_vertical / s.width()  ;
            Dx = Cx + part ;

        }
        break ;

    case fixed_aspect :

        //new version for non-square matrices

    {

        // Cy = By + boarderY ; // bottom edge of spectrum box
        Cy = By ; // NEW. bottom edge of spectrum box
        Dy = 1 - boarderY ;


        double szer_pole = ( Dx - Cx ) * s.width() ;
        double wys_pole = ( Dy - Cy ) * s.height() ;
        double asp_pole = szer_pole / wys_pole ;
        double aspect_bin_xy = 1.0 * specif.bin / specif.bin_y ;

        //  cout << " before rectangulating --"
        //  << "  szer_pole=" << szer_pole
        //  << " wys_pole=" << wys_pole
        //  << " co daje aspekt " << asp_pole
        //  << ", natomiast aspekt danych x/y = " << aspect_bin_xy
        //  << endl ;
        //


        if ( asp_pole > aspect_bin_xy )
        {
            //cout << " puste miejsce z prawej   -------------" << endl ;
            // calculate how many pixels in horizontal can be maximum
            double horizontal_pix = wys_pole * aspect_bin_xy ;
            //cout << "maximum horiz pixels of mapa can be " << horizontal_pix << endl;
            // now recalculate these pixels into position in vertical;
            double part = horizontal_pix / s.width()  ;
            Dx = Cx + part ;

        }
        else
        {
            //cout << "Puste miejsce bedzie z gory ------------" << endl ;
            // calculate how many pixels in vertical can be maximum
            double vertical_pix = szer_pole / aspect_bin_xy ;
            //cout << "maximum vertical pixels of mapa can be " << vertical_pix << endl;
            // now recalculate these pixels into position in verical;
            double part = vertical_pix / s.height()  ;
            Dy = Cy + part ;

        }

    }
        break ;

    case free_aspect :
    {
        // calculating the standard boarder
        double boarderY = pixels5 / s.height()  ;

        //double aspect = ((double)s.height()) / s.width()   ;

        Ay = boarderY ; // bottom edge of channels
        By = Ay + 5 * ( boarderY ) ; // 2* wys_fontu (10 pix) ;
        if ( s.height()   < 250 )
        {
            By = By / 1.5  ;
            // cout << " recalculated Ay=" << Ay << " By=" << By
            //<< " this aspect = " << aspect
            // << endl ;
        }

        // Cy = By + boarderY ; OLD
        Cy = By ; // NEW. bottom edge of spectrum box

        Dy = 1 - boarderY ;

        break ;
    }
    } // endo of switch

    /*----
      cout << " after squaring"
      << " Cx= " << Cx
      << " Dx=" << Dx
      << "      Cy= " << Cy
      << " Dy=" << Dy
      << " bo szer_pole=" <<  szer_pole
      << " wys_pole=" << wys_pole
      << endl ;
      --*/

    /*---
      cout << "After recalculation "
      << "  Ax=" << Ax
      << "  Bx=" << Bx
      << "  Cx=" << Cx
      << "  Dx=" << Dx
      << " s.width()=" <<  s.width()
      << endl ;
      ----*/
#endif
}
//*****************************************************************************************
/*---
  int spectrum_2D::give_max_channel_skas()
  {

  // return (max_channel) ;
  return spectrum_table.size() ;

  }
  ----*/
//*****************************************************************************************
// function called from the toolbar (QMdiArea)
void spectrum_2D::expand()
{

    if ( fabs (marker_older - marker_younger) <= 0 )
    {
        cout << "rejected expansion\n" << endl ;
        return ;  // do not expand
    }

    if ( marker_older < marker_younger )
    {
        min_x = marker_older ;
        max_x = marker_younger ;
    }
    else
    {
        min_x = marker_younger ;
        max_x = marker_older ;
    }

    // cout << "Expand, in region " << min_x << " to " << max_x << endl ;
    b_matrix->force_new_pixmap ( true ) ; // it is a general change
    update() ; //draw_all_on_screen();

}
//*****************************************************************************************
// function called from the toolbar (QMdiArea)
void spectrum_2D::expand ( typ_x mleft, typ_x mtop, typ_x mright, typ_x mbottom )
{

    // we must check if it is legal (inside the matrix)
    //  cout << "w expand, przed korekcja  left = " << mleft
    //    << " mright = " << mright
    //    << " bottom = " << mbottom
    //    << " top = " << mtop << endl ;
    min_x = max ( mleft, specif.beg ) ;

    max_x = min ( mright, specif.end )  ;

    min_y = max ( mbottom, specif.beg_y )  ;
    max_y = min ( mtop, specif.end_y );

    //  cout << "po przestawieniu min_x = " << min_x
    //    << " max_x = " << max_x
    //    << " min_y = " << min_y
    //    << " max_y = " << max_y << endl ;

    // not too big zoom----------------------------------------------------

    double width_of_one_bin_x = ( specif.end - specif.beg ) / specif.bin;
    double width_of_one_bin_y = ( specif.end_y - specif.beg_y ) / specif.bin_y ;

    if ( max_x - min_x < 3 * width_of_one_bin_x )
        max_x = min_x + ( 3 * width_of_one_bin_x ) ;

    if ( max_y - min_y < 3 *  width_of_one_bin_y )
        max_y = min_y + ( 3 * width_of_one_bin_y )   ;
    //-----------------------------------------------------------------
    //  cout << "po korekcji 'za malej' min_x = " << min_x
    //    << " max_x = " << max_x
    //    << " min_y = " << min_y
    //    << " max_y = " << max_y
    //    << " bo bin x= " << specif.bin
    //    << " bo bin y= " << specif.bin_y
    //    << endl ;

    b_matrix->force_new_pixmap ( true ) ; // it is a general change
    update() ; // draw_all_on_screen();

}
//**********************************************************************************
void spectrum_2D::mousePressEvent ( QMouseEvent *e )
{
    //cout << " w polu dialogowym Pixl=" << e->x()
    //      << "means channel =" << b_matrix->pix2worX(e->x() )
    //      << endl ;

    if ( b_matrix-> was_mouse_click() ) // if it was in a spectrum field
    {
        // double click obslugujemy juz w box_matrix

        //cout << "to byl single click " << endl ;
        // dowiadujemy sie jaki to byl klawisz

        if ( e->button() & Qt::LeftButton )
        {
            // it will be about the marker
            // remember the new
            //    remember_as_marker(b_matrix->give_x_mouse_click()) ;
            //  b_matrix->set_markers(marker_younger, marker_older);

            // cout << "This was the left button " << endl ;

        }
        else if ( e->button() & Qt::RightButton )
        {

                // enum options {

            //Q3PopupMenu *contekst_Menu = new Q3PopupMenu;
            QMenu *context_Menu = new QMenu;

            // how many vertices are seleced now ---------------
            total_vertices_selected = 0 ;
            more_than_one_polygon_selected = false ;
            for ( unsigned int i = 0 ; i < banana.size() ; i++ )
            {
                int how_many = banana[i].how_namy_vertices_selected() ;

                if ( how_many != 0 && total_vertices_selected != 0 )
                {
                    more_than_one_polygon_selected = true ;
                }
                total_vertices_selected += how_many ;
            }
            //-------------------------------------------
            ostringstream strum ;

            typ_x xxx = ( b_matrix->give_x_mouse_click() );
            typ_x yyy = ( b_matrix->give_y_mouse_click() ) ;
            //cout << "yyy = " << yyy << endl;
            int value =  give_value_of_pixel ( xxx, yyy ) ;

            strum << "Matrix point x="
                  <<   xxx
                  << " y="
                  <<    yyy
                  << ",  contains value: "
                  << value
                //<< ( ( value >= 32000 ) ? " (SATURATED)" : "")

                ;


            b_matrix-> make_rubberband_off();

            context_Menu->addAction ( strum.str().c_str(), this,  SLOT ( slot_no_operation() ) );
            context_Menu->addSeparator();

            context_Menu->addAction("Help about mouse clicking", this, SLOT(slot_help_about_mouse_clicking() ) );
            context_Menu->addAction("Help about MOVING vertices of polygon", this, SLOT(slot_help_about_moving_vertices() ) );
            context_Menu->addAction("Help about SELECTING vertices of polygon", this, SLOT(slot_help_about_selecting_vertices() ) );
            context_Menu->addSeparator();

            context_Menu->addAction (
                QString ( "'Crosshair cursor' mode. Click here to %1" ).arg ( b_matrix->is_crosshair_mode() ? "Stop" : "Begin" ),
                this,
                SLOT ( slot_crosshair_mode() ) ) ;

            //             contekst_Menu->addAction (
            //                 "Crosshair cursor RESET",
            //                 26 );
            context_Menu->addSeparator();
            context_Menu->addAction ( "Add a NEW polygon gate", this, SLOT ( slot_add_new_polygon() ) );

            if ( more_than_one_polygon_selected )
            {
                context_Menu->addAction ( "NOTE: options below are disabled when you select vertices "
                                        " in more than one polygon", this, SLOT ( slot_no_operation () ) );
            }

            QAction* ptr_delete_polygon =
                context_Menu->addAction ( "Delete a selected polygon gate", this, SLOT ( slot_delete_polygon () ) );


            // contekst_Menu->addAction("Add a GRID of polygon gates", this, SLOT(slot_18() ) );   // <-- nobody is  using  this


            QAction* ptr_copy_selected_polygon =
                context_Menu->addAction ( "Copy a selected polygon gate", this, SLOT ( slot_copy_polygon () ) );

            QAction* ptr_paste_polygon =
                context_Menu->addAction ( "Paste a polygon gate (possible even to other matrix) ", this, SLOT ( slot_paste_polygon () ) );
            QAction* ptr_integrate_selected_polygons =
                context_Menu->addAction ( "Integrate the selected polygons", this, SLOT ( slot_integrate_polygon () ) );

            context_Menu->addSeparator();
            context_Menu->addAction ( "Z axis threshold", this, SLOT ( slot_ask_for_z_threshold () ) );
            context_Menu->addSeparator();

            context_Menu->addAction ( "Add one new vertex (after every selected vertex) ", this, SLOT ( slot_add_vertices ( ) ) );
            QAction* ptr_delete_select_vertices =
                context_Menu->addAction ( "Delete selected vertices", this, SLOT ( slot_remove_vertex ()) );
            QAction* ptr_deselect_all_vertices =
                context_Menu->addAction ( "Deselect all vertices", this, SLOT ( slot_deselect_all_vertices () ) );

            ostringstream strum2 ;

            strum2 << "UNDO moving of polygons/vertices ( possible "
                   << ( b_matrix->how_many_undo_banana_possible() )
                   << " steps ) ";

            string mess = strum2.str() ;

            QAction* ptr_banana_undo =
                context_Menu->addAction ( mess.c_str() , this, SLOT ( slot_undo_banana_change () ) );
            ptr_banana_undo->setEnabled(
                ( b_matrix->how_many_undo_banana_possible() != 0 )
                );

            context_Menu->addSeparator();
            context_Menu->addAction ( "Put a tag with my comment just at this point", this, SLOT ( slot_add_tag () ) );
            context_Menu->addAction ( "Erase a nearest tag comment [point its beginning!]", this, SLOT ( slot_erase_nearest_tag () ) );
            context_Menu->addAction ( "Remove all tags from this matrix", this, SLOT ( slot_remove_all_tags () ) );
            context_Menu->addSeparator();

            // here we can disable some options-------------------
            if ( total_vertices_selected == 0 )
            {
                ptr_deselect_all_vertices->setEnabled(false);
                ptr_delete_select_vertices->setEnabled(false);
                ptr_integrate_selected_polygons->setEnabled(false);
            }
            if ( more_than_one_polygon_selected ||  total_vertices_selected == 0 )
            {
                ptr_delete_polygon->setEnabled(false);
                ptr_copy_selected_polygon-> setEnabled(false);
            }

            if ( !flag_polygon_in_clipboard )
            {
                ptr_paste_polygon->setEnabled(false);
            }

            context_Menu->addAction ( "Show the list of X and Y incrementers of this spectrum", this, SLOT ( slot_show_incrementers () ) );
            context_Menu->addAction ( "Show time of last zeroing of this spectrum", this, SLOT ( slot_show_time_of_last_zeroing () ) );


            //             QList<QAction *>   lista =  contekst_Menu->actions();
            //             for ( int i = 0 ; i < lista.count() ; i++ )
            //             {
            //                 cout << "Action nr " << i << " is enabled ? = " << lista[i].isEnabled() << endl;
            //             }
            //-------------------------------------------

            context_Menu->exec ( mapToGlobal ( QPoint ( e->
#if  (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
                                                  position().
#endif
                                                  x() , e->
#if  (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
                                                  position().
#endif
                                                  y() ) ) );

            delete context_Menu;
            b_matrix-> inform_that_banana_just_deletd();
            //            e->accept();

        }  // end if right button

        b_matrix->clear_flag_was_mouse_click() ;
        //b_matrix->update();   // this is important for drawing only

    } // end if inside box of matrix
    else
    {
        //cout <<  " Click in neutral place " << e->x()  << endl ;
    }
    //  QWidget::mousePressEvent ( e );

}
//*******************************************************************************
void spectrum_2D::remember_as_marker ( typ_x k )
{

    // here we can adjust it to real bin position
    k = justify_to_bin_center ( k ) ;
    // to be always in the middle of the channel

    marker_older = marker_younger ;
    marker_younger = k ;

}
//*******************************************************************************
void spectrum_2D::full()
{


    // which part of matrix will be drawn
    marker_older =  min_x = specif.beg;
    marker_younger =  max_x = specif.end ;

    min_y = specif.beg_y;
    max_y = specif.end_y ;
    //--------------------------

    min_counts_on_map = 0 ;
    max_counts_on_map = 1 ;

    // lets look for a maximum value
    if ( spectrum_table.size() > 0 )
    {
        max_counts_on_map = 3; // minimum we make such

        // now we can imitate a spectrum
        for ( unsigned int i = 0 ; i < spectrum_table.size()  ; i++ )
        {
            if ( spectrum_table[i] > max_counts_on_map )
                max_counts_on_map  =  spectrum_table[i] ;
        }
        //max_counts  = (int) (1.2 * max_counts) ;
    }

    //---------------------------
    b_matrix->set_markers ( marker_younger, marker_older );

    b_matrix->force_new_pixmap ( true ) ;
    //    draw_all_on_screen();

}
//*******************************************************************************
void spectrum_2D::auto_maximum()
{
    //    cout << __func__ << endl;

    max_counts_on_map = 3; // minimum we make such
    for ( unsigned int i = 0 ; i < spectrum_table.size()  ; i++ )
    {
        if ( spectrum_table[i] > max_counts_on_map )
            max_counts_on_map  =  spectrum_table[i] ;
    }
    //     cout <<__PRETTY_FUNCTION__
    //         << "  " << name_of_spectrum
    //         << " - max counts = " << max_counts_on_map << endl;
}
//*******************************************************************************
void spectrum_2D::shift_right ( int divider )
{

    //cout << "divider  is=" << divider << endl ;
    typ_x range  = ( max_x - min_x ) ;
    if ( range  <= 0 )
        return ; // range = 1 ;

    //cout << "Range is=" << range << endl ;

    max_x = max_x + ( range / divider ) ;
    if ( max_x > specif.end )
        max_x = specif.end;

    min_x = max_x - range ;

    b_matrix->force_new_pixmap ( true ) ;
    //draw_all_on_screen();

}
//*******************************************************************************
void spectrum_2D::shift_left ( int divider )
{

    typ_x range  = ( max_x - min_x ) ;
    if ( range <= 0 )
        return ; // range = 1 ;

    min_x = min_x - ( range / divider ) ;
    if ( min_x < specif.beg )
        min_x = specif.beg;
    max_x = min_x + range ;

    b_matrix->force_new_pixmap ( true ) ;
    //draw_all_on_screen();
}


//*******************************************************************************
void spectrum_2D::scale_to_maxY()
{

    // claculate the scale in this region
    max_counts_on_map = 3 ;
    // check if the min, max values are not illegal
    adjust_range_to_legal_values();


    int first_drawn_channel = static_cast< int> ( ( min_x - specif.beg ) / specif.waga )  ;
    int last_drawn_channel  = static_cast< int> ( ( max_x - specif.beg ) / specif.waga ) ;
    int first_drawn_y = static_cast<int> ( ( min_y - specif.beg_y ) / specif.waga_y )  ;
    int last_drawn_y  = static_cast<int> ( ( max_y - specif.beg_y ) / specif.waga_y ) ;

    // loops goes on real channels !
    for ( int r = first_drawn_y ; r < last_drawn_y ; r ++ )
        for ( int i = first_drawn_channel ; i < last_drawn_channel ; i ++ )
        {
            if ( spectrum_table[ ( r * specif.bin ) + i] > max_counts_on_map )
            {
                max_counts_on_map = spectrum_table[ ( r * specif.bin ) + i] ;
            }
        }
    //min_counts_on_map = 0 ;
    b_matrix->force_new_pixmap ( true ) ;
    //draw_all_on_screen();

}
//*******************************************************************************
void spectrum_2D::scaleY_by_2()
{

    // warning: to do divide scale by 2, means that the spectrum is geting 2 times higher
    // so it is

    //cout << "1/2, before  " << max_counts_on_map ;
    if ( !flag_log_scale )
    {
        max_counts_on_map /= 2 ;
    }
    else
    {
        max_counts_on_map /= 10 ;
    }

    if ( max_counts_on_map < 10 )  // if it would be less then 10 the integer mulitplic 1.1* max_counts_on_map would not work
        max_counts_on_map = 10 ;
    //cout << "  scale_by_2*, after " << max_counts_on_map << endl ;
    //draw_all_on_screen();
    b_matrix ->force_new_pixmap(true);
}
//*******************************************************************************
void spectrum_2D::scaleY_by_05()
{

    // warning: to scale by 0.5, means that the spectrum is geting  2 times smaller
    if ( !flag_log_scale )
    {
        if ( max_counts_on_map > 100000000L )
        {
            //cout << "\a" << flush ;
            return ;
        }
        max_counts_on_map *= 2 ;
        //cout << " 0.5*, after " << max_counts_on_map << endl ;
    }
    else
    {
        if ( max_counts_on_map > 100000000L )
        {
            //cout << "\a" << flush ;
            return ;
        }
        max_counts_on_map *= 10 ;
        //cout << " Log  0.5*, after " << max_counts_on_map << endl ;
    }

    if ( max_counts_on_map < 10 )  // if it would be less then 10 the integer mulitplic 1.1* max_counts_on_map would not work
        max_counts_on_map = 10 ;
    // 	cout << "  scale_by_05, after " << max_counts_on_map << endl ;
    //update();
    b_matrix ->force_new_pixmap(true);
    //draw_all_on_screen();
}
//**********************************************************************************
void spectrum_2D::scaleY_by_factor ( double  scaling_factor )
{
    // warning: to scaling_factor   0.5, means that the spectrum is geting  2 times smaller

    if ( !flag_log_scale )
    {
        if ( scaling_factor > 1 &&  max_counts_on_map > 10000000L )
        {
            //cout << "\a" << flush ;
            return ;
        }
        max_counts_on_map *= scaling_factor ;
        //         cout << " Linear, scaling_factor= " << scaling_factor << " , max_counts_on_map = " << max_counts_on_map << endl ;
    }
    else  // ================= LOG ===============================
    {
        if ( scaling_factor > 1 )
        {
            if ( max_counts_on_map > 10000000L ) //magnification-----
            {
                //cout << "\a" << flush ;
                return ;
            }
            max_counts_on_map *= ( 2.0*scaling_factor ) ;
        }
        else   // making smaller-------
        {
            max_counts_on_map /= ( 2*scaling_factor ) ;
        }
    }

    if ( max_counts_on_map < 10 )  // if it would be less then 10 the integer mulitplic 1.1* max_counts_on_map would not work
        max_counts_on_map = 10 ;
    //     cout << " after max_counts_on_map " << max_counts_on_map << endl ;
    //draw_all_on_screen();
    b_matrix ->force_new_pixmap(true);
}

//*******************************************************************************
void spectrum_2D::integrate ( T4results_of_integration *ptr )
{


    adjust_range_to_legal_values();

    int first_int_x_channel = ( int ) ( ( min_x - specif.beg ) / specif.waga )  ;
    int last_int_x_channel  = ( int ) ( ( max_x - specif.beg ) / specif.waga ) ;

    int first_int_y_channel = ( int ) ( ( min_y - specif.beg_y ) / specif.waga_y )  ;
    int last_int_y_channel  = ( int ) ( ( max_y - specif.beg_y ) / specif.waga_y ) ;

    //#ifdef NIGDY

    // bool saturated = false ;
    long int sum = 0 ;


    bool flag_threshold_z = false;
    if ( min_z_threshold > 1 )
    {
        switch (
            askQuestionWithButtons(         // +
                "How to consider z axis threshold during integration?",
                QString ( "Currently a threshold is set on Z axis of the matrix %1.\n"
                        "Should I count ( for integration ) only only the points above the threshold? ( so only non-black points ) ?"
                        ).arg ( give_spectrum_name().c_str() ) ,
                "Only points above threshold",
                "All ( even black ) points",
                "Cancel", 1)
            )


        {
        case 1:
            flag_threshold_z = true;
            break;
        case 2:
            flag_threshold_z = false;
            break;
        default:
        case 3:
            return ;
        } // end case;
    }  // endif



    int how_many_channels = 0 ;
    //++++++++++++++++++++++++++++++++++++++++

    //    if(first_int_channel == last_int_channel)
    //   {
    //     sum = spectrum_table[first_int_channel] ;
    //     how_many_channels = 1 ;
    //     sum_tla += (int) (flag_bgr_valid ? (first_int_channel*bgr_slope + bgr_offset +0.5) : 0) ;
    //   }
    //    else




    // real integration ---------------------------------------------
    // loops goes on real channels !
    for ( int y = first_int_y_channel ; y < last_int_y_channel ; y ++ )
        for ( int i = first_int_x_channel ; i < last_int_x_channel ; i ++ )
        {
            //             if ( spectrum_table[y * specif.bin + i] >= 32000 )
            //                 saturated = true ;

            if (
                ( flag_threshold_z && spectrum_table[y * specif.bin + i] >= min_z_threshold )
                || !flag_threshold_z
                )
            {
                sum += spectrum_table[y * specif.bin + i]    ;
                how_many_channels++ ;
            }
        }


    // to have any time information ================
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    // using the STL class string
    string statement  ;

    ostringstream ss  ;

    ss
        << "=============================== "
        <<  asctime ( timeinfo )
        << "Integration of visible part of 2D spectrum "
        << windowTitle().toStdString()
        << ( flag_threshold_z ?  "\nNOTE: only the pixels above the current  displaying threshold" : "" )
        << ( ( flag_threshold_z == false && min_z_threshold > 1 ) ? "\nNOTE: even the pixels below the current displaying threshold" : "" )
        << "\n ( Pixels which down-left corner is visible - are summed ) "
        << "\nregion: "
        << " x =[" << specif.beg + ( first_int_x_channel * specif.waga )
        << " to  " << specif.beg + ( ( last_int_x_channel ) * specif.waga )
        << "],   "
        << " y =[" << specif.beg_y + ( first_int_y_channel * specif.waga_y )
        << " to  " << specif.beg_y + ( last_int_y_channel * specif.waga_y )
        << "],   " << how_many_channels
        << " bins.  \nRESULT :   " << ( sum ) << "\n"
        //             << ( saturated ? "WARNING: some bins were saturated ( >= 32000 counts ) " : "")
        ;


    statement = ss.str() ;

    // showing the result text window on the screen

    ptr->show() ;
    ptr->add_results ( statement.c_str() ) ;
    ptr->show() ;
    ptr->raise() ;    // to the top

}
//*************************************************************************************
void spectrum_2D::give_parameters ( typ_x * min_ch, typ_x * max_yy, typ_x * max_ch, typ_x * min_yy,
                                  spectrum_descr *sd )
//typ_x *sp_beg, typ_x *sp_end)
{

    // this is the function for an application, to learn how to position the sliders and scrollers
    *min_ch = min_x ;
    *max_ch = max_x ;
    *min_yy = min_y ;
    *max_yy = max_y ;

    *sd = specif ;


    //     cout << " 2D give_parameters "
    //          << "x ( " << min_x << ", " << max_x
    //          << " )   y ( " << min_y << ",  " << max_y << " ) "
    //          << ",  spectrum  = "  ;

    //     cout
    //         << "bin = " << specif.bin << " beg=" << specif.beg << ", end=" << specif.end << ", waga=" << specif.waga
    //         << "bin_y = " << specif.bin_y << " ( beg_y=" << specif.beg_y << ", end_y=" << specif.end_y << " ), waga_y=" << specif.waga_y
    //         << endl;


    //   << endl;

    //*sp_beg = specif.beg ;
    //*sp_end = specif.end ;

}
//********************************************************************************
void spectrum_2D::scrollbar_horizontal_moved ( int int_left_edge )
{
    // in fact it was really scrollbar
    // this trick is because sliders do not accept any double numbers

    //  cout << "scrolbar mover , value = " << int_left_edge << endl ;
    typ_x my_left_edge = ( typ_x ) int_left_edge /   scrollbar_poziomy_mnoznik  ;
    // cout << "Po korekcji = " << my_left_edge << endl ;

    typ_x range  = ( max_x - min_x ) ;
    if ( range <= 0 )
        return ; // range = 0.01 ;

    //cout << "Range is=" << range << endl ;

    max_x = my_left_edge + range ;
    min_x = my_left_edge ;

    if ( max_x > specif.end )
        max_x = specif.end  ;
    if ( max_x - min_x < range )
        min_x = max_x - range ;

    //draw_all_on_screen();
    b_matrix ->force_new_pixmap(true);

}
//*******************************************************************************
void spectrum_2D::slider_horizontal ( int value )
{


    // cout << "slider horizontal mover , value = " << value << endl ;
    typ_x my_value = ( typ_x ) value /   scrollbar_poziomy_mnoznik  ;
    // cout << "Po korekcji = " << my_value << endl ;



    if ( my_value < 5 )
        my_value = 5 ;

    if ( min_x + my_value  >  specif.end )
    {
        max_x = specif.end ;
        min_x = max_x - my_value ;
    }
    else
    {
        max_x = min_x + my_value ;
    }

    if ( min_x < specif.beg )
        min_x = specif.beg ;

    //draw_all_on_screen();


}
//*******************************************************************************
void spectrum_2D::scroller_vertical_moved ( int value_bottom )
{
    //     cout << "spectrum_2D::scroller_vertical_moved ( " << value_bottom << " ) " << endl ;

    double vb =  1.0 * value_bottom  /   scrollbar_poziomy_mnoznik;
    //     cout << "podzielony = " << vb << " ) " << endl ;
    typ_x current_range  = ( max_y - min_y ) ;
    if ( current_range < 0.001 )
        current_range = 0.001 ;

    //     cout << "before change:  max_y = " << max_y << ", min_y = "<< min_y << endl ;

    max_y = -vb ;
    min_y = max_y - current_range ;

    //     cout << "after   change:  max_y = " << max_y << ", min_y = "<< min_y << endl ;

    b_matrix ->force_new_pixmap ( true );
    //draw_all_on_screen();
}
//*******************************************************************************
int spectrum_2D::giveCurrentMaxCounts()
{

    int cur_max = 0 ;
    for ( unsigned int i = 0 ; i < spectrum_table.size(); i++ )
    {
        if ( spectrum_table[i] > cur_max )
            cur_max = spectrum_table[i] ;
    }
    return cur_max ;
}
//*******************************************************************************

//*******************************************************************************
void spectrum_2D::set_parameters ( typ_x min_ch,  typ_x max_yy, typ_x  max_ch, typ_x min_yy )
{

    //     cout << ">>>>>>>>>>>>>>>>> F. spectrum_2D::set_parameters" << endl ;;

    //     cout
    //         << "wstep:    x ( " << min_ch << ", " << max_ch
    //         << " )   y ( " << min_yy << ",  " << max_yy << " ) "
    //         << " range = " << ( max_yy - min_yy)
    //         << endl;

    adjust_range_to_legal_values();

    typ_x tmp_min_x = max ( min_ch, specif.beg );
    typ_x tmp_max_x = min ( max_ch, specif.end );

    typ_x tmp_min_y = max ( min_yy, specif.beg_y );
    typ_x tmp_max_y = min ( max_yy, specif.end_y );


    //	if ( ( tmp_min_x != tmp_max_x )  && ( tmp_min_y != tmp_max_y ) )
    {
        min_x = tmp_min_x ;
        max_x = tmp_max_x ;

        min_y = tmp_min_y ;
        max_y = tmp_max_y ;
    }

    //     cout
    //         << "PO chwili  x ( " << min_x << ", " << max_x
    //         << " )   y ( " << min_y << ",  " << max_y << " ) "
    //         << endl;
    //================================

    // this can be nonsense in case of matrices

    // this is the function for an application, to learn how to position the sliders and scrollers
    // be carefull, spectra can have different length. For example 4096 and 256

    if ( max_ch < specif.end )
    {
        min_x = min_ch ;
        max_x = max_ch ;
    }
    else
    {
        //cout << "Uwaga max_ch = " << max_ch << "gdy spextrum length " << spectrum_length << endl ;
    }

    min_y = min_yy ;
    max_y = max_yy ;

    // this aobve was sometimes going out of legal
    adjust_range_to_legal_values();

    //     cout
    //         << "x ( " << min_x << ", " << max_x
    //         << " )   y ( " << min_y << ",  " << max_y << " ) "
    //         << " range = " << ( max_yy - min_yy)
    //         << "\n>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
    b_matrix->force_new_pixmap(true);
    update();
    // draw_all_on_screen();
}
//*******************************************************************************
void spectrum_2D::save()   // virtual function to save spectrum
{

    // NOTE: ??? Never used?


    // selecting the name ?
    QString fname = windowTitle() + ".mat3asc" ;

    // selecting the format ASCII / binary  (by default in ASCII ?)
    //real_save_ascii(fname);
}
//*******************************************************************************
void spectrum_2D::save_as ( string prefix )
{

#ifdef  NIGDY
        // dialog to choose the name and the format
    QFileDialog* fd = new QFileDialog ( this, "Save current matrix as a file ", true );
    fd->setMode ( QFileDialog::AnyFile );
    fd->setFilter ( "3 column ASCII matrices ( *.mat3asc ) " );
    QString fileName;
    if ( fd->exec() == QDialog::Accepted )
    {
        fileName = fd->selectedFile();
    }
    //cout << "Nazwa " << fileName << endl ;
    if ( fileName )
    {
        // real saving
        real_save_ascii ( fileName, false ); // so far NO radware header for matrices
    }


#else


    if ( prefix.empty() )
    {

            // dialog to choose the name and the format
#if 0
        QFileDialog* fd = new
                QFileDialog ( gpath.spectra, QString::null, this,
                              // new QFileDialog( this,
                              "Save current matrix as a file ", true );

        fd->setMode ( QFileDialog::AnyFile );







        //note: below the filters are separated with two semicolons.
        fd->setFilters ( QString ( "a ) Three  column ASCII matrices ( *.mat3asc );; b ) Binary original cracow format ( *.mat ) " ) );

        //  fd->setFilters(QString("1 ).  Just two columned ascii ( *.asc );; 2 ).  Radware header, two columned ascii ( *.asc );; 3 ).  Binary original cracow format ( *.spc ) ")  );
#endif // 0

        QString fileName;
        QString filter;

        Tplate_spectrum::flag_repainting_allowed = false;

        fileName = QFileDialog::getSaveFileName ( this,
                                                tr ( "Save current matrix as a file " ),
                                                (gpath.Qspy_dir() + "./type_name.mat"),
                                                tr ( "a ) Three  column ASCII matrices ( *.mat3asc );; b ) Binary original cracow format ( *.mat ) " ),
                                                &filter
                                                );

        Tplate_spectrum::flag_repainting_allowed = true;

        //if ( fd->exec() == QDialog::Accepted )
        {
            //fileName = fd->selectedFile();
            // filter = fd->selectedFilter();


            //     cout << "Nazwa= >" << fileName << "<, filter= >"<< filter << "<"
            //     << gpath.spectra
            //     << " a name_of_spectrum " << name_of_spectrum
            //     << endl ;

            if ( filter[0] == 'b' )
            {
                // just  making copy of this spectrum file
                string kom = "cp " + gpath.spectra + name_of_spectrum + " ";
                kom  += fileName.toStdString() ;
                //       cout << "Komenda =" << kom << "=" <<endl;
                int answ [[maybe_unused]] = system ( kom.c_str() );
                // answ = answ;
            }
            else  // here we are also when "Cancel was pressed
            {

                //       cout << "Nazwa= >" << fileName << "<, filter= >"<< filter << "<"
                //       << " flag radware = " << flag_radware_header
                //       << endl ;

                if ( fileName.size() )  // OK with some name was chosen by the dialog
                {
                    // real saving
                    real_save_ascii ( fileName, false ); // so far NO radware header for matrices
                }

            }

        } // if accepted

    }
    else // prefix NOT empty so always binary with prefix
    {
        // just  making copy of this spectrum file
        string kom = "cp " + gpath.spectra + name_of_spectrum + " ";
        kom  += ( gpath.spectra + prefix + name_of_spectrum )   ;
        //       cout << "Komenda =" << kom << "=" <<endl;
        int answ2 [[maybe_unused]] = system ( kom.c_str() );
        // answ2 = answ2;

    }

#endif


}
//*******************************************************************************************************
void spectrum_2D::real_save_ascii ( QString & nazwa , bool /* flag_radware*/ )
{

    string path_filename = //gpath.spectra +
        nazwa.toStdString();

    ofstream plik ( path_filename.c_str() );
    if ( !plik )
    {
        QMessageBox::information ( this, "Saving the spectrum problems" ,
                                 QString ( "Can't create the file %1" )
                                     .arg ( path_filename.c_str() ),
                                 QMessageBox::Ok );
        return ;
    }
    else
    {
        for ( unsigned int i = 0 ; i < spectrum_table.size() ; i++ )
        {
            // calculate the coordinates
            double x = ( i / specif.bin ) * specif.waga + specif.beg ;
            double y = ( i / specif.bin ) * specif.waga_y + specif.beg_y ;
            if ( spectrum_table[i] ) // we save only non-zero    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                plik << x << "\t" << y << "\t"
                     <<  spectrum_table[i] << endl ;
        }
    }
}
//*************************************************************************************
void spectrum_2D::set_the_name ( QString & name )
{

    this->setWindowTitle ( name );
}
//****************************************************************************************
void spectrum_2D::print_it()
{
#if 0
    // cout << "spec widget, Decyzja drukowania " << endl ;
    flaga_this_is_printing = true ;
    printer = new QPrinter ;

    if ( printer->setup ( this ) )
    {
        //   cout << "Po setupie "
        //     << printer->docName()
        //     << endl ;
        QPainter piorko ; //(printer) ;
        //cout << "Po piorku " << endl ;

        // b_counts->set_printing_mode(&piorko);
        // b_channels->set_printing_mode(&piorko);
        //b_matrix->set_printing_mode(&piorko);
        if ( piorko.begin ( printer ) )
        {
            cout << "blad , return " << endl ;
            return ;
        }

        draw_all ( &piorko ) ;
        //   cout << "Po draw " << endl ;
    }
    //    delete printer ;
    //    printer = 0 ;
#endif

    //    flaga_this_is_printing = false ;
}
//**************************************************************************
void spectrum_2D::paint_all_on_screen()
{

    // cout << " spectrum_2D::paint_all_on_screen()" << endl;
    //  flaga_this_is_printing = false ;
    QPainter piorko ( this ) ;

    //b_matrix->force_new_pixmap ( true ) ;
    paint_all ( &piorko ) ;

}
//**************************************************************************
void spectrum_2D::read_in_file ( const char *name, bool this_is_first_time )
{

    //   cout << "F. read_in_file " << name << endl;
    if ( re_read_counter > 0 )
    {
        // several times refresh more often second
        re_read_counter-- ;
        timer_refresh->stop();
        timer_refresh->start ( 1000 * 10 ); // (1000 is 1 second)
    }
    else
    {
        // if not - so it is normal
        timer_refresh->stop();
        timer_refresh->start ( 1000 * current_options.give_realistic_refreshing_time_2D() ); // (1000 is 1 second)
    }




    bool flag_file_opened = false;
    flag_error_while_reading = false;
    bool too_short = true ;
    const int how_many_tries = 900;
    // loop to repeat 'too-short' read (when spy writes this matrix just now
    for ( int n_try = 0 ; n_try < how_many_tries && too_short ; n_try++ ) //
    {

        //             cout << "Function: Read in file, parameter " << name << endl ;
        spectrum_table.clear() ;
        // reading from the disk file
        string path_filename = gpath.spectra + name ;
        ifstream plik ( path_filename.c_str(), ios::binary );
        if(plik) flag_file_opened = true;

        if ( plik && (n_try < (how_many_tries -2)) )
        {
            int ile = 0 ;

            // at first read binnings
            double tab[6] = {0};
            plik.read ( ( char* ) tab, sizeof ( tab ) );

            if ( !plik )
            {
                // 				cout << "Error while reading the specif data " << endl;
                continue;
            }

            // NOTE:      the trick is that when the specif.bin value is given as "negative" - this means, that the matrix
            //             is not 16 bit word (short int)
            //             but 32 bit word (normal int)

            bool flag_this_is_32bit_cell = tab[0] < 0 ?   true:false;

            specif.bin = abs ( ( int ) tab[0] ) ;
            specif.beg = tab[1];
            specif.end = tab[2];

            specif.waga = ( specif.end - specif.beg ) / specif.bin ;

            // ------ Y
            specif.bin_y = ( int ) tab[3];
            specif.beg_y = tab[4];
            specif.end_y = tab[5];

            specif.waga_y = ( specif.end_y - specif.beg_y ) / specif.bin_y ;



            //             cout << "Specif -------111111111111111111111111111111111111111111111111\n" ;
            //             for ( int i = 0 ; i < 6 ; i ++ )/
            //                 cout << " tab[" << i << "] = " << tab[i] ;
            //
            //             cout << "\n specif.bin= " << specif.bin
            //                  << " specif.beg=" << specif.beg
            //                  << " specif.end=" << specif.end
            //                  << " specif.waga=" <<  specif.waga
            //
            //
            //                  << "\nspecif.beg_y=" << specif.beg_y
            //                  << " specif.end_y=" << specif.end_y
            //                  << " specif.bin_y= " << specif.bin_y
            //                  << " specif.waga_y=" <<  specif.waga_y << endl;


            if ( flag_this_is_32bit_cell == false )   // standard situation 16 bit
            {
                short int short_value ;
                do
                {
                    plik.read ( ( char* ) &short_value, sizeof ( short_value ) ) ; // for simulaion it is commented
                    ile++ ;
                    if ( !plik )
                    {
                        if(!plik.eof() )
                        { cout << "error while reading ile=" << ile << endl;}
                        break ;
                    }
                    spectrum_table.push_back ( short_value ) ;
                }
                while ( plik ) ;
            }
            else   // 32 bit situation, modern
            {
                int int_value ;
                do
                {
                    plik.read ( ( char* ) &int_value, sizeof ( int_value ) ) ; // for simulaion it is commente
                    ile++ ;
                    if ( !plik )
                    {
                        //cout << "error while reading ile=" << ile << endl;
                        break ;
                    }
                    spectrum_table.push_back ( int_value ) ;
                }
                while ( plik ) ;
            }

            //						 cout << "sizeof(int) = " << sizeof(int)
            //						 << "  sizeof(short int) = " << sizeof(short int)
            //						 << "  sizeof(long) = " << sizeof(long)
            //						 << endl;
            //						// now
            //						cout << " read  spectrum , ile= "<< ile << endl ;

            //                              cout << "spectrum_table.size = " << spectrum_table.size()
            //                                    << ", sqrt = " <<sqrt(spectrum_table.size()) << endl ;

            if ( spectrum_table.size() == ( unsigned ) ( specif.bin * specif.bin_y )
                ||
                spectrum_table.size() == (( unsigned ) ( specif.bin * specif.bin_y ) *2)  // in case flag_this_is_32bit_cell ???
                )
            {
                too_short = false ; // <--- no more tries, because GOOD!
            }
            else
            {
                static int counter = 0 ;
                if( !(++counter % 100) ) cout << "problem with reading matrix "
                         << path_filename
                         << "should be "
                         << specif.bin * specif.bin_y
                         << "pixels, but it was read "
                         << spectrum_table.size()
                         << ", another try.. "
                         << n_try << endl;
                continue; // another try
            }
            //   specif.bin = (int) (sqrt( (double)spectrum_table.size()));
            //   specif.bin_y =  (int) sqrt( (double)spectrum_table.size());


        }
        else
        {
            if ( timer_refresh )
            {
                timer_refresh->stop(); // 20 seconds
                delete timer_refresh ;
                timer_refresh = nullptr ;
            }
            flag_error_while_reading = true;
            string mess =  "Matrix \n";
            mess += name ;
            if(flag_file_opened == false){
                mess += "\nwas not found ";
            }else{
                mess += "\nhad error during reading";
            }
            mess += "\nTry to call this spectrum again" ;

            QMessageBox::information ( this, "Error while reading a file", mess.c_str() );

            spectrum_table.push_back ( 0 ) ;
            specif.beg = 0 ;
            specif.end = 1 ;
            specif. bin = 1  ;       //   333 ;
            // vertical axis
            specif.beg_y = 0 ;
            specif.end_y = 1 ;
            specif.bin_y = 1  ;       //   333 ;

            too_short = false ; // <--- no more tries

        }

    } // end of for (too short read)




    // if this is too big matrix
    if ( ( specif.bin_y * specif.bin ) >= ( 2001 * 2000 ) )
    {

        if ( timer_refresh )
        {
            timer_refresh->stop(); // 20 seconds
            //      delete timer_refresh ;
            //      timer_refresh = 0 ;

            //cout << " Error while opening a spectrum " << path_filename << endl ;
            string mess =  "Matrix \n   ";
            mess += name ;
            mess += "\nis very big, so 'automatic refreshing' it on the screen would take too much time\n"
                    "Greware suggests not to re-read it automatically. \n"
                    "(You  can always refresh it manually - just by recalling it again)"
                    "\n\nNOTE you can avoid this question by unchecking an option in: \n"
                    "      Cracow_preferences->Setting times of Refreshing-->ask questions about...\n\n"
                    "So, decide if you want to skip auto-refresh, or you want to suffer but keep auto-refresh active:";

            if ( current_options.give_preference_asking_about_refreshing_big_matrices() )
                if ( flag_ask_if_refresh_the_huge_matrices &&
                    (policy == not_decided || policy == skip_one || policy == refresh_one)
                    )
                {
                    QMessageBox  mb;
                    mb.setText(mess.c_str());

                    mb.addButton("Skip refreshing of this matrix", QMessageBox::YesRole);
                    auto def = mb.addButton("Skip also for all next big matrices", QMessageBox::YesRole );
                    mb.setDefaultButton(def);
                    mb.addButton("I want auto-refreshing for this matrix", QMessageBox::NoRole );
                    mb.addButton("Auto-refresh this matrix, and all next ones", QMessageBox::NoRole );

                    auto odp = mb.exec();
                    switch ( odp )
                    {
                    default:
                    case 0: // skip this one
                        policy = skip_one;
                        break;

                    case 1:  // skip all
                        policy = skip_all;
                        break;

                    case 2:   // suffer_one
                        policy = refresh_one;
                        break;

                    case 3:   // suffer_all
                        policy = refresh_all;
                        flag_he_wants_to_suffer_so_do_not_ask_more = true;
                        break;
                    }
                }
            // so now we react for the answers

            if(policy == skip_one || policy == skip_all)
            {
                timer_refresh->stop(); // 20 seconds
                delete timer_refresh ;
                timer_refresh = nullptr ;

            }
            else if(policy == refresh_one || policy == refresh_all){
                set_new_refreshing_times();
            }
        }

    }

    //#################################### post-settings #####################################

    marker_older =  specif.beg ;
    marker_younger =   specif.end ;

    b_matrix->set_markers ( marker_younger, marker_older );

    specif.waga = ( specif.end - specif.beg ) / specif.bin ; // zlotowki na kanal
    specif.waga_y = ( specif.end_y - specif.beg_y ) / specif.bin_y ; // zlotowki na kanal

    //     cout << "Specif -------\n"
    //          << "specif.beg=" << specif.beg
    //          << " specif.end=" << specif.end
    //          << " specif.bin= " << specif.bin
    //          << " specif.waga=" <<  specif.waga
    //
    //          << "\nspecif.beg_y=" << specif.beg_y
    //          << " specif.end_y=" << specif.end_y
    //          << " specif.bin_y= " << specif.bin_y
    //          << " specif.waga_y=" <<  specif.waga_y << endl;

    recalculate_my_geometry() ;    // now we know it is a square  or rectangle

    if ( this_is_first_time )
        full();
    else{
        // draw_all_on_screen() ;
    }
}
//******************************************************************************
#if 0

bool spectrum_2D::find_x_description ( const char * name )
{

    // now the system has changed and bins are written as first
    // six double words in the contents of spectrum
    // so we do not have to read them here
    //
    // now this function is only checking if this is active
    // matrix, or it is not anymore aculutated (but what for...?)


    //char *desc_name = "description_of_ranges.txt";
    string path_filename = gpath.spectra + "description_of_ranges.txt";
    ifstream plik ( path_filename.c_str() ) ;

    if ( !plik )
        return false ;


    char wyraz[500] ;
    while ( 1 )
    {

        if ( !plik )
            break ; //return false ;
        if ( plik.eof() )
            break ; // return false;

        plik.getline ( wyraz, sizeof ( wyraz ) ) ; // because filename can have some spacies inside
        //cout << "przeczytane " << wyraz << endl ;

        /*---
        cout << "any key " ;
        int nic ;
        cin >> nic ;
        --*/


        if ( strcmp ( name, wyraz ) == 0 ) // found  ----------
        {
            // this bins now are inside the contents of the matrix
            // so we do not have to read them separately
            //    plik  >> specif.bin
            //       >> specif.beg
            //       >> specif.end
            //       >> specif.bin_y
            //       >> specif.beg_y
            //       >> specif.end_y ;

            //      cout << "spectrum description found "
            //        << wyraz
            //         << " bins_x = " <<  specif.bin
            //        << " first ch = " <<  specif.beg
            //        << " last ch = " <<  specif.end
            //        << ", Y bins = " << specif.bin_y
            //        << " first ch =" << specif.beg_y
            //        << " last ch = " << specif.end_y
            //        << endl ;

            return true ;
        }
        // try without extension ----------------------
        //cout << " is not " << name << endl ;


        char *wsk = strrchr ( wyraz, ' ' ) ;
        if ( wsk )
            *wsk = 0 ;
        strcat ( wyraz, ".mat" );
        //cout << " after appending is  " << wyraz << endl ;

        if ( strcmp ( name, wyraz ) == 0 ) // found
        {
            //    plik
            //      >> specif.bin
            //      >> specif.beg
            //      >> specif.end
            //      >> specif.bin_y
            //      >> specif.beg_y
            //      >> specif.end_y ;

            //      cout << "spectrum description found "
            //        << wyraz
            //         << " bins_x = " <<  specif.bin
            //        << " first ch = " <<  specif.beg
            //        << " last ch = " <<  specif.end
            //        << ", Y bins = " << specif.bin_y
            //        << " first ch =" << specif.beg_y
            //        << " last ch = " << specif.end_y
            //        << endl ;

            return true ;
        }


        //  int fff ;
        //  cin >> fff ;
    } // end of while

    return false ;

}
#endif // 0

//***************************************************************************************
void spectrum_2D::log_linear ( bool stan )
{

    flag_log_scale = stan ;
    b_matrix->force_new_pixmap ( true ) ; // it is a general change
    b_scale->repaint();
    // update() ; //draw_all_on_screen();
    //draw_all_on_screen();
}
//************************************************************* vector<typ_x> backgr_markers ;
void spectrum_2D::set_backgr_marker ( typ_x dana )
{

    recent_bgr_marker =  justify_to_bin_center ( dana );
    backgr_markers.push_back ( recent_bgr_marker )  ;

    sort ( backgr_markers.begin(),  backgr_markers.end() );
    bgr_slope = 0 ;
    bgr_offset = 0 ;
    flag_bgr_valid = false ;

    //    to test
    //    for(unsigned int i = 0 ; i <  backgr_markers.size() ; i++)
    //    {
    //  cout << i << " ) = " << backgr_markers[i] << endl ;
    //    }
}
//*****************************************************************************
vector<typ_x> * spectrum_2D::give_bg_markers()
{

    return & backgr_markers ;
}

//*****************************************************************************
vector<polygon_gate>&  spectrum_2D::give_vector_of_polygons()
{

    return banana ;
}
//******************************************************************************
typ_x spectrum_2D::justify_to_bin_center ( typ_x dana )
{

    // here we can adjust it to real bin position
    int binek = static_cast<int> ( ( dana - specif.beg ) / specif.waga )  ;
    dana = ( binek * specif.waga ) + specif.beg
           +
           0.5 * specif.waga  ; // to be always in the  middle of the channel
    return dana ;
}

//***************************************************************************
int spectrum_2D::typ_x_to_bin ( typ_x value )
{

    return static_cast<int>( ( value - specif.beg ) / specif.waga )  ;
}

//***************************************************************************
bool spectrum_2D::give_bg_info ( double * sl, double *off )
{

    *sl = bgr_slope ;
    *off = bgr_offset ;
    return flag_bgr_valid ;
}
//*************************************************************************
void spectrum_2D::paint_all ( QPainter *  /* piorko*/ )
{

    //     cout << "spectrum_2D::paint_all \nBefore drawing X " <<   min_x << " - " <<  max_x
    //          <<  "  Y= " << min_y
    //           << " - " <<  max_y
    //           << endl ;

    // if this is rubberband mode, we do not want to destroy XOR
    //     if ( b_matrix->is_rubberband_on() && b_matrix->is_crosshair_mode())
    // 	         return ; // it would destroy rubber XOR

    // static int level = 0 ;
    //    level++;

    //    cout << "spectrum_2D::paint_all - level=" << level << endl;

    //    if(level >= 2) {
    //        cout << " rejected call" << endl;
    //        level--;
    ////        update();
    //       // b_channels->update() ;
    //        return;
    //    }
    /*---
    double gorka = max_counts_on_map ;
    if(flag_log_scale)
    {
    gorka = log10(gorka)+1;
    }
    -------*/

    if(flag_auto_maximum) auto_maximum(); // JG- !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //    static int licznik;
    //    cout << "Tutaj dałem full zrobic to lepiej!!!!!!!!! " << licznik++ << endl;

    // Note: if we have to draw pixels  0-100, the the last pixel will start at 100 and finish at 100.9999
    // so the real range of picture should be  min_y : max_y+waga_y

    //cout << "Just before changing---------------------" << endl;
    b_matrix->change_region_of_spectrum ( min_x, max_y, max_x,  min_y );
    // cout << "Just after changing---------------------" << endl;
    b_counts->change_region_of_counts ( min_y,  max_y );
    b_channels->change_region_of_channels ( min_x, max_x );

    int gorka = max_counts_on_map ;
    int doleczek = min_counts_on_map  ;
    if(flag_log_scale)
    {
        gorka = static_cast<int> (log10((double) gorka) + 1) ;
        doleczek = -1 ;
        //          cout << "in Spectrum_1D gorka = "
        //           <<   gorka
        //           << endl ;
    }

    b_scale->change_region_of_counts (doleczek, gorka);


    if(flag_repaint_counts_box)
    {
        b_counts->update();
        flag_repaint_counts_box = false;
    }
    if(flag_repaint_channels_box)
    {
        b_channels->update();
        flag_repaint_channels_box = false;
    }

    if(flag_repaint_spectrum_box)
    {
        b_matrix-> update() ; // update();
        flag_repaint_spectrum_box = false;
    }

    if(flag_repaint_bscale_box){
        b_scale->update();
        flag_repaint_bscale_box = false;
    }

    // here it would be nice to update the scrollbars and sliders

    //    cout << "$$$ draw_all, przed emisja sygnalu update scrolls,  ["
    //      << min_x
    //      << "-"
    //      << max_x
    //      << "]"
    //      << endl ;


    emit spec_update_scrolls_and_sliders()  ;

    //   cout << "$$$ draw_all,  po emisji sygnalu update scrolls,  ["
    //      << min_x
    //      << "-"
    //      << max_x
    //      << "]"
    //      << endl ;


    // min_channel => value ;
    //level--;
}

//*****************************************************************************
bool spectrum_2D::give_flag_log_scale()
{
    //
    return flag_log_scale ;
}

//****************************************************************************

//************************************************************************
void spectrum_2D::show_polygons ( bool stan )
{
    flag_show_polygons = stan ;
    b_matrix->force_new_pixmap ( true ) ; // it is a general change

    //draw_all_on_screen();
}
//*************************************************************************
void spectrum_2D::lupa_button_pressed()
{
    b_matrix->enter_lupa_mode()   ;
}
//**********************************************************************
void spectrum_2D::keyPressEvent ( QKeyEvent *e )
{
    //  cout << "keypressed event in spectrum_2D" << endl;
    //     appl_form_ptr->statusBar()->message(
    //       "Tip: example of jurek tip",
    //       15*1000); // 15 seconds on the screen
    e->ignore() ;
}

//********************************************************************************
void spectrum_2D::wheelEvent ( QWheelEvent * e )
{

    e->accept() ;
    //     cout << "this is a wheel event  in spectrum 2D, delta = "<<  e->angleDelta().y() << endl ;
    if ( e->angleDelta().y() < 0 )
    {
        //         scaleY_by_05();
        scaleY_by_factor ( 0.9 );
    }
    else
    {
        //       scaleY_by_2();
        scaleY_by_factor ( 1.2 );
    }
    //e->ignore() ;

}
//********************************************************************************
void spectrum_2D::error_more_than_one_polygon_selected()
{

    QMessageBox::information ( this, "I dont know which polygon you mean !" ,
                             QString ( "There are vertices selected in more than one polygons now, "
                                     "so I don't know which polygon you mean !  "
                                     " please select vertices in only ONE polygon" ) ,
                             QMessageBox::Ok );

}
//*************************************************************************
void spectrum_2D::erase_polygon ( int nr )    // polygon gates
{
    //vector<polygon_gate>iterator:: it ;
    //= banana[i] ;
    banana.erase ( banana.begin() + nr ) ;

}
//*******************************************************************************
int spectrum_2D::give_value_of_pixel ( typ_x x, typ_x y )
{
    int ch_x = static_cast<int > ( ( x - specif.beg ) / specif.waga )  ;
    int ch_y = static_cast<int > ( ( y - specif.beg_y ) / specif.waga_y )  ;

    if ( ( ( ch_y * specif.bin ) + ch_x ) < 0 ) return -1;

    if ( (static_cast<signed int> (spectrum_table.size()) -1 < ( ( ch_y * specif.bin ) + ch_x )) )
        return -1;

    return spectrum_table[ ( ch_y * specif.bin ) + ch_x] ;

}
//********************************************************************************
void spectrum_2D::focusInEvent ( QFocusEvent * e )
{
    if ( e->gotFocus() )
    {
        //cout << "spectrum 2D got a focus " << endl ;
        emit enable_selected_actions ( 2 ); // 2d spectrum
    }
}
/***********************************************************************
* this is a function to shift the spectrum (matrix) on the picture up or down
*  the argument is telling if we are going higher (plus) or lower (minus)
*  by the definition devider = 1 means:
*                                               move as much as this part of the picture
* which you see now.
*  if the argument will be 10 - means:
*                                            move by 1/10 of the current picture
************************************************************************/
void spectrum_2D::shift_up_down ( int divider )
{
    //     cout << "spectrum_2D::shift_up_down  divider  is=" << divider << endl ;
    typ_x range  = ( max_y - min_y ) ;
    if ( range  <= 0 )
        return ; // range = 1 ;

    //cout << "Range is=" << range << endl ;


    max_y = max_y + ( range / divider ) ;
    if ( max_x > specif.end_y )
        max_y = specif.end_y;

    min_y = max_y - range ;
    if ( min_y < specif.beg_y )
        min_y = specif.beg_y;

    // draw_all();
    b_matrix ->force_new_pixmap ( true );
    //draw_all_on_screen();

}
//**********************************************************************
bool spectrum_2D::find_polygons_with_names_as_this_marix()
{
    // for FRS we have to set some banana gates on the matrix zet_vs_aoq
    //
    // here we try to find any polygons which names start with the name of this
    // matrix.

    // warning about erasing all existing polygon_gates
    //      ARE YOU SURE ?

    banana.clear() ;

    // find the string which should start the name of banana
    string family_name = name_of_spectrum  ;

    // cout << "Searching polygon gated for " << name_of_spectrum << endl ;

    family_name.erase ( family_name.rfind ( ".mat" ) ) ; // removing the .mat extension
    //cout << "this is the beginning of the banana filenames " << family_name << endl ;

    // looking into directory (with the filter set)

    // taking the spectra names from the directory - only
    // opening all spectra files from current directory
    QDir katalog ( gpath.polygons.c_str() ) ;


    QStringList filters;
    filters << QString ( family_name.c_str() ) + "_polygon" + "*.poly";
    katalog.setNameFilters ( filters );

    QStringList lista = katalog.entryList ( ) ;

    int banan_nr = 0 ;
    for ( QStringList::Iterator it = lista.begin() ; it != lista.end() ;  ++it, banan_nr++ )
    {
        // cout << "found files : " << *it << "\n";

        // loop of reading
        string path_filename = gpath.polygons + ( *it ).toStdString() ;
        ifstream plik ( path_filename.c_str() );
        if ( !plik )
        {
            cout << "Error in opening file " << path_filename   << endl ;
            break;
        }

        // ----------------------add new polygon
        // polygon gates
        polygon_type fence ;
        double x, y ;
        while ( !plik.eof() )
        {
            plik >> zjedz >> x >> zjedz >> y ;
            if ( !plik )
                break ;
            fence.push_back ( vertex ( x, y ) ) ;
        }

        // removing extension ?

        string disk_name = ( *it ).toStdString() ;
        // how to make screen name form it ?
        // disk name:  matrix_screen_name.poly
        //

        string::size_type nr = disk_name.rfind ( ".poly" ) ;
        string screen_name = disk_name ;
        screen_name.replace ( nr, 999, "" ); // removing ".poly" extension

        screen_name.replace ( 0,
                            family_name.length(),
                            "" );

        screen_name.replace ( 0,
                            prefix_of_polygon_names.length(),   // removing "_polygon_"
                            "" );
        // cout << "so far screen name = " << screen_name << endl ;

        polygon_gate b ( screen_name,  // screen name
                       disk_name,   // disk name
                       fence );


        //cout << "Before pushing to vector " << screen_name << endl;
        banana.push_back ( b );
        //cout << "Created polygon named " << screen_name
        //  << " now size of banana is " << banana.size()
        //  << ", last pushed polygon has vertices "
        //  << banana[banan_nr]
        //  << endl ;


        // cout << "succesfully created new polygon" << endl ;

    }
    //cout << "end of searching loop " ;

    update() ;
    return true ;

}
//****************************************************************************
void  spectrum_2D::add_new_polygon()
{
    // ----------------------add new polygon
    // polygon gates

#if 0
    bool ok;
    QString screen_name_of_polygon
            = QInputDialog::getText (
                "Adding the new polygon gate",
                "Enter the name of this polygon gate:",
                QLineEdit::Normal,
                QString::null, &ok, this );
#endif

    bool ok;
    QString screen_name_of_polygon = QInputDialog::getText ( this, tr ( "Adding the new polygon gate" ),
                                                           tr ( "Enter the name of this polygon gate:" ),
                                                           QLineEdit::Normal,
                                                           QDir::home().dirName(),
                                                           &ok );




    if ( ok && !screen_name_of_polygon.isEmpty() )
    {
        // user entered something and pressed OK



        string matr_name = windowTitle().toStdString() ;
        // removing last ".mat"
        matr_name.erase ( matr_name.rfind ( ".mat" ) );
        ostringstream ss  ;
        ss   << matr_name
           << prefix_of_polygon_names    // "_polygon_"
           << screen_name_of_polygon.toStdString()
           << ".poly"   ;// << ends ;
        string disk_name_of_polygon  = ss.str() ;



        string screen = screen_name_of_polygon.toStdString() ;

#ifdef OLD_BANAN

        int m = banana.size() ;
        //to fit to the size of current window
        typ_x szer_win = max_x - min_x ;
        typ_x wys_win = max_y - min_y ;

        polygon_gate b ( screen,
                       disk_name_of_polygon,
                       min_x + 0.3 * szer_win, max_y - 0.3 * wys_win,
                       max_x - 0.3 * szer_win, min_y + 0.3 * wys_win
                       );


        //cout << "Before pushing to vector " << screen << endl;
        banana.push_back ( b );
        //cout << "Created polygon named " << screen
        //  << " now size of banana is " << banana.size()
        //  << ", last pushed polygon has vertices "
        //  << banana[m]
        //  << endl ;
        banana[m].save_to_disk();

//cout << "succesfully created new polygon" << endl ;
#else

        QMessageBox::information ( this, "Creating the  polygon" ,
                                 QString ( "Now start clicking in the  places where you would  like  to have vertices of your polygon\n"
                                         "The polygon  will be automatically closed (and finished) when  you Double click" ) ,
                                 QMessageBox::Ok );


        b_matrix->drawing_new_banana_mode ( screen, disk_name_of_polygon );
        b_matrix->update();
        update();
#endif

    }
    else
    {
        // user entered nothing or pressed Cancel
    }
}
//*****************************************************
//*****************************************************
void spectrum_2D::black_white_spectrum ( bool on )
{
    flag_white_black = on ;
    // refresh painting
    b_matrix ->force_new_pixmap ( true );
    //draw_all_on_screen();
}
//********************************************************************************
void spectrum_2D::re_read_spectrum()   // run by timer started in init()
{
    read_in_file ( name_of_spectrum.c_str(), false ); // false = not the first time, update
    b_matrix ->force_new_pixmap ( true );
    update() ;
}
/*************************************************************** No descriptions */
void spectrum_2D::closeEvent ( QCloseEvent * e )
{
    e->accept();
    destroy();
}
//*************************************************************************
/**  no descrition */
void spectrum_2D::set_new_refreshing_times()
{
    if ( timer_refresh ) // not null
        timer_refresh->start ( 1000 * current_options.give_realistic_refreshing_time_2D() );

}
//************************************************************************
string spectrum_2D::projection ( int axis_x )
{

    polygon_gate * poly_ptr = nullptr;

    int how_many_polygons_selected = 0 ;
    for ( unsigned int i = 0 ; i < banana.size() ; i++ )
    {
        if ( banana[i].how_namy_vertices_selected() != 0 )
        {
            how_many_polygons_selected++;
        }
    }

    if ( how_many_polygons_selected == 1 )
    {

        for ( unsigned int i = 0 ; i < banana.size() ; i++ )
        {
            if ( banana[i].is_selected() )
            {
                poly_ptr = &banana[i] ;
                string tekst =  string ( "Currently a  polygon called " ) + poly_ptr->screen_name +
                               " is selected. Do you want a projection  of points only from the polygon?";

                switch ( askQuestionWithButtons(        // +
                    "Do you want a conditional projection?",
                    QString ( tekst.c_str() ) ,
                    "Only points from the  polygon",
                    "All points visible on  the matrix",
                    "Cancel", 1 )
                        )
                {
                case  1:
                    break;
                case 2:
                    poly_ptr = nullptr ;
                    break;

                default: // ?
                case 3:
                    return "";
                }

                break ;
            }
        } // endfor

    }


    bool flag_threshold_z = false;
    if ( min_z_threshold > 1 )
    {
        switch (askQuestionWithButtons(         // +
            "How to consider z axis threshold during projecting?",
            QString ( "Currently a threshold is set on Z axis of the matrix %1.\n"
                    "Should I use (for projecting) only only the points above the threshold? (so only non-black points)?"
                    ).arg ( give_spectrum_name().c_str() ) ,
            "Only points above threshold",
            "All (even black) points",
            "Cancel", 1 ) )
        {
        case 1:
            flag_threshold_z = true;
            break;
        case 2:
            flag_threshold_z = false;
            break;

        default: // ?
        case 3:
            return "";
        } // end case;
    }  // endif

    adjust_range_to_legal_values();

    //int how_many_channels = 0 ;
    int first_int_x_channel = ( int ) ( ( min_x - specif.beg ) / specif.waga )  ;
    int last_int_x_channel  = ( int ) ( ( max_x - specif.beg ) / specif.waga )  - 1;

    int first_int_y_channel = ( int ) ( ( min_y - specif.beg_y ) / specif.waga_y )  ;
    int last_int_y_channel  = ( int ) ( ( max_y - specif.beg_y ) / specif.waga_y ) - 1;

    // real integration ---------------------------------------------
    // loops goes on real channels !
    //  for(int y = first_int_y_channel ; y < last_int_y_channel ; y ++)
    //    for(int i = first_int_x_channel ; i < last_int_x_channel ; i ++)
    //    {
    //      sum += spectrum_table[y*specif.bin_y + i]    ;
    //      how_many_channels++ ;
    //    }


    // loops goes on real channels !
    double dbin ;
    if ( axis_x )
    {
        dbin = last_int_x_channel - first_int_x_channel + 1;
    }
    else
    {
        dbin = last_int_y_channel - first_int_y_channel + 1;
    }

    vector<int> proj ( ( int ) dbin ) ;

    if ( axis_x )
    {

        for ( int x = first_int_x_channel ; x <= last_int_x_channel ; x ++ )
            for ( int y = first_int_y_channel ; y <= last_int_y_channel ; y ++ )
            {
                if ( poly_ptr )
                {
                    double real_x = x * specif.waga + specif.beg;
                    double real_y = y * specif.waga_y + specif.beg_y;

                    //      cout << "Testing the point " << real_x
                    //      << ", y= " << real_y << endl;

                    // NOTE: here below - many times it had to be used bin, not bin_y !
                    // during the calculations of the position in the spectrum_table

                    if ( poly_ptr->test_inside ( real_x, real_y ) == false )
                    {
                        continue;
                    }
                }
                int ch = ( y * specif.bin ) + x;
                if ( ( flag_threshold_z && ( spectrum_table[ch] >= min_z_threshold ) ) // when threshold active, and this point is above
                    ||
                    !flag_threshold_z                               // or when we do not care of threshold
                    )
                {
                    proj[x - first_int_x_channel] += spectrum_table[ch];
                }
            }
    }
    else
    {
        for ( int y = first_int_y_channel ; y <= last_int_y_channel ; y ++ )
            for ( int x = first_int_x_channel ; x <= last_int_x_channel ; x ++ )
            {
                if ( poly_ptr )
                {
                    double real_x = x * specif.waga + specif.beg;
                    double real_y = y * specif.waga_y + specif.beg_y;

                    //      cout << "Testing the point " << real_x
                    //      << ", y= " << real_y << endl;

                    // NOTE: here below - many times it had to be used bin, not bin_y !
                    // during the calculations of the position in the spectrum_table

                    if ( poly_ptr->test_inside ( real_x, real_y ) == false )
                    {
                        continue;
                    }

                }


                int  ch = ( y * specif.bin ) + x;
                if ( ( flag_threshold_z && ( spectrum_table[ch] >= min_z_threshold ) ) // when threshold active, and this point is above
                    ||
                    !flag_threshold_z                               // or when we do not care of threshold
                    )
                {
                    proj[y - first_int_y_channel] += spectrum_table[ch];
                }
            }
    }


    // save it on disk ======================================
    string proj_name = name_of_spectrum ;
    // removing the extension
    string::size_type p = proj_name.rfind ( ".mat" );
    if ( p != string::npos )
        proj_name.erase ( p );
    proj_name += axis_x ? "_projection_x.spc" : "_projection_y.spc" ;

    string pathed_proj_name = gpath.spectra + proj_name ;
    ofstream plik ( pathed_proj_name.c_str(), ios::binary ) ;


    double bbb, eee ;
    if ( axis_x )
    {
        bbb = min_x ;
        eee = max_x ;
    }
    else
    {
        bbb = min_y ;
        eee = max_y ;
    }

    plik.write ( ( char* ) &dbin, sizeof ( dbin ) );
    plik.write ( ( char* ) &bbb, sizeof ( bbb ) );
    plik.write ( ( char* ) &eee, sizeof ( eee ) );

    for ( unsigned k = 0 ; k < proj.size() ; k++ )
    {
        int chan = proj[k];
        plik.write ( ( char* ) &chan, sizeof ( chan ) );
        if ( !plik )
            cout << "Error while writing bin nr= " << k << endl;
    }
    if ( !plik )
        cout << "Error while saving projection " << endl;

    // we may append the info after the name of the spectrum
    ostringstream info ;
    info << " Projection on "
         << ( axis_x ? " X axis " : "Y axis" )
         << " when " ;
    if ( axis_x )
        info << "y [" << min_y << ", " << max_y << "]";
    else
        info << "x [" << min_x << ", " << max_x << "]";
    if ( poly_ptr )
    {
        info << ", gated also by polygon: " << ( poly_ptr->screen_name ) ;
    }

    return plik ? ( proj_name + info.str() ) : "" ;
}
//************************************************************************
/** No descriptions */
void spectrum_2D::integrate_polygon ( polygon_gate &g )
{
    // I decide not to consider Z threshold, because it has no physics sense.

    long int sum = 0 ;
    //bool saturation = false;
    int how_many_channels = 0 ;

    // real integration ---------------------------------------------
    // loops goes on pixels channels !
    for ( int y = 0 ; y < specif.bin_y ; y ++ )
        for ( int x = 0 ; x < specif.bin  ; x ++ )
        {
            double real_x = x * specif.waga + specif.beg;
            double real_y = y * specif.waga_y + specif.beg_y;

            //      cout << "Testing the point " << real_x
            //      << ", y= " << real_y << endl;

            // NOTE: here below - many times it had to be used bin, not bin_y !
            // during the calculations of the position in the spectrum_table

            if ( g.test_inside ( real_x, real_y ) )
            {
                //                 if ( spectrum_table[y * specif.bin + x] >= 32000 )
                //                 {
                //                     saturation = true ;
                //                     cout << "bin x= " << x
                //                          << ", y= " << y
                //                          << "  (" << real_x << ", " << real_y << " has contents "
                //                          << spectrum_table[y * specif.bin + x]
                //                          << " so it is saturated "
                //                          << endl;
                //                 }
                sum += spectrum_table[y * specif.bin  + x]    ;
                //spectrum_table[y*specif.bin  + x]  = 77  ;  // orange color
                how_many_channels++ ;
                //        cout <<" -- inside" << endl;
            }
            //else spectrum_table[y*specif.bin  + x]  = 22  ;  // green color
        }


    // to have any time information =======a=========
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    // This may be important =======================================
    // Calculating angle between first and second vertex
    // in case if somebody would like to know this angle for rotation

    polygon_type::iterator it ;
    int nr = 0 ;

    double delta_x = 0;
    double delta_y = 0;
    //    cout << "Polygon called " << g.screen_name << endl;
    for(it = g.polygon.begin() ; it != g.polygon.end() ;  nr++, it++ )
    {
        // here we are jumping through coordinates
        cout << "vertex nr " << nr
             << " [ "
             << (*it).x   // x
             << ", "
             << (*it).y
             << "], "
             << endl ;

        if(nr == 0)
        {
            delta_x = -(*it).x ;
            delta_y = -(*it).y ;
        }
        if(nr == 1)
        {
            delta_x += (*it).x ;
            delta_y += (*it).y ;
        }

    } // end of for interator

    double length = sqrt((delta_x * delta_x) + (delta_y * delta_y ));

    double alfa_s = asin(delta_y / length) * 360 / (2 * M_PI);
    //    double alfa_c = acos(delta_x / length) * 360 / (2 * M_PI);

    cout  << " delta_y = " << delta_y
         << " delta_x = " << delta_x
         << " length = " << length
         << "\n sin degree " << alfa_s
         << endl;

    // When the angle is > 90 wee need some correction  ;
    // firsta quart does need any correcitona

    // 2 quart
    if(delta_x < 0 && delta_y >= 0)
    {
        alfa_s = (180 - alfa_s);
        //       cout << "po korekcie dla 1 cwartki delta_x (180 - alfa_s) " <<  << endl;
    }
    // 3 quart
    else if(delta_x < 0 && delta_y < 0)
    {
        alfa_s = (180 - alfa_s);
        //           cout << "po korekcie dla 3 cwartki delta_x (180 - alfa_s) " << (180 - alfa_s) << endl;
    }
    // 4 quart
    else if(delta_x > 0 && delta_y < 0)
    {
        alfa_s = (360 +  alfa_s);
        //           cout << "po korekcie dla czwartej ćwiartki delta_y < 0  " << (360 + alfa_s) << endl;
    }


    // using the STL class string
    string statement  ;

    ostringstream ss  ;

    ss
        << "=============================== "
        <<  asctime ( timeinfo )
        << "Integration of part of 2D spectrum "
        << windowTitle().toStdString()
        << "\ninside polygon " << g.screen_name
        <<  "\n" << how_many_channels
        << " bins had 'down left corner' inside the polygon"
        << "\nRESULT :   " << ( sum ) << "\n"

        //             << ( ( saturation ) ? "Warning: Some bins were SATURATED  (>=32000)" : "")
        << ( ( min_z_threshold > 1 )? "\nCurrently existing Z threshold was not used while this integration\n" : "")
        << "\nThe first vector of this poligon gate has angle to X axis [degrees] = "
        << alfa_s
        << "   (so to Y axis it is: " << (90 - alfa_s) << " )";
    //  << ends ;

    statement = ss.str() ;

    // showing the result text window on the screen
    integr_resul_ptr->add_results ( statement.c_str() ) ;
    integr_resul_ptr->show() ;
    integr_resul_ptr->raise() ;   // to the top

}
//***********************************************************************
void spectrum_2D::zero_spectrum()
{
    //return;
    ofstream plik ( ( gpath.spectra + name_of_spectrum ).c_str(), ios::binary );
    if ( !plik )
        return;

    cout << "spectrum_2D::zero_spectrum()   - zeroing " << name_of_spectrum << endl;

    double bin_double = specif.bin;   // no matter if  .bin is int or double, we must write it as double

    plik.write ( ( char* ) &bin_double, sizeof ( bin_double ) );

    plik.write ( ( char* ) &specif.beg, sizeof ( specif.beg ) );
    plik.write ( ( char* ) &specif.end, sizeof ( specif.end ) );
    plik.write ( ( char* ) &specif.bin_y, sizeof ( specif.bin_y ) );
    plik.write ( ( char* ) &specif.beg_y, sizeof ( specif.beg_y ) );
    plik.write ( ( char* ) &specif.end_y, sizeof ( specif.end_y ) );

    //int number = specif.bin * specif.bin_y ;

    //    int *table = new int[number];
    //    memset(table, 0, number*sizeof(int) );

    auto size = spectrum_table.size();
    //cout << "size is " << size << " for spectrum " << name_of_spectrum << ", number = " << number << endl;
    // funny - sometimes size is not equal number

    spectrum_table.clear();
    spectrum_table.resize(size);

    //    plik.write ( ( char* ) spectrum_table.data() , sizeof ( int ) * number );
    plik.write ( ( char* ) spectrum_table.data() , sizeof ( int ) * size );
    if ( !plik )
        cout << "error writing" << endl;

    plik.close() ;


    // Drawing kind of cross X to show that it was really zero-ed

    //  spectrum_table.clear() ;
    //  spectrum_table.reserve(specif.bin * specif.bin_y);
    for ( int i = 0 ; i < specif.bin ; i++ )
    {
        if ( i * specif.bin + i < ( signed ) spectrum_table.size() )
        {
            spectrum_table[i * specif.bin + i] = 1000 ;
            spectrum_table[ ( specif.bin * ( specif.bin_y - i ) )  + i] = 1000 ;
        }
    }


    b_matrix ->force_new_pixmap ( true );
    update();
    //draw_all_on_screen();
}
//********************************************************************
bool spectrum_2D::is_polygon_name_unique ( string name )
{
    for ( unsigned int i = 0 ; i < banana.size() ; i++ )
    {
        if ( banana[i].screen_name == name )
            return false ;
    }
    return true ;
}
//********************************************************************
void spectrum_2D::paste_polygon()
{
    // is it in the range of the matrix ?
    if ( clipboard_polygon->give_maxX() >= max_x
        ||
        clipboard_polygon->give_minX() <= min_x
        ||
        clipboard_polygon->give_maxY() >= max_y
        ||
        clipboard_polygon->give_minY() <= min_y )
    {
        auto answer = askYesNoCancel( "Polygon may be partly outside " ,
                                     "The pasted polygon may have some (all?) vertices outside the current view of the matrix");
        if ( answer != QMessageBox::Yes )
        {
            return;
        }
    }

    bool ok = true ;
    QString screen_name_of_polygon = clipboard_polygon->screen_name.c_str() ;

    //   if(screen_name_of_polygon == "") // somebody pressed escape, or entered empty string
    //   {
    //         return;
    //   }
    if ( !is_polygon_name_unique ( clipboard_polygon->screen_name ) )
    {
        ok = false ;
        screen_name_of_polygon
            = QInputDialog::getText ( this,
                                    "Polygon name is not unique",
                                    "Polygon with such a name already exist here, enter the new name ",
                                    QLineEdit::Normal,
                                    clipboard_polygon->screen_name.c_str() , &ok );


        //        QInputDialog::getText(this, tr("Adding the new polygon gate"),
        //                                                    tr("Enter the name of this polygon gate:"),
        //                                                 QLineEdit::Normal,
        //                                                  QDir::home().dirName(),
        //                                                 &ok);



        if ( !ok )   // ressed escape
        {
            return;
        }
    }

    if ( !is_polygon_name_unique ( screen_name_of_polygon.toStdString() ) )
    {
        showWarningMessage(
            "Choose another name for the pasted polygon" ,
            screen_name_of_polygon + " - there is already poligon with this name on this matrix, sorry.\n"
                                     "Try pasting once more and choose another name"
            );
        ok = false ;
    }
    //  cout << "Possible to paste this polygon: "<<   screen_name_of_polygon
    //  << ", ok = " << ok << endl;

    if ( ok && !screen_name_of_polygon.isEmpty() )
    {
        // user entered something and pressed OK
        //    cout << "You accepted this" << endl;

        //int m = banana.size() ;

        string matr_name = windowTitle().toStdString()  ;
        // removing last ".mat"
        matr_name.erase ( matr_name.rfind ( ".mat" ) );
        ostringstream ss  ;
        ss   << matr_name
           << prefix_of_polygon_names    // "_polygon_"
           << screen_name_of_polygon.toStdString()
           << ".poly"   ;

        clipboard_polygon->disk_name  = ss.str() ;
        //to fit to the size of current window
        clipboard_polygon->screen_name = screen_name_of_polygon.toStdString() ;



        clipboard_polygon->save_to_disk();
        banana.push_back ( *clipboard_polygon );
        flag_repaint_spectrum_box = true;
        update() ;
        //    cout << "Polygon succesfully pasted " << endl;
    }
    //  else
    //  {
    //    cout << "You rejected, of the polygon name was empty" << endl;
    //  }



}
//********************************************************************
/** To display on the menu bar */
string spectrum_2D::give_undo_text ( int /*nr*/ )
{
    return "No undo for 2D implemented yet (but: undo for polygons is in popup menu)" ;
}
//**********************************************************************
/** storing the parameters in case of later undo command */
/*----
void spectrum_2D::remember_for_undo(string s)

{

}
---------------*/


//*********************************************************************************
/** creating many polygons, which cover the full view */
void spectrum_2D::grid_of_polygons()    //
{


    bool ok;
    QString screen_name_of_polygon
        = QInputDialog::getText ( this,
                                "Adding the grid of many polygon gates",
                                "You are going to create many polygon gates which will cover the whole visible part of this matrix\n"
                                "Enter the name of this polygon gate:",
                                QLineEdit::Normal,
                                QString(), &ok );

    if ( ! ( ok && !screen_name_of_polygon.isEmpty() ) )
        return ;
    // user entered something and pressed OK

    // is it in the range of the matrix ?

    int how_many_x = 3 ;
    int how_many_y = 3 ;


    how_many_x = QInputDialog::getInt( this,
                                      "Columns of the poligon grid",
                                      "Your grid may have N rows and M columns\n"
                                      "How many columns of polygons? ",
                                      how_many_x, 1, 100, 1,
                                      &ok );

    if ( !ok )
        return ;


    how_many_y = QInputDialog::getInt ( this,
                                      "Rows of the poligon grid",
                                      "Your grid may have N rows and M columns\n"
                                      "How many rows of polygons? : ",
                                      how_many_y, 1, 100, 1,
                                      &ok );

    if ( !ok )
        return ;



    double x_width = ( max_x - min_x ) / how_many_x ;
    double y_width = ( max_y - min_y ) / how_many_y ;
    for ( int nr_x = 0 ; nr_x  < how_many_x ; nr_x++ )
    {
        for ( int nr_y = 0 ; nr_y  < how_many_y ; nr_y++ )
        {
            double ax = min_x + ( nr_x * x_width );
            double ay =  min_y + ( nr_y * y_width ) ;

            double bx = ax;
            double by = ay + y_width;

            double cx = ax + x_width;
            double cy = ay + y_width;

            double dx = ax + x_width;
            double dy = ay;

            string nnn = windowTitle().toStdString();
            string::size_type pos = nnn.rfind ( ".mat" );
            cout << "Found on the position " << pos << endl;
            if ( pos != string::npos )
                nnn.erase ( pos );

            ostringstream t;
            t << gpath.polygons <<    nnn
              << "_polygon_"
              << screen_name_of_polygon.toStdString()
              << "_x"
              << setw ( 2 )
              << setfill ( '0' )
              << nr_x
              << "_y"
              << setw ( 2 )
              << nr_y
              << ".poly";
            //<< " for x= " << x << ", y= " << y


            string pname = t.str() ;
            cout << "filename is " << pname << endl ;


            ofstream s ( pname.c_str() ) ;

            s
                << ax << "\t" << ay << "\n"
                << bx << "\t" << by << "\n"
                << cx << "\t" << cy << "\n"
                << dx << "\t" << dy << endl;

            s.close() ;

        } // end for
    } // end for


}
//*******************************************************************************

/** No descriptions */
void spectrum_2D::adjust_range_to_legal_values()
{

    if(min_x > max_x) swap(min_x, max_x);
    if(min_y > max_y) swap(min_y, max_y);

    // check if the min, max values are not illegal
    if ( min_x < specif.beg )
        min_x = specif.beg;

    if(min_x >= specif.end)    // possible if ranges are coming from other matrix
        min_x = specif.beg;

    if ( min_y  < specif.beg_y  || min_y >= specif.end_y )
        min_y = specif.beg_y;

    if ( max_x > specif.end  || max_x <= specif.beg)
        max_x = specif.end;

    if ( max_y > specif.end_y  || max_y <= specif.beg_y)
        max_y = specif.end_y;
}
//*******************************************************************************
/** No descriptions */
void spectrum_2D::ask_z_threshold()
{
    //  remember_for_undo("Set threshold of z axis") ;

    int old_z_min = b_matrix->give_z_threshold();
    bool ok;
    int value = QInputDialog::getInt ( this,
                                     "Setting the threshold for z axis (counts)",
                                     "Type the threshold value, [below which all points of matrix are drawn as black]\n(1 - means no threshold)",
                                     old_z_min, 1, 2147483647, 1, &ok );
    if ( ok )
    {
        // user entered something and pressed OK
        min_z_threshold = value;
        // update() ;
        b_matrix->set_z_threshold ( ( int ) min_z_threshold );
        b_matrix->force_new_pixmap ( true ) ;
        //draw_all_on_screen();

    }
    else
    {
        // user pressed Cancel
    }
}
//*************************************************************************
/** User defined comments, to make hardcopy nicer */
void spectrum_2D::add_tag_with_comment ( double x, double y )
{
    //  remember_for_undo("Add tag comment") ;
    static QStringList lst ;
    // lst << "" ;
    bool ok;
    QString res = QInputDialog::getItem ( this,
                                        "Adding a tag with a comment",
                                        "Type your comment string or choose the old one to edit it:",
                                        lst, lst.size() - 1 , true, &ok );
    if ( ok )
    {
        // user selected an item and pressed OK
        nalepka.push_back ( Tpinup ( x, y, res.toStdString() ) ) ;
        lst += res ;
        flag_repaint_spectrum_box = true;
        update();
    }
    else
    {
        // user pressed Cancel
    }

}
//*******************************************************************
/** No descriptions */
void spectrum_2D::erase_nearest_tag_with_comment ( double x, double y )
{
    //  remember_for_undo("Erase the nearest TAG with comment") ;
    // cout << "---------- x = " << x << ", y= " << y << endl ;

    if ( nalepka.size() == 0 ) {
        return ; // no tags at all
    }

    uint nearest = 0 ;
    double minimum = 999999 ;
    for ( unsigned int i = 0 ; i < nalepka.size() ; i++ )
    {

        double distance =
            sqrt ( ( ( x - nalepka[i].give_x() ) * ( x - nalepka[i].give_x() ) )
                 + ( ( y - nalepka[i].give_y() ) * ( y - nalepka[i].give_y() ) )
                 );
        //    cout << " dist of [" << nalepka[i].give_x()
        //      << ", "<< nalepka[i].give_y()
        //      << "] "
        //      << nalepka[i].give_info()
        //      << " = " << distance << endl;
        if ( distance < minimum )
        {
            minimum = distance ;
            nearest = i ;
        }
    }


    string mmm = "Are you sure you want to delete TAG: "
                 + nalepka[nearest].give_info() ;
    auto result = askYesNoCancel( "Deleting the TAG " , mmm.c_str());
    if ( result == QMessageBox::Yes )
    {
        // are you sure....
        nalepka.erase ( nalepka.begin() + nearest );
        flag_repaint_spectrum_box = true;
        update();
    }

}
//*************************************************************************
/** No descriptions */
void spectrum_2D::remove_all_tags()
{
    //remember_for_undo("Remove all tags") ;
    //  string mmm = "Are you sure you want to delete all tags from this spectrum ?";
    auto result =askYesNoCancel("Deleting the all TAG " ,
                                 "Are you sure you want to delete all tags from this spectrum ?"    );
    if ( result == QMessageBox::Yes )
    {
        // are you sure....
        nalepka.clear() ;
        flag_repaint_spectrum_box = true;
        update();
    }
}
/*************************************************************************/
/*!
\fn spectrum_2D::is_possible_to_erase_this_polygon(string banana_name)
checks is some condition is not using this banana
*/
bool spectrum_2D::is_possible_to_erase_this_polygon ( string banana_name )
{
    /*loop over all conditions available on the disk and checking if  any of them
    is using a banana with this name
    */
    //cout << " F. is_possible_to_erase_this_polygon(string banana_name)" << endl;

    bool possible = true;
    string list_of_conditions_which_uses;

    // taking the spectra names from the directory - only
    QDir katalog2 ( gpath.Qconditions() );
    // in the condition dir

    QStringList filters;
    filters << user_cond_filter.c_str() ;
    katalog2.setNameFilters ( filters );


    QStringList lista = katalog2.entryList ( ) ;
    int nr = 0 ;
    for ( QStringList::Iterator it = lista.begin() ; it != lista.end() ;  ++it, nr++ )
    {
        //    cout << "Trying to read condition definition " << *it << endl;
        string name_c = gpath.conditions + ( *it ).toStdString();

        // Opening the definition and checking  if there is a banana name here
        ifstream plik ( name_c.c_str() );
        if ( !plik )
        {
            cout << "Error while opening condition definition " << name_c << endl;
            possible = false;
            continue ; // error wile opening
        }

        string word;
        while ( !plik.eof() )
        {
            plik >> word ;
            if ( !plik )
                break;

            if ( word == banana_name )
            {
                // this condition is using the given polygon, so can't be erased
                ostringstream numerek ;
                numerek << nr + 1 ;
                list_of_conditions_which_uses += numerek.str() + ( ") " ) + ( *it ).toStdString() + ",    " ;
                //  cout << "Found the name " <<  banana_name
                //   << " iniside this condition " << name_c << endl;
                possible = false;
                break;
            }
        } // end of while reading one condition file

    } // end of reading all conditonis

    if ( !possible )
    {
        // removing from the disk file
        string mmm ;
        mmm += "The polygon named\n";
        mmm +=  banana_name;
        mmm += " \ncan not be removed , because it us used by condition(s):\n";
        mmm += list_of_conditions_which_uses;
        mmm += "\n(You can delete a condition - using the the condtion manager)";

        showWarningMessage( "Impossible to delete the polygon gate",
                           mmm.c_str(),
                           QMessageBox::Critical); // critical error

    }// end if impossible
    return possible;
}
//***************************************************************************
void spectrum_2D::show_list_of_incrementers()
{
    QMessageBox::information ( this, "List of X and Y incrementers of this 2D spectrum",
                             list_of_incrementers_of_this_spectrum.c_str(),
                             QMessageBox::Ok );

}
//***********************************************************************
void spectrum_2D::show_time_of_last_zeroing()
{
    time_t when  =
        appl_form_ptr->give_time_of_zeroing_spectrum ( name_of_spectrum );

    string mess ;
    if ( when )
    {
        mess =  string ( "Last zeroing of the spectrum " ) + name_of_spectrum
               + " was at:  " + ctime ( &when ) ;
    }
    else
    {
        mess = "Sorry, an information about the zeroing of this spectrum is currently not available";
    }
    QMessageBox::information ( this,
                             "Time of last zeroing",
                             mess.c_str(),
                             QMessageBox::Ok );
}
//*******************************************************************
void spectrum_2D::draw_scale_or_not ( bool stan )
{
    flag_draw_scales = stan;

    recalculate_my_geometry();

    // nonsense to :   remember_for_undo("log/linear view") ;
    b_matrix ->force_new_pixmap(true);

}
//*******************************************************************
void spectrum_2D::read_tags_from_disk()
{
    // opening the file
    string name = gpath.my_binnings + name_of_spectrum + ".pinuptxt" ;
    // cout << "Trying to open the file: >" << name << "<" << endl;
    ifstream s ( name.c_str() );
    if ( !s )
    {
        //cout << name << " - pinuptxt file does not exist" << endl;
        return;
    }
    // loop over the tags
    for ( unsigned int i = 0 ;  ; i++ )
    {
        //     cout << "Reading nalepka nr " << i << endl;
        Tpinup nal;

        nal.read_from_disk_file ( s );
        if ( !s || s.eof() )
            break;
        nalepka.push_back ( nal );
    }
    s.close();
}
//*******************************************************************
void spectrum_2D::save_tags_to_disk()
{
    // cout << "spectrum_2D::save_tags_to_disk() for " << name_of_spectrum << endl;
    string name = gpath.my_binnings + name_of_spectrum + ".pinuptxt" ;

    if ( nalepka.size()  == 0 )
    {
        //   cout << "removing the file" << name << endl;
        remove
            ( name.c_str() );
        return;
    }

    // opening the file
    ofstream s ( name.c_str() );
    if ( !s )
    {
        cout << "Can't open file " << name << " for writing " << endl;
        return;
    }
    // loop over the tags
    for ( unsigned int i = 0 ; i < nalepka.size() ; i++ )
    {
        //     cout << "Saving nalepka nr " << i << endl;
        nalepka[i].save_to_disk_file ( s );
    }
    s.close();
}
//*******************************************************************
void spectrum_2D::slot_delete_polygon()
{
    // ----------------------delete a polygon, also from the disk file  !!

    if ( more_than_one_polygon_selected )
    {
        error_more_than_one_polygon_selected() ;
        return;
    }

    for ( unsigned int i = 0 ; i < banana.size() ; i++ )
    {
        if ( banana[i].is_selected() )
        {
            if ( is_possible_to_erase_this_polygon ( banana[i].disk_name ) )
            {
                // removing from the disk file
                string mmm = "The polygon named:\n "
                             + banana[i].screen_name
                             + " \nhas a disk name: \n"
                             + ( gpath.polygons + banana[i].disk_name )
                             + " \nwill be removed from the program.\nAre you sure ?" ;

                auto result = askYesNoCancel("Deleting the polygon gate" , mmm.c_str());
                if ( result == QMessageBox::Yes )
                {
                    banana[i].remove_from_disk() ;
                    banana.erase ( banana.begin() + i ) ; // from the memory
                    b_matrix->inform_that_banana_just_deletd();
                    break ;
                }
            } // end if possible - no condition is using this banana
        } // end  if selected
    } // end for loop
    //cout << "succesfully removed polygon" << endl ;

    flag_repaint_spectrum_box = true;
    update();

}
//*******************************************************************
void spectrum_2D::slot_remove_vertex ( )
{
    // ----------------------- remove selected  ---------------------
    //cout << "Before removing " << endl ;


    switch ( total_vertices_selected )
    {
    case 0:
        // warning - no selected vertices
        QMessageBox::information ( this, "No selected vertices !" ,
                                 QString ( "At first please select one vertex by clicking on it." ) ,
                                 QMessageBox::Ok );
        break ;
    default :  // warning: more than one seleced
    {
        auto result = askYesNoCancel(
            "More than one vertex selected,   are you sure ?" ,
            QString ( "Now, %1 vertices are selected. Do you really remove all of them?"
                    )  .arg ( total_vertices_selected )
            );


        if ( result != QMessageBox::Yes )
            break ;
        // cout<< "The answer was OK" << endl ;
        // else, we just go down, without break statement
    }
        [[fallthrough]];
    case 1:
        for ( unsigned int i = 0 ; i < banana.size() ; i++ )
            banana[i].remove_selected_vertices();
    } // endo of small switch
    //cout << "after removing " << endl ;
    flag_repaint_spectrum_box = true;
    update();


}
//**************************************************************************
void spectrum_2D::slot_add_vertices ( )
{
    // ----------------------- add vertices

    // cout << "Before " << __func__ << endl ;
    // warning
    switch ( total_vertices_selected )
    {
    case 0:
        // warning - no selected vertices
        QMessageBox::information
            ( this,
             "No selected vertices !" ,
             QString ( "At first please select one vertex by clicking on it." ) ,
             QMessageBox::Ok );
        break ;

    default :  // warning: more than one seleced
    {
        auto result = askYesNoCancel( "Warning" ,
                                     QString ( "Now, %1 vertices are selected. Do you really want to put a new vertex after "
                                             "EACH of them ? " )  .arg ( total_vertices_selected )
                                     );
        if ( result != QMessageBox::Yes )
            break ;
        // else = no break here
    }
        [[fallthrough]];

    case 1:   // ---- one selected vertex -----------
        for ( unsigned int i = 0 ; i < banana.size() ; i++ )
        {
            banana[i].add_vertex_after_selected_vertices();
            //cout << "In polygon " << banana[i] << endl;
        }
        break;
    } // end of smal switch total_vertices_selected
    //cout << "after adding " << endl ;
    flag_repaint_spectrum_box = true;
    update();

}
//*************************************************************************************************
void spectrum_2D::slot_deselect_all_vertices()
{
    // ---------------------- deselect all
    for ( unsigned int i = 0 ; i < banana.size() ; i++ )
        banana[i].deselect_all_vertices();

    flag_repaint_spectrum_box = true;
    update();
}
//*************************************************************************************************
void spectrum_2D::slot_integrate_polygon()
{
    //         if(more_than_one_polygon_selected)
    //         {
    //           error_more_than_one_polygon_selected() ;
    //           break ;
    //         }

    // update();
    setCursor ( Qt::WaitCursor ) ;
    for ( unsigned int i = 0 ; i < banana.size() ; i++ )
    {
        if ( banana[i].is_selected() )
        {
            integrate_polygon ( banana[i] );
        }
    }

    unsetCursor();
}
//*****************************************************************************************************
void spectrum_2D::slot_copy_polygon()
{
    // copy
    if ( more_than_one_polygon_selected )
    {
        error_more_than_one_polygon_selected() ;
        return;
    }
    for ( unsigned int i = 0 ; i < banana.size() ; i++ )
    {
        if ( banana[i].is_selected() )
        {
            //polygon_gate
            if ( !clipboard_polygon )
                clipboard_polygon = new polygon_gate;
            *clipboard_polygon = banana[i] ;
            flag_polygon_in_clipboard = true;
            break ;
        }
    }
    //update() ;

}
//****************************************************************************************************
void spectrum_2D::slot_help_about_moving_vertices()
{

    showWarningMessage(
        "Help information about changing position of vertices" ,
        QString ( "You may:\n"
                "   - move just one vertex of a polygon,\n"
                "   - move all selected vertices of a polygon,\n"
                "   - move a whole polygon.\n\n\n "

                "To move just one vertex:\n"
                "     - just clik on it and drag to a new position\n\n\n"

                "To move whole polygon \n"
                "     - Just clik on any vertex (while pressing Ctrl) and start draging a polygon to a new position\n\n\n"

                "To move only some selected vertices:\n"
                "     1. Click on a first chosen vertex\n"
                "           (it will become white)\n"
                "     2. Press Shift key while clicking on all next chosen vertices\n"
                "           (they will also become white)\n"
                "     3. Press Ctrl + Shift key and click (next to some vertex) and start dragging into new position\n\n\n"
                "Remember: you can always UNDO this operation (it is another option of popup menu)"
                ) ,
        QMessageBox::Information);
}
//****************************************************************************************************
void spectrum_2D::slot_help_about_selecting_vertices()
{

    showWarningMessage("Help information about selecting vertices of polygon" ,
                       QString ( "You may:\n"
                               "   - select vertices just by clicking on them\n"
                               "   - select vertices by a marking a region using a `rubber band`.\n\n\n"

                               "To select just one vertex:\n"
                               "     - Just clik on it. All other vertices will become un-selected\n\n"

                               "To continue selecting next vertices:\n"
                               "     - Pressing Shift - clik on next vertices.\n\n\n"

                               "USING A RUBBERBAND (more convenient) -----------------------------------------\n\n"

                               "You may also mark a region where all vertices should be selected\n"
                               "     - Click a mouse and dragg \n"
                               "       You will see a red-line marked region. When you finish:\n"
                               "                - all vertices inside this region will become selected, \n"
                               "                - all outside this region will become un-selected.\n\n\n"

                               "You may also mark a region where all vertices should be selected\n"
                               "without un-selecting the others:\n"
                               "     - Press Shift while Clicking a mouse and dragging\n"
                               "       You will see a red-line marked region. When you finish:\n"
                               "                 - all vertices inside this region will become selected, \n"
                               "                 - all outside this region will become as they were before.\n\n\n"
                               "To deselect all vertices - just mark a small rubberband region in a neutral place."
                               ) ,
                       QMessageBox::Information);
}
//***************************************************************************************************
void spectrum_2D::slot_help_about_mouse_clicking()
{
    showWarningMessage( "Help information about mouse clicking on 2D spectra" ,
                       QString (
                           "On the 2D spectrum display you may use following actions with a mouse:\n\n"
                           "   *  Double  click   -  to zoom IN the region around the clicked place\n"
                           "   *  Shift double  click   -  to zoom OUT the region around the clicked place\n"
                           "   *  Right click - to see a context popup menu\n\n\n"
                           "On a X-axis box (or on a Y-axis box) of the 2D spectrum you may:\n\n"
                           "   *  Roll a mouse wheel -  to  expand/compress the scale\n"
                           "   *  Press a mouse and drag  - to move along this axis (if possible)\n"
                           "\n"
                           ) ,
                       QMessageBox::Information);
}
//***************************************************************************************************
void spectrum_2D::set_layout_normal()
{

    //    cout << "spectrum_2D::set_layout_normal()" << endl;

    flag_impossible_to_draw_scales = false;
    layout->invalidate();

    //    layout->setColumnStretch(0,3);
    //    layout->setColumnStretch(1,97);

    //    layout->setRowStretch(0,97);
    //    layout->setRowStretch(1,3);


    layout->setColumnStretch(0,3);
    layout->setColumnStretch(1, 90);  // (1,90);
    layout->setColumnStretch(2,3);          // 3

    layout->setRowStretch(0,97);
    layout->setRowStretch(1,3);


    layout->setColumnMinimumWidth(0, 15);
    layout->setRowMinimumHeight(1, 15);
    layout->setSpacing(1);

    setLayout(layout);
}
//***************************************************************************
void spectrum_2D::set_layout_without_scale()
{
    //    cout << "spectrum_2D::set_layout_without_scale()" << endl;

    flag_impossible_to_draw_scales = true;

    layout->invalidate();

    layout->setColumnStretch(0,1);
    layout->setColumnStretch(1,1000);
    layout->setRowStretch(0,1000);
    layout->setRowStretch(1,0);


    layout->setColumnMinimumWidth(0, 0);
    layout->setRowMinimumHeight(1, 0);
    layout->setSpacing(0);

    setLayout(layout);
    //    b_matrix->force_new_pixmap ( true ) ; // it is a general change
    //    repaint() ; //update();

}
//***************************************************************************
int askQuestionWithButtons(const QString& title,            // +
                           const QString& question,
                           const QString& option1,
                           const QString& option2,
                           const QString& option3,
                           int defaultOption,
                           QMessageBox::Icon  iconType,
                           QWidget* parent)
{
    // Tworzymy QMessageBox z pytaniem
    QMessageBox messageBox(parent);
    messageBox.setIcon(iconType);
    messageBox.setWindowTitle(title);   // Ustawienie tytułu

    messageBox.setText(question);

    // Dodajemy trzy przyciski odpowiedzi
    QPushButton* button1 =  messageBox.addButton(option1, QMessageBox::ActionRole);
    QAbstractButton* abutton1 = (QAbstractButton*) button1;

    QPushButton*  button2 = messageBox.addButton(option2, QMessageBox::ActionRole);
    QAbstractButton* abutton2 = (QAbstractButton*) button2;

    QPushButton*  button3 = messageBox.addButton(option3, QMessageBox::ActionRole);
    QAbstractButton* abutton3 = (QAbstractButton*) button3;

    // Ustawiamy domyślnie zaznaczony przycisk
    switch (defaultOption) {
    case 1:
        messageBox.setDefaultButton( button1);
        break;
    case 2:
        messageBox.setDefaultButton(button2);
        break;
    case 3:
        messageBox.setDefaultButton(button3);
        break;
    default:
        // Nie ustawiamy domyślnego przycisku, jeśli numer nie jest 1-3
        break;
    }

    // Wyświetlamy okno dialogowe
    messageBox.exec();

    // Sprawdzamy, który przycisk został naciśnięty, i zwracamy odpowiedni numer
    if (messageBox.clickedButton() == abutton1) {
        return 1;
    } else if (messageBox.clickedButton() == abutton2) {
        return 2;
    } else if (messageBox.clickedButton() == abutton3) {
        return 3;
    }

    return -1; // Wartość domyślna w przypadku błędu
}
//*************************************************************************************
void showWarningMessage(const QString& title, const QString& warningText,
                        QMessageBox::Icon  iconType,
                        QWidget* parent )
{
    // Tworzymy QMessageBox z ostrzeżeniem
    QMessageBox messageBox(parent);
    messageBox.setIcon(iconType);      // Ustawienie ikony na "Warning"
    messageBox.setWindowTitle(title);             // Ustawienie tytułu okna
    messageBox.setText(warningText);              // Ustawienie tekstu ostrzeżenia

    // Dodanie przycisku OK
    messageBox.addButton(QMessageBox::Ok);

    // Wyświetlenie okna dialogowego
    messageBox.exec();
}
//*************************************************************************************
QMessageBox::StandardButton askYesNoCancel(const QString& title,
                                           const QString& question,
                                           QWidget* parent)
{
    // Tworzymy QMessageBox z pytaniem
    QMessageBox messageBox(parent);
    messageBox.setIcon(QMessageBox::Question);   // Ikona pytania
    messageBox.setWindowTitle(title);           // Tytuł okna
    messageBox.setText(question);               // Treść pytania

    // Dodanie trzech przycisków: Yes, No, Cancel
    QPushButton* yesButton = messageBox.addButton(QMessageBox::Yes);
    QPushButton* noButton = messageBox.addButton(QMessageBox::No);
    QPushButton* cancelButton = messageBox.addButton(QMessageBox::Cancel);

    // Ustawienie domyślnego przycisku (opcjonalnie)
    messageBox.setDefaultButton(QMessageBox::No);

    // Wyświetlenie okna dialogowego i czekanie na odpowiedź użytkownika
    messageBox.exec();

    // Zwracanie typu przycisku wybranego przez użytkownika
    if (messageBox.clickedButton() == (QAbstractButton*) yesButton) {
        return QMessageBox::Yes;
    } else if (messageBox.clickedButton() == (QAbstractButton*)noButton) {
        return QMessageBox::No;
    } else if (messageBox.clickedButton() == (QAbstractButton*) cancelButton) {
        return QMessageBox::Cancel;
    }

    // Jeśli nie można określić przycisku (teoretycznie nie powinno się zdarzyć)
    return QMessageBox::NoButton;
}
//*****************************************************************************************************
