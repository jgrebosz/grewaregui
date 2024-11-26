#ifndef T4about_DLG_H
#define T4about_DLG_H

#include <QDialog>

namespace Ui {
class T4about_dlg;
}

class T4about_dlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit T4about_dlg(QWidget *parent = nullptr);
    ~T4about_dlg();
    
private slots:
    void on_pushButton_allow_clicked();

private:
    Ui::T4about_dlg *ui;
};
////////////////////////////////////////////////////////////////
extern bool allow_secret_part ;


#endif // T4about_DLG_H
