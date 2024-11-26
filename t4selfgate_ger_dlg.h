#ifndef T4SELFGATE_GER_DLG_H
#define T4SELFGATE_GER_DLG_H

#include <QDialog>


#include "Tself_gate_ger_descr.h"

namespace Ui {
class T4selfgate_ger_dlg;
}
/////////////////////////////////////////////////////////////////////////////////////////////
class T4selfgate_ger_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit T4selfgate_ger_dlg(QWidget *parent = 0);
    ~T4selfgate_ger_dlg();

    void set_parameters( const Tself_gate_ger_descr *d );
    void get_parameters( Tself_gate_ger_descr * d );

private slots:
    //void on_push_banana_4_clicked();
    void accept();
    void update_checked_unchecked();

    void on_checkBox_en4_clicked();
    void on_checkBox_en20_clicked();
    void on_checkBox_time_clicked();
    void on_check_xy_difference_clicked();
    void on_checkBox_LR_time_clicked();
    void on_checkBox_SR_time_clicked();
    void on_checkBox_en_tim_banana_clicked();
    void on_checkBox_geom_theta_clicked();
    void on_checkBox_geom_phi_clicked();
    void on_checkBox_gp_theta_clicked();
    void on_checkBox_gp_phi_clicked();
    void on_checkBox_mult_clicked();
    void on_checkBox_AC_clicked();

    void on_push_banana_clicked();
    void on_pushButton_angle_picture_clicked();
    void on_push_xy_info_clicked();

protected:
    void init();

    void info_xy_selfgate();
    void change_text_of_time( bool state );
    void choosing_banana();
    void time_chooser_pressed();
    void show_picture_gp();



bool this_is_germanium;

//enum defined in the other class
Tself_gate_ger_descr::which_time_for_banana    chosen_time;

private:
    Ui::T4selfgate_ger_dlg *ui;
};

#endif // T4SELFGATE_GER_DLG_H
