#ifndef T4ADDBACK_DLG_H
#define T4ADDBACK_DLG_H

#include "ui_t4addback_dlg.h"

class T4addback_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit T4addback_dlg(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);

    void accept();
    void init();
    void show_help();
    void limit_clicked();


private slots:
    void on_checkBox_Addback_on_clicked(bool checked);

private:
    Ui::T4addback_dlg ui;
};

#endif // T4ADDBACK_DLG_H
