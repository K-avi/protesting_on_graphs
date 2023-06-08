#ifndef MOVEMENT_H 
#define MOVEMENT_H

#include "common.h"

#include "graph_table.h"
#include "tactics.h"
#include "walker.h"


uint8_t init_pos(GraphTable * gtable); //call before iterating 


uint8_t iterate_ntimes( GraphTable * gtable, Tactics * tactics, uint32_t iter_num); 
/*main iteration function O(w*i) w number of walkers, i number of iterations*/

uint8_t iterate_ntimes_dump( GraphTable * gtable, Tactics * tactics, uint32_t iter_num, FILE * stream);
//same as iterate_ntimes but also dumps graph


#ifdef debug_mode

uint8_t prepare_ite( GraphTable * gtable); //O(1)
uint8_t iterate_once(GraphTable * gtable , Tactics * t); //O(w) where w is the number of walkers

#endif

#endif