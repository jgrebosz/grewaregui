#ifndef T4SELfGATE_SELECTOR_H
#define T4SELfGATE_SELECTOR_H

#include <QDialog>
#include <string>

namespace Ui {
class T4selfgate_selector;
}

class T4selfgate_selector : public QDialog
{
    Q_OBJECT

public:
    explicit T4selfgate_selector(QWidget *parent = 0);
    ~T4selfgate_selector();
    void set_parameters(std::string prompt, QStringList lista, int current, std::string color, int width);
    QString get_selected_string();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::T4selfgate_selector *ui;
};

#endif // T4SELfGATE_SELECTOR_H
