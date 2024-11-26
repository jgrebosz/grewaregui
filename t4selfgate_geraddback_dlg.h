#ifndef T4SELFGATE_GERADDBACK_DLG_H
#define T4SELFGATE_GERADDBACK_DLG_H

#include <QDialog>

#include "Tself_gate_ger_addback_descr.h"

namespace Ui {
class T4selfgate_geraddback_dlg;
}
/////////////////////////////////////////////////////////////////////////////////
class T4selfgate_geraddback_dlg : public QDialog
{
    Q_OBJECT

public:
	explicit T4selfgate_geraddback_dlg(QWidget *parent = nullptr);
    ~T4selfgate_geraddback_dlg();
    void  set_parameters( const Tself_gate_ger_addback_descr *d );
    void  get_parameters( Tself_gate_ger_addback_descr * d );
public slots:
    void  update_checked_unchecked();


private slots:
    //void  update_checked_unchecked();
    void on_checkBox_en4_clicked();

    void on_checkBox_time_clicked();

    void on_checkBox_LR_time_clicked();

    void on_checkBox_SR_time_clicked();

    void on_checkBox_en_tim_banana_clicked();

    void on_checkBox_geom_theta_clicked();

    void on_checkBox_geom_phi_clicked();

    void on_checkBox_geom_phi_secondary_clicked();

    void on_checkBox_geom_theta_secondary_clicked();
    void  accept();

    void on_checkBox_AC_clicked();

    void on_push_time_chooser_clicked();

    void on_push_banana_clicked();

protected:
    void  init();
    void  choose_banana();
    void  time_chooser_pressed();



private:
    Ui::T4selfgate_geraddback_dlg *ui;

    Tself_gate_ger_addback_descr::which_time_for_banana chosen_time;

};

#endif // T4SELFGATE_GERADDBACK_DLG_H
