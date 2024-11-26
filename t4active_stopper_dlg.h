#ifndef T4ACTIVE_STOPPER_DLG_H
#define T4ACTIVE_STOPPER_DLG_H

#include <QDialog>

namespace Ui {
class T4active_stopper_dlg;
}

class T4active_stopper_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit T4active_stopper_dlg(QWidget *parent = 0);
    ~T4active_stopper_dlg();
protected:
    void accept();
    void init();

    void set_default();


private slots:
    void on_buttonHelp_clicked();

private:
    Ui::T4active_stopper_dlg *ui;
};

#endif // T4ACTIVE_STOPPER_DLG_H
