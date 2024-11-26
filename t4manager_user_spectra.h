#ifndef T4MANAGER_USER_SPECTRA_H
#define T4MANAGER_USER_SPECTRA_H

#include <QDialog>
#include <vector>
#include "user_spectrum_description.h"

namespace Ui {
class T4manager_user_spectra;
}

class T4manager_user_spectra : public QDialog
{
    Q_OBJECT
    
public:
    explicit T4manager_user_spectra(QWidget *parent = 0);
    ~T4manager_user_spectra();
    
private slots:
    void on_push_create_new_clicked();
//    void on_table_cellChanged(int row, int column);
    void on_table_cellClicked(int row, int column);
    void on_table_cellDoubleClicked(int row, int column);
    void on_push_remove_selected_released();
    void on_push_edit_selected_clicked();
    void on_push_clone_clicked();

    void on_ComboBox_filter_textChanged(const QString &);

    void on_push_A_1_clone_clicked();

    void on_push_log_clicked();

    void on_review_cellDoubleClicked(int , int );

	void on_ComboBox_filter_editTextChanged(const QString &arg1);

	void on_pushButton_clone_default_clicked();

private:
    Ui::T4manager_user_spectra *ui;

    void update_the_table();
    void update_review();
    void table_changed( int row, int col );
    void row_is_selected();
    void create_condition();
    void edit_condition();
    void make_report_file();
    void review_double_clicked();

    std::vector<user_spectrum_description> vec_spectra;
    string is_name_unique( std::string n, int nr );
	string correct_to_the_official_incrementer(string specname, string name);
};

#endif // T4MANAGER_USER_SPECTRA_H
