//unused atm; REALLY want to get back to it at some point
#include "arena.h"
#include "graph_table.h"
#include "misc.h"
#include "tactics.h"
#include "walker.h"


uint8_t init_arena(Arena *arena, uint64_t size){
    /*intialises the field of a non null arena*/

    arena->memory=(uint8_t* ) malloc(sizeof(uint8_t) * size);
    if(!arena->memory){report_err("init_arena", AR_MALLOC); return AR_MALLOC;}

    arena->size=size;
    arena->top_index=0;
    
    return AR_OK;
}// tested ;  ok

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
   // printf("alloc size %d\n", alloc_size);
    
    void * ret = &arena->memory[arena->top_index];
    arena->top_index+=alloc_size;

    return ret;
}//not tested ; do not use 

//should be wrapped inside of a macro
uint64_t calculate_size( uint32_t nb_nodes,uint32_t nb_lines ,uint32_t nb_rules , uint32_t nb_walkers){
    /*this will be the single most critical calculation of the program. 
    don't. fuck. this. up*/

    uint64_t ret;

    //I'm off by 40 bytes wtf 
    ret=nb_lines * sizeof(Line); //line array 
    ret+= nb_lines * sizeof(uint32_t); //flux cur 
    ret+=nb_lines * sizeof(uint32_t); // flux next 

    ret+= nb_nodes *sizeof(GraphTableEntry); //table entries
    ret+=sizeof(GraphTableEntry * ); //gte ptr
    ret+=sizeof(LineArray*); //in gtable 
    ret+=sizeof(WalkerArray*); // in gtable 

    ret+=sizeof(WalkerCurNext*); //walker cur next ptr in gt
    ret+= nb_walkers* sizeof(Walker); //walker array
    ret+= nb_nodes * sizeof(uint32_t); // walker cur in array
    ret+= nb_nodes * sizeof(uint32_t); // walker next in array

    ret+= nb_rules* sizeof(Rule);

    ret+=40* sizeof(uint8_t); 
    //I'm off by 40 bytes from the actual size needed for some 
    //reason ; I don't like doing this but I'll just add em 
    return ret;

}//unsafe af ; not tested !!! don't use 
/*
even if it works check that it doesn't give too much stuff 
it's gonna be hard to take padding into account tbhuint8_t addRule( Tactics * t , uint8_t rule_coeff, rule_fun);
*/
