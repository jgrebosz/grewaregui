#ifndef T4ZET_AOQ_RANGES_H
#define T4ZET_AOQ_RANGES_H

#include <QDialog>

namespace Ui {
class T4zet_aoq_ranges;
}

class T4zet_aoq_ranges : public QDialog
{
    Q_OBJECT

public:
    explicit T4zet_aoq_ranges(QWidget *parent = 0);
    ~T4zet_aoq_ranges();

    void accept();

private:
    Ui::T4zet_aoq_ranges *ui;
};

#endif // T4ZET_AOQ_RANGES_H
