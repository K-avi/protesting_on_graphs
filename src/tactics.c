#include "tactics.h"
#include "common.h"
#include "graph_table.h"
#include "misc.h"
#include "search.h"
#include "walker.h"
#include <stdbool.h>
#include <stdint.h>
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

static uint8_t rule_attraction_proba( GraphTable * gtable , uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils){
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
        return rule_rand(gtable, node_from, walker_index, sutils);
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
     
    return rule_rand(gtable,node_from,walker_index, sutils);
}//tested; 
//seems ok

uint8_t rule_rand( GraphTable * gtable , uint32_t node_from, uint32_t walker_index, SEARCH_UTILS * sutils){
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

uint8_t rule_teleport( GraphTable * gtable , uint32_t node_from, uint32_t walker_index, SEARCH_UTILS * sutils){
    /*
    chooses a random node in the graph and teleports to it 
    O(1)
    */
    if(!gtable) { report_err( "rule_teleport", GT_NULL ) ; return GT_NULL;} 
    if(node_from>gtable->table_size) { report_err( "rule_teleport", GT_SIZE ) ; return GT_SIZE;} 
    
    Line  line_to = gtable->arrLine->array[rand()%gtable->arrLine->size];
   
    
    gtable->wkcn->next_num[line_to.node_index]++;
    gtable->warray->array[walker_index].index_entry= line_to.node_index;

    return T_OK;
}// not tested, prolly ok tested ; seems ok

static uint8_t rule_attraction( GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils){
    /*
    chooses the neighbor with the most entry and goes there 

    WARNING : will update the flux field of gtable 

    O(d(node_from)) ~ constant 
    */
    if(!gtable) { report_err( "rule_attraction", GT_NULL ) ; return GT_NULL;} 
  
    if(node_from>gtable->table_size) { report_err( "rule_atrraction", GT_SIZE ) ; return GT_SIZE;} 
    if(gtable->entries[node_from].neighboor_num==0) { report_err( "rule_attraction", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 

    int64_t max=INT64_MIN; 
    uint8_t diff=0;
    GraphTableEntry * cur_entry = &gtable->entries[node_from];
    Line * line_to=NULL;

    Line * arr_max[cur_entry->neighboor_num]; 
    memset(arr_max, 0, sizeof(Line *)* cur_entry->neighboor_num);
    uint32_t cur_arr_max = 0 ;

    for(uint32_t i=0; i<cur_entry->neighboor_num ; i++){
        
        Line * cur_line = cur_entry->first_neighboor_ref +i;

         if(( gtable->wkcn->cur_num[cur_line->node_index] != max) && max != INT64_MIN) { 
            if(diff < UINT8_MAX) diff++;
        }

        if(gtable->wkcn->cur_num[cur_line->node_index] > max){
            line_to= cur_line;
            cur_arr_max = 0 ; 
            arr_max[cur_arr_max++] = line_to ;

            max= gtable->wkcn->cur_num[cur_line->node_index]; 
            
        }else if ( gtable->wkcn->cur_num[cur_line->node_index] == max){
            line_to = cur_line ; 
            arr_max[cur_arr_max++] = line_to ;
        }
    }   
    
    if(cur_arr_max == 0)  { report_err( "rule_attraction no neighbors 2", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 
    if(!diff) return rule_rand(gtable,  node_from, walker_index, sutils);
    if(cur_arr_max == 1) { 
        gtable->arrLine->next_flux[ arr_max[0] - gtable->arrLine->array]++;
        gtable->wkcn->next_num[arr_max[0]->node_index]++;
        gtable->warray->array[walker_index].index_entry= arr_max[0]->node_index;
    }else{
        uint32_t r = (rand()%UINT32_MAX)%cur_arr_max;

        gtable->arrLine->next_flux[ arr_max[r] - gtable->arrLine->array]++;
        gtable->wkcn->next_num[arr_max[r]->node_index]++;
        gtable->warray->array[walker_index].index_entry= arr_max[r]->node_index;
    }

    return T_OK;

}//fixed ; works

static uint8_t rule_repulsion( GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils){
    /*
    chooses the neighbor with the most entry and goes there 

    WARNING : will update the flux field of gtable 

    O(d(node_from)) ~ constant 
    */
    if(!gtable) { report_err( "rule_repulsion", GT_NULL ) ; return GT_NULL;} 
  
    if(node_from>gtable->table_size) { report_err( "rule_atrraction", GT_SIZE ) ; return GT_SIZE;} 
    if(gtable->entries[node_from].neighboor_num==0) { report_err( "rule_repulsion", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 

    int64_t min=INT64_MAX; 
    uint8_t diff=0;
    GraphTableEntry * cur_entry = &gtable->entries[node_from];
    Line * line_to=NULL;

    Line * arr_min[cur_entry->neighboor_num]; 
    memset(arr_min, 0, sizeof(Line *)* cur_entry->neighboor_num);
    uint32_t cur_arr_min = 0 ;

    for(uint32_t i=0; i<cur_entry->neighboor_num ; i++){
        
        Line * cur_line = cur_entry->first_neighboor_ref +i;

         if(( gtable->wkcn->cur_num[cur_line->node_index] != min) && min != INT64_MAX) { 
            if(diff < UINT8_MAX) diff++;
        }

        if(gtable->wkcn->cur_num[cur_line->node_index] < min){
            line_to= cur_line;
            cur_arr_min = 0 ; 
            arr_min[cur_arr_min++] = line_to ;

            min= gtable->wkcn->cur_num[cur_line->node_index]; 
            
        }else if ( gtable->wkcn->cur_num[cur_line->node_index] == min){
            line_to = cur_line ; 
            arr_min[cur_arr_min++] = line_to ;
        }
    }   
    
    if(cur_arr_min == 0)  { report_err( "rule_repulsion no neighbors 2", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 
    if(!diff) return rule_rand(gtable,  node_from, walker_index, sutils);
    if(cur_arr_min == 1) { 
        gtable->arrLine->next_flux[ arr_min[0] - gtable->arrLine->array]++;
        gtable->wkcn->next_num[arr_min[0]->node_index]++;
        gtable->warray->array[walker_index].index_entry= arr_min[0]->node_index;
    }else{
        uint32_t r = (rand()%UINT32_MAX)%cur_arr_min;

        gtable->arrLine->next_flux[ arr_min[r] - gtable->arrLine->array]++;
        gtable->wkcn->next_num[arr_min[r]->node_index]++;
        gtable->warray->array[walker_index].index_entry= arr_min[r]->node_index;
    }

    return T_OK;

}//fixed ; works

static uint8_t rule_attra_vision( GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils){
    /*
    chooses the neighbor with the most entry and goes there 

    WARNING : will update the flux field of gtable 

    O(d(node_from)) ~ constant 
    */
    if(!gtable) { report_err( "rule_attraction", GT_NULL ) ; return GT_NULL;} 
  
    if(node_from>gtable->table_size) { report_err( "rule_atrraction", GT_SIZE ) ; return GT_SIZE;} 
    if(gtable->entries[node_from].neighboor_num==0) { report_err( "rule_attraction", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 

    int64_t max=INT64_MIN; 
    uint8_t diff=0;
    GraphTableEntry * cur_entry = &gtable->entries[node_from];
    Line * line_to=NULL;

    Line * arr_max[cur_entry->neighboor_num]; 
    memset(arr_max, 0, sizeof(Line *)* cur_entry->neighboor_num);
    uint32_t cur_arr_max = 0 ;

    for(uint32_t i=0; i<cur_entry->neighboor_num ; i++){


        int64_t sum_nbwk = 0 ;
        
        Line * cur_line = cur_entry->first_neighboor_ref +i;

        sutils->sid_array[node_from] = sutils->cur_sid+1;
        uint8_t failure = dfs_limited_nbwk(gtable, sutils, global_vision, cur_line->node_index, &sum_nbwk);
        
        if(failure){report_err("rule_attra_vision", failure); return failure;}

        if((sum_nbwk != max) && max != INT64_MIN) { 
            if(diff < UINT8_MAX) diff++;
        }

        if(sum_nbwk > max){
            line_to= cur_line;
            cur_arr_max = 0 ; 
            arr_max[cur_arr_max++] = line_to ;

            max= sum_nbwk; 
            
        }else if ( sum_nbwk == max){
            line_to = cur_line ; 
            arr_max[cur_arr_max++] = line_to ;
        }
    }   
    
    if(cur_arr_max == 0)  { report_err( "rule_attraction no neighbors 2", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 
    if(!diff) return rule_rand(gtable,  node_from, walker_index, sutils);
    if(cur_arr_max == 1) { 
        gtable->arrLine->next_flux[ arr_max[0] - gtable->arrLine->array]++;
        gtable->wkcn->next_num[arr_max[0]->node_index]++;
        gtable->warray->array[walker_index].index_entry= arr_max[0]->node_index;
    }else{
        uint32_t r = (rand()%UINT32_MAX)%cur_arr_max;

        gtable->arrLine->next_flux[ arr_max[r] - gtable->arrLine->array]++;
        gtable->wkcn->next_num[arr_max[r]->node_index]++;
        gtable->warray->array[walker_index].index_entry= arr_max[r]->node_index;
    }

    return T_OK;

}//fixed ; works

static uint8_t rule_alignement(GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils){
    /*
    where d(n) is the number of neighbors of n
    O(d(n)*a) where d(n) is the degree of neighboors and a is the 
        print("simul done running") 
    the average of the sum of the degrees of the neighboors of 
    n
    */
    if(!gtable) { report_err( "rule_alignement", GT_NULL ) ; return GT_NULL;} 
  
    if(node_from>gtable->table_size) { report_err( "rule_alignement", GT_SIZE ) ; return GT_SIZE;} 
    if(gtable->entries[node_from].neighboor_num==0) { report_err( "rule_alignement", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 
        
    int64_t flux_max=INT64_MIN;
    GraphTableEntry * cur_entry = &gtable->entries[node_from];
    Line * line_to=NULL;
    uint8_t diff=0;

    Line * arr_max[cur_entry->neighboor_num]; 
    memset(arr_max, 0, sizeof(Line *)* cur_entry->neighboor_num);
    uint32_t cur_arr_max = 0 ;
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

        if((flux_from_to - flux_to_from != flux_max) && flux_max != INT64_MIN) { 
            if(diff < UINT8_MAX) diff++;
        }

        if(flux_from_to - flux_to_from > flux_max){
            line_to= cur_line;
            cur_arr_max = 0 ; 
            arr_max[cur_arr_max++] = line_to ;

            flux_max= flux_from_to- flux_to_from; 
            
        }else if ( flux_from_to - flux_to_from == flux_max){
            line_to = cur_line ; 
            arr_max[cur_arr_max++] = line_to ;
        }
        
    }   

    if(cur_arr_max == 0)  { report_err( "rule_alignement no neighbors 2", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 
    if(!diff) return rule_rand(gtable,  node_from, walker_index, sutils);
    if(cur_arr_max == 1) { 
        gtable->arrLine->next_flux[ arr_max[0] - gtable->arrLine->array]++;
        gtable->wkcn->next_num[arr_max[0]->node_index]++;
        gtable->warray->array[walker_index].index_entry= arr_max[0]->node_index;
    }else{
        uint32_t r = (rand()%UINT32_MAX)%cur_arr_max;

        gtable->arrLine->next_flux[ arr_max[r] - gtable->arrLine->array]++;
        gtable->wkcn->next_num[arr_max[r]->node_index]++;
        gtable->warray->array[walker_index].index_entry= arr_max[r]->node_index;
    }
    return T_OK;
}//fixed; seems ok ; awful tbh 

static uint8_t rule_align_vision(GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils){
    /*
    where d(n) is the number of neighbors of n
    O(d(n)*a) where d(n) is the degree of neighboors and a is the 
        print("simul done running") 
    the average of the sum of the degrees of the neighboors of 
    n
    */
    if(!gtable) { report_err( "rule_alignement", GT_NULL ) ; return GT_NULL;} 
  
    if(node_from>gtable->table_size) { report_err( "rule_alignement", GT_SIZE ) ; return GT_SIZE;} 
    if(gtable->entries[node_from].neighboor_num==0) { report_err( "rule_alignement", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 
        
    int64_t flux_max=INT64_MIN;
    GraphTableEntry * cur_entry = &gtable->entries[node_from];
    Line * line_to=NULL;
    uint8_t diff=0;

    Line * arr_max[cur_entry->neighboor_num]; 
    memset(arr_max, 0, sizeof(Line *)* cur_entry->neighboor_num);
    uint32_t cur_arr_max = 0 ;
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

        int64_t flux_sum; 
        sutils->sid_array[node_from] = sutils->cur_sid+1; //to avoid returning to starting node
        uint8_t failure = dfs_limited_flux(gtable, sutils, global_vision, cur_line->node_index, &flux_sum);
        if(failure){ report_err("jv pàkljbùpwob,", failure); return failure;}

        flux_sum += (int64_t)flux_from_to - (int64_t)flux_to_from; 
        
        if(( flux_sum != flux_max) && flux_max != INT64_MIN) { 
            if(diff < UINT8_MAX) diff++;
        }

        if(flux_sum > flux_max){
            line_to= cur_line;
            cur_arr_max = 0 ; 
            arr_max[cur_arr_max++] = line_to ;

            flux_max= flux_sum;
            
        }else if ( flux_sum == flux_max){
            line_to = cur_line ; 
            arr_max[cur_arr_max++] = line_to ;
        }
        
    }   

    if(cur_arr_max == 0)  { report_err( "rule_alignement no neighbors 2", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 
    if(!diff) return rule_rand(gtable,  node_from, walker_index, sutils);
    if(cur_arr_max == 1) { 
        gtable->arrLine->next_flux[ arr_max[0] - gtable->arrLine->array]++;
        gtable->wkcn->next_num[arr_max[0]->node_index]++;
        gtable->warray->array[walker_index].index_entry= arr_max[0]->node_index;
    }else{
        uint32_t r = (rand()%UINT32_MAX)%cur_arr_max;

        gtable->arrLine->next_flux[ arr_max[r] - gtable->arrLine->array]++;
        gtable->wkcn->next_num[arr_max[r]->node_index]++;
        gtable->warray->array[walker_index].index_entry= arr_max[r]->node_index;
    }
    return T_OK;
}//not tested ; awful tbh 

static uint8_t rule_align_proba(GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils){
    /**/
    if(!gtable) { report_err( "rule_alignement", GT_NULL ) ; return GT_NULL;} 
  
    if(node_from>gtable->table_size) { report_err( "rule_alignement_proba", GT_SIZE ) ; return GT_SIZE;} 
    if(gtable->entries[node_from].neighboor_num==0) { report_err( "rule_alignement_proba", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 

    uint32_t diff = 0; //checks that you see at least two disctinct values

    GraphTableEntry * cur_entry = &gtable->entries[node_from];
    Line * line_cur=NULL;
    double coeff_arr[cur_entry->neighboor_num]; //C99 or higher required
    uint64_t tot=0;
    int64_t min_flux = INT64_MAX;


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

        int64_t flux_sub = flux_from_to - flux_to_from;

        if(flux_sub != 0 ) diff++;

        tot += flux_sub; 
        if(flux_sub < min_flux) min_flux = flux_sub;

        coeff_arr[i] = flux_sub; 

    }
        

    if(!diff){ //case where only every flux is zero 
        return rule_rand(gtable, node_from, walker_index, sutils);
    }
    uint64_t new_tot = tot; 
    min_flux = min_flux < 0 ? - min_flux : min_flux; 
    new_tot+= min_flux*cur_entry->neighboor_num; //not sure abt this 
    if(new_tot == 0 ) return rule_rand(gtable, node_from, walker_index, sutils);
    //makes the choice 
    double randval= (double) ((double)rand()/(double)RAND_MAX);
    for(uint32_t i=0; i<cur_entry->neighboor_num; i++){
        
        coeff_arr[i] = ((double)((double)coeff_arr[i]+(double)min_flux))/ (double) new_tot;
      
        if(randval <= coeff_arr[i]){
            line_cur= cur_entry->first_neighboor_ref+i; //retrieves the line 

            //moves and updates fields 
            gtable->arrLine->next_flux[ line_cur- gtable->arrLine->array]++;
            gtable->wkcn->next_num[line_cur->node_index]++;
            gtable->warray->array[walker_index].index_entry= line_cur->node_index;

            return T_OK;
        }
        
    }  

    //division can round stuff poorly 
    //so I use a special case if I move past the last entry
    //moves and updates fields 
    line_cur= cur_entry->first_neighboor_ref+ (cur_entry->neighboor_num - 1) ; //retrieves the line      
            
    gtable->arrLine->next_flux[ line_cur- gtable->arrLine->array]++;
    gtable->wkcn->next_num[line_cur->node_index]++;
    gtable->warray->array[walker_index].index_entry= line_cur->node_index;
    
    return T_OK;
}// tested ; prolly ok

static uint8_t rule_align_proba_threshold(GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils){
    /*
    similar to alignement with coefficient ; only difference is that the values 
    equal to zero are not used 
    */
    if(!gtable) { report_err( "rule_alignement", GT_NULL ) ; return GT_NULL;} 
  
    if(node_from>gtable->table_size) { report_err( "rule_alignement", GT_SIZE ) ; return GT_SIZE;} 
    if(gtable->entries[node_from].neighboor_num==0) { report_err( "rule_alignement", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 

    uint32_t diff = 0; //checks that you see at least two disctinct values

    GraphTableEntry * cur_entry = &gtable->entries[node_from];
    Line * line_cur=NULL;
    double coeff_arr[cur_entry->neighboor_num]; //C99 or higher required
    uint64_t tot=0;
   


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

        int64_t flux_sub = flux_from_to - flux_to_from;
        if (flux_sub < 0) flux_sub = 0;
        if(flux_sub != 0 ) diff++;
        
        tot += flux_sub; 

        coeff_arr[i] = flux_sub; 

    }
        

    if(!diff){ //case where only every flux is zero 
        return rule_rand(gtable, node_from, walker_index, sutils);
    }else if(diff==1){
        return rule_alignement(gtable, node_from,  walker_index, sutils);
    }

    
    
    if( tot == 0 ) return rule_rand(gtable, node_from, walker_index, sutils);
    //makes the choice 
    double randval= (double) rand()/RAND_MAX;
    for(uint32_t i=0; i<cur_entry->neighboor_num; i++){
        
        coeff_arr[i] = (double)((double)coeff_arr[i]/ (double) tot);
      
        if(randval < coeff_arr[i]){
            line_cur = cur_entry->first_neighboor_ref+i; //retrieves the line 

            //moves and updates fields 
            gtable->arrLine->next_flux[ line_cur- gtable->arrLine->array]++;
            gtable->wkcn->next_num[line_cur->node_index]++;
            gtable->warray->array[walker_index].index_entry= line_cur->node_index;

            return T_OK;
        }
        
    }  
    //division can round stuff poorly 
    //so I use a special case if I move past the last entry
    //moves and updates fields 
    line_cur= cur_entry->first_neighboor_ref+ (cur_entry->neighboor_num - 1) ; //retrieves the line      
            
    gtable->arrLine->next_flux[ line_cur- gtable->arrLine->array]++;
    gtable->wkcn->next_num[line_cur->node_index]++;
    gtable->warray->array[walker_index].index_entry= line_cur->node_index;

    return T_OK;
}// not tested ; prolly ok

static uint8_t rule_align_proba_stalk(GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils){
    /*
    similar to alignement with coefficient; 
    the difference is that the "flux" is just 
    the number of walers in the line (i,j)
    and not (i,j) - (j,i)
    */
    if(!gtable) { report_err( "rule_alignement", GT_NULL ) ; return GT_NULL;} 
  
    if(node_from>gtable->table_size) { report_err( "rule_alignement", GT_SIZE ) ; return GT_SIZE;} 
    if(gtable->entries[node_from].neighboor_num==0) { report_err( "rule_alignement", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 

    uint32_t diff = 0; //checks that you see at least two disctinct values

    GraphTableEntry * cur_entry = &gtable->entries[node_from];
    Line * line_cur=NULL;
    double coeff_arr[cur_entry->neighboor_num]; //C99 or higher required
    uint64_t tot=0;
   


    for(uint32_t i=0; i<cur_entry->neighboor_num ; i++){

        Line * cur_line = cur_entry->first_neighboor_ref +i;
        uint32_t cur_line_index=  cur_line - gtable->arrLine->array;
        uint32_t flux_from_to= gtable->arrLine->cur_flux[cur_line_index];

        if(flux_from_to != 0 ) diff++;

        tot += flux_from_to; 

        coeff_arr[i] = flux_from_to; 

    }
        
    if(!diff){ //case where only every flux is zero 
        return rule_rand(gtable, node_from, walker_index, sutils);
    }else if(diff==1){
        return rule_alignement(gtable, node_from,  walker_index, sutils);
    }

    //makes the choice 
    double randval= (double) rand()/RAND_MAX;
    for(uint32_t i=0; i<cur_entry->neighboor_num; i++){
        
        coeff_arr[i] = (double)((double)coeff_arr[i]/ (double) tot);
      
        if(randval < coeff_arr[i]){
            line_cur = cur_entry->first_neighboor_ref+i; //retrieves the line 

            //moves and updates fields 
            gtable->arrLine->next_flux[ line_cur- gtable->arrLine->array]++;
            gtable->wkcn->next_num[line_cur->node_index]++;
            gtable->warray->array[walker_index].index_entry= line_cur->node_index;

            return T_OK;
        }
        
    }  
    //division can round stuff poorly 
    //so I use a special case if I move past the last entry
    //moves and updates fields 
    line_cur= cur_entry->first_neighboor_ref+ (cur_entry->neighboor_num - 1) ; //retrieves the line      
            
    gtable->arrLine->next_flux[ line_cur- gtable->arrLine->array]++;
    gtable->wkcn->next_num[line_cur->node_index]++;
    gtable->warray->array[walker_index].index_entry= line_cur->node_index;

    return T_OK;
}// not tested ; prolly ok


static uint8_t rule_sleep(GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils){
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

static uint8_t rule_propulsion(GraphTable * gtable, uint32_t node_from, uint32_t walker_index, SEARCH_UTILS * sutils){
    /*the propulsion itself */

    if(!gtable) { report_err( "rule_propulsion", GT_NULL ) ; return GT_NULL;} 
  
    if(node_from>gtable->table_size) { report_err( "rule_propulsion", GT_SIZE ) ; return GT_SIZE;} 
    if(gtable->entries[node_from].neighboor_num==0) { report_err( "rule_propulsion", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;}

    GraphTableEntry * cur_entry = &gtable->entries[node_from];
    Line * line_to=NULL;

    if(cur_entry->neighboor_num==1 &&  //checks if u can't move ; sleep if it's the case 
       gtable->warray->array_prev[walker_index].index_entry==
       (cur_entry->first_neighboor_ref)->node_index){
   
         
        return rule_sleep(gtable, node_from, walker_index,  sutils);
    }

    uint32_t neighbor_num =gtable->entries[node_from].neighboor_num;
    Line * filtered_tab[neighbor_num-1];
    uint32_t cpt=0;

    //generates array of neighbor to pull from 
    for(uint32_t i=0; i<neighbor_num ; i++){
        line_to = (gtable->entries[node_from].first_neighboor_ref +i);
        if( line_to->node_index != gtable->warray->array_prev[walker_index].index_entry)
            filtered_tab[cpt++]= line_to;
    }
    if(cpt!=0){
        line_to = filtered_tab[rand()%cpt];
    }else{ //weird ass case ; happened to me bc a node had "two lines" to the same node which 
    //like some type of multi graph
        
        return rule_sleep(gtable, node_from, walker_index, sutils);
    }
    //update pos / flux
    gtable->arrLine->next_flux[ line_to- gtable->arrLine->array]++;
    gtable->wkcn->next_num[line_to->node_index]++;
   //printf("%u \n",walker_index);
    gtable->warray->array[walker_index].index_entry= line_to->node_index;

    
    return T_OK;
}// tested ;ok ; hate it


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
        }else if (!strncmp(rule_str, "alico", 5)){
            if(app_index>rule_count){ report_err("parse_rule_fn size check", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
            rule_fun_arr[app_index++]= &rule_align_proba;
        }else if (!strncmp(rule_str, "alivi", 5)){
            if(app_index>rule_count){ report_err("parse_rule_fn size check", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
            rule_fun_arr[app_index++]= &rule_align_vision;
        }else if (!strncmp(rule_str, "attvi", 5)){
            if(app_index>rule_count){ report_err("parse_rule_fn size check", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
            rule_fun_arr[app_index++]= &rule_attra_vision;
        }else if (!strncmp(rule_str, "alpos", 5)){
            if(app_index>rule_count){ report_err("parse_rule_fn size check", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
            rule_fun_arr[app_index++]= &rule_align_proba_threshold;
            
        }else if (!strncmp(rule_str, "stalk", 5)){
            if(app_index>rule_count){ report_err("parse_rule_fn size check", PRS_INVALID_FORMAT); return PRS_INVALID_FORMAT;}
            rule_fun_arr[app_index++]= &rule_align_proba_stalk;
            
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
