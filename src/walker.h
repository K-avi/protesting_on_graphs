#ifndef WALKER_H
#define WALKER_H 

#include "common.h"
#include "graph.h"
#include <stdint.h>

//might be irrelevant 

typedef struct s_walker{
    uint32_t id; 
    //what does a walker need to know ? good question. 
}Walker;

typedef struct s_walker_table_entry{
    uint32_t capa; 
    uint32_t curr_in; 
    Walker * walkers;
}WalkerTableEntry ; //deletion w swap ????
//confused as to how to update this

typedef struct s_walker_table{
//table indexed by node key containing the walkers at a specific key

    uint32_t size; //fixed size array cuz the size is the number of entries (possibly mod something ?)
    WalkerTableEntry * table;

}WalkerTable;

uint8_t initTable( WalkerTable * walkerTab,  uint32_t size);
uint8_t freeTable( WalkerTable * walkerTab);

uint8_t printTable( WalkerTable * walkerTab);

uint8_t initPos( WalkerTable * walkerTab , uint64_t num_walker );

#endif