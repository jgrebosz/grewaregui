#include "gate_1D.h"
#include <qfile.h>

// ********************************************************
void gate_1D::save_to_disk()
{
  //vector<typ_x>::iterator it ;
  //int nr = 0 ; 
  //s << "boundaries called " << oval.name << endl;
  string path_name = gpath.gates_1D + disk_name ;
  ofstream plik(path_name.c_str() );
  //cout  << "s--------------------------------aving boundaries called " << path_name << endl;
  for(unsigned i = 0 ; i < boundaries.size() ;  i+=2  )
    {
      // here we are jumping through coordinates
      plik << boundaries[i] 
		 << "\t\t"
		 << boundaries[i+1]
		 << "\t\t // left and right range of the window nr "
		 << (i/2)+1
		 << " in the gate "
		 << screen_name 
		 << endl ;
    } // end of for interator
}
//********************************************************
void gate_1D::remove_from_disk()
{
	// removing from the disk file
	
    string path_name = gpath.gates_1D + disk_name ;
	QFile file(path_name.c_str() );
	file.remove() ;
}
