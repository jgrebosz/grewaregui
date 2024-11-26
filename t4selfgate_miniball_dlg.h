#ifndef T4SELFGATE_MINIBALL_DLG_H
#define T4SELFGATE_MINIBALL_DLG_H

#include <QDialog>
#include "Tself_gate_mib_descr.h"



namespace Ui {
class T4selfgate_miniball_dlg;
}
////////////////////////////////////////////////////////////////////////////
class T4selfgate_miniball_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit T4selfgate_miniball_dlg(QWidget *parent = 0);
    ~T4selfgate_miniball_dlg();
    void set_parameters( const Tself_gate_mib_descr *d );
    void get_parameters( Tself_gate_mib_descr * d );

private slots:
    void on_pushButton_angle_picture_clicked();
    void accept();


    void on_checkBox_core_energy_clicked();

    void on_checkBox_time_clicked();

    void on_checkBox_geom_theta_clicked();

    void on_checkBox_geom_phi_clicked();

    void on_checkBox_gp_theta_clicked();

    void on_checkBox_gp_phi_clicked();

    void on_checkBox_mult_clicked();

protected:
    void update_checked_unchecked();

private:
    Ui::T4selfgate_miniball_dlg *ui;
};

#endif // T4SELFGATE_MINIBALL_DLG_H
