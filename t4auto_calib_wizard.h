#ifndef T4AUTO_CALIB_WIZARD_H
#define T4AUTO_CALIB_WIZARD_H

#include <QWizard>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
///////////////////////////////////////////////////////////////////////////////////////////
namespace Ui {
class T4auto_calib_wizard;
}
///////////////////////////////////////////////////////////////////////////////////////////
class T4auto_calib_wizard : public QWizard
{
    Q_OBJECT
    
public:
    explicit T4auto_calib_wizard(QWidget *parent = 0);
    ~T4auto_calib_wizard();
    
private slots:
    void on_push_run_gf4_clicked();
    void on_push_install_clicked();
//    void on_T4auto_calib_wizard_accepted();
    void on_pushButton_show_spectra_names_clicked();
    void on_pushButton_save_skeleton_clicked();
    void on_pushButton_restore_skeleton_clicked();
    void on_lineEdit_skeleton_editingFinished();
    void on_lineEdit_skeleton_textChanged(const QString &arg1);
    void on_lineEdit_one_textChanged(const QString &arg1);
    void on_lineEdit_two_textChanged(const QString &arg1);
    void on_lineEdit_three_textChanged(const QString &arg1);

private:
    Ui::T4auto_calib_wizard *ui;

    void type_of_det_changed();
    bool just_restore_skeleton(std::string fname);
    bool just_save_skeleton(string fname);
    bool convert_one_spectrum( std::string binary_spec, std::string core_name );
    void copy_all_comments( ifstream &plik, ofstream &out );
    void change_texts_rising_miniball();
    int channel_threshold;
    std::vector<std::string> spectra_names;

};
///////////////////////////////////////////////////////////////////////////////////////////
#endif // T4AUTO_CALIB_WIZARD_H
