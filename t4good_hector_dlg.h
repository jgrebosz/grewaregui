#ifndef T4GOOD_HECTOR_DLG_H
#define T4GOOD_HECTOR_DLG_H

#include <QDialog>

namespace Ui {
class T4good_hector_dlg;
}

class T4good_hector_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit T4good_hector_dlg(QWidget *parent = 0);
    ~T4good_hector_dlg();

private slots:
    void on_push_default_clicked();

    void on_checkBox_good_enFAST_clicked(bool checked);

    void on_checkBox_good_enSLOW_clicked(bool checked);

    void on_checkBox_good_time_clicked(bool checked);

    void on_push_LED_Adc_clicked();

    void on_push_adc_low_clicked();

    void on_push_adc_high_clicked();

    void on_push_LED_Tdc_clicked();

    void on_push_tdc_low_clicked();

    void on_push_tdc_high_clicked();

    void on_pushline_enFAST_good_threshold_clicked();

    void on_pushline_enSLOW_good_threshold_clicked();

    void on_push_time_threshold_clicked();



protected:
    void init();
    void accept();
    void set_default();
    void set_en20_threshold();
    void set_en4_thresh();
    void set_time_threshold();
    void set_led_adc();
    void set_led_tdc();
    void set_adc_low();
    void set_adc_high();
    void set_tdc_low();
    void set_tdc_high();


    int led_adc_nr;
      int led_tdc_nr;
      int led_adc_low;
      int led_adc_high;
      int led_tdc_low;
      int led_tdc_high;

private:
    Ui::T4good_hector_dlg *ui;
};

#endif // T4GOOD_HECTOR_DLG_H
