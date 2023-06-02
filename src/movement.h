#ifndef MOVEMENT_H 
#define MOVEMENT_H

#include "common.h"

#include "graph_table.h"
#include "tactics.h"
#include "walker.h"


uint8_t initPos(GraphTable * gtable, WalkerArray *walkerArr);
uint8_t update(GraphTable * gt, WalkerArray * walkerArr);

#endif