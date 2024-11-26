/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <fstream>
using namespace std;
#include "Tfile_helper.h"

extern paths path ;
//******************************************************************************
void Tbinning_change::init()
{
   this_is_2D_spectrum = false;
   suffix = ".binning";
}
//******************************************************************************
void Tbinning_change::set_parameters( string name, spectrum_descr sd, int dim )
{
   
   memcpy(&spde, &sd, sizeof(sd));
   this_is_2D_spectrum = (dim == 2);
   // remember the name in case if we want to save this file
   spectrum_name = name ;
   string in("The current binning of the spectrum ");
   in += name ;
   text_info->setText(in.c_str() );
   
   check_binning_on_the_disk_file();
   
   // enable/disable 1D or 2D tab  
   //tabWidget2->setCurrentPage(dim - 1); 
   
   tabWidget2->setTabEnabled( tabWidget2->page(0), (dim == 1));
   tabWidget2->setTabEnabled( tabWidget2->page(1), (dim == 2));
   
   switch(dim)
   {
   case 1: 
      // reading the current binning from the disk - 
      lineEdit_bins->setText(QString::number(spde.bin) );
      lineEdit_first_bin->setText(QString::number(spde.beg) );
      lineEdit_last_bin->setText(QString::number(spde.end) );
      
      break;
      
   case 2:
      {
         lineEdit_bins_x2->setText(QString::number(spde.bin) );
         lineEdit_first_bin_x2->setText(QString::number(spde.beg) );
         lineEdit_last_bin_x2->setText(QString::number(spde.end) );
         
         lineEdit_bins_y->setText(QString::number(spde.bin_y) );
         lineEdit_first_bin_y->setText(QString::number(spde.beg_y) );
         lineEdit_last_bin_y->setText(QString::number(spde.end_y) );
         
      }
      break;
   }	// end switch 
}
//******************************************************************************
void Tbinning_change::accept()
{
   
   // at first let's check if there was any change
   bool has_changed = false;
   
   if(!this_is_2D_spectrum)
   {
      has_changed = (
         spde.bin   != lineEdit_bins->text().toInt()
         ||
         spde.beg != lineEdit_first_bin->text().toDouble()
         ||
         spde.end != lineEdit_last_bin->text().toDouble()
         );
   }
   else // it must be 2D otherwise
   {
      has_changed =
         (lineEdit_bins_x2->text().toInt() != spde.bin)
         || (lineEdit_first_bin_x2->text().toDouble() != spde.beg)
         || (lineEdit_last_bin_x2->text().toDouble()  != spde.end )
         || (lineEdit_bins_y->text().toInt()   != spde.bin_y)
         || (lineEdit_first_bin_y->text().toDouble()   != spde.beg_y)
         || (lineEdit_last_bin_y->text().toDouble()   != spde.end_y);
   }
   
   if(has_changed)
   {
      string pathed_name = path.my_binnings +  spectrum_name + suffix;
      // saving this to the disk file
      ofstream plik(pathed_name.c_str());
      
      if(!this_is_2D_spectrum )
      {
         plik 
            << lineEdit_bins->text().toInt()
            << "\t// how many X bins \n"      
            <<  lineEdit_first_bin->text().toDouble()
            << "\t// left edge of the first X bin  \n"      
            << lineEdit_last_bin->text().toDouble()
            << "\t// right edge of the last X bin \n"      
            << endl; 
         
      }
      else
      {
         plik 
            << lineEdit_bins_x2->text().toInt()
            << "\t// how many X bins \n"      
            << lineEdit_first_bin_x2->text().toDouble()
            << "\t// left edge of the first X bin \n"      
            << lineEdit_last_bin_x2->text().toDouble()
            << "\t// right edge of the last X bin \n"      
            << endl; 
         
         plik 
            << lineEdit_bins_y->text().toInt()
            << "\t// how many Y bins \n"      
            <<   lineEdit_first_bin_y->text().toDouble()
            << "\t// left edge of the first Y bin \n"      
            <<  lineEdit_last_bin_y->text().toDouble()
            << "\t// right edge of the last Y bin \n"      
            << endl;    
      }
      
      
      QDialog::accept();
      QMessageBox::information ( this, "Note",
                                 "You just made the change of the binning\n"
                                 "This kind of change requires that you FINISH the work of the SPY\n"
                                 "and RUN it again - because the spectra are created only on the\n"
                                 "begining of the SPY's work\n(so just 'UPDATE' is NOT ENOUGH!)\n\n"
                                 "The last contens of the spectra (which will have now different range)\n"
                                 "will be erased.\n"
                                 "All other spectra will not be changed" ,
                                 QMessageBox::Ok); 
      
   }
   else QDialog::accept();
   
}
//**********************************************************************************
bool Tbinning_change::check_binning_on_the_disk_file()
{
   // this function checks if the current binning file is equal to this what is displayed
   // in cracow viewer. It should be normally the same, but if the user did not restarted
   // the spy after the last change of the binning, the spectrum will still have the old binning
   // while on the disk the new binning file is waiting to be applied to the spy
      
   string pathed_name = path.my_binnings +  spectrum_name + suffix ;
   
   // reading saving this to the disk file
   ifstream plik(pathed_name.c_str());
   if(!plik) return 0 ; // no binning file was before
   
   int bx, by;
   double x0, x1, y0, y1;
   bool difference = false ;
   
   if(!this_is_2D_spectrum )
   {
      plik >> zjedz >> bx >> zjedz >> x0 >> zjedz >> x1 ;
      difference = 
         (bx != spde.bin  
          || x0 != spde.beg
          || x1 != spde.end);
      
   }
   else
   {
      plik >> zjedz >> bx >> zjedz >> x0 >> zjedz >> x1 ;
      plik >> zjedz >> by >> zjedz >> y0 >> zjedz >> y1 ;
      
      difference = 
         (bx != spde.bin  
          || x0 != spde.beg
          || x1 != spde.end
          || by != spde.bin_y  
          || y0 != spde.beg_y
          || y1 != spde.end_y )  ;
      
   }
   
   
   if(difference)
   {
      
   switch( QMessageBox::warning( this, "Strange difference",
        "The spectrum in Cracow has currently different binning that the file on the disk\n"
        "This may happen if you just changed the binning, but did not run the SPY again\n"
        "- so the SPY still does not know, about the last change.\n\n"
        "No probem, but tell me know which version of binning should I display now:"
        ,
        "Newer, disk version - (hence not updated yet)",
        "Older version, (which I see in cracow)", 0, 0, 1 ) )
   {
      case 0: // The user clicked the Newer, disk version 
        //
      spde.bin  = bx;
      spde.beg = x0;
      spde.end = x1;
      if(this_is_2D_spectrum )
      {
         spde.bin_y  = by;
         spde.beg_y = y0;
         spde.end_y  = y1 ;
      }
      break;
      
      case 1: // The user clicked the Quit or pressed Escape
      // exit
      break;
   }   
      
   
} // end if difference 
   return false ; // we do not use this result  
}
