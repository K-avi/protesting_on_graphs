#ifndef SEARCH_H 
#define SEARCH_H
#include "common.h"
#include "graph_table.h"
#include <stdint.h>


#define GA_SIZE_DEF 8
#define STACK_SIZE_DEF 32


enum{ GA_TYPE, STACK_TYPE};


typedef struct dyn_arr{  //generic uint32 dynamic array 
    uint32_t size , cur_in; 
    uint32_t * elem; 

}s_dynamic_array;

typedef s_dynamic_array GROUP_ARR; //the gp array / stack are typedefs of dynamic arrays
typedef s_dynamic_array S_STACK;


typedef struct search_util{ 

    /*
    an array to store which nodes have been seen
    */

    uint64_t cur_sid; //current search id
    /*
    static array to store ids of search
    used to know wether a node has been seen in a certain 
    search in O(1); prolly better ways to do this 
    */
    uint32_t sid_size;
    uint64_t * sid_array;

    /*
    a single stack for the search ; the idea 
    is to do as few allocs as possible 
    so this guy is used to do every search
    */

    S_STACK stack;

}SEARCH_UTILS;

uint8_t init_search_utils(SEARCH_UTILS * search_util,  uint32_t static_arr_size );
void free_search_utils(SEARCH_UTILS * search_util);

uint8_t dfs_limited_flux(GraphTable * gt , SEARCH_UTILS * search_util, uint8_t depth,
                         uint32_t starting_node ,int64_t * flux_sum);

uint8_t dfs_limited_nbwk(GraphTable * gt , SEARCH_UTILS * search_util, uint8_t depth,
                         uint32_t starting_node ,int64_t * nb_sum);
                         

uint8_t get_group_sizes(GraphTable * gt, SEARCH_UTILS * sutils, char * res_file);

#define init_gp_arr(arr) (init_dyn_arr((arr), GA_SIZE_DEF , GA_TYPE))
#define free_gp_arr(arr) (free_dyn_arr((arr)))
#define app_ga_arr(arr,value)  (app_dyn_arr(  (arr), (value), GA_TYPE))

#define init_stack(arr) (init_dyn_arr((arr), STACK_SIZE_DEF , STACK_TYPE))
#define free_stack(arr) (free_dyn_arr((arr)));
#define stack_darr(arr,value) (app_dyn_arr((arr), (value) , STACK_TYPE))
#define pop_stack(arr,ret) (pop_dyn_arr((arr), (ret)))

#ifdef debug_mode 
#endif 

#endif