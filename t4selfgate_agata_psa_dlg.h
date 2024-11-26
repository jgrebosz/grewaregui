#ifndef T4SELFGATE_AGATA_PSA_DLG_H
#define T4SELFGATE_AGATA_PSA_DLG_H

#include <QDialog>
#include "Tself_gate_agata_psa_descr.h"

namespace Ui {
class T4selfgate_agata_psa_dlg;
}
////////////////////////////////////////////////////////////////////////////////
class T4selfgate_agata_psa_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit T4selfgate_agata_psa_dlg(QWidget *parent = 0);
    ~T4selfgate_agata_psa_dlg();
    void set_parameters( const Tself_gate_agata_psa_descr *d );
    void get_parameters( Tself_gate_agata_psa_descr * d );

private slots:
    void on_checkBox_energy_clicked();
    void on_checkBox_time_clicked();
    void on_checkBox_x_clicked();
    void on_checkBox_y_clicked();
    void on_checkBox_z_clicked();
    void on_checkBox_mult_clicked();
    void accept();

private:
    Ui::T4selfgate_agata_psa_dlg *ui;

    void update_checked_unchecked();

};

#endif // T4SELFGATE_AGATA_PSA_DLG_H
