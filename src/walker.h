#ifndef WALKER_H
#define WALKER_H 

#include "common.h"

#include <stdint.h>
#include <stdio.h>

//might be irrelevant 

typedef struct s_graph_table_entry GraphTableEntry;
typedef struct s_line Line;
struct s_graph_table_entry{

    Line * first_neighboor_ref;
    uint32_t neighboor_num;
    uint32_t node_key; 
    //stores node index; bloated but allows fast lookup of neighboor for the walkers
    //to store
};


typedef struct s_walker{
    uint32_t id; 
    GraphTableEntry * cur_entry; //gte of the node where the where the walker currently is
}Walker;

typedef struct s_warray{
    uint32_t size; 
    Walker *array;
}WalkerArray ; //where the actual walkers will be stored and initialised and so on

typedef struct s_walker_num_curnext{
    //structure to store the number of walker at each node ;updated to keep track of where 
    //walkers came in and stuff 
    uint32_t size;
    uint32_t *cur_num;
    uint32_t *next_num; 
}WalkerCurNext;

uint8_t initWalkerArray( WalkerArray * wArray, uint32_t size);
void freeWalkerArray(WalkerArray * wArray);

void printWarray(WalkerArray *wArray, FILE * stream);

uint8_t initWalkerCurNext(WalkerCurNext * wkcn , uint32_t size);
void freeWalkerCurNext(WalkerCurNext * wkcn);

uint8_t swapWalkerCurNext (WalkerCurNext * wkcn);

#endif