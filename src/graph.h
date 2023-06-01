#ifndef GRAPH_H 
#define GRAPH_H 


#include "common.h"



//deprecated  : replaced by the graph table struct 
typedef struct s_line{
    uint32_t node_index; 
    int32_t flux;
}Line; 

typedef struct s_node{

    uint32_t node_index;
    uint32_t neighboor_num;

    uint32_t first_neighboor_index ; 
    //using an index instead of a pointer bc it would cause problems after realloc 
    //might use a pointer if realloc becomes unnecessary but it would imply more pre-treatment

}Node; 


typedef struct s_node_array{//dynamic array wrapper of Node
    uint32_t capa; 
    uint32_t currently_in;

    Node * array ;
}NodeArray;

typedef struct s_line_array{//dynamic array wrapper of Node
    uint32_t capa; 
    uint32_t currently_in;

    Line * array ;
}LineArray;



typedef struct s_graph { //Graph is basically a "wrapper" of nodes and lines 

    LineArray * ArrLine; 
    NodeArray * ArrNode;  
    
}Graph; 

uint8_t initGraph(Graph * g);
void freeGraph(Graph * g);

uint8_t loadGraph(Graph * g ,char* path); //should prolly take Graph * as arg and return succes
uint8_t writeGraph( Graph * g,  char *path );

uint8_t printGraph( Graph * g, FILE * stream);



#ifdef debug_mode

uint8_t addAdjList( Graph * g, uint32_t node_index, uint32_t neighboor_num, uint32_t * neighboors_index);
#endif

#endif