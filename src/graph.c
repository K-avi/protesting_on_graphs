#include "common.h"

#include "graph.h"
#include "memory.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum{ //enum of error flags for graph functions 
    G_OK, G_NODE , G_INIT_LINE, G_NULL, G_MALLOC_FAIL
}GraphErrFlag; 

/* */
static int initLineArr( LineArray * lineArr){
    /*
    */
    if(!lineArr){
        perror("invalid call to initLineArr() ; lineArr is NULL\n");
        return G_NULL;
    }
    lineArr->array=NULL;
    lineArr->array= (Line*) GROW_ARRAY(Line, lineArr->array, 0, DEFAULT_CAPA_LINE);

    if(!lineArr->array){
        perror("couldn't allocate memory to lineArr->array in initLineArr\n");
        return G_MALLOC_FAIL;
    }

    lineArr->capa=DEFAULT_CAPA_LINE;
    lineArr->currently_in=0;

    return G_OK;
}

static void freeLineArr( LineArray * lineArr){
    if(!lineArr) return;

    if(lineArr->array) free(lineArr->array);
    free(lineArr);
}


static int initNodeArr( NodeArray* nodeArr){
    /*
    */
    if(!nodeArr){
        perror("invalid call to initLineArr() ; lineArr is NULL\n");
        return G_NULL;
    }
    nodeArr->array=NULL;
    nodeArr->array= (Node*) GROW_ARRAY(Node, nodeArr->array, 0, DEFAULT_CAPA_LINE);

    if(!nodeArr->array){
        perror("couldn't allocate memory to lineArr->array in initLineArr\n");
        return G_MALLOC_FAIL;
    }

    nodeArr->capa=DEFAULT_CAPA_LINE;
    nodeArr->currently_in=0;

    return G_OK;
}

static void freeNodeArr( NodeArray * nodeArr){
    if(!nodeArr) return;

    if(nodeArr->array) free(nodeArr->array);
    free(nodeArr);
}



/* graph init / free */

int initGraph(Graph * g){
    /*
    initialises a NULL graph ; returns 0 on succes ; an error otherwise
    */
    if(!g){
        perror("invalid call to initGraph() ; g is NULL\n");
        return G_NULL;
    }

    g->ArrLine=(LineArray*) malloc(sizeof(LineArray));
    if(!g->ArrLine){
        perror("couldn't allocate memory to g->Arrline in initGraph\n");
        return G_MALLOC_FAIL;
    }


    g->ArrNode= (NodeArray * ) malloc(sizeof(NodeArray));
    if(!g->ArrNode){
        free(g->ArrLine);
        perror("couldn't allocate memory to g->ArrNode in initGraph\n");
        return G_MALLOC_FAIL;
    }
    int ret= 0;

    ret = initLineArr((g)->ArrLine);

    if(ret) {
        freeNodeArr(g->ArrNode);
        freeLineArr(g->ArrLine);

        return ret ;
    }

    ret = initNodeArr( (g)->ArrNode);

    if(ret) {
        freeNodeArr(g->ArrNode);
        freeLineArr(g->ArrLine);
        
        return ret ;
    }

    return ret; 
}

void freeGraph(Graph *g){
    /*
    */
    if(!g) return; //should report something

    freeNodeArr(g->ArrNode);
    freeLineArr(g->ArrLine);
    
}


int printGraph(Graph * g, FILE * stream){
    /*
    prints a graph in a adjacency list format 
    */

    if(!g){
        return G_NULL;
    }

    if(!(g->ArrNode && g->ArrLine)) return G_NULL;

    NodeArray* nodeArr = g->ArrNode;
    
    for(uint32_t i=0; i<nodeArr->currently_in; i++){
        fprintf(stream, "%u:",nodeArr->array[i].node_index);
        for(uint32_t j=0; j<nodeArr->array[i].neighboor_num; j++){
            fprintf(stream, "%u %d:", (*(nodeArr->array[i].first_neighboor+j)).node_index , (*(nodeArr->array[i].first_neighboor+j)).flux) ;
        }
        fprintf(stream, "\n");
    }

    return G_OK;
}