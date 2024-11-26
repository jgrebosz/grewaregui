/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
#include "paths.h"
//*****************************************************************************
void Tmerge_2D_spectra_dialog::set_parameters( QStringList *ptr_names, QStringList *ptr_factors )
{
  names = ptr_names ;
  factors = ptr_factors;
  int nr = 0 ;
  table1->setNumRows(0);
  QStringList::Iterator itfac =  factors->begin();
  for ( QStringList::Iterator it = names->begin() ; it != names->end() ;  ++it, nr++, ++itfac )
  {
    //cout << *it << "\n";
    if(nr+1 > table1->numRows())
    {
      table1->insertRows(nr, 1);
    }
    table1->setText(nr, 0, *it);
    table1->setText(nr, 1, *itfac);
  }

  table1->adjustColumn(0);
  table1->adjustColumn(1);

}
//****************************************************************************
void Tmerge_2D_spectra_dialog::factor_double_clicked()
{
  // which row is selected
  int nr = table1->currentRow();

  // Take the text from the table


  double old_value = table1->text(nr, 1).toDouble() ;

  // display the dialog
  bool ok;
  double res = QInputDialog::getDouble("Adding factor",
                                       QString("Enter a new value of the factor for spectrum %1").arg(table1->text(nr, 0)),
                                       old_value, -10000,
                                       10000, 8, &ok, this );
  if ( ok )
  {
    // user entered something and pressed OK
    // store this back into the table
    QString new_text;
    new_text.setNum(res);
    table1->setText(nr, 1, new_text);
  }
  else
  {
    // user pressed Cancel
  }
}
//****************************************************************************
void Tmerge_2D_spectra_dialog::remove_row()
{

  // which row is selected
  int nr = table1->currentRow();

  // are you sure?

  if(QMessageBox::warning(this,
                          "Removing the spectrum from the table",
                          QString(
                            "You are going to remove the row nr %1\n which is: %2  factor %3\n"
                            "Are you sure?").arg(nr+1).
                          arg(table1->text(nr, 0)).
                          arg(table1->text(nr, 1))
                          ,
                          QMessageBox::Yes,
                          QMessageBox::No,
                          QMessageBox::Cancel)
      == QMessageBox::Yes)
  {

    // remove it

    table1->removeRow(nr);
  }
}
//*******************************************
void Tmerge_2D_spectra_dialog::accept()
{
  string spec_name;
  double factor;
  bool flag_ok = true;

  // binning of the first spectrum
  double first_binx;
  double first_begx;
  double first_endx;
  double first_biny;
  double first_begy;
  double first_endy;
  
  typedef short int spec_cell;

  spec_cell * read_buf = 0;
  spec_cell * sum_buf = 0 ; // NULL
  int length = 0;

  //bool success = true;

  try
  {

    for (int nr = 0; nr < table1->numRows() ;  nr++ )
    {
      spec_name = table1->text(nr, 0).ascii();
      factor = table1->text(nr, 1).toDouble(&flag_ok);
      if(!flag_ok)
      {
        ostringstream s;
        s << "Error while converting the factor from the row nr " << nr+1 << " into a  value";
        throw s.str();
      }

      // opening the spectrum
      ifstream plik(spec_name.c_str(), ios::binary);
      if(!plik)
      {
        throw string("Error while opening the spectrum ") +  spec_name ;
      }

      // reading the binning and comparing with the first spectrum
      double binx;
      double begx;
      double endx;
      double biny;
      double begy;
      double endy;
      
      plik.read((char*) &binx, sizeof(binx));
      plik.read((char*)&begx, sizeof(begx));
      plik.read((char*)&endx, sizeof(endx));
      plik.read((char*)&biny, sizeof(biny));
      plik.read((char*)&begy, sizeof(begy));
      plik.read((char*)&endy, sizeof(endy));
      

      if(nr == 0 )  // if this is a frist spectrum
      {
        first_binx = binx;
        first_begx = begx;
        first_endx = endx;
        first_biny = biny;
        first_begy = begy;
        first_endy = endy;
        
        length = (int)( binx * biny);
        
//         cout << "First spectrum has "<< endl;
        read_buf = new spec_cell[length];
        sum_buf = new spec_cell[length];
        if(!read_buf || !sum_buf)  
        {
          throw string("Internal error: the reservation of the memory for the result was not successfull") ;         
        }
        memset(sum_buf, 0, sizeof(spec_cell) *length);
      }
      else // if this is not the frist, but some next spectrum
      {
//         cout << "Next spectrum has " << endl;
        // compare binning
        if( first_binx != binx ||  first_begx != begx ||  first_endx != endx ||
            first_biny != biny ||  first_begy != begy ||  first_endy != endy)
        {
          ostringstream s;
          s << "Impossible. You may add only the 2D spectra with the same binning. \nThe binning of the spectrum named "
              << spec_name  << " is:\n"
              << " X bins= " <<  binx
              << ", X begin= " <<  begx
              << ", X end = " <<  endx
              << ", Y bins= " <<  biny
              << ", Y begin= " <<  begy
              << ", Y end = " <<  endy
              << "\n is wrong, because the first spectum on  the list had the following binning:\n"
              << " X bins= " <<  first_binx
              << ", X begin= " <<  first_begx
              << ", X end = " <<  first_endx
              << ", Y bins= " <<  first_biny
              << ", Y begin= " <<  first_begy
              << ", Y end = " <<  first_endy;
          throw s.str();          
        }
      }

//       cout  << "Following binning "
//           << spec_name  << " is:\n"
//           << " X bins= " <<  binx
//           << ", X begin= " <<  begx
//           << ", X end = " <<  endx
//           << ", Y bins= " <<  biny
//           << ", Y begin= " <<  begy
//           << ", Y end = " <<  endy << endl;
      
      // adding
      plik.read( (char*) read_buf, sizeof(spec_cell) * length);     
      if(!plik)
      {
        throw string("Error while reading the spectrum\n")+  spec_name ;
      }

      for(int i = 0 ; i < binx * biny ; i++)
      {
        sum_buf[i] += static_cast<spec_cell> (read_buf[i] * factor );
      }
    }// end of for loop over all rows of the table
    
      // asking for a output name
    QFileDialog* fd = new QFileDialog( path.spectra, "*.mat", this,
                                       "Type the name of the output spectrum",
                                       TRUE );
    fd->setMode( QFileDialog::AnyFile );
    fd->setFilter( "2D spectrum from spy-cracow  (*.mat)" );
    QString fileName;
    if ( fd->exec() == QDialog::Accepted )
    {
      QString fileName = fd->selectedFile();
    // saving the result

      ofstream out(fileName.ascii(), ios::binary);
      if(!out)
      {
        throw string("Error while opening the result file ") + fileName.ascii() + " for writing " ;
      }
      out.write((char*) &first_binx, sizeof(first_binx));
      out.write((char*)&first_begx, sizeof(first_begx));
      out.write((char*)&first_endx, sizeof(first_endx));
      out.write((char*)&first_biny, sizeof(first_biny));
      out.write((char*)&first_begy, sizeof(first_begy));
      out.write((char*)&first_endy, sizeof(first_endy));

      out.write( (char*) sum_buf, sizeof(spec_cell) * length);
      if(!out)
      {
        throw string("Error while writing the spectrum ")+ fileName.ascii()  ;
      }
      out.close();
      if(read_buf) delete [] read_buf;
      if(sum_buf) delete [] sum_buf ;
      
      QMessageBox::information(this,
                            "Merging the 2D spectra",
                            "Success. You may watch this matrix with usual procedures",
       
                            QMessageBox::Ok | QMessageBox::Default,
                            QMessageBox::NoButton,
                            QMessageBox::NoButton);  // error

      QDialog::accept();

    }

  } // endtry
  catch(string &s)
  {
    
    QMessageBox::critical(this,
       "Merging the 2D spectra",
       s.c_str(),
       
       QMessageBox::Ok | QMessageBox::Default,
       QMessageBox::NoButton,
       QMessageBox::NoButton);  // error
    
    if(read_buf) delete [] read_buf;
    if(sum_buf) delete [] sum_buf ;

  }


}
//**********************************************************************
void Tmerge_2D_spectra_dialog::add_spectrum_name()
{

  //  cout << "Tmerge_2D_spectra_dialog::add_spectrum_name()" << endl;
  // dialog to choose the name and the format
  QFileDialog* fd = new QFileDialog( path.spectra, "*.mat", this,
                                     "Select a 2D spectrum file to be merged",
                                     TRUE );
  fd->setMode( QFileDialog::AnyFile );
  fd->setFilter( "2D spectrum from spy-cracow  (*.mat)" );
  QString fileName;
  if ( fd->exec() == QDialog::Accepted )
  {
    QString fileName = fd->selectedFile();
    // saving it into the tableEmbedImage

    int nr = table1->numRows();
    //nr++;

    table1->insertRows(nr);
    table1->setText(nr, 0, fileName);
    table1->setText(nr, 1, "1");  // addinf factor 1
    table1->adjustColumn(0);
    table1->adjustColumn(1);
  }

}
//*******************************************************************************************************
void Tmerge_2D_spectra_dialog::init()
{
  table1->setNumRows(0);

}
