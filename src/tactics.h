#ifndef tactics_h
#define tactics_h 

#include "common.h"


typedef struct s_rule{

    double rule_coeff; 
    uint32_t (*rule_fun)() ; //placeholder ; not sure of the parameters needed in this function yet
} Rule; 
//rule should have some kind of function pointer ?

typedef struct s_tactics{ 
/*
tactics is an array of rules used to determine a decision w a linear combination 
of the results of each rule
*/
    uint32_t capa;
    uint32_t numb;

    Rule * rule_arr;
}Tactics;

uint8_t initTactics(Tactics * t);
void freeTactics( Tactics * t);
uint8_t addRule( Tactics * t , Rule * r); //maybe not like this though

#endif 