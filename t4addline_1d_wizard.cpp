#include "t4addline_1d_wizard.h"
#include "ui_t4addline_1d_wizard.h"

#include <QMessageBox>
#include <iostream>
using namespace std;


//**********************************************************************************************************
T4addline_1d_wizard::T4addline_1d_wizard(QWidget *parent) :
  QWizard(parent),
  ui(new Ui::T4addline_1d_wizard)
{
  ui->setupUi(this);
  // To avoid writing this in Polish
  setButtonText(QWizard::NextButton, "Next  >" );
  setButtonText(QWizard::BackButton, "<  Back" );
  setButtonText(QWizard::CancelButton, "Cancel" );
  setButtonText(QWizard::FinishButton, "Finish" );
}
//**********************************************************************************************************
T4addline_1d_wizard::~T4addline_1d_wizard()
{
  delete ui;
}
//**********************************************************************************************************
int T4addline_1d_wizard::set_incrementor( QString name )
{
  incrementor_name = name ;
  QString big_name = QString("<h3>%1</h3>").arg(name);

  ui->textLabel2_incr_name->setText(big_name);
  ui->textLabel2_incr_name_2->setText(big_name);
  ui->textLabel_incrementor_name->setText(big_name);

  // look at the keyword and judge what is approproate
  is_all = name.contains("ALL_", Qt::CaseSensitive);
  is_when = name.contains("_WHEN_", Qt::CaseInsensitive);

  //	cout << "is all detected as " << is_all
  //<< ", when detected as " << is_when
  //	<< endl;



  if(is_when && ! is_all)setStartId (page__keyword_WHEN);
  if(is_when &&  is_all)setStartId (page__keywords_ALL_WHEN);


  ui->radioButton_obligatory->setChecked(true) ;
  ui->radioButton_if_possible->setChecked(false) ;
  ui->radioButton_always_ALL->setChecked(false) ;
  ui->radioButton_if_possible_ALL->setChecked(true) ;

  //helpButton () ->hide();
  return 0 ; // have to return something
}
//***********************************************************************************************************************
int T4addline_1d_wizard::nextId() const
{

  //  if(is_all)showPage (0);
  //  else

//  cout << "I am on page nr" << currentId() << ", title: "
//       << currentPage()->title().toStdString() << endl;

  switch (currentId())
    {
    case page__keyword_WHEN:
      return page__what_was_chosen;

    case page__keywords_ALL_WHEN:
      return page__what_was_chosen;

    case  page__what_was_chosen:
      return -1;

    default :
      cout << "Shoul never be here!" << endl;
    }

  return -1;
}
//***********************************************************************************************************************
bool T4addline_1d_wizard::validateCurrentPage()
{
//  cout << "F. validateCurrentPage with the tile >>>>>>>> "
//       << currentPage()->title().toStdString()  << endl;

  // page__keyword_WHEN,  page__keywords_ALL_WHEN, page__what_was_chosen

  switch (currentId())
    {

    default:
    case  page__what_was_chosen: //  "What was chosen"
      {
        // setFinishEnabled ( currentPage (), true) ;

        if(is_all)
          {
            make_obligatory = ui->radioButton_always_ALL->isChecked() ;
          }
        else
          {
            make_obligatory = ui->radioButton_obligatory->isChecked() ;
          }
        ui->textLabel_when_to_test->setText(make_obligatory?
                                              "<h3>consider this line as false</h3>" :
                                              "<h3>consider this line as true</h3>" );

      }
    }

  return true;
}
//***********************************************************************************************************************
void T4addline_1d_wizard::on_pushButton_for_example_when_clicked()
{
  QMessageBox::information( this, "Why we need the 'WHEN'' keyword?\n\n",
                            "The position of the flying ion may not be possible to calculate.\n"
                            "for the current event\n"
                            "(because one of the electrodes didn't fire). \n\n"
                            "What value should have the incrementor 'multiwire_position_x' in such event ?"
                            "Setting this incrementer to an artificial value = 0  is not a good solution\n"
                            "(because the value 0 means for us just a perfect measurement of multiwire chamber)\n"
                            "In such a case - the incrementer in this event is marked as: 'not valid' \n"
                            "\nHere you have to decide how the condition algorytm should proceed with such incrementer");
}
//***********************************************************************************************************************
void T4addline_1d_wizard::on_pushButton_expl_all_when_clicked()
{
  QMessageBox::information( this, "Why we need the ALL, WHEN keywords?\n\n",
                            "It may happen for example when 'ALL'' means"
                            "the 'a set of all 105 germanium crystals'.\n"
                            "Of course it is very unlikely that 105 crystals will (fire) be present "
                            "in one event.\n"
                            "So some items may not be present.\n\n"
                            "Here - you have to decide how this gate (condition) should be evaluated in such istuation");
}
