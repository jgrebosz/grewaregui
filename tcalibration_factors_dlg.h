#ifndef TCALIBRATION_FACTORS_DLG_H
#define TCALIBRATION_FACTORS_DLG_H

#include <QDialog>
#include <vector>

#include "Tfile_line_det_cal_lookup.h"

namespace Ui {
class Tcalibration_factors_dlg;
}

class Tcalibration_factors_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit Tcalibration_factors_dlg(QWidget *parent = 0)
       ;
    ~Tcalibration_factors_dlg();
    void setup(Tfile_line_det_cal_lookup line,
               string first_thr_txt, string second_thr_txt,
               string first_cal_txt, string second_cal_txt);

    Tfile_line_det_cal_lookup line_local;

private slots:

    void on_spinBox_degree_valueChanged(int degree);

    void on_tableWidget_cellChanged(int row, int column);

    void on_buttonBox_accepted();

    void on_spinBox_degree_2_valueChanged(int degree);

    void on_tableWidget_2_cellChanged(int row, int column);

private:
    Ui::Tcalibration_factors_dlg *ui;



};

#endif // TCALIBRATION_FACTORS_DLG_H
