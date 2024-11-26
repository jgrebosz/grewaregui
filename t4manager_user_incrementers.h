#ifndef T4MANAGER_USER_INCREMENTERS_H
#define T4MANAGER_USER_INCREMENTERS_H

#include <QDialog>
#include <string>
using namespace std;
#include <vector>
#include <QTimer>

#include "Tincrementer_description_cracow.h"

namespace Ui {
class T4manager_user_incrementers;
}
///////////////////////////////////////////////////////////////////////////////////////
class T4manager_user_incrementers : public QDialog
{
    Q_OBJECT

public:
    explicit T4manager_user_incrementers(QWidget *parent = 0);
    ~T4manager_user_incrementers();

protected:
    void add_new();
    //    void remove_selected();
    void edit_existing();
    void init();
    void update_the_table();
    void table_changed( int /*row*/, int /*col*/ );
    //    void clone_definition();
    string is_name_unique( string n, int nr );
    void row_is_selected();

//    void destroy();
    void update_review();
    void make_report_file();
    //void review_double_clicked();
    void update_review2();
    bool anybody_uses_this_incrementer();
     string names_of_spectra_which_use(string incrementer_name);
     string names_of_conditions_which_use(string incrementer_name);
     string names_of_other_incrementers_which_use(string incrementer_name);

    bool is_possible_to_remove_this_incrementer( string incrementer_name );
    void filtered_condtions_only();
    void cloning_A_1_();
    //void remove_incrementer_from_row(int row);



    Tincrementer_description_cracow *increm ;
    vector<Tincrementer_description_cracow> vec_incrementer_descr;
//    QTimer *statistics_timer ;


private slots:
    void check_stitistics();

    void on_push_create_new_clicked();
    void on_push_edit_selected_clicked();
    void on_table_cellDoubleClicked(int row, int column);
    void on_review_cellDoubleClicked(int row, int column);
    void on_push_remove_selected_clicked();
    void on_push_clone_clicked();
    void on_ComboBox_filter_editTextChanged(const QString &arg1);

    void on_push_log_clicked();

private:
    Ui::T4manager_user_incrementers *ui;
};

#endif // T4MANAGER_USER_INCREMENTERS_H
