#ifndef T4UPDATE_INFO_DLG_H
#define T4UPDATE_INFO_DLG_H

#include <QDialog>

namespace Ui {
class T4update_info_dlg;
}

class T4update_info_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit T4update_info_dlg(QWidget *parent = 0);
    ~T4update_info_dlg();

private slots:
    void on_push_update_clicked();

    void on_push_later_clicked();

private:
    Ui::T4update_info_dlg *ui;
};

#endif // T4UPDATE_INFO_DLG_H
