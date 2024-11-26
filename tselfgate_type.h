#ifndef Tselfgate_type_H
#define Tselfgate_type_H

/* NOTE:
 * The original of this file is always in the Greware directory,
 * not in the spy directory. So always copy it from Greware-->Spy,
 * not the other way
 */


#include <string>
#include <map>
#include <vector>
#include <iostream>
using namespace std;
//----------------------------------------------------------------------------------------------------
class Tselfgate_type
{
public:

    /* Old, original (RISING) order in enum was:
 *    enum type_of_selfgate  { unknown = 0 , germanium_crystal = 1,
 *                              Hector_BaF_crystal = 2, miniball_crystal = 3,
 *                              ger_cluster_addback
 *                            } ;
*/

    enum Types { not_available,
                 german_crystal,
                 hector,
                 miniball,
                 cluster_addback,
                 agata_psa,           // Agata pulsshape analysis
                 galileo_ge,     // Galileo Germanium detectors
                 kratta,
                 paris
               };

    Types my_type;

    Tselfgate_type(Types t);
    explicit Tselfgate_type(int nr) { my_type = static_cast<Types> (nr);  }


    // The MAP below -------------------------------------------------
    // KEY = enum code of the selfgtes
    // VALUE =  vector of 3 strings which  contains:  [0] name_of_type, / [1] extension / [2] info


    static std::map<Types, std::vector<std::string> >  selfgate_types_map;

    //------------------------------------------------------------
    bool sg_is_not_available() { return my_type == not_available ; }
    std::string  give_type_name() const // Types t)
    {
        return selfgate_types_map[my_type][0] ;
    }
    //------------------------------------------------------------
    std::string  give_type_extension() const
    {
        return selfgate_types_map[my_type][1] ;
    }
    //------------------------------------------------------------
    std::string  give_type_filter() const
    {
        return "*" + selfgate_types_map[my_type][1] ;
    }
    //------------------------------------------------------------
    std::string  give_type_info()
    {
        return selfgate_types_map[my_type][2] ;
    }

    // ++++++++++ static ++++++++++++++++++++=
    static
    std::string  give_type_info(Types t)
    {
        return selfgate_types_map[t][2] ;
    }


    static
    Types  give_type_of_selfgate_called(string fname);

    //--- HELPER Friends functions ---------------------------------------------
    friend std::ostream & operator<< (std::ostream &s, const Types &t)
    {
        s << static_cast<int>(t);
        return s;
    }

    friend std::ostream & operator<< (std::ostream &s, const  Tselfgate_type &t)
    {
        s << static_cast<int>(t.my_type);
        return s;
    }
    //*******************************************************************

    //----------------------------------------------------------------
    friend std::istream & operator>> (std::istream &s, Types &t)
    {
        int a;
        s >> a;
        t = static_cast<Types>(a);
        return s;
    }
    //----------------------------------------------------------------
    friend std::istream & operator>> (std::istream &s, Tselfgate_type &t)
    {
        int a;
        s >> a;
        t = static_cast<Tselfgate_type>(a);
        return s;
    }
};

#endif // Tselfgate_type_H
