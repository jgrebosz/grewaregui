#ifndef T4selfgate_kratta_2dlg_H
#define T4selfgate_kratta_2dlg_H

#include <QDialog>

class Tself_gate_kratta_descr;

namespace Ui {
class T4selfgate_kratta_2dlg;
}
/////////////////////////////////////////////////////////////////////////////
class T4selfgate_kratta_2dlg : public QDialog
{
    Q_OBJECT

public:
    explicit T4selfgate_kratta_2dlg(QWidget *parent = 0);
    ~T4selfgate_kratta_2dlg();

    void set_parameters( const Tself_gate_kratta_descr *d );
    void get_parameters( Tself_gate_kratta_descr * d );
protected:

    //void show_picture_gp();
    void init();
    void update_checked_unchecked();
    void accept();

private slots:

    void on_pushButton_OK_clicked();
    void on_pushButton_Cancel_clicked();

//    void on_checkBox_energy_fast_clicked();

//    void on_checkBox_energy_slow_clicked();

//    void on_checkBox_time_clicked();

    void on_checkBox_geom_theta_clicked();

    void on_checkBox_geom_phi_clicked();

    void on_checkBox_gp_theta_clicked();

    void on_checkBox_gp_phi_clicked();

    void on_pushButton_picture_clicked();


    void on_checkBox_energy_pd0_clicked();

    void on_checkBox_energy_pd1_clicked();

    void on_checkBox_energy_pd2_clicked();


    void on_checkBox_time_pd0_clicked();

    void on_checkBox_time_pd1_clicked();

    void on_checkBox_time_pd2_clicked();


    void on_checkBox_pd0_pd1_banana_toggled(bool);

    void on_checkBox_pd1_pd2_banana_toggled(bool);

    void on_push_pd0_pd1_banana_clicked();

    void on_push_pd1_pd2_banana_clicked();

private:
    Ui::T4selfgate_kratta_2dlg *ui;
};

#endif // T4selfgate_kratta_2dlg_H
