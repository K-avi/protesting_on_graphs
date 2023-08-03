#ifndef GRAPH_TABLE_H //placeholder name 
#define GRAPH_TABLE_H

#include "../common.h"
#include "walker.h"

typedef struct s_graph_table_entry GraphTableEntry;

typedef struct s_line{ //line struct 
    uint32_t node_index; //index of b in the line (a,b) stored as 
    //I pass by index  bc theyre smaller than pointers 
}Line; 

typedef struct s_line_array{//STATIC ARRAY  
/*the line array stores the adjacency lists of the graphs; 
the idea is to put every adjacency list one after the other 
to access them faster*/

    uint32_t size; //size of the array
    uint32_t cur_in ;//kinda usefull when initialising

    /*these two are simple arrays storing the flux in the line stored at index i of the 
    Line* part of the struct at their index i.
    even though it's theoretically longer to look up the flux this way 
    it is significantly faster to update it before the beginning of the next iteration.
    it's in O(1) bc I just need to swap cur and next and flush one of them 
    */
    uint32_t * cur_flux;  //might store on 16bits 
    uint32_t * next_flux;

    Line * array ; //the main array to store the lines of the graph

}LineArray;


typedef struct s_graph_table{ //the main table structure 

    uint32_t table_size ;
    uint32_t curgen ; //generation of the simulation stored in the table cache and updated at
    // the end of an iteration in the simul

    GraphTableEntry * entries; //the entries of the table storing adjacency lists 
    //and other stuff

    LineArray* arrLine; //the big array containing the adjacency lists

    WalkerArray * warray; //the big array containing the walkers
    WalkerCurNext * wkcn;//wrapper array to acces and update the walker position 

}GraphTable ;

uint8_t initGraphTab(GraphTable *gt, uint32_t arrline_size ,uint32_t table_size,  uint32_t warray_size ,uint32_t curgen , uint8_t prop_flag);
void freeGraphTab(GraphTable * gt);

uint8_t loadGraphTab(GraphTable *gt, char *path,  double warray_coeff,uint32_t curgen, uint8_t prop_flag);
uint8_t writeGraphTab( GraphTable * gt,  char *path );

uint8_t printGraphTab( GraphTable * gt, FILE * stream);

uint8_t swap_flux_curnext(LineArray * larr );


#ifdef debug_mode 
void printLineArr( LineArray * lineArr, FILE * stream);


#endif

#endif