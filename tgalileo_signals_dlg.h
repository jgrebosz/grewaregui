#ifndef TGALILEO_SIGNALS_DLG_H
#define TGALILEO_SIGNALS_DLG_H

#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <vector>
#include <string>
using namespace std;
#include <Tfile_line_det_cal_lookup.h>


namespace Ui {
class TGalileo_signals_dlg;
}

/////////////////////////////////////////////////////////////////////////////////////
class TGalileo_signals_dlg : public QDialog
{
    Q_OBJECT
vector<Tfile_line_det_cal_lookup> &fline;
public:
    explicit TGalileo_signals_dlg(vector<Tfile_line_det_cal_lookup> &fline_, QWidget *parent = 0 );
    ~TGalileo_signals_dlg();
    void setup(std::string title,
               string kolory, string label_first_threshold_, string label_second_threshold_,
               string first_calibration_txt,
               string second_calibration_txt);





private slots:
    void on_CheckBox_00_clicked();

    void on_CheckBox_01_clicked();

    void on_CheckBox_02_clicked();

    void on_CheckBox_03_clicked();

    void on_CheckBox_04_clicked();

    void on_CheckBox_05_clicked();

    void on_CheckBox_06_clicked();

    void on_CheckBox_07_clicked();

    void on_CheckBox_08_clicked();

    void on_CheckBox_09_clicked();

    void on_CheckBox_10_clicked();

    void on_CheckBox_11_clicked();

    void on_CheckBox_12_clicked();

    void on_CheckBox_13_clicked();

    void on_CheckBox_14_clicked();

    void on_CheckBox_15_clicked();

    void on_CheckBox_16_clicked();

    void on_CheckBox_17_clicked();

    void on_CheckBox_18_clicked();

    void on_CheckBox_19_clicked();

    void on_CheckBox_20_clicked();

    void on_CheckBox_21_clicked();

    void on_CheckBox_22_clicked();

    void on_CheckBox_23_clicked();

    void on_CheckBox_24_clicked();

    void on_CheckBox_25_clicked();

    void on_CheckBox_26_clicked();

    void on_CheckBox_27_clicked();

    void on_CheckBox_28_clicked();

    void on_CheckBox_29_clicked();

    void on_CheckBox_30_clicked();

    void on_CheckBox_31_clicked();

    void on_pushButton_00_clicked();

    void on_pushButton_01_clicked();

    void on_pushButton_02_clicked();

    void on_pushButton_03_clicked();

    void on_pushButton_04_clicked();

    void on_pushButton_05_clicked();

    void on_pushButton_06_clicked();

    void on_pushButton_07_clicked();

    void on_pushButton_08_clicked();

    void on_pushButton_09_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_28_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_30_clicked();

    void on_pushButton_31_clicked();

    void on_buttonBox_accepted();

    //void on_tabWidget_currentChanged(int index);

    void on_tabWidget_currentChanged(int index);

private:
    Ui::TGalileo_signals_dlg *ui;

    std::vector<QPushButton*> pushButtons;
    std::vector<QLabel*> labels;
    std::vector<QLabel*> specnames;
    std::vector<QCheckBox*> radios;

    std::vector<bool> enables;
    //std::vector<std::vector<double> >  cal_factors;
    vector<string> page_colors

    {  /* " background-color : rgb(230,230,230)",*/
        " background-color : rgb(230,230,255)",
        " background-color : rgb(230,255,230)",
        " background-color : rgb(230,255,255)",
        " background-color : rgb(255,230,230)",
        " background-color : rgb(255,230,255)",
        " background-color : rgb(230,255,255)",
        " background-color : rgb(255,250,255)"
    };

string first_calibration_tekst;
string second_calibration_tekst;

string label_first_threshold;
string label_second_threshold;


    void update_enable_disable();
    void display_next_tab_data(int tab_nr);

    void edit_calib_factors(int nr);
public:
    bool give_enable(unsigned int nr) { return enables[nr]; }

};

#endif // TGALILEO_SIGNALS_DLG_H
