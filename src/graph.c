#include "common.h"

#include "graph.h"
#include "memory.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>


enum{ //enum of error flags for graph functions 
    G_OK, G_NODE , G_INIT_LINE, G_NULL, G_MALLOC_FAIL, G_NEIGHBOORS_ARR, G_REALLOC_FAIL,
    G_NOWRITE, G_OPENFAIL
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

uint8_t initGraph(Graph * g){
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


uint8_t printGraph(Graph * g, FILE * stream){
    /*
    prints a graph in a adjacency list format 

    probably faster to build one big string and then print it only once instead of calling print 
    every time 
    */

    if(!g){
        return G_NULL;
    }

    if(!(g->ArrNode && g->ArrLine)) return G_NULL;

    NodeArray* nodeArr = g->ArrNode;
    LineArray * lineArr= g->ArrLine;
    
    for(uint32_t i=0; i<nodeArr->currently_in; i++){
        fprintf(stream, "%u:%u:",nodeArr->array[i].node_index, nodeArr->array[i].neighboor_num);
        for(uint32_t j=0; j<nodeArr->array[i].neighboor_num; j++){

            uint32_t curindex= nodeArr->array[i].first_neighboor_index+j;

            if(j!=nodeArr->array[i].neighboor_num-1) 
                fprintf(stream, "%u,", lineArr->array[curindex].node_index);
            else
                fprintf(stream, "%u", lineArr->array[curindex].node_index);  
        }
        fprintf(stream, "\n");
    }

    return G_OK;
}//tested; placeholder; will prolly change
//will have to make it build a beeeeeg string and print it once instead of the current implem of it
//at some point

/* functions to add lines to the graph */



static uint8_t appNode(Graph * g, uint32_t node_index , uint32_t neighboor_num, uint32_t first_neighboor){
    /*
    appends a node to the graph g given it's index , number of neighboors and first neighboor index
    */
    if(!g) return G_NULL;

    if(g->ArrNode->currently_in==g->ArrNode->capa){
        uint32_t oldCapa= g->ArrNode->capa;
        g->ArrNode->capa=GROW_CAPACITY(g->ArrNode->capa);
        g->ArrNode->array=(Node*) GROW_ARRAY(Node, g->ArrNode->array, oldCapa, g->ArrNode->capa);

        if(!g->ArrNode->array){
            perror("realloc error in appNode\n");
            return G_REALLOC_FAIL;
        }
    }

    NodeArray * arrNode = g->ArrNode;
    arrNode->array[arrNode->currently_in].node_index=node_index; 
    arrNode->array[arrNode->currently_in].neighboor_num=neighboor_num; 
    arrNode->array[arrNode->currently_in].first_neighboor_index=first_neighboor;

    arrNode->currently_in++;

    return G_OK;
}// tested; ok


static uint8_t appLine( Graph * g , const uint32_t* new_neighboors, uint32_t neighboor_num ){
    /*
    appends the array "new_neighboors" to the line array of a graph g 
    */
    if(!g) return G_NULL;

    if(g->ArrLine->capa <= g->ArrLine->currently_in + neighboor_num){
        uint32_t oldCapa= g->ArrLine->capa;
        g->ArrLine->capa=GROW_CAPACITY(g->ArrLine->capa);
        g->ArrLine->array=(Line*) GROW_ARRAY(Line, g->ArrLine->array, oldCapa, g->ArrLine->capa);

        if(!g->ArrLine->array){
            perror("realloc error in appLine\n");
            return G_REALLOC_FAIL;
        }
    }

    for(uint32_t i=0; i<neighboor_num ; i++){
        
        g->ArrLine->array[g->ArrLine->currently_in].node_index= new_neighboors[i];
        g->ArrLine->array[g->ArrLine->currently_in].flux=0;

        g->ArrLine->currently_in++;
    }


    return G_OK;
}// tested; ok

//will make static after testing
uint8_t addAdjList( Graph * g, uint32_t node_index, uint32_t neighboor_num, uint32_t * neighboors_index){
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
    uint8_t retval= appNode(g,  node_index, neighboor_num, first_neighboor);

    if(retval) {
        perror("error caught in appNode in addAdjList\n");
        return retval;
    }

    retval= appLine(g, neighboors_index, neighboor_num);

    if(retval) {
        perror("error caught in appLine in addAdjList\n");
        return retval;
    }

    return G_OK;
}// tested ;ok


/* function to handle proper I/O of graphs */

Graph * loadGraph(char* file, uint8_t * succes_flag){
    /*
    */

    *succes_flag=G_OK;

    return NULL;
}//not done

uint8_t writeGraph(Graph * g, char *path ){
    /*
    more of a wrapper on the graph print function than anything else tbh 
    security flaws ; not sure they are really relevant though
    */
    if(!access(path, W_OK)){
        perror("in writeGraph , can't write at the path given\n");
        return G_NOWRITE;
    }
    FILE * f = fopen(path, "w");

    if(!f){
        return G_OPENFAIL;
    }

    uint8_t succes= printGraph(g, f);
    fclose(f);

    return succes;
}//tested; some security flaws ig