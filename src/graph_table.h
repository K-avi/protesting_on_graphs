#ifndef GRAPH_TABLE_H //placeholder name 
#define GRAPH_TABLE_H

#include "common.h"
#include "walker.h"
#include <stdint.h>

typedef struct s_graph_table_entry GraphTableEntry;

typedef struct s_line{ //prototype for new line struct 
    GraphTableEntry * tabRef; //index of b in the line (a,b) as pointer cuz faster to dereference
    uint32_t node_index; //index of b in the line (a,b) as index cuz faster to print
    
    int32_t flux_cur; //flux cur is the value of the current flux in the generation ; the one 
                      //actually usefull when calculating movement 
    int32_t flux_next;  //this is the flux that will be calculated during the simulation iteration 
                        //and swapped with flux_cur at the start of the next iteration 
}Line; 

typedef struct s_line_array{//STATIC ARRAY 
    uint32_t size; 
    uint32_t cur_in;
    Line * array ;
//hollup this fella could be dynamic actually it's not very relevant 
//I'll see which one is more convenient
}LineArray;

struct s_graph_table_entry{

    WalkerTableEntry walker_entry ; //maybe pass em by reference 
    Line * first_neighboor_ref;
    uint32_t neighboor_num;
};


typedef struct s_graph_table{ //table indexed by key of node (it's an int)
    uint32_t table_size ;
    
    GraphTableEntry * entries;
    LineArray*  arrLine;

    uint32_t curgen ; //generation of the simulation stored in the table cache and updated at
                      // the end of an iteration in the simul

}GraphTable ;


uint8_t initGraphTab(GraphTable * gt,uint32_t arrline_size, uint32_t table_size, uint32_t we_size, uint32_t curgen);
void freeGraphTab(GraphTable * gt);

uint8_t loadGraphTab(GraphTable * gt ,char* path, uint32_t we_size , uint32_t curgen); //should prolly take Graph * as arg and return succes
uint8_t writeGraphTab( GraphTable * gt,  char *path );

uint8_t printGraphTab( GraphTable * gt, FILE * stream);

uint8_t pushEntryGT( GraphTable* gtable, uint32_t index_entry, Walker * walker_ref ); 
uint8_t popEntryGT( GraphTable * gtable, uint32_t index_entry,  Walker ** wkref_ret); //might be useless 
void printEntriesGT( GraphTable * gtable, FILE * stream);


#ifdef debug_mode 
// uint8_t appNodeGt (GraphTable * gt, uint32_t node_index , uint32_t neighboor_num, 
                        //    nLine*  first_neighboor_ref);
 //uint8_t appLineGt( GraphTable * gt , uint32_t node_index, int32_t flux );
#endif

#endif