#ifndef polygon_gate_h
#define polygon_gate_h

#include <time.h>
#include <string>
#include <fstream>

#include <list>
#include <string>
using namespace std ;

#include "paths.h"
extern Tpaths path;
#include "swiat.h"   // for typ_x  typedef
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class vertex   // just as a structure
{
public:
	double x, y ;
	bool selected ;
	vertex(double xx = 0 , double yy = 0, bool sel = false): x(xx),  y(yy), selected(sel) {}}
;
//--------------------------------
typedef list<vertex> polygon_type ;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class polygon_gate
{
public:

	int m ;
	string screen_name; // short name, to show on the graph
	string disk_name ; // full name of the file on the disk
	polygon_type   polygon;

	polygon_gate()   // to be able to make empty clipboard polygon
	{
		screen_name = "" ;
		//polygon = 0 ;
		disk_name = "" ;
		//      vertex v[6] = {
		//        vertex(0, 0),
		//        vertex(0, 1),
		//        vertex((0 + 1)/2.0, 1),
		//        vertex(1, 1) ,
		//        vertex(1, 0),
		//        vertex((0+1)/2.0, 0),
		//      } ;
		//
		//      for(int i = 0 ; i < 6 ; i++)
		//      {
		//        polygon.push_back(v[i]) ;
		//      }
	}

	polygon_gate(const string &n, const string & dis, polygon_type &p )
	{
		screen_name = n ;
		polygon = p ;
		disk_name = dis ;
	}
	//--------------------------------------
	// constructor for new, random gate

	polygon_gate(const string & m_name, const string &dis_nam,
				 typ_x x0, typ_x  y1,    // these arguments we give if we want the polygon be of special size,
				 typ_x x1, typ_x y0)  //         for example to fit into the current window
	{


		vertex v[6] = {
			vertex(x0, y0),
			vertex(x0, y1),
			vertex((x0+x1)/2.0, y1),
			vertex(x1, y1) ,
			vertex(x1, y0),
			vertex((x0+x1)/2.0, y0),
		} ;

		for(int i = 0 ; i < 6 ; i++)
		{
			polygon.push_back(v[i]) ;
		}
		screen_name = m_name ;
		disk_name = dis_nam ;
		// cout << " polygon size is " << polygon.size() << endl;

	}
	//--------------------------------------
	void deselect_all_vertices();
	int  how_namy_vertices_selected();
	//********************************************
	void remove_selected_vertices();


	//*************************************************
	void add_vertex_after_selected_vertices();
	void move_vertices_of_kloned_polygon();


	//*************************************************
	bool is_selected()
	{
		if(how_namy_vertices_selected()) return true ;
		else return false ;
	}
	//*************************************************
	bool is_vertex_selected(int nr)
	{
		int n = 0 ;
		for(auto it = polygon.begin() ; it != polygon.end() ;  ++n, it++ )
		{
			// here we are jumping through coordinates			
			if(nr == n && it->selected)
			{
				return true ;
			}
		}
		return false ;
	}
	//--------------------------------------
	void save_to_disk();
	void remove_from_disk();
	//---------------------------------------
	friend
	ostream &  operator<<(ostream & s, polygon_gate & oval)
	{
		polygon_type::iterator it ;
		int nr = 0 ;
		s << "Polygon called " << oval.screen_name << endl;
		for(it = oval.polygon.begin() ; it != oval.polygon.end() ;  nr++, it++ )
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
	/** No descriptions */
	bool test_inside(double x, double y);

	// to check while pasting if it will be visible
	double give_minX() ;
	double give_maxX() ;
	double give_minY() ;
	double give_maxY() ;


} ;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //polygon_gate_h
