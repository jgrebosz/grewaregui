#include "tplate_spectrum.h"
#include "box_of_counts.h"
#include <qstring.h>
#include <math.h>
#include "spectrum_2D.h"

#include "appl_form.h"
extern appl_form  *appl_form_ptr;
//*******************************************************************
box_of_counts::box_of_counts(QWidget *a_parent, bool flag_rainbow, string name)
    :
      QWidget(a_parent) , swiatek()
{
    setWindowTitle(name.c_str() );

    parent = dynamic_cast<Tplate_spectrum*>(a_parent) ;

    flag_draw_rainbow_scale = flag_rainbow ;

    //    only if less than a full range
    setCursor ( QCursor (Qt::SizeVerCursor/* OpenHandCursor*/ ) ) ;
    setMouseTracking ( true );

}
//********************************************************************
/** really drawing function */
void box_of_counts::draw_all(QPainter * piorko)
{

//	static int counter;
//	cout << __PRETTY_FUNCTION__ << " update box_counts  " << (counter++) << endl;

    static int level = 0 ;
    Tplate_spectrum *p = dynamic_cast<Tplate_spectrum*>(parent); // for
    if(!p->give_flag_draw_scales()) return;



    if(level > 0)
    {
        cout << "Level = " << level << ", so ignoring box_counts::draw_all"<< endl;
        return;
    }

    level++;


    // drawing a sunken frame --------------------------
    QRect r = rect() ;

    piorko->setPen(Qt::darkGray );
    piorko->drawLine(r.bottomLeft(), r.topLeft() );
    piorko->drawLine(r.topLeft(), r.topRight() );
    piorko->setPen(Qt::white );
    piorko->drawLine(r.topRight(), r.bottomRight() );
    piorko->drawLine(r.bottomRight(), r.bottomLeft() );


    // drawing a scale-----------------------------

    piorko->setPen(Qt::black);   // prepering the pen


    //   spectrum_widget *p = dynamic_cast<spectrum_widget*>(parent); // for log

    // preparing the coordinates
    ustaw_swiat(r.left(), r.top(), r.right(), r.bottom(),
                0,
                maxY,
                100,
                minY);

    //    cout << "box of counts:  swiat   minY= " << minY << " maxY= " << maxY
    //         << "   <--- r.top = " << r.top() << ", r.bottom() = " << r.bottom()
    //         << endl;

    //#####################################################

    if(flag_draw_rainbow_scale)
    {
        drawing_rainbow_background_scale(piorko);
    }
    //#####################################################





    if(parent->give_dimension() == 1 || flag_draw_rainbow_scale)
    {
        draw_for_1D(piorko);
    }else
    {
        draw_for_2D(piorko);
    }

    level--;
}
//***********************************************************************
//***********************************************************************
void box_of_counts::mouseMoveEvent ( QMouseEvent * event )
{

    if(flag_draw_rainbow_scale){
        setCursor ( QCursor (Qt::ForbiddenCursor ) ) ;
        return ;
    }

    int now_y = event->y();


    appl_form_ptr->send_to_statusbar(
                "Hint: use a mouse wheel to expand spectrum,  click and drag the scale to move a spectrum,",  10*1000   ); //  seconds on the screen


    if(event->buttons() != Qt::LeftButton) return;


    setCursor ( QCursor (Qt::SizeVerCursor) ) ;
	int moved_pixels = last_dragged_y - now_y;
	if(moved_pixels)
    {
		 double diff =   pix2worY(last_dragged_y) - pix2worY(now_y) ;

        //   // if it was moved really
//			cout<< "box of counts, mouse move  event moved by "
//				<< - (now_y - last_dragged_y) << " pixels "
//				<< " in word coordinates diff is  = " << diff
//				<< endl;

        last_dragged_y = now_y ;

        typ_x  min_ch, max_co, max_ch, min_co;
        spectrum_descr sd;
        parent->give_parameters(&min_ch, &max_co, & max_ch, & min_co, &sd);

		bool flag_impossible_more = false;

		if( (max_co + diff > sd.end_y)          // if too much to the right
                ||
		 (min_co + diff < sd.beg_y) )    //    too much to the left
		{
			// if( parent->give_dimension() == 2)  // do not do int for matrices?
				flag_impossible_more = true;   // always block going outside boundaries
		}

//		cout << "Imposible--> max_co + diff = " << (max_co + diff)
//			 << " > sd.end_y = " << sd.end_y
//			 << " lub < sd.beg_y " << sd.beg_y << endl;

		if(flag_impossible_more )
		{
//			cout << "max_co + diff = " << (max_co + diff)
//				 << " > sd.end_y = " << sd.end_y
//				 << " lub < sd.beg_y " << sd.beg_y << endl;


            //forbidden coursor
            setCursor ( QCursor (Qt::ForbiddenCursor ) ) ;
            appl_form_ptr->send_to_statusbar(
                        "Dragging more in this direction - is not possible (OUT OF LIMITS of the spectrum)",  5*1000   ); //  seconds on the screen

        }
        else{
            setCursor ( QCursor (Qt::OpenHandCursor ) ) ;
            parent->set_parameters(min_ch, max_co+diff, max_ch, min_co+diff);
             appl_form_ptr->if_needed_apply_to_other_spectra();

        }
    }else
    {
        //       cout << "no move now_y ="   << now_y << endl;
    }
	//parent ->flag_repaint_spectrum_box = true;
}
//******************************************************************************************
void box_of_counts:: mousePressEvent ( QMouseEvent * event )
{
    // remembering the pressing point

    last_dragged_y= event->y() ;
    //    cout << "Pressing at point " <<  last_moved_x
    //         << " in word coordinates it is channel = "
    //         << pix2worX(last_moved_x)

    //         << "so it was moved by "
    //         << endl;
 setCursor ( QCursor (Qt::OpenHandCursor) ) ;
}
//*****************************************************************************
void box_of_counts:: mouseReleaseEvent( QMouseEvent *  /* e */ )
{
    setCursor ( QCursor (Qt::SizeVerCursor ) ) ;
}


//******************************************************************************************
void box_of_counts:: wheelEvent ( QWheelEvent * e )   // expanding spectrum around coursor
{

    if(parent->give_dimension() == 1)    // Normal spectrum 1D ========================
    {
			//  cout << "this is a Counts wheel event  in spectrum 1D::" << __func__ << " , delta = "<<  e->delta() << endl ;

//        if ( e->delta() < 0 )
       if ( e->angleDelta().y() < 0 )
        {
            //         scaleY_by_05();
            parent->scaleY_by_factor ( 0.9 );
             appl_form_ptr->if_needed_apply_to_other_spectra();
        }
        else
        {
            //       scaleY_by_2();
            parent->scaleY_by_factor ( 1.2 );
             appl_form_ptr->if_needed_apply_to_other_spectra();
        }

    }else{ // dimension 2     MATRIX ===========================================
        //       cout << "this is a wheel event  in spectrum 2D::" << __func__ << " , delta = "<<  e->delta() << endl ;

        if(flag_draw_rainbow_scale){
            setCursor ( QCursor (Qt::ForbiddenCursor ) ) ;
            return ;
        }

        typ_x   y=      pix2worY(
            #if  (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
                e->position().y()
            #else
                 e->y()
                // e->position().y()
            #endif



                    );
        int delta = e->angleDelta().y();

        //    cout << "wheelEvent in countas, pt " << y
        //         << ", delta= " << delta
        //         << endl ;

        double factor = 1.1;
        if(delta < 0) factor = 1/ factor;

        typ_x  min_ch, max_co, max_ch, min_co;
        spectrum_descr sd;

        parent->give_parameters(&min_ch, &max_co, & max_ch, & min_co, &sd);

        //     cout << "Before wheel (" <<min_co  << " : " << max_co << ")" << ",    ( x = " << min_ch << ", " << max_ch << ")" << endl;

        typ_x kon_new = (max_co - y) * factor  + y;
        typ_x pocz_new = y - ( (y- min_co) * factor);
        // if too much to the right

        if( (kon_new > sd.end_y)  || (pocz_new < sd.beg_y))
        {
            //forbidden coursor
            //setCursor (  QCursor (Qt::ForbiddenCursor ) ) ;

            // preparing nice text --------------------
            string andtxt;
            if((kon_new > sd.end_y)  && (pocz_new < sd.beg_y) )
            {
                andtxt = " and ";
            }

            string txt = "Warning: unzooming " ;
            if(kon_new > sd.end_y)
            {
                kon_new = sd.end_y;
                txt += "more UP ";
            }
            if(pocz_new < sd.beg_y)
            {
                pocz_new = sd.beg_y;

                txt += andtxt ;
                txt += " more DOWN ";
            }
            appl_form_ptr->send_to_statusbar(
                        txt +=" - is not possible",  5*1000   ); //  seconds on the screen
        }
        else{
            setCursor ( QCursor (Qt::SizeVerCursor/*OpenHandCursor*/ ) ) ;
            appl_form_ptr->send_to_statusbar("",  1*1000   ); //  seconds on the screen
        }
        //    cout << "wheel wants (pocz " <<pocz_new  << " : kon =" << kon_new << ")" << ",    ( x = " << min_ch << ", " << max_ch << ")" << endl;
        parent->set_parameters(min_ch, kon_new, max_ch, pocz_new);
         appl_form_ptr->if_needed_apply_to_other_spectra();

    } // endif 2D


}
//*******************************************************************************************************************
void box_of_counts::drawing_rainbow_background_scale(QPainter * piorko)
{
    int how_many_colors = box_of_matrix::paleta.size();
    //    cout << "how_many_colors = " << how_many_colors << endl;
    if(how_many_colors <= 0) return;




    if(parent->give_flag_log_scale())  {  // black color below 1 for log scale

        piorko->setBrush (
                    (parent->give_flag_black_white()) ?
                        QColor(255, 255,255) : QColor(0,0,0) );

        //         cout << " wy(log10(0) =" << wy(0)
        //              << "Wys = " << wy(log10(1))
        //                 << endl;

        piorko->fillRect (
                    2,
                    wy(0),

                    92, // szer
                    wy(log10(1)), //100 , // rely(40), // rely(log10(0) - log10(-1)) , // wys
                    piorko->brush() ) ;
    }




    double wys = 2*rely(maxY /how_many_colors);
    if(wys< 1) wys = 1;

    //    cout << "wys="  << wys  << endl;

    for(int color = 0 ; color < how_many_colors; ++color)
    {
        // int color = i ; // 50000;

        //if ( color < 0 ) color = 1 ; // to draw even weakest points !

        QColor  paleta_helper =box_of_matrix::paleta[min ( color, 255 ) ]  ;
        piorko->setBrush ( paleta_helper ) ;
        double posY = 1.0 * color * maxY/how_many_colors;

        piorko->fillRect (
                    2,
                    // flag_log? wy(log10(posY)) :
                    wy(posY), // 20 , // row_calculated,
                    92, // szer
                    wys ,   // wys
                    piorko->brush() ) ;
    }


}
//*******************************************************************************************************************
void box_of_counts::draw_for_1D(QPainter *piorko)
{
#define scout if(flag_draw_rainbow_scale) cout

    QRect r = rect() ;

    // adjusting the size of the font to the size of the widget
    QFont sansFont("Helvetica [Cronyx]");

    int rozm =(r.right()- r.left()) /2.1  ;

    // cout << "ROzm = " << rozm << endl;
    if((rozm-1) >0) sansFont.setPixelSize(rozm-1);
    else sansFont.setPixelSize(1);

    sansFont.setStyleHint( QFont::SansSerif );
    piorko->setFont(sansFont);


    //*****************
    // calcutaling the range for numbering

    typ_x pocz =   minY ; // minCounts ;
    typ_x kon =   maxY ; // maxCounts ;

    typ_x roznica = fabs(kon - pocz) ;

    if(roznica < 0.0001){
        cout << "COUNTS pocz = " << pocz
             << ", kon = " << kon << ", roznica " << roznica << endl;

        return;
    }
    int ilem = (int) log10(roznica)  ;

    int numerek = (int) pow(10.0, (double)ilem) ;

    double kopia_numerka = (double) numerek ;
    //		cout << "Numerek will be written every " << numerek << endl ;

    int kreska = numerek/10 ;


    //		cout << "mark will be every " << kreska << endl ;

    // if we had few numbers (eg. less than 5, then we will
    // write 2 times more often
    // so instead every 100 they will be every 50

    if(roznica < (3 * numerek))
    {
        if(numerek/2 > 0)
        {
            numerek/=2 ;
        }
        //.			cout << "more dense numbers " << numerek << endl ;
    }

    // NOWE
    if(numerek == 0) numerek = 1;

    int dodatek = 1 ;

    // for a log scale make a bacground colour - yellow ?
    //     setPaletteBackgroundColor (p->give_flag_log_scale() ?
    //                                QColor (220, 220, 238)  // was 210
    //                                : QColor(238, 238,230) );


    QColor color =  (parent->give_flag_log_scale() ?
                         QColor (220, 220, 238)  // was 210
                       : QColor(238, 238,230) );
    QPalette palette;
    palette.setColor(backgroundRole(), color);
    setPalette(palette);

    if(parent->give_flag_log_scale() == false

            //                ||             // for 2D spectrum counts scale must be always linear
            //                ptr != 0  // if this is not null
            )
    {
        //        if(flag_draw_rainbow_scale){
        //            cout << "Not a log scale!" << endl;
        //        }

        int nr_wlasnie_rysowanej_dzialki = 0;
        QPainterPath skok;
        //----------------------- loop for drawing the linear scale ----------------
        for(int i = (int) pocz ; i < kon ; i+=dodatek)
        {
            if(!(i%numerek))
            {
                //           piorko->moveTo(wx(95),   wy(i) );
                //QPainterPath skok;
                skok.moveTo(wx(95),   wy(i) );
                //piorko->drawPath(skok);

                if(i < pocz+ 0.8*(kon-pocz))
                {
                    piorko->drawText(wx(1), wy(i),
                                     QString("%1").arg(i/kopia_numerka, 4) ) ;
                }
                //           piorko->lineTo(wx(80),   wy(i) ) ;
                //QPainterPath skok2;
                skok.lineTo(wx(80),   wy(i) ) ;
                //piorko->drawPath(skok2);

            }

            if(kreska > 0)
            {
                if(!(nr_wlasnie_rysowanej_dzialki%kreska))
                {
                    QPainterPath skok2;
                    skok2.moveTo(wx(95),   wy(i) );
                    skok2.lineTo(wx(90),   wy(i) ) ;
                    piorko->drawPath(skok2);


                    // jesli znalezlismy pierwsze miejsce kreseczki,
                    // to dalej nie musimy juz robic petli z przyrostem 1
                    // tylko szybciej, z przyrostem "kreseczka"
                    dodatek=kreska ;
                }
            }

        }
        // -------------------------- end of drawing loop -------------------
        piorko->drawPath(skok);

        //===============================

        // writing the power
        piorko->setPen(Qt::blue);   // preparing the pen

        int posY = 20; //wy(worldBottom + (0.9*(worldTop-worldBottom)  ) );


        int pi_rozm =  (int)(0.4 * rozm);
        piorko->drawText(wx(5),   posY+pi_rozm , "*10") ;

        //        cout << "pi_rozm=" << pi_rozm << flush ;
        if(pi_rozm > 0)
        {
            sansFont.setPixelSize(pi_rozm);
            // here should be piorko->setFont
        }
        piorko->drawText(wx(75),  posY
                         , QString("%1").arg(ilem) ) ;



        // writing
        {
            int min_z = parent->give_minZthreshold() ;
            if(min_z > 1)
            {
                piorko->setBrush(QColor(50,50,50));   // preparing the pen

                QPainterPath skok3;
                skok3.moveTo(wx(10),   wy(min_z) );
                skok3.lineTo(wx(80),   wy(min_z) ) ;
                skok3.lineTo(wx(80),   wy(0) ) ;
                skok3.lineTo(wx(10),   wy(0) ) ;


                piorko->drawPath(skok3);

                piorko->setPen(QColor(250,250,250));   // preparing the pen
                piorko->drawText(wx(5),  wy(min_z)
                                 , QString("%1").arg(min_z) ) ;
            }
        }









    }
    else   // if it must be in a LOG10 scale ##########################
    {


        //        if(flag_draw_rainbow_scale){
        //            string n = this->windowTitle().toStdString();
        //            cout << "Rainbow Log scale for " << n << endl;
        //        }

        // preparing the coordinates
        ustaw_swiat(r.left(), r.top(), r.right(), r.bottom(),
                    0,
                    maxY,
                    100,
                    minY);


        piorko->setPen(Qt::black);

        //        scout << "zakres do opisania jest min= " <<  pocz
        //              << " a max = " << kon
        //              << " warning, pow(10, kon) = " << pow(10, kon) << endl ;

        pocz = (typ_x)pow(10, pocz);
        kon = (typ_x) pow(10, kon ) ;
        int kroczek = 1 ;
        //        scout << "after pow 10 is min= " <<  pocz
        //              << " a max = " << kon << endl ;
        if(kon < 999999999)
            for(int i = 0 ; i < kon ; i+=kroczek)
            {
                //cout << i << ") " << flush ;
                if(wy(log10((double)i)) < 0) continue;

                switch(i/kroczek)
                {
                    case 10:
                    {
                        kroczek *= 10 ;
                        if(log10((double)kroczek) > 9)
                        {
                            i = (int) kon ;
                            continue ;
                        }

                        //                    piorko->setPen(red);   // preparing the pen
                        piorko->setPen( QPen(Qt::black, 2, Qt::SolidLine) );


                        QPainterPath skok;
                        skok.moveTo(wx(95),   wy(log10((double)i)) );
                        skok.lineTo(wx(60),   wy(log10((double)i)) ) ;
                        piorko->drawPath(skok);

                        piorko->setPen( QPen(Qt::black, 1, Qt::SolidLine) );

                        //piorko->setPen(blue);   // preparing the pen
                        piorko->drawText(wx(1),   wy(log10((double)i)) ,
                                         QString(" %1").arg((int)log10((double)kroczek),0));

                        //                    scout << "case 10: napisanie '" << (int)log10((double)kroczek)
                        //                          << "'', na x = " << wx(1)
                        //                          << ", y= " << wy(log10((double)i))
                        //                          << " dla i =" << i
                        //                          << " kroczek = " << kroczek
                        //                          << endl;


                        piorko->setPen(Qt::black);   // preparing the pen
                    }
                    break ;

                    case 0:
                    case 1:
                        //piorko->setPen(blue);   // preparing the pen

                    {
                        QPainterPath skok;
                        skok.moveTo(wx(95),   wy(log10((double)i)) );
                        skok.lineTo(wx(70),   wy(log10((double)i)) ) ;
                        piorko->drawPath(skok);
                    }


                        //piorko->setPen(blue);   // preparing the pen
                        piorko->drawText(wx(1),   wy(log10((double)i)) ,
                                         QString(" %1").arg((int)log10((double)kroczek),0));

                        //                    scout << "0/1 napisanie '" << (int)log10((double)kroczek)
                        //                          << "'', na x = " << wx(1)
                        //                          << ", y= " << wy(log10((double)kroczek))
                        //                          << " dla i =" << i
                        //                          << " kroczek = " << kroczek
                        //                          << endl;


                        //QString("!%1=").arg(kroczek, 0) ) ;
                        //piorko->setPen(black);   // preparing the pen
                    break ;

                        //			case 2:
                        //			case 5:
                        //                    piorko->setPen(green);   // preparing the pen
                        //				//if(log10(kon - pocz) > 3) break ; // gdy za gesto
                        //				//cout << "kreska i napisanie " << (int) (i/kroczek) << endl;
                        //				piorko->moveTo(wx(95),   wy(log10(i)) );
                        //				piorko->lineTo(wx(85),   wy(log10(i)) ) ;
                        //				//piorko->drawText(wx(1),   wy(log10(i)) , QString("%1").arg(i/kroczek, 4) ) ;
                        //				break ;


                    default:

                        if(wy(log10((double)i)) >= 0)
                        {
                            //                        scout << "default: - naryskownie kreseczki, bo i =" << i << "w miejscu x="
                            //                             << wx(95)
                            //                             << ", y="
                            //                             << wy(log10((double)i))
                            //                             << " dla i =" << i
                            //                             << " kroczek = " << kroczek
                            //                             << endl ;

                            QPainterPath skok;
                            skok.moveTo(wx(95),   wy(log10((double)i)) );
                            skok.lineTo(wx(85),   wy(log10((double)i)) ) ;
                            piorko->drawPath(skok);
                        }

                    break;
                }
            }

        // writing the power
        piorko->setPen(Qt::blue);   // preparing the pen

        int posY = 5 ; // 20 ; // wy(worldBottom + (0.92*(worldTop-worldBottom)  ));
        int t = (int)(0.4 * rozm) ;
        if(t < 1) t = 1;

        piorko->drawText(wx(3),
                         posY,
                         100, 100,
                         Qt::AlignTop,
                         "log"

                         );


        piorko->drawText(wx(5), posY + 4*t                          ,
                         100, 100,
                         Qt::AlignTop,
                         "*10") ;

        sansFont.setPixelSize(t > 0 ? t : 1);
        piorko->drawText(wx(75),
                         posY + 3*t,
                         100, 100,
                         Qt::AlignTop,
                         "n");



        // writing
        {
            int min_z = parent->give_minZthreshold() ;
            if(min_z > 1)
            {
                piorko->setBrush(QColor(50,50,50));   // preparing the pen

                QPainterPath skok;
                skok.moveTo(wx(10),  wy(log10((double)min_z)));
                skok.lineTo(wx(80),  wy(log10((double)min_z)));
                skok.lineTo(wx(80),   wy(0) ) ;
                skok.lineTo(wx(10),   wy(0) ) ;


                piorko->drawPath(skok);

                piorko->setPen(QColor(250,250,250));   // preparing the pen
                piorko->drawText(wx(5),  wy(log10((double)min_z) )
                                 , QString("%1").arg(min_z) ) ;
            }
        }



    }  // end log scale
}
//*******************************************************************************************************************
void box_of_counts::draw_for_2D(QPainter *piorko)
{

    QRect r = rect() ;
    // adjusting the size of the font to the size of the widget
    QFont sansFont("Helvetica [Cronyx]");

    int rozm =(r.right()- r.left()) /2  ;

    // cout << "ROzm = " << rozm << endl;
    if((rozm-1) >0) sansFont.setPixelSize(rozm-1);
    else sansFont.setPixelSize(1);

    sansFont.setStyleHint( QFont::SansSerif );
    piorko->setFont(sansFont);

    //*****************
    // calcutaling the range //  obliczenie zakresu do opisania

    //     cout << "Test point 1"   << endl ;

    typ_x pocz = (typ_x)worldBottom ; // min x ;
    typ_x kon =  (typ_x)worldTop; //max x ;

    typ_x roznica = fabs(kon - pocz) ;

    //        cout
    //                << " pocz = " << pocz  <<  ", kon= " << kon
    //                << ",    (roznica = " << roznica << ")"
    //                << endl ;



    double dzialka_skali = 0 ;
    double pierwsza_dzialka = 0 ;
    int wynik  ;
    if(roznica >=1)
    {
        //cout << "Zaczynamy prace z roznica = " << roznica << endl ;
        int rezultat = (int) roznica ;
        wynik = 0 ;
        while(1)
        {
            rezultat /= 10 ;
            if (rezultat == 0) break ;
            wynik++ ;
            //	  	  cout 	<< "rezultat " << rezultat
            //	  		  << ", wynik = " << wynik
            //	  		  << endl ;
        }
        //       cout << "ostatecznie " << wynik << "czyli 10**n = "
        //             << pow(10.0, (double) wynik) << endl ;
        wynik-- ;
        // int ile_miejsc = wynik ;
        //cout << "miejsc skali = " << ile_miejsc << endl;

        pierwsza_dzialka = (int(minY / dzialka_skali)  * dzialka_skali);
        //      cout << "Pierwsza dzialka = " << pierwsza_dzialka  << endl ;
    }
    else
    {
        //      cout << "roznica < 1 " << endl ;
        //      cout << "Zaczynamy prace z roznica = " << roznica << endl ;
        double rezultat = fabs(roznica) ;
        wynik = -1;
        while(1)
        {
            rezultat *= 10 ;
            if (rezultat >=1) break ;
            wynik-- ;
            //	  cout 	<< "rezultat " << rezultat
            //       << ", wynik = " << wynik  << endl ;
        }
        //      cout << "ostatecznie " << wynik << "czyli 10**n = "
        //       << pow(10.0, (double) wynik) << endl ;

    }
    dzialka_skali = pow(10.0, (double) wynik);
    double ile_dzialek = roznica / dzialka_skali ;
    if(ile_dzialek > 100) dzialka_skali *= 10 ;  // was > 10


    int co_ile_dzialek_numerek = 1 ; // was 2 ;
    pierwsza_dzialka = (int(minY / dzialka_skali)  *
                        dzialka_skali) - dzialka_skali;

    //cout << "dzialka skali = " << dzialka_skali
    // <<  " pierwsza dzialka = " << pierwsza_dzialka << endl ;

    if(roznica / dzialka_skali < 10  ) // 5 )   // czyli jesli kreseczek mialoby
        // byc mniej niz... n
    {
        //cout << "zageszczamy skale, bylo co "  << dzialka_skali   ;
        dzialka_skali/=4 ;
        co_ile_dzialek_numerek *= 2 ;

        //cout << ", teraz jest " << dzialka_skali  << endl   ;
    }


    //		// mozliwe ze jest to bardzo waski obszar ------------------
    //		if  ( rely(roznica) < 400)
    //    {
    //      // cout << "Czy zagescic roznica / dzialka_skali =	"
    //			// << (roznica / dzialka_skali) << endl ;
    //
    //      if( roznica / dzialka_skali > 2)
    //			{
    //				dzialka_skali *= 2 ;   // was 2
    //				// co_ile_dzialek_numerek *= 2 ;
    //
    //				//cout << "Rozrzedzamy numerki, dzialka " << dzialka_skali << endl ;
    //			}
    //    }


    //---------------------
    QFontMetrics  fm = piorko->fontMetrics ();
    int height_of_text = (fm.height());

    // we do not want that the numbers overlap vertically
    int how_many_numbers = 0 ;
    for(typ_x i = pierwsza_dzialka  ; i <= maxY ;
        i+= dzialka_skali,
        how_many_numbers++  ) {};

    if  ( rely(roznica) <  height_of_text * how_many_numbers)
    {
        //cout << "The numbers would overlap " << endl;
        co_ile_dzialek_numerek *= 4 ;
    }

    // tu bedzie skok po wartosciach niecalkowitych, ale swiatowych
    int nr_wlasnie_rysowanej_dzialki  = 0 ;

    for(typ_x i = pierwsza_dzialka  ; i <= maxY ;
        i+= dzialka_skali,
        nr_wlasnie_rysowanej_dzialki ++  )
    {

        // zwykla kreska skali
        {
            //         piorko->moveTo(wx(100),   wy(i) );
            //         if(  !(nr_wlasnie_rysowanej_dzialki   % co_ile_dzialek_numerek  )  )
            //           piorko->lineTo(wx(90),   wy(i) ) ;    // krotsza kreseczka
            //         else
            //           piorko->lineTo(wx(95),   wy(i) ) ;     // dluzsza kreseczka

            QPainterPath skok;
            skok.moveTo(wx(100),   wy(i) );
            if(  !(nr_wlasnie_rysowanej_dzialki   % co_ile_dzialek_numerek  )  )
                skok.lineTo(wx(90),   wy(i) ) ;    // krotsza kreseczka
            else
                skok.lineTo(wx(95),   wy(i) ) ;     // dluzsza kreseczka

            piorko->drawPath(skok);
        }

        //COTO ;
        // czy moze tu ma byc numerek
        if(!(nr_wlasnie_rysowanej_dzialki   % (co_ile_dzialek_numerek)  ))
        {
            //piorko->moveTo(wx(i),   wy(100) );
            double wartosc = i ;
            if(fabs(wartosc) < 0.000001) wartosc = 0 ;

            QString napis = QString(" %1").arg(wartosc) ;
            int pixels_for_text = fm.horizontalAdvance(napis) ;
            if(pixels_for_text > relx(100) )
                for(int m = 0  ; pixels_for_text > relx(100) ; m++)
                {
                    if(rozm-m < 1) break ;
                    //				  cout << "pixels_for_text was ="<< pixels_for_text
                    //				  << " what is bigger than available relx(100) = " << relx(100)
                    //						<< "reducing font to " << rozm-m << endl;
                    sansFont.setPixelSize(rozm-m);
                    piorko->setFont(sansFont);

                    QFontMetrics  fm2 = piorko->fontMetrics ();
                    pixels_for_text = fm2.horizontalAdvance(napis) ;
                    //	            cout << "Now pxft = " << pixels_for_text << endl;
                }

            int half_height = (fm.height()/2);

            // for zero, leading the channels we want it, even non symetric
            if(i == pierwsza_dzialka) half_height = -1 ;

            //COTO ;

            //				cout << " wy(i) " <<  wy(i)
            //					<< " half_height " << half_height
            //					<< ", wy(maxY) "  << wy(maxY)
            //					<< ", wy(minY) "  << wy(minY)
            //					<< endl;

            if(
                    wy(i) + half_height  < wy(minY)     // OK, bec pixels are increasing down screen
                    &&
                    wy(i) - half_height > wy(maxY)   // OK   -"-
                    )
            {
                piorko->drawText(wx(5) , //   ,
                                 wy(i),
                                 napis );
            }

            //piorko->drawRect (wx(2),   wy(i), 5, wy(i) ) ;
        }
        //COTO ;
    }
}
//*****************************************************************************************************

