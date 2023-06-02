#include "tactics.h"
#include "memory.h"




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

uint8_t addRule( Tactics * t , double rule_coeff,  uint32_t (*rule_fun)() ){
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

/*
static uint32_t ruleRand( Graph * g, uint32_t from_node ){

    //return neighboors[rand()%neighboor_num];
    return 1;
}//placeholder

*/