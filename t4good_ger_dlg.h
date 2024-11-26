#ifndef T4GOOD_GER_DLG_H
#define T4GOOD_GER_DLG_H

#include <QDialog>

namespace Ui {
class T4good_ger_dlg;
}

class T4good_ger_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit T4good_ger_dlg(QWidget *parent = 0);
    ~T4good_ger_dlg();

private slots:
    void on_pushline_en20_good_threshold_clicked();
    void on_pushline_en4_good_threshold_clicked();
    void on_pushline_en4_good_threshold_upper_clicked();
    void on_push_time_threshold_lower_clicked();
    void on_push_time_threshold_upper_clicked();

    void on_push_default_clicked();

    void on_checkBox_good_en20_clicked(bool checked);

    void on_checkBox_good_en4_clicked(bool checked);

    void on_checkBox_good_time_clicked(bool checked);

protected:
    void init();
    void accept();
    void set_default();

private:
    Ui::T4good_ger_dlg *ui;
};

#endif // T4GOOD_GER_DLG_H
