#ifndef MOVEMENT_H 
#define MOVEMENT_H

#include "common.h"

#include "graph_table.h"
#include "tactics.h"
#include "walker.h"


uint8_t initPos(GraphTable * gtable, WalkerArray *walkerArr);

uint8_t iterate_ntimes( GraphTable * gtable, Tactics * tactics, WalkerArray * warray, uint32_t iter_num);

uint8_t iterate_ntimesVAR1( GraphTable * gtable, Tactics * tactics, uint32_t iter_num);


#ifdef debug_mode
uint8_t moveEntry(GraphTable * gtable,uint32_t node_from , uint32_t node_to); 
uint8_t update(GraphTable * gt, WalkerArray * walkerArr);

uint8_t prepareIteration( GraphTable * gtable, WalkerArray * warray);
uint8_t iterateGen(GraphTable * gtable, Tactics* tactics);

uint8_t iterateGenVAR1(GraphTable * gtable, Tactics* tactics);
uint8_t prepareIterationVAR1( GraphTable * gtable);

#endif

#endif