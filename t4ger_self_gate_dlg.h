#ifndef T4GER_SELF_GATE_DLG_H
#define T4GER_SELF_GATE_DLG_H

#include <QDialog>

namespace Ui {
class T4ger_self_gate_dlg;
}

class T4ger_self_gate_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit T4ger_self_gate_dlg(QWidget *parent = 0);
    ~T4ger_self_gate_dlg();

private:
    Ui::T4ger_self_gate_dlg *ui;
};

#endif // T4GER_SELF_GATE_DLG_H
