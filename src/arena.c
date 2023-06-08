
#include "arena.h"
#include "graph_table.h"
#include "misc.h"
#include "tactics.h"
#include "walker.h"

#include <obstack.h>
//oh boy

uint8_t init_arena(Arena *arena, uint64_t size){
    /*intialises the field of a non null arena*/

    arena->memory=(uint8_t* ) malloc(sizeof(uint8_t) * size);
    if(!arena->memory){report_err("init_arena", AR_MALLOC); return AR_MALLOC;}

    arena->size=size;
    arena->top_index=0;
    
    return AR_OK;
}//not tested ; prolly ok

void free_arena(Arena * arena){
    /* frees an arena*/
    if(!arena) return;
    free(arena->memory);
}//that's all folks


void * get_memory(Arena * arena, uint8_t * failure, size_t alloc_size){
    /*watch out. If there is a single mistake in this 
    or in how I use it everything will collapse*/
    if(!arena){ report_err("get_memory", AR_NULL); *failure=AR_NULL ; return NULL;}
    if(alloc_size+arena->top_index>arena->size){ 
        report_err("get_memory", AR_FULL); 
        *failure=AR_FULL ; 
        return NULL;
    }
    
    void * ret = &arena->memory[arena->top_index];
    arena->top_index+=alloc_size;
    arena->top_index+=alloc_size;

    return ret;
}//not tested ; do not use 

//should be wrapped inside of a macro
uint64_t calculate_size( uint32_t nb_nodes,uint32_t nb_lines ,uint32_t nb_rules , uint32_t nb_walkers){
    /*this will be the single most critical calculation of the program. 
    don't. fuck. this. up*/

    uint64_t ret;

    //elements of the graphtable.h 
    ret= sizeof(GraphTable) + sizeof(LineArray); // base gtable struct

    ret+= nb_lines* sizeof(Line) + 2* sizeof( uint32_t); 
    //line array and two times the size of a flux array in the linearray

    ret+= nb_nodes *( sizeof(GraphTableEntry) ) ;
    //there is nb_nodes gte 

    ret+= 2* nb_nodes * (sizeof(uint32_t)); //to store the walker_cur
    //and walker_next array

    //elements of walker.h 
    ret+= sizeof(WalkerCurNext) + sizeof(WalkerArray); //base walker struct

    //store elements of warray
    ret+= nb_walkers* sizeof(Walker); //I mean yeah
    

    //elements of tactics.h
    ret+= sizeof(Tactics); //base tactics struct 

    ret+= nb_rules*sizeof(Rule); //straight forward stuff 

    return ret;

}//unsafe af ; not tested !!! don't use 
/*
even if it works check that it doesn't give too much stuff 
it's gonna be hard to take padding into account tbh
*/
