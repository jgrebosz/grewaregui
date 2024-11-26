#include "t4important_dlg.h"

#include <iostream>
#include <unistd.h>
#include <qapplication.h>
//*************************************************************************

T4important_dlg::T4important_dlg(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);

    time_of_no_alarm = 0 ;

}
//*************************************************************************
void T4important_dlg::changeEvent(QEvent *e)
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
//**************************************************************************
void T4important_dlg::set_parameter( QString message, QString filename, bool *flag_on_screen)
{
  ui.textEdit->setText(message);
  file_of_message = filename ;
  plt_flag_on_screen = flag_on_screen ;
  QApplication::beep ();
  QApplication::beep ();
  QApplication::beep ();
}
//**************************************************************************
void T4important_dlg::accept()
{
  // remove the file
  hide();
  unlink(file_of_message.toStdString().c_str() );
  *plt_flag_on_screen = false ;
  QDialog::accept();
}
//**************************************************************************
void T4important_dlg::set_header( QString name )
{
  QString ttt = "<p align=\"center\"><font color=\"#ff0000\" size=\"+4\"><b>"
                + name +   "</b></font></p>" ;
  ui.textLabel_mess->setText(ttt) ;
}
//**************************************************************************
void T4important_dlg::no_alarm10minutes()
{
  time_of_no_alarm = time(NULL);
  ui.pushButton2->setEnabled(false );
  accept();
}
//**************************************************************************
bool T4important_dlg::may_I_alarm_now()
{
  if(time(NULL) >= time_of_no_alarm + 600)
  {
    ui.pushButton2->setEnabled(true );
    return true;
  }
  else
    return false;
}
//**************************************************************************
