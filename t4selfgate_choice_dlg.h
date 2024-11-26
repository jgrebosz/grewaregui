#ifndef T4SELFGATE_CHOICE_DLG_H
#define T4SELFGATE_CHOICE_DLG_H

#include <QDialog>

#include "tselfgate_type.h"

namespace Ui {
class T4selfgate_choice_dlg;
}

class T4selfgate_choice_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit T4selfgate_choice_dlg(QWidget *parent = 0);
    ~T4selfgate_choice_dlg();

    void set_type_of_selfgate(Tselfgate_type t);
    std::string get_selected(){return selected_selfgate_name;}

private slots:
    void on_buttonBox_accepted();

private:
    Ui::T4selfgate_choice_dlg *ui;
    std::string selected_selfgate_name;
};

#endif // T4SELFGATE_CHOICE_DLG_H
