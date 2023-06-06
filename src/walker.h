#ifndef WALKER_H
#define WALKER_H 

#include "common.h"


#include <bits/types/FILE.h>
#include <stdint.h>
#include <stdio.h>

//might be irrelevant 

typedef struct s_graph_table_entry GraphTableEntry;
typedef struct s_walker{
    uint32_t id; 
    uint32_t curgen;
    //what does a walker need to know ? good question. 
    GraphTableEntry * cur_entry; //gte of the node where the where the walker currently is
}Walker;

typedef struct s_warray{
    
    uint32_t size; 
    Walker *array;

}WalkerArray ; //where the actual walkers will be stored and initialised and so on


typedef struct s_wkref_stack{
    uint32_t stack_capa ; 
    uint32_t stack_in;

    Walker ** walker_stack;
}WalkerRefStack;


typedef struct s_walker_table_entry{
    uint32_t nb_walker_cur ; //keep track of the nb of the walkers in the cur_stack 
    //before emptying it; useful to make rules and so on
    WalkerRefStack cur_stack;
    WalkerRefStack next_stack;

 
}WalkerTableEntry ; //deletion w swap ????
//confused as to how to update this


uint8_t initWalkerArray( WalkerArray * wArray, uint32_t size);
void freeWalkerArray(WalkerArray * wArray);

void printWarray(WalkerArray *wArray, FILE * stream);
uint8_t initWalkerEntry( WalkerTableEntry * tabEntry , uint32_t size );
void freeWalkerEntry(WalkerTableEntry * tabEntry);

uint8_t push_wte_nextstack( WalkerTableEntry * tabEntry, Walker * walker_ref);
uint8_t pop_wte_curstack( WalkerTableEntry * tabEntry , Walker ** wkref_ret);

void printWalkerEntry( WalkerTableEntry * tabEntry,  FILE* stream);

#endif