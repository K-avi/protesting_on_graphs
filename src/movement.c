#include "movement.h"
#include "common.h"
#include "graph_table.h"
#include "tactics.h"
#include <stdint.h>

uint8_t initPos(GraphTable * gtable, WalkerArray *walkerArr){
    /*
    randomly inits the position of the walkers contained 
    in warray passed as argument in the 
    table wtab
    */
    if(!gtable) return GT_NULL;
    if(!walkerArr) return WA_NULL;

    for(uint32_t i=0; i<walkerArr->size;i++){
       uint8_t failure= pushEntryGT( gtable , rand()%gtable->table_size, &walkerArr->array[i]);
       if(failure) return failure;
    }

    return MV_OK;
}// new version , test at some point  ; ok

uint8_t moveEntry(GraphTable * gtable,uint32_t node_from , uint32_t node_to){
    /*
    pops a node in the gtable[ node_from] entry to the gtable[ node_to] entr
    doesn't report wkref== null 
    O(1)
    */  
    if(!gtable) return GT_NULL;

    Walker * popped_ref;
    uint8_t failure= popEntryGT(gtable, node_from, &popped_ref);
    if (failure) return failure;

    failure= pushEntryGT(gtable, node_to, popped_ref);
    if(failure) return failure;

    return MV_OK;
}//new version ; not tested 
//doesnt update flux 

//crap 
uint8_t iterateGen(GraphTable * gtable, Tactics* tactics){
    /*
    the main movement function ; takes a fully intialised gtable and iterates
    1 generation on it the function is in ~O(n*t) where n is the number of 
    walkers and t the time needed to calculate the tactic (select a node) for 1 walker 
    */
    if(!gtable) return GT_NULL;
    
    for(uint32_t i=0; i<gtable->table_size;i++){
        
        for(uint32_t j=0; j<gtable->entries[i].walker_entry.cur_stack.stack_in; j++){
            
            Walker * wkref;
            uint8_t failure = popEntryGT(gtable, i, &wkref);
            if(failure) return failure;
            
            Line line_chosen;
            failure= chooseNodeVar(tactics, gtable, i, &line_chosen);
            if(failure) return failure;
        }
    }
    gtable->curgen++;
    return MV_OK;
}//doesnt update flux atm 
//far from finished 
//actually awful
//damnit
/*
needs to : 

-update flux 
-update gen of walker 
-update gen of nodes or smtg
*/

