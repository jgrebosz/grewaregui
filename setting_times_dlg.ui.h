/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/


void Setting_times_dlg::set_defaults()
{
	spinBox_update_1D->setValue(20);
	spinBox_update_2D->setValue(30);
	spinBox_update_sum->setValue(60);
	checkBox_ask_about_big_matrices->setChecked(true);
     
}


void Setting_times_dlg::give_times( int * time_1D, int *time_2D, int * time_sum, bool * flag )
{
	*time_1D = spinBox_update_1D->value();	
	*time_2D = spinBox_update_2D->value();
	*time_sum = spinBox_update_sum->value();
	*flag = checkBox_ask_about_big_matrices->isChecked();
}

void Setting_times_dlg::set_times( int time_1D, int time_2D, int time_sum, bool flag )
{
	spinBox_update_1D->setValue(time_1D);
	spinBox_update_2D->setValue(time_2D);
	spinBox_update_sum->setValue(time_sum);
	checkBox_ask_about_big_matrices->setChecked(flag);
}
