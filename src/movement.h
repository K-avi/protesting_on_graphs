#ifndef MOVEMENT_H 
#define MOVEMENT_H

#include "common.h"

#include "graph_table.h"
#include "tactics.h"
#include "walker.h"


uint8_t initPos(GraphTable * gtable, WalkerArray *walkerArr);


uint8_t iterateGen(GraphTable * gtable, Tactics* tactics);

#ifdef debug_mode
uint8_t moveEntry(GraphTable * gtable,uint32_t node_from , uint32_t node_to); 
uint8_t update(GraphTable * gt, WalkerArray * walkerArr);
uint8_t prepareIteration( GraphTable * gtable, WalkerArray * warray);
#endif

#endif