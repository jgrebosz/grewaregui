#ifndef ENUM_SELFGATE_TYPES_H
#define ENUM_SELFGATE_TYPES_H


#error  "shoud not include this obsolete file"

#include <string>
#include <vector>
#include <map>


using namespace std;


////////////////////////////////////////////////////////////////////////////////////
enum class  Type_of_selfgate {   // NEVER CHANGE THIS ORDER Or VALUES
    not_available,
    german_crystal,
    cluster_addback,
    hector,
    miniball,
    agata_psa           // Agata puls shape analysis

    // any new items put always at the end
};

//--------------------------------------------------------------------------------
// enum code / name / extension / explanation
//--------------------------------------------------------------------------------

 extern map<Type_of_selfgate, vector<string> >  selfgate_info_map;
 void init_selfgate_info();

//*********************************************************************************
inline     string give_selfgate_type_name(Type_of_selfgate co)
{

    return selfgate_info_map[co][0] ; // name

}
//*********************************************************************************
inline string give_selfgate_type_extension(Type_of_selfgate co)
{
    return selfgate_info_map[co][1] ; // slefgate


}
//*********************************************************************************
inline string give_selfgate_explanation(Type_of_selfgate co)
{
    return selfgate_info_map[co][2] ; // explanation

}
#endif // ENUM_SELFGATE_TYPES_H



