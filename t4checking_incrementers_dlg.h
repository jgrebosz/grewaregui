#ifndef T4CHECKING_INCREMENTERS_DLG_H
#define T4CHECKING_INCREMENTERS_DLG_H

#include <QDialog>
#include <string>
using namespace std;


class T4incrementer_selector;

namespace Ui {
	class T4checking_incrementers_dlg;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class T4checking_incrementers_dlg : public QDialog
{
	Q_OBJECT

public:
	explicit T4checking_incrementers_dlg(QWidget *parent = nullptr);
	~T4checking_incrementers_dlg();

private slots:
	void on_pushButton_start_clicked();

	void on_checkBox_first_errors_clicked(bool checked);

	void on_buttonBox_rejected();

	//void on_pushButton_stop_checking_clicked();

	void on_checkBox_ignore_noinc_toggled(bool checked);

	void on_checkBox_ignore_fan_spectra_toggled(bool checked);

private:
	Ui::T4checking_incrementers_dlg *ui;

	std::vector<std::string> suggest_incrementer(T4incrementer_selector * dlg, std::string filter);
	std::string smart_suggestion(std::string incrementer, int pocz);

	string nieb_pocz ;
	string czerw_pocz ;
	string ziel_pocz ;
	string kolor_kon ;

	bool flag_first_errors = true;
	bool flag_ignore_noinc = true;
	bool flag_ignore_fan_spectra = true;
	// bool flag_stop_checking = false;
};

#endif // T4CHECKING_INCREMENTERS_DLG_H
