#include "tactics.h"
#include "common.h"
#include "memory.h"
#include <stdint.h>




uint8_t initTactics(Tactics * t){
    /*
    initialises a Tactics ptr 
    */
    if(!t){
        return T_NULL;
    }

    t->rule_arr=NULL;
    t->rule_arr=(Rule*) GROW_ARRAY(Rule, t->rule_arr, 0, DEFAULT_CAPA_TACTICS);

    t->capa=DEFAULT_CAPA_TACTICS;
    t->numb=0;

    return T_OK;
}//not tested; prolly ok


void freeTactics( Tactics * t){
    /*
    */
    if(!t) return;
    if(t->rule_arr)free(t->rule_arr);
    free(t);
}//not tested ; prolly ok

uint8_t addRule( Tactics * t , double rule_coeff,  uint8_t (*rule_fun)( GraphTable * gtable, uint32_t node_from, uint32_t * index_node_to) ){
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


uint8_t ruleRand( GraphTable * gtable , uint32_t node_from, uint32_t * index_node_to){
    /*
    chooses a random neighboor node at entry node from in a gt;
    
    WARNING : in order to repport errors the node chosen is
    returned by reference in the index_node_to argument
    O(1)
    */
    if(!gtable) return GT_NULL;
    if(!index_node_to) return NDREF_NULL;
    if(node_from>gtable->table_size) return GT_SIZE;

    if(gtable->entries[node_from].neighboor_num==0) return MV_NONEIGHBOORS;
    *index_node_to=(gtable->entries[node_from].first_neighboor_ref+(rand()%gtable->entries[node_from].neighboor_num))->node_index;

    return MV_OK;
}//not tested 
//should make equivalent of the func where I just return the ref 
//this is clearly a rule fn why the hell is it there 

uint8_t ruleMost( GraphTable * gtable, uint32_t node_from, uint32_t * index_node_to){
    /*
    chooses a random neighboor node at entry node from in a gt;
    
    WARNING : in order to repport errors the node chosen is
    returned by reference in the index_node_to argument
    O(1)
    */
    if(!gtable) return GT_NULL;
    if(!index_node_to) return NDREF_NULL;
    if(node_from>gtable->table_size) return GT_SIZE;
    
    int32_t flux_max= INT32_MIN;
    for(uint32_t i=0; i<gtable->entries[node_from].neighboor_num;i++){
        if( (gtable->entries[node_from].first_neighboor_ref+i)->flux_cur > flux_max  )
         *index_node_to= (gtable->entries[node_from].first_neighboor_ref+i)->node_index;
    }

    return MV_OK;
}//not tested 

uint8_t chooseNode( Tactics * t, GraphTable* gtable, uint32_t node_from, uint32_t *index_node_to){
    /*
    chooses a rule from tactics t to use to select a node to move to
    */
    if(!t) return T_NULL;
    if(!gtable) return GT_NULL;
    if(!index_node_to) return NDREF_NULL;

    uint8_t failure= t->rule_arr[0].rule_fun(gtable, node_from, index_node_to);
    if(failure) return failure;


    return MV_OK;
}//placeholder ;only relies on first rule of tactic

uint8_t chooseNodeVar( Tactics * t, GraphTable* gtable, uint32_t node_from, Line * line_ref){
    /*
    chooses a rule from tactics t to use to select a node to move to
    */
    if(!t) return T_NULL;
    if(!gtable) return GT_NULL;
    if(!line_ref) return LINEREF_NULL;

    uint8_t failure= t->rule_arr[0].rule_fun(gtable, node_from, line_ref);
    if(failure) return failure;


    return MV_OK;
}//placeholder ;only relies on first rule of tactic