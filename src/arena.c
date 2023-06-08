
#include "arena.h"
#include "graph_table.h"
#include "misc.h"
#include "tactics.h"
#include "walker.h"
#include <stdint.h>
#include <stdlib.h>

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
}

uint64_t calculate_size( uint32_t nb_nodes,uint32_t nb_lines ,uint32_t nb_rules , uint32_t nb_walkers){
    /*this will be the single most critical calculation of the program. 
    don't. fuck. this. up*/

    uint64_t ret;
    ret= sizeof(GraphTable) + sizeof(LineArray); // base gtable struct
    ret+= sizeof(WalkerCurNext) + sizeof(WalkerArray); //base walker struct
    ret+= sizeof(Tactics); //base tactics struct 
    return ret;

}//unsafe af 