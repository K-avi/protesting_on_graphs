#include "tactics.h"
#include "common.h"
#include "misc.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t initTactics(Tactics * t, uint32_t size){
    /*
    initialises a Tactics ptr 
    */
    if(!t){ report_err("initTactics", T_NULL); return T_NULL;}

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
    if(!t){ report_err("addRule", T_NULL); return T_NULL;}

    if(t->capa == t->numb){
        uint32_t oldCapa= t->capa; 
        t->capa = GROW_CAPACITY(t->capa);
        t->rule_arr=GROW_ARRAY(Rule, t->rule_arr, oldCapa, t->capa);
        if(!t->rule_arr){
            report_err("addRule",T_REALLOC);
            return T_REALLOC;
        }
    }

    double double_capped_coeff = rule_coeff > 1.0 ? 1.0 : rule_coeff;
    double_capped_coeff = double_capped_coeff < 0.0 ? 0.0 : double_capped_coeff;

    if(t->numb==0){
        t->rule_arr[t->numb].rule_coeff= (uint8_t) UINT8_MAX/double_capped_coeff;
        t->rule_arr[t->numb].rule_fun=rule_fun;
        t->numb++;
    }else{
        uint8_t prev_coeff = t->rule_arr[t->numb-1].rule_coeff;
        uint16_t new_coeff= ((uint16_t)prev_coeff + (uint16_t)UINT8_MAX/double_capped_coeff );
        uint8_t uint_capped_coeff=  new_coeff? UINT8_MAX : (uint8_t)new_coeff;

        t->rule_arr[t->numb].rule_coeff= uint_capped_coeff;
        t->rule_arr[t->numb].rule_fun=rule_fun;
        t->numb++;
    }

    return T_OK;
}//new version tested; seems ok ; errro prone though 
/*
checks for some edge cases :

coeff is represented as 8 bit integer. At each index the coeff is the sum of prev coeff + the one 
of the current rules; this allows to stop when you find the correct coeff 

doesn't repport coeffs too high or low; should prolly not add them 
and report error instead 
*/


uint8_t rule_rand( GraphTable * gtable , uint32_t node_from, Line * line_ref){
    /*
    chooses a random neighboor node at entry node from in a gt;
    
    WARNING : in order to repport errors the node chosen is
    returned by reference in the index_node_to argument

    WARNING : will update the flux field of gtable 
    O(1)
    */
    if(!gtable) { report_err( "rule_rand", GT_NULL ) ; return GT_NULL;} 
    if(node_from>gtable->table_size) { report_err( "rule_rand", GT_SIZE ) ; return GT_SIZE;} 
    if(gtable->entries[node_from].neighboor_num==0) { report_err( "rule_rand", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 

    Line * lf = gtable->entries[node_from].first_neighboor_ref+ rand()%gtable->entries[node_from].neighboor_num;
    *line_ref= *lf;
 
    line_ref->node_index=lf->tabRef->node_key;
    gtable->arrLine->next_flux[ lf- gtable->arrLine->array]++;

    return T_OK;
}// seems ok


uint8_t choose_node( Tactics * t, GraphTable* gtable, uint32_t node_from, Line * line_ref){
    /*
    chooses a rule from tactics t to use to select a node to move to
    O(c+t) where c is the complexity of the tactic chosen and t the number of tactics to choose from 
    */
    if(!t) { report_err( "choose_node", T_NULL ) ; return T_NULL;} 
    if(!gtable){ report_err( "choose_node", GT_NULL ) ; return GT_NULL;} 
    //if(!line_ref) return LINEREF_NULL;

    double randval= (double)rand() / (double)RAND_MAX ;
    uint8_t uint_coeff = (uint8_t) UINT8_MAX / randval;

    for(uint32_t i=0; i<t->capa ; i++){
        if( uint_coeff< t->rule_arr[i].rule_coeff){
             uint8_t failure= t->rule_arr[i].rule_fun(gtable, node_from, line_ref);
             if(failure) report_err("choose_node", failure);
             return failure;
        }
    }  
    report_err("choose_node", T_CANTCHOOSE);
    return T_CANTCHOOSE;
}//tested; seems ok ; error prone though
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

uint8_t parse_rule_str( Tactics * t , char * rule_str){
    /*parses a single rule str*/
    if(!rule_str){ report_err("parse_rule_str", PRS_NULL); return PRS_NULL;}

    if(!strncmp(rule_str, "rand", 4)){
        rule_str+=4;

        if(*rule_str!=':') {report_err("parse_rule_str format 0", PRS_INVALID_FORMAT);}
        rule_str++;

        char * end = rule_str;
        double coeff= strtod(rule_str, &end);
        if(end==rule_str) {report_err("parse_rule_str format 0", PRS_INVALID_FORMAT);}
        addRule(t, coeff,&rule_rand);

        return PFN_OK;
    }
    report_err("parse_rule_str format 2", PRS_INVALID_FORMAT);
    return PRS_INVALID_FORMAT;
}//not tested 


uint8_t parse_args(Tactics *t, uint32_t argc , char ** argv ){
    /*
    parses a list of rules to append to a tactic t 
    */
    if(!t) {report_err("in parse_args", T_NULL); return T_NULL;}
    
    if(argc==0){
        addRule(t, 1.0, &rule_rand);
    }else{
        for(uint32_t i=0 ; i<argc; i++){
           uint8_t failure= parse_rule_str(t, argv[i]);
           if(failure) {report_err("parse_args", failure);}
        }
    }
    return T_OK;
}//not tested 