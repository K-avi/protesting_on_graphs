#ifndef GRAPH_TABLE_H //placeholder name 
#define GRAPH_TABLE_H

#include "common.h"
#include "walker.h"

#include <stdint.h>


typedef struct s_graph_table_entry GraphTableEntry;

typedef struct ns_line{ //prototype for new line struct 
    GraphTableEntry * tabRef; //index of b in the line (a,b) as pointer cuz faster to dereference
    uint32_t node_index; //index of b in the line (a,b) as index cuz faster to print
    int32_t flux;
}nLine; 

typedef struct ns_line_array{//STATIC ARRAY 
    uint32_t size; 
    uint32_t cur_in;
    nLine * array ;
//hollup this fella could be dynamic actually it's not very relevant 
//I'll see which one is more convenient
}nLineArray;

struct s_graph_table_entry{

    WalkerTableEntry walker_entry ; //maybe pass em by reference 
    nLine * first_neighboor_ref;
    uint32_t neighboor_num;

};


typedef struct s_graph_table{ //table indexed by key of node (it's an int)
    uint32_t table_size ;
    
    GraphTableEntry * entries;
    nLineArray*  arrLine;

}GraphTable ;


uint8_t initGraphTab(GraphTable * gt,uint32_t arrline_size, uint32_t table_size, uint32_t we_size);
void freeGraphTab(GraphTable * gt);

uint8_t loadGraphTab(GraphTable * gt ,char* path, uint32_t we_size); //should prolly take Graph * as arg and return succes
uint8_t writeGraphTab( GraphTable * gt,  char *path );

uint8_t printGraphTab( GraphTable * gt, FILE * stream);

uint8_t addEntryGT( GraphTable* gtable, uint32_t index_entry, Walker * walker_ref ); 
uint8_t removeEntryGT( GraphTable * gtable, uint32_t index_entry, uint32_t walker_id);


#ifdef debug_mode 
 uint8_t appNodeGt (GraphTable * gt, uint32_t node_index , uint32_t neighboor_num, \
                            nLine*  first_neighboor_ref);
 uint8_t appLineGt( GraphTable * gt , uint32_t node_index, int32_t flux );
#endif

#endif