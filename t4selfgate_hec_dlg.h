#ifndef T4SELFGATE_HEC_DLG_H
#define T4SELFGATE_HEC_DLG_H

#include <QDialog>

class Tself_gate_hec_descr;

namespace Ui {
class T4selfgate_hec_dlg;
}
/////////////////////////////////////////////////////////////////////////////
class T4selfgate_hec_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit T4selfgate_hec_dlg(QWidget *parent = 0);
    ~T4selfgate_hec_dlg();

    void set_parameters( const Tself_gate_hec_descr *d );
    void get_parameters( Tself_gate_hec_descr * d );
protected:

    //void show_picture_gp();
    void init();
    void update_checked_unchecked();
    void accept();

private slots:

    void on_pushButton_OK_clicked();
    void on_pushButton_Cancel_clicked();

    void on_checkBox_energy_fast_clicked();

    void on_checkBox_energy_slow_clicked();

    void on_checkBox_time_clicked();

    void on_checkBox_geom_theta_clicked();

    void on_checkBox_geom_phi_clicked();

    void on_checkBox_gp_theta_clicked();

    void on_checkBox_gp_phi_clicked();

    void on_pushButton_picture_clicked();

    void on_push_banana_clicked();

    void on_checkBox_fast_slow_banana_clicked(bool checked);

private:
    Ui::T4selfgate_hec_dlg *ui;
};

#endif // T4SELFGATE_HEC_DLG_H
