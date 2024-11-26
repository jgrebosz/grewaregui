#ifndef T4IMPORTANT_DLG_H
#define T4IMPORTANT_DLG_H

#include "ui_t4important_dlg.h"
#include <time.h>
//*****************************************************************************
class T4important_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit T4important_dlg(QWidget *parent = 0);
 bool may_I_alarm_now();
   void set_header( QString name );
    void set_parameter( QString message, QString filename, bool *flag_on_screen);

protected:
    void changeEvent(QEvent *e);


    //void T4important_dlg::init();


    void accept();

    void no_alarm10minutes();



    time_t time_of_no_alarm;
    bool *plt_flag_on_screen;
    QString file_of_message ;

private:
    Ui::T4important_dlg ui;
};

#endif // T4IMPORTANT_DLG_H
