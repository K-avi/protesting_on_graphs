#include "movement.h"
#include "common.h"
#include "misc.h"


uint8_t init_pos(GraphTable * gtable){
    /*inits position of walkers on the graphs; 
    
    warning : puts walker num in the NEXT_NUM array 
    watch out for compatibility with the prepare_ite fn
    */
    if(!gtable){ report_err("init_pos", GT_NULL); return GT_NULL;}

    for(uint32_t i=0; i<gtable->warray->size;i++){
        uint32_t randval= (uint32_t) rand()%gtable->table_size;
        
        gtable->warray->array[i].cur_entry=&gtable->entries[randval];
        gtable->wkcn->next_num[randval]++;
    }
    return MV_OK;
}//tested ; ok


uint8_t prepare_ite( GraphTable * gtable){
    /*
    prepartion fn to call before iterating; 
    flushes the previous values of flux and position of walkers arrays and 
    swaps the next and current ones 
    O(1)
    */
    if(!gtable){ report_err("prepare_ite", GT_NULL); return GT_NULL;}

    uint8_t failure= swapWalkerCurNext(gtable->wkcn);
    if(failure) { report_err("prepare_ite", failure); return failure;}

    failure= swap_flux_curnext(gtable->arrLine);
    if(failure) { report_err("prepare_ite", failure); return failure;}

    gtable->curgen++;
    return MV_OK;

}//tested ; ok
uint8_t iterate_once(GraphTable * gtable , Tactics * t){
    /*
    iteration function; 
    for every walker; chooses a node ; update relevant fields for next iteration 
    (flux, number of walkers in a node , walker position,... )
    warning : some of the fields are updated by the call to choose node 
    if the choose_node fn doesn't update flux correctly it might cause an issue. 
    While this is highly a questionnable choice, 
    it was also a convenient one
    O(w) where w is the number of walkers 
    */

    for(uint32_t i=0; i<gtable->warray->size;i++){
        Line line_to;
        
        uint8_t failure= choose_node(t, gtable,  gtable->warray->array[i].cur_entry - gtable->entries, &line_to);
        if(failure)return failure;
        gtable->wkcn->next_num[line_to.node_index]++;
        gtable->warray->array[i].cur_entry= &gtable->entries[line_to.node_index];
        
    }
    return MV_OK;
}//tested; seems ok

uint8_t iterate_ntimes( GraphTable * gtable, Tactics * tactics, uint32_t iter_num){
    /*
    O(w*i)
    the main iteration function ; simply iterates i times ; more of a wrapper around 
    iterate_once and prepare it
    */
    if(!gtable){ report_err("iterate_ntimes", GT_NULL); return GT_NULL;}
    if(!tactics){ report_err("iterate_ntimes", T_NULL); return T_NULL;}
    uint8_t failure;
    for(uint32_t i=0; i<iter_num; i++){
        
        failure= prepare_ite(gtable);
        if(failure){report_err("iterate_ntimes prepare ite call", failure); return failure;}
        failure= iterate_once(gtable, tactics);
        if(failure){report_err("iterate_ntimes iterate_once call", failure); return failure;}
    }
    return MV_OK;
}//tested; ok