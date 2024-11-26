#ifndef T4SETTING_TIMES_DLG_H
#define T4SETTING_TIMES_DLG_H

#include <QDialog>

namespace Ui {
class T4setting_times_dlg;
}

class T4setting_times_dlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit T4setting_times_dlg(QWidget *parent = 0);
    ~T4setting_times_dlg();

    void give_times( int * time_1D, int *time_2D, int * time_sum, bool * flag );
    void set_times( int time_1D, int time_2D, int time_sum, bool flag );
    
private slots:
    void on_push_default_values_clicked();

private:
    Ui::T4setting_times_dlg *ui;
};

#endif // T4SETTING_TIMES_DLG_H
