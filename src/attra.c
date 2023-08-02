#include "attra.h"
#include "misc.h"

#include "misc_rules.h"

uint8_t rule_attraction( GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils){
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

uint8_t rule_attra_vision( GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils){
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

uint8_t rule_attraction_proba( GraphTable * gtable , uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils){
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

uint8_t rule_repulsion( GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils){
    /*
    chooses the neighbor with the least walkers and goes there 

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
    
    if(cur_arr_min == 0)  { report_err( "rule_agoraphobic no neighbors 2", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 
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

}//tested prolly ok

uint8_t rule_agoraphobic( GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils){
    /*
    chooses the neighbor with the least walkers over but more than 0 walkers

    WARNING : will update the flux field of gtable 

    O(d(node_from)) ~ constant 
    */
    if(!gtable) { report_err( "rule_agoraphobic", GT_NULL ) ; return GT_NULL;} 
  
    if(node_from>gtable->table_size) { report_err( "rule_agoraphobic", GT_SIZE ) ; return GT_SIZE;} 
    if(gtable->entries[node_from].neighboor_num==0) { report_err( "rule_agoraphobic", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 

    int64_t min=INT64_MAX; 
    uint8_t diff=0;
    GraphTableEntry * cur_entry = &gtable->entries[node_from];
    Line * line_to=NULL;

    Line * arr_min[cur_entry->neighboor_num]; 
    memset(arr_min, 0, sizeof(Line *)* cur_entry->neighboor_num);
    uint32_t cur_arr_min = 0 ;

    for(uint32_t i=0; i<cur_entry->neighboor_num ; i++){
        
        Line * cur_line = cur_entry->first_neighboor_ref +i;

         if(( gtable->wkcn->cur_num[cur_line->node_index] != min) && min != INT64_MAX && gtable->wkcn->cur_num[cur_line->node_index]!=0) { 
            if(diff < UINT8_MAX) diff++;
        }

        if(gtable->wkcn->cur_num[cur_line->node_index] < min && gtable->wkcn->cur_num[cur_line->node_index] != 0){
            line_to= cur_line;
            cur_arr_min = 0 ; 
            arr_min[cur_arr_min++] = line_to ;

            min= gtable->wkcn->cur_num[cur_line->node_index]; 
            
        }else if ( gtable->wkcn->cur_num[cur_line->node_index] == min){
            line_to = cur_line ; 
            arr_min[cur_arr_min++] = line_to ;
        }
    }   
    
    
    if(!diff) return rule_rand(gtable,  node_from, walker_index, sutils);
    if(cur_arr_min == 0)  return rule_rand(gtable,  node_from, walker_index, sutils);
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

}//not tested; prolly ok