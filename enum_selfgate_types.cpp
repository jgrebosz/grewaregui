

#include "enum_selfgate_types.h"

using namespace std;


//////////////////////////////////////////////////////////////////////////////////////
//enum class  Type_of_selfgate {   // NEVER CHANGE THIS ORDER Or VALUES
//    not_available,
//    german_crystal,
//    cluster_addback,
//    hector,
//    miniball,
//    agata_psa           // Agata puls shape analysis

//    // any new items put always at the end
//};

//--------------------------------------------------------------------------------
// enum code / name / extension / explanation
//--------------------------------------------------------------------------------

map<Type_of_selfgate, vector<string> >  selfgate_info_map;
//*********************************************************************************
void init_selfgate_info()
{
    selfgate_info_map[Type_of_selfgate::not_available] = {"X", ".not_available", ""} ;

    selfgate_info_map[Type_of_selfgate::german_crystal] = {"german_crystal", ".self_gate_ger_crystal", "single crystal of germanium (Rising)"} ;
    selfgate_info_map[Type_of_selfgate::cluster_addback] = {"cluster_addback", ".self_gate_ger_addback_cluster", "cluster of crystals of germanium (Rising)"} ;
    selfgate_info_map[Type_of_selfgate::hector] = {"hector", ".self_gate_hec_crystal", "BaF of HECTOR detectors"} ;

    selfgate_info_map[Type_of_selfgate::miniball] = {"miniball", ".self_gate_mib_crystal", "Germanium detectors of MINIBALL"} ;
    selfgate_info_map[Type_of_selfgate::agata_psa] = {"agata_psa", ".self_gate_agata_psa", "Germanium detectors of AGATA - for pulse shape analysis"} ;

}




