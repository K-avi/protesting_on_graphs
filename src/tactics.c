#include "tactics.h"
#include "memory.h"
#include <stdint.h>

enum { 
    T_OK, T_NULL
}TacticsErrFlag;


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
}


void freeTactics( Tactics * t){
    /*
    */
    if(!t) return;
    if(t->rule_arr)free(t->rule_arr);
    free(t);
}

uint8_t addRule( Tactics * t , Rule * r){

    return T_OK;
}