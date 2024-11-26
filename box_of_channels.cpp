#include "box_of_channels.h"
#include <qstring.h>
#include <math.h>

#include "appl_form.h"
extern appl_form  *appl_form_ptr;

#include "spectrum_1D.h"
#include "spectrum_2D.h"
//*******************************************************************

box_of_channels::box_of_channels(QWidget
								 //spectrum_widget
								 *a_parent, string name):
	QWidget(a_parent) , swiatek()
{
	setWindowTitle(name.c_str() );
	// remembering the factors
	//    f_left  = factor_left;
	//    f_top = factor_top;
	//    f_right = factor_right;
	//    f_bottom = factor_bottom;

	parent = dynamic_cast<Tplate_spectrum*>(a_parent) ;

	minX = 0;
	maxX = 4095;


	//    only if less than a full range
	setCursor ( QCursor (Qt::SizeHorCursor ) ) ;
	setMouseTracking ( true );
}
//***********************************************************************
// called from paintEvent (defined in a header file)
void box_of_channels::draw_all( QPainter * piorko )
{

	//     cout << "box_of_channels::draw_all " << int(time(NULL))  << endl;

//	static int counter;
//	cout << __PRETTY_FUNCTION__ << " update box_channels  " << (counter++) << endl;

	Tplate_spectrum *p = dynamic_cast<Tplate_spectrum*>(parent); // for
	if(!p->give_flag_draw_scales())
	{
		// cout << "rejceted scales" << endl;
		return;
	}

	//    if(f_left == f_right)
	//    {
	//        cout << "box channels rejected " << __LINE__ << endl;
	//        return;
	//    }
	//    if(f_top == f_bottom)
	//    {
	//        cout << "box channels rejected " << __LINE__ << endl;
	//        return;
	//    }
	//asking for a geometry of the parent widget
	//    QSize size_of_window = parent->size() ;


	//    cout << "size_of_window.height() = " << size_of_window.height() << endl;

	//    piorko->setPen(Qt::black );
	//    piorko->drawLine(QLineF(0, size_of_window.height(),  0, size_of_window.width()) );

	//  COTO ;
	//    setGeometry( (int) (f_left * size_of_window.width()),
	//                 (int) (f_top * size_of_window.height()),
	//                 (int) ((f_right-f_left) * size_of_window.width()),
	//                 (int) ((f_bottom-f_top) * size_of_window.height()));


	// drawing the frame only on the screen (not on paper)
	QRect box_rectangle = rect() ;

	//    cout
	//            << "r.bottomLeft().x= "<< r.bottomLeft().x()
	//            << ", y= " << r.bottomLeft().y()
	//            << ",   r.topLeft() x= "<< r.topLeft().x()
	//            << ", y()= " << r.topLeft().y()
	//            << endl ;

	//    piorko->setPen(Qt::black );
	//    piorko->drawLine(r.bottomLeft(), r.topRight() );

	// drawing a sunken frame --------------------------
	//    COTO;
	piorko->setPen(Qt::darkGray );
	piorko->drawLine(box_rectangle.bottomLeft(), box_rectangle.topLeft() );
	piorko->drawLine(box_rectangle.topLeft(), box_rectangle.topRight() );
	piorko->setPen(Qt::white );
	piorko->drawLine(box_rectangle.topRight(), box_rectangle.bottomRight() );
	piorko->drawLine(box_rectangle.bottomRight(), box_rectangle.bottomLeft() );

	//    COTO ;
	// drawing a scale-----------------------------

	//    cout << "in channels before world setting  x "
	//         <<   minX << "-"
	//           <<  maxX
	//            << endl;


	piorko->setPen(Qt::black);   // prepering the pen

	// preparing the coordinates
	ustaw_swiat(box_rectangle.left(), box_rectangle.top(), box_rectangle.right(), box_rectangle.bottom(),
				minX, 100, maxX, 0);

	//    cout << "in channels 0-100 is   r.top, r.bottom = "
	//         <<   r.top() << "-"
	//           <<  r.bottom()
	//            << endl ;


	// COTO ;
	QFont sansFont("Helvetica [Cronyx]");

	// adjusting the size of the font to the size of the widget

	int font_size_in_pixels =(int) (0.65 * (box_rectangle.bottom()- box_rectangle.top()));

	if(font_size_in_pixels >0)
		sansFont.setPixelSize(font_size_in_pixels);

	sansFont.setStyleHint( QFont::SansSerif );
	piorko->setFont(sansFont);


	//*****************
	// calcutaling the range //  obliczenie zakresu do opisania

	//     cout << "Test point 1"   << endl ;

	typ_x pocz = (typ_x)worldLeft ; // min x ;
	typ_x kon =  (typ_x)worldRight ; //max x ;

	typ_x roznica_kon_pocz = fabs(kon - pocz) ;

	//      cout
	//              << "CHANNELS pocz = " << pocz  <<  ", kon= " << kon
	//              << ", Roznica = " << roznica_kon_pocz
	//              << endl ;

	spectrum_descr sd = parent->give_specif();


	if(roznica_kon_pocz < 0.000001) return;

	double dzialka_skali = 0 ;
	double pierwsza_dzialka = 0 ;


	// ile cyfr na ekranie. Jesli > 1 to wartość dodatnia i okresla liczbę cyfr
	// jesli < 1 to wartośc ujemna i okresla ile cyfr ulamka dziesietnego


	int ilo_cyfrowa  = ilo_cyfrowe_liczby(roznica_kon_pocz);


	dzialka_skali = pow(10.0, (double) ilo_cyfrowa);
	//    double
	int ile_dzialek = roznica_kon_pocz / dzialka_skali ;
	if(ile_dzialek > 10)
	{
		dzialka_skali *= 10 ;
	}
	if(dzialka_skali == 0) dzialka_skali = 1;

	ile_dzialek = roznica_kon_pocz / dzialka_skali ;


	pierwsza_dzialka = (int(minX / dzialka_skali)  * dzialka_skali) - dzialka_skali;

	int co_ile_dzialek_numerek = 2 ; // was 2 ;
	int ile_numerkow =  ile_dzialek /co_ile_dzialek_numerek;
	if(ile_numerkow > 10)
	{
		co_ile_dzialek_numerek = ile_numerkow / 6  ;
		ile_numerkow =  ile_dzialek /co_ile_dzialek_numerek;
	}



	//      cout << "----- Wstepnie: "
	//           <<  " pierwsza_dzialka=" << pierwsza_dzialka
	//            << " dzialka_skali = " << dzialka_skali
	//            << ", ile_dzialek = " << ile_dzialek
	//            << ", co_ile_dzialek_numerek = " << co_ile_dzialek_numerek

	//            << ", ile_numerkow= " << ile_numerkow
	//            << endl;


	QString przykladowy_tekst;
	przykladowy_tekst = QString("%1").arg(pow(10, abs(ilo_cyfrowa)+1) );




	// Perhaps some corrections =============================
	if(roznica_kon_pocz / dzialka_skali < 5 )   // czyli jesli kreseczek mialoby
		// byc mniej niz... n
	{
		dzialka_skali/= 5; // 4 ;
		ile_dzialek = roznica_kon_pocz / dzialka_skali;

		ile_numerkow =  ile_dzialek /co_ile_dzialek_numerek;

		//co_ile_dzialek_numerek *= 2 ;
		//        cout << "Kor 1. kreseczek byloby mniej niz 5 to, zageszczamy skale, (terez dzialka skali co "
		//             << dzialka_skali  << ")" << ", ile_dzialek= " << ile_dzialek
		//             //<< ", co_ile_dzialek_numerek =" << co_ile_dzialek_numerek
		//             << endl   ;
	}

	// how wide  [pix] is text "0000" ?    ------------------

	QRectF numberBox=    piorko->boundingRect ( 0, 0, 100, 100, Qt:: AlignLeft, przykladowy_tekst  );
	//    cout << "size of tekst =" << przykladowy_tekst.toStdString() << " is = "
	//         << numberBox.width() << " pixels "
	//         << " while width of filels is = " << box_rectangle.width()
	//         << " pix. W sumie " << ile_numerkow  << " tekstow zajme = "
	//         << ile_numerkow * numberBox.width()
	//         << endl;


	// correction to the number of text
	double max_allowed_space = 0.6 * box_rectangle.width() ;
	double min_allowed_space = 0.3 * box_rectangle.width() ;


	for(int proba=0 ; proba < 10 ; proba++)
	{
		int summary_texts = ile_numerkow * numberBox.width();

		if( summary_texts < max_allowed_space   // ideal situation
				&&
				summary_texts > min_allowed_space)
		{
			if(dzialka_skali < sd.waga) dzialka_skali = sd.waga;
			break;      // OK
		}
		else if(summary_texts > max_allowed_space )   // too many texts
		{
			co_ile_dzialek_numerek +=1 ; //2;
			//            cout << "too MANY texts, so corrected " << endl;
		}
		else // if(summary_texts < min_allowed_space )   // to few texts
		{
			co_ile_dzialek_numerek -= 1; // 2;
			if( co_ile_dzialek_numerek < 1)
			{
				co_ile_dzialek_numerek  = 5;
				// a zageszczamy dzialki
				dzialka_skali/= 5; // 4 ;

				//              if(dzialka_skali < sd.waga) dzialka_skali = sd.waga;

				ile_dzialek = roznica_kon_pocz / dzialka_skali;
				ile_numerkow =  ile_dzialek /co_ile_dzialek_numerek;

			}
			//            cout << "too FEW texts, so corrected, now  co_ile_dzialek_numerek =" << co_ile_dzialek_numerek << endl;
		}


		ile_numerkow =  ile_dzialek /co_ile_dzialek_numerek;

		//        cout << " korekta co_ile_dzialek_numerek = " << co_ile_dzialek_numerek
		//             << ", ile_numerkow =" << ile_numerkow
		//             << " szer tekstow = " << (ile_numerkow * numberBox.width() )
		//             << endl;
	}

	// end of for proba

	//    if  ( relx(roznica_kon_pocz) < 400)
	//    {
	//        // cout << "Czy zagescic roznica / dzialka_skali =  "
	//        // << (roznica / dzialka_skali) << endl ;

	//        if( (roznica_kon_pocz / dzialka_skali) > 3)   // was 2
	//        {
	//            dzialka_skali *= 2 ;   // was 2

	//            //co_ile_dzialek_numerek = 3 ;
	//            cout << "Kor 2.  obszar < 400 pikseli, Rozrzedzamy numerki, dzialka " << dzialka_skali << endl ;
	//        }
	//    }

	// ile nam wyjdzie numerkow na ekranie. Jesli jeden, to za mało!
	ile_numerkow =  ile_dzialek /co_ile_dzialek_numerek;


	//      cout << "-----Ostatecznie: "
	//           <<  " pierwsza_dzialka=" << pierwsza_dzialka
	//            << " dzialka_skali = " << dzialka_skali
	//            << ", ile_dzialek = " << ile_dzialek
	//            << ", co_ile_dzialek_numerek = " << co_ile_dzialek_numerek
	//            << ", ile_numerkow= " << ile_numerkow
	//            << endl;


	//---------------------
	QFontMetrics  fm = piorko->fontMetrics ();

	// tu bedzie skok po wartosciach niecalkowitych, ale swiatowych

	int nr_wlasnie_rysowanej_dzialki  = 0 ;


	int ile_numerkow_wypisano = 0;

	ile_numerkow_wypisano = 0;
	QPainterPath skok;

	// loop over dzialka with a step=dzialka skali  untill we are out of range (maxX)
	for(typ_x i = pierwsza_dzialka  ; i <= maxX ;
		i+= dzialka_skali,
		nr_wlasnie_rysowanej_dzialki ++  )
	{
		if(nr_wlasnie_rysowanej_dzialki > 1000)
			break;   //???jurek
		// zwykla kreska skali

		skok.moveTo(wx(i),   wy(100) );
		if(  !(nr_wlasnie_rysowanej_dzialki   % co_ile_dzialek_numerek  )  )
			skok.lineTo(wx(i),   wy(80) ) ;    // krotsza kreseczka
		else
			skok.lineTo(wx(i),   wy(50) ) ;     // dluzsza kreseczka


		// COTO ;
		// czy moze tu ma byc numerek
		if(!(nr_wlasnie_rysowanej_dzialki   % (co_ile_dzialek_numerek)  ))
		{
			//piorko->moveTo(wx(i),   wy(100) );
			double wartosc = i ;
			if(fabs(wartosc) < 0.000001)
				wartosc = 0 ;

			QString napis = QString("%1").arg(wartosc) ;
            fm.horizontalAdvance(napis) ;

            int half_length = (fm.horizontalAdvance(napis)/2);

			// for zero, leading the channels we want it, even non symetric
			if(i == pierwsza_dzialka)
				half_length = -1 ;

			//COTO ;
			if(wx(i) - half_length  > wx(minX)
					&&
					wx(i) + half_length < wx(maxX) )
			{
				piorko->drawText(wx(i) - half_length  ,
								 wy(10),
								 napis );
				ile_numerkow_wypisano++;

			}

			piorko->drawRect (wx(i),   wy(100), 1, wy(60) ) ;
		}


	}
	piorko->drawPath(skok);
}
//************************************************************************
int box_of_channels::ilo_cyfrowe_liczby(typ_x roznica_kon_pocz)
{
	//       cout << __func__<< ", Zaczynamy prace z roznica = " << roznica_kon_pocz << endl ;

	//       int ic = (to_string((int)roznica_kon_pocz )).size();
	//       cout << "    cyfr = " << ic << endl;

	//return ic;
	int ilo_cyfrowa = 0 ;


	if(roznica_kon_pocz >=1)
	{
		//        cout << "roznica  >= 1 czyli " << roznica_kon_pocz << endl ;
		int rezultat = (int) roznica_kon_pocz ;
		ilo_cyfrowa = 0 ;
		while(1)
		{
			rezultat /= 10 ;
			if (rezultat == 0)
				break ;
			ilo_cyfrowa++ ;
			//        cout  << "rezultat " << rezultat
			//          << ", wynik = " << wynik
			//          << endl ;
		}
		//       cout << "ostatecznie " << wynik << "czyli 10**n = "
		//             << pow(10.0, (double) wynik) << endl ;
		ilo_cyfrowa-- ;


		//        pierwsza_dzialka = (int(minX / dzialka_skali)  * dzialka_skali);
		//               cout << " po dzieleniu przez zero - Pierwsza dzialka = " << pierwsza_dzialka  << endl ;
	}
	else
	{
		//        cout << "roznica < 1 " << endl ;
		double rezultat = fabs(roznica_kon_pocz) ;
		ilo_cyfrowa = -1;
		while(1)
		{
			rezultat *= 10 ;
			if (rezultat >=1)
				break ;
			ilo_cyfrowa-- ;
			//            cout  << "rrrezultat " << rezultat
			//                  << ", wynik = " << wynik  << endl ;
		}
		//      cout << "ostatecznie " << wynik << "czyli 10**n = "
		//       << pow(10.0, (double) wynik) << endl ;

	}

	// cout << "    Liczba ma cyfr :" << ilo_cyfrowa << endl;
	return ilo_cyfrowa;
}
//***********************************************************************
void box_of_channels::mouseMoveEvent ( QMouseEvent * event )
{
	int now_x = event->x();


	appl_form_ptr->send_to_statusbar(
				"Hint: use a mouse wheel to expand spectrum,  click and drag the scale to move a spectrum,",  10*1000   ); //  seconds on the screen


	if(event->buttons() != Qt::LeftButton) return;


	if(fabs(last_dragged_x - now_x) > 0)
	{

		double diff =   pix2worX(last_dragged_x) - pix2worX(now_x) ;

		//   // if it was moved really
		//    cout<< "box of channels, mouse move  event moved by "
		//        << - (now_x - last_dragged_x) << " pixels "
		//        << " in word coordinates it is  = " << diff
		//        << endl;

		last_dragged_x = now_x ;

		typ_x  min_ch, max_co, max_ch, min_co;
		spectrum_descr sd;
		parent->give_parameters(&min_ch, &max_co, & max_ch, & min_co, &sd);

		if( (max_ch + diff > sd.end)          // if too much to the right
				||
				(min_ch + diff < sd.beg) )    //    too much to the left
		{
			//forbidden coursor
			setCursor ( QCursor (Qt::ForbiddenCursor ) ) ;
			appl_form_ptr->send_to_statusbar(
						"Dragging more in this direction - is not possible (OUT OF LIMITS of the spectrum)",  5*1000   ); //  seconds on the screen
		}
		else{
			setCursor ( QCursor (Qt::OpenHandCursor ) ) ;
			parent->set_parameters(min_ch+diff, max_co, max_ch+diff, min_co);
			appl_form_ptr->if_needed_apply_to_other_spectra();
		}
        parent ->flag_repaint_spectrum_box = true;
	}else
	{
		//       cout << "no move now_x ="   << now_x << endl;
	}
}
//******************************************************************************************
void box_of_channels:: mousePressEvent ( QMouseEvent * event )
{
	// remembering the pressing point

	last_dragged_x= event->x() ;
	//    cout << "Pressing at point " <<  last_moved_x
	//         << " in word coordinates it is channel = "
	//         << pix2worX(last_moved_x)

	//         << "so it was moved by "
	//         << endl;
	setCursor ( QCursor (Qt::OpenHandCursor ) ) ;
}
//*****************************************************************************
void box_of_channels:: mouseReleaseEvent( QMouseEvent *  /* e */ )
{
	setCursor ( QCursor (Qt::SizeHorCursor ) ) ;
}
//******************************************************************************************
void box_of_channels:: wheelEvent ( QWheelEvent * e )   // expanding spectrum around coursor
{

	e->accept() ;   // if this was not present here - the spectrum_1D wheelEvent was also reacting
	// just after this function

    typ_x   x=      pix2worX(


                     #if  (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
                        e->position().x()
                    #else
                        e->x()
//                        e->position().x()
                    #endif

                );
    int delta = e->angleDelta().y();
	//	  cout << "WheelEvent in box_of_channels, pt " << x
	//  //         << ", delta= " << delta
	//		   << endl ;

	double factor = 1.1;
	if(delta < 0) factor = 1/ factor;

	typ_x  min_ch, max_co, max_ch, min_co;
	spectrum_descr sd;


	parent->give_parameters(&min_ch, &max_co, & max_ch, & min_co,
							&sd);
	//  cout	<< "wheel PRZED horiz (" <<min_ch  << " : " << max_ch << ")"
	//			<< ", vertical (" << min_co << ", " << max_co << ")"  << endl;

	typ_x kon_new = (max_ch - x) * factor  + x;
	typ_x pocz_new = x - ( (x- min_ch) * factor);
	// if too much to the right

	if( (kon_new > sd.end)  || (pocz_new < sd.beg))
	{
		// forbidden coursor
		// setCursor (  QCursor (Qt::ForbiddenCursor ) ) ;

		// preparing nice text
		string andtxt;
		if((kon_new > sd.end)  && (pocz_new < sd.beg) )
		{
			andtxt = " and ";
		}

		string txt = "Warning: unzooming " ;
		if(kon_new > sd.end)
		{
			kon_new = sd.end;
			txt += "more right ";
		}
		if(pocz_new < sd.beg)
		{
			pocz_new = sd.beg;

			txt += andtxt ;
			txt += " more left ";
		}
		appl_form_ptr->send_to_statusbar(
					txt +=" - is not possible",  5*1000   ); //  seconds on the screen
	}
	else{
		setCursor ( QCursor (Qt::SizeHorCursor ) ) ;
		appl_form_ptr->send_to_statusbar(
					"",  1*1000   ); //  seconds on the screen
	}
//	cout	<< "wheel wants (" <<pocz_new  << " : " << kon_new << ")"
//			<< ", vertical (" << min_co << ", " << max_co << ")"  << endl;
    parent->flag_repaint_spectrum_box = true;
	parent->set_parameters(pocz_new, max_co, kon_new, min_co);
	parent->setFocus(); //   activateWindow();
	parent ->flag_repaint_channels_box = true;
	parent->update();
	appl_form_ptr->if_needed_apply_to_other_spectra();
}
//******************************************************************************************

