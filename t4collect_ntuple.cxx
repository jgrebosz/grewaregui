#include "t4collect_ntuple.h"

#include <fstream>
#include "paths.h"
#include "Tfile_helper.h"
#include <qdir.h>
//*********************************************************************************************************
T4collect_ntuple::T4collect_ntuple(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);

    ui.check_create_ntuple->setChecked(false);
    // pixmapLabel_istotope->setEnabled(false);
    // check_only_istotopes->setEnabled(false);
    ui.check_create_raw_ntuple->setChecked(false);
    ui.comboBox_conditions->clear();
    ui.comboBox_conditions->addItem ("No_condition");

    // try to open the file with this option
    ifstream plik(( gpath.options + "collect_ntuple_file.option").c_str());
    if(!plik)
    {
        return ;
    }

    try{
        bool flag = (bool) Nfile_helper::find_in_file(plik, "collect_ntuple_file");
        ui.check_create_ntuple->setChecked(flag);

        string cond_name;

        Nfile_helper::spot_in_file(plik, "condition_name");
        plik >> cond_name;

        load_combo_conditions(cond_name);
        //  pixmapLabel_istotope->setEnabled(flag);
        //  check_only_istotopes->setEnabled(flag);


        /*
     flag = (bool) Nfile_helper::find_in_file(plik, "add_only_events_with_identified_isotopes");
     check_only_istotopes->setChecked(flag);

     flag = (bool) Nfile_helper::find_in_file(plik, "mw_must_be_present");
     checkBox_mw->setChecked(flag);

     flag = (bool) Nfile_helper::find_in_file(plik, "tof_must_be_present");
     checkBox_tof->setChecked(flag);

     flag = (bool) Nfile_helper::find_in_file(plik, "sci_pos_must_be_present");
     checkBox_sci->setChecked(flag);

     flag = (bool) Nfile_helper::find_in_file(plik, "cate_xy_must_be_present");
     checkBox_cate_xy->setChecked(flag);

     flag = (bool) Nfile_helper::find_in_file(plik, "cate_energy_must_be_present");
     checkBox_cate_energy->setChecked(flag);
   */
        // new type of data - STEP 1 RAW Ntuple
        flag = (bool) Nfile_helper::find_in_file(plik, "collect_raw_ntuple_file");
        ui.check_create_raw_ntuple->setChecked(flag);
    }
    catch(...)
    {

    }
    collect_state_changed();
}
//**************************************************************************************************
void T4collect_ntuple::changeEvent(QEvent *e)
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
//*************************************************************************
void T4collect_ntuple::accept()
{
    ofstream plik(( gpath.options + "collect_ntuple_file.option").c_str());
    plik << "collect_ntuple_file\t"
         << (ui.check_create_ntuple->isChecked() ?  1 : 0 )
         << "\ncondition_name\t"
         << ui.comboBox_conditions->currentText().toStdString() << user_cond_ext

            /*  << "\nadd_only_events_with_identified_isotopes\t"
          << (check_only_istotopes->isChecked() ?  1 : 0 )
          user->condition_name = comboBox_conditions->currentText().ascii();
          << "\nmw_must_be_present\t"
          <<  (checkBox_mw->isChecked() ?  1 : 0 )
          << "\ntof_must_be_present\t"
          <<  (checkBox_tof->isChecked() ?  1 : 0 )
          << "\nsci_pos_must_be_present\t"
          << (checkBox_sci->isChecked() ?  1 : 0 )
          << "\ncate_xy_must_be_present\t"
          <<    (checkBox_cate_xy->isChecked() ?  1 : 0 )
          << "\ncate_energy_must_be_present\t"
          <<  (checkBox_cate_energy->isChecked() ?  1 : 0 )
        */
         << "\n\ncollect_raw_ntuple_file\t"
         << (ui.check_create_raw_ntuple->isChecked()?  1 : 0 )

         << endl ;
    plik.close() ;

    QDialog::accept();
}
//***********************************************************************
//***********************************************************************
void T4collect_ntuple::on_button_default_clicked()    // make_default()
{
    ui.check_create_ntuple->setChecked(false);
     ui.check_create_raw_ntuple->setChecked(false);
    collect_state_changed();
}
//***********************************************************************
void T4collect_ntuple::collect_state_changed()
{
    bool flag = ui.check_create_ntuple->isChecked();
    ui.textLabel_condition->setEnabled(flag);
    ui.comboBox_conditions->setEnabled(flag);

}
//***********************************************************************
void T4collect_ntuple::on_check_create_ntuple_stateChanged(int /*arg1*/)
{
     collect_state_changed();
}

//***********************************************************************
void T4collect_ntuple::load_combo_conditions(string last_name)
{

    QDir katalog(gpath.Qconditions()) ;
    QStringList lista_cond = katalog.entryList ( QStringList(user_cond_filter.c_str())) ;

    ui.comboBox_conditions->clear();
    ui.comboBox_conditions->addItem ("No_condition");
    int nr = 1 ;  // bec "No condition" is on the 0 position
    int current = 0 ;

    for ( QStringList::Iterator it = lista_cond.begin() ; it != lista_cond.end() ;  ++it, nr++ )
    {
        //    cout << "now1 =" << *it << "\n";

        QString nam = *it ;
        if(last_name == nam.toStdString())
        {
            current = nr;
        }
        // removing the extension
        int i = nam.lastIndexOf( user_cond_ext.c_str() );
        nam.truncate(i);
        ui.comboBox_conditions->addItem (nam);
    }
    ui.comboBox_conditions->setCurrentIndex(current);
}
//*********************************************************************
