extern options current_options ;
extern paths path ;
class polygon_gate;
//***********************************************************************
bool spectrum_widget::flag_white_black;
//***********************************************************************
void spectrum_widget::paintEvent( QPaintEvent * )
{}
//***********************************************************************
void spectrum_widget::init()
{
  re_read_counter = 0 ;
  // initial fake values, the real ones we obtain from the printer setup
  paper_width = 500;
  paper_height = 300 ;
  flag_draw_scales = true;
}
//***********************************************************************
void spectrum_widget::resizeEvent( QResizeEvent * )
{}
//***********************************************************************
void spectrum_widget::full()
{}
//***********************************************************************
void spectrum_widget::shift_right( int )
{}
//***********************************************************************
void spectrum_widget::shift_left( int )
{}
//***********************************************************************
void spectrum_widget::scale_to_maxY()
{}
//***********************************************************************
void spectrum_widget::scaleY_by_2()
{}
//******************************************************************************
void spectrum_widget::scaleY_by_05()
{}
//******************************************************************************
void spectrum_widget::integrate( results_of_integration * )
{}
//******************************************************************************
void spectrum_widget::give_parameters( typ_x *, typ_x *, typ_x *, typ_x *, spectrum_descr * )
{}
//******************************************************************************
void spectrum_widget::scrollbar_horizontal_moved( int )
{}
//******************************************************************************
void spectrum_widget::scaleY_by_factor( double )
{}
//******************************************************************************
void spectrum_widget::slider_horizontal( int )
{}
//******************************************************************************
void spectrum_widget::scroller_vertical_moved( int )
{}
//******************************************************************************
int spectrum_widget::giveCurrentMaxCounts()
{
  return 0 ;
}
//******************************************************************************
void spectrum_widget::spectrum_widget_destroyed( QObject * )
{

  //    cout << "specrum destroyed " << endl;
}
//******************************************************************************
void spectrum_widget::set_parameters( typ_x, typ_x, typ_x, typ_x)
{
  cout << "Abstract version of the virtual function spectrum_widget::set_parameters"
  << endl;
}
//******************************************************************************
void spectrum_widget::save()
{}
//******************************************************************************
void spectrum_widget::save_as(string /*prefix*/)
{}
//******************************************************************************
void spectrum_widget::real_save_ascii( QString & , bool /*flag_radware*/)
{}
//******************************************************************************
void spectrum_widget::set_the_name( QString & )
{}
//******************************************************************************
void spectrum_widget::print_it()
{}
//******************************************************************************
void spectrum_widget::draw_all_on_screen()
{
    cout << "F. spectrum_widget::draw_all_on_screen()" << endl;

}
//******************************************************************************
void spectrum_widget::read_in_file( const char *, bool /*this_is_first_time*/ )
{}
//******************************************************************************
bool spectrum_widget::find_x_description( const char * )
{
  return false ;
}
//******************************************************************************
void spectrum_widget::log_linear( bool )
{}
//******************************************************************************
void spectrum_widget::draw_all( QPainter * )
{
     cout << "F. spectrum_widget::draw_all(QPainter *)" << endl;

}
//******************************************************************************
bool spectrum_widget::give_flag_log_scale()
{
  return false ;
}
//******************************************************************************
void spectrum_widget::expand()
{}
//******************************************************************************
void spectrum_widget::show_polygons( bool )
{}
//******************************************************************************
void spectrum_widget::lupa_button_pressed()
{}
//******************************************************************************
bool spectrum_widget::undo()
{
  return false ;
}
//******************************************************************************
void spectrum_widget::shift_up_down( int )
{}
//******************************************************************************
spectrum_descr spectrum_widget::give_specif()
{
  return specif ;
}
//******************************************************************************
void spectrum_widget::show_disk_names_of_gates( bool /*flag*/ )
{}
//******************************************************************************
void spectrum_widget::get_focus( QObject * )
{
  //    cout << " this spectrum widget get a focus" << endl;
}
//******************************************************************************
//******************************************************************************
bool spectrum_widget::give_flag_black_white()
{
  return flag_white_black ;
}
//**********************************************************************
void spectrum_widget::re_read_spectrum()
{}
//**********************************************************************
void spectrum_widget::black_white_spectrum(bool /*on*/)
{}
//**********************************************************************
void spectrum_widget::destroy()
{
  if(timer_refresh)
  {
    timer_refresh->stop(); // 20 seconds
    delete timer_refresh ;
    timer_refresh = 0 ;
  }
  //    cout << "Destroy of abstract spectrum " << endl;
}
//**********************************************************************
void spectrum_widget::closeEvent_( QCloseEvent * e )
{
  //cout << "This was close event in spectrum widget" << endl;
  e->accept();
  destroy();  // stop the timer
}
//**********************************************************************
void spectrum_widget::copy_markers( spectrum_widget * /*doc*/ )
{}
//***********************************************************************
string spectrum_widget::give_spectrum_name()
{
  return name_of_spectrum ;
}
//*********************************************************************
void spectrum_widget::set_new_refreshing_times()
{}
//*********************************************************************
void spectrum_widget::re_read_counter_after_zeroing( int count )
{
  re_read_counter = count ;
}
//*********************************************************************
string spectrum_widget::projection( int /*axis_x*/ )
{
  cout << "executing spectrum_widget::projection( int /*axis_x*/ )" << endl;
  return "" ;  // empty string - in case of the 1D spectrum
}
//*********************************************************************
void spectrum_widget::find_notice_in_description()
{
  string desc_name = path.spectra + "descriptions.txt";
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
            if(flag_loading)  // remove trailing white spaces
            {
              string::size_type pos = notice.find("\n");
              if(pos != string::npos)
                notice.erase(pos);
            }

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
  //cout << "Finally description of the spectum " << name << " was not found " << endl ;
  notice = "" ;
  return  ;
}
//**************************************************************************
string spectrum_widget::give_notice()
{
  return notice;
}
//****************************************************************************
void spectrum_widget::zero_spectrum()
{

   cout << "Abstract spectrum_widget::zero_spectrum() " << endl;
}
//****************************************************************************
string spectrum_widget::give_undo_text( int /*nr*/ )
{
  return "" ;
}
//****************************************************************************
void spectrum_widget::print_postscript()
{
  cout << "Abstract version of the spectrum_widget::print_postscript() - was called "
  << endl;
}
//****************************************************************************

bool spectrum_widget::printing_mode()
{
  return flaga_this_is_printing;
}

//****************************************************************************
int spectrum_widget::give_paper_width()
{
  return paper_width ;
}
//****************************************************************************
int spectrum_widget::give_paper_height()
{
  return paper_height ;
}

//***************************************************************************
// For printing - the channel box should be sticked to the x axis of the spectrum
// and counts box should be sticked to the y axis of the spectrum. So counts and chnnel boxes
// should ask where is the spectrum box
//***************************************************************************
void spectrum_widget::give_Cx_Cy(double *x, double *y )
{
  *x = -1;
  *y = -1 ;
  return ;
}
//**********************************************************************
void spectrum_widget::freezing_photo_of_spectrum()
{}
//**********************************************************************
void spectrum_widget::read_list_of_incrementers( ifstream &plik )
{
  //cout << "F. void spectrum_widget::read_list_of_incrementers( ifstream &plik )"
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
    //cout << " read line: " << wyraz << endl ;
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
            plik >> first_word ; // to skip while spaces
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
              string::size_type pos = list_of_incrementers_of_this_spectrum.find("\n");
              if(pos != string::npos)
                list_of_incrementers_of_this_spectrum.erase(pos);
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
void spectrum_widget::draw_scale_or_not( bool stan )
{
flag_draw_scales = stan;
}

//*************************************************************
bool spectrum_widget::give_flag_draw_scales()
{
return flag_draw_scales ;
}


void spectrum_widget::mouseMoveEvent_( QMouseEvent * /*e*/ )
{

}
