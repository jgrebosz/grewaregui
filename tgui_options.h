#ifndef TGUI_OPTIONS_H
#define TGUI_OPTIONS_H

#include <QDialog>

namespace Ui {
class Tgui_options;
}

class Tgui_options : public QDialog
{
    Q_OBJECT
    
public:
    explicit Tgui_options(QWidget *parent = 0);
    ~Tgui_options();

  int give_font_size();
  void set_font_size(int size);
  void background_for_1D_spectra(QColor c);
  QColor give_background_for_1D_spectra();

private slots:
    bool on_buttonBox_accepted();

private:
    Ui::Tgui_options *ui;

    QColor    dark_blue;
    QColor    dark_green;
    QColor    white;
    QColor    black;
};

#endif // TGUI_OPTIONS_H
