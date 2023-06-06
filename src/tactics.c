#include "tactics.h"
#include "common.h"
#include "graph_table.h"
#include "memory.h"

#include <stdint.h>
#include <stdlib.h>


uint8_t initTactics(Tactics * t, uint32_t size){
    /*
    initialises a Tactics ptr 
    */
    if(!t){
        return T_NULL;
    }

    t->rule_arr=NULL;
    t->rule_arr=(Rule*) GROW_ARRAY(Rule, t->rule_arr, 0, size);

    t->capa=size;
    t->numb=0;

    return T_OK;
}//not tested; prolly ok


void freeTactics( Tactics * t){
    /*
    */
    if(!t) return;
    if(t->rule_arr)free(t->rule_arr);
    
}//not tested ; prolly ok

uint8_t addRule( Tactics * t , double rule_coeff,  uint8_t (*rule_fun)( GraphTable * gtable, uint32_t node_from, Line * line_ref) ){
    /*
    */
    if(!t) return T_NULL;
    if(t->capa == t->numb){
        uint32_t oldCapa= t->capa; 
        t->capa = GROW_CAPACITY(t->capa);
        t->rule_arr=GROW_ARRAY(Rule, t->rule_arr, oldCapa, t->capa);
        if(!t->rule_arr){
            return T_REALLOC;
        }
    }

    t->rule_arr[t->numb].rule_coeff=rule_coeff;
    t->rule_arr[t->numb].rule_fun=rule_fun;

    t->numb++;

    return T_OK;
}//not tested 


uint8_t rule_rand( GraphTable * gtable , uint32_t node_from, Line * line_ref){
    /*
    chooses a random neighboor node at entry node from in a gt;
    
    WARNING : in order to repport errors the node chosen is
    returned by reference in the index_node_to argument
    O(1)
    */
    if(!gtable) return GT_NULL;
  
    if(node_from>gtable->table_size){printf("node_from %u gt->size %u\n", node_from, gtable->table_size); return GT_SIZE;}

    if(gtable->entries[node_from].neighboor_num==0) return MV_NONEIGHBOORS;

    Line * lf = gtable->entries[node_from].first_neighboor_ref+ rand()%gtable->entries[node_from].neighboor_num;
    *line_ref= *(gtable->entries[node_from].first_neighboor_ref+(rand()%gtable->entries[node_from].neighboor_num));
    
   
    gtable->arrLine->next_flux[ lf- gtable->arrLine->array]++;

    return MV_OK;
}//not tested 
//should make equivalent of the func where I just return the ref 
//this is clearly a rule fn why the hell is it there 

uint8_t choose_node( Tactics * t, GraphTable* gtable, uint32_t node_from, Line * line_ref){
    /*
    chooses a rule from tactics t to use to select a node to move to
    */
    if(!t) return T_NULL;
    if(!gtable) return GT_NULL;
    //if(!line_ref) return LINEREF_NULL;

    uint8_t failure= t->rule_arr[0].rule_fun(gtable, node_from, line_ref);
    if(failure) return failure;

    
    //printf("from choose flux %d id %u\n", line_ref->flux_next, line_ref->node_index);
    return MV_OK;
}//placeholder ;only relies on first rule of tactic 
//not tested at ALL