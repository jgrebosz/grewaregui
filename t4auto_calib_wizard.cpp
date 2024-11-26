#include "t4auto_calib_wizard.h"
#include "ui_t4auto_calib_wizard.h"

#include <QFileDialog>

#include "paths.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
using namespace std;
#include "Tfile_helper.h"
#include "tplate_spectrum.h"


//******************************************************************************************
T4auto_calib_wizard::T4auto_calib_wizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::T4auto_calib_wizard)
{
    ui->setupUi(this);

	// To avoid writing this in Polish
	setButtonText(QWizard::NextButton, "Next  >" );
	setButtonText(QWizard::BackButton, "<  Back" );
	setButtonText(QWizard::CancelButton, "Cancel" );
	setButtonText(QWizard::FinishButton, "Finish" );

//      ui->helpButton () ->hide();
      ui->pushButton_show_spectra_names->setEnabled(0);
      // // /*  radio_clusters->setChecked(true);
      // //   radio_clover->setChecked(false);*/
      ui->checkBox_archive->setChecked(true) ;
      //   radioButton_8_gfactor->setChecked(false);
      //   radioButton_15_clusters->setChecked(true);

      type_of_det_changed();

      // restore the previous skeleton
      string n = gpath.autom + "last_skeleton";
      just_restore_skeleton(n);


}
//******************************************************************************************
T4auto_calib_wizard::~T4auto_calib_wizard()
{
    delete ui;
}
//******************************************************************************************

//*********************************************************************

void T4auto_calib_wizard::on_push_run_gf4_clicked()
{
      //    string markers = "ABCDEFGJKLMNPQR" ;
  //   string markers =
  //     (radioButton_15_clusters->isChecked())? "ABCDEFGJKLMNPQR"  : "ABCDGJKL" ;

  // NOTE: in case of the g-factors experiments we have only 8 custers, but it does not
  // matter. Their names are one of this set of letters above. The non existing spectra
  // will be ignored by the 'convert_one_spectrum' function.

  // if we have some noise at the begining of spectrum
  channel_threshold = ui->spinBox1_thrsh->value();

  update() ;

  // read spectra files into the program and save them in the ascii format
  // (because so far they are in the binary format)

  // make list  list.txt
  string listname = gpath.autom + "list.txt" ;
  ofstream plik(listname.c_str() );

  if(!plik)
  {
    string mess =  "Can't open file to write list of spectra " + listname ;
    ui->textEdit_error_messages->setPlainText(mess.c_str()) ;
    return ;
  }

  //vector<int> channel ;



  // now for xia electronics---------------------------

  for(unsigned int d = 0 ; d < spectra_names.size() ; d++)
  {

    string core_name = spectra_names[d] ;
    string binary_spec = gpath.spectra +  core_name ; // + ".spc" ;

    ui->textEdit_error_messages->setPlainText(binary_spec.c_str()) ;

    if(convert_one_spectrum(binary_spec, core_name ))
    {
      // store the name of the output into the list file
      plik << core_name << ".txt" << endl;
    }

  }



  plik.close() ;
  ui->textEdit_error_messages->setPlainText("Running gf5...") ;

  string batch_file_txt = "cd " + gpath.autom + "\n./gf5 list.txt" ;
  string name_batch = gpath.autom + "do_it.bat" ;
  plik.open(name_batch.c_str());
  plik << batch_file_txt ;
  plik.close() ;


  update() ;  // ????
  // run gf4 list.txt
  string komenda = "source " + gpath.autom + "do_it.bat" ;
  int result = system(komenda.c_str()) ;  result = result;


  // take  error messages---------------------------------------
  string log_name = gpath.autom + "list.log" ;
  ifstream plik_in(log_name.c_str()) ;
  if(!plik_in)
  {
    string mess =  "Can't open file " + log_name ;
    ui->textEdit_error_messages->setPlainText(mess.c_str());
    return ;
  }


  string very_long ;
  string line ;
  while(getline(plik_in, line, '\n') )
  {
    very_long += (line + "\n") ;
  }
  ui->textEdit_error_messages->setPlainText(very_long.c_str());


  plik_in.close() ;
  // take results---------------------------------

  string result_name = gpath.autom + "list.out" ;
  ifstream plik_outowy(result_name.c_str()) ;
  if(!plik_outowy)
  {
    string mess =  "Can't open file " + result_name ;
    ui->textEdit_error_messages->setPlainText(mess.c_str());
    return ;
  }

  very_long = "" ;
  line = "" ;
  while(getline(plik_outowy, line, '\n') )
  {
    very_long += (line + "\n") ;
  }
  ui->textEdit_results->setPlainText(very_long.c_str());

  //#ifdef NIGDY
  //=============================================
  // now we can clean the temproary ASCII spectra
  ifstream p(listname.c_str());
  string spec_name;
  komenda = "rm " ;

  while(p)
  {
    p >> spec_name;
    komenda += gpath.autom + spec_name + " " ;
  }
  // komenda += " >> null";
  result = system(komenda.c_str()) ;
  p.close();

  // Another list of spectra (with extension *.spe) ============
  // Perhaps they should not be removed, because the user could
  // fit them manually ?

  listname = gpath.autom + "list.in" ;
  ifstream plik2(listname.c_str());

  spec_name.clear();
  komenda = "rm " ;

  while(plik2)
  {
    plik2 >> spec_name;
    komenda += gpath.autom + spec_name + " " ;
  }
  // komenda += " >> null";
  result = system(komenda.c_str()) ;
  //   cout << "KOM: =" << komenda << endl;

  //#endif

}
//*************************************************************************

void T4auto_calib_wizard::on_push_install_clicked()
{
#define CCC // cout << __FILE__ << " linia " << __LINE__ << endl ;

  CCC
  try
  {
    // copy current into history subdirectory with the date

    string cal_filename_no_extension = "exotic_calibration" ;

    // radio_miniball->isChecked() ?
    //       "exotic_calibration"
    //       :
    //       "pisolo_calibration";

    CCC
    string cal_filename_with_extension = cal_filename_no_extension + ".txt";

    string oldfile = gpath.calibration + cal_filename_with_extension;

    // preparing the date to modify the name

    time_t rawtime;
    struct tm * timeinfo;
    CCC
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    CCC
    printf ( "Current date and time are: %s", asctime (timeinfo) );
    string date ;
    ostringstream s(date) ;
    s  << setfill ('0')
    << setw(2) << timeinfo->tm_mday << "_"
    << setw(2) << (timeinfo->tm_mon+1) << "_"
    << setw(2) << timeinfo->tm_year + (1900 - 2000) << "_"
    << setw(2) << timeinfo->tm_hour << ":"
    << setw(2) << timeinfo->tm_min << ":"
    << setw(2) << timeinfo->tm_sec ;
    CCC
    string command ;
    string archfile ;

    if(ui->checkBox_archive->isChecked() )
    {
      archfile = gpath.calibration + "history/" + cal_filename_no_extension +"_till_" + date + ".txt";
      string command2 = "cp " +oldfile + " " +archfile ;
      int result = system(command2.c_str() );
      if(result)cout << "Problem with the command  system(cp...)"<< endl;
    }

    CCC
    // open the old_file,
    string old_name = gpath.calibration + cal_filename_with_extension ;
    ifstream plik_old(old_name.c_str()) ;
    if(!plik_old)
    {
      string mess =  "Can't open file " + old_name ;
      ui->lineEdit_install_messages->setText(mess.c_str());
      return ;
    }
    CCC
    // open the new_file
    // open the result_file,

    string new_name = gpath.calibration + cal_filename_no_extension+ "_new.txt" ;
    ofstream plik_new(new_name.c_str()) ;
    if(!plik_new)
    {
      string mess =  "Error: Can't open file " + new_name ;
      ui->lineEdit_install_messages->setText(mess.c_str());
      return ;
    }

    string result_name = gpath.autom + "list.out" ;
    ifstream plik_res(result_name.c_str()) ;
    if(!plik_res)
    {
      string mess =  "Error: Can't open file " + result_name ;
      ui->lineEdit_install_messages->setText(mess.c_str());
      return ;
    }
    CCC
    string keyword ;
    do
    {
      CCC
      // read the old file
      // when this is the comment -> copy this to new file
      copy_all_comments(plik_old, plik_new) ;

      // when this is the non comment - it must be a keyword
      plik_old >> keyword ;
      if(!plik_old)
        break;

      // write this key word to the new file
      plik_new << keyword ;
      CCC
      // search this keyword in the result file
      try
      {
        // read factor0.
        copy_all_comments(plik_old, plik_new) ;  // which were following keyword
        CCC
        // in the result file the keyword is different
        string res_keyword = keyword ;
        string::size_type pos = keyword.find("factors") ;
        CCC
        if(pos == string::npos)
        {
          cout << "Some error in the keyword " << keyword
          << " - I skip this keyword !" << endl;
          continue;

        }
        cout << " Succesfully processed  keyword " << keyword << endl;

        res_keyword.erase(pos) ;
        CCC
        res_keyword  += "raw.txt" ;

        CCC
        Nfile_helper::spot_in_file(plik_res, res_keyword) ;

        // if was found in the resultat file
        //   write the 3 coeff into the new file

        double factor0, factor1, factor2, dummy_factor ;

        plik_res >> factor0 ;
        plik_old >> dummy_factor ;
        plik_new << factor0 ;
        copy_all_comments(plik_old, plik_new) ;  // which were follwoing it
        CCC

        // read factor1.
        plik_res >> factor1 ;
        plik_old >> dummy_factor ;
        plik_new << factor1 ;
        copy_all_comments(plik_old, plik_new) ;  // which were follwoing it


        // read factor2.
        plik_res >> factor2 ;
        plik_old >> dummy_factor ;
        plik_new << factor2 ;
        copy_all_comments(plik_old, plik_new) ;  // which were follwoing it
      }

      catch(Tfile_helper_exception & k)  //   else copy the old coefficients into the new file
      {
        CCC
        // here we are if there is not such a keyword in the result file -
        // this means that we do not have new parameters for this keyword
        // just copy the old ones
        double tmp ;
        for(int i = 0 ; i < 3 ; i++)
        {
          plik_old >> tmp ;
          plik_new << tmp ;
          copy_all_comments(plik_old, plik_new) ;  // which were following it
        }
        CCC
      }
      CCC
    }
    while(plik_old) ;   // do is as long as the old file has data

    plik_old.close();
    plik_new.close();
    plik_res.close();
    CCC
    if(ui->checkBox_archive->isChecked() )
    {
      string archfile_new = gpath.calibration + "history/" +
                            cal_filename_no_extension + "_since_" + date + ".txt";
      command = "cp " + new_name  + " " +archfile_new ;
      int result = system(command.c_str());
      if(result)cout << "Problem with the command  system(cp...)"<< endl;
    }
    CCC
    // finally coping new into real, old one
    command = "cp " + new_name  + " " + old_name ;
    int res = system(command.c_str());

    string mess = "Succesfully installed into the file " + old_name ;
    if(res == -1)
    {
      mess = "Error while command " + command  ;
    }
    ui->lineEdit_install_messages->setText(mess.c_str());
    CCC
  }
  catch(...)
  {
    cout << "install - exception thrown " << endl;
  }
}
//*************************************************************************






////*************************************************************************
//void T4auto_calib_wizard::on_T4auto_calib_wizard_accepted()   // button NEXT
//{
//  QWizard::next() ;

//  // we are already on the new page
////  if(ui->title     (currentPage()) == "Install")
////  {
////    setFinishEnabled ( currentPage (), true) ;
////  }

////  setHelpEnabled (currentPage (), false) ;
////  helpButton () ->hide();
//}
//************************************************************************

//******************************************************************
void  T4auto_calib_wizard::copy_all_comments( ifstream &plik, ofstream &out )
{
  //plik.unset(ios::skipws) ;

  char c = 'x' ;
  do
  {
    //plik >> c  ;
    plik.read(&c, sizeof(char)) ;
    if (c == '/' && (plik.peek() == '/' ) )
    {
      string linia;
      getline(plik, linia);
      out << '/' << linia << "\n" ;
    }
    else if(isspace(c))
    {
      out << c ; // << "<-WS" ;
    }
    else
    {
      plik.putback(c) ;
      //plik.setf(ios::skipws) ;
      return ; // plik ;
    }
  }
  while(plik) ;
  return ; // plik ;

}
//***********************************************************************
bool  T4auto_calib_wizard::convert_one_spectrum( string binary_spec, string core_name )
{

  // cout << "convert_one_spectrum: " << binary_spec
  // << ", core_name = " << core_name
  // << endl;`

  ifstream specfile(binary_spec.c_str(), ios::binary) ;
  if(!specfile)
  {
    string mess =  "Can't open spectrum for reading " + binary_spec ;
    ui->textEdit_error_messages->setPlainText(mess.c_str()) ;
    return false ;
  }




#ifdef NIGDY
  // read into the program
  double left_edge ;
  specfile.read((char*) &left_edge, sizeof(left_edge));

  double bin_width;
  specfile.read((char*) &bin_width, sizeof(bin_width));
  vector<int> channel ;


  int n = 0 ;
  for(  ; specfile ; n++)
  {
    int value ;
    specfile.read((char*) &value, sizeof(value));
    if(!specfile)
    {
      break ;
    }
    channel.push_back(value) ;
  }


  // build the name of the output spectrum
  string ascii_spec = gpath.autom + core_name + ".txt" ;

  ui->textEdit_error_messages->setPlainText(ascii_spec.c_str()) ;

  // save into dir automatic as ascii 2 column
  ofstream plik_wyj(ascii_spec.c_str());
  if(!plik_wyj)
  {
    string mess =  "Error while opening the file " + ascii_spec ;
    ui->textEdit_error_messages->setPlainText(mess.c_str()) ;
    return ;
  }

  for(int i = 0 ; i < n ; i++, left_edge += bin_width )
  {
    plik_wyj << left_edge << "\t\t" << channel[i] << "\n" ;
    if(!plik_wyj)
    {
      string mess =  "Error while writing to the file " + ascii_spec ;
      ui->textEdit_error_messages->setPlainText(mess.c_str()) ;
      return ;
    }

  }
#endif
  //++++++++++++++++++++++++++++++++++++++++

  // read into the program
  double how_many_bins ;
  specfile.read((char*) &how_many_bins, sizeof(how_many_bins));

  double left_edge2;
  specfile.read((char*) &left_edge2, sizeof(left_edge2));

  double right_edge2;
  specfile.read((char*) &right_edge2, sizeof(right_edge2));


  vector<int> channel ;


  int n = 0 ;
  for(  ; specfile ; n++)
  {
    int value ;
    specfile.read((char*) &value, sizeof(value));
    if(!specfile)
    {
      break ;
    }
    channel.push_back(value) ;
  }


  // build the name of the output spectrum
  string ascii_spec = gpath.autom + core_name + ".txt" ;

  ui->textEdit_error_messages->setPlainText(ascii_spec.c_str()) ;

  // save into dir automatic as ascii 2 column
  ofstream plik_wyj(ascii_spec.c_str());
  if(!plik_wyj)
  {
    string mess =  "Error while opening the file " + ascii_spec ;
    ui->textEdit_error_messages->setPlainText(mess.c_str()) ;
    return false;
  }
  double bin_width = (right_edge2 - left_edge2 )/ how_many_bins;

  for(int i = 0 ; i < n ; i++, left_edge2 += bin_width )
  {
    plik_wyj << left_edge2 << "\t\t"
    << (left_edge2 >= channel_threshold ? channel[i] : 0)  // if we have some noise at the begining of spectrum
    << "\n" ;
    if(!plik_wyj)
    {
      string mess =  "Error while writing to the file " + ascii_spec ;
      ui->textEdit_error_messages->setPlainText(mess.c_str()) ;
      return false;
    }

  }

  plik_wyj.close() ;
  return true;
}
//**************************************************************************
void  T4auto_calib_wizard::type_of_det_changed()
{
  change_texts_rising_miniball();
  //   buttonGroup_symbols->setEnabled(radio_clusters->isChecked());
}
//**************************************************************************
void T4auto_calib_wizard::change_texts_rising_miniball()
{
  using namespace std;
  string invocation (
    "Installing the coefficients - means that the new coeffincients will be "
    "introduced into the <font color=\"#00aa00\"><tt>"
    "./calibration/rising_calibration.txt</tt></font> file (replacing the former ones)");

  string explanation("<p align=\"left\"><b>to archive - means:</b><br><br>a) the previous calibration "
                     "file into the subdirectory \"./calibration/history\" <br>"
                     "with the current file name appended with text<br>"
                     "<tt>rising_calibration_<font color=\"#ff0000\">till</font>_<i>"
                     "<font color=\"#0000ff\">current_date_time</font>"
                     "</i>.txt</tt><br><br>b) The new calibration will be stored as "
                     "<tt>rising_calibration_<font color=\"#ff0000\">since</font>_<i>"
                     "<font color=\"#0000ff\">current_date_time</font></i>.txt</tt></p></p>");
  string rising("rising");
  string miniball("miniball");



//  ui->textLabel_means->setText(invocation.c_str());
  ui->textLabel_archive->setText( explanation.c_str());
}
//*****************************************************************************
void T4auto_calib_wizard::on_pushButton_show_spectra_names_clicked()
{
  string text_spectra_names ;
  spectra_names.clear();

  // take the current skeleton
  string  skeleton = ui->lineEdit_skeleton->text().toStdString();
  // for  %1
  istringstream s1(ui->lineEdit_one->text().toStdString());

  vector<string> one ;
  for(int i = 0 ;  ; i++)
  {
    string wyraz ;
    s1 >>  wyraz ;
    if(!s1) break;
    one.push_back(wyraz);
  }
  // for %2
  istringstream s2(ui->lineEdit_two->text().toStdString() );
  vector<string> two ;
  for(int i = 0 ;  ; i++)
  {
    string wyraz ;
    s2 >>  wyraz ;
    if(!s2) break;
    two.push_back(wyraz);
  }

  // for %3
  istringstream s3(ui->lineEdit_three->text().toStdString() );
  vector<string> three ;
  for(int i = 0 ;  ; i++)
  {
    string wyraz ;
    s3 >>  wyraz ;
    if(!s3) break;
    three.push_back(wyraz);
  }

  if(one.empty()) one.push_back("");
  if(two.empty()) two.push_back("");
  if(three.empty()) three.push_back("");

  // make the list
  for(unsigned int i1 = 0 ; i1 < one.size() ; i1++)
    for(unsigned int i2 = 0 ; i2 < two.size() ; i2++)
      for(unsigned int i3 = 0 ; i3 < three.size() ; i3++)
      {

        string result = skeleton;
        size_t  poz = result.find("%1");
        if(poz != string::npos)
        {
          result.replace(poz, 2, one[i1] );
        }

        poz = result.find("%2");
        if(poz != string::npos)
        {
          result.replace(poz, 2, two[i2] );
        }

        poz = result.find("%3");
        if(poz != string::npos)
        {
          result.replace(poz, 2, three[i3] );
        }

        //cout << "result: " << result << endl;
        spectra_names.push_back(result);
        text_spectra_names += (result + "\n") ;
        // put the names into the vector of namespace
        // put in into the listbox
      }
  ui->textEdit_spectra_names->setText(text_spectra_names.c_str() );

//  ui->nextButton ()->show();
  //


  // restore the previous skeleton
  string n = gpath.autom + "last_skeleton";
  just_save_skeleton(n);

  //   ofstream   plik(n.c_str() );
  //
  //   if(plik)
  //   {
  //     string linia = ui->lineEdit_skeleton->text().ascii() ;
  //
  //     plik << linia << endl;
  //
  //     //saving the lines
  //
  //     linia =  ui->lineEdit_one->text().ascii() ;
  //     plik << linia << endl;
  //     linia =  ui->lineEdit_two->text().ascii() ;
  //     plik << linia << endl;
  //     linia =  ui->lineEdit_three->text().ascii() ;
  //     plik << linia << endl;
  //   }

}
//********************************************************************************

void T4auto_calib_wizard::on_pushButton_save_skeleton_clicked()
{
  // choosing filename

   Tplate_spectrum::flag_repainting_allowed = false;
  QString fileName =
    QFileDialog::getSaveFileName ( this,    // parent
                                   "Select the file to store your scheme for creating the spectra names",  // caption
                                   QString(gpath.autom.c_str()),   // dir
                                   QString("skeleton of spectra names   (*.skel)")
                                 );
 Tplate_spectrum::flag_repainting_allowed = true;
  // fileName->setMode( QFileDialog::AnyFile );
  //  if ( fd->exec() == QDialog::Accepted )
  if( ! fileName.isEmpty())
  {
    //    fileName = fd->selectedFile();

    //     cout << "Nazwa " << fileName << endl ;

    if( just_save_skeleton(fileName.toStdString() ))
    {
      return ; // true;
    }
  }
  return ; // false;

}
//*********************************************************************************
void T4auto_calib_wizard::on_pushButton_restore_skeleton_clicked()
{
  // choosing filename
  // choosing filename

   Tplate_spectrum::flag_repainting_allowed = false;
  QString fileName =
    QFileDialog::getOpenFileName ( this,
                                   "Select the file to restore your scheme for creating the spectra names",
                                   QString(gpath.autom.c_str()),
                                   QString("skeleton of spectra names   (*.skel)")
                                 );
  Tplate_spectrum::flag_repainting_allowed = true;

  // fileName->setMode( QFileDialog::AnyFile );
  //  if ( fd->exec() == QDialog::Accepted )
  if( ! fileName.isEmpty())
  {
    //    fileName = fd->selectedFile();

    //     cout << "Nazwa " << fileName << endl ;
    if( just_restore_skeleton(fileName.toStdString() ))
    {
      return ; // true;
    }
  }
  return  ; // false;

}
//************************************************************************************
void T4auto_calib_wizard::on_lineEdit_skeleton_textChanged(const QString &  /*arg1*/)
{
  on_lineEdit_skeleton_editingFinished() ;
}
//************************************************************************************
void T4auto_calib_wizard::on_lineEdit_skeleton_editingFinished()
{
      // clear the list of produced spectra
  ui->textEdit_spectra_names->setText("");

  // clear the next button
//  nextButton ()->hide();

  // clear the vector of names
  spectra_names.clear();

  // perhaps make the spectra immediately?
  on_pushButton_show_spectra_names_clicked();
}

//************************************************************************************
bool T4auto_calib_wizard::just_save_skeleton(string fname)
{
  // if there is no extension  *.skel

  string::size_type   pos = fname.rfind(".skel") ;
  //   cout << "pos = " << pos << endl;

  if(pos != fname.size() - 4)
  {
    fname += ".skel" ;
  }
  //   cout << "po korekcji nazwa = " << fname << endl;

  ofstream   plik(fname.c_str() );
  if(plik)
  {
    string linia = ui->lineEdit_skeleton->text().toStdString();

    plik << linia << endl;

    //saving the lines

    linia =  ui->lineEdit_one->text().toStdString();
    plik << linia << endl;
    linia =  ui->lineEdit_two->text().toStdString();
    plik << linia << endl;
    linia =  ui->lineEdit_three->text().toStdString();
    plik << linia << endl;

    plik.close();
    return true;
  }
  return false;
}
//************************************************************************************
bool T4auto_calib_wizard::just_restore_skeleton(string fname)
{
  ifstream   plik(fname.c_str() );

  if(plik)
  {
    string linia ;
    getline(plik, linia);
    //    plik >> linia;
    ui->lineEdit_skeleton->setText(linia.c_str() );

    //readin the lines

    linia = "" ;
    getline(plik, linia);
    ui->lineEdit_one->setText(linia.c_str());

    linia = "" ;
    getline(plik, linia);
    ui->lineEdit_two->setText(linia.c_str());

    linia = "" ;
    getline(plik, linia);
    ui->lineEdit_three->setText(linia.c_str());
    return true;
  }
  return false;
}
//*********************************************************************************
void T4auto_calib_wizard::on_lineEdit_one_textChanged(const QString & /*arg1*/)
{
    on_lineEdit_skeleton_editingFinished() ;
}
//*********************************************************************************
void T4auto_calib_wizard::on_lineEdit_two_textChanged(const QString &  /*arg1*/)
{
    on_lineEdit_skeleton_editingFinished() ;
}
//*********************************************************************************
void T4auto_calib_wizard::on_lineEdit_three_textChanged(const QString & /*arg1*/ )
{
    on_lineEdit_skeleton_editingFinished() ;
}
//*********************************************************************************
