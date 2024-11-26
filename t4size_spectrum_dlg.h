#ifndef T4SIZE_SPECTRUM_DLG_H
#define T4SIZE_SPECTRUM_DLG_H

#include <QDialog>
#include <vector>
using std::vector;
#include "swiat.h"

namespace Ui {
class T4size_spectrum_dlg;
}
///////////////////////////////////////////////////////////////
class T4size_spectrum_dlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit T4size_spectrum_dlg(QWidget *parent = 0);
    ~T4size_spectrum_dlg();


    void  set_parameters( vector<typ_x>  tab, spectrum_descr sd, int dim);    // int tab[4]
    vector<typ_x>  give_parameters();
    void  apply_all_this_kind();
    void  enable_2D( bool two_D );


    
private:
    Ui::T4size_spectrum_dlg *ui;
};

#endif // T4SIZE_SPECTRUM_DLG_H
