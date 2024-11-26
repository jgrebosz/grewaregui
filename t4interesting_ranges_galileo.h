#ifndef T4INTERESTING_RANGES_GALILEO_H
#define T4INTERESTING_RANGES_GALILEO_H

#include <QDialog>
#include <QLineEdit>
#include <fstream>



namespace Ui {
  class T4interesting_ranges_galileo;
}

class T4interesting_ranges_galileo : public QDialog
{
  Q_OBJECT

public:
  explicit T4interesting_ranges_galileo(QWidget *parent = 0);
  ~T4interesting_ranges_galileo();

private slots:
  void on_buttonBox_accepted();

private:
  Ui::T4interesting_ranges_galileo *ui;

  void read_and_set(std::ifstream &file, std::string txt, QLineEdit * ledit);

};

#endif // T4INTERESTING_RANGES_GALILEO_H
