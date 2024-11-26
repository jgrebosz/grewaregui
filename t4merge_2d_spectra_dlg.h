#ifndef T4MERGE_2D_SPECTRA_DLG_H
#define T4MERGE_2D_SPECTRA_DLG_H

#include <QDialog>
#include <string>
#include <vector>
#include "paths.h"


struct binning_2D
{
    double bin, beg, end, bin_y, beg_y, end_y;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
class T4merge_2d_spectra_dlg;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class T4merge_2d_spectra_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit T4merge_2d_spectra_dlg ( QWidget *parent = 0 );
    ~T4merge_2d_spectra_dlg();

private slots:
    void on_pushButton_remove_row_released();

    void on_table1_cellDoubleClicked ( int row, int column );

    void on_buttonOk_clicked();

    void on_button_add_clicked();

    void on_pushButton_restore_the_list_clicked();

    void on_pushButton_save_the_list_clicked();

    void on_pushButton_add_to_automate_list_clicked();

    void on_pushButton_remove_from_automate_list_clicked();

    void on_pushButton_exec_periodic_manually_clicked();

    void on_pushButton_screen_spectra_clicked();

protected:
    bool  read_in_file ( std::string fname, std::vector<int> & spec )  ;
    binning_2D binning ;            // nr of bins, np. 1024 (are double!!!)
    binning_2D binning_sum ;            // nr of bins, np. 1024

    bool save_periodic_list_to_disk( );
    bool restore_periodic_list_from_disk( );

    QString  periodic_list_file = { "periodic_summing_list.txt" };
    bool execute_periodic_summings();
    bool calculations(int nr_of_spectrum, string spec_name, double factor, vector<int> &sum_spectrum);
    bool save_result2D_spectrum(string name, vector<int> &sum_spectrum);
private:
    Ui::T4merge_2d_spectra_dlg *ui;
};

#endif // T4MERGE_2D_SPECTRA_DLG_H
