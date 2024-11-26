#include "t4addback_dlg.h"
#include <iostream>
using namespace std ;
#include "Tfile_helper.h"
#include "paths.h"
#include <fstream>

//******************************************************************************************************
T4addback_dlg::T4addback_dlg(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    init();
}
//******************************************************************************************************
void T4addback_dlg::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui.retranslateUi(this);
        break;
    default:
        break;
    }
}
//*********************************************************************************************************
void T4addback_dlg::accept()
{
    ofstream plik(( gpath.options + "addback.option").c_str());

    plik
                << "addback_on\t" << (ui.checkBox_Addback_on->isChecked() ? 1 : 0)
                << "\nmax_fold\t"
                     << ui.spinBox_maxfold->value()

       << "\nadjacent_mode\t"
         << (ui.radioButton_adjacent->isChecked()?  1 : 0 )
       << "\nnonadjacent_mode\t"
         << (ui.radioButton_non_adjacent->isChecked()?  1 : 0 )
       << "\nsum_all_mode\t"
         << (ui.radioButton_all_segments->isChecked()?  1 : 0 )

         << "\naddback_option_max_acceptable_time_diff_is_enabled\t"
         << (ui.checkBox_time_limit->isChecked()?  1 : 0 )
         << "\naddback_option_max_acceptable_time_diff_to_the_fastest\t" << ui.spinBox_time_dfii_max->value()
         << "\nprimary_seg_algorithm\t"
         <<  (ui.radioButton_max20->isChecked() ?  1 : 0 )
         << "\nEprimary_threshold\t" << ui.spinBox_e_primary->value()
                 << endl ;
       plik.close() ;

       QDialog::accept();
}
//**************************************************************************
void T4addback_dlg::init()
{
      ui.radioButton_adjacent->setChecked(true);
      ui.spinBox_maxfold->setValue(2);
      ui.radioButton_max20->setChecked(1);
      ui.checkBox_Addback_on->setChecked(0);
      ui.groupBox_parameters->setEnabled(0);
      ui.spinBox_e_primary->setValue(0);
      ui.checkBox_time_limit->setChecked(false);
      ui.buttonHelp->hide();
  // try to open the file with this option
    ifstream plik(( gpath.options + "addback.option").c_str());
    if(!plik)
    {
      cout << "Cant open addback options file" << endl;
      return ;
    }

    try{
      bool flag = (bool) Nfile_helper::find_in_file(plik, "addback_on") ;
      ui.checkBox_Addback_on->setChecked(flag);
      ui.groupBox_parameters->setEnabled(flag);


      int max_fold = (int) Nfile_helper::find_in_file(plik, "max_fold");
      // cout << "Read fold =" << max_fold << endl;
      ui.spinBox_maxfold->setValue(max_fold);

      flag = (bool) Nfile_helper::find_in_file(plik, "adjacent_mode");
      ui.radioButton_adjacent->setChecked(flag);

      flag = (bool) Nfile_helper::find_in_file(plik, "nonadjacent_mode");
      ui.radioButton_non_adjacent->setChecked(flag);

      flag = (bool) Nfile_helper::find_in_file(plik, "sum_all_mode");
      ui.radioButton_all_segments->setChecked(flag);


      int primary_segment_algorithm= (int) Nfile_helper::find_in_file(plik, "primary_seg_algorithm");
      ui.radioButton_max20->setChecked(primary_segment_algorithm);

       int Eprimary_threshold = (int)
                                        Nfile_helper::find_in_file(plik, "Eprimary_threshold");
       ui.spinBox_e_primary->setValue(Eprimary_threshold);

     // max time allowed

         flag = (bool) Nfile_helper::find_in_file(plik, "addback_option_max_acceptable_time_diff_is_enabled");
              ui.checkBox_time_limit->setChecked(flag);


     ui.spinBox_time_dfii_max->setValue(
         (int) 	Nfile_helper::find_in_file(plik, "addback_option_max_acceptable_time_diff_to_the_fastest"));


    }
    catch(...)
    {
        cout << "Exception while reading addback option file " << endl;
    }

    limit_clicked(); // enable/disable some lines of dialog
}
//****************************************************************************
void T4addback_dlg::show_help()
{
/*The addback energy can be Doppler corrected, but
there is a problem:  which angle to use.
We need to assume which was the first, primary crystal
of the cluster. (In other words: which crystal registered
the oryginal gamma quantum comming from the target).</i>
*/
}
//****************************************************************

void T4addback_dlg::limit_clicked()
{
  bool stan = ui.checkBox_time_limit->isChecked() ;
// setting the enable, disable limit data
  ui.textLabel2->setEnabled(stan);
     ui. textLabel3->setEnabled(stan);
      ui.spinBox_time_dfii_max->setEnabled(stan);

}
//****************************************************************************
void T4addback_dlg::on_checkBox_Addback_on_clicked(bool checked)
{
   ui.groupBox_parameters ->setEnabled(checked);
}
