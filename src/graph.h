#ifndef GRAPH_H 
#define GRAPH_H 


#include "common.h"
#include <stdint.h>
#include <stdio.h>


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

int initGraph(Graph * g);
void freeGraph(Graph * g);

int loadGraph(char* file);
int writeGraph( char *path );

int printGraph( Graph * g, FILE * stream);

#endif