#include "tactics.h"
#include "common.h"
#include "graph_table.h"
#include "misc.h"
#include "search.h"
#include "walker.h"

#include "align.h"
#include "attra.h"
#include "misc_rules.h"

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

uint8_t addRule( Tactics * t , uint16_t rule_coeff, rule_fun fn){
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

static uint8_t rule_speed_reaction(GraphTable * gtable, uint32_t node_from , uint32_t choice_coeff, bool * movement_choice) {
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
    if( tot >= choice_coeff )*movement_choice= true;
    else *movement_choice= false;

    return T_OK;
}//tested; weird ; seems ok


uint8_t rule_speed_constant(GraphTable * gtable, uint32_t node_from , uint32_t choice_coeff, bool * movement_choice ){
    /*O(1)
    returns true if the value generated is higher than the constant 
    probability to move 
    */
    if(!movement_choice) { report_err( "rule_speed_constant", GT_NULL ) ; return GT_NULL; }
    

    *movement_choice=  ((rand()%UINT32_MAX))+1 > choice_coeff ;
    return T_OK;
}//tested ; seems ok

static uint8_t rule_speed_crowd(GraphTable * gtable, uint32_t node_from , uint32_t choice_coeff, bool * movement_choice) {
    /*
    kinda weird but prolly ok ; 
    maybe shouldn't use double ? 
    and calculate w integers instead I dunno
    */
  
    if(!gtable) { report_err( "rule_speed_crowd", GT_NULL ) ; return GT_NULL;} 
    if(node_from>gtable->table_size) { report_err( "rule_speed_constant", GT_SIZE ) ; return GT_SIZE;}

    double mv_chance =  1.0 /(double)gtable->wkcn->cur_num[node_from] ;
    double mv_choice = (double)rand()/(double)RAND_MAX ;
    
    *movement_choice = mv_choice <= mv_chance;

    return T_OK;
}// tested ; seems ok


uint8_t choose_node( Tactics * t, GraphTable* gtable, uint32_t node_from, uint32_t walker_index, SEARCH_UTILS * search_util){
    /*
    chooses a rule from tactics t to use to select a node to move to
    O(c+t) where c is the complexity of the tactic chosen and t the number of tactics to choose from 
    */
    if(!t) { report_err( "choose_node", T_NULL ) ; return T_NULL;} 
    if(!gtable){ report_err( "choose_node", GT_NULL ) ; return GT_NULL;} 
    //if(!line_ref) return LINEREF_NULL;

    double randval= (double)rand() / (double)RAND_MAX ;
    uint16_t uint_coeff = (uint16_t)  ( (double)UINT16_MAX * randval);
    bool mv_check ;
    uint8_t failure = t->meta_function.meta_function(gtable, node_from, t->meta_function.rule_coeff ,&mv_check);
    if(failure){ report_err("choose_node", failure); return failure;}

    if(!mv_check){   
        
        uint32_t prev_index= gtable->warray->array[walker_index].index_entry;
        return rule_sleep(gtable,  node_from,  walker_index, NULL);

        if(gtable->warray->array_prev)//only used when propulsion is active
            gtable->warray->array_prev[walker_index].index_entry=prev_index;
    }

    for(uint32_t i=0; i<t->numb ; i++){
        if( uint_coeff<= t->rule_arr[i].rule_coeff){

            uint32_t prev_index= gtable->warray->array[walker_index].index_entry;

            failure= t->rule_arr[i].rule_function(gtable, node_from, walker_index, search_util);  
            if(failure){ report_err("choose_node err1", failure); return failure;
            }
            if(gtable->warray->array_prev+walker_index > gtable->warray->array_prev+gtable->warray->size){
                 report_err("choose_node err weird", failure); return failure;
            }
           if(gtable->warray->array_prev!=NULL){
            ((Walker*)(gtable->warray->array_prev+walker_index))->index_entry=prev_index;
            
            }
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

it also checks if u actually have the right to move 
*/


uint8_t parse_rule_coeff( uint8_t argc , char ** argv, uint8_t rule_count, uint16_t * coeff_arr ){
    /*
    puts the coeffs contained in string containing the arguments coeff into 
    an array of coeffs passed as arg

    warning : assumes argv is not null and coeff_arr is the same length as argv
    */
    if(!coeff_arr){ report_err("parse_rule_coeff", PRS_NULL); return PRS_NULL;}

    double sum=0;
    double * index_arr = malloc( rule_count* sizeof(double));
    uint8_t arr_append_index=0;
   
    for(uint8_t i=0 ; i< argc; i++){
        if(argv[i][0]!='m'){ //ignores meta rules
          
            char * cur_coeff_string= strstr(argv[i], ":");
            if(!cur_coeff_string) { report_err("parse_rule_coeff format err1", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
            cur_coeff_string++;

            char * end=cur_coeff_string; 
            double coeff = strtod(cur_coeff_string, &end);
            sum+=coeff;

            if(cur_coeff_string == end) { report_err("parse_rule_coeff format err2", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
            if(arr_append_index==rule_count){ report_err("parse_rule_coeff format err3", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}

            index_arr[arr_append_index++]=coeff; 
        }
    }
    uint32_t casted_sum= 0;
    if(!sum){report_warning("parse_rule_coeff", PRS_NOSUM); coeff_arr[0]=UINT16_MAX; return T_OK;}

    for(uint8_t i=0; i<rule_count ; i++){
        if(casted_sum>UINT16_MAX) { fprintf(stderr,"casted sum %u\n", casted_sum); report_err("parse_rule_coeff", PRS_COEFF); return PRS_COEFF;}

        coeff_arr[i]= (uint16_t)( ( (double)((double)index_arr[i]/(double)sum)* (double)UINT16_MAX )+casted_sum );
        //printf("i:%u coeffarri:%u\n", i, coeff_arr[i]);
        casted_sum=coeff_arr[i];
       
    }
    free(index_arr);
    coeff_arr[rule_count-1]= UINT16_MAX; //bc of rounding last value often ends up beneath 255 so I'm 
    //hardcoding it just in case 

    return T_OK;
}//new version; tested;seems ok ; error prone

static uint8_t parse_rule_fn(  uint8_t argc , char ** argv, uint8_t rule_count, rule_fun * rule_fun_arr, uint8_t * prop_flag){
    /*parses a single rule str*/
    if(!rule_fun_arr){ report_err("parse_rule_str", PRS_NULL); return PRS_NULL;}

    uint8_t app_index= 0;

    for(uint8_t i=0; i<argc ; i++){
        
        char * rule_str = argv[i];
        if(!strncmp(rule_str, "rand", 4)){
          //  printf("rand reached\n");
            if(app_index>rule_count){ report_err("parse_rule_fn size check", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
            rule_fun_arr[app_index++]= &rule_rand;
        }else if (!strncmp(rule_str, "align", 5)){
            if(app_index>rule_count){ report_err("parse_rule_fn size check", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
            rule_fun_arr[app_index++]= &rule_alignement;
        }else if (!strncmp(rule_str, "attra", 5)){
          //   printf("attra reached\n");
            if(app_index>rule_count){ report_err("parse_rule_fn size check", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
            rule_fun_arr[app_index++]= &rule_attraction;
        }else if (!strncmp(rule_str, "attco", 5)){
            if(app_index>rule_count){ report_err("parse_rule_fn size check", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
            rule_fun_arr[app_index++]= &rule_attraction_proba;
        }else if (!strncmp(rule_str, "alibad", 6)){
            if(app_index>rule_count){ report_err("parse_rule_fn size check", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
            rule_fun_arr[app_index++]= &rule_align_proba;
        }else if (!strncmp(rule_str, "alivi", 5)){
            if(app_index>rule_count){ report_err("parse_rule_fn size check", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
            rule_fun_arr[app_index++]= &rule_align_vision;
        }else if (!strncmp(rule_str, "attvi", 5)){
            if(app_index>rule_count){ report_err("parse_rule_fn size check", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
            rule_fun_arr[app_index++]= &rule_attra_vision;
        }else if (!strncmp(rule_str, "alico", 5)){
            if(app_index>rule_count){ report_err("parse_rule_fn size check", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
            rule_fun_arr[app_index++]= &rule_align_proba_threshold;
            
        }else if (!strncmp(rule_str, "aliex", 5)){
            if(app_index>rule_count){ report_err("parse_rule_fn size check", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
            rule_fun_arr[app_index++]= &rule_align_proba_exclusion;
            
        }else if (!strncmp(rule_str, "follow", 6)){
            if(app_index>rule_count){ report_err("parse_rule_fn size check", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
            rule_fun_arr[app_index++]= &rule_align_follow;
            
        }else if (!strncmp(rule_str, "follco", 6)){
            if(app_index>rule_count){ report_err("parse_rule_fn size check", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
            rule_fun_arr[app_index++]= &rule_align_proba_follow;
            
        }else if (!strncmp(rule_str, "propu", 5)){
            if(app_index>rule_count){ report_err("parse_rule_fn size check", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
            rule_fun_arr[app_index++]= &rule_propulsion;
            *prop_flag=1;
        }else if (!strncmp(rule_str, "repu", 4)){
            if(app_index>rule_count){ report_err("parse_rule_fn size check", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
            rule_fun_arr[app_index++]= &rule_repulsion;
            
        }else if (!strncmp(rule_str, "goku", 4)){
            if(app_index>rule_count){ report_err("parse_rule_fn size check", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
            rule_fun_arr[app_index++]= &rule_teleport;
        }else if (!strncmp(rule_str, "phobic", 6)){
            if(app_index>rule_count){ report_err("parse_rule_fn size check", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
            rule_fun_arr[app_index++]= &rule_agoraphobic;
        }else if (!strncmp(rule_str, "mconst", 6) || !strncmp(rule_str, "mprop", 5) || !strncmp(rule_str, "mcrowd", 6)){
            //hello there :)
        }else{
            report_err("parse_rule_fn", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;
        }
    }  
    return PFN_OK;
}// new version; tested; seems ok
 
static uint8_t parse_meta_rules( uint8_t argc, char ** argv, uint8_t* rule_count_ref, Tactics * t){
    /*
    parses the meta rules in the args given to the program ;
    does so by custom parsing the coeff and arg type given ; 
    also substract them from the total of rules given to the program 
    if multiple meta args are given ; only the last one is taken into account
    */
    for(uint8_t i=0 ; i<argc; i++){
  
        if(argv[i][0]=='m'){
            (*rule_count_ref)--;
            if(!strncmp(argv[i], "mconst", 6)){
                t->meta_function.meta_function=&rule_speed_constant;

                char* str_coeff= strstr(argv[i], ":"); 
                if(!str_coeff){ report_err("parse_meta_rules", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
                str_coeff++;

                char * end=str_coeff; 
                double coeff = strtod(str_coeff, &end);
                if(end==str_coeff){ report_err("parse_meta_rules", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}

                coeff= coeff>1.0 ? 1.0 : coeff; //makes sures that nothing goes wrong 
                coeff = coeff < 0.0 ? 0.0 : coeff;
                t->meta_function.rule_coeff= (uint16_t) ( coeff * (double)UINT16_MAX);
           
            }else if (!strncmp(argv[i], "mprop" , 5)){ 
                t->meta_function.meta_function=&rule_speed_reaction;
                char* str_coeff= strstr(argv[i], ":"); 
                if(!str_coeff){ report_err("parse_meta_rules", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
                str_coeff++;
                char * end=str_coeff; 
                double coeff = strtod(str_coeff, &end);
            
                if(end==str_coeff){ report_err("parse_meta_rules", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}

                t->meta_function.rule_coeff= (uint32_t) coeff;
            }else if (!strncmp(argv[i], "mcrowd" , 6)){  //doesn't check for bad stuff 
            //after it ; I don't think it really matters tbh
                t->meta_function.meta_function=&rule_speed_crowd;
                t->meta_function.rule_coeff= 0;
            }else{
                report_err("parse_meta_rules", PRS_INVALID_FORMAT);
                return PRS_INVALID_FORMAT;
            }
        }
    }
    if(*rule_count_ref==argc){//if no meta fn is given ; set to always move 
        t->meta_function.meta_function=&rule_speed_constant;
        t->meta_function.rule_coeff=0;
    }
    return PFN_OK;
}//tested ;seems ok ; error prone
//new version; not tested; seems ok
//so f***** ugly 

uint8_t parse_args(Tactics *t, uint8_t argc , char ** argv, uint8_t* prop_flag ){
    /*
    parses a list of rules to append to a tactic t 
    */
    if(!t) {report_err("in parse_args", T_NULL); return T_NULL;}

    if(argc ==0 ){ //default behavior when no optionnal arguments are given
        addRule(t,UINT16_MAX, &rule_rand);
        t->meta_function.meta_function=&rule_speed_constant;
        t->meta_function.rule_coeff=0;
        return T_OK;
    }
    //parses the 'meta' rules
    uint8_t rule_count=argc;
    uint8_t failure= parse_meta_rules(argc , argv, &rule_count, t);
    if(failure) {  report_err("parse_args", failure); return failure;}


    if(rule_count == 0){ //if only meta rules are given defaults to rand
        addRule(t, UINT16_MAX, &rule_rand);
        return T_OK;
    }

    uint16_t * coeff_arr = malloc(rule_count*sizeof(uint16_t));
    if(!coeff_arr) { report_err("parse_args malloc coeff", PRS_NULL); return PRS_NULL ;}

    rule_fun * rule_arr = malloc(rule_count* sizeof(rule_fun));
    if(!rule_arr) { free(coeff_arr); report_err("parse_args malloc rule", PRS_NULL); return PRS_NULL ;}

    
    failure = parse_rule_fn(argc, argv, rule_count,rule_arr, prop_flag);
    if(failure) { free(coeff_arr); free(rule_arr); report_err("parse_args", failure); return failure;}

    failure = parse_rule_coeff(argc, argv, rule_count, coeff_arr);
     if(failure) { free(coeff_arr); free(rule_arr); report_err("parse_args", failure); return failure;}
    

    for(uint8_t i=0; i<rule_count; i++){
        addRule(t, coeff_arr[i], rule_arr[i]);
    }
    free(coeff_arr);
    free(rule_arr);
    return T_OK;
}// new version;  tested ; seems ok ; error prone 
