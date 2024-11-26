#include "t4tiling_marix_dlg.h"
#include "ui_t4tiling_marix_dlg.h"

T4tiling_marix_dlg::T4tiling_marix_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4tiling_marix_dlg)
{
    ui->setupUi(this);
}
//*****************************************************************************
T4tiling_marix_dlg::~T4tiling_marix_dlg()
{
    delete ui;
}
//*****************************************************************************
void T4tiling_marix_dlg::setup(int nr_of_windows, int nr_columns, int nr_rows_arg)
{
    how_many_windows = nr_of_windows;
    QString f = QString("You want to display %1 spectra in a following way :").arg(nr_of_windows);
    ui->label_how_many->setText(f);
    ui->label_how_many->adjustSize();

    //
    while(nr_columns * nr_rows_arg < nr_of_windows )
    {
        ++nr_rows_arg;
    }

    ui->spinBox_columns->setValue(nr_columns);
    ui->spinBox_rows->setValue(nr_rows_arg);
    // table

    //    ui->tableWidget->setRowCount(nr_rows);
    //    ui->tableWidget->setColumnCount(nr_rows);
    //    ui->tableWidget->adjustSize();



}
//********************************************************************************
void T4tiling_marix_dlg::paintEvent(QPaintEvent *  /*e*/ )
{
    QPainter painter(this);
    QPen linepen(Qt::yellow);
    linepen.setWidth(2);
    linepen.setCapStyle(Qt::RoundCap);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(linepen);

    nr_col = ui->spinBox_columns->value();
    nr_rows = ui->spinBox_rows->value();


    while(how_many_windows > nr_rows * nr_col)
    {
        ++nr_rows;
    }
    ui->spinBox_rows->setValue(nr_rows);

    //auto f = rect();
    int szer = rect().width() / (2 * nr_col) ;
    int wys = rect().height() / (2 *nr_rows);

    int pos_x = rect().width() * 0.1;
    pos_x = ui->label_how_many->pos().x();
    int pos_y = rect().height() * 0.4;
    pos_y = ui->label_how_many->pos().y() * 4;

    int max_y = 0;
    int max_x = 0;

    bool flag_error = false;
//    if(how_many_windows > nr_rows * nr_col)
//    {

//        flag_error = true;
//    }



    int rect_counter = 0 ;
    for(int w = 0 ; w < nr_rows ; ++w)
        for(int i = 0 ; i < nr_col ; ++i, ++rect_counter)
        {
            int x = pos_x + szer * i;
            int y = pos_y + wys* w;
            auto r = QRect(QPoint(x, y), QPoint(x+ szer - 5, y+ wys - 5));

            if(rect_counter < how_many_windows) painter.fillRect(r, flag_error ?
                                                                     QColor(100, 0, 0) :
                                                                     QColor(0, 100, 0) );
            painter.drawRect (r);

            max_y = y+ wys - 5;
            max_x = x+ szer - 5;

        }
    // painter.drawLine(QPoint(10,10), QPoint(100, 100) );

    // painter.drawLine( rect().topLeft(), rect().bottomRight() );

   int poz_x_row_txt = max_x + 20;
   if(flag_error){
       QString msg = QString ("Error: not enough for %1 windows!").arg(how_many_windows);

        linepen.   setColor( Qt::red);
        painter.setPen(linepen);
        painter.drawText( QPoint(poz_x_row_txt,  max_y + (rect().height() - max_y) /2 -
                                  (ui->frame_columns->height() / 2)),  msg );


   }
   ui->buttonBox->setDisabled(flag_error);

    // move spinner nr of columns below the graph
    ui->frame_columns->move(pos_x + ((max_x - pos_x)/2) - ui->frame_columns->width() / 2,
                            max_y + (rect().height() - max_y) /2 -
                            (ui->frame_columns->height() / 2));
    ui->frame_rows->move(max_x + 20, pos_y + pos_y + (max_y - pos_y) /2 );
}
//*****************************************************************************
void T4tiling_marix_dlg::on_spinBox_columns_editingFinished()
{

}
//*****************************************************************************
void T4tiling_marix_dlg::on_spinBox_rows_editingFinished()
{
    //    ui->tableWidget->setRowCount(
    //                ui->spinBox_rows->value()
    //                );
    //    ui->tableWidget->update();
}
//*****************************************************************************
void T4tiling_marix_dlg::on_buttonBox_accepted()
{

}
//*****************************************************************************

void T4tiling_marix_dlg::on_spinBox_rows_valueChanged(int /*arg*/)
{
    update();
}
//*****************************************************************************
void T4tiling_marix_dlg::on_spinBox_columns_valueChanged(int /*arg1*/)
{
    update();
}
//*****************************************************************************
