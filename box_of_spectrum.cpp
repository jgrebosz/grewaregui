#include "box_of_spectrum.h"
#include "tplate_spectrum.h"
#include "spectrum_1D.h"
#include "gate_1D.h"

#include <qstring.h>
#include <QPainterPath>
#include <math.h>

#include <iostream>

#undef COTO
#define COTO

#include "appl_form.h"
extern appl_form  *appl_form_ptr;

extern QColor background_colour_of_1D_spectrum;
class spectrum_1D ;
//*******************************************************************

box_of_spectrum::box_of_spectrum (
    //        double factor_left,
    //                                   double factor_top,
    //                                   double factor_right,
    //                                   double factor_bottom,
    QWidget *a_parent, string name ) :
    QWidget ( a_parent) , swiatek()
{
    setWindowTitle(name.c_str() );
    // remembering the factors
    //    f_left  = factor_left;
    //    f_top = factor_top;
    //    f_right = factor_right;
    //    f_bottom = factor_bottom;

    parent = dynamic_cast< spectrum_1D* > ( a_parent ) ; // we need it 1D

    // parent = (spectrum_1D*)a_parent ;
    // parent = dynamic_cast<spectrum_widget*>(a_parent) ;

    spectrum_ptr = nullptr ;
    printer = nullptr ;
    overlay_mode = false ;

    // constructing a frame responsible for this scale
    //     setPaletteBackgroundColor ( parent->give_flag_black_white() ?
    //                                 Qt::white
    //                                 :
    //                                 QColor ( 0, 85,0 ) ); // dark green

    QColor color =  (parent->give_flag_black_white() ?
                        Qt::white
                                                    :
                        //QColor (0, 85,0)  // just green
                        //                         QColor (0, 45,0)   // very dark green
                        background_colour_of_1D_spectrum   // very dark blue

                    );  // dark green

    QPalette palette;
    palette.setColor(backgroundRole(), color);
    palette.setColor(QPalette::AlternateBase, background_colour_of_1D_spectrum);  // Window, Window AlternateBase
    setPalette(palette);


    
    //setPaletteBackgroundColor ( QColor (0, 0,85) );  // <- blue marine

    //setGeometry(QRect(10,10, 100, 300)) ;

    flag_was_mouse_click = false ;
    x_mouse_click = 0;
    y_mouse_click = 0;
    pressed_button_state = Qt::NoButton ;
    setMouseTracking ( true );
    //    setMouseTracking (false  );

    //    setContextMenuPolicy(Qt::CustomContextMenu);
    //    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ctxMenu(const QPoint &)));


}
//*********************************************************************
void box_of_spectrum::showSpectrumContextMenu(QPoint pos)
{

    QPoint globalPos = pos;
    QPoint localPos = mapFromGlobal(globalPos);

    // some options of popup menu would like to have it in "channels"
    x_mouse_click = typ_x ( pix2worX (localPos.x()) );
    y_mouse_click = typ_x ( pix2worY (localPos.x()) );
    parent->moj_showContextMenu(localPos);

    // cout << " end of "<< __PRETTY_FUNCTION__ << endl;
}
//*********************************************************************
void box_of_spectrum::ctxMenu(const QPoint &) // [[deprecated]]
{

}

//***********************************************************************
void box_of_spectrum::paintEvent ( QPaintEvent * )
{
    QPainter piorko_ekranowe ( this );
    //    QPainter painter(this->viewport());
    //piorko_ekranowe.begin(parent);
    //	static int counter;
    //	cout << __PRETTY_FUNCTION__ << " update box_spectrum  " << (counter++) << endl;
    draw_all ( &piorko_ekranowe );
}
//***********************************************************************
void box_of_spectrum::draw_all ( QPainter * pior )
{
    // COTO;
    //    int tmp_poziom = 60 ;
    //    pior->drawText(5, tmp_poziom += 20 , "5555 pocz funkcji box_of_spectrum::draw_all ");
    //    QBrush b1( Qt::blue );
    //    QBrush b2( Qt::green, Qt::Dense6Pattern );    // green 12% fill
    //    QBrush b3( Qt::NoBrush );       // void brush
    //    QBrush b4( Qt::CrossPattern );      // Qt::black cross pattern
    //
    //    pior->setPen( Qt::red );
    //    pior->setBrush( Qt::yellow );
    //    pior->drawRect( 40, 40, 80, 600 );


    // spectrum_widget *p = dynamic_cast<spectrum_widget*> parent ;
    spectrum_1D *p = ( spectrum_1D* ) parent ;
    krot = p->give_rebin_factor();

    //    bool flag_printing_ps = parent->printing_mode();

    //asking for a geometry   of the parent widget
    //    QSize size_of_window = parent->size() ;

    //    setGeometry ( ( int ) ( f_left * size_of_window.width() ),
    //                  ( int ) ( f_top * size_of_window.height() ),
    //                  ( int ) ( ( f_right-f_left ) * size_of_window.width() ),
    //                  ( int ) ( ( f_bottom-f_top ) * size_of_window.height() ) );




    // drawing the frame

    // cout << "before printing " << endl ;
    QPainter &piorko = *pior ;

    QRect r = rect() ;
    //   pior->setClipRect (r) ;


    //bool , now member
    black_white = parent->give_flag_black_white() ;


    if( background_colour_of_1D_spectrum == QColor(255,255,255) )
    {
        black_white = true;
    }


    // do we have to prepare the new pixmap ?
    if ( r.size() != pix.size()  || make_new_pixmap  )
    {
        //    pior->drawText(5, tmp_poziom += 20, "po if new pixmap ");

        //    cout << "making new pixmap sizes of rect and pixmap are "
        //      << ((r.size() != pix.size() ) ? " different " : " equal")
        //      << " flag make new pix = " << make_new_pixmap
        //      << endl ;
        make_new_pixmap = false ;



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
        piorko.setPen ( black_white ? Qt::black : Qt::white ); // preparing the pen


        // To test proste widmo 1D
        piorko.end();   // end of normal painting on the screen
        piorko.begin ( &pix ); // start to paint on the pixmap

        //        cout << __FILE__ << ", "<< __LINE__ << " backgr_color R=" << backgr_color.red() << endl;
        pix.fill ( black_white ? Qt::white  : background_colour_of_1D_spectrum ); // so - no data, we do not have to draw anything


        // drawing a sunken frame --------------------------
        piorko.setPen ( black_white ? Qt::black : Qt::darkGray);
        piorko.drawLine ( r.bottomLeft(), r.topLeft() );
        piorko.drawLine ( r.topLeft(), r.topRight() );
        piorko.setPen ( black_white ? Qt::black : Qt::white );
        piorko.drawLine ( r.topRight(), r.bottomRight() );
        piorko.drawLine ( r.bottomRight(), r.bottomLeft() );


        // drawing a contents -----------------------------

        //          cout << "in box_of_spectrum before World setting  x "
        //         <<   minX << "-"
        //         <<  maxX
        //         <<  "  Y= " << minY
        //         << "-" <<  maxY
        //         << endl ;

        piorko.setPen ( black_white ? Qt::black : Qt::white ); // prepering the pen

        ustaw_swiat ( r.left(), r.top(), r.right(), r.bottom(), // preparing the coordinates
                    minX, maxY, maxX, minY );


        //  cout << "rect left = " << r.left()
        //      << ",  top = "  << r.top()
        //      << ",  right = " <<  r.right()
        //      << ",  bottom = " <<  r.bottom()
        //      << endl;

        //    pior->drawText(5, tmp_poziom += 20, "888  po ustaw swiwat ");

        //    pior->drawText(wx(20), wy(70), "wx 99999 wg wspolrzednych swatatowych ");


        //QFont sansFont("Helvetica [Cronyx]");

        //int rozm =(int) ( 0.1 * QMIN( r.width(), r.height()  ) )  ; // adjusting the size of the font to the size of the widget
        // int rozm = 20    ;
        //sansFont.setPixelSize(rozm);

        //sansFont.setStyleHint( QFont::SansSerif );
        piorko.setFont ( sansFont );



        enum mode_of_drawing { normal_single_mode, overlay_spectra_mode, frozen_photo_mode } current_mode;
        current_mode = normal_single_mode;
        // In case overplot or freezing - we have to draw more than one spectrum
        unsigned int how_many_specta_to_draw  = 1;  // normal situation

        if ( overlay_mode )
        {
            how_many_specta_to_draw   = parent->overplot_vector.size() ;
            current_mode = overlay_spectra_mode;
        }
        else if ( parent->frozen_photo_vector.size()  > 0 )
        {
            current_mode =  frozen_photo_mode;
            how_many_specta_to_draw   = parent->frozen_photo_vector.size() +1 ;
        }

        //     cout
        //     //<< "SO FAR FROZEN ARE " << tmp
        //     << ",  current_mode = "<< current_mode
        //     << ", frozen_photo_vector.size() = " << parent->frozen_photo_vector.size()
        //     << endl;

        //*************************************************
        // Loop over all spectra which have to be drawn on this picutre (one - normal, many - for overplot option)

        for ( unsigned nr = 0 ; nr < how_many_specta_to_draw ; nr++ )
        {

            /*
                  vector<int> & widmo =
                                             (!overlay_mode) ?
                                                      (*spectrum_ptr)
                                             :
                                       (parent->overplot_vector[nr].spec_table) ;
            */
            /**/

            vector<int> *where_spectrum  = nullptr;

            switch ( current_mode )
            {
            default:
            case normal_single_mode:
                where_spectrum = spectrum_ptr;
                break;

            case overlay_spectra_mode:
                where_spectrum =   &parent->overplot_vector[nr].spec_table ;
                //         cout << "overplot vecor " << endl;
                //         specif->waga = (specif->end - specif->beg)/ specif->bin ;   // zlotowki na kanal
                //         cout << "specif->waga = " << specif->waga << endl ;
                specif->waga = 5; // fake
                break;

            case frozen_photo_mode:
                if ( nr == how_many_specta_to_draw -1 )
                    where_spectrum = spectrum_ptr;  // the oryginal spectrum drawn as last (to be on the top)
                else
                    where_spectrum = & ( parent->frozen_photo_vector[nr].spec_frozen ); // all next are frozen
                break;
            }
            // finally we can set the reference
            vector<int> & widmo =  *where_spectrum;


            //       cout << "Drawing  spectrum nr " << nr << endl;

            /**/
            //    piorko.moveTo(wx(minX),   wy(widmo[minX]) );
            //    for(int i = (int) minX ; i < (int)maxX ; i ++)
            //    {
            //  piorko.lineTo(wx(i),   wy(widmo[i])) ;
            //  piorko.lineTo(wx(i+1), wy(widmo[i]));
            //    }


            // calcutaling the range //  obliczenie zakresu do narysowania

            //       double xwaga = (specif->end - specif->beg)/ specif->bin ;   // zlotowki na kanal
            //       cout << "xwaga = " << xwaga << endl ;
            if ( specif->waga == 0 )
            {
                //         cout << " waga is 0. most probably he spectrum was not correctly read. " << endl;
                // It may happen, when we were trying to read
                // the spectrum in the same time, when the spy was trying to write it
                continue;  // go to the next spectrum
            }

            int first_drawn_channel = ( int ) ( ( minX - specif->beg ) / specif->waga )  ;
            int last_drawn_channel  = ( int ) ( ( maxX - specif->beg ) / specif->waga ) ;

            /*--
            cout  << ", first_drawn_channel="   <<  first_drawn_channel
            << ", last_drawn_channel=" <<   last_drawn_channel << endl;
            -------*/

            double waga = specif->waga ;
            double beg = specif->beg ;

            // preparation before drawing the first channel of this specrum
            COTO;
            if ( current_mode == frozen_photo_mode )
            {
                if ( nr == how_many_specta_to_draw -1 )
                {
                    piorko.setPen ( black_white ?    Qt::black   :   Qt::white ); // oryginal is non-coloured
                }
                else // petrified versions
                {
                    COTO;
                    // changing colour of the petrified  spectrum
                    // opis
                    sansFont.setPixelSize ( 12 );
                    piorko.setFont ( sansFont );
                    piorko.setPen ( black_white ?    Qt::black   :   Qt::yellow);

                    if ( nr==0 )
                        piorko.drawText ( wx ( minX ),
                                        wy ( maxY- ( 0.01 * ( maxY-minY ) ) ) +13   ,
                                        "frozen clone done at" );
                    COTO;
                    piorko.setPen ( black_white ?
                                      parent->frozen_photo_vector[nr].color //+ 0x808080
                                              :
                                      parent->frozen_photo_vector[nr].color //+ 0x808080
                                  );   // prepaing the pen
                    COTO;
                    // opis
                    //        sansFont.setPixelSize(12);
                    //        piorko.setFont(sansFont);
                    //        int how_many_over = parent->overplot_vector.size();
                    //        double dist = 1.0 / (2* how_many_over);
                    COTO;
                    QString napis = QString ( " %1) %2" ).arg ( nr ).arg ( parent->frozen_photo_vector[nr].spec_name.c_str() );
                    COTO;
                    //        QString napis = QString(" %1)").arg(nr);
                    COTO;
                    //   piorko.drawText(wx(minX),
                    //                               wy(maxY- (0.01 *(maxY-minY)) ) + (13*(nr+1))  ,  napis );
                    COTO;

                    piorko.drawText ( wx ( minX + ( 0.05 * ( nr / 20 ) * ( maxX-minX ) ) ),
                                    wy ( maxY- ( 0.01 * ( maxY-minY ) ) ) + ( 13* ( ( ( nr ) %20 ) +1 ) ) +13 ,
                                    napis );

                    // piorko.moveTo ( wx ( minX ),   wy ( 0 ) );
                    QPainterPath skok;
                    skok.moveTo ( wx ( minX ),   wy ( 0 ) );
                    piorko.drawPath(skok);

                } // end  non-oryginal
            } // end of petrified

            if ( overlay_mode )
            {

                first_drawn_channel = ( int ) ( ( minX - parent->overplot_vector[nr].sd.beg )
                                             / parent->overplot_vector[nr].sd.waga )  ;
                first_drawn_channel = max ( 0, first_drawn_channel );

                last_drawn_channel  = ( int ) ( ( maxX - parent->overplot_vector[nr].sd.beg )
                                            / parent->overplot_vector[nr].sd.waga ) ;
                //                last_drawn_channel  = min ( parent->overplot_vector[nr].sd.bin-1, (double) last_drawn_channel );
                last_drawn_channel  = min ( parent->overplot_vector[nr].sd.bin-1, last_drawn_channel );

                waga = parent->overplot_vector[nr].sd.waga ;
                beg = parent->overplot_vector[nr].sd.beg ;
                //         cout << "Drawing overlay spectrum nr " << nr << endl;
                // changing colour of the spectrum
                // opis
                sansFont.setPixelSize ( 12 );
                piorko.setFont ( sansFont );
                piorko.setPen ( black_white ?    Qt::black   :   Qt::yellow);

                constexpr int HOW_MANY_MAXIMUM_SPECTRA_NAMES = 8;
                string opis;
                if ( nr==0 ){
                    if(how_many_specta_to_draw < HOW_MANY_MAXIMUM_SPECTRA_NAMES){
                        opis = " Overlayed spectra are: ";
                    }else {
                        opis = " Right mouse button to see the name of the spectrum nr..." ;
                    }
                }

                piorko.drawText ( wx ( minX ),
                                wy ( maxY- ( 0.01 * ( maxY-minY ) ) ) +13   ,
                                opis.c_str() );
                piorko.setPen ( black_white ?
                                  parent->overplot_vector[nr].color + 0x808080
                                          :
                                  parent->overplot_vector[nr].color + 0x808080
                              );   // prepaing the pen
                // opis
                //        sansFont.setPixelSize(12);
                //        piorko.setFont(sansFont);
                //        int how_many_over = parent->overplot_vector.size();
                //        double dist = 1.0 / (2* how_many_over);

                QString napis;
                if(how_many_specta_to_draw < HOW_MANY_MAXIMUM_SPECTRA_NAMES){
                    napis = QString(" %1) %2").arg(nr).arg(  parent->overplot_vector[nr].spec_name.c_str());
                }else {
                    napis = QString ( " %1) " ).arg ( nr );
                }

                //   piorko.drawText(wx(minX),
                //                               wy(maxY- (0.01 *(maxY-minY)) ) + (13*(nr+1))  ,  napis );


                piorko.drawText ( wx ( minX + ( 0.05 * ( nr / 20 ) * ( maxX-minX ) ) ),
                                wy ( maxY- ( 0.01 * ( maxY-minY ) ) ) + ( 13* ( ( ( nr ) %20 ) +1 ) ) +13 ,
                                napis );

                //piorko.moveTo ( wx ( parent->overplot_vector[nr].sd.beg ),   wy ( 0 ) );
                QPainterPath skok;
                skok.moveTo ( wx ( parent->overplot_vector[nr].sd.beg ),   wy ( 0 ) );
                piorko.drawPath(skok);
            }
            else
            {
                // piorko.moveTo(wx(minX),   wy(widmo[first_drawn_channel ]) );
                //piorko.moveTo ( wx ( minX ),   wy ( 0 ) );
                QPainterPath skok;
                skok.moveTo ( wx ( minX ),   wy ( 0 ) );
                piorko.drawPath(skok);
                
            }

            COTO;
            // after preparation we can make the loop over all drawn channels of this spectrum

            if ( p->give_flag_log_scale() == false )   // LINEAR SCALE ========
            {
                //  piorko.moveTo(wx(minX),   wy(widmo[first_drawn_channel ]) );
                int county ;
                QPainterPath skok;
                // loops goes on real channels !----------------------------------------
                for ( int i = first_drawn_channel ; i <= last_drawn_channel ; i ++ )
                {
                    if ( krot == 1 ) // no rebin on screen
                    {
                        county =wy ( ( widmo[i] > maxY ) ?
                                        maxY + ( ( minY + maxY ) >> 3 )
                                                      :
                                        widmo[i] );
                    }
                    else // for rebining on the screen
                    {
                        static int mean = 0 ;

                        if ( ! ( ( i-first_drawn_channel ) % krot ) ) // every n-th channel we make the new mean
                        {
                            mean = 0 ;
                            for ( int k = 0 ; k < krot ; k++ )
                            {
                                mean += widmo[i] ;
                            }
                            //mean /= krot;   // <-- commented because people say, that is should be sum, not mean
                        }
                        // now we use this mean
                        county =wy ( ( mean > maxY ) ?
                                        maxY + ( ( minY + maxY ) >> 3 )
                                                  :
                                        mean );
                        cout << "county = " << county << endl;
                    }

                    skok.lineTo ( wx ( ( i ) * waga + beg ),      county ) ;
                    skok.lineTo ( wx ( ( i+1 ) * waga + beg ), county ) ;
                    
                    //cout << "channel " << i << " has value " << widmo[i] << endl;

                }
                piorko.drawPath(skok);
            }
            else
            {
                // LOG scale==========================================
                //piorko.setPen(red);   // prepering the pen

                QPainterPath skok;

                ustaw_swiat ( r.left(), r.top(), r.right(), r.bottom(), // preparing the coordinates
                            minX, maxY ,
                            maxX, minY );

                // loops goes on real channels !----------------------------------------
                //piorko.moveTo(wx(minX),   wy(log10(widmo[first_drawn_channel ]) )) ;
                double county ;
                for ( int i = first_drawn_channel ; i <= last_drawn_channel ; i ++ )
                {
                    if ( krot == 1 ) // no rebin on screen
                    {
                        county  =  widmo[i] ;
                    }
                    else // for rebining on the screen
                    {
                        static int mean = 0 ;
                        if ( ! ( ( i-first_drawn_channel ) % krot ) ) // every n-th channel we make the new mean
                        {
                            mean = 0 ;
                            for ( int k = 0 ; k < krot ; k++ )
                            {
                                mean += widmo[i] ;
                            }
                            // mean /= krot;             // <-- commented because people say, that is should be sum, not mean
                        }
                        // now we use this mean
                        county =mean;
                    }
                    //cout << "counts=  " <<  county ;
                    if ( county == 0 )
                        county = 0.1 ;
                    county = log10 ( county ) ;
                    //cout << ",  log10(counts)=  " <<  county  << endl ;
                    county = wy ( county ) ;
                    //minX+ (i*waga) ;
                    //piorko.lineTo ( wx ( ( i ) *  waga +  beg ), ( int ) county ) ;
                    //piorko.lineTo ( wx ( ( i+1 ) * waga +  beg ), ( int ) county ) ;

                    //                    QPainterPath skok;
                    skok.lineTo ( wx ( ( i ) * waga + beg ),     (int) county ) ;
                    skok.lineTo ( wx ( ( i+1 ) * waga + beg ), (int) county ) ;
                    //                    piorko.drawPath(skok);
                }

                // helpfull log grig ----------------
                //               piorko.setPen(black_white ? Qt::black : Qt::red);
                //               for(int i = 1 ; i < 100000 ; i *= 10)
                //               {
                //                   piorko.moveTo(wx(beg),   wy(i) );
                //                   piorko.lineTo   (wx(specif->end),   wy(i)) ;
                //                   // opis
                //                   QString napis = QString("%1").arg(i) ;
                //                   piorko.drawText(wx(beg),   wy(i),  napis );
                //               }

                piorko.drawPath(skok);
            }  // endof log scale
        }// end for overlay

    } // end of drawing on pixmap
    else
    {
        // id est if the old pixmap is good

        // cout << "sizes of rectangle and pixmap are equall, so just przylepienie  " << endl ;
    }

    // Spectrum one/many was drawn on this pictrure. Now we can draw some extra things
    // like markers, etc.

    //    if ( flag_printing_ps ==false )
    //    {


    piorko.end();
    // this we do with old AND with new pixmap !
    //    cout << "zawsze przylepienie bitmapy" << endl;
    piorko.begin ( this );
    piorko.drawPixmap ( r.topLeft(), pix );
    //    }


    //      cout << "Drawing in a range " << minX << "-" << maxX
    //        << ", waga " << specif->waga
    //        << " specif bins= " << specif->bin
    //         <<  ", specif->beg" << specif->beg
    //        << ", specif->end=" << specif->end
    //   << ", first_drawn_channel="   <<  first_drawn_channel
    //  << ", last_drawn_channel=" <<   last_drawn_channel
    //        << endl ;

    draw_integration_markers ( piorko );
    draw_background_markers ( piorko );
    draw_1Dgates ( piorko );

    // pinup infos ====================
    double fontsiz = rect().width() / 50.0;
    if(fontsiz > 5)
        draw_pinup_notices(piorko, fontsiz);

}
//***********************************************************************
void box_of_spectrum::draw_integration_markers ( QPainter& piorko )
{
    // here we can draw integration markers --------------------------------------------

    //    cout << "Markers on the screen are "
    //   << marker[0] << " and " << marker[1]  << endl;

    if ( marker[0]  ||  marker[1] ) // if both are Not zero
    {
        piorko.setPen ( black_white ? Qt::black : Qt::yellow); // preparing the pen

        for ( int i = 0 ; i < 2 ; i ++ ) // drawing 2 markers
        {
            //piorko.moveTo ( wx ( marker[i] ),   wy ( minY ) );
            //piorko.lineTo ( wx ( marker[i] ),   wy ( maxY ) ) ;
            QPainterPath skok;
            skok.moveTo ( wx ( marker[i] ),   wy ( minY ) );
            skok.lineTo ( wx ( marker[i] ),   wy ( maxY ) ) ;
            piorko.drawPath(skok);
            
            // opis
            QString napis = QString ( "%1" ).arg ( marker[i] ) ;
            piorko.drawText ( wx ( marker[i] ),   wy ( maxY- ( 0.1 * ( i +1 ) * ( maxY-minY ) ) ),  napis );
        }
    }
}
//*********************************************************************
void box_of_spectrum::draw_background_markers ( QPainter& piorko )
{
    //QPen p(green, 1,
    //piorko.setPen(green);   // preparing the pen
    piorko.setPen ( QPen ( black_white ? Qt::black : Qt::green, 1, Qt::DashLine ) ); // preparing the pen
    const vector<typ_x> &bg_markers = * ( ( spectrum_1D* ) ( parent ) )->give_bg_markers() ;

    for ( unsigned int i = 0 ; i <   bg_markers.size() ; i ++ ) // drawing all markers
    {
        //         piorko.moveTo ( wx ( bg_markers[i] ),   wy ( minY ) );
        //         piorko.lineTo ( wx ( bg_markers[i] ),   wy ( maxY ) ) ;

        QPainterPath skok;
        skok.moveTo ( wx ( bg_markers[i] ),   wy ( minY ) );
        skok.lineTo ( wx ( bg_markers[i] ),   wy ( maxY ) ) ;
        piorko.drawPath(skok);
        
        // opis
        QString napis = QString ( "%1 %2" ).arg ( char ( 'a'+i ) ). arg ( bg_markers [i] ) ;

        piorko.drawText ( wx ( bg_markers[i] ),   wy ( maxY- ( 0.05 * ( i +1 ) * ( maxY-minY ) ) ),  napis );
    }



    // drawing the line of the background ------------------------------------------
    double slope, offset ;
    if ( parent->give_bg_info ( &slope,  &offset ) )
    {
        piorko.setPen ( QPen ( black_white ? Qt::black : Qt::cyan, 1, Qt::SolidLine ) ); // preparing the pen
        int last_bgr_point =  bg_markers.size()-1 ;
        if ( bg_markers.size() % 2 )
            last_bgr_point-- ;    // if impair nr of bgr markers

        double y1 = ( slope* ( bg_markers[0] ) + offset ) ;
        double y2 = ( slope* ( bg_markers[ last_bgr_point  ] ) + offset ) ;

        if ( parent->give_flag_log_scale() == true )   //%%%%%%%%%%%%%%%%%%
        {
            /*----
            if (y1 > 0 && y2 > 0)
            {
            y1 = log10(y1) ;
            y2 = log10(y2) ;
            piorko.moveTo(wx( bg_markers[0]), wy(  y1)) ;            // x1 y1
            piorko.lineTo(wx( bg_markers[last_bgr_point ]), wy(y2 ) ) ;   //x2 y2
            }
            else
            ----*/
            {
                // drawing for strange logarythm, so that we draw only this what is bigger than zero
                // drawing channel by channel
                for ( int i = static_cast<int> ( bg_markers[0] ) ; i <= bg_markers[last_bgr_point] ; i++ )
                {
                    double p1 = ( i * slope )
                    + offset ;
                    double p2 = ( ( i+1 ) * slope ) + offset ;
                    if ( p1 > 0 && p2 > 0 )
                    {
                        //                         piorko.moveTo ( wx ( i ), wy ( log10 ( p1 ) ) ) ; // x1 y1
                        //                         piorko.lineTo ( wx ( i+1 ), wy ( log10 ( p2 ) ) ) ; //x2 y2
                        QPainterPath skok;
                        skok.moveTo ( wx ( i ), wy ( log10 ( p1 ) ) ) ; // x1 y1
                        skok.lineTo ( wx ( i+1 ), wy ( log10 ( p2 ) ) ) ; //x2 y2
                        piorko.drawPath(skok);

                    }
                } // endof for
            }
        }
        else  // non logaritmic draw //%%%%%%%%%%%%%%%%%%%%%%%
        {
            //             piorko.moveTo ( wx ( bg_markers[0] ), wy ( y1 ) ) ;      // x1 y1
            //             piorko.lineTo ( wx ( bg_markers[last_bgr_point ] ), wy ( y2 ) ) ; //x2 y2

            QPainterPath skok;
            skok.moveTo ( wx ( bg_markers[0] ), wy ( y1 ) ) ;      // x1 y1
            skok.lineTo ( wx ( bg_markers[last_bgr_point ] ), wy ( y2 ) ) ; //x2 y2
            piorko.drawPath(skok);
            

            //cout << "linia od puntku " << bg_markers[0] << ", " << y1 << endl ;
            //cout << "        do puntku " << bg_markers[last_bgr_point] << ", " << y2 << endl ;
        }

    }
    else
    {
        // cout << "no give_bg_info?" << endl ;
    }


}
//******************************************************************************
void box_of_spectrum::draw_1Dgates ( QPainter & piorko )
{
    const  vector<gate_1D>& my_gate = ( ( spectrum_1D* ) ( parent ) )->give_vector_of_gates();

    for ( unsigned int i = 0 ; i <   my_gate.size() ; i ++ ) // drawing all gates
    {
        // every gate can have many points
        double gate_height = maxY - ( ( i+1 ) * 0.2* ( maxY - minY ) ) ;
        for ( unsigned int k = 0 ; k < my_gate[i].boundaries.size() ; k+= 2 )
        {
            piorko.setPen ( QPen ( black_white ? Qt::black : Qt::darkYellow, 2, Qt::DashDotDotLine ) ); // preparing the pen

            //             piorko.moveTo ( wx ( my_gate[i].boundaries[k] ),   wy ( minY ) ); // left side
            //             piorko.lineTo ( wx ( my_gate[i].boundaries[k] ),   wy ( gate_height ) ) ;
            QPainterPath skok;
            skok.moveTo ( wx ( my_gate[i].boundaries[k] ),   wy ( minY ) ); // left side
            skok.lineTo ( wx ( my_gate[i].boundaries[k] ),   wy ( gate_height ) ) ;
            piorko.drawPath(skok);

            piorko.setPen ( QPen ( black_white ? Qt::black : Qt::darkYellow, 7, Qt::SolidLine ) );
            //             piorko.lineTo ( wx ( my_gate[i].boundaries[k+1] ),   wy ( gate_height ) ) ;

            QPainterPath skok2;
            skok2.moveTo ( wx ( my_gate[i].boundaries[k] ),   wy ( gate_height ) ) ;
            skok2.lineTo ( wx ( my_gate[i].boundaries[k+1] ),   wy ( gate_height ) ) ;

            piorko.drawPath(skok2);

            

            piorko.setPen ( QPen ( black_white ? Qt::black : Qt::darkYellow, 2, Qt::DashDotDotLine ) );
            //             piorko.moveTo ( wx ( my_gate[i].boundaries[k+1] ),   wy ( minY ) ); // right side
            //             piorko.lineTo ( wx ( my_gate[i].boundaries[k+1] ),   wy ( gate_height ) ) ;

            QPainterPath skok3;
            skok3.moveTo ( wx ( my_gate[i].boundaries[k+1] ),   wy ( minY ) ); // right side
            skok3.lineTo ( wx ( my_gate[i].boundaries[k+1] ),   wy ( gate_height ) ) ;

            piorko.drawPath(skok3);

            // opis

            piorko.drawText ( wx ( my_gate[i].boundaries[k] ),   wy ( gate_height )-5,
                            my_gate[i].screen_name.c_str() );
        }
    }

}
//******************************************************************************
void box_of_spectrum::draw_pinup_notices(QPainter & piorko, int fontsize)
{
    // The info about the gates required (dark yellow)
    piorko.setPen ( QPen ( black_white ? Qt::black : Qt::darkYellow, 2, Qt::SolidLine ) ); // preparing the pen

    QFont sansFont ( "Sans", fontsize );
    piorko.setFont ( sansFont );


    // calculating the position of notice
    double x_notice = minX + 0.03 * ( maxX- minX ) ;
    double y_notice = maxY - ( 0.1* ( maxY - minY ) ) ;
    if ( parent->give_flag_log_scale() )
    {
        y_notice = pow ( 10, y_notice );
    }


    parent->nalepka_notice.set_xy ( x_notice,  y_notice );


    sansFont.setPixelSize ( (parent->nalepka_notice.give_info().size()) > 90 ? // for very long notice
                              0.6 * fontsize : fontsize );

    piorko.setFont ( sansFont );
    parent->nalepka_notice.draw ( this, &piorko, parent->give_flag_log_scale() );

    // -----------
    if ( krot != 1 ) // this is a rebining factor
    {
        piorko.setPen ( QPen ( black_white ? Qt::black : Qt::red, 2, Qt::SolidLine ) ); // preparing the pen
        QString inf = QString ( "1 bin = %1 channels" ).arg ( krot ) ;
        parent->rebining_notice.set_info ( inf.toStdString() ) ;
        // calculating the position of notice
        x_notice = minX + 0.8 * ( maxX- minX ) ;
        y_notice = maxY - ( 0.15* ( maxY - minY ) ) ;
        if ( parent->give_flag_log_scale() )
        {
            y_notice = pow ( 10, y_notice );
        }
        parent->rebining_notice.set_xy ( x_notice,  y_notice );
        //  sansFont.setPixelSize(12);
        //  piorko.setFont(sansFont);
        parent->rebining_notice.draw ( this, &piorko, parent->give_flag_log_scale() );
    }

    //----------- other user definied notices -----------------
    piorko.setPen ( QPen ( black_white ? Qt::black : Qt::cyan, 2, Qt::SolidLine ) ); // preparing the pen

    sansFont.setPixelSize ( fontsize );
    piorko.setFont ( sansFont );

    for ( unsigned int i = 0 ; i < parent->nalepka.size() ; i ++ ) // drawing all gates
    {
        parent->nalepka[i].draw ( this, &piorko, parent->give_flag_log_scale() );
    }
}
//******************************************************************************
//******************************************************************************
//******************************************************************************

void box_of_spectrum::set_spectrum_pointer ( vector<int> * ptr, spectrum_descr * d )
{
    spectrum_ptr = ptr ;
    specif = d ;
}
//***********************************************************************
void   box_of_spectrum::take_position_from_mouse() // [[deprecated]]
{
    // we make it easer: just take the last mouse position
    cout << __PRETTY_FUNCTION__ << " is deprecated " << endl;

    x_mouse_click =real_x;
    y_mouse_click = real_y;

    // cout << "last mouse positon is = " << x_mouse_click
    //      << ", y = " << y_mouse_click
    //      << " at spectrum name " << real_spectrum_name
    //      << endl;

}

//***********************************************************************
void   box_of_spectrum::mousePressEvent ( QMouseEvent * e )
{
    // we set the values
    COTO;
    x_mouse_click = typ_x ( pix2worX ( e->
#if  (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
                                   position().
#endif
                                   x() ) );
    y_mouse_click = int ( pix2worY ( e->
#if  (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
                                 position().
#endif
                                 y() ) ) ;
    pressed_button_state = e->button() ;
    COTO;
       // cout << "started mousepress in  box_spectrum" << endl;
       //    cout << "in mousePressEvent,   Pixl=" << e->x()
       //   << "  means channel =" <<  pix2worX(e->x() )
       //   << endl ;

    //       if(pressed_button_state == Qt::RightButton)
    //       {

        //           cout << " right mouse button " << endl;
        //           QMenu *menu = new QMenu;
        //              menu->addAction(tr("Test Item"), this, SLOT(test_slot()));
        //              menu->exec(mapToGlobal(QPoint(e->x() ,e->y()   )));

    //              delete menu;




    //       QPopupMenu *contekst_Menu = new QPopupMenu;
    //
    //       contekst_Menu->insertItem( "Log/Linear",  parent, SLOT(expand() ), CTRL+Key_N );
    //       contekst_Menu->insertItem( "set Background Marker", parent, SLOT(set_backgr_marker() ),    CTRL+Key_O );
    //
    //       contekst_Menu->  exec(mapToGlobal(QPoint(e->x() ,e->y()   )) );
    //       }
    //       else

    //warning: at first event will be normal, one click press mouse.
    // then immediately it will come here once more
COTO;
    if ( e->type() ==QEvent::MouseButtonDblClick ) // double click
    {
        e->accept () ;
        flag_was_mouse_click = false;
        // cout << "double click in box_spectrum" << endl;
        //COTO;
        parent->expand_double_click ( x_mouse_click, y_mouse_click, ( e->modifiers ()  & Qt::ShiftModifier ) ) ;
        // cout << "Afer expand function " << endl ;
        //COTO;

    }
    if(e->button() & Qt::RightButton)
    {
#if  (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
        auto pos = e->position(). toPoint();

        // cout << "Myszkowa Pozycja  scene x = "  <<  p.x()
        //      << endl;
#else
        auto pos = e->pos();
#endif
        // cout << "Myszkowa Pozycja x = " << pos.x() << ", y = " << pos.y()              << endl;
        parent->show_context_1d_menu(pos);

    }
    else
    {
        COTO;
        // cout << "single click in box_spectrum" << endl;

        flag_was_mouse_click = true ;
        e->ignore ();   // so it will go to the parent
        COTO;
    }
    //COTO;
    update();
    // cout << "finished mousepress in  box_spectrum" << endl;
}
//***********************************************************************
void box_of_spectrum::force_new_pixmap ( bool b )
{
    make_new_pixmap = b ;
    //     setPaletteBackgroundColor ( parent->give_flag_black_white() ?
    //                                 Qt::white
    //                                 :
    //                                 QColor ( 0, 85,0 ) ); // dark green}
    QColor color =  (parent->give_flag_black_white() ?
                        Qt::white
                                                    :
                        background_colour_of_1D_spectrum);  // dark green
    QPalette palette;
    palette.setColor(backgroundRole(), color);
    setPalette(palette);
    parent->flag_repaint_spectrum_box = 1;
    parent->flag_repaint_counts_box = 1;
    parent->flag_repaint_channels_box = 1;
    parent->flag_repaint_bscale_box= false;
    parent->update();

}
//**************************************************************
//**************************************************************
void box_of_spectrum::set_overlay_mode ( bool b )
{
    overlay_mode = b ;
}
//***************************************************************
void box_of_spectrum::mouseMoveEvent ( QMouseEvent* e )
{
    {
        // showing the position in the "Tip" area
        // double
        real_x = pix2worX ( e->pos().x() );
        // double
        real_y = pix2worY ( e->pos().y() );
        real_spectrum_name = parent->give_spectrum_name().c_str() ;
        //int contents =    parent->give_value_of_pixel ( real_x,real_y );

        if(parent->give_flag_log_scale() )
        {
            real_y = pow(10, real_y);
        }
        //         if ( contents >=0 )
        {
            //             QString wydruk ;
            //             wydruk=
            appl_form_ptr->send_to_statusbar(
                QString ( "Spectrum  X: %1  Y: %2     %3" ).arg ( real_x, 10 )
                    .arg ( real_y, 10 )
                    .arg( parent->give_spectrum_name().c_str()).toStdString(),
                10*1000 ); //  seconds on the screen
        }

    }
}
