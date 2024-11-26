#ifndef T4VERBOSE_DLG_H
#define T4VERBOSE_DLG_H

#include <QDialog>

namespace Ui {
class T4verbose_dlg;
}
//************************************************************************************
class T4verbose_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit T4verbose_dlg(QWidget *parent = 0);
    ~T4verbose_dlg();

    void accept();
    void show_help();

private slots:
    void on_checkBox_enable_verbose_clicked(bool checked);

    void on_buttonHelp_clicked();

    void on_buttonCancel_clicked();

private:
    Ui::T4verbose_dlg *ui;
    void init();

};

#endif // T4VERBOSE_DLG_H
