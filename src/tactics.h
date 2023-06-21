#ifndef tactics_h
#define tactics_h 

#include "common.h"
#include "graph_table.h"


typedef uint8_t(*rule_fun)( GraphTable * gtable, uint32_t node_from, uint32_t walker_index);
typedef uint8_t(*metarule_fun)(GraphTable * gtable, uint32_t node_from , uint32_t choice_coeff, bool * movement_choice);


typedef struct s_rule{
/*
stores the coeff and function pointer of a rule in a tactic; 
coeff is a double between 0 and 1 corresponding to the probability of choosing the number

maybe don't use floats for the coeff cuz they slow af
*/
    uint16_t rule_coeff;  
    rule_fun rule_function;
     
} Rule; 

typedef struct s_meta_rule{
    uint16_t rule_coeff;
    metarule_fun meta_function; //placeholder 
}MetaRule;


typedef struct s_tactics{ 
/*
tactics stores rules in a dynamic array used to determine a decision w a
linear combination 
of the results of each rule or something
*/
    uint32_t capa; //should prolly be static + use uint8_t 
    uint32_t numb;

    MetaRule meta_function;
    Rule * rule_arr;
}Tactics;

uint8_t initTactics(Tactics * t, uint32_t size);
void freeTactics( Tactics * t);

uint8_t parse_args(Tactics *t, uint8_t argc , char ** argv );

uint8_t choose_node( Tactics * t, GraphTable* gtable, uint32_t node_from, uint32_t walker_index);

#ifdef debug_mode


uint8_t rule_rand( GraphTable * gtable , uint32_t node_from, uint32_t walker_index);
uint8_t addRule( Tactics * t , uint8_t rule_coeff, rule_fun);

#endif

#endif 