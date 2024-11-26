#ifndef T4TILING_MARIX_DLG_H
#define T4TILING_MARIX_DLG_H

#include <QDialog>
#include <QPainter>

namespace Ui {
class T4tiling_marix_dlg;
}

class T4tiling_marix_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit T4tiling_marix_dlg(QWidget *parent = 0);
    ~T4tiling_marix_dlg();

    void setup(int nr_of_windows, int nr_columns, int nr_rows_arg);
    void take_results(int & columns, int & rows)
    {
          columns = nr_col;
          rows = nr_rows;
    }
protected:

    void paintEvent(QPaintEvent *);


private slots:
    void on_spinBox_columns_editingFinished();

    void on_spinBox_rows_editingFinished();

    void on_buttonBox_accepted();

    void on_spinBox_rows_valueChanged(int arg1);

    void on_spinBox_columns_valueChanged(int arg1);

private:
    Ui::T4tiling_marix_dlg *ui;
    int how_many_windows;
    int nr_col;
    int nr_rows;
};

#endif // T4TILING_MARIX_DLG_H
