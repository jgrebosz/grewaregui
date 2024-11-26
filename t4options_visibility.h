#ifndef T4OPTIONS_VISIBILITY_H
#define T4OPTIONS_VISIBILITY_H

#include <QDialog>

#include <vector>
using namespace std;


namespace Ui {
class T4options_visibility;
}

class T4options_visibility : public QDialog
{
    Q_OBJECT

public:
    explicit T4options_visibility(QWidget *parent = 0);
    ~T4options_visibility();

    void set_parameters(std::vector<QAction *> vec, int /* nr_experiment */ );

private slots:
    void on_buttonBox_accepted();
    void on_table_cellClicked(int row, int column);
    void on_radioButton_exotic_clicked(bool);

protected:
    void refresh_table();

private:
    Ui::T4options_visibility *ui;

    std::vector<QAction *> vec_of_actions;


};

#endif // T4OPTIONS_VISIBILITY_H
