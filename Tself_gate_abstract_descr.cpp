/***************************************************************************
                          Tself_gate_abstract_descr.cpp  -  description
                             -------------------
    begin                : Thu Sep 11 2003
    copyright            : (C) 2003 by dr. Jerzy Grebosz, IFJ Krakow, Poland
    email                : jerzy.grebosz@ifj.edu.pl
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "Tself_gate_abstract_descr.h"

#include "Tself_gate_agata_psa_descr.h"
#include "Tself_gate_ger_addback_descr.h"
#include "Tself_gate_ger_descr.h"
#include "Tself_gate_hec_descr.h"
#include "Tself_gate_mib_descr.h"
#include "Tself_gate_kratta_descr.h"
#include "Tself_gate_ge_galileo_descr.h"

#include "paths.h"

//*****************************************************************************
Tself_gate_abstract_descr::Tself_gate_abstract_descr() : sg_type(0)
{
}
//*****************************************************************************
Tself_gate_abstract_descr *   Tself_gate_abstract_descr::create_descr_for_sg_file(string sg_name)
{
    //
    Tselfgate_type typ =  Tselfgate_type::give_type_of_selfgate_called(sg_name);
    Tself_gate_abstract_descr *d = nullptr;

    switch(typ.my_type)
    {
        default:
            throw runtime_error(
                        " Required self-gate file called : " +  sg_name
                        + " does not have any supported extension ");

        break;

        case Tselfgate_type::german_crystal:   d = new Tself_gate_ger_descr ; break;
        case Tselfgate_type::hector:   d = new Tself_gate_hec_descr ; break;
        case Tselfgate_type::miniball:   d = new Tself_gate_mib_descr ; break;
        case Tselfgate_type::cluster_addback:   d = new Tself_gate_ger_addback_descr ; break;
        case Tselfgate_type::agata_psa:   d = new Tself_gate_agata_psa_descr ; break;
        case Tselfgate_type::galileo_ge:   d = new Tself_gate_ge_galileo_descr; break;
            case Tselfgate_type::kratta:   d = new Tself_gate_kratta_descr ; break;
    }

    d->read_definition_from(gpath.conditions + sg_name);
//    d->read_definition_from("./conditions/" + sg_name);
    return d;
}
