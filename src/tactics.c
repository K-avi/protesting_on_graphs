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

uint8_t addRule( Tactics * t , uint8_t rule_coeff,  uint8_t (*rule_fun)( GraphTable * gtable, uint32_t node_from, Line * line_ref) ){
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

    t->rule_arr[t->numb].rule_coeff= rule_coeff;
    t->rule_arr[t->numb].rule_fun=rule_fun;
    t->numb++;

    return T_OK;
}//not tested 
//assumed that parameters are passed correctly ; do not call outside of parse args

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
 
    line_ref->node_index= &(gtable->entries[line_ref->node_index])- gtable->entries;
    gtable->arrLine->next_flux[ lf- gtable->arrLine->array]++;

    return T_OK;
}// seems ok
//changed watch out 


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
        if( uint_coeff<= t->rule_arr[i].rule_coeff){
             uint8_t failure= t->rule_arr[i].rule_fun(gtable, node_from, line_ref);
             if(failure) report_err("choose_node err1", failure);
             return failure;
        }
    }  
    report_err("choose_node err2", T_CANTCHOOSE);
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


uint8_t parse_rule_coeff( uint8_t argc , char ** argv, uint8_t * coeff_arr ){
    /*
    puts the coeffs contained in string containing the arguments coeff into 
    an array of coeffs passed as arg

    warning : assumes argv is not null and coeff_arr is the same length as argv
    */
    if(!coeff_arr){ report_err("parse_rule_coeff", PRS_NULL); return PRS_NULL;}

    double sum=0;
    double * index_arr = malloc( argc* sizeof(double));

    for(uint8_t i=0 ; i< argc; i++){
        char * cur_coeff_string= strstr(argv[i], ":");
        if(!cur_coeff_string) { report_err("parse_rule_coeff format err1", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
        cur_coeff_string++;

        char * end=cur_coeff_string; 
        float coeff = strtod(cur_coeff_string, &end);
        sum+=coeff;

        if(cur_coeff_string == end) { report_err("parse_rule_coeff format err2", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
        index_arr[i]=coeff;
    }
    uint16_t casted_sum= 0;

    for(uint8_t i=0; i<argc ; i++){
        coeff_arr[i]= (uint8_t) index_arr[i]/(sum *255) +casted_sum ;
        casted_sum+=coeff_arr[i];
        if(casted_sum>UINT8_MAX) { report_err("parse_rule_coeff", PRS_COEFF); return PRS_COEFF;}
    }
    free(index_arr);
    return T_OK;
}//not tested 

uint8_t parse_rule_fn(  uint8_t argc , char ** argv, 
    uint8_t  (**rule_fun)( GraphTable * gtable, uint32_t node_from, Line * line_to_ref)){
    /*parses a single rule str*/
    if(!rule_fun){ report_err("parse_rule_str", PRS_NULL); return PRS_NULL;}

    for(uint8_t i=0; i<argc ; i++){

        char * rule_str = argv[i];
        if(!strncmp(rule_str, "rand", 4)){
            rule_fun[i]= &rule_rand;
        }else{
            report_err("parse_rule_fn", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;
        }
    }
    
    return PFN_OK;
}//not tested 
 


uint8_t parse_args(Tactics *t, uint8_t argc , char ** argv ){
    /*
    parses a list of rules to append to a tactic t 
    */
    if(!t) {report_err("in parse_args", T_NULL); return T_NULL;}
    uint8_t * coeff_arr = malloc(argc*sizeof(uint8_t));
    if(!coeff_arr) { report_err("parse_args malloc coeff", PRS_NULL); return PRS_NULL ;}

    rule_fun * rule_arr = malloc(argc* sizeof(rule_fun));
    if(!rule_arr) { free(coeff_arr); report_err("parse_args malloc rule", PRS_NULL); return PRS_NULL ;}

    
    uint8_t failure = parse_rule_fn(argc, argv, rule_arr);
    if(failure) { free(coeff_arr); free(rule_arr); report_err("parse_args", failure); return failure;}

    failure = parse_rule_coeff(argc, argv, coeff_arr);
     if(failure) { free(coeff_arr); free(rule_arr); report_err("parse_args", failure); return failure;}
    

    for(uint8_t i=0; i<argc; i++){
        addRule(t, coeff_arr[i], rule_arr[i]);
    }
    free(coeff_arr);
    free(rule_arr);
    return T_OK;
}//not tested 