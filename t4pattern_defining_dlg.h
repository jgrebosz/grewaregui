#ifndef T4PATTERN_DEFINING_DLG_H
#define T4PATTERN_DEFINING_DLG_H

#include <QDialog>
#include <vector>
#include <string>

using namespace std;

namespace Ui {
class T4pattern_defining_dlg;
}

class T4pattern_defining_dlg : public QDialog
{
    Q_OBJECT

public:
	explicit T4pattern_defining_dlg(QWidget *parent = nullptr);
    ~T4pattern_defining_dlg();
    void set_parameters(std::string specname,
                   std::string pattern1_, string pattern2_,
                   std::string *one,
                   std::string *two, std::string contents);
    void   get_parameters(vector<string> * vone, vector<string> * vtwo, vector<string> *filenames);
    string find_patterns_and_make_skeleton_with_procents(string text);

	string find_patterns_and_replace(string text, string pattern, string replacement, bool & flag_some_pattern_found);

	void choose_proper_neverused_character();

    string make_a_clone_from_skeleton_using_kombination(string skeleton,
                                                        string proc1,
                                                        string proc2,
                                                        string *result_bw_glob,
                                                        bool *flag_any_change);

private slots:
    void on_lineEdit_one_textChanged(const QString &);
    void on_lineEdit_two_textChanged(const QString &);
    void on_checkBox_pattern2_clicked(bool checked);
    void on_lineEdit_pattern1_textChanged(const QString &arg1);
    void on_lineEdit_pattern2_textChanged(const QString &arg1);
    void on_pushButton_clicked();
    void on_pushButton_give_numbers_clicked();

private:
    Ui::T4pattern_defining_dlg *ui;
    void show_spectra_names();

    std::vector<std::string> spectra_names;
    std::vector<std::string> bw_spectra_names; // black white spectra (without colour markers)
    std::vector<std::string> vec_one ;
    std::vector<std::string> vec_two ;

    string nieb_pocz ;
    string czerw_pocz ;
    string kolor_kon ;

    string pattern1 = {""};
	bool flag_second_pattern_checked {false};
    string pattern2 = {""};


    void prepare_filename_skeleton();
    string original_specname;
    string suggested_one ;
    string suggested_second;
    void prepare_file_contents_skeleton();
	string make_percents_in_color(string txt);
    string template_file_contents;

	char neverused_character;   // which we will use for replacing pattern1 purposes
};

#endif // T4PATTERN_DEFINING_DLG_H
