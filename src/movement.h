#ifndef MOVEMENT_H 
#define MOVEMENT_H

#include "common.h"

#include "graph_table.h"
#include "tactics.h"
#include "walker.h"
#include <stdint.h>


uint8_t init_pos(GraphTable * gtable);


uint8_t prepare_ite( GraphTable * gtable);
uint8_t iterate_once(GraphTable * gtable , Tactics * t);

uint8_t iterate_ntimes( GraphTable * gtable, Tactics * tactics, uint32_t iter_num);


#ifdef debug_mode


#endif

#endif