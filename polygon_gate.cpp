#include "polygon_gate.h"
#include <qfile.h>
#include <vector>
#include <qmessagebox.h>
#include <cstdlib>
using namespace std;



//**************************************************************
void polygon_gate::deselect_all_vertices()
{
    polygon_type::iterator it ;
    int nr = 0 ;
    //s << "Polygon called " << oval.name << endl;
    for(it = polygon.begin() ; it != polygon.end() ;  nr++, it++ )
    {
        if((*it).selected)
            (*it).selected = false ;
    }
}
//**************************************************************
int  polygon_gate::how_namy_vertices_selected()
{
    int result = 0 ;
    polygon_type::iterator it ;
    int nr = 0 ;
    //s << "Polygon called " << oval.name << endl;
    for(it = polygon.begin() ; it != polygon.end() ;  nr++, it++ )
    {
        if((*it).selected)
            result++ ;
    }
    return result ;
}
//********************************************
void polygon_gate::remove_selected_vertices()
{
    polygon_type::iterator it ;
    int nr = 0 ;
    //s << "Polygon called " << oval.name << endl;
    for(it = polygon.begin() ; it != polygon.end() ;  nr++, it++ )
    {
        if((*it).selected)
        {
            polygon.erase(it) ;
            // and start again from the begining of the list
            it = polygon.begin() ;
        }
    }
}

//**************************************************************
void polygon_gate::add_vertex_after_selected_vertices()
{
    polygon_type::iterator it ;
    polygon_type::iterator it2 ;
    int nr = 0 ;
    //s << "Polygon called " << oval.name << endl;
    for(it = polygon.begin() ; it != polygon.end() ;  nr++, it++ )
    {
        if((*it).selected)
        {
            // create the new vertex
            //it->x ;
            it2 = it ;
            it2++ ;
            // in case if it was last vertex, we need the first
            if(it2 == polygon.end())
                it2 = polygon.begin() ;

            vertex v(
                        (it->x + it2->x) / 2,
                        (it->y + it2->y) / 2  ,
                        false 	);

            polygon.insert(++it, v) ;
            it-- ;

        }
    }
}
//**************************************************************

void polygon_gate::move_vertices_of_kloned_polygon()
{
    polygon_type::iterator it ;

    //int nr = 0 ;
    //s << "Polygon called " << oval.name << endl;
    for(it = polygon.begin() ; it != polygon.end() ;  it++ )
    {
        it->x +=10 ;
        it->y +=10 ;
    }
}

// ********************************************************
void polygon_gate::save_to_disk()
{
    polygon_type::iterator it ;
    int nr = 0 ;
    //s << "Polygon called " << oval.name << endl;
    string path_name = gpath.polygons + disk_name ;
    ofstream plik(path_name.c_str() );
    //cout  << "s--------------------------------aving Polygon called " << path_name << endl;
    for(it = polygon.begin() ; it != polygon.end() ;  nr++, it++ )
    {
        // here we are jumping through coordinates
        plik
                << (*it).x   // x
                << "\t\t"
                << (*it).y
                << "\t\t // x, y of the point nr "
                << nr
                << "\t polygon: " << screen_name
                << endl ;
    } // end of for interator
}
//********************************************************
void polygon_gate::remove_from_disk()
{
    // removing from the disk file

    string path_name = gpath.polygons + disk_name ;
    //	QFile file(path_name.c_str() );
    //	file.remove() ;

    // now we change the taktics - we move this to some subdirectory
    // create 'removed'subdirectrory (if it does not exists) ---------------
    string remdir = gpath.polygons + "removed/" ;
    string creating_command = string("mkdir  ") + remdir + " > /dev/null 2>&1";
    int result = system(creating_command.c_str());

    // NOTE: expression " > /dev/null 2>&1" means:
    // The error message (which usually goest to error stream (descriptor 2 means error stream)
    // should be redirected to normal stream (1)
    // and this should finally go to null device
    //------------------------------------------------------------------------------
    // if(result)cout << "No problem!"<< endl;

    // --------- now the moving
    string moving_command = "mv " + gpath.polygons + disk_name + " ";
    moving_command += remdir + disk_name;
    //cout << "Komenda " << moving_command << endl;
    result = system(moving_command.c_str());
    if(result)cout << "Problem with moving the definition, no backup available"<< endl;


    string  info_to_restore = "Your polygon gate was successfully removed.";
    info_to_restore += string("\n\nNote: It was not destroyed, but just transfered into subdirectory called\n")
            + remdir
            + "\n You may restore this  polygon manually by copying it back into  the directory\n "
            + gpath.polygons ;

    QMessageBox::information( nullptr, "Successfuly removed", info_to_restore.c_str() );

}
//*****************************************************************
bool polygon_gate::test_inside(double x, double y)
{


    polygon_type::iterator it ;


    // last point is always equal to the firs

    //int nr = 0 ;

    typedef struct
    {
        double x, y;
    }
    Tpara ;

    vector< pair<double, double> > P ;

    for(it = polygon.begin() ; it != polygon.end() ;  it++ )
    {
        P.push_back(pair<double, double>(it->x, it->y)) ;
    }
    P.push_back(P[0]);


    int crossings = 0;
    for (unsigned int i = 0; i < P.size()-1 ; i++)
    {
        if ((P[i].first < x && x < P[i+1].first) || (P[i].first > x && x > P[i+1].first))
        {
            double t = (x - P[i+1].first) / (P[i].first - P[i+1].first);
            double cy = t*P[i].second + (1-t)*P[i+1].second;
            if (y == cy)
                return true ; // (on boundary)
            else if (y > cy)
                crossings++;
        }
        if(P[i].first == x && P[i].second <= y)
        {
            if (P[i].second == y)
                return true ; // (on boundary);
            if (P[i+1].first == x)
            {
                if ((P[i].second <= y && y <= P[i+1].second)
                        ||
                        (P[i].second >= y && y >= P[i+1].second))
                    return true ; // (on boundary);
            }
            else if (P[i+1].first > x)
                crossings++;
            if (P[i-1].first > x)
                crossings++;
        }
    }

    return (crossings % 2);
}
//**********************************************************************
double polygon_gate::give_minX()
{
    polygon_type::iterator it ;
    it = polygon.begin();
    double minX = (*it).x ;

    for(it = polygon.begin() ; it != polygon.end() ; it++ )
    {
        if(minX > (*it).x )
            minX = (*it).x ;
    } // end of for interator
    return minX;
}
//**********************************************************************
double polygon_gate::give_maxX()
{

    polygon_type::iterator it ;
    it = polygon.begin();
    double maxX = (*it).x ;

    for(it = polygon.begin() ; it != polygon.end() ; it++ )
    {
        if(maxX < (*it).x )
            maxX = (*it).x ;
    } // end of for interator
    return maxX;

}
//**********************************************************************
double polygon_gate::give_minY()
{
    polygon_type::iterator it ;
    it = polygon.begin();
    double minY = (*it).y ;

    for(it = polygon.begin() ; it != polygon.end() ; it++ )
    {
        if(minY > (*it).y )
            minY = (*it).y ;
    } // end of for interator
    return minY;
}
//**********************************************************************
double polygon_gate::give_maxY()
{

    polygon_type::iterator it ;
    it = polygon.begin();
    double maxY = (*it).y ;

    for(it = polygon.begin() ; it != polygon.end() ; it++ )
    {
        if(maxY < (*it).y )
            maxY = (*it).y ;
    } // end of for interator
    return maxY;


}
//**********************************************************************

