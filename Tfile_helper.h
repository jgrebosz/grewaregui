#ifndef TFILE_HELPER_H
#define TFILE_HELPER_H



// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// NOTE: this is a poorer version of the spy File helper
//  It does not have some function - as:
//
//** find in the given directory the files which names contain a filter substring */
// 1. vector<string> find_files_in_directory(string dir, string filter);
// 2. void repair_the_stream(typ_strumienia& s)  ;

// 3. bool read_default_gate(string gate_name, double *low, double *high);
// 4. bool read_banana(string gate_name, TjurekPolyCond *  *polygon);

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



#include <fstream>
#include <string>
using namespace std ;



// two classes to carry the exception /////////////////////
class Tfile_helper_exception
{
public:
	string message ;
	
};
// //////////////////////////////////////////////////////////
class Tno_keyword_exception : public Tfile_helper_exception { } ;
class Treading_value_exception : public Tfile_helper_exception { } ;
// /////////////////////////////////////////////////////////
    

//**********************************************************************
namespace File_helper   // was Tfile_helper - see using declaration at the end
{
  
  /* This function searches (in the given file) a keyword and then reads the
  value which follows it. Then returns this value as the result.  */

  double find_in_file(ifstream& s, string slowo) ;

  /* find a keyword, - just for positioning cursor  */
  void spot_in_file(ifstream& s, string slowo);
  void repair_the_stream(ifstream& s)  ;

}
//**********************************************************************
extern istream & zjedz(istream & plik);



namespace  FH = File_helper;
namespace  Nfile_helper = File_helper;

#endif
