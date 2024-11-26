#ifndef T4GOOD_KRATTA_DLG_H
#define T4GOOD_KRATTA_DLG_H

#include <QDialog>

namespace Ui {
class T4good_kratta_dlg;
}

class T4good_kratta_dlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit T4good_kratta_dlg(QWidget *parent = 0);
    ~T4good_kratta_dlg();
    
private slots:
    void on_pushButton_pd0_low_clicked();
    void on_pushButton_pd0_high_clicked();
    void on_checkBox_pd0_toggled(bool checked);
    void on_checkBox_pd1_toggled(bool checked);
    void on_pushButton_pd1_low_clicked();
    void on_pushButton_pd1_high_clicked();
    void on_checkBox_pd2_toggled(bool checked);
    void on_pushButton_pd2_low_clicked();
    void on_pushButton_pd2_high_clicked();

    void on_buttonBox_accepted();

private:
    Ui::T4good_kratta_dlg *ui;
};

#endif // T4GOOD_KRATTA_DLG_H
