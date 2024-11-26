#include "tgui_options.h"
#include "ui_tgui_options.h"
//******************************************************************************
Tgui_options::Tgui_options(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Tgui_options)
{
  ui->setupUi(this);
  dark_blue = QColor(0, 0, 60);
  dark_green = QColor(0, 60, 0);
  white =  QColor(255, 255, 255);
  black = QColor(0, 0, 0);
  // read from file?

}
//******************************************************************************
Tgui_options::~Tgui_options()
{
  delete ui;
}
//******************************************************************************
bool Tgui_options::on_buttonBox_accepted()
{
  return true;
}
//******************************************************************************
int Tgui_options::give_font_size()
{
  return ui->spinBox_size_of_text->value();
}
//******************************************************************************
void Tgui_options::set_font_size(int size)
{
  ui->spinBox_size_of_text->setValue(size);
}
//******************************************************************************
void Tgui_options::background_for_1D_spectra(QColor c)
{

  if(c == dark_blue )     // dark blue
    {
      ui->radioButton_blue_colour->setChecked(true);
    }
  else if (c == dark_green)     // dark green
    {
      ui->radioButton_green_colour->setChecked(true);
    }
  else if (c == black)     // black
    {
      ui->radioButton_black_colour->setChecked(true);
    }
  else if (c == white)   // white
    {
      ui->radioButton_white_colour->setChecked(true);
    }

}
//******************************************************************************
QColor Tgui_options::give_background_for_1D_spectra()
{

  if(ui->radioButton_blue_colour->isChecked()) return dark_blue ;
  if(ui->radioButton_green_colour->isChecked()) return dark_green ;
  if(ui->radioButton_white_colour->isChecked()) return white ;
  if (ui->radioButton_black_colour->isChecked()) return black ;
return QColor(50, 0, 0);
}
