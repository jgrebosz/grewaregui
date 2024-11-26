#ifndef T4LYCCA_PARAMETERS_H
#define T4LYCCA_PARAMETERS_H

#include <QDialog>

namespace Ui {
class T4lycca_parameters;
}

class T4lycca_parameters : public QDialog
{
    Q_OBJECT

public:
    explicit T4lycca_parameters(QWidget *parent = 0);
    ~T4lycca_parameters();

protected:
    void accept();
    void init();
    void help_Slot();
    void set_default();


private:
    Ui::T4lycca_parameters *ui;
};

#endif // T4LYCCA_PARAMETERS_H
