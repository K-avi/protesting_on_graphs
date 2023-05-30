#include "common.h"

#include "graph.h"
#include "memory.h"
#include <stdint.h>


enum{ //enum of error flags for graph functions 
    G_OK, G_NODE , G_INIT_LINE, G_NULL, G_MALLOC_FAIL, G_NEIGHBOORS_ARR
}GraphErrFlag; 

/* */
static int initLineArr( LineArray * lineArr){
    /*
    initialises an already allocated LineArray by allocating it's array field 
    and setting it's other fields to their default values
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
}//tested ; ok

static void freeLineArr( LineArray * lineArr){
    if(!lineArr) return;

    if(lineArr->array) free(lineArr->array);
    free(lineArr);
}//tested ; ok


static int initNodeArr( NodeArray* nodeArr){
    /*
    initialises an already allocated NodeArray by allocating it's array field 
    and setting it's other fields to their default values
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
}//tested; ok

static void freeNodeArr( NodeArray * nodeArr){
    if(!nodeArr) return;

    if(nodeArr->array) free(nodeArr->array);
    free(nodeArr);
}//tested ;ok

/* graph init / free */

int initGraph(Graph * g){
    /*
    initialises an already allocated graph ; returns 0 on succes ; an error otherwise
    see documentation of initNodeArr and initLineArr
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
}//tested ; ok

void freeGraph(Graph *g){
    if(!g) return; //should report something

    freeNodeArr(g->ArrNode);
    freeLineArr(g->ArrLine);
    
}//tested; ok


int printGraph(Graph * g, FILE * stream){
    /*
    prints a graph in a adjacency list format 
    */

    if(!g){
        return G_NULL;
    }

    if(!(g->ArrNode && g->ArrLine)) return G_NULL;

    NodeArray* nodeArr = g->ArrNode;
    LineArray * lineArr= g->ArrLine;
    
    for(uint32_t i=0; i<nodeArr->currently_in; i++){
        fprintf(stream, "%u:",nodeArr->array[i].node_index);
        for(uint32_t j=0; j<nodeArr->array[i].neighboor_num; j++){

            uint32_t curindex= nodeArr->array[i].first_neighboor_index+j; 
            fprintf(stream, "%u %d:", lineArr->array[curindex].node_index, lineArr->array[curindex].flux);
        }
        fprintf(stream, "\n");
    }

    return G_OK;
}//not tested; placeholder; will prolly change


static int appNode(Graph * g, uint32_t node_index , uint32_t neighboor_num, uint32_t first_neighboor){

    //realloc case 

    NodeArray * arrNode = g->ArrNode;
    arrNode->array[arrNode->currently_in].node_index=node_index; 
    arrNode->array[arrNode->currently_in].neighboor_num=neighboor_num; 
    arrNode->array[arrNode->currently_in++].first_neighboor_index=first_neighboor;

    return G_OK;
}//not done


static int appLine( Graph * g , uint32_t* new_neighboors, uint32_t neighboor_num ){

    return G_OK;
}//not done

int addAdjList( Graph * g, uint32_t node_index, uint32_t neighboor_num, uint32_t * neighboors_index){
    /*
    adds the node of index "node_index" into the graph g . 

    the number of neighboors of node_index is initialised to "neighboor_num"

    the neighboors_index argument contains the list of neighboors of the node_index.

    this list corresponds to all of the lines (a,b) where a is the node_index node and 
    b the neighboors.

    the flux of the new lines is initialised to zero. 
    */  

    if(!g){
        return G_NULL;
    }
    if(!neighboors_index){
        return G_NEIGHBOORS_ARR;
    }

    uint32_t first_neighboor= g->ArrLine->currently_in;
    appNode(g,  node_index, neighboor_num, first_neighboor);

    appLine(g, neighboors_index, neighboor_num);

    //need to handle the realloc case of the index 


    return G_OK;
}//not done
