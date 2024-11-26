#ifndef T4INFO_DLG_H
#define T4INFO_DLG_H

#include <QDialog>

namespace Ui {
class T4info_dlg;
}

class T4info_dlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit T4info_dlg(QWidget *parent = 0);
    ~T4info_dlg();
    void set_parameter( QString message);
    void set_label(QString etykieta);

private:
    Ui::T4info_dlg *ui;
};

#endif // T4INFO_DLG_H
