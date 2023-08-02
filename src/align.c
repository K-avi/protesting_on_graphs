#include "align.h"
#include "misc.h"


#include "tactics.h" //remove later

#include "misc_rules.h"


uint8_t rule_alignement(GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils){
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

uint8_t rule_align_follow(GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils){
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

        if((flux_from_to != flux_max) && flux_max != INT64_MIN) { 
            if(diff < UINT8_MAX) diff++;
        }

        if(flux_from_to > flux_max){
            line_to= cur_line;
            cur_arr_max = 0 ; 
            arr_max[cur_arr_max++] = line_to ;

            flux_max= flux_from_to; 
            
        }else if ( flux_from_to  == flux_max){
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


uint8_t rule_align_vision(GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils){
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

uint8_t rule_align_proba(GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils){
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

        coeff_arr[i] = tot; 

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

uint8_t rule_align_proba_threshold(GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils){
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

        coeff_arr[i] = tot; 

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
}// tested ; prolly ok

uint8_t rule_align_proba_follow(GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils){
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

        coeff_arr[i] = tot; 

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
}//  tested ; prolly ok


uint8_t rule_align_proba_exclusion(GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils){
    /*

    variant of probalistic align where negative values 
    are excluded instead of ignored 
    where d(n) is the number of neighbors of n
    O(d(n)*a) where d(n) is the degree of neighboors and a is the 
        print("simul done running") 
    the average of the sum of the degrees of the neighboors of 
    n
    */
    if(!gtable) { report_err( "rule_alignement", GT_NULL ) ; return GT_NULL;} 
  
    if(node_from>gtable->table_size) { report_err( "rule_alignement", GT_SIZE ) ; return GT_SIZE;} 
    if(gtable->entries[node_from].neighboor_num==0) { report_err( "rule_alignement", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 
        
    
    GraphTableEntry * cur_entry = &gtable->entries[node_from];
   
    uint8_t diff=0;

    Line * arr_pos[cur_entry->neighboor_num]; 
    double coeff_arr[cur_entry->neighboor_num];

    memset(arr_pos, 0, sizeof(Line *)* cur_entry->neighboor_num);
    memset(coeff_arr, 0, sizeof(double)* cur_entry->neighboor_num);

    uint32_t cur_arr_pos = 0 ;

    double sum_pos = 0 ;
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

        if((flux_from_to - flux_to_from > 0) ) { 
            if(diff < UINT8_MAX) diff++;
        }

        if(flux_from_to - flux_to_from >= 0){
           
            arr_pos[cur_arr_pos] = cur_line ;
            

            sum_pos += flux_from_to - flux_to_from;

            coeff_arr[cur_arr_pos] = sum_pos;
            cur_arr_pos++ ; 
            
        }
        
    }   

    if(cur_arr_pos == 0)  { return rule_sleep(gtable, node_from,  walker_index, sutils);} 
    
    if(cur_arr_pos == 1) { 
        gtable->arrLine->next_flux[ arr_pos[0] - gtable->arrLine->array]++;
        gtable->wkcn->next_num[arr_pos[0]->node_index]++;
        gtable->warray->array[walker_index].index_entry= arr_pos[0]->node_index;

        return T_OK;
    }else{ //separate case where everyone is 0 and normal case
        
        if(!diff) //only zeroes
        {
        uint32_t r = (rand()%UINT32_MAX)%cur_arr_pos;

        gtable->arrLine->next_flux[ arr_pos[r] - gtable->arrLine->array]++;
        gtable->wkcn->next_num[arr_pos[r]->node_index]++;
        gtable->warray->array[walker_index].index_entry= arr_pos[r]->node_index;

        return T_OK;
        }
        else{
            double randval = (double)rand()/(double)RAND_MAX;
            
            for (uint32_t i =0 ; i<cur_arr_pos ; i++){

                coeff_arr[i]/=sum_pos;
                if(coeff_arr[i] >= randval){
                    gtable->arrLine->next_flux[ arr_pos[i] - gtable->arrLine->array]++;
                    gtable->wkcn->next_num[arr_pos[i]->node_index]++;
                    gtable->warray->array[walker_index].index_entry= arr_pos[i]->node_index;

                    return T_OK;
                }
            }
        }
    }
    //in case of a floating point rounding error or something like that
    gtable->arrLine->next_flux[ arr_pos[cur_arr_pos-1] - gtable->arrLine->array]++;
    gtable->wkcn->next_num[arr_pos[cur_arr_pos-1]->node_index]++;
    gtable->warray->array[walker_index].index_entry= arr_pos[cur_arr_pos-1]->node_index;

                    
    return T_OK;
}//fixed; seems ok ; awful tbh 