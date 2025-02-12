#ifndef gate_1D_h
#define gate_1D_h

#include <time.h>
#include <string>
#include <fstream>

#include <vector>
#include <string>

#include "paths.h"
extern Tpaths path;
#include "swiat.h"   // for typ_x  typedef 

//--------------------------------
//typedef list<typ_x> boundaries ;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class gate_1D
{
public:
      
	int m ; 
	string screen_name; // short name, to show on the graph
	string disk_name ; // full name of the file on the disk
	vector<typ_x>  boundaries;   
	
	gate_1D(const string &n, const string & dis, vector<typ_x>  &p )
	{
		screen_name = n ;
		boundaries = p ;
		disk_name = dis ;
  }
  //--------------------------------------
  // constructor for new, random gate

  
  //--------------------------------------
  void deselect_all_vertices();
  int  how_namy_vertices_selected();
  //********************************************
  void remove_selected_vertices();
  
  
  //*************************************************
  void add_vertex_after_selected_vertices();
  void move_vertices_of_kloned_boundaries();
  
  
  //*************************************************
  bool is_selected()
  {
    if(how_namy_vertices_selected()) return true ;
    else return false ;
  }
  //--------------------------------------
	void save_to_disk(); 
	void remove_from_disk();
  //---------------------------------------

	
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //gate_1D_h
