#include "common.h"
#include "graph.h"
#include "memory.h"

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


enum{ //enum of error flags for graph functions 
    G_OK, G_NODE , G_INIT_LINE, G_NULL, G_MALLOC_FAIL, G_NEIGHBOORS_ARR, G_REALLOC_FAIL,
    G_NOWRITE, G_OPENFAIL, G_NOREAD, G_READFAIL, G_PARSE
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
        fprintf(stream, "%u,%u,",nodeArr->array[i].node_index, nodeArr->array[i].neighboor_num);
        for(uint32_t j=0; j<nodeArr->array[i].neighboor_num; j++){

            uint32_t curindex= nodeArr->array[i].first_neighboor_index+j;

            if(j!=nodeArr->array[i].neighboor_num-1) 
                fprintf(stream, "%u:%d;", lineArr->array[curindex].node_index, lineArr->array[curindex].flux );
            else
                fprintf(stream, "%u:%d", lineArr->array[curindex].node_index, lineArr->array[curindex].flux );  
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
}// tested; ok ; useless 



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


static inline bool peek(const char * str, char expected){
    return *str==expected;
}

/* function to handle proper I/O of graphs */

static inline uint8_t appLineOne( Graph * g , uint32_t new_neighboor , int32_t flux ){
    /*
    appends ONE line to the line array of a graph g ; variant of the appLine function ; 
    easier to use in the load function
    */

    if(!g) return G_NULL;

    if(g->ArrLine->capa <= g->ArrLine->currently_in ){
        uint32_t oldCapa= g->ArrLine->capa;
        g->ArrLine->capa=GROW_CAPACITY(g->ArrLine->capa);
        g->ArrLine->array=(Line*) GROW_ARRAY(Line, g->ArrLine->array, oldCapa, g->ArrLine->capa);

        if(!g->ArrLine->array){
            perror("realloc error in appLine\n");
            return G_REALLOC_FAIL;
        }
    }

    g->ArrLine->array[g->ArrLine->currently_in].node_index= new_neighboor;
    g->ArrLine->array[g->ArrLine->currently_in].flux=flux;

    g->ArrLine->currently_in++;

    return G_OK;
}//not tested; should be ok though

uint8_t loadGraph( Graph * g ,char* path){
    /*
    loads into an INITIALISED graph the csv formatted graph written in the 
    file corresponding to the path given.

    warning : this function expects an EMPTY initialised graph; I don't guarantee good
    behavior on graphs already containing elements 
    */
    if(!g){
        perror("in loadGraph , graph passed in NULL\n");
        return G_NULL;
    }

    int acc_right= access(path, R_OK);
    if(acc_right){
        fprintf(stderr, "in loadGraph , can't write at the path given %d\n", acc_right);
        return G_NOREAD;
    }

    FILE * f = fopen(path, "r");
    if(!f) return G_READFAIL;

    char line[256];
    memset(line, 0, 256);
    
    g->ArrLine->currently_in=0;
    g->ArrNode->currently_in=0;
    
    while(fgets(line, 256, f)){
        char * end,*cur=line;

        uint32_t node_index = (uint32_t) strtol(cur, &end, 10);
        if(peek(end, ',') && cur!=end) cur=(++end);
        else return G_PARSE;

        uint32_t neighboor_num = (uint32_t) strtol(cur, &end, 10);
        if(peek(end, ',') && cur!=end) cur=(++end);
        else return G_PARSE;

        uint8_t errflag= appNode(g, node_index, neighboor_num, g->ArrLine->currently_in);
        if(errflag) return errflag; 

        // adds the index and neighboor num to g->nodes
        for(uint32_t i=0; i<neighboor_num; i++){
            /*
            adds the lines / flux
            */
            uint32_t new_neighboor= (uint32_t) strtol(cur, &end, 10);
            if(peek(end, ':') && cur!=end) cur=(++end);
            else {
                printf("here\n");
                return G_PARSE;
            }

            int32_t flux= strtol(cur, &end, 10);
            if(i!=neighboor_num-1){
                if(peek(end, ';') && cur!=end ) cur=(++end);
                else {
                    printf("there\n");
                    return G_PARSE;
                }
            }else{
                if( cur==end) return G_PARSE; 
            }

            uint8_t errflag_in = appLineOne(g, new_neighboor, flux);
            if(errflag_in ) return errflag_in;
        }    
    }
    fclose(f);
    return G_OK;
}//tested; seems ok ; ugly though ; error prone so watch out 

uint8_t writeGraph(Graph * g, char *path ){
    /*
    more of a wrapper on the graph print function than anything else tbh 
    security flaws ; not sure they are really relevant though
    */
    int acc_right=access(path, W_OK);
    if(acc_right){
        fprintf(stderr, "in writeGraph , can't write at the path given %d\n", acc_right);
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

    