#ifndef T4MANAGER_USER_CONDITIONS_H
#define T4MANAGER_USER_CONDITIONS_H

#include <QDialog>
#include <QTimer>
#include <QTableWidget>

#include <string>
using std::string;
#include <vector>
using std::vector;

#include "Tcondition_description_cracow.h"



namespace Ui {
class T4manager_user_conditions;
}

class T4manager_user_conditions : public QDialog
{
    Q_OBJECT
    
public:
    explicit T4manager_user_conditions(QWidget *parent = 0);
    ~T4manager_user_conditions();
    
private slots:
    void on_push_create_new_clicked();
    void on_push_remove_selected_clicked();
    void on_push_edit_selected_condition_clicked();
    void on_push_How_to_clone_clicked();
    void on_push_A_1_cloning_clicked();
    void on_push_Save_review_as_file_clicked();
    void on_ComboBox_filter_textChanged(const QString &);
    void check_statistics();
    void on_table_cellDoubleClicked(int, int);
    void on_review_cellDoubleClicked(int row, int column);

	void on_ComboBox_filter_editTextChanged(const QString &arg1);

private:
    Ui::T4manager_user_conditions *ui;

    vector<Tcondition_description_cracow> vec_cond_descr;
    QTimer *statistics_timer ;


    void update_the_table();
    void table_changed( int /*row*/, int /*col*/ );

    string is_name_unique( string n, int nr );
    void row_is_selected();

    void update_review();

    bool anybody_uses_this_condition();
    bool is_possible_to_remove_this_condition( string condition_name );
    void filtered_condtions_only();
//    //void cloning_A_1_();
    void make_new_row_in_review_if_needed(int row);


};

#endif // T4MANAGER_USER_CONDITIONS_H
