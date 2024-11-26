/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
**************************************************************************/
#include <string>
using namespace std ;
extern bool allow_secret_part ;

void about_dialog::init()
{
	string str = string("version ") + __DATE__ + "  " + __TIME__ ;
	lineEdit_version->setText(str.c_str());
}

//*****************************************************************
void about_dialog::allow_conditions_button()
{
  allow_secret_part = true ;
}
