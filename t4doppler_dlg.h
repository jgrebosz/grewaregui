#ifndef T4DOPPLER_DLG_H
#define T4DOPPLER_DLG_H

#include <QDialog>

namespace Ui {
class T4doppler_dlg;
}
//////////////////////////////////////////////////////////////////////////////////////////////
class T4doppler_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit T4doppler_dlg(QWidget *parent = 0);
    ~T4doppler_dlg();

protected:

    void set_parameters( int mw, int lycca, int algorithm);
    void init();
    void store_parameters();
    void set_energy_deposit();
    void set_beta();
    void set_beta_radio_chosen();
    void make_picture_small();
    void beta_payfield();
    void calculator_from_Tpn();



    double beta_fixed_value;
    double energy_deposit_in_target ;
    int beta_from_where;
    enum { beta_type_frs = 1, beta_type_fixed};

private slots:
    void on_push_minimise_clicked();
    void accept();

    void on_push_beta_clicked();

//    void on_buttonGroup3_clicked(bool checked);

    void on_radio_tracked_algorithm_toggled(bool checked);

    void on_radio_beta_manually_toggled(bool checked);

    void on_push_other_betas_clicked();

    void on_push_Tpn_clicked();

    void on_push_Ed_clicked();

private:
    Ui::T4doppler_dlg *ui;
};

#endif // T4DOPPLER_DLG_H
