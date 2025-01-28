
#include "spectrum_1D.h"
#include "box_of_spectrum.h"
#include "box_of_channels.h"
#include "box_of_counts.h"
#include <math.h>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <algorithm>
#include <string>
//#include <qinputdialog.h>
//#include <qstringlist.h>
#include <QStringList>
//#include <QFileDialog>
#include <QInputDialog>


//#include <qpaintdevicemetrics.h>
#include <ctime>


#include "gate_1D.h"
#include <QTimer>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>


#include "paths.h"
#include "options.h"
extern Tpaths path;
extern options current_options;
#include <t4markers_manager.h>

//#undef COTO
//#define COTO

extern istream & zjedz(istream & plik) ;


#include "appl_form.h"
extern appl_form  *appl_form_ptr;

// static
std::vector<std::vector<typ_x>>  spectrum_1D::set_of_markers;
//*********************************************************************
//********************************************************************
void spectrum_1D::paintEvent(QPaintEvent *)
{


    //    cout << "-----spectrum_1D::PaintEvent " << windowTitle().toStdString() << endl ;
    if(flag_repainting_allowed)  draw_all_on_screen();
    //    cout << "-----End of spectrum_1D::PaintEvent " << windowTitle().toStdString() << endl ;

}
//********************************************************************
void spectrum_1D::init()
{
    // this function is a part of the constructor of the base class.
    // so no matter if it is virtual - it will be called anyway.

    //  cout << " this is init() of spectrum_1D, RTTI =  "
    //    << typeid(*this).name()
    //    << endl ;

    // por. chapter "calling the virtual function form the constructor
    // of the base class"  in my Symfonia

    setMouseTracking ( true );
    dimension = 1 ;   // 1D spectrum

    prefix_of_gate_1D_names = "_gate_" ;
    //    flaga_this_is_printing = false ;
    flag_log_scale =  false;
    flag_draw_scales    =    appl_form_ptr->give_flag_draw_spectra_scales() ;
    //flag_white_black = false ;  // ask the owner !

    re_read_counter = 0 ;




    //    Ax = 0.01;
    //    Bx = 0.15;
    //    Cx = 0.20 ;
    //    Dx = 0.95;

    //    Ay = 0.01;
    //    By = 0.15;
    //    Cy = 0.2;
    //    Dy = 0.92;



    b_counts = new box_of_counts(this, false, "scaleY");  // false --> no rainbow scale
    b_channels = new box_of_channels(this, "scaleX");
    b_spectrum = new box_of_spectrum(this, "spectrum");
    b_spectrum ->force_new_pixmap(true);

    //#ifdef RAINBOW_SCALE
    //	b_scale =  new box_of_counts (this, true, "scaleZ" );   // true -> with rainbow scale
    //#endif

    layout = nullptr;
    layout = new QGridLayout;
    layout->addWidget(b_counts, 0,0);
    layout->addWidget(b_spectrum, 0,1);
    layout->addWidget(b_channels, 1,1);
    //#ifdef RAINBOW_SCALE
    //	layout->addWidget(b_scale, 0,2);
    //#endif
    set_layout_normal() ;
    recalculate_my_geometry() ;



    b_spectrum->set_spectrum_pointer(&spectrum_table, & specif);

    min_x = min_counts = 0 ;
    rebin_factor = 1; // means: no rebining, every channel is drawn separately

    // max_channel = spectrum_length-1 ;
    max_counts = 250 ;

#ifdef NIGDY
#endif // NIGDY

    marker_older = marker_younger = 0;   // integration and expansion markers

    bgr_slope = 0;
    bgr_offset = 0 ;
    flag_bgr_valid = false ;
    recent_bgr_marker = -999999 ;  // nothing have been set yet


    find_gates_with_names_as_this_spectrum();
    find_notice_in_description();
    nalepka_notice.set_info(give_notice()) ; // real coordinates during painting

    // reading the tag notices from disk

    read_tags_from_disk();


    // ---------- timer to refresh the spectrum from the disk ------
    timer_refresh = new QTimer(this);
    connect(timer_refresh, SIGNAL(timeout()),
            this, SLOT(re_read_spectrum()));
    timer_refresh->start(1000 * current_options.give_realistic_refreshing_time_1D()); // (1000 is 1 second)

    //    connect(sig_resize, SIGNAL(timeout()),
    //            b_counts,   SLOT(re_read_spectrum()));
}
//**************************************************************************
void spectrum_1D::resizeEvent(QResizeEvent * /*r*/  )
{
    //cout << "spectrum_1D::resizeEvent(QResizeEvent *) " << int(time(NULL) )<< endl;


    // perhaps here we have to recalculate geometry of all the boxes
    recalculate_my_geometry();

    return;
    // here we have to send a new walues
    //    b_counts->new_factors(Ax, 1 - Dy, Bx, 1 - Cy);
    //    b_channels->new_factors(Cx, 1 - By, Dx, 1 - Ay);
    //    b_spectrum->new_factors(Cx, 1 - Dy, Dx, 1 - Cy);

    //    b_counts->update();
    //    b_counts->resizeEvent(r); //  repaint();


    //    b_channels->update();
    //    b_spectrum->update();
}
//***************************************************************************
void spectrum_1D::destroy()   // destructor
{
    // since now it is a vector, we do not neeed to destroy it manually
    //delete [] spectrum_table;
    if(timer_refresh)
    {
        timer_refresh->stop(); // 20 seconds
        delete timer_refresh ;
        timer_refresh = nullptr ;
    }
    save_tags_to_disk();


    // how do we destroy box_matrix box_channels etc. ?
    delete b_channels;
    delete b_counts;
    delete b_spectrum;
    b_channels 	= nullptr;
    b_counts = nullptr;
    b_spectrum = nullptr;

}
//****************************************************************************
/* This function calculates (in pixels) the positios of the ruling lines
of the spectrum document. It makes it depending of the current size
of the spectrum widget.
------------------------------------------------------------------------------------------*/
void spectrum_1D::recalculate_my_geometry()
{
    QSize s = size() ;
    //    if(flaga_this_is_printing)
    //    {
    //        s.setWidth(400) ;
    //        s.setHeight(300);
    //    }


    if(s.height() < 80 ||  (flag_draw_scales == false) )
    {
        set_layout_without_scale();
    }else{
        set_layout_normal();
    }

    //	double pixels5 =   0;
    //	if(flag_draw_scales  &&  !flag_impossible_to_draw_scales)
    //	{
    //		pixels5 = 2;
    //	}

    flag_repaint_spectrum_box = 1;
    return;

// ================================================

#if 0

    // calculating the standard boarder
    // double pixels5 =  flag_draw_scales? 2 :   0;      //1.0 ; //5.0 ;


    double boarderX = pixels5  / s.width()  ;


    // calculating the geometry of all the boxes
    Ax = boarderX;    // left edge of the scale of counts
    Bx = Ax + 5 * boarderX  ; //  5 digits of text  ; // right edge of the scale of counts

    // CHANGE
    Bx = Ax + 11 * boarderX  ; //  5 digits of text  ; // right edge of the scale of counts


    //Cx = Bx + boarderX ; // left edge of the spectrum filed
    Cx = Bx ;

    Dx = 1 - boarderX; // right edge of the spectrum field

    // vertical geometry
    double boarderY = pixels5 / s.height()  ;

    //double aspect = ((double)s.height()) / s.width()   ;

    Ay = boarderY ; // bottom edge of channels
    By = Ay + 11 * (boarderY) ; // 2* wys_fontu (10 pix) ;


    // to not to make channel scale (and font ) too high ,
    // when it is narrow widget

    /*---------
    cout      << " original       Ay=" << Ay << "  By=" << By
    << "  By-Ay" << By-Ay
    //<< "  Dx=" << Dx
    //<< "  Cx=" << Cx
    //<< "  Dx-Cx" << Dx-Cx
    << " aspect = " << aspect
    <<   endl ;
    ----*/

    if(s.width()   < 250)
    {
        By = By / 1.5  ;
        // cout << " recalculated Ay=" << Ay << " By=" << By
        //<< " this aspect = " << aspect
        // << endl ;
    }

    // Cy = By + boarderY; // bottom edge of spectrum box  OLD
    Cy = By; // bottom edge of spectrum box NEW

    Dy = 1 - boarderY ;


    /*--
    cout << "After recalculation "
    << "  Ax=" << Ax
    << "  Bx=" << Bx
    << "  Cx=" << Cx
    << "  Dx=" << Dx
    << " s.width()=" <<  s.width()
    << endl ;
    ---*/

    //  b_spectrum->new_factors(Cx, 1-Dy, Dx, 1-Cy);
#endif  // 0

}
//**************************************************************************
//int spectrum_1D::give_max_channel_skas()
//{

//	// return (max_channel) ;
//	return spectrum_table.size() ;

//}
//***************************************************************************
// function called from the toolbar (QMdiArea)
void spectrum_1D::expand()
{
    //cout << "zwykly ekspand" << endl ;
    if(fabs((double)(marker_older - marker_younger)) <= 0)
    {
        //cout << "rejected expansion\n" << endl ;
        return ;  // do not expand
    }

    remember_for_undo("Expand");
    if(marker_older < marker_younger)
    {
        min_x = marker_older ;
        max_x = marker_younger ;
    }
    else
    {
        min_x = marker_younger ;
        max_x = marker_older ;
    }
    //draw_all();
    // cout << "Expand, in region " << min_x << " to " << max_x << endl ;
    b_spectrum ->force_new_pixmap(true);
    flag_repaint_spectrum_box = 1;
    //draw_all_on_screen();
    appl_form_ptr->send_to_statusbar(
        "Tip: You can also expand  quicker:  by setting the second yellow marker with Double Click",
        15000);

}
//***************************************************************************
void spectrum_1D::expand_double_click(typ_x pos_x, typ_x  /*pos_y*/, bool shift)
{
    remember_for_undo("Expand by double click");
    //cout << "double click expand function" << endl ;
    // pos_y = pos_y ; // to avoid warnings

    // here wa a procedure to expand 50% around the point of clicking.
    // but finally I decided to expand in classical way.
    // Note: DEexpanding is make in a new way

    // expanding by 50 %
    if(!shift)
    {
        expand() ;   // classical
        //return ;
    }
    else
    {
        // if there was shift, so deexpanding
        typ_x  now_width = max_x - min_x ;
        typ_x  future_half_width = 0.3 * (now_width / 2) ; // new way of expanding, not liked by me...
        if(shift)
            future_half_width = 3 * (now_width / 2) ; // deexpanding

        min_x  = max(specif.beg,   pos_x - future_half_width) ;
        max_x = min(specif.end,  pos_x + future_half_width) ;

        b_spectrum ->force_new_pixmap(true);
        //        draw_all_on_screen();
    }

}
//****************************************************************************
void spectrum_1D::mousePressEvent(QMouseEvent *e)
{
    //cout << "F. spectrum_1D::mousePressEven " << endl ;
    //cout << " w polu dialogowym Pixl=" << e->x()
    //      << "means channel =" << b_spectrum->pix2worX(e->x() )
    //      << endl ;
    e->accept() ;
    if(b_spectrum-> was_mouse_click())    // if it was in a spectrum field
    {
        // COTO;
        if(e->type() == QEvent::MouseButtonDblClick)
        {
            // Warning, at first widget gets a mouspress event, and then Doubleclik.
            // so it will be here around two times...
            COTO;
            //cout << "to byl doubleclick  1D" << endl ;
            //expand() ;
        }
        else
        {
            //  COTO;
            //   cout << "to byl single click  1D" << endl ;
            // dowiadujemy sie jaki to byl klawisz

            if(e->button() & Qt::LeftButton)
            {
                // it will be about the marker
                // remember the new
                remember_as_marker(b_spectrum->give_x_mouse_click()) ;
                b_spectrum->set_markers(marker_younger, marker_older);
                //   appl_form_ptr->statusBar()->message(
                //   "Tip: Shift Click -allows you to correct a wrong  setting of the last yellow marker");
            }
            else if(e->button() & Qt::RightButton)
            {
                //                cout << "F. spectrum_1D::mousePressEven - RIGHT button" << endl ;
                QMenu *context_Menu = new QMenu;

                context_Menu->addAction("Help about mouse clicking", this, SLOT(slot_help_about_1D_mouse_clicking() ) );
                context_Menu->addSeparator();
                context_Menu->addAction("SET background marker at this point", this, SLOT(slot_set_bg_marker() ));
                context_Menu->addAction("erase RECENT  background marker", this, SLOT(slot_erase_recent_bg_marker() ));
                context_Menu->addAction("erase the NEAREST background marker", this, SLOT(slot_erase_nearest_bg_marker() ));
                context_Menu->addAction("erase ALL background markers", this, SLOT(slot_erase_all_bg_markers() ));

                context_Menu->addSeparator();

                context_Menu->addAction("[Obsolate] Create the default GATE from current background markers", this, SLOT(slot_save_bg_markers_as_default_gate() ));
                context_Menu->addAction("Remove one of the GATEs", this, SLOT(slot_remove_default_gate() ));

                context_Menu->addSeparator();
                // context_Menu->addAction("Remember the Set of Integration and Bgr markers", this, SLOT(slot_remember_all_markers() ));
                context_Menu->addAction("Manager of sets of Integ and Bgr markers...", this, SLOT(slot_paste_all_markers() ));

                context_Menu->addSeparator();

                context_Menu->addAction("place a yellow integration marker in channel nr...", this, SLOT(slot_place_integ_marker_by_value() ));
                context_Menu->addAction("Rebin on screen...", this, SLOT(slot_rebin_on_screen() ));

                context_Menu->addSeparator();
                context_Menu->addAction("Put a tag with my comment just in this point...", this, SLOT(slot_add_a_tag() ));
                context_Menu->addAction("Erase a nearest comment [point a beginning!]", this, SLOT(slot_add_nalepka() ));
                context_Menu->addAction("Remove all TAGS [peak centr. and width]", this, SLOT(slot_remove_all_tags() ));
                context_Menu->addSeparator();

                if(kind_of_spectrum == overplot)
                {
                    context_Menu->addAction("Name of the overlay spectrum nr...", this, SLOT(slot_name_overlay_spec_nr() ));
                }

                if(kind_of_spectrum != overplot)
                {
                    //  contekst_Menu->insertItem("Make frozen photo of the spectrum", this, SLOT(slot_14() ));
                }
                context_Menu->addAction("Show the list of incrementers of this spectrum", this, SLOT(slot_show_list_of_incrementers() ));
                context_Menu->addAction("Show time of last zeroing of this spectrum", this, SLOT(slot_times_of_last_zeroing() ));


                context_Menu->exec(mapToGlobal(QPoint(e->
#if  (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
                                                      position().

#endif
                                                      x() ,e->
#if  (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
                                                      position().
#endif
                                                      y()   )));

                delete context_Menu;
            }
            b_spectrum->clear_flag_was_mouse_click() ;
            b_spectrum->update();
        }
    }
    else
    {
        COTO;
        //cout << " Click in neutral place " << e->x()  << endl ;
    }

}
//**************************************************************************
void spectrum_1D::remember_as_marker(typ_x k)
{
    remember_for_undo("Set 'integration' marker") ;
    // here we can adjust it to real bin position
    k = justify_to_bin_center(k) ;
    // to be always in the middle of the channel

    marker_older = marker_younger ;
    marker_younger = k ;

}
//****************************************************************************
void spectrum_1D::full()
{
    remember_for_undo("Full spectrum view") ;
    marker_older =  min_x = specif.beg;
    max_x = specif.end ;  // if 10 bins form 0 to 10 that means that the last bin is (9.0 - 9.9990)
    marker_younger = specif.end - specif.waga ;

    min_counts = giveCurrentMinCounts(); // was 0
    max_counts = giveCurrentMaxCounts() ;

    // lets look for a maximum value

    //  if(kind_of_spectrum == overplot)
    //  {
    //    // here we may make global values
    //    for(unsigned int i=0 ; i < overplot_vector.size() ; i++)
    //    {
    //      specif.end_y = max(specif.end_y, overplot_vector[i].sd.end_y);
    //
    //      cout << i << ")  max y " << overplot_vector[i].sd.end_y << endl ;
    //    }
    //    cout << "max y " << specif.end_y << endl ;
    //    max_counts = specif.end_y ;
    //
    //  }
    //  else
    //  {
    //    if(spectrum_table.size() > 0)
    //    {
    //      max_counts = 15; // minimum we make such
    //
    //      // now we can imitate a spectrum
    //      for(unsigned int i = 0 ; i < spectrum_table.size() /*specif.bin*/  ; i++)
    //      {
    //        if(spectrum_table[i] > max_counts)  max_counts  =  spectrum_table[i] ;
    //      }
    //      max_counts  = (int) (1.2 * max_counts) ;
    //    }
    //  }
    //

    b_spectrum->set_markers(marker_younger, marker_older);

    //draw_all();
    b_spectrum ->force_new_pixmap(true);
    //update(); // draw_all_on_screen();

}
//***************************************************************************
void spectrum_1D::shift_right(int divider)
{
    remember_for_undo("Shift right") ;
    //cout << "divider  is=" << divider << endl ;
    typ_x range  = (max_x - min_x) ;
    if(range  <= 0)
        return ; // range = 1 ;

    //cout << "Range is=" << range << endl ;

    max_x = max_x + (range / divider) ;
    if(max_x > specif.end)
        max_x = specif.end;

    min_x = max_x - range ;

    // draw_all();
    b_spectrum ->force_new_pixmap(true);
    //    draw_all_on_screen();

}
//**************************************************************************
void spectrum_1D::shift_left(int divider)
{
    remember_for_undo("Shift left") ;
    typ_x range  = (max_x - min_x) ;
    if(range <= 0)
        return ; // range = 1 ;

    min_x = min_x - (range / divider) ;
    if(min_x < specif.beg)
        min_x = specif.beg;
    max_x = min_x + range ;
    //draw_all();
    b_spectrum ->force_new_pixmap(true);
    //    draw_all_on_screen();
}
//***************************************************************************
void spectrum_1D::scale_to_maxY()
{
    remember_for_undo("Scale to max Y") ;
    // claculate the scale in this region
    max_counts = 10 ;


    int first_drawn_channel = (int)((min_x - specif.beg) / specif.waga)  ;
    int last_drawn_channel  = (int)((max_x - specif.beg) / specif.waga) ;


#ifdef NIGDY

    // loops goes on real channels !
    for(int i = first_drawn_channel ; i < last_drawn_channel ; i ++)
    {
        if(spectrum_table[i] > max_counts)
            max_counts = spectrum_table[i] ;
    }
#endif


    if(kind_of_spectrum == overplot)
    {
        // here we may make global values
        for(unsigned int i = 0 ; i < overplot_vector.size() ; i++)
        {
            first_drawn_channel = (int)((min_x - overplot_vector[i].sd.beg) / overplot_vector[i].sd.waga) ;
            last_drawn_channel  = (int)((max_x - overplot_vector[i].sd.beg) / overplot_vector[i].sd.waga) ;
            //   cout << "Search between " << min_x << " - " << max_x << " "
            //    << "first bin= " << first_drawn_channel
            //    << ", last bin= " << last_drawn_channel
            //    << endl;
            for(unsigned int k = min(0, first_drawn_channel) ; k < (unsigned) last_drawn_channel  ; k++)
            {
                max_counts = max(max_counts, overplot_vector[i].spec_table[k]);
            }
            //   cout << i << ")  max y " << max_counts << endl ;
        }
        //  cout << " max_counts " << max_counts << endl ;
        //max_counts = specif.end_y ;

    }
    else
    {
        if(spectrum_table.size() > 0)
        {
            max_counts = 15; // minimum we make such

            // loops goes on real channels !
            if(rebin_factor == 1) {

                for(int i = first_drawn_channel ; i < last_drawn_channel ; i ++)
                {
                    if(spectrum_table[i] > max_counts)
                        max_counts = spectrum_table[i] ;
                }
            } else{
                int pod_suma = 0 ;
                for(int i = first_drawn_channel ; i < last_drawn_channel - rebin_factor; i +=rebin_factor)
                {
                    pod_suma = 0;
                    for(int r = 0 ; r < rebin_factor ; ++r)
                        pod_suma += spectrum_table[i+r];

                    if(pod_suma > max_counts)
                    {
                        max_counts = pod_suma ;
                    }
                }

            }

        } // if size
    } // else


    //==================================
    min_counts = giveCurrentMinCounts() ;
    max_counts = (int)(1.1 * max_counts);
    //draw_all();
    b_spectrum ->force_new_pixmap(true);
    //    draw_all_on_screen();

}
//***************************************************************************
void spectrum_1D::scaleY_by_2()
{
    remember_for_undo("Scale Y by 2") ;
    // warning: to do divide scale by 2, means that the spectrum is geting 2 times higher
    // so it is

    //cout << "1/2, before  " << max_counts ;
    if(!flag_log_scale)
    {
        max_counts /= 2 ;

        if(max_counts < 5)
            max_counts = 5 ;
        //cout << "  2*, after " << max_counts << endl ;

    }
    else
    {
        max_counts /= 10 ;
        if(max_counts < 1)
            max_counts = 1 ;   // was 2
        //  cout << "LOG  2*, after max_counts =" << max_counts << endl ;
    }
    b_spectrum ->force_new_pixmap(true);
    //    draw_all_on_screen();
}
//*************************************************************************
void spectrum_1D::scaleY_by_05()
{
    remember_for_undo("Scale Y by 0.5") ;
    // warning: to scale by 0.5, means that the spectrum is geting  2 times smaller
    if(!flag_log_scale)
    {
        if(max_counts > 100000000L)
        {
            cout << "\a" << flush ;
            return ;
        }
        max_counts *= 2 ;
        //cout << " 0.5*, after " << max_counts << endl ;
    }
    else
    {
        if(max_counts > 100000000L)
        {
            cout << "\a" << flush ;
            return ;
        }
        max_counts *= 10 ;
        //cout << " Log  0.5*, after " << max_counts << endl ;
    }
    b_spectrum ->force_new_pixmap(true);
    //    draw_all_on_screen();
}


//********************************************************************************
void spectrum_1D::wheelEvent ( QWheelEvent * e )
{

    //	cout << "This is a wheel event in spectrum 1D::"
    //	<< __func__ << " , delta = "<<  e->angleDelta().y() << endl ;
    e->accept() ;
    //cout << "Po accept in spectrum_1D::wheelEvent " << endl;

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
//*************************************************************************
void spectrum_1D::integrate(T4results_of_integration *ptr)
{

    if(name_of_spectrum.find("@over") != string::npos)
    {
        QMessageBox::warning
            (nullptr, "Wrong spectrum" ,
             QString("We do not integrate an overplot spectrum\n") + name_of_spectrum.c_str(),
             QMessageBox::Yes | QMessageBox::Default,
             QMessageBox::NoButton );

        return ; // we do not integrate overplots
    }

    long int mysum = 0 ;
    typ_x left, right ;

    if(marker_younger < marker_older)
    {
        left = marker_younger ;
        right = marker_older ;
    }
    else
    {
        left = marker_older ;
        right = marker_younger ;
    }


    // ==============================  background --------
    // int suma_Xi_Yi = 0 ;
    // int suma_X_kwadrat = 0 ;
    double suma_Xi = 0 ;
    int suma_Yi = 0 ;
    int n = 0 ;

    if(backgr_markers.size())
    {
        // cout << "There is " << backgr_markers.size()
        // << " background markers " << endl ;

        for(unsigned int obszar = 0 ; obszar < backgr_markers.size()  ; obszar += 2)
        {
            // cout << " counting in region nr " <<  obszar << ": "  << flush ;
            for(int k = typ_x_to_bin(backgr_markers[obszar])
                 ;
                 k <=  typ_x_to_bin(backgr_markers[1 + obszar]) ;   k++)
            {
                //suma_Xi += k ;
                //double real_channel =(justify_to_bin_left_edge(k)* specif.waga) + specif.beg;
                double real_channel = (justify_to_bin_center(k) * specif.waga) + specif.beg;
                suma_Xi += real_channel ;

                suma_Yi += spectrum_table[k] ;
                n ++ ;
                //      cout << "bin: "<<  k
                //       << " (is really " << real_channel
                //       << ") , " << flush ;
            }
            //cout << endl ;
        }

        // --- mediae -----------------
        double xs = 1.0 * suma_Xi / n ;
        double ys = 1.0 * suma_Yi / n ;


        // caluculating variance --------------
        double c = 0 ;
        double d = 0 ;

        for(unsigned int obszar = 0 ; obszar < backgr_markers.size()  ; obszar += 2)
        {

            for(int k = typ_x_to_bin(backgr_markers[obszar])
                 ;
                 k <=  typ_x_to_bin(backgr_markers[1 + obszar]) ;   k++)
            {

                //double z = k - xs  ;
                // double real_channel =(justify_to_bin_left_edge(k)* specif.waga) + specif.beg;
                double real_channel = (justify_to_bin_center(k) * specif.waga) + specif.beg;
                double z = real_channel - xs  ;
                d = d + (z * z) ;
                c = c + z * (spectrum_table[k] - ys) ;
            }
        }
        // cout << endl ;

        if(d >= 0)
        {
            bgr_slope  = c / d ;
            bgr_offset = ys - (bgr_slope * xs) ;

            flag_bgr_valid = true ;
            //    cout << "after calculations:    slope =  "
            //      << bgr_slope
            //      << ",  offset " << bgr_offset
            //      <<endl;
        }
    }

    int how_many_channels = 0 ;
    int first_int_channel = (int)((left - specif.beg) / specif.waga)  ;
    int last_int_channel  = (int)((right - specif.beg) / specif.waga) ;

    // NEW, bec. somebody could copy markers from the other spectrum with the different
    // range and binning, which can be illegal here
    if(first_int_channel < 0)
        first_int_channel = 0;
    if(first_int_channel > (signed int)(spectrum_table.size() - 1))
        first_int_channel = spectrum_table.size() - 1;


    if(last_int_channel < 0)
        last_int_channel = 0;
    if(last_int_channel > (signed int) spectrum_table.size() - 1)
        last_int_channel = spectrum_table.size() - 1;




    int sum_tla = 0 ;

    //++++++++++++++++++++++++++++++++++++++++
    int peak_area = 0 ;
    double peak_area_error = 0 ;

    double centroid_channel = 0 ;
    double centroid_channel_error = 0 ;

    double peak_width = 0 ;
    double peak_width_error =  0 ;
    // cout << "\n\n=====================================\n" ;
    //********************************************

    if( fabs(left - right) < 0.001)
    {
        mysum = spectrum_table[first_int_channel] ;
        how_many_channels = 1 ;
        //double real_channel =(justify_to_bin_center(first_int_channel)* specif.waga) + specif.beg;
        double real_channel = (first_int_channel * specif.waga) + specif.beg + (0.5 * specif.waga);

        sum_tla += (int)(flag_bgr_valid ? (real_channel * bgr_slope + bgr_offset + 0.5) : 0) ;
    }
    else
    {

        // real integration ---------------------------------------------
        // loops goes on real channels !


        for(int i = first_int_channel ; i <= last_int_channel ; i ++)
        {
            mysum += spectrum_table[i]    ;
            double real_channel = (i * specif.waga) + specif.beg + (0.5 * specif.waga);
            sum_tla += (int)(flag_bgr_valid ? (real_channel * bgr_slope + bgr_offset + 0.5) : 0) ;
            how_many_channels++ ;
        }

    }
    peak_area = mysum - sum_tla ;
    peak_area_error = sqrt((double)mysum + sum_tla) ;



    //========================================================
    double real_channel_of_local_maximum = 0;
    int local_maximum = -999999;

    if(peak_area != 0)
    {
        // calculating the centroid channel --------------------------------
        double tmp = 0 ;

        for(int i = first_int_channel ; i <= last_int_channel ; i ++)
        {
            //double real_channel =(justify_to_bin_center(i)* specif.waga) + specif.beg;
            double real_channel = (i * specif.waga) + specif.beg + (0.5 * specif.waga);

            tmp += (spectrum_table[i] - (int)(flag_bgr_valid ? (real_channel * bgr_slope + bgr_offset + 0.5) : 0))
                   *  real_channel ;

            // specially for time calibration, we like the channel of maximum, not the centroid
            if(spectrum_table[i] > local_maximum)
            {
                real_channel_of_local_maximum = real_channel;
                local_maximum = spectrum_table[i];
            }


        }

        centroid_channel = (1.0 / peak_area) * tmp ;

        // calculating centroid channel error -------------------------------
        tmp = 0 ;
        for(int i = first_int_channel ; i <= last_int_channel ; i ++)
        {
            double real_channel = (i * specif.waga) + specif.beg + (0.5 * specif.waga);

            tmp += (spectrum_table[i] + (int)(flag_bgr_valid ? (real_channel * bgr_slope + bgr_offset + 0.5) : 0))
                   * (real_channel - centroid_channel) * (real_channel - centroid_channel);
        }
        //cout << "calcul centroid_channel_error, tmp= " << tmp ;
        centroid_channel_error = (1.0 / peak_area) * sqrt(tmp) ;
        //      cout <<" so centroid_channel_error =" << centroid_channel_error
        //        << " because sqrt = " << sqrt(tmp)
        //        << endl ;

        //=========================================================
        // peak width ------------------------------------------------
        tmp = 0 ; //???????????
        for(int i = first_int_channel ; i <= last_int_channel ; i ++)
        {
            double real_channel = (i * specif.waga) + specif.beg + (0.5 * specif.waga);

            tmp += (spectrum_table[i] - (int)(flag_bgr_valid ?
                                                   (real_channel * bgr_slope + bgr_offset + 0.5) : 0))
                   *
                   (real_channel - centroid_channel) * (real_channel - centroid_channel) ;
        }

        peak_width = 2.35 * sqrt((1.0 / peak_area) * tmp);

        // ----- peak witdh error ----------------------------------------
        tmp = 0 ;
        for(int i = first_int_channel ; i <= last_int_channel ; i ++)
        {
            double real_channel = (i * specif.waga) + specif.beg + (0.5 * specif.waga);

            tmp +=
                pow(
                    (real_channel - centroid_channel) * (real_channel - centroid_channel)
                            *  2.35 * 2.35
                        - (peak_width * peak_width)
                    , 2  // power of 2
                    )
                *
                (
                    spectrum_table[i] + (int)(flag_bgr_valid ? (real_channel * bgr_slope + bgr_offset + 0.5) : 0)
                    );
        }
        peak_width_error = (1.0 / (2 * peak_width * peak_area)) * sqrt(tmp) ;

    } // end if (peak_area != 0)

    // to have any time information ================
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // using the STL class string
    string statement  ;

    ostringstream ss  ;
    if(ptr->multi_line_mode())
    {
        ss << "=============================== "
           <<  asctime(timeinfo)
           << "Integration in spectrum "
           << windowTitle().toStdString()
           << "\nregion: [" << (first_int_channel * specif.waga) + specif.beg + (0.5 * specif.waga)
           << " - " << (last_int_channel * specif.waga) + specif.beg + (0.5 * specif.waga)
           << "]  has " << how_many_channels
           << " bins.  \nRESULT :   " << (mysum - sum_tla)
           << "\n                   (spectrum integr:  " << mysum
           << ") - (background integr:  " << sum_tla
           << ") \n"

           << "peak_area = \t" << peak_area
           << "     (+- "
           << peak_area_error

           << ")\n";

        if(peak_area)
        {
            ss << "centroid_channel =  " << centroid_channel
               << " (+ - "
               << centroid_channel_error
               << ")\npeak_width = \t" << peak_width
               << " (+ - "
               << peak_width_error
               << ")"
               << "\nmaximum at = \t" <<  real_channel_of_local_maximum
               << ", its value = "  << local_maximum ;
        }
        // << ends ;

    }
    else
    {
        ss << "RESULT :   " << setw(9) << (mysum - sum_tla)
        << " ; Peak Area = " << setw(6) << peak_area
        << "(+ -" << setprecision(3) << peak_area_error
        << ");";
        if(peak_area)
        {
            ss <<
                "Centroid = " << setw(6) << centroid_channel
               << "(+ -" << setprecision(3) << centroid_channel_error

               << "); Width = " << setw(6) << peak_width
               << "(+ -"
               << setprecision(3) << peak_width_error
               << ") ;  "

               << " Maximum at = " <<  real_channel_of_local_maximum
               << "; its value = "  << local_maximum
               << "; region: [" << setw(6) << (first_int_channel * specif.waga) + specif.beg + (0.5 * specif.waga)
               << " - " << setw(6) << (last_int_channel * specif.waga) + specif.beg + (0.5 * specif.waga)
               << "], " << setw(6) << how_many_channels
               << " bins.  ; "
               << " / (Spc: " << mysum
               << ") - (Bgr: " << sum_tla
               << ");  ";
        }
        ss << windowTitle().toStdString();
        //<< ends ;
    }
    statement = ss.str() ;

    // QMessageBox::information (this, "INFO Integration result" , rura) ;

    // showing the result text window on the screen
    ptr->add_results(statement.c_str(), name_of_spectrum, centroid_channel, real_channel_of_local_maximum) ;

    ptr->add_markers(left, right, backgr_markers);
    ptr->show() ;
    ptr->raise() ;    // to the top



#if 0

    ostringstream ss2  ;

    ss2 << " "
        << setprecision(4) << centroid_channel
        << " [" << peak_width << "]";
    //<< ends ;

    statement = ss2.str() ;

    int vector_channel = (int)((centroid_channel - specif.beg) / specif.waga)  ;

    if(
            (vector_channel >= 0)
            &&
            (vector_channel < (int) spectrum_table.size()))

    {
        double pos_y = spectrum_table[vector_channel] ;
        nalepka.push_back(Tpinup(centroid_channel, pos_y, statement.c_str())) ;
    }
#endif

}
//****************************************************************************
void spectrum_1D::give_parameters(typ_x * min_ch, typ_x * max_co,
                                  typ_x * max_ch, typ_x * min_co,
                                  spectrum_descr *sd)
//typ_x *sp_beg, typ_x *sp_end)
{

    // this is the function for an application, to learn how to position the sliders and scrollers
    *min_ch = min_x ;
    *max_ch = max_x ;
    *min_co = min_counts ;
    *max_co = max_counts ;
    //cout << "adress of specif = " << (&specif)  << endl ;
    *sd = specif ;
    //*sp_beg = specif.beg ;
    //*sp_end = specif.end ;

}
//************************************************************************
void spectrum_1D::scrollbar_horizontal_moved(int int_left_edge)
{
    // to not to store intermediate positions while we drag the scrollbar
    static time_t last = time(nullptr) ;
    if(time(nullptr) - last > 2)
    {
        remember_for_undo("Scrollbar horizontal moved");
    }
    last = time(nullptr) ;


    // this trick is because sliders do not accept any double numbers

    //  cout << "scrolbar mover , value = " << int_left_edge << endl ;
    typ_x my_left_edge = (typ_x)int_left_edge /   scrollbar_poziomy_mnoznik  ;
    // cout << "Po korekcji = " << my_left_edge << endl ;

    typ_x range  = (max_x - min_x) ;
    if(range <= 0)
        return ; // range = 0.01 ;

    //cout << "Range is = " << range << endl ;

    max_x = my_left_edge + range ;
    min_x = my_left_edge ;

    if(max_x > specif.end)
        max_x = specif.end  ;
    if(max_x - min_x < range)
        min_x = max_x - range ;

    b_spectrum ->force_new_pixmap(true);
    //    draw_all_on_screen();

}
//*****************************************************************************
void spectrum_1D::scaleY_by_factor(double  scaling_factor)
{

    remember_for_undo("Scale by fatctor [n]") ;
    //  for(int i = min_channel ; i <= max_channel ; i++)
#if 0
    int first_drawn_channel = (int)((min_x - specif.beg) / specif.waga)  ;
    int last_drawn_channel  = (int)((max_x - specif.beg) / specif.waga) ;

    // claculate the scale in this region
    max_counts = 0 ;

    // loops goes on real channels !
    for(int i = first_drawn_channel ; i <= last_drawn_channel ; i ++)
    {
        if(spectrum_table[i] > max_counts)
            max_counts = spectrum_table[i] ;
    }

    if(value < 2)
        value = 2 ;

    max_counts = (int)(min_counts + value) ;
    /*--
        cout << "scale by factor *** Max counts = " << max_counts
                << " value = " << value
                        << ", summ = "
                                << (min_counts + value)
                                << endl;
                                ---*/
    // draw_all();
    b_spectrum ->force_new_pixmap(true);
    //    draw_all_on_screen();
#else
    // warning: to scaling_factor   0.5, means that the spectrum is geting  2 times smaller

    //	flag_repaint_counts_box = true;
    if ( !flag_log_scale )
    {
        if ( scaling_factor > 1 &&  max_counts > 10000000L )
        {
            //cout << "\a" << flush ;
            return ;
        }
        max_counts *= scaling_factor ;
        //         cout << " Linear, scaling_factor= " << scaling_factor << " , max_counts = " << max_counts << endl ;
    }
    else  // ================= LOG ===============================
    {
        if ( scaling_factor > 1 )
        {
            if ( max_counts > 10000000L ) //magnification-----
            {
                //cout << "\a" << flush ;
                return ;
            }
            max_counts *= ( 2.0*scaling_factor ) ;
        }
        else   // making smaller-------
        {
            max_counts /= ( 2*scaling_factor ) ;
        }
    }

    if ( max_counts < 10 )  // if it would be less then 10 the integer mulitplic 1.1* max_counts_on_map would not work
        max_counts = 10 ;
    //     cout << " after max_counts " << max_counts << endl ;



    b_spectrum ->force_new_pixmap(true);
    //    draw_all_on_screen();

#endif
}
//************************************************************************
void spectrum_1D::slider_horizontal(int value)
{
    remember_for_undo("Slider horizontal moved");

    // cout << "slider horizontal mover , value = " << value << endl ;
    typ_x my_value = (typ_x)value /   scrollbar_poziomy_mnoznik  ;
    // cout << "Po korekcji = " << my_value << endl ;



    if(my_value < 5)
        my_value = 5 ;

    if(min_x + my_value  >  specif.end)
    {
        max_x = specif.end ;
        min_x = max_x - my_value ;
    }
    else
    {
        max_x = min_x + my_value ;
    }

    if(min_x < specif.beg)
        min_x = specif.beg ;

    b_spectrum ->force_new_pixmap(true);
    //    draw_all_on_screen();


}
//***********************************************************************
void spectrum_1D::scroller_vertical_moved(int value_bottom)
{
    remember_for_undo("scroller veritcal moved");
    value_bottom  /=   scrollbar_poziomy_mnoznik;

    // cout << "F. scroller_vertical_moved(int value_bottom), argu = " << value_bottom << endl ;
    //cout << "divider  is = " << divider << endl ;
    int current_range  = (max_counts - min_counts) ;
    if(current_range < 10)
        current_range = 10 ;
    /*----
        cout
        << "VSCR Range is = " << current_range
                << ",  bottom = " << value_bottom << endl ;
                        ------*/
    //cout << "before change: max_coutns = " << max_counts << ", min_counts = "<< min_counts << endl ;
    max_counts = -value_bottom ;
    min_counts = max_counts - current_range ;
    //cout << "after   change: max_coutns = " << max_counts << ", min_counts = "<< min_counts << endl ;

    b_spectrum ->force_new_pixmap(true);
    //    draw_all_on_screen();


}
//*************************************************************************
int spectrum_1D::giveCurrentMaxCounts()
{

    //  int cur_max = 0 ;
    //  for(unsigned int i = 0 ; i < spectrum_table.size(); i++)
    //  {
    //    if(spectrum_table[i] > cur_max) cur_max = spectrum_table[i] ;
    //  }
    //  return cur_max ;




    if(kind_of_spectrum == overplot)
    {
        // here we may make global values
        for(unsigned int i = 0 ; i < overplot_vector.size() ; i++)
        {
            specif.end_y = max(specif.end_y, overplot_vector[i].sd.end_y);

            //cout << i << ")  max y " << overplot_vector[i].sd.end_y << endl ;
        }
        //cout << "max y " << specif.end_y << endl ;
        max_counts = (int) specif.end_y ;

    }
    else
    {
        if(spectrum_table.size() > 0)
        {
            max_counts = 15; // minimum we make such

            // now we can imitate a spectrum
            for(unsigned int i = 0 ; i < spectrum_table.size() /*specif.bin*/  ; i++)
            {
                if(spectrum_table[i] > max_counts)
                    max_counts  =  spectrum_table[i] ;
            }
            max_counts  = (int)(1.2 * max_counts) ;
            // cout << "max_counts " << max_counts << endl ;
        }
    }
    return max_counts ;


}
//************************************************************************

//************************************************************************
void spectrum_1D::set_parameters(typ_x min_ch, typ_x max_co, typ_x  max_ch, typ_x min_co)
{

    //cout << __func__ << " !!! max_co = " << max_co << ", max_counts = " << max_counts << endl;

    // to not to store intermediate positions while we drag the scrollbar
    static time_t last = time(nullptr) ;
    if(time(nullptr) - last > 2)
    {
        remember_for_undo("Changing display region");
    }
    last = time(nullptr) ;


    // this is the function for an application, to learn how to position the sliders and scrollers
    // be carefull, spectra can have different length. For example 4096 and 256

    if(min_ch >= specif.beg && max_ch <= specif.end)
    {
        min_x = max(specif.beg, min_ch) ;
        max_x = min(specif.end, max_ch) ;
    }

#ifdef NIGDY

    if(max_ch <= specif.end)
    {
        min_x = min_ch ;
        max_x = max_ch ;
    }
    else
    {
        //cout << "Uwaga max_ch = " << max_ch << "gdy spextrum length " << spectrum_length << endl ;
    }
#endif
    min_counts = static_cast<int>(min_co) ;
    max_counts = static_cast<int>(max_co) ;

    b_spectrum ->force_new_pixmap(true);
}
//*************************************************************************
void spectrum_1D::save()   // virtual function to save spectrum
{

    // selecting the name ?
    QString fname = windowTitle() + ".asc" ;

    // selecting the format ASCII / binary  (by default in ASCII ?)
    real_save_ascii(fname, false);  // means - not radware header
}
//************************************************************************
void spectrum_1D::save_as(string prefix)
{
    if(prefix.empty())
    {
        // dialog to choose the name and the format


#if 0
        QString QFileDialog::getSaveFileName
                ( QWidget * parent = 0, const QString & caption = QString(),
                  const QString & dir = QString(), const QString & filter = QString(),
                  QString * selectedFilter = 0, Options options = 0 );

#endif

        QString fileName;
        QString selected_filter;  // QString("1).  Just two columned ascii(*.asc);;2).  Radware header, two columned ascii(*.asc);;3).  Binary original cracow format(*.spc)");
        QString filters = "3).  Binary original greware format(*.spc)";  // QString("1).  Just two columned ascii(*.asc);;2).  Radware header, two columned ascii(*.asc);;3).  Binary original cracow format(*.spc)");
        Tplate_spectrum::flag_repainting_allowed = false;


        string name = name_of_spectrum;

        if(kind_of_spectrum == typ_spec::sum){
            filters = "3).  Binary original greware format(*.spc)";

            // change name from path::@sum_SOMETHING.spc
            // to path::SOMETHING.spc
            auto gdzie = name.find("@sum_");
            if(gdzie != string::npos) {
                name = name.substr(1);
                //name.replace(gdzie, 0, "");
            }
        }


        fileName = QFileDialog::getSaveFileName(this,
                                                tr("Save current spectrum as a file"),
                                                gpath.Qspectra() +  name.c_str(),
                                                filters,
                                                &selected_filter
                                                );

        Tplate_spectrum::flag_repainting_allowed = true;

        if(fileName.isEmpty())
            return ;
        // cout << "Nazwa " << fileName.toStdString() << endl ;
        if(fileName.endsWith ( ".spc") == false && fileName.endsWith ( ".asc") == false )
        {
            fileName.append(".spc");
        }


        if(kind_of_spectrum == typ_spec::sum){
            save_sum_spectrum(name) ;
            return;
        }
        
        if(fileName.size() )  // we choose something and pressed OK
        {

            bool flag_radware_header = false;

            //     cout << "Nazwa = > " << fileName << "<, filter = >"<< filter << " < "
            //     << gpath.spectra
            //     << " a name_of_spectrum " << name_of_spectrum
            //     << endl ;

            if(selected_filter[0] == '3')
            {
                // just  making copy of this spectrum file
                string kom = "cp " + gpath.spectra + name_of_spectrum + " ";
                kom  += fileName.toStdString()  ;
                //       cout << "Komenda = " << kom << " = " <<endl;
                [[maybe_unused]] int result = system(kom.c_str());
            }
            else
            {
                if(selected_filter[0] ==  '2')  // Radware filter starts with '2'
                {
                    flag_radware_header = true;
                }

                //       cout << "Nazwa = > " << fileName << "<, filter = >"<< filter << " < "
                //       << " flag radware = " << flag_radware_header
                //       << endl ;

                if(fileName.size() )
                {
                    // real saving
                    real_save_ascii(fileName, flag_radware_header);
                }

            } // end else filter

        } // if size
    }
    else // prefix NOT empty so always binary with prefix
    {
        // UWAGA: JESLI TO JEST SUM SPECTRUM @ TO W TEN SPOSÓB ZAPISZEMY
        // DEFINICJĘ (RECEPTĘ) WIDMA A NIE JEGO AKTUALNĄ TREŚĆ.
        
        
        // just  making copy of this spectrum file
        string kom = "cp " + gpath.spectra + name_of_spectrum + " ";
        kom  += (gpath.spectra + prefix + name_of_spectrum)   ;
        //       cout << "Komenda = " << kom << " = " <<endl;
        [[maybe_unused]] int dummy = system(kom.c_str());
        //dummy = dummy;

    }
}
//***********************************************************************
void spectrum_1D::real_save_ascii(QString & nazwa, bool flag_radware_header)
{
    ofstream plik(nazwa.toStdString() );
    if(!plik)
    {
        QMessageBox::information(this, "Saving the spectrum problems" ,
                                 QString("Can't create the file %1")
                                     .arg(nazwa),
                                 QMessageBox::Ok);
        return ;
    }
    else
    {
        if(flag_radware_header)
        {
            string nazwa_no_path = nazwa.toStdString();
            string::size_type pos = nazwa_no_path.rfind('/');
            if(pos != string::npos)   //
            {
                nazwa_no_path = nazwa_no_path.substr(pos + 1);
            }


            plik << "SPECTRUM_NAME\t" << nazwa_no_path
                 << "\nDIMENSION\t\t\t1\nCOUNTTYPE\t\t\tfloat"
                    "\nBASE\t\t\t\t0"
                    "\nRANGE\t\t\t\t4096"
                    "\nDATA\n"  ;
        }


        for(unsigned int i = 0 ; i < spectrum_table.size() ; i++)
        {
            plik
                << specif.beg + (i * specif.waga) << "\t" // <--- first colum (bin)
                << spectrum_table[i]                    // <--- second value
                << "\n" ; //endl ;
        }
        //plik << flush ;  // done by destructor
    }
}
//***********************************************************************
void spectrum_1D::set_the_name(QString & name)
{
    setWindowTitle(name);
}
//****************************************************************************
void spectrum_1D::print_it()
{
    // cout << "spec widget, Decyzja drukowania " << endl ;
#if 0
    flaga_this_is_printing = true ;

    printer = new QPrinter ;


    printer->setOutputToFile(true);  // <-- for testing purposes we prefeare the file
    printer->setOutputFileName("testing_hardcopy.ps");
    printer->setOrientation(QPrinter::Landscape);

    if(printer->setup(this))
    {
        // wyglada na to ze ze powyzsza instrukcja sprawila, ze nastapilo
        // takie odswiezenie ekranu, ze flag printowania sie wyzerowal
        // dlatego ponawiam go
        flaga_this_is_printing = true ;

        printer->setPrintProgram("lpr") ;    // <--- to okazalo sie wymagane !!!!!!!!

        //      cout << "Program drukujacy to: >" << (printer->printProgram()) << "<"
        //      << ", a nazwa printera to >" << (printer->printerName()) << "<"
        //      << ", printerSelectionOption ()= >"  << (printer->printerSelectionOption ()) << "<"
        //      << endl ;

        //   cout << "Po setupie "
        //     << printer->docName()
        //     << endl ;
        QPainter piorko(printer) ;
        //cout << "Po piorku " << endl ;

        //  QBrush b1( Qt::blue );
        QBrush b2(Qt::green, Qt::Dense6Pattern);      // green 12% fill
        //    QBrush b3( Qt::NoBrush );       // void brush
        //    QBrush b4( Qt::CrossPattern );      // black cross pattern

        piorko.setBrush(b2);
        piorko.drawRoundRect(10, 10, 800, 800, 20, 20);
        //    piorko.setBrush( b3 );
        //    piorko.drawEllipse( 250, 10, 200, 100 );
        //    piorko.setBrush( b4 );
        //    piorko.drawPie( 250, 150, 200, 100, 45*16, 90*16 );


        //++++++++++++++++++++++++

        QPaintDeviceMetrics metrics(printer);   // need width/height
        // of printer surface


        paper_height  = metrics.height();
        paper_width =  metrics.width();


        /*----
            const int MARGIN = metrics.logicalDpiX() / 2; // half-inch margin
            int yPos        = MARGIN;               // y position for each line

            QFontMetrics fm = piorko.fontMetrics();

            for( int i = 0 ; i < 10  ; i++ ) {
            //            if ( printer.aborted() )
            //                break;
            if ( yPos + fm.lineSpacing() > metrics.height() - MARGIN ) {
            // no more room on this page
            //                if ( !printer.newPage() )          // start new page
            //                    break;                           // some error
            yPos = MARGIN;                   // back to top of page
            }
            piorko.drawText( MARGIN, yPos, metrics.width() - 2*MARGIN,
            fm.lineSpacing(), ExpandTabs, "linijka tekstu "   );
            yPos += fm.lineSpacing();
            }
            -----*/

        // moje proby --------- zakonczone -------------------

        //      ustaw_swiat(0, 0, metrics.width(), metrics.height() ,   // preparing the coordinates
        //        minX, maxY, maxX, minY);

        /*
            b_counts->set_printing_mode(&piorko);
            b_channels->set_printing_mode(&piorko);
            b_spectrum->set_printing_mode(&piorko);
            */
        draw_all(&piorko) ;

        //   cout << "Po draw " << endl ;

    }
    delete printer ;
    printer = 0 ;
#endif

    //    flaga_this_is_printing = false ;
}
//**************************************************************************
void spectrum_1D::draw_all_on_screen()
{
    //  flaga_this_is_printing = false ;
    QPainter piorko(this) ;

    draw_all(&piorko) ;
}
//**************************************************************************
void spectrum_1D::read_in_file(const char *name, bool this_is_first_time)
{

    // just after zeroing we would like to read spectrum every 1 second
    if(re_read_counter > 0)
    {
        // several times refresh every 1 second
        re_read_counter-- ;
        timer_refresh->stop();
        timer_refresh->start(1000 * 1);  // (1000 is 1 second)
    }
    else
    {
        // if not - so it is normal
        timer_refresh->stop();
        timer_refresh->start(1000 * current_options.give_realistic_refreshing_time_1D()); // (1000 is 1 second)
    }


    spectrum_table.clear() ;


    specif.beg_y = 0 ;
    specif.end_y = 3 ;

    //------------
    kind_of_spectrum = normal ;

    string fname = name ;
    if(fname.find("@sum_") != string::npos)
    {
        // sum spectra are refreshed not so often
        timer_refresh->stop();
        timer_refresh->start(1000 * current_options.give_refreshing_time_sum_1D()); // (1000 is 1 second)

        kind_of_spectrum = sum ;
        read_in_sum_spectra(fname);

    }
    else if(fname.find("@over_") != string::npos)
    {
        kind_of_spectrum = overplot ;
        read_in_overplot_spectra(fname, this_is_first_time);
        // overlay spectra are refreshed not so often
        timer_refresh->stop();
        timer_refresh->start(1000 * current_options.give_refreshing_time_sum_1D()); // (1000 is 1 second)
    }
    //-------
    else
    {
        bool too_short = true ;
        // loop to repeat 'too - short' read (when spy writes this spectrum just now)
        for(int n_try = 0 ; n_try < 10 && too_short ; n_try++) //
        {
            spectrum_table.clear() ;
            //here we check if it is ascii or binary spectum (by the extension)
            string::size_type pos = string(name).rfind(".asc");
            bool flag_ascii = false ;
            if(pos == strlen(name) - 4)
            {
                flag_ascii = true ;
            }
            // opening disk file  in ASCII or binary mode --------
            string path_filename = gpath.spectra + name ;
            ifstream plik ;

            if(flag_ascii)
                plik.open(path_filename.c_str());
            else
                plik.open(path_filename.c_str(), ios::binary);


            // Reading -------------------------------------------------------------
            double bin_left_edge = 0.0 ;
            if(plik)
            {
                if(flag_ascii) // ---------------- ascii ---------
                {
                    for(int i = 0 ; plik ; i++)
                    {
                        int value ;
                        plik >> bin_left_edge >> value ;
                        if(!plik)
                            break ;

                        switch(i)
                        {
                        case 0 :
                            specif.beg = bin_left_edge;
                            break ;

                        default: //?
                        case 1 :
                            specif.waga = bin_left_edge - specif.beg ;
                            break ;
                        }

                        // if more than maximum counts, let's update the max Y
                        if(value > specif.end_y)
                            specif.end_y = value ;

                        spectrum_table.push_back(value) ;
                    } // for
                    specif.end = (bin_left_edge) + (spectrum_table.size() * specif.waga) ;
                }
                else  //------------------ binary -----------------
                {

                    //         plik.read((char*) &bin_left_edge, sizeof(bin_left_edge));
                    //         double waga ;
                    //         plik.read((char*) &waga, sizeof(waga)) ;

                    double bins = 1,    // it is double, bec this is how the spy is writing
                        left = 0 ,
                        right = 0  ;

                    plik.read((char*) &bins, sizeof(bins));

                    plik.read((char*) &left, sizeof(left));
                    plik.read((char*) &right, sizeof(right));

                    //                    cout << " a binary spectrum " << path_filename
                    //                         << " bins = " << bins
                    //                         << ", left = " << left
                    //                         << ", right = " << right << endl;
                    if(!plik)
                    {
                        // sometimes spectrum may be zero !
                        specif.beg = 0 ;
                        spectrum_table.push_back(0) ;  // one channel only
                        specif.end =  spectrum_table.size() - 1 ;      //   333 ;
                    }

                    //         double waga ;
                    //         plik.read((char*) &waga, sizeof(waga)) ;
                    while(plik)
                    {
                        int value ;

                        plik.read((char*) &value, sizeof(value)) ;
                        if(!plik)
                            break ;

                        // if more than maximum counts, let's update the max Y
                        //            if(value> specif.end_y) specif.end_y = value ;

                        spectrum_table.push_back(value) ;
                    } // for

                    specif.bin = static_cast<int> (bins) ;   // we read double, but now we place it into int
                    specif.beg = left;
                    specif.end = right;
                    specif.waga = (right - left) / bins ;
                }



                // now
                //cout << " read  spectrum, channels  " << i << endl ;

            }  // if plik
            else
            {
                // error while reading
                if(timer_refresh)   // so stop refreshing
                {
                    timer_refresh->stop();
                    delete timer_refresh ;
                    timer_refresh = nullptr ;
                }


                string mess =  "Spectrum \n";
                mess += path_filename;
                mess += "\nwas not found \n" ;
                QMessageBox::information(this, "File does not exist ? ", mess.c_str());

                //cout << " error while opening a spectrum " << path_filename << endl ;
                specif.beg = 0 ;
                spectrum_table.push_back(0) ;  // one channel only
                specif.end =  spectrum_table.size() - 1 ;      //   333 ;
            }  // else not plik


            if(spectrum_table.size() == (unsigned) specif.bin)
            //			if(spectrum_table.size() == specif.bin)
            {
                too_short = false ; // <--- no more tries
            }
        } // try once more


    } // else  normal spectrum

    //specif.bin =  spectrum_table.size();

    specif.bin_y =  static_cast<int>(specif.end_y - specif.beg_y + 1);
    // find the real descriptions of the x axis (for example -200, 200)
    // COTO;



    //cout << " max zliczen specif_end_y = " << specif.end_y << endl ;
    //  specif.waga = (specif.end - specif.beg)/ specif.bin ;   // zlotowki na kanal
    specif.waga_y = 1 ;


    if(this_is_first_time)
    {
        marker_younger =  marker_older =  specif.beg ;
        b_spectrum->set_markers(marker_younger, marker_older);

        full();
        // for the first time we do not make undo
        undo_list.pop_back() ;
    }
    else
    {
        // when this is timer updating
        //update(); // draw_all_on_screen();
    }

    // cout << "read_in_file"<<  name_of_spectrum << "\n" << specif << endl;

}
//****************************************************************
#if 0

bool spectrum_1D::find_x_description(const char * name)
{
    cout << __func__ << ", Is this function ever used? " << endl;

    // now the system has changed and bins are written as first
    // 2 double words in the contents of spectrum
    // so we do not have to read them here
    //
    // now this function is only checking if this is active
    // spectrum , or it is not anymore aculutated (but what for...?)

    string desc_name = gpath.spectra + "description_of_ranges.txt";
    ifstream plik(desc_name.c_str()) ;

    if(!plik)
        return false ;

    //cout << "Searching in the " << desc_name << endl ;

    char wyraz[500] ;
    while(1)
    {
        //
        if(!plik)
        {
            cout << "reading of  " << desc_name << " failed " << endl ;
            break ;
        }
        if(plik.eof())
        {
            cout  << desc_name  << " riched the eof " << endl ;
            break ;
        }

        plik.getline(wyraz, sizeof(wyraz)) ;   // because filename can have some spacies inside
        //cout << " read line: " << wyraz << endl ;
        //

        if(strcmp(name, wyraz) == 0)  // found  ----------
        {
            // not needed anymore
            //      plik
            //        >> specif.bin
            //        >>  specif.beg
            //        >> specif.end ;

            //      cout << "spectrum description found " << wyraz
            //        <<  "values are:  " << specif.bin
            //        <<  "  " << specif.beg
            //        <<  "  " << specif.end ;

            return true ;
        }
        // try without extension ----------------------
        //    cout << " is not what we lookfor:  " << name
        //                << ", so lets try with the extension *.asc"
        //                << endl ;
        //

        char *wsk = strrchr(wyraz, ' ') ;
        if(wsk)
            *wsk = 0 ;
        strcat(wyraz, ".spc");
        //cout << " after appending is  " << wyraz << endl ;

        if(strcmp(name, wyraz) == 0)  // found
        {
            plik >> specif.bin  >> specif.beg >> specif.end ;
            //      cout << "spectrum description found " << wyraz
            //        <<  "  values are:  bin= " << specif.bin
            //                      <<  ", beg=  " << specif.beg
            //                      <<  ", end  " << specif.end ;
            //
            //      cout << "\nreturning by B" << endl;
            return true ;
        }



        //  int fff ;
        //  cin >> fff ;
    }


    //cout << "Finally description of the spectum " << name
    // << " was not found " << endl ;
    return false ;

}
#endif // 0

//*************************************************************************
void spectrum_1D::log_linear(bool stan)
{
    // nonsense to :   remember_for_undo("log/linear view") ;
    flag_log_scale = stan ;

    b_spectrum ->force_new_pixmap(true);
    update();
}
//*******************************************************************
void spectrum_1D::set_backgr_marker(typ_x dana)
{
    //    remember_for_undo("Set background maker (function) " ) ;  <--- done earlier
    recent_bgr_marker =  justify_to_bin_center(dana);
    backgr_markers.push_back(recent_bgr_marker)  ;

    sort(backgr_markers.begin(),  backgr_markers.end());
    bgr_slope = 0 ;
    bgr_offset = 0 ;
    flag_bgr_valid = false ;

    //    to test
    //    for(unsigned int i = 0 ; i <  backgr_markers.size() ; i++)
    //    {
    //  cout << i << ") = " << backgr_markers[i] << endl ;
    //    }
}
//*****************************************************************************
vector<typ_x> * spectrum_1D::give_bg_markers()
{
    return & backgr_markers ;
}

//**************************************************************************
typ_x spectrum_1D::justify_to_bin_center(typ_x dana)
{

    // here we can adjust it to real bin position
    if(!(specif.waga > 0))
        return dana ;  // often in overlay spectra

    typ_x binek = (int)((dana - specif.beg) / specif.waga)  ;
    dana = (binek * specif.waga) + specif.beg
           +
           0.5 * specif.waga  ; // to be always in the middle of the channel
    return dana ;
}
//**************************************************************************
typ_x spectrum_1D::justify_to_bin_left_edge(typ_x dana)
{
    if(!(specif.waga > 0))
        return dana ;  // often in overlay spectra
    // here we can adjust it to real bin position
    typ_x binek = (int)((dana - specif.beg) / specif.waga)  ;
    dana = (binek * specif.waga) + specif.beg ;
    return dana ;
}
//*********************************************************************
int spectrum_1D::typ_x_to_bin(typ_x value)
{
    return (int)((value - specif.beg) / specif.waga)  ;
}
//**********************************************************************
bool spectrum_1D::give_bg_info(double * sl, double *off)
{

    *sl = bgr_slope ;
    *off = bgr_offset ;
    return flag_bgr_valid ;
}
//*************************************************************************
void spectrum_1D::draw_all(QPainter *  /*piorko*/)
{
    // cout << "spectrum_1D::draw_all" << endl;

    typ_x gorka = max_counts ;
    typ_x doleczek = min_counts  ;
    if(flag_log_scale)
    {
        gorka = (int)log10((double) gorka) + 1 ;
        doleczek = -1 ;
        //		          cout << "in Spectrum_1D gorka = "
        //		           <<   gorka
        //		           << endl ;
    }

    b_spectrum->change_region_of_spectrum(min_x, gorka, max_x,  doleczek);
    b_counts->change_region_of_counts(doleczek, gorka);
    b_channels->change_region_of_channels(min_x, max_x);

    //#ifdef RAINBOW_SCALE
    //	b_scale->change_region_of_counts(doleczek, gorka);
    //#endif

    //    cout << "in Spectrum_1D  =  flag_repaint_counts_box ="
    //         << flag_repaint_counts_box << endl;
    //    cout << "in Spectrum_1D gorka = "
    //         <<   gorka << ", doleczek = " << doleczek
    //           << endl ;
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
        b_spectrum->update();
        flag_repaint_spectrum_box = false;
    }

    //         cout << "spectrum_1D::draw_all finished " << endl;


    //    }



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

}

//**************************************************************************
bool spectrum_1D::give_flag_log_scale()
{
    return flag_log_scale ;
}

//************************************************************************
void spectrum_1D::erase_nearest_bgr_marker(typ_x place)
{
    // remember_for_undo("Erase the nearest bgr marker ") ; <--- done earlier
    vector <typ_x>::iterator Iter;
    vector <typ_x>::iterator nearest = backgr_markers.end() ;
    //cout << "lista markerow =" ;
    typ_x minimum  = 999999 ;

    for(Iter = backgr_markers.begin() ; Iter != backgr_markers.end() ; Iter++)
    {
        //cout << " " << *Iter;
        if(fabs(place - *Iter)  < minimum)
        {
            minimum = fabs(place - *Iter);
            nearest = Iter ;
        }

    }

    if(nearest != backgr_markers.end())
    {
        // are you sure....
        backgr_markers.erase(nearest);
    }

    //cout << endl;
    sort(backgr_markers.begin(),  backgr_markers.end());
    bgr_slope = 0 ;
    bgr_offset = 0 ;
    flag_bgr_valid = false ;
}
//************************************************************************
void spectrum_1D::focusInEvent(QFocusEvent * e)
{
    if(e->gotFocus())
    {
        //cout << "spectrum 1D got a focus for " << give_spectrum_name() << endl ;
        emit enable_selected_actions(1);  // 1d spectrum
    }
}
//**************************************************************************
bool  spectrum_1D::undo()
{
    //  cout << "spectrum_1D::undo()" << endl ;

    // taking from the undo list and restoring it
    //( packing should be made after any significant operation)


    if(undo_list.empty())
        return false ;

    //  cout
    //    << "In the undo list there are " << undo_list.size()
    //    << " positions " << endl;



    min_counts = undo_list.back().min_counts ;
    max_counts = undo_list.back().max_counts;
    min_x = undo_list.back().min_x ;
    max_x = undo_list.back().max_x ;

    //    cout << "RE-Storing from undo -==== max_x = " << max_x  << endl ;


    bgr_slope = undo_list.back().bgr_slope;
    bgr_offset = undo_list.back().bgr_offset;

    marker_older = undo_list.back().marker_older;
    marker_younger = undo_list.back().marker_younger;

    recent_bgr_marker  = undo_list.back().recent_bgr_marker;
    backgr_markers = undo_list.back().backgr_markers;
    flag_bgr_valid  = undo_list.back().flag_bgr_valid;
    my_gates_1D = undo_list.back().my_gates_1D ;
    nalepka = undo_list.back().nalepka ;
    rebin_factor = undo_list.back().rebin_factor;

    //cout << "Restoring from undo -==== marker_older = " << marker_older
    //        << ", marker_younger= " << marker_younger  << endl;
    b_spectrum->set_markers(marker_younger, marker_older);

    //  cout << "undo:  " << undo_list.back().undo_description_text << endl ;

    undo_list.pop_back()  ;
    b_spectrum ->force_new_pixmap(true);
    // draw_all_on_screen();

    return true ; // why ?
}
/***********************************************************************
     * this is a function to shift the spectrum (matrix) on the picture up or down
     *  the argument is telling if we are going higher (plus) or lower (minus)
     *  by the definition devider = 1 means:
     *     move as much as this part of the picture
     * which you see now.
     *  if the argument will be 10 - means:
     *        move by 1/10 of the current picture
     ************************************************************************/
void spectrum_1D::shift_up_down(int divider)
{
    remember_for_undo("Shift up/down") ;
    //cout << "divider  is=" << divider << endl ;
    int range  = (max_counts - min_counts) ;
    if(range  <= 0)
        return ; // range = 1 ;

    //cout << "Range is=" << range << endl ;

    max_counts = max_counts + (range / divider) ;
    if(max_x > specif.end_y)
        max_counts = static_cast<int>(specif.end_y) ;

    min_counts = max_counts - range ;
    if(min_counts < specif.beg_y)
        min_counts =  static_cast<int>(specif.beg_y) ;

    // draw_all();
    b_spectrum ->force_new_pixmap(true);
    //draw_all_on_screen();

}
//****************************************************************************
void spectrum_1D::save_bg_markers_as_gate()
{
    if(backgr_markers.size() != 2)
    {

            //    string mmm =
            //      "To define the gate you need to set exactly 2 background markers";
        showWarningMessage( "Error" , "To define the gate you need to set exactly 2 background markers");
        return ;
    }

    // ----------------------add new gate --------------------------------

    // at first dialog window about the name of the gate?
    bool ok = true;

    QString screen_name_of_gate = "GATE" ;

#ifdef QUESTION_ABOUT_NAME

    = QInputDialog::getText(
        "Defining the new 1D gate",
        "Enter the name of this 1D gate:",
        QLineEdit::Normal,
        "GATE" /*QString::null*/ , &ok, this);
#endif

    if(ok && !screen_name_of_gate.isEmpty())
    {
        // user entered something and pressed OK

        // preparing a filename for this gate
        string spec_name = windowTitle().toStdString()  ;

        spec_name.erase(spec_name.rfind(".spc")); // removing extension ".spc"
        ostringstream ss  ;
        ss   << spec_name
           << prefix_of_gate_1D_names    // "_gate_"
           << screen_name_of_gate.toStdString()
           << ".gate";																			// adding a new extension ".gate"

        string disk_name_of_gate  = ss.str() ;

        string screen = screen_name_of_gate.toStdString() ;
        gate_1D b(screen,
                  disk_name_of_gate,
                  backgr_markers
                  );

        //cout << "Before pushing to vector " << screen << endl;
        my_gates_1D.push_back(b);

        //cout << "Created gate named " << screen
        //  << " now size of gate_1D is " << gate_1D.size()
        //  << ", last pushed gate has vertices "
        //  << gate_1D[m]
        //  << endl ;

        // writing it on the disk , but what for?
        my_gates_1D[my_gates_1D.size() - 1].save_to_disk();

        //cout << "succesfully created new gate" << endl ;

        // ---------------------- erase all markers ------------------------
        backgr_markers.clear() ;
        bgr_slope = 0 ;
        bgr_offset = 0 ;
        flag_bgr_valid = false ;
        recent_bgr_marker = -999999 ;  // nothing have been set yet

        // refresh, because somebody could use already
        // existing name to replace the old version of the
        // gate
        find_gates_with_names_as_this_spectrum();
        appl_form_ptr->warning_spy_is_in_action() ;
    }
    else
    {
        // user entered nothing or pressed Cancel
    }
}
//***************************************************************
/** there can be many gates on one spectrum. We need this function
        for the class box of spectrum to draw the gate on the screen */

vector<gate_1D>&  spectrum_1D::give_vector_of_gates()
{
    return my_gates_1D ;
}
//*****************************************************************
/** No descriptions */
bool spectrum_1D::find_gates_with_names_as_this_spectrum()
{
    // for FRS we have to set some  gates on the multiwire sum spectum
    //
    // here we try to find any gates_1D which names start with the name of this
    // spectrum.


    my_gates_1D.clear() ;

    // find the string which should start the name of banana
    string family_name = name_of_spectrum  ;

    //cout << "Searching spectrum gated for " << name_of_spectrum << endl ;

    if(family_name.rfind(".spc") != string::npos)
    {
        family_name.erase(family_name.rfind(".spc")) ;    // removing the .mat extension
    }
    //cout << "this is the beginning of the  filenames " << family_name << endl ;

    // looking into directory (with the filter set)

    // taking the spectra names from the directory - only
    // opening all spectra files from current directory
    QDir katalog(gpath.gates_1D.c_str()) ;

    QStringList filters;
    filters << QString ( family_name.c_str() ) + "*.gate";
    katalog.setNameFilters(filters);

    QStringList lista = katalog.entryList (  ) ;



    int gate_nr = 0 ;
    for(QStringList::Iterator it = lista.begin() ; it != lista.end() ;  ++it, gate_nr++)
    {
        // cout << "found files : " << *it << "\n";

        // loop of reading
        string path_filename = gpath.gates_1D + (*it).toStdString() ;
        ifstream plik(path_filename.c_str());
        if(!plik)
        {
            cout << "Error in opening file " << path_filename   << endl ;
            break;
        }

        // ----------------------add new polygon
        // polygon gates
        //polygon_type fence ;
        vector<typ_x> fence;
        double x, y ;
        while(!plik.eof())
        {
            plik >> zjedz >> x >> zjedz >> y ;
            if(!plik)
                break ;
            fence.push_back(x) ;
            fence.push_back(y) ;
        }

        // removing extension ?

        string disk_name = (*it).toStdString() ;
        // how to make screen name form it ?
        // disk name:  matrix_screen_name.poly
        //

        string::size_type nr = disk_name.rfind(".gate") ;
        string screen_name = disk_name ;
        screen_name.replace(nr, 999, "");  // removing ".gate" extension

        screen_name.replace(0,
                            family_name.length(),
                            "");

        screen_name.replace(0,
                            prefix_of_gate_1D_names.length(),   // removing "_gate_"
                            "");
        // cout << "so far screen name = " << screen_name << endl ;

        gate_1D b(screen_name,    // screen name
                  disk_name,   // disk name
                  fence);


        // cout << "Before pushing to vector " << screen_name << endl;
        my_gates_1D.push_back(b);
        //    cout << "Created polygon named " << screen_name
        //      << " now size of gatea is " << gatea.size()
        //      << ", last pushed polygon has vertices "
        //      << gatea[gate_nr]
        //      << endl ;


        // cout << "succesfully created new polygon" << endl ;

    }
    //cout << "end of searching loop " ;

    update() ;
    return true ;

}
//**************************************************************

/** You will be asked name by name - should it be removed */
void spectrum_1D::remove_the_gate_1D()
{

    // for FRS we have to set some  gates on the multiwire sum spectum
    //
    // here we try to find any gates_1D which names start with the name of this
    // spectrum.

    // find the string which should start the name of banana
    string family_name = name_of_spectrum  ;

    // cout << "Searching spectrum gated for " << name_of_spectrum << endl ;

    family_name.erase(family_name.rfind(".spc")) ;    // removing the .mat extension
    // cout << "this is the beginning of the  filenames " << family_name << endl ;
    // COTO ;
    // looking into directory (with the filter set)

    // taking the spectra names from the directory - only
    // opening all spectra files from current directory
    QDir katalog(gpath.gates_1D.c_str()) ;
    //    QStringList lista = katalog.entryList(QString(family_name.c_str()) + "*.gate") ;

    QStringList filters;
    filters << QString ( family_name.c_str() ) + "*.gate";
    katalog.setNameFilters(filters);

    QStringList lista = katalog.entryList (  ) ;




    int gate_nr = 0 ;
    for(QStringList::Iterator it = lista.begin() ; it != lista.end() ;  ++it, gate_nr++)
    {
        // cout << "Taken from the list ->  file : " << (*it).toStdString() << endl;;
        switch(askQuestionWithButtons(  // +
            "Removing the 1D gate from the disk group definition remover",
            QString("Do you want to permanently delete the gate  stored "
                    "on the disk as\n\t %1\n"
                    "Are you sure \n\n").arg(*it),
            "Yes", "No", "Cancel", 2))
        {
        case 1:   //Yes :
        {
            string pathed_name = gpath.gates_1D + (*it).toStdString() ;
            string mmm = "Are you sure you want to delete " + pathed_name ;
            auto result = askYesNoCancel("Deleting the 1D gate" , mmm.c_str());
            if(result == QMessageBox::Yes)  // yes
            {
                QFile file(pathed_name.c_str());
                file.remove() ;
            }


        }
        break;

        case 2:  // No:
            break ;

        default:
        case 3: // Cancel :
           it = lista.end() ; // stop the loop
           it-- ;  // because the loop will now skip to the next
            break ;

        }

    }
    //cout << "end of searching loop " ;
    find_gates_with_names_as_this_spectrum();
    update() ;
    return  ;

}

//***************************************************** No descriptions */
void spectrum_1D::black_white_spectrum(bool on)
{
    // nonsense to :  remember_for_undo("Black & white / Color spectrum") ;
    flag_white_black = on ;
    // refresh painting
    b_spectrum ->force_new_pixmap(true);
    //    draw_all_on_screen();
}
//**************************************************************************
//**************************************************************************
void spectrum_1D::re_read_spectrum()
{

    //  cout << "Time to re read the spectrum "
    //     << name_of_spectrum << "  "
    //     << time(0)
    //     << endl;

    read_in_file(name_of_spectrum.c_str(), false);   // false = not the first time, update
    b_spectrum ->force_new_pixmap(true);
    update() ;
}
/************************************************************ */
/** when somebody press X to close spectrum */
void spectrum_1D::closeEvent(QCloseEvent * e)
{
    //cout << "This was close event " << endl;
    e->accept();
    destroy();  // stop the timer

}
//***************************************************************************
/** with dialog precisely setting the marker */
void spectrum_1D::set_integr_marker()
{
    remember_for_undo("Set integration marker manually") ;
    bool ok;
    double res = QInputDialog::getDouble(this,
                                         "Setting the integration marker precisely", "Enter a decimal number:",
                                         0, specif.beg, specif.end, 2, &ok);
    if(ok)
    {
        // user entered something and pressed OK
        remember_as_marker(res) ;
        b_spectrum->set_markers(marker_younger, marker_older);
    }
    else
    {
        // user pressed Cancel
    }
}
//***************************************************************************
/** remember all integration and background markers  */
void spectrum_1D::remember_integration_and_bgr_markers()
{
    remember_for_undo("Set integration marker manually") ;

    vector<typ_x>   tmp;
    set_of_markers.push_back(tmp);


    int nr = set_of_markers.size();
    nr--;

    typ_x marker_first = marker_older;
    typ_x marker_second = marker_younger;

    if(marker_older > marker_younger)
    {
        marker_first = marker_younger;
        marker_second = marker_older;
    }

    set_of_markers[nr].push_back(marker_first);
    set_of_markers[nr].push_back(marker_second);

    auto s = backgr_markers;
    std::sort(s.begin(), s.end());
    for( auto b : s)
    {
        //        cout << " " << b ;
        set_of_markers[nr].push_back(b);
    }
    //    cout << "\nRemembered as set nr " << nr << endl;


    // zapis wszystkiego na dysk

    store_all_marker_on_a_disk();
    // INFO
    string msg = "The background markers are stored as a set nr " + to_string(nr) +
                 "\n\n ";
    QMessageBox msgBox;
    msgBox.setText(msg.c_str() );
    msgBox.exec();

}
//**************************************************************************
void spectrum_1D::paste_integration_and_bgr_markers(int set_nr)
{

    // cout << __PRETTY_FUNCTION__ << "  nr = " << set_nr << endl;
    int i = set_nr;
    remember_for_undo("Paste all markers to the spectrum") ;

    marker_younger = set_of_markers[i][0];
    marker_older = set_of_markers[i][1];
    b_spectrum->set_markers(marker_younger, marker_older);
    //     cout << "YELLOW = " << marker_older << " - " << marker_younger << endl;

    backgr_markers.clear();
    for(uint b = 2 ; b < set_of_markers[i].size() ; b++)
        backgr_markers.push_back( set_of_markers[i][b] ) ;
    //    }

    b_spectrum ->force_new_pixmap(true);
    flag_repaint_spectrum_box = 1;
    // draw_all_on_screen();
}
//**************************************************************************
string spectrum_1D::take_integration_and_bgr_markers()
{
    string result ;
    ostringstream s(result);
    s << "Integration markers: "
      << marker_older << " - " << marker_younger
      << "    Background markers: ";


    auto bgs = backgr_markers;
    std::sort(bgs.begin(), bgs.end());
    for( auto b : bgs)
    {
        s << " " << b ;
    }
    return s.str();
}

//**************************************************************************
void spectrum_1D::paste_integration_and_bgr_markers()
{
    uint how_many = set_of_markers.size() ;

    if(!how_many){
        string msg = "No set of markers was remembered yet";
        QMessageBox msgBox;
        msgBox.setText(msg.c_str() );
        msgBox.exec();
        //        return;
    }


    t4markers_manager dlg(nullptr, set_of_markers,this);
    //    dlg.set_data(set_of_markers);
    dlg.exec();



}
//**************************************************************************
//**************************************************************************
//void spectrum_1D::paste_integration_and_bgr_markers_older()
//{
//    uint how_many = set_of_markers.size() ;

//    if(!how_many){
//        string msg = "No set of markers was remembered yet";
//        QMessageBox msgBox;
//        msgBox.setText(msg.c_str() );
//        msgBox.exec();
//        return;
//    }


//    bool ok;
//    int i = QInputDialog::getInt(this, tr("Paste Set nr of markers"),
//                                 tr("Type a set number: "), 0, 0, how_many-1, 1, &ok);
//    if (ok){
//        //        cout << "Sukces, accepted = " << i
//        //             << ", size = " << how_many
//        //             << endl;
//        //if(i < )

//        remember_for_undo("Paste all markers to the spectrum") ;

//        marker_younger = set_of_markers[i][0];
//        marker_older = set_of_markers[i][1];
//        b_spectrum->set_markers(marker_younger, marker_older);

//        backgr_markers.clear();
//        for(uint b = 2 ; b < set_of_markers[i].size() ; b++)
//            backgr_markers.push_back( set_of_markers[i][b] ) ;
//    }
//}
//**************************************************************************

void spectrum_1D::read_from_disk_sets_of_markers()
{
    string pat_name = gpath.Qoptions().toStdString() + "stored_regions_of_integration.txt" ;
    // cout << pat_name << endl;
    ifstream file(pat_name);
    string linia;
    typ_x marker;
    set_of_markers.clear();

    while(file)
    {
        getline(file, linia);
        if(file)
        {
            vector<typ_x>   tmp;
            set_of_markers.push_back(tmp);

            // reading all markers in one line
            istringstream p(linia);
            uint nr_linii = set_of_markers.size();
            while(p){
                p >> marker ;
                if(!p) break;
                set_of_markers[nr_linii - 1].push_back(marker);
            }
        }

    }
    // show all

    //    for(uint el = 0;  el < set_of_markers.size() ; el++)
    //    {
    //        for(uint m = 0;  m < set_of_markers[el].size() ; m++)
    //        {
    //            cout << set_of_markers[el][m] << "  ";
    //        }
    //        cout << endl;
    //    }
}
//**************************************************************************
void spectrum_1D::store_all_marker_on_a_disk(){
    // zapis wszystkiego na dysk
    string pat_name = gpath.Qoptions().toStdString() + "stored_regions_of_integration.txt" ;
    //cout << pat_name << endl;
    ofstream file(pat_name);
    for(uint line = 0 ; line < set_of_markers.size(); line++)
    {
        for (auto x : set_of_markers[line])
        {
            file << "       ";
            file  << x << "  " ;
        }
        file  << endl  ;
    }}
//**************************************************************************
void spectrum_1D::copy_markers(Tplate_spectrum *doc)
{

    remember_for_undo("Copy markers to this spectrum") ;
    spectrum_1D * ptr = dynamic_cast<spectrum_1D*>(doc);
    if(!ptr)
        return ;

    backgr_markers =  *(ptr->give_bg_markers());
    marker_older = ptr->marker_older ;
    marker_younger = ptr->marker_younger ;

    b_spectrum->set_markers(marker_younger, marker_older);
    b_spectrum ->force_new_pixmap(true);
    //    draw_all_on_screen();

}
//*************************************************************************
/** reading many spectra, which names are stored in "@sum_" file */
bool spectrum_1D::read_in_sum_spectra(string fname)
{


    // opening the file with the names
    string path_names = gpath.spectra + fname ;
    ifstream plik_names(path_names.c_str());
    if(!plik_names)
    {
        cout << "Can't open file with names of the sum spectra " << path_names << endl;
        return false ;
    }


    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    string name ; // for spectrum name

    // to make the sum spectrum, all of them should have the same binning
    // just to check this - we will remember the binning of the first spectrum
    double bins_0 = 1, left_0 = 0, right_0 = 0 ;
    string name_0;

    for(int nr_spectrum = 0 ; plik_names ; nr_spectrum++)
    {
        double adding_factor ;
        plik_names >> name >> adding_factor;
        if(!plik_names)
            break ;


        //------------------ binary -----------------
        // reading from the disk file binary
        //-------------------------------------------

        string path_filename = gpath.spectra + name ;
        //    cout << "Trying to open the spectrum >" << path_filename << "<" << endl;
        ifstream plik(path_filename.c_str(), ios::binary);

        //double bin_left_edge ;

        if(plik)
        {

            double bins, left, right ;

            plik.read((char*) &bins, sizeof(bins));
            plik.read((char*) &left, sizeof(left));
            plik.read((char*) &right, sizeof(right));

            if(nr_spectrum == 0)
            {
                bins_0 = bins;
                left_0 = left;
                right_0 = right;
                name_0 = name;
            }
            else if(bins_0 != bins ||  left_0 != left || right_0 != right)
            {
                ostringstream warning_text ;
                warning_text
                    << "Can't add the spectrum " << name
                    << " to the sum spectrum , because ist binning is mismatched.\n"
                    << "\nDetailed explanation. The first spectrum called: " << name_0
                    << "\nhas bins: " << bins_0
                    << " left edge: " << left_0
                    << " right edge: " << right_0
                    << "\nwhile the spectrum '" << name
                    << "\nhas bins: " << bins
                    << " left edge: " << left
                    << " right edge: " << right
                    << "\nso summing of this spectrum is ignored.\nPlease change the definition of the sum spectrum!" ;

                showWarningMessage("Error" , warning_text.str().c_str(),
                                   QMessageBox::Critical);


                continue;
            }

            for(int i = 0 ; plik ; i++)
            {
                int value ;

                //plik.read((char*) &bin_left_edge, sizeof(bin_left_edge));
                plik.read((char*) &value, sizeof(value)) ;
                if(!plik)
                    break ;

                // if more than maximum counts, let's update the max Y
                if(value > specif.end_y)
                    specif.end_y = value ;

                value = static_cast<int>(value * adding_factor);  // to make subtraction possible

                if(nr_spectrum == 0)
                    spectrum_table.push_back(value) ;
                else
                    spectrum_table[i] += value ;

            } // for
            plik.close() ;


            //         specif.beg = bin_left_edge;
            //         specif.waga = waga ;
            //
            //         // now
            //         //cout << " read  spectrum, channels  " << i << endl ;
            //         specif.end = (bin_left_edge) + (spectrum_table.size() * specif.waga) ;

            specif.bin = (int) bins ;
            specif.beg = left;
            specif.end = right;
            specif.waga = (right - left) / bins ;

        }  // if plik

        else
        {
            // error while reading
            if(timer_refresh)   // so stop refreshing
            {
                timer_refresh->stop();
                delete timer_refresh ;
                timer_refresh = nullptr ;
            }
            string mess =  string("While making the sum spectrum ") + fname + "\n the contributing spectrum \n";
            mess += path_filename;
            mess += "\nwas not opened correctly  (Does it really exist now?)\n(The sum spectrum will not be refreshed anymore)" ;

            QMessageBox::information(this, "File does not exist ? ", mess.c_str());

            //cout << " error while opening a spectrum " << path_filename << endl ;
            specif.beg = 0 ;
            spectrum_table.push_back(0) ;  // one channel only
            specif.end =  spectrum_table.size() - 1 ;      //   333 ;
        }
        plik.close() ;

    } // endof for

    QApplication::restoreOverrideCursor();

    return true ;

}
//*************************************************************************
//******************************* ******************************************
/** reading many spectra, which names are stored in "@sum_" file */
bool spectrum_1D::save_sum_spectrum(string fname)
{

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    string path_filename =  gpath.spectra + fname ;

    cout << "Trying to save the spectrum >" << path_filename << "<" << endl;
    ofstream plik(path_filename.c_str(), ios::binary);
    if(!plik)
    {
        cout << "Can't open file for sum spectrum " << path_filename << endl;
        return false ;
    }



    double bins = specif.bin,
        left = specif.beg,
        right = specif.end ;

    plik.write((char*) &bins, sizeof(bins));
    plik.write((char*) &left, sizeof(left));
    plik.write((char*) &right, sizeof(right));


    cout << "Binow = " << bins
         << ", left = " << left
         << ", right = " << right << endl;
    for(unsigned int i = 0 ; i < spectrum_table.size() ; i++)
    {
        int value = spectrum_table[i];
        //cout << " [" << i << "]  " << value << "   " << endl;

        plik.write((char*) &value, sizeof(value)) ;
        if(!plik)
            break ;

    } // for
    plik.close() ;

    QApplication::restoreOverrideCursor();
    return true ;
}
//*************************************************************************
/**  no descrition */
void spectrum_1D::set_new_refreshing_times()
{
    if(name_of_spectrum.find("@sum_", 0) != string::npos)   // if this is a sum spectrum
    {
        if(timer_refresh)
            timer_refresh->start(1000 * current_options.give_refreshing_time_sum_1D());
    }
    else  // if this is a normal spectrum
    {
        if(timer_refresh)
            timer_refresh->start(1000 * current_options.give_realistic_refreshing_time_1D());
    }
}
//*************************************************************************
/** User defined comments, to make hardcopy nicer */
void spectrum_1D::add_tag_with_comment(double x, double y)
{
    remember_for_undo("Add tag comment") ;
    static QStringList lst ;
    // lst << "" ;
    bool ok;
    QString res = QInputDialog::getItem(this,
                                        "Adding a tag with a comment",
                                        "Type your comment string or choose the old one to edit it:",
                                        lst, lst.size() - 1 , true, &ok);
    if(ok)
    {
        // user selected an item and pressed OK
        nalepka.push_back(Tpinup(x, y, res.toStdString())) ;
        lst += res ;
    }
    else
    {
        // user pressed Cancel
    }

}
//*******************************************************************
/** No descriptions */
void spectrum_1D::erase_nearest_tag_with_comment(double x, double y)
{
    if(!nalepka.size())
        return;

    remember_for_undo("Erase the nearest TAG with comment") ;
    // cout << "---------- x = " << x << ", y= " << y << endl ;

    int nearest = -1 ;
    double minimum = 999999 ;
    for(unsigned int i = 0 ; i < nalepka.size() ; i++)
    {

        double distance =
            sqrt(((x - nalepka[i].give_x()) * (x - nalepka[i].give_x()))
                 + ((y - nalepka[i].give_y()) * (y - nalepka[i].give_y()))
                 );
        //    cout << " dist of [" << nalepka[i].give_x()
        //      << ", "<< nalepka[i].give_y()
        //      << "] "
        //      << nalepka[i].give_info()
        //      << " = " << distance << endl;
        if(distance < minimum)
        {
            minimum = distance ;
            nearest = i ;
        }

    }


    string mmm = "Are you sure you want to delete TAG: "
                 + nalepka[nearest].give_info() ;
    auto result = askYesNoCancel( "Deleting the TAG " , mmm.c_str() );
    if(result == QMessageBox::Yes)
    {
        // are you sure....
        nalepka.erase(nalepka.begin() + nearest);
        save_tags_to_disk();
    }

}
//*************************************************************************
/** No descriptions */
void spectrum_1D::remove_all_tags()
{
    remember_for_undo("Remove all tags") ;
    //  string mmm = "Are you sure you want to delete all tags from this spectrum ?";
    auto result =askYesNoCancel( "Deleting the all TAG " ,
                                 "Are you sure you want to delete all tags from this spectrum ?");
    if(result == QMessageBox::Yes)
    {
        // are you sure....
        nalepka.clear() ;
        save_tags_to_disk();
    }
}
/*************************************************************************/
void spectrum_1D::zero_spectrum()
{
    if(kind_of_spectrum == normal)  // sum, overplot spectra can not be zeroed on disk, bec this is just list of names
    {
        //cout << "spectrum_1D::zero_spectrum()   - zeroing 1D spectrum " << name_of_spectrum << endl;
        ofstream plik((gpath.spectra + name_of_spectrum).c_str(), ios::binary);
        if(!plik)
            return;


        //cout << "Storing zeroed spectrum " << specif << endl;

        //		plik.write((char*) &specif.bin, sizeof(specif.bin));
        double bin_double = specif.bin;   // on a disk, bin is stored as double


        plik.write((char*) &bin_double, sizeof(bin_double));
        plik.write((char*) &specif.beg, sizeof(specif.beg));
        plik.write((char*) &specif.end, sizeof(specif.end));

        int how_many = specif.bin; // 10;
        int *table = new int[how_many] ; //new int[how_many];  // warning: garbage here
        memset(table, 0, how_many *sizeof(int));

        plik.write((char*) table, sizeof(int) * how_many);   // just 10 zeros
        plik.close() ;


        //  cout << "max_counts = " << max_counts
        //    << ", specif.bin = " << specif.bin << endl;

        // drawing a speciall pattern  (like a cross - on the spectrum in memory)
        for(int i = 0 ; i < specif.bin ; i += 2)
        {
            //spectrum_table[i] = i ;
            //spectrum_table[i+1] = specif.bin  -  i ;
            spectrum_table[i] = (int)(1.0 * i * max_counts / specif.bin);
            spectrum_table[i + 1] = (int)(max_counts  - spectrum_table[i]) ;
        }

        b_spectrum ->force_new_pixmap(true);
        //        draw_all_on_screen();
    }
}
//***********************************************************************
/** reading many spectra, which names are stored in "@over_" file */
bool spectrum_1D::read_in_overplot_spectra(string fname, bool this_is_first_time)
{
    if(this_is_first_time)
    {
        overplot_vector.clear();
    }
    // opening the file with the names
    string path_names = gpath.spectra + fname ;
    ifstream plik_names(path_names.c_str());
    if(!plik_names)
    {
        cout << "Can't open file with names of the overploted spectra " << path_names << endl;
        return false ;
    }

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    for(int nr_spectrum = 0 ; plik_names ; nr_spectrum++)
    {
        vector<int> tmp_spectrum_table ;  // <--- the same name (hiding)
        spectrum_descr tmp_specif ;  // hiding


        string name ; // for spectrum name
        ifstream plik ;
        plik_names >> name ;
        if(!plik_names)
            break ;
        //     cout << "Over name " << name << endl ;


        //------------------ binary -----------------
        // reading from the disk file binary
        //-------------------------------------------

        string path_filename = gpath.spectra + name ;
        plik.open(path_filename.c_str(), ios::binary);


        if(plik)
        {
            tmp_spectrum_table.clear() ;
            double bins, left, right ;
            plik.read((char*) &bins, sizeof(bins));
            plik.read((char*) &left, sizeof(left));
            plik.read((char*) &right, sizeof(right));

            while(plik)
            {
                int value ;

                plik.read((char*) &value, sizeof(value)) ;
                if(!plik)
                    break ;

                // if more than maximum counts, let's update the max Y
                if(value > tmp_specif.end_y)
                    tmp_specif.end_y = value ;

                //if(nr_spectrum == 0)
                tmp_spectrum_table.push_back(value) ;
                //else tmp_spectrum_table[i] += value ;

            } // for

            //         tmp_specif.beg = bin_left_edge;
            //         tmp_specif.waga = waga ;
            //
            //         // now
            //         //cout << " read  spectrum, channels  " << i << endl ;
            //         tmp_specif.end = (bin_left_edge) + (tmp_spectrum_table.size() * tmp_specif.waga) ;

            tmp_specif.bin = (int) bins ;
            tmp_specif.beg = left;
            tmp_specif.end = right;
            tmp_specif.waga = (right - left) / bins ;


            if(this_is_first_time)
            {
                overplot_vector.push_back(sover(tmp_specif, tmp_spectrum_table, name, 0)); // color 0 : generate it
            }
            else
            {
                overplot_vector[nr_spectrum] = sover(tmp_specif, tmp_spectrum_table, name, overplot_vector[nr_spectrum].color);
            }

        }  // if plik
        else
        {
            // error while reading
            if(timer_refresh)   // so stop refreshing
            {
                timer_refresh->stop();
                delete timer_refresh ;
                timer_refresh = nullptr ;
            }
            string mess =  "Spectrum \n";
            mess += path_filename;
            mess += "\nwas not found \n" ;
            QMessageBox::information(this, "File does not exist ? ", mess.c_str());

            //cout << " error while opening a spectrum " << path_filename << endl ;
            tmp_specif.beg = 0 ;
            tmp_spectrum_table.push_back(0) ;  // one channel only
            tmp_specif.end =  tmp_spectrum_table.size() - 1 ;      //   333 ;
        }
        plik.close() ;

    } // endof for


    // here we may make global values
    for(unsigned int i = 0 ; i < overplot_vector.size() ; i++)
    {
        specif.beg = min(specif.beg, overplot_vector[i].sd.beg);
        specif.end = max(specif.end, overplot_vector[i].sd.end);
        specif.end_y = max(specif.end_y, overplot_vector[i].sd.end_y);
    }
    specif.bin = 0 ;
    specif.waga = 0 ;
    b_spectrum->set_overlay_mode(true);

    QApplication::restoreOverrideCursor();

    return true ;

}
/*************************************************************************************/
void spectrum_1D::name_overlay_spec_nr()
{

    bool ok;
    double res = QInputDialog::getDouble(this,
                                         "Show me the name of the spectrum", "Enter a number of chosen overlay specturm:",
                                         0, 0, overplot_vector.size() - 1, 0, &ok);
    if(ok)
    {
        // user entered something and pressed OK
        unsigned int nr = (unsigned int) res;

        QMessageBox::information(this, "Identification of the spectrum " ,
                                 QString("Spectrum nr %1 it is %2").arg(nr)
                                     .arg(overplot_vector[nr].spec_name.c_str()),
                                 QMessageBox::Ok);
    }
}
//*************************************************************************************
/** storing the parameters in case of later undo command */
void spectrum_1D::remember_for_undo(string descr_text)
{
    Tundo remember ;
    remember.min_counts = min_counts ;
    remember.max_counts = max_counts ;
    remember.min_x  = min_x  ;
    remember.max_x = max_x ;

    //  cout << "Storing in undo -==== max_x = " << max_x  << endl;;

    remember.bgr_slope =  bgr_slope ;
    remember.bgr_offset = bgr_offset ;

    remember.marker_older = marker_older ;
    remember.marker_younger = marker_younger ;

    //  cout << "Storing in undo -==== marker_older = " << marker_older
    //        << ", marker_younger= " << marker_younger  << endl;

    remember.recent_bgr_marker = recent_bgr_marker  ;
    remember.backgr_markers = backgr_markers ;

    remember.flag_bgr_valid = flag_bgr_valid  ;
    remember.my_gates_1D = my_gates_1D ;
    remember.undo_description_text  = descr_text ;
    remember.nalepka  = nalepka ;
    remember.rebin_factor = rebin_factor;

    undo_list.push_back(remember) ;

    //  cout << "Inside list, back element  has max_x = " << (undo_list.back().max_x) << endl ;

    if(undo_list.size() > 100)
        undo_list.pop_front() ;

}
//******************************************************************************************
/** To display on the menu bar  */
string spectrum_1D::give_undo_text(int /*nr*/)
{
    if(undo_list.size() > 0)
        return undo_list.back().undo_description_text ;   // returning the recent undo text
    else
        return "No Undo for selected spectrum - now";
}
//******************************************************************************************
/** No descriptions */
void spectrum_1D::print_postscript()
{
    print_it() ; // later, just change the name of the function print_it into print_postscript
}
//*****************************************************************************************
/** For printing - the channel box should be sticked to the x axis of the spectrum
     and counts box should be sticked to the y axis of the spectrum. So counts and chnnel boxes
     should ask where is the spectrum box */
//***************************************************************************
void spectrum_1D::give_Cx_Cy(double *x, double *y)
{
    *x = Cx ;
    *y =  Cy;
}
//**************************************************************************
/** For sum/difference spectrum we may need negative values */
int spectrum_1D::giveCurrentMinCounts()
{
    min_counts = 0; // minimum we make such

    if(kind_of_spectrum == overplot)
    {
        // here we may make global values
        for(unsigned int i = 0 ; i < overplot_vector.size() ; i++)
        {
            specif.end_y = max(specif.end_y, overplot_vector[i].sd.end_y);

            //cout << i << ")  max y " << overplot_vector[i].sd.end_y << endl ;
        }
        //cout << "max y " << specif.end_y << endl ;
        max_counts = (int) specif.end_y ;   // ???????????????? what and why ?

    }
    else
    {
        if(spectrum_table.size() > 0)
        {
            min_counts = 0; // minimum we make such

            // now we can imitate a spectrum
            for(unsigned int i = 0 ; i < spectrum_table.size() /*specif.bin*/  ; i++)
            {
                if(spectrum_table[i] < min_counts)
                    min_counts  =  spectrum_table[i] ;
            }
            min_counts  = (int)(1.2 * min_counts) ;
        }
    }
    return min_counts ;

}
//*****************************************************************************
/** This function sets the rebinning factor */
void spectrum_1D::rebin_on_screen()
{
    remember_for_undo("Set rebinning factor") ;
    bool ok;
    int value = QInputDialog::getInt(this,
                                     "Setting the rebinning factor", "How many neighbouring channels has to be displayed as one?:\n(1 - means no rebin)",
                                     1, 1, 512, 1, &ok);
    if(ok)
    {
        // user entered something and pressed OK
        rebin_factor = value;
        // update() ;
        b_spectrum ->force_new_pixmap(true);
        //        draw_all_on_screen();

    }
    else
    {
        // user pressed Cancel
    }
}
//***************************************************************************
/** To petrify the current state of the spectrum to display it in the "blackround" - kind of overlay */
void spectrum_1D::make_foto()
{
#ifdef NIGDY
    // Open the dialog window to give the colour and the name of the spectrum
    Tfrozen_photo_dlg dlg;
    dlg.exec();

// The options could be - add or modif colour/name-comment
#endif

}
//************************************************

/*!
        \fn spectrum_1D::freezing_photo_of_spectrum()
     */
void spectrum_1D::freezing_photo_of_spectrum()
{
    int kol =  (int) (6 - frozen_photo_vector.size());
    kol =
        (kol & 1 ? 0xff0000 : 0)
        + (kol & 2 ? 0x00ff00 : 0)
        + (kol & 4 ? 0x0000ff : 0)   ;
    time_t rawtime;
    time(&rawtime);
    string when = ctime(&rawtime) ;

    frozen_photo_vector.push_back
        (
            frozen_one_spectrum(spectrum_table, when.c_str(), kol) // kolor
            );
    b_spectrum ->force_new_pixmap(true);
    //    draw_all_on_screen();

}
//***************************************************************************
void spectrum_1D::show_list_of_incrementers()
{
    if(list_of_incrementers_of_this_spectrum.empty())
        list_of_incrementers_of_this_spectrum = "No information available about this spectrum";

    QMessageBox::information(this, "List of incrementers of this spectrum",
                             list_of_incrementers_of_this_spectrum.c_str(),
                             QMessageBox::Ok);
}
//***********************************************************************
void spectrum_1D::show_time_of_last_zeroing()
{
    time_t when  =
        appl_form_ptr->give_time_of_zeroing_spectrum(name_of_spectrum);

    string mess ;
    if(when)
    {
        mess =  string("Last zeroing of the spectrum ") + name_of_spectrum
               + " was at:  " + ctime(&when) ;
    }
    else
    {
        mess = "Sorry, an information about the zeroing of this spectrum is currently not available";
    }
    QMessageBox::information(this, "Time of last zeroing",
                             mess.c_str(),
                             QMessageBox::Ok);
}
//*******************************************************************
void spectrum_1D::draw_scale_or_not(bool stan)
{
    flag_draw_scales = stan;
    resizeEvent(nullptr);
    // nonsense to :   remember_for_undo("log/linear view") ;
    b_spectrum ->force_new_pixmap(true);
    //    draw_all_on_screen();
}

//*******************************************************************
void spectrum_1D::read_tags_from_disk()
{
    // opening the file
    string name = gpath.my_binnings + name_of_spectrum + ".pinuptxt" ;
    //   cout << "Trying to open the file: >" << name << "<" << endl;
    ifstream s(name.c_str());
    if(!s)
    {
        //     cout << "file does not exist" << endl;
        return;
    }
    // loop over the tags
    for([[__maybe_unused__]]  unsigned int i = 0 ;  ; i++)
    {
        //     cout << "Reading nalepka nr " << i << endl;
        Tpinup nal;

        nal.read_from_disk_file(s);
        if(!s || s.eof())
            break;
        nalepka.push_back(nal);
    }
    s.close();
}
//*******************************************************************
void spectrum_1D::save_tags_to_disk()
{
    // opening the file
    //   cout << "spectrum_1D::save_tags_to_disk() for " << name_of_spectrum << endl;
    string name = gpath.my_binnings + name_of_spectrum + ".pinuptxt" ;

    if(nalepka.size()  == 0)
    {
        //   cout << "removing the file" << name << endl;
        remove
            (name.c_str());
        return;
    }

    ofstream s(name.c_str());
    if(!s)
    {
        cout << "Can't open file " << name << " for writing " << endl;
        return;
    }
    // loop over the tags
    for(unsigned int i = 0 ; i < nalepka.size() ; i++)
    {
        //     cout << "Saving nalepka nr " << i << endl;
        nalepka[i].save_to_disk_file(s);
    }
    s.close();
}
//***************************************************************************
void spectrum_1D::set_layout_normal()
{

    // cout << "spectrum_1D::set_layout_normal()" << endl;

    flag_impossible_to_draw_scales = false;
    layout->invalidate();


#ifdef RAINBOW_SCALE
    //	layout->setColumnStretch(0,3);
    //	layout->setColumnStretch(1, 90);  // (1,90);
    //	layout->setColumnStretch(2,3);          // 3
#else
    layout->setColumnStretch(0,20);
    layout->setColumnStretch(1,980);
#endif

    layout->setRowStretch(0,97);
    layout->setRowStretch(1,3);

    layout->setColumnMinimumWidth(0, 19);

    layout->setRowMinimumHeight(1, 15);


    layout->setSpacing(0);

    setLayout(layout);
}
//***************************************************************************
void spectrum_1D::set_layout_without_scale()
{
    //cout << "spectrum_1D::set_layout_without_scale()" << endl;

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

}
//***************************************************************************
void spectrum_1D::slot_set_bg_marker()
{
    // ----------------------set marker
    remember_for_undo ( "Set background marker" );
    set_backgr_marker ( b_spectrum->give_x_mouse_click() );
}
//***************************************************************************
void spectrum_1D::slot_erase_all_bg_markers()
{
    remember_for_undo ( "Erase all background markers" );
    backgr_markers.clear() ;
    bgr_slope = 0 ;
    bgr_offset = 0 ;
    flag_bgr_valid = false ;
    recent_bgr_marker = -999999 ;  // nothing have been set yet
}
//***************************************************************************
void spectrum_1D::slot_erase_recent_bg_marker()
{
    remember_for_undo ( "Erase recent background marker" );
    vector <typ_x>::iterator Iter;

    //cout << "lista markerow =" ;
    if ( backgr_markers.empty() )
        return;
    //cout << "lista markerow ma rozmiar =" << backgr_markers.size( )
    // << endl  ;

    for ( Iter = backgr_markers.begin() ; Iter != backgr_markers.end() ; Iter++ )
    {
        //cout << " " << *Iter << " while recent was = "
        //<< recent_bgr_marker << endl ;
        if ( fabs(recent_bgr_marker - *Iter) < 0.001 )
        {
            backgr_markers.erase ( Iter );
            recent_bgr_marker = -999999 ;
            break ;  // out of for loop
        }
    }
    //cout << endl;

    bgr_slope = 0 ;
    bgr_offset = 0 ;
    flag_bgr_valid = false ;
}
//***************************************************************************
void spectrum_1D::slot_erase_nearest_bg_marker()
{
    remember_for_undo ( "Erase the nearest background marker" );
    erase_nearest_bgr_marker ( b_spectrum->give_x_mouse_click() );
}
//***************************************************************************
void spectrum_1D::slot_save_bg_markers_as_default_gate()
{
    save_bg_markers_as_gate();
}
//***************************************************************************
void spectrum_1D::slot_remove_default_gate()
{
    remember_for_undo ( "Remove the GATE" );
    remove_the_gate_1D() ;
}
//***************************************************************************
void spectrum_1D::slot_place_integ_marker_by_value()
{
    set_integr_marker() ;
}
//***************************************************************************
void spectrum_1D::slot_remember_all_markers()
{
    remember_integration_and_bgr_markers() ;
}
//***************************************************************************
void spectrum_1D::slot_paste_all_markers()
{
    paste_integration_and_bgr_markers() ;
}
//***************************************************************************
void spectrum_1D::slot_remove_all_tags()
{
    remove_all_tags();
}
//***************************************************************************
void spectrum_1D::slot_add_a_tag()
{
    add_tag_with_comment (
        b_spectrum->give_x_mouse_click(),
        b_spectrum->give_y_mouse_click() );
}
//***************************************************************************
void spectrum_1D::slot_add_nalepka()
{
    if ( !nalepka.size() )
        return;
    erase_nearest_tag_with_comment (
        b_spectrum->give_x_mouse_click(),
        b_spectrum->give_y_mouse_click() );
}
//***************************************************************************
void spectrum_1D::slot_name_overlay_spec_nr()
{
    name_overlay_spec_nr();
}
//***************************************************************************
void spectrum_1D::slot_rebin_on_screen()
{
    rebin_on_screen();
}
//***************************************************************************
void spectrum_1D::slot_make_foto()
{
    make_foto();
}
//***************************************************************************
void spectrum_1D::slot_show_list_of_incrementers()
{
    show_list_of_incrementers() ;
}
//***************************************************************************
void spectrum_1D::slot_times_of_last_zeroing()
{
    show_time_of_last_zeroing();
}
//***************************************************************************
void spectrum_1D::mouseMoveEvent ( QMouseEvent * e )
{

    appl_form_ptr->send_to_statusbar(
        QString ( "Spectrum name:    %1" )
            .arg(
                give_spectrum_name().c_str()).toStdString(),

        10*1000 ); //  seconds on the screen
    e->ignore() ;
}
//***************************************************************************************************
void spectrum_1D::slot_help_about_1D_mouse_clicking()
{

    cout << "Wywolanie funkcji help" << endl;

    showWarningMessage("Help information about mouse clicking on 1D spectra" ,
                       QString (

                           "On the 1D spectrum display you may use following actions with a mouse:\n\n"

                           "   *  Single click   -  to set a yellow marking line\n"
                           //"Shift + Single click   -  correct the last \"single click marker\"\n"
                           "   *  Double  click   -  to expand the region marked with two marking lines\n"
                           "   *  Shift  + double click   -  Zoom OUT the region\n"
                           "   *  Right click - show popup menu \n\n\n"

                           "On a X-axis box (or on a Y-axis box) of the spectrum you may:\n\n"
                           "   *  Roll a mouse wheel -  to  expand/compress the scale\n"
                           "   *  Press a mouse and drag  - to move along this axis (if possible)\n"
                           "\n"
                           ), QMessageBox::Information);
}
//***************************************************************************************************
