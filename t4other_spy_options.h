#ifndef T4OTHER_SPY_OPTIONS_H
#define T4OTHER_SPY_OPTIONS_H

#include <QDialog>

namespace Ui {
class T4other_spy_options;
}

class T4other_spy_options : public QDialog
{
    Q_OBJECT

public:
    explicit T4other_spy_options(QWidget *parent = 0);
    ~T4other_spy_options();
protected:
    void accept();
    void init();

private slots:
    void on_buttonOk_clicked();

private:
    Ui::T4other_spy_options *ui;
};

#endif // T4OTHER_SPY_OPTIONS_H
