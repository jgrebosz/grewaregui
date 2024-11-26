#ifndef T4ADDLINE_1D_WIZARD_H
#define T4ADDLINE_1D_WIZARD_H

#include <QWizard>

namespace Ui {
  class T4addline_1d_wizard;
}

class T4addline_1d_wizard : public QWizard
{
  Q_OBJECT

public:
  explicit T4addline_1d_wizard(QWidget *parent = 0);
  ~T4addline_1d_wizard();

  int set_incrementor( QString name );
  bool give_obligatory() {return make_obligatory; }


private slots:
  int nextId() const;
  bool validateCurrentPage();
  void on_pushButton_for_example_when_clicked();
  void on_pushButton_expl_all_when_clicked();

private:
  Ui::T4addline_1d_wizard *ui;

  bool make_obligatory;
  bool is_when;
  bool is_all;
  bool when_not_valid_consider;
  QString incrementor_name;

  enum { page__keyword_WHEN = 0,  page__keywords_ALL_WHEN, page__what_was_chosen };


};

#endif // T4ADDLINE_1D_WIZARD_H
