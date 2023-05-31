#ifndef WALKER_H
#define WALKER_H 

#include "common.h"
#include "graph.h"
#include <stdint.h>


typedef struct s_walker{
    uint32_t id; 
    //what does a walker need to know ? gooq question. 
}Walker;

typedef struct s_walker_table{

/*
table indexed by node array containing the walkers 
*/

    Walker * placeholder;


}WalkerTable;

uint8_t initTable( WalkerTable * walkers, Graph *g);
uint8_t freeTable( WalkerTable * walkers);

uint8_t printTable( WalkerTable * walkers);

uint8_t initPos( WalkerTable * walkers , Graph * g);

#endif