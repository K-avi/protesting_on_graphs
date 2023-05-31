#ifndef tactics_h
#define tactics_h 

#include "common.h"
#include "graph.h"
#include <stdint.h>


typedef struct s_rule{

    double rule_coeff; 
    uint32_t (*rule_fun)() ; //placeholder ; not sure of the parameters needed in this function yet
} Rule; 
//rule should have some kind of function pointer ?

typedef struct s_tactics{ 
/*
tactics stores rules in a dynamic array used to determine a decision w a
linear combination 
of the results of each rule or something
*/
    uint32_t capa;
    uint32_t numb;

    Rule * rule_arr;
}Tactics;

uint8_t initTactics(Tactics * t);
void freeTactics( Tactics * t);
uint8_t addRule( Tactics * t , double rule_coeff,  uint32_t (*rule_fun)() ); //maybe not like this though

uint32_t getToNode( Tactics * t , Rule * r, Graph * g, uint32_t from_node); 

#endif 