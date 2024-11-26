#ifndef T4picture_angle_H
#define T4picture_angle_H

#include <QDialog>

namespace Ui {
class T4picture_angle;
}

class T4picture_angle : public QDialog
{
    Q_OBJECT

public:
    explicit T4picture_angle(QWidget *parent = 0);
    ~T4picture_angle();

private slots:
    void on_buttonOk_clicked();

private:
    Ui::T4picture_angle *ui;
};

#endif // T4picture_angle_H
