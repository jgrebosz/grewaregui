#ifndef T4BINNING_CHANGE_DLG_H
#define T4BINNING_CHANGE_DLG_H

#include <QDialog>
#include <string>
#include "swiat.h"

namespace Ui {
class T4binning_change_dlg;
}

class T4binning_change_dlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit T4binning_change_dlg(QWidget *parent = 0);
    ~T4binning_change_dlg();
    void set_parameters( std::string name, spectrum_descr sd, int dim );

private slots:
    void on_buttonOk_released();

private:
    Ui::T4binning_change_dlg *ui;

    bool this_is_2D_spectrum;
    std::string spectrum_name;
    spectrum_descr spde;
    string suffix;

    bool check_binning_on_the_disk_file();

};

#endif // T4BINNING_CHANGE_DLG_H
