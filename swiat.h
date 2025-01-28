#ifndef _SWIAT_H__
#define _SWIAT_H__

/*__________________________________________________________________________*/
// typedef int typ_x ;
typedef double typ_x ;
#include <iostream>
using namespace std ;



// tutaj, bo nie chce mi sie robic osobnego pliku
// class to keep bining and sending it to box
//////////////////////////////////////////////////////////////////////////////////////////////////////////
class spectrum_descr  // here bin is int (but in spy perhaps it is double?)
{
public:
	//typ_x
	int bin;
	typ_x                   // for normal spectra  and matrices
	beg,
	end,
	waga;

	// for matrices only
	typ_x bin_y;
	typ_x
	beg_y ,
	end_y,
	waga_y;

	// to prevent warnings,  lets make the intialisaqtion
	spectrum_descr() :bin(0), beg(0), end(0), waga(0), bin_y(0), beg_y(0), end_y(0), waga_y(0)
	{
		//        bin = 0;
		//        beg = end = waga = 0;
		//        // for matrices only
		//        bin_y = 0 ;
		//        beg_y  = end_y = waga_y = 0;
	}

	friend
	ostream &  operator<<(ostream & s, const spectrum_descr &d)
	{
		s << "X: bin=" << d.bin
		  << ", beg = " << d.beg
		  << ", end = " << d.end
		  << ", waga = " << d.waga
		  << "\nY: bin=" << d.bin_y
		  << ", beg = " << d.beg_y
		  << ", end = " << d.end_y
		  << ", waga = " << d.waga_y
		  << endl;
		return s;
	}

} ;
/////////////////////////////////////////////////////////////////////////////////
const int scrollbar_poziomy_mnoznik = 1000 ;    // 100
// NOTE: sometimes spectra have range 0.3 - 0.8
// but scrollbars can be only integer, so we had to multply thre range 0.3 * mnoznik
// 0.8 * mnoznik

#define COTO cout << "In the file " << __FILE__ << ", function: "<< __func__ << " line " << __LINE__ << endl ;


#define ZAOKR(x) (static_cast<int> (x+0.5))
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class swiatek
{
protected:

	int
	lewy_bok_dev {0},
	gorny_bok_dev {0},
	dolny_bok_dev{100},
	rozm_xdev{0} ,
	rozm_ydev{0} ;

	double
	rozm_xwor{0},              // rozmiar we wspolrzednych swiatowych
	rozm_ywor{0} ,
	scaler_x {0},
	scaler_y {0};

	double
	worldLeft {0},             // wpolrzedne swiatowe
	worldBottom {0},
	worldRight {0},
	worldTop {0};
public:

	int pozycja_x {0},
	pozycja_y {0};      /* poz pixelowa na ekranie */
	double pozycja_xw {0},
	pozycja_yw {0};   /* poz we wsp world */

public:
	swiatek()
	{
		ustaw_swiat(0, 0, 100, 100,   // dane pikselowe
					0, 100, 100, 0);
	}

	swiatek(int left, int top, int right, int bottom,   // dane pikselowe
			double xleft, double yt, double xr, double yb)
	{
		ustaw_swiat(left, top, right, bottom,   // dane pikselowe
					xleft, yt, xr, yb);
	}
	//--------------------------------------------------------------------
	// na wypadek, gdyby w trakcie pracy na ekranie ktos rozciagnal okno,
	// mamy szanse wywolania powtornego przeliczenia
	void ustaw_swiat(
			int left, int top, int right, int bottom,   // dane pikselowe
			double x0, double y1, double x1, double y0); // dane swiata,
	// jak uklad kartezjanski czyli najpierw lewy dolny rog

	/*--------------------------------------------------------------------*/
	int wx(double  x)     /* zamiana x wsp world  na pixelowa */
	{
		return static_cast<int> ( ((x- worldLeft) * scaler_x) /*+ lewy_bok_dev*/ +0.5 );
	}
	//---------------------------------------------------------------------
	int wx(int x) { return wx( static_cast<double>( x) ); }
	//int wx(double x) { return wx( (double) x); }
	/*--------------------------------------------------------------------*/
	int wy(double y)      /* zamiana y wsp world  na pixelowa */
	{
		return static_cast<int> ( dolny_bok_dev - ((y-worldBottom) * scaler_y) - 0.5);
	}
	//---------------------------------------------------------------------
	int wy(int y) { return wy( static_cast<double>(y) ) ;}
	//int wy(double y) { return wy( (double) y) ;}
	/*____________________________________________________________________*/
	// przydaje sie tez przyrostowo, czyli ile pikseli ma odcinek, ktorego
	// dlugosc podajemy we wspolrzednych swatowych
	/*--------------------------------------------------------------------*/
	int relx(double x)
	{
		return static_cast<int> (  ((x) * scaler_x) +0.5);
	}
	/*____________________________________________________________________*/
	int rely(double y)
	{
		return static_cast<int> ( ((y) * scaler_y) + 0.5 );
	}
	//======================================================================
	void pozycje(double x, double y)           /* zapamietanie pozycji pix i world
						  kursora graficznego             */
	{
		pozycja_x = ZAOKR( ((pozycja_xw = x) - worldLeft) * scaler_x) ;
		pozycja_y = ZAOKR( ((pozycja_yw = y) - worldBottom) * scaler_y) ;
	}
	/*------------------------------------------------------------------------*/
	void rel_pozycje(double x, double y)      /* przesuniecie rel zapamietanych
							 pozycji pix i world  kursora graficznego */

	{
		pozycja_xw += x ;
		pozycja_yw += y ;

		pozycja_x += ZAOKR( (x ) * scaler_x) ;
		pozycja_y += ZAOKR( (y ) * scaler_y) ;
	}
	/*--------------------------------------------------------------------*/
	/* moze sie przydac w druga strone */
    // double pix2worX(int x)      /* zamiana wsp pixelowych na world   */
    // {
    // 	return  ( worldLeft + (x/scaler_x)) ;
    // }
      double pix2worX(double x)      /* zamiana wsp pixelowych na world   */
    {
        return  ( worldLeft + (x/scaler_x)) ;
    }
	//--------------------------------------------------------------------
	double pix2worY(int y)      /* zamiana y wsp pixelowa na world  */
	{
		return( worldTop - (y / scaler_y) );
	}
	/*____________________________________________________________________*/

}
;
////////////////////////////////////////////////////////////////////////////
#endif //_SWIAT_H_
