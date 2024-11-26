#ifndef T4SUM_SPECTRA_DIALOG_H
#define T4SUM_SPECTRA_DIALOG_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class T4sum_spectra_dialog;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class T4sum_spectra_dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit T4sum_spectra_dialog(QWidget *parent = 0);
    ~T4sum_spectra_dialog();

    void set_parameters( QStringList *ptr_names, QStringList *ptr_factors );

protected:
    QStringList *factors;
    QStringList *names;

    
private slots:
    void on_pushButton_remove_row_clicked();
    void on_tableWidget1_cellDoubleClicked(int row, int column);

private:
    Ui::T4sum_spectra_dialog *ui;
};

#endif // T4SUM_SPECTRA_DIALOG_H
