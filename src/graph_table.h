#ifndef GRAPH_TABLE_H //placeholder name 
#define GRAPH_TABLE_H

#include "common.h"
#include "walker.h"
#include <stdint.h>

typedef struct s_graph_table_entry GraphTableEntry;

typedef struct s_line{ //prototype for new line struct 
    GraphTableEntry * tabRef; //index of b in the line (a,b) as pointer cuz faster to dereference
    uint32_t node_index; //index of b in the line (a,b) as index cuz faster to print
    
}Line; 
//add a first time seen field to check if flux next n 

typedef struct s_line_array{//STATIC ARRAY 
    uint32_t size; 
    uint32_t cur_in ;//kinda usefull when initialising
    Line * array ;

    uint32_t * cur_flux; 
    uint32_t * next_flux;

}LineArray;


typedef struct s_graph_table{ //table indexed by key of node (it's an int)
    uint32_t table_size ;
    
    GraphTableEntry * entries;
    LineArray*  arrLine;
    WalkerArray * warray;

    WalkerCurNext * wkcn;
    uint32_t curgen ; //generation of the simulation stored in the table cache and updated at
                      // the end of an iteration in the simul

}GraphTable ;

uint8_t initGraphTab(GraphTable *gt, uint32_t arrline_size ,uint32_t table_size,  uint32_t warray_size ,uint32_t curgen );
void freeGraphTab(GraphTable * gt);

uint8_t loadGraphTab(GraphTable *gt, char *path,  uint32_t warray_size,uint32_t curgen);
uint8_t writeGraphTab( GraphTable * gt,  char *path );

uint8_t printGraphTab( GraphTable * gt, FILE * stream);


void printLineArr( LineArray * lineArr, FILE * stream);

uint8_t swap_flux_curnext(LineArray * larr );

#ifdef debug_mode 
// uint8_t appNodeGt (GraphTable * gt, uint32_t node_index , uint32_t neighboor_num, 
                        //    nLine*  first_neighboor_ref);
 //uint8_t appLineGt( GraphTable * gt , uint32_t node_index, int32_t flux );
#endif

#endif