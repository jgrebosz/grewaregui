#ifndef T4COLLECT_NTUPLE_H
#define T4COLLECT_NTUPLE_H

#include "ui_t4collect_ntuple.h"
#include <string>
using namespace  std;


class T4collect_ntuple : public QDialog
{
    Q_OBJECT

public:
    explicit T4collect_ntuple(QWidget *parent = 0);




protected:
    void changeEvent(QEvent *e);
    void make_default();
    void collect_state_changed();
    void load_combo_conditions(string last_name);

private slots:
    void on_button_default_clicked();
     void accept();

     void on_check_create_ntuple_stateChanged(int arg1);

private:
    Ui::T4collect_ntuple ui;
};

#endif // T4COLLECT_NTUPLE_H
