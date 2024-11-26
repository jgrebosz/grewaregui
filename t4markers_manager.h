#ifndef T4MARKERS_MANAGER_H
#define T4MARKERS_MANAGER_H

#include <QDialog>

#include <spectrum_1D.h>


namespace Ui {
class t4markers_manager;
}
//**********************************************************************************************
class t4markers_manager : public QDialog
{
    Q_OBJECT

public:
    explicit t4markers_manager(QWidget *parent ,
                               std::vector<std::vector<typ_x>>  & set_of_markers__,
                               spectrum_1D *widmo
                               );
    ~t4markers_manager();
    void refresh();


private slots:
    void on_pushButton_paste_clicked();
    void on_pushButton_remove_clicked();


    void on_pushButton_insert_clicked();

private:
    Ui::t4markers_manager *ui;
    spectrum_1D *widmo_ptr;

public:
    std::vector<std::vector<typ_x>>  & set_of_markers;
};

#endif // T4MARKERS_MANAGER_H
