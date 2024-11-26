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
#ifdef NIGDY
	
  gate_1D(const string & m_name, const string &dis_nam, 
	       typ_x x0, typ_x  y1,    // these arguments we give if we want the boundaries be of special size,
	       typ_x x1, typ_x y0)  //         for example to fit into the current window 
  {
    
    
    vertex v[4] = { vertex(x0, y0),  vertex(x0, y1), 
		    vertex(x1, y1) , vertex(x1, y0) } ;
    for(int i = 0 ; i < 4 ; i++)  
      {
	boundaries.push_back(v[i]) ;
      }
    screen_name = m_name ; 
    disk_name = dis_nam ;
    // cout << " boundaries size is " << boundaries.size() << endl;
    
  }
#endif
  
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
#ifdef NIGDY	
	
	friend 
  ostream &  operator<<(ostream & s, gate_1D & oval)
  {
    vector<typ_x>::iterator it ;
    int nr = 0 ; 
    s << "boundaries called " << oval.screen_name << endl;
    for(it = oval.boundaries.begin() ; it != oval.boundaries.end() ;  nr++, it++ )
      {
	// here we are jumping through coordinates
	s << "vertex nr " << nr 
	  << " [ " 
	  << (*it).x   // x 
	  << ", "
	  << (*it).y
	  << "]"
	  << endl ;
      } // end of for interator
    return s ;
  }
  
#endif
	
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //gate_1D_h
