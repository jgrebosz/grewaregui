#ifndef T4SETTING_PATHS_DLG_H
#define T4SETTING_PATHS_DLG_H

#include <QDialog>
#include <string>

namespace Ui {
class T4setting_paths_dlg;
}
//////////////////////////////////////////////////////////////////////////
class T4setting_paths_dlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit T4setting_paths_dlg(QWidget *parent = 0);
    ~T4setting_paths_dlg();

    void set_spy_path( std::string sp );
    std::string get_spy_path();
    void browse_dir();

    
private slots:
    void on_pushButton_browse_spec_clicked();

    void on_buttonOk_clicked();

private:
    Ui::T4setting_paths_dlg *ui;
};
//////////////////////////////////////////////////////////////////////////
#endif // T4SETTING_PATHS_DLG_H
