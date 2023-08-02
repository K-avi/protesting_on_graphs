#include "search.h"
#include "common.h"
#include "graph_table.h"
#include "misc.h"


static uint8_t init_dyn_arr(s_dynamic_array * arr, uint32_t size, uint8_t type){
    /*
    init function for dynamic arrays in search.c ; 
    supports init for STACKs
     and group arrays 
    */
    if(!arr){
        if(type==GA_TYPE){
            report_err("init_GROUP_ARR", GA_NULL); 
            return GA_NULL;
        }else{
            report_err("init_GROUP_ARR", STACK_NULL); 
            return GA_NULL;
        }
    }
    arr->elem = calloc( size, sizeof(uint32_t));
    if(!arr->elem){
        
        if(type == GA_TYPE ){
            report_err("init_GROUP_ARR", GA_MALLOC); 
            return GA_MALLOC;
        }else{
            report_err("init_GROUP_ARR", STACK_MALLOC); 
            return GA_NULL;
        }
    }

    arr->cur_in = 0; 
    arr->size = size;

    return ERRFLAG_OK;
}//not tested ; watch out for flags and shit

static void free_dyn_arr(s_dynamic_array * arr){
    /*
    frees the array in a non null dynamic array ; 
    doesn't free the dynamic array pointer passed 
    as argument.
    */
    if(arr)if(arr->elem)free(arr->elem);
}//ok


static uint8_t app_dyn_arr( GROUP_ARR * arr, uint32_t value , uint8_t type){
    /*
    implicit "cast" of the structure as a group array for the sake of simplicity
    adds a node index to a group arr; 
    handles realloc ; reports errors and stuff
    */
    if(!arr){
        if(type== GA_TYPE){report_err("app_dyn_arr", GA_NULL); return GA_NULL;
        }else {report_err("app_dyn_arr", STACK_NULL); return STACK_NULL;}
    }

    if(arr->cur_in == arr->size ){
       
        arr->size = GROW_CAPACITY(arr->size);
        arr->elem = realloc(arr->elem, arr->size * sizeof(uint32_t));
        if(!arr->elem){
            if(type== GA_TYPE){report_err("app_dyn_arr", GA_REALLOC); return GA_REALLOC;
            }else {report_err("app_dyn_arr", STACK_REALLOC); return STACK_REALLOC;}
        }
    }
    arr->elem[arr->cur_in++] = value;
    return ERRFLAG_OK;
}//not tested ; prolly ok 

static uint8_t pop_dyn_arr(S_STACK * arr , uint32_t * ret){
    /*
    implicit "cast" of the structure as a stack
    */
    if(!arr){report_err("pop_dyn_arr", STACK_NULL); return STACK_NULL;}
    if(!arr){report_err("pop_dyn_arr", ERRFLAG_NULL); return ERRFLAG_NULL;}


    (arr->cur_in)--;
    *ret =  arr->elem[arr->cur_in];

    return STACK_OK;
}///not tested

uint8_t init_search_utils(SEARCH_UTILS * search_util,  uint32_t static_arr_size ){
    /*
    initialises the search_util array reference given 
    */
    if(!search_util){ report_err("init_search_utils", SU_NULL); return SU_NULL;}

    search_util->cur_sid=0;
    search_util->sid_size = static_arr_size;
    
    search_util->sid_array = calloc(static_arr_size, sizeof(uint64_t));
    if(!search_util->sid_array){ report_err("init_search_utils", SU_MALLOC); return SU_MALLOC;}

    uint8_t failure = init_stack(&search_util->stack);
    if(failure){ report_err("init_search_utils", failure); return failure;}

    return SU_OK;
}

void free_search_utils(SEARCH_UTILS * search_util){
    /*
    */
    if(!search_util) return; 
    free_stack(&search_util->stack);
    if(search_util->sid_array) free(search_util->sid_array);
}


uint8_t dfs_limited_nbwk(GraphTable * gt , SEARCH_UTILS * search_util, uint8_t depth,
                         uint32_t starting_node ,int64_t * nb_sum){
    /*
    based on naive iterative dfs 

    used to get sum of nb nodes of every 
    neighbor of the starting node + nb neighbors of 
    the starting node up to a certain depth. 
    depth=1 is looking at you + your neighbors, 
    depth=2 is you, your neighbors, your neighbors neighbors,...
    the sum is passed by reference in the "nb_sum" argument
    */
    if(!gt){ report_err("dfs_limited_nvwk", GT_NULL); return GT_NULL;}
    if(!search_util){ report_err("dfs_limited_nvwk", SU_NULL); return SU_NULL;}
    if(!nb_sum ){report_err("dfs_limited_nvwk", ERRFLAG_NULL); return ERRFLAG_NULL;}

    if(!depth){
        //*nb_sum = 0;
        //report_warning("dfs_limited_nvwk", WA_ZERDEPTH);
        return LDFS_OK;
    }

    search_util->stack.cur_in=0;
    search_util->cur_sid++;
    uint8_t cur_depth = 0;
    
    search_util->sid_array[starting_node] = search_util->cur_sid;
    uint8_t failure = stack_darr(&search_util->stack, starting_node);
    if(failure){report_err("dfs_limited_nvwk", failure) ; return failure;}
    
    while (search_util->stack.cur_in && cur_depth < depth) {

        uint32_t cur_node;
        failure = pop_stack(&search_util->stack, &cur_node);
        if(failure){ report_err("dfs_limited_nvwk", failure); return failure;}

        *nb_sum+= gt->wkcn->cur_num[cur_node];

        for(uint32_t i =0 ; i < gt->entries[cur_node].neighboor_num; i++){//appends neighbors not already 
        //seen AND with walkers to the stack
                    
            uint32_t neighbor_index = (gt->entries[cur_node].first_neighboor_ref+i)->node_index;
            //printf("%lu %lu\n",search_util->cur_sid ,search_util->sid_array[cur_node]  );
            if( search_util->cur_sid != search_util->sid_array[neighbor_index]){ 
                
                search_util->sid_array[neighbor_index]= search_util->cur_sid; //updates index of search
               // printf("app\n");
                stack_darr(&search_util->stack, neighbor_index); 
            }
        }
        cur_depth++;   
    }


    return ERRFLAG_OK;
}//tested; seems ok ; error prone 

uint8_t dfs_limited_flux(GraphTable * gt , SEARCH_UTILS * search_util, uint8_t depth,
                         uint32_t starting_node ,int64_t * flux_sum){
    /*
    based on iterative dfs
    */

    if(!gt){ report_err("dfs_limited_flux", GT_NULL); return GT_NULL;}
    if(!search_util){ report_err("dfs_limited_flux", SU_NULL); return SU_NULL;}
    if(!flux_sum ){report_err("dfs_limited_flux", ERRFLAG_NULL); return ERRFLAG_NULL;}

    if(!depth){
        //*flux_sum = 0;
        //report_warning("dfs_limited_flux", WA_ZERDEPTH);
        return ERRFLAG_OK;
    }

    search_util->stack.cur_in=0;
    search_util->cur_sid++;
    uint8_t cur_depth = 0;
    
    search_util->sid_array[starting_node] = search_util->cur_sid;
    uint8_t failure = stack_darr(&search_util->stack, starting_node);
    if(failure){report_err("weird_name_dfs", failure) ; return failure;}
    
    while (search_util->stack.cur_in && cur_depth < depth) {

        uint32_t cur_node;
        failure = pop_stack(&search_util->stack, &cur_node);
        if(failure){ report_err("dfs_limited_flux", failure); return failure;}

        GraphTableEntry * cur_entry = &gt->entries[cur_node];
        
        for(uint32_t i=0; i<cur_entry->neighboor_num ; i++){
     
            Line * cur_line = cur_entry->first_neighboor_ref +i;

            if(search_util->cur_sid != search_util->sid_array[cur_line->node_index]){ //stacks the stuff                
                search_util->sid_array[cur_line->node_index]= search_util->cur_sid; //updates index of search
                stack_darr(&search_util->stack, cur_line->node_index); 
           
                uint32_t cur_line_index=  cur_line - gt->arrLine->array;
                int32_t flux_from_to= gt->arrLine->cur_flux[cur_line_index];
            
                int64_t flux_to_from= INT64_MIN;
                for(uint32_t j=0; j<gt->entries[cur_line->node_index].neighboor_num; j++){
                    Line * cur_line_inside = gt->entries[cur_line->node_index].first_neighboor_ref+j;
                    if(cur_line_inside->node_index==cur_node){
                        flux_to_from= gt->arrLine->cur_flux[cur_line_inside - gt->arrLine->array];
                        break;
                    }
                }
                if(flux_to_from == INT64_MIN){
                    report_err( "depth_lim_search_align weird case ", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;
                }

                
                *flux_sum += flux_from_to - flux_to_from; 
            }
            cur_depth++;   
        }
    }
    //
    //printf("%ld\n",*flux_sum);
    return ERRFLAG_OK;
}
//tested seems ok; error prone



uint8_t get_group_sizes(GraphTable * gt, SEARCH_UTILS * sutils, char* res_file){
    /*
    creates a trace of the size and number of walkers per group in a file; 

    uses a dfs to do so so O(n) where n is the size of the graph

    the file is formatted where the first value of each line is the number of walkers in the group 
    and the second number is the size of every group ; 

    each couple is separated by a \n
    */
    if(!gt){report_err("get_group_sizes", GT_NULL); return GT_NULL;}
    if(!sutils){ report_err("get_group_sizes", SU_NULL); return SU_NULL;}

    sutils->stack.cur_in=0;
    sutils->cur_sid++;


    int64_t start = -1; 
    uint64_t cur_gp_size = 0 ; //nb nodes in the group 
    uint64_t cur_gp_nbwk = 0 ;  // nbwk in the group

    uint8_t failure;

    FILE * fsearch = fopen(res_file,"w");

    //search loop -> start dfs at each non seen non empty node 
    for(uint32_t i = 0 ; i<gt->wkcn->size; i++){
        
        if(gt->wkcn->cur_num[i]!=0 && sutils->sid_array[i] != sutils->cur_sid){
            start = i; 

            stack_darr(&(sutils->stack), start);
            sutils->sid_array[start]=sutils->cur_sid;

            cur_gp_size = 0 ; //number of nodes in the group 
            cur_gp_nbwk = 0 ; //number of walkers in the group

            while(sutils->stack.cur_in){

                uint32_t cur_node;
                failure = pop_stack(&sutils->stack, &cur_node);
                if(failure){ report_err("get_group_nodes", failure); return failure;}

                cur_gp_nbwk+= gt->wkcn->cur_num[cur_node];
                cur_gp_size++;
               
                for(uint32_t i =0 ; i < gt->entries[cur_node].neighboor_num; i++){//appends neighbors not already 
                //seen AND with walkers to the stack
                    
                    uint32_t neighbor_index = (gt->entries[cur_node].first_neighboor_ref+i)->node_index;
                  
                    if(gt->wkcn->cur_num[neighbor_index] && sutils->sid_array[neighbor_index] != sutils->cur_sid){ 
                      
                        
                        sutils->sid_array[neighbor_index]=sutils->cur_sid;
                        stack_darr(&sutils->stack, neighbor_index);
                    }
                }
            }

            if(cur_gp_nbwk > 1){ //lonely walkers are NOT groups 
                fprintf(fsearch, "%lu %lu \n", cur_gp_nbwk, cur_gp_size);
            }
        }
        
    }
    if(start == -1){report_err("get_group_nodes", GA_NOWK);return GA_NOWK;}

    fclose(fsearch);
    return GA_OK;
}//not tested; 
//error prone  
