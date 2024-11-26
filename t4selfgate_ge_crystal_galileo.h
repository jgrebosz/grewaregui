#ifndef T4SELFGATE_GE_crystal_GALILEO_H
#define T4SELFGATE_GE_crystal_GALILEO_H

#include <QDialog>

#include "Tself_gate_ge_galileo_descr.h"


//class Tself_gate_ge_galileo_descr;
//-----------------------------------------------------------
namespace Ui {
  class T4selfgate_ge_galileo;
}
/////////////////////////////////////////////////////////////////////////////
class T4selfgate_ge_galileo : public QDialog
{
  Q_OBJECT

public:
  explicit T4selfgate_ge_galileo(QWidget *parent = 0);
  ~T4selfgate_ge_galileo();
  void set_parameters(const Tself_gate_ge_galileo_descr *d );
  void get_parameters( Tself_gate_ge_galileo_descr * d );
  void accept();
  void info_xy_selfgate();


private slots:
  void on_checkBox_en4_clicked();

  void on_checkBox_time_clicked();

  void on_check_xy_difference_clicked();

  void on_push_xy_info_pressed();

  void on_checkBox_en_tim_banana_clicked();

  void on_push_banana_pressed();

  void on_checkBox_geom_theta_clicked();

  void on_checkBox_geom_phi_clicked();

  void on_checkBox_gp_theta_clicked();

  void on_checkBox_gp_phi_clicked();

  void on_pushButton_angle_picture_pressed();

  void on_checkBox_AC_energy_clicked();

  void on_checkBox_AC_time_clicked();

  void on_radioButton_Anticoincidence_toggled(bool checked);

  void on_radioButton_coincidence_toggled(bool checked);

private:
  Ui::T4selfgate_ge_galileo *ui;

  void init();
  void update_checked_unchecked();
  void change_text_of_time( bool state );
  void choosing_banana();
  void show_picture_gp();


};

#endif // T4SELFGATE_GE_crystal_GALILEO_H
