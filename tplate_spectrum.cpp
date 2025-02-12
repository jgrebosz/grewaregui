#include "tplate_spectrum.h"
#include "ui_tplate_spectrum.h"

#include "t4results_of_integration.h"

#include <QCloseEvent>
#include "options.h"
#include "paths.h"
#include <QTimer>
#include <fstream>

extern options current_options ;
extern Tpaths path ;
class polygon_gate;
class T4results_of_integration;

bool Tplate_spectrum::flag_repainting_allowed = true;
//***********************************************************************
Tplate_spectrum::Tplate_spectrum(QWidget *parent) :
    QWidget( (QWidget*)  parent  ),
   // QMdiSubWindow(  parent  ),

    ui(new Ui::Tplate_spectrum)
{
    ui->setupUi(this);
    dimension = 0 ;
    // setGeometry(10,10, 180, 100);



}
//***********************************************************************
Tplate_spectrum::~Tplate_spectrum()
{
    delete ui;
}
//***********************************************************************

//***********************************************************************
bool Tplate_spectrum::flag_white_black;
//***********************************************************************
void Tplate_spectrum::paintEvent( QPaintEvent * )
{}
//***********************************************************************
void Tplate_spectrum::init()
{
    re_read_counter = 0 ;
    // initial fake values, the real ones we obtain from the printer setup
//    paper_width = 500;
//    paper_height = 300 ;
    flag_draw_scales = true;
}
////***********************************************************************
//void Tplate_spectrum::resizeEvent( QResizeEvent * )
//{}
//***********************************************************************
void Tplate_spectrum::full()
{}
//***********************************************************************
void Tplate_spectrum::shift_right( int )
{}
//***********************************************************************
void Tplate_spectrum::shift_left( int )
{}
//***********************************************************************
void Tplate_spectrum::scale_to_maxY()
{}
//***********************************************************************
void Tplate_spectrum::scaleY_by_2()
{}
//******************************************************************************
void Tplate_spectrum::scaleY_by_05()
{}
//******************************************************************************
void Tplate_spectrum::integrate( T4results_of_integration * )
{}
//******************************************************************************
void Tplate_spectrum::give_parameters( typ_x *, typ_x *, typ_x *, typ_x *, spectrum_descr * )
{}
//******************************************************************************
// void Tplate_spectrum::scrollbar_horizontal_moved( int )
// {}
//******************************************************************************
void Tplate_spectrum::scaleY_by_factor( double )
{}
//******************************************************************************
// void Tplate_spectrum::slider_horizontal( int )
// {}
//******************************************************************************
// void Tplate_spectrum::scroller_vertical_moved( int )
// {}
//******************************************************************************
int Tplate_spectrum::giveCurrentMaxCounts()
{
    return 0 ;
}
//******************************************************************************
void Tplate_spectrum::Tplate_spectrum_destroyed( QObject * )
{

    //    cout << "specrum destroyed " << endl;
}

//******************************************************************************
void Tplate_spectrum::set_parameters( typ_x, typ_x, typ_x, typ_x)
{
    cout << "Abstract version of the virtual function Tplate_spectrum::set_parameters"
         << endl;
}
//******************************************************************************
void Tplate_spectrum::save()
{}
//******************************************************************************
void Tplate_spectrum::save_as(string /*prefix*/)
{}
//******************************************************************************
void Tplate_spectrum::real_save_ascii( QString & , bool /*flag_radware*/)
{}
//******************************************************************************
void Tplate_spectrum::set_the_name( QString & )
{}
//******************************************************************************
//void Tplate_spectrum::print_it()
//{}
//******************************************************************************
void Tplate_spectrum::draw_all_on_screen()
{}
//******************************************************************************
void Tplate_spectrum::read_in_file( const char *, bool /*this_is_first_time*/ )
{}
//******************************************************************************

//******************************************************************************
void Tplate_spectrum::log_linear( bool )
{  }
//******************************************************************************
void Tplate_spectrum::auto_scale( bool )
{ cout << "Wirtualna " << __PRETTY_FUNCTION__ << endl;}
//******************************************************************************
void Tplate_spectrum::draw_all( QPainter * )
{}
//******************************************************************************
bool Tplate_spectrum::give_flag_log_scale()
{
    return false;
}
//******************************************************************************
void Tplate_spectrum::expand()
{}
//******************************************************************************
void Tplate_spectrum::show_polygons( bool )
{}
//******************************************************************************
void Tplate_spectrum::lupa_button_pressed()
{}
//******************************************************************************
bool Tplate_spectrum::undo()
{
    return false ;
}
//******************************************************************************
void Tplate_spectrum::shift_up_down( int )
{}
//******************************************************************************
spectrum_descr Tplate_spectrum::give_specif()
{
    return specif ;
}
//******************************************************************************
void Tplate_spectrum::show_disk_names_of_gates( bool /*flag*/ )
{}
//******************************************************************************
void Tplate_spectrum::get_focus( QObject * )
{
    //    cout << " this spectrum widget get a focus" << endl;
}
//******************************************************************************
//******************************************************************************
bool Tplate_spectrum::give_flag_black_white()
{
    return flag_white_black ;
}
//**********************************************************************
void Tplate_spectrum::re_read_spectrum()
{}
//**********************************************************************
void Tplate_spectrum::black_white_spectrum(bool /*on*/)
{}
//**********************************************************************
void Tplate_spectrum::destroy()
{
    if(timer_refresh)
    {
        timer_refresh->stop(); // 20 seconds
        delete timer_refresh ;
		timer_refresh = nullptr ;
    }
    //    cout << "Destroy of abstract spectrum " << endl;
}
//**********************************************************************
void Tplate_spectrum::closeEvent_( QCloseEvent * e )
{
    //cout << "This was close event in spectrum widget" << endl;
    e->accept();
    destroy();  // stop the timer
}
//**********************************************************************
void Tplate_spectrum::copy_markers( Tplate_spectrum * /*doc*/ )
{}
//***********************************************************************
string Tplate_spectrum::give_spectrum_name()
{
    return name_of_spectrum ;
}
//*********************************************************************
void Tplate_spectrum::set_new_refreshing_times()
{}
//*********************************************************************
void Tplate_spectrum::re_read_counter_after_zeroing( int count )
{
    re_read_counter = count ;
}
//*********************************************************************
string Tplate_spectrum::projection( int /*axis_x*/ )
{
    cout << "executing Tplate_spectrum::projection( int /*axis_x*/ )" << endl;
    return "" ;  // empty string - in case of the 1D spectrum
}
//*********************************************************************
void Tplate_spectrum::find_notice_in_description()
{
    string desc_name = gpath.spectra + "descriptions.txt";
    ifstream plik(desc_name.c_str()) ;
    if(!plik)
        return  ;

    //cout << "Searching in the " << desc_name << endl ;
    string wyraz ;

    plik >> wyraz;
    bool flag_version_with_incrementers = false;
    if(wyraz == "version_with_incrementers")
    {
        flag_version_with_incrementers = true;
        //cout << "The descrition file may use the new system - with the incrementers" << endl;
    }
    while(1)
    {
        //
        if(!plik)
        {
            //cout << "reading of  " << desc_name << " failed " << endl ;
            break ;
        }
        if(plik.eof() )
        {
            //cout  << desc_name  << " riched the eof " << endl ;
            break ;
        }

        getline(plik, wyraz);
        //plik >> wyraz ;
        //cout << " read line: " << wyraz << endl ;
        //

        if(wyraz == name_of_spectrum)  // found  ----------
        {
            // not needed anymore
            // scan to the nearest: {
            char c ;
            bool flag_loading = false ;
            do
            {
                plik.get(c);
                switch(c)
                {
                case '{':
                {
                    // read spaces ?

                    string first_word ;
                    plik >> first_word ; // to skip while spaces
                    if(first_word == "}")
                    {
                        if(flag_version_with_incrementers)
                            read_list_of_incrementers(plik);
                        return ;
                    }
                    notice = first_word ;
                    flag_loading = true ;
                    break;
                }

                default:
                    if(flag_loading)
                    {
                        notice += c ;
                    }
                    break ;

                case '}':
                {
//                    if(flag_loading)  // remove trailing \n  NO!
//                    {
//                        //string::size_type pos = notice.find("\n");
////                        if(pos != string::npos)
////                            notice.erase(pos);
//                    }

                    // here we may read the list of incrementers
                    if(flag_version_with_incrementers)
                        read_list_of_incrementers(plik);

                    return  ;
                }
                }
            }
            while(plik) ;

        }

    }
    //cout << "Description of the spectum " << name << " was not found " << endl ;
    notice = "" ;
    return  ;
}
//**************************************************************************
string Tplate_spectrum::give_notice()
{
    return notice;
}
//****************************************************************************
void Tplate_spectrum::zero_spectrum()
{

    cout << "Abstract Tplate_spectrum::zero_spectrum() " << endl;
}
//****************************************************************************
string Tplate_spectrum::give_undo_text( int /*nr*/ )
{
    return "" ;
}
//****************************************************************************
void Tplate_spectrum::print_postscript()
{
    cout << "Abstract version of the Tplate_spectrum::print_postscript() - was called "
         << endl;
}
//****************************************************************************

//bool Tplate_spectrum::printing_mode()
//{
//    return flaga_this_is_printing;
//}

//****************************************************************************
//int Tplate_spectrum::give_paper_width()
//{
//    return paper_width ;
//}
////****************************************************************************
//int Tplate_spectrum::give_paper_height()
//{
//    return paper_height ;
//}

//***************************************************************************
// For printing - the channel box should be sticked to the x axis of the spectrum
// and counts box should be sticked to the y axis of the spectrum. So counts and chnnel boxes
// should ask where is the spectrum box
//***************************************************************************
void Tplate_spectrum::give_Cx_Cy(double *x, double *y )
{
    *x = -1;
    *y = -1 ;
    return ;
}
//**********************************************************************
void Tplate_spectrum::freezing_photo_of_spectrum()
{}
//**********************************************************************
void Tplate_spectrum::read_list_of_incrementers( ifstream &plik )
{
    //cout << "F. void Tplate_spectrum::read_list_of_incrementers( ifstream &plik )"
    //  << endl;

    string wyraz;

    while(1)
    {
        //
        if(!plik)
        {
            //cout << "reading of  " << desc_name << " failed " << endl ;
            break ;
        }
        if(plik.eof() )
        {
            //cout  << desc_name  << " riched the eof " << endl ;
            break ;
        }

        getline(plik, wyraz);
        //plik >> wyraz ;
		// cout << " read line: " << wyraz << endl ;
        //

        //      if(wyraz == name_of_spectrum)  // found  ----------
        {
            // not needed anymore
            // scan to the nearest: {
            char c ;
            bool flag_loading = false ;
            do
            {
                plik.get(c);
                switch(c)
                {

                case '{':  // opening the incrementers list
                {
                    string first_word ;
					plik >> first_word ; // to skip white spaces
					if(first_word == "}")  // so empty list of   incrementers
					{
						return ;
					}

                    list_of_incrementers_of_this_spectrum = first_word ;
                    flag_loading = true ;
                    break;
                }

                default:
                    if(flag_loading)
                    {
                        list_of_incrementers_of_this_spectrum += c ;
                    }
                    break ;

                case '}':
                {
                    if(flag_loading)  // remove trailing white spaces
                    {
//                        string::size_type pos = list_of_incrementers_of_this_spectrum.find("\n");
//                        if(pos != string::npos)
//                            list_of_incrementers_of_this_spectrum.erase(pos);
                    }
                    return  ;
                }
                }
            }
            while(plik) ;
        }
    }
}
//*************************************************************
void Tplate_spectrum::draw_scale_or_not( bool stan )
{
    flag_draw_scales = stan;
}
//*************************************************************
bool Tplate_spectrum::give_flag_draw_scales()
{
    return flag_draw_scales;
}
//*************************************************************
void Tplate_spectrum::mouseMoveEvent_( QMouseEvent * /*e*/ )
{

}
