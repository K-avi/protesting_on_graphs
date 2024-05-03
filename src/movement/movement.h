// protesting_on_graphs: A simulation of .... protesters on graphs
// Copyright (C) 2023 Ivan Mulot-Radojcic 
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 or later.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details: https://www.gnu.org/licenses/

#ifndef MOVEMENT_H 
#define MOVEMENT_H

#include "../common.h"

#include "../graph_protesters/graph_table.h"
#include "../tactics/tactics.h"
#include "../graph_protesters/protester.h"


uint8_t init_pos(GraphTable * gtable); //call before iterating 


uint8_t iterate_ntimes( GraphTable * gtable, Tactics * tactics, uint32_t iter_num); 
/*main iteration function O(w*i) w number of protesters, i number of iterations*/

uint8_t iterate_ntimes_dump( GraphTable * gtable, Tactics * tactics, uint32_t iter_num,
                             char * trace_name, uint16_t flux_start, uint16_t spread_flag);
//same as iterate_ntimes but also dumps graph

uint8_t printGraphTabVar(GraphTable * gt, FILE * stream);
#ifdef debug_mode

uint8_t prepare_ite( GraphTable * gtable); //O(1)
uint8_t iterate_once(GraphTable * gtable , Tactics * t); //O(w) where w is the number of protesters

#endif

#endif