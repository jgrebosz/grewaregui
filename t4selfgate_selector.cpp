#include "t4selfgate_selector.h"
#include "ui_t4selfgate_selector.h"
//************************************************************************************************
T4selfgate_selector::T4selfgate_selector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4selfgate_selector)
{

    ui->setupUi(this);
}
//************************************************************************************************
T4selfgate_selector::~T4selfgate_selector()
{
    delete ui;
}
//************************************************************************************************
void T4selfgate_selector::set_parameters(std::string prompt, QStringList lista, int current, std::string color, int width)
{
      setFixedWidth( width);

    ui->label_prompt->setText(prompt.c_str() );
     ui->listWidget->addItems(lista);
    ui->listWidget->setCurrentRow(current);
//   ui->listWidget->setStyleSheet();

    std::string styl = "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba("
    + color +           //"190, 190, 190, 255"
     "), stop:1 rgba(255, 255, 255, 255))" ;

    ui->listWidget->setStyleSheet( styl.c_str() );


}
//************************************************************************************************
QString T4selfgate_selector::get_selected_string()
{
    return ui->listWidget->currentItem()->text();
}
//************************************************************************************************
void T4selfgate_selector::on_buttonBox_accepted()
{
    QDialog::accept();
}
