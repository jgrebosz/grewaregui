#ifndef T4USER_CONDITION_WIZARD_H
#define T4USER_CONDITION_WIZARD_H

#include <QWizard>
#include <QTableWidget>
#include <string>
using std::string;
#include <vector>
using std::vector;

#include "Tcondition_description_cracow.h"
class Tcondition_description_cracow;

namespace Ui {
  class T4user_condition_wizard;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
class T4user_condition_wizard : public QWizard
{
  Q_OBJECT

public:
  explicit T4user_condition_wizard(QWidget *parent = nullptr);
  ~T4user_condition_wizard();

  void set_parameters( Tcondition_description_cracow *cond_ptr );



private slots:
  void on_pushButton_add_1D_OR_clicked();
  void accept();

  void on_pushButton_remove_1D_OR_clicked();
  void on_pushButton_1D_OR_set_many_cells_clicked();
  void on_pushButton_add_1D_AND_clicked();
  void on_pushButton_remove_1D_AND_clicked();
  void on_pushButton_1D_AND_set_many_cells_clicked();

  void on_table_1D_AND_gates_cellClicked(int row, int column);
  void on_pushButton_add_2D_OR_item_clicked();
  void on_pushButton_remove_2D_OR_row_clicked();
  void on_checkBox_short_polygon_names_OR_clicked(bool checked);

  void on_table_2D_OR_gates_cellDoubleClicked(int row, int column);
  void on_pushButton_add_2D_AND_item_clicked();

  void on_pushButton_remove_2D_AND_row_clicked();
  void on_checkBox_short_polygon_names_AND_clicked(bool checked);
  void on_table_2D_AND_gates_cellDoubleClicked(int row, int column);
  void on_table_2D_AND_gates_cellClicked(int row, int column);

  void on_table_other_conditions_AND_cellClicked(int row, int column);


  void on_toolButton_nodes_AND_add_clicked();
  void on_toolButton_nodes_AND_remove_clicked();
  void on_toolButton_nodes_OR_remove_clicked();
  void on_toolButton_nodes_NAND_remove_clicked();
  void on_toolButton_nodes_NOR_remove_clicked();
  void on_toolButton_nodes_OR_add_clicked();
  void on_toolButton_nodes_NAND_add_clicked();
  void on_toolButton_nodes_NOR_add_clicked();
  void on_table_1D_OR_gates_cellDoubleClicked(int row, int column);

  void on_table_1D_AND_gates_cellDoubleClicked(int row, int column);

  void on_checkBox_OR_gates_clicked(bool checked);
  void on_checkBox_AND_gates_clicked(bool checked);
  void on_checkBox_OR_bananas_clicked(bool checked);
  void on_checkBox_AND_bananas_clicked(bool checked);
  void on_checkBox_external_conditions_clicked(bool checked);

//  void on_T4user_condition_wizard_helpRequested();
    void help();

protected:

  int nextId () const;  // virtual f. from base class
  bool validateCurrentPage();

  enum { page__name,
         page__5possibilities,
         page__1D_OR,
         page__1D_AND,
         page__2D_OR,
         page__2D_AND,
         page__external_nodes,
         page__finish } ;


  bool user_wants_1D_OR,
  user_wants_1D_AND,
  user_wants_2D_OR,
  user_wants_2D_AND,
  user_wants_external_nodes;

private:
  Ui::T4user_condition_wizard *ui;

  Tcondition_description_cracow *cond ;

  //  void next();

  void name_changed();

  //  void add_1D_AND_item();
  //  void add_1D_OR_item();
  //  void add_2D_AND_item();
  //  void add_2D_OR_item();
  //  void add_item_to_nodes();


  //  void remove_1D_AND_item();
  //  void remove_1D_OR_item();
  //  void remove_2D_OR_item();
  //  void remove_2D_AND_item();
  //  void remove_item_nodes();

  void set_many_1D_AND_cells();
  void set_many_1D_OR_cells();

  //void table_1D_AND_double_click();
  //void table_1D_AND_row_is_selected();


  void redisplay_table_1D_AND();
  void redisplay_table_2D_AND();



  void change_var_x_AND(int row);
  void change_var_y_AND(unsigned int row);
  void change_polygon_AND(unsigned int row);
  void dbl_click_on_2D_AND();

  void table_1D_AND_value_changed();
  void table_2D_AND_value_changed();




  //void table_1D_OR_double_click();
  void redisplay_table_1D_OR();
  void table_1D_OR_row_is_selected();

  void redisplay_table_2D_OR();


  void change_var_x_OR(unsigned int row_nr);
  void change_var_y_OR(unsigned int row_nr);
  void change_polygon_OR(unsigned int row);
  //  void dbl_click_on_2D_OR();

  void table_1D_OR_value_changed();
  void table_2D_OR_value_changed();

  void redisplay_tables_nodes();


  void remove_other_cond_name_from_table( QTableWidget *tab, string tab_name,
                                          vector<string> &cond_names);

  void add_external_condition_to_the_nodes_table(vector<std::string> &wekt, [[maybe_unused]] string explanation);


   void remove_selected_rows_from_some_1Dtable(QTableWidget *tab, vector<Tcondition_description_cracow::Titem_1D> &wekt, string wyjasn);
   void remove_selected_rows_from_some_2Dtable(QTableWidget *tab, vector<Tcondition_description_cracow::Titem_2D> &wekt, string wyjasn);



  QString consider_true ;
  QString consider_false ;
  string  double_click_here {"double_click_here"};
  int check_double_click_here(QTableWidget *table_ptr);


};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // T4USER_CONDITION_WIZARD_H
