#ifndef RULES_H 
#define RULES_H 



#include "common.h"


typedef struct s_rule Rule; 
//rule should have some kind of function pointer ?

typedef struct s_tactics{

    uint32_t capa;
    uint32_t numb;

    Rule * rule_arr;
}Tactics;

#endif 