#ifndef T4GOOD_MINIBAL_H
#define T4GOOD_MINIBAL_H

#include <QDialog>

namespace Ui {
class T4good_minibal;
}

class T4good_minibal : public QDialog
{
    Q_OBJECT

public:
    explicit T4good_minibal(QWidget *parent = 0);
    ~T4good_minibal();

protected:
    void accept();
    void set_default();
    void set_en4_thresh();
    void set_time_threshold();
    void init();



private slots:
    void on_push_default_clicked();

    void on_pushline_core_energy_good_threshold_clicked();

    void on_push_time_threshold_clicked();

    void on_checkBox_good_core_energy_clicked(bool checked);

    void on_checkBox_good_time_clicked(bool checked);

private:
    Ui::T4good_minibal *ui;
};

#endif // T4GOOD_MINIBAL_H
