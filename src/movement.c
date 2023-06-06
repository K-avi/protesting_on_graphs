#include "movement.h"
#include "common.h"
#include "graph_table.h"
#include "memory.h"
#include "tactics.h"
#include "walker.h"


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t init_pos(GraphTable * gtable){
    /*inits position of walkers on the graphs; 
    
    warning : puts walker num in the NEXT_NUM array 
    watch out for compatibility with the prepare_ite fn
    */
    if(!gtable) return MV_NULL;

    for(uint32_t i=0; i<gtable->warray->size;i++){
        uint32_t randval= (uint32_t) rand()%gtable->table_size;
        
        gtable->warray->array[i].cur_entry=&gtable->entries[randval];
        gtable->wkcn->next_num[randval]++;
    }
    return MV_OK;
}


uint8_t prepare_ite( GraphTable * gtable){
    /*
    prepartion fn to call after it 
    O(1)
    */
    if(!gtable) return  GT_NULL;

    uint8_t failure= swapWalkerCurNext(gtable->wkcn);
    if(failure) return failure;

    failure= swap_flux_curnext(gtable->arrLine);
    if(failure) return failure;

    gtable->curgen++;
    return MV_OK;

}
uint8_t iterate_once(GraphTable * gtable , Tactics * t){
    /*
     main ite function
    O(w) where w is the number of walkers 
    */

    for(uint32_t i=0; i<gtable->warray->size;i++){
        Line line_to;

        uint8_t failure= choose_node(t, gtable,  gtable->warray->array[i].cur_entry->node_key, &line_to);
        if(failure)return failure;
    
        gtable->wkcn->next_num[line_to.node_index]++;
    }
    return MV_OK;
}

uint8_t iterate_ntimes( GraphTable * gtable, Tactics * tactics, uint32_t iter_num){
    /* O(n*i)*/
    
    uint8_t failure;
    for(uint32_t i=0; i<iter_num; i++){
        
        failure= prepare_ite(gtable);
        if(failure) return failure;
        failure= iterate_once(gtable, tactics);
        if(failure) return failure;
    }

    return MV_OK;
}