#ifndef T4SELECT_SPECTRA_H
#define T4SELECT_SPECTRA_H

#include <QDialog>
#include<QMdiArea>
#include <QStringList>

#include <string>
#include <vector>

using std::vector;
using std::string;

//class Q3ListBoxItem;
class QListWidgetItem;
class QMdiArea;


namespace Ui {
class T4select_spectra;
}
//////////////////////////////////////////////////////////////////////////////////
class T4select_spectra : public QDialog
{
    Q_OBJECT
    
public:
    explicit T4select_spectra(QWidget *parent = 0);
    ~T4select_spectra();

    T4select_spectra(const T4select_spectra&) = delete;
    T4select_spectra& operator=(const T4select_spectra&) = delete;
    void init();
    void set_parameters ( QMdiArea  * ws );
    void set_filter (QStringList fff );
    void set_listbox_chosen ( vector<string> parcel );
    void show_chosen_spectra();
    QStringList give_last_filter();
    std::vector<std::string> give_listbox_chosen();
    
protected:
    void select_one_pressed();

    void remove_highlighted();
    void remove_all();
    void define_group();
    void combo_groups_activated( );
    void show_choosen_and_exit();
    void selected_filter();
    void delete_group();
    void refresh_defined_groups();




    void zeroing_choosen_spectra();
    /** select all spectra currently filtered */
    void select_all();
    void create_the_sum_spectrum();
    bool check_common_binnings ( string spec_name,
                               double *bin,
                               double *beg,
                               double *end );
    // extended mode is when we want to use CTRL/SHIFT keys - just like in Windows
    void extended_selection_mode ( bool extended );
    void add_to_history ( QString s );
    void select_chosen_as_ascii();
    void create_the_overplot_spectrum();
    //    void right_mouse_on_chosen ( Q3ListBoxItem * itemek, const QPoint & e );
    void zero_all_available_spectra();
    void filter_dialog();
    void add_spectra_names_currently_displayed();
    void move_selected_chosen_up_down(bool up);



private slots:


    void on_push_zero_all_all_clicked();

    void on_checkBox2_toggled(bool checked);

    void on_PushButton_choose_clicked();

    //    void on_ListBox_available_doubleClicked(QListWidget *arg1);

    void on_PushButton_deselect_clicked();

    void on_PushButton_deselect_all_clicked();

    void on_Push_B_OK_clicked();

    void on_Combo_groups_activated(int index);

    void on_Button_show_exit_pressed();



    void on_PushButton_Delete_group_clicked();

    void on_Button_group_pressed();

    void on_pushButton_zeroing_pressed();

    void on_PushButton_choose_ALL_clicked();

    void on_pushButton_sum_clicked();

    void on_push_save_as_ascii_clicked();

    void on_pushButton_overlay_clicked();

    //    void on_ListBox_chosen_rightButtonClicked(QListWidget *arg1, const QPoint &arg2);

    void on_pushButton_filter_dialog_clicked();

    void on_Check_add_overwrite_clicked();

    void on_ListBox_available_itemDoubleClicked(QListWidgetItem *);

    void on_toolButton_shift_up_clicked();

    void on_toolButton_shift_down_clicked();

    void on_ListBox_chosen_itemSelectionChanged();

    void on_push_add_from_screen_clicked();

    void on_lineEdit_anti_filter_textChanged(const QString &arg1);

    void on_checkBox_anti_toggled(bool checked);
    void on_ComboBox_filter_editTextChanged(const QString);
    void on_ComboBox_filter_currentTextChanged(QString arg1);

    // no signals of this (anymore?)
    // void on_ComboBox_filter_textChanged(const QString &arg1);

    //void on_ListBox_available_activated(const QModelIndex &index);

    // void on_ComboBox_filter_activated(const QString &arg1);

    void on_checkBox_case_sens_clicked(bool checked);

    void on_Button_show_exit_clicked();

    //void on_checkBox_toggled(bool checked);

    void on_checkBox_enable_drag_toggled(bool checked);





private:
    Ui::T4select_spectra *ui;

    QStringList group_names;
    QMdiArea *arch_ws;
    std::string *path_to_spectra_ptr ;
    std::string *path_to_polygons_ptr;
    QStringList all_spectra_names;

    QString antifilter;
    std::vector<QString>    stos_filtrow;
    bool flag_case_sensitive = 0;
};
extern string ROWS_COLUMNS_KEYWORD;
#endif // T4SELECT_SPECTRA_H
