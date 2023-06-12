#include "tactics.h"
#include "common.h"
#include "graph_table.h"
#include "misc.h"
#include "walker.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t initTactics(Tactics * t, uint32_t size){
    /*initialises a Tactics ptr */
    if(!t){ report_err("initTactics", T_NULL); return T_NULL;}

    t->rule_arr=NULL;
    t->rule_arr=(Rule*) GROW_ARRAY(Rule, t->rule_arr, 0, size);

    t->capa=size;
    t->numb=0;

    return T_OK;
}//tested; ok


void freeTactics( Tactics * t){
    /**/
    if(!t) return;
    if(t->rule_arr)free(t->rule_arr);
}//tested ;  ok

uint8_t addRule( Tactics * t , uint8_t rule_coeff, rule_fun fn){
    /*adds a rule function to a tactics structure*/
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
    t->rule_arr[t->numb].rule_function=fn;
    t->numb++;

    return T_OK;
}//tested ;ok
//assumed that parameters are passed correctly ; do not call outside of parse args

uint8_t rule_rand( GraphTable * gtable , uint32_t node_from, uint32_t walker_index){
    /*
    chooses a random neighboor node at entry node from in a gt;
    
    WARNING : in order to repport errors the node chosen is
    returned by reference in the index_node_to argument

    WARNING : will update the flux field of gtable ; position 
    of walker and wkcn->next_num;
    O(1)
    */
    if(!gtable) { report_err( "rule_rand", GT_NULL ) ; return GT_NULL;} 
  
    if(node_from>gtable->table_size) { report_err( "rule_rand", GT_SIZE ) ; return GT_SIZE;} 
    if(gtable->entries[node_from].neighboor_num==0) { report_err( "rule_rand", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 

    Line * line_to = gtable->entries[node_from].first_neighboor_ref+ rand()%gtable->entries[node_from].neighboor_num;
   
    gtable->arrLine->next_flux[ line_to- gtable->arrLine->array]++;
    gtable->wkcn->next_num[line_to->node_index]++;
    gtable->warray->array[walker_index].index_entry= line_to->node_index;

    return T_OK;
}// new version ;tested ; seems ok

uint8_t rule_attraction( GraphTable * gtable, uint32_t node_from , uint32_t walker_index){
    /*
    chooses the neighbor with the most entry and goes there 

    WARNING : will update the flux field of gtable 

    O(d(node_from)) ~ constant 
    */
    if(!gtable) { report_err( "rule_attraction", GT_NULL ) ; return GT_NULL;} 
  
    if(node_from>gtable->table_size) { report_err( "rule_atrraction", GT_SIZE ) ; return GT_SIZE;} 
    if(gtable->entries[node_from].neighboor_num==0) { report_err( "rule_attraction", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 

    int64_t max=INT64_MIN; 
    GraphTableEntry * cur_entry = &gtable->entries[node_from];
    Line * line_to=NULL;
    for(uint32_t i=0; i<cur_entry->neighboor_num ; i++){

        Line * cur_line = cur_entry->first_neighboor_ref +i;
        if( gtable->wkcn->cur_num[cur_line->node_index] > max ) {
            line_to = cur_line; 
            max= gtable->wkcn->cur_num[cur_line->node_index];
        }
    }   
    if(!line_to)  { report_err( "rule_attraction no neighbors 2", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 

    gtable->arrLine->next_flux[ line_to- gtable->arrLine->array]++;
    gtable->wkcn->next_num[line_to->node_index]++;
    gtable->warray->array[walker_index].index_entry= line_to->node_index;

    return T_OK;

}//tested; seems ok


uint8_t rule_attraction_proba( GraphTable * gtable , uint32_t node_from , uint32_t walker_index){
    /*
    calculates the sum of the number of walkers stored on the neighbor nodes 
    of node_from ; associates coefficient proportionnal to the number of 
    walkers they store.
    Generate a random number to pick a node and then moves the walker 
    at walker_index to it

    O(d(node_from)*2) kinda awful
    */
    if(!gtable) { report_err( "rule_attraction", GT_NULL ) ; return GT_NULL;} 
  
    if(node_from>gtable->table_size) { report_err( "rule_atrraction", GT_SIZE ) ; return GT_SIZE;} 
    if(gtable->entries[node_from].neighboor_num==0) { report_err( "rule_attraction", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 

    GraphTableEntry * cur_entry = &gtable->entries[node_from];
    Line * line_cur=NULL;
    double coeff_arr[cur_entry->neighboor_num]; //C99 or higher required
    uint64_t tot=0;

    //calculates coefficient for every neighboor 
    for(uint32_t i=0; i<cur_entry->neighboor_num ; i++){
       line_cur = cur_entry->first_neighboor_ref+i;
       tot+= gtable->wkcn->cur_num[line_cur->node_index];
       coeff_arr[i]=tot;
    }

    if(tot==0){ //selects random node if all of the neighbors are empty
        return rule_rand(gtable, node_from, walker_index);
    }

    //makes the choice 
    double randval= (double) rand()/RAND_MAX;
    for(uint32_t i=0; i<cur_entry->neighboor_num; i++){
        coeff_arr[i]/=tot;
        if(randval< coeff_arr[i]){
            line_cur= cur_entry->first_neighboor_ref+i; //retrieves the line 

            //moves and updates fields 
            gtable->arrLine->next_flux[ line_cur- gtable->arrLine->array]++;
            gtable->wkcn->next_num[line_cur->node_index]++;
            gtable->warray->array[walker_index].index_entry= line_cur->node_index;

            return T_OK;
        }
    }  
    
    return T_CANTCHOOSE;
}//tested; 
//seems ok

uint8_t rule_alignement(GraphTable * gtable, uint32_t node_from , uint32_t walker_index){
    /*
    where d(n) is the number of neighbors of n
    O(d(n)*a) where d(n) is the degree of neighboors and a is the 
    the average of the sum of the degrees of the neighboors of 
    n
    */
    if(!gtable) { report_err( "rule_alignement", GT_NULL ) ; return GT_NULL;} 
  
    if(node_from>gtable->table_size) { report_err( "rule_alignement", GT_SIZE ) ; return GT_SIZE;} 
    if(gtable->entries[node_from].neighboor_num==0) { report_err( "rule_alignement", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 
    
    
    int64_t flux_max=INT64_MIN;
    GraphTableEntry * cur_entry = &gtable->entries[node_from];
    Line * line_to=NULL;

    for(uint32_t i=0; i<cur_entry->neighboor_num ; i++){

        Line * cur_line = cur_entry->first_neighboor_ref +i;
        uint32_t cur_line_index=  cur_line - gtable->arrLine->array;
        uint32_t flux_from_to= gtable->arrLine->cur_flux[cur_line_index];

        int64_t flux_to_from= INT64_MIN;
        for(uint32_t j=0; j<gtable->entries[cur_line->node_index].neighboor_num; j++){
            Line * cur_line_inside = gtable->entries[cur_line->node_index].first_neighboor_ref+j;
            if(cur_line_inside->node_index==node_from){
                flux_to_from= gtable->arrLine->cur_flux[cur_line_inside - gtable->arrLine->array];
                break;
            }
        }
        if(flux_to_from == INT64_MIN){
            report_err( "rule_alignement weird case ", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;
        }

        if(flux_from_to - flux_to_from > flux_max){
            line_to= cur_line;
            flux_max= flux_from_to- flux_to_from;
        }
    }   

    if(!line_to)  { report_err( "rule_alignement no neighbors 2", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 
    gtable->arrLine->next_flux[ line_to- gtable->arrLine->array]++;
    gtable->wkcn->next_num[line_to->node_index]++;
    gtable->warray->array[walker_index].index_entry= line_to->node_index;

    return T_OK;
}// tested; seems ok ; awful tbh


uint8_t rule_sleep(GraphTable * gtable, uint32_t node_from , uint32_t walker_index){
    /*
    will node move the walker 
    does so by setting it's next node to it's current node and not updating any flux.
    
    WARNING: will update the number of elements at the node of index 
    node_from for the next iteration
    O(1)
    */
    if(!gtable) { report_err( "rule_speed_constant", GT_NULL ) ; return GT_NULL;} 
    if(node_from>gtable->table_size) { report_err( "rule_speed_constant", GT_SIZE ) ; return GT_SIZE;} 
    gtable->wkcn->next_num[node_from]++;
   
    return T_OK;
}//tested ; seems ok
//make a metarule

uint8_t rule_speed_reaction(GraphTable * gtable, uint32_t node_from , uint64_t choice_coeff, bool * movement_choice) {
    /*the hell */
    if(!gtable) { report_err( "rule_alignement", GT_NULL ) ; return GT_NULL;} 
    if(node_from>gtable->table_size) { report_err( "rule_speed_constant", GT_SIZE ) ; return GT_SIZE;}

    Line * line_cur=NULL;
    GraphTableEntry * cur_entry = &gtable->entries[node_from]; 
    uint64_t tot=0;

    for(uint32_t i=0; i< gtable->entries->neighboor_num; i++){
        line_cur = cur_entry->first_neighboor_ref+i;
        tot+= gtable->wkcn->cur_num[line_cur->node_index];
    }
    if(tot>choice_coeff) *movement_choice=0;
    *movement_choice=tot;

    return T_OK;
}//not done ; weird 
//make a metarule
//mv choice 0 or 1 if u moove

uint8_t rule_speed_constant(GraphTable * gtable, uint32_t node_from , uint64_t choice_coeff, bool * movement_choice ){
    if(!movement_choice) { report_err( "rule_speed_constant", GT_NULL ) ; return GT_NULL; }
    *movement_choice= (rand()%UINT64_MAX)/choice_coeff;
    return T_OK;
}
//will have to modify to accept the same thing as speed reaction

uint8_t choose_node( Tactics * t, GraphTable* gtable, uint32_t node_from, uint32_t walker_index){
    /*
    chooses a rule from tactics t to use to select a node to move to
    O(c+t) where c is the complexity of the tactic chosen and t the number of tactics to choose from 
    */
    if(!t) { report_err( "choose_node", T_NULL ) ; return T_NULL;} 
    if(!gtable){ report_err( "choose_node", GT_NULL ) ; return GT_NULL;} 
    //if(!line_ref) return LINEREF_NULL;

    double randval= (double)rand() / (double)RAND_MAX ;
    uint8_t uint_coeff = (uint8_t) (UINT8_MAX / randval);

    for(uint32_t i=0; i<t->numb ; i++){
     
        if( uint_coeff<= t->rule_arr[i].rule_coeff){
             uint8_t failure= t->rule_arr[i].rule_function(gtable, node_from, walker_index);
             if(failure) report_err("choose_node err1", failure);
             return failure;
        }
    }  

    report_err("choose_node err2", T_CANTCHOOSE);
    return T_CANTCHOOSE;
}//new version ; tested; seems ok ; error prone
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
    if(!sum){report_err("parse_rule_coeff format err3", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}

    for(uint8_t i=0; i<argc ; i++){
        if(casted_sum>UINT8_MAX) { fprintf(stderr,"casted sum %u\n", casted_sum); report_err("parse_rule_coeff", PRS_COEFF); return PRS_COEFF;}

        coeff_arr[i]= (uint8_t)( ( (double)((double)index_arr[i]/(double)sum)* (double)255 )+casted_sum );
        //printf("i:%u coeffarri:%u\n", i, coeff_arr[i]);
        casted_sum=coeff_arr[i];
        //if(casted_sum>UINT8_MAX) { report_err("parse_rule_coeff", PRS_COEFF); return PRS_COEFF;}
    }
    free(index_arr);
    coeff_arr[argc-1]= 255; //bc of rounding last value often ends up beneath 255 so I'm 
    //hardcoding it just in case 

    return T_OK;
}//tested ; seems ok ; error prone

uint8_t parse_rule_fn(  uint8_t argc , char ** argv, rule_fun * rule_fun_arr){
    /*parses a single rule str*/
    if(!rule_fun_arr){ report_err("parse_rule_str", PRS_NULL); return PRS_NULL;}

    for(uint8_t i=0; i<argc ; i++){

        char * rule_str = argv[i];
        if(!strncmp(rule_str, "rand", 4)){
           
            rule_fun_arr[i]= &rule_rand;
        }else if (!strncmp(rule_str, "align", 5)){
            rule_fun_arr[i]= &rule_alignement;
        }else if (!strncmp(rule_str, "attra", 5)){
            rule_fun_arr[i]= &rule_attraction;
        }else if (!strncmp(rule_str, "attco", 5)){
            rule_fun_arr[i]= &rule_attraction_proba;
        }else if (!strncmp(rule_str, "sleep", 5)){
            rule_fun_arr[i]= &rule_sleep;
        }else{
            report_err("parse_rule_fn", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;
        }
    }  
    return PFN_OK;
}// tested ; seems ok
 


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
}// tested ; seems ok
