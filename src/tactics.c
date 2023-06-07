#include "tactics.h"
#include "misc.h"

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
}//tested; ok


void freeTactics( Tactics * t){
    /*
    */
    if(!t) return;
    if(t->rule_arr)free(t->rule_arr);
    
}//tested ;  ok

uint8_t addRule( Tactics * t , double rule_coeff,  uint8_t (*rule_fun)( GraphTable * gtable, uint32_t node_from, Line * line_ref) ){
    /*
    adds a rule function to a tactics structure
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
}//tested ; ok
//doesn't check if the coeff is ok and so on; if I only use it to parse 
//arguments and put them in the fn I might not need to tbh 


uint8_t rule_rand( GraphTable * gtable , uint32_t node_from, Line * line_ref){
    /*
    chooses a random neighboor node at entry node from in a gt;
    
    WARNING : in order to repport errors the node chosen is
    returned by reference in the index_node_to argument

    WARNING : will update the flux field of gtable 
    O(1)
    */
    if(!gtable) return GT_NULL;
    if(node_from>gtable->table_size){printf("node_from %u gt->size %u\n", node_from, gtable->table_size); return GT_SIZE;}
    if(gtable->entries[node_from].neighboor_num==0) return MV_NONEIGHBOORS;

    Line * lf = gtable->entries[node_from].first_neighboor_ref+ rand()%gtable->entries[node_from].neighboor_num;
    *line_ref= *lf;
    line_ref->node_index=lf->tabRef->node_key;
    gtable->arrLine->next_flux[ lf- gtable->arrLine->array]++;

    return T_OK;
}//tested ; seems ok


uint8_t choose_node( Tactics * t, GraphTable* gtable, uint32_t node_from, Line * line_ref){
    /*
    chooses a rule from tactics t to use to select a node to move to
    O(c+t) where c is the complexity of the tactic chosen and t the number of tactics to choose from 
    */
    if(!t) return T_NULL;
    if(!gtable) return GT_NULL;
    //if(!line_ref) return LINEREF_NULL;

    double randval= (double)rand() / (double)RAND_MAX ;
    for(uint32_t i=0; i<t->capa ; i++){
        if( randval< t->rule_arr[i].rule_coeff){
             uint8_t failure= t->rule_arr[i].rule_fun(gtable, node_from, line_ref);
             return failure;
        }
    }  
    return T_CANTCHOOSE;
}//not tested; don't use double they are too slow
/*
the idea of this function is that it assumes that at each rule index 
the float represents the maxval where a choice is made ; 
for exemple ; let's say I have 3 rules; two with 25% chance to be chosen 
and one with 50% .


at the first index, the coeff of the rule will be 25 , 
at the second one it will be 50 and at the last one it will be 100

when I generate a number between 0 and 1 I will check at each rule if 
I selected a number smaller than the current coeff of the rule and will 
use this rule if so 
*/