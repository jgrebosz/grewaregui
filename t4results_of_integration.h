#ifndef T4results_OF_INTEGRATION_H
#define T4results_OF_INTEGRATION_H

#include <QDialog>
#include <swiat.h>

namespace Ui {
   class T4results_of_integration;
}

///////////////////////////////////////////////////////////////
class T4results_of_integration : public QDialog
{
    Q_OBJECT
    
public:
	explicit T4results_of_integration(QWidget *parent = nullptr);
    ~T4results_of_integration();

    T4results_of_integration(const T4results_of_integration&) = delete;
    T4results_of_integration& operator=(const T4results_of_integration&) = delete;

	void add_results(const char *ss,
					 std::string name_of_spectrum__ = "" ,
					 double centroid_channel = 0,
					 double real_channel_of_local_maximum = 0);
    bool multi_line_mode();
    void add_markers(double marker1, double marker2, std::vector<double> bg)
    {
      yellow_markers.push_back(marker1);
      yellow_markers.push_back(marker2);
      background_markers = bg;
    }

private slots:

    void on_button_clear_clicked();

    void on_pushButton_save_to_file_clicked();

	void on_pushButton_copy_centroid_clicked();

	void on_pushButton_copy_maximum_clicked();

	void on_pushButton_calculate_clicked();

	void on_pushButton_remember_clicked();

    // void on_pushButton_skip_clicked();

    void on_pushButton_copy_centroid_B_clicked();

    void on_pushButton_copy_maximum_B_clicked();

//    void on_pushButton_grab_clicked();

    void on_pushButton_drugi_clicked();

    void on_pushButton_remove_line_clicked();

    void on_tabWidget_tabBarClicked(int index);

private:
    Ui::T4results_of_integration *ui;

	// for simple calibration purposes
	double  centroid_channel = 0;
	double real_channel_of_local_maximum = 0;
	std::string name_of_spectrum;
    // new idea
    std::vector<typ_x> yellow_markers;
    std::vector<typ_x> background_markers;

};
////////////////////////////////////////////////////////////////
extern T4results_of_integration * integr_resul_ptr;
#endif // T4results_OF_INTEGRATION_H
