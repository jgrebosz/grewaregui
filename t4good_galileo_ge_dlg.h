#ifndef T4GOOD_GALILEO_GE_DLG_H
#define T4GOOD_GALILEO_GE_DLG_H

#include <QDialog>

namespace Ui {
  class T4good_galileo_ge_dlg;
}

class T4good_galileo_ge_dlg : public QDialog
{
  Q_OBJECT

public:
  explicit T4good_galileo_ge_dlg(QWidget *parent = 0);
  ~T4good_galileo_ge_dlg();

private slots:
  void on_buttonBox_accepted();

  void on_push_default_clicked();

  //void on_pushline_energy_good_threshold_clicked();

  void on_pushline_energy_good_threshold_upper_clicked();

  void on_push_time_threshold_lower_clicked();

  void on_push_time_threshold_upper_clicked();

  void on_checkBox_good_energy_clicked(bool checked);

  void on_checkBox_good_time_clicked(bool checked);

  void on_pushline_energy_good_threshold_lower_clicked();

  void on_push_banana_clicked();

  void on_checkBox_wEn_vs_eEn_banana_toggled(bool checked);

protected:
    void init();

private:
  Ui::T4good_galileo_ge_dlg *ui;
};

#endif // T4GOOD_GALILEO_GE_DLG_H
