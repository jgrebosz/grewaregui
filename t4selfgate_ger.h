#ifndef T4SELFGATE_GER_H
#define T4SELFGATE_GER_H

#include <QDialog>

namespace Ui {
class T4selfgate_ger;
}

class T4selfgate_ger : public QDialog
{
    Q_OBJECT

public:
    explicit T4selfgate_ger(QWidget *parent = 0);
    ~T4selfgate_ger();

private:
    Ui::T4selfgate_ger *ui;
};

#endif // T4SELFGATE_GER_H
