#include "gate_1D.h"
#include <qfile.h>


#ifdef NIGDY
//**************************************************************
void gate_1D::deselect_all_vertices()
{
  vector<typ_x>::iterator it ;
  int nr = 0 ;
  //s << "boundaries called " << oval.name << endl;
  for(it = boundaries.begin() ; it != boundaries.end() ;  nr++, it++ )
  {
      if((*it).selected)  (*it).selected = false ;
  }
}
//**************************************************************
int  gate_1D::how_namy_vertices_selected()
{
  int result = 0 ;
  vector<typ_x>::iterator it ;
  int nr = 0 ;
  //s << "boundaries called " << oval.name << endl;
  for(it = boundaries.begin() ; it != boundaries.end() ;  nr++, it++ )
    {
      if((*it).selected) result++ ;
    }
  return result ;
}
//********************************************
void gate_1D::remove_selected_vertices()
{
  vector<typ_x>::iterator it ;
  int nr = 0 ;
  //s << "boundaries called " << oval.name << endl;
  for(it = boundaries.begin() ; it != boundaries.end() ;  nr++, it++ )
    {
      if((*it).selected)
	{
	  boundaries.erase(it) ;
	  // and start again from the begining of the list
	  it = boundaries.begin() ;
	}
    }
}

//**************************************************************
void gate_1D::add_vertex_after_selected_vertices()
{
  vector<typ_x>::iterator it ;
  vector<typ_x>::iterator it2 ;
  int nr = 0 ;
  //s << "boundaries called " << oval.name << endl;
  for(it = boundaries.begin() ; it != boundaries.end() ;  nr++, it++ )
    {
      if((*it).selected)
	{
	  // create the new vertex
	  //it->x ;
	  it2 = it ;
	  it2++ ;
	  // in case if it was last vertex, we need the first
	  if(it2 == boundaries.end()) it2 = boundaries.begin() ;

	  vertex v(
		   (it->x + it2->x) / 2,
		   (it->y + it2->y) / 2  ,
		   false 	);

	  boundaries.insert(++it, v) ;
	  it-- ;

	}
    }
}
//**************************************************************

void gate_1D::move_vertices_of_kloned_boundaries()
{
  vector<typ_x>::iterator it ;

  //int nr = 0 ;
  //s << "boundaries called " << oval.name << endl;
  for(it = boundaries.begin() ; it != boundaries.end() ;  it++ )
    {
      it->x +=10 ;
      it->y +=10 ;
    }
}

#endif // NIGDY

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
