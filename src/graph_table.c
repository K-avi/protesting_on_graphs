#include "graph_table.h"
#include "graph.h"
#include "memory.h"
#include "walker.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum{
    GT_OK, GT_NULL, GT_MALLOC, GT_REALLOC, GT_READ, GT_WRITE , GT_INDEX, GT_NOREAD, GT_READFAIL,
    GT_PARSE, GT_NOWRITE , GT_OPENFAIL
}GT_Errflag;

enum{ 
    GE_OK, GE_NULL, GE_MALLOC, GE_REALLOC, GT_ARFULL,
}GE_Errflag; 

uint8_t initGtEntry( GraphTableEntry * gentry , int32_t we_size){
    /*
    initialises a non null GraphTableentry
    */
    if(!gentry) return GE_NULL;

    
    uint8_t failure = initWalkerEntry( &gentry->walker_entry, we_size);
    if(failure) return failure;

    gentry->first_neighboor_ref=NULL;
    gentry->neighboor_num=0;

    return GE_OK;
}//tested; ok

void freeGtEntry( GraphTableEntry * gentry){
    /* frees the content of a gt entry ;*/
    if(!gentry) return;
    freeWalkerEntry(&gentry->walker_entry);
    
}//tested; ok


static uint8_t initnLineArr( nLineArray * lineArr, uint32_t arrline_size){
    /*
    initialises an already allocated lineArr
    */
    if(!lineArr) return GT_NULL;

    lineArr->array=NULL; 
    lineArr->array=(nLine*) GROW_ARRAY(nLine, lineArr->array, 0, arrline_size);

    if(!lineArr->array) return GT_MALLOC;

    lineArr->cur_in=0; 
    lineArr->size=arrline_size;

    return GT_OK;
}// tested ok

static void freeLineArr( nLineArray * lineArr){
    /*
    frees the content of a lineArray
    warning : doesn't free the line array itself
    */
    if(!lineArr) return;
    if(lineArr->array) free(lineArr->array);
} // tested ok

uint8_t initGraphTab(GraphTable *gt, uint32_t arrline_size ,uint32_t table_size, uint32_t we_size ){
    /*
    initialises a non null graph table 
    */
    if(!gt) return GT_NULL;

    gt->table_size= table_size; 
    
    gt->entries=NULL;
    gt->entries=GROW_ARRAY(GraphTableEntry, gt->entries, 0, table_size );

    if(!gt->entries){
        return GT_REALLOC;
    }

    for(uint32_t i=0; i<table_size; i++){
        uint8_t failure = initGtEntry(&gt->entries[i], we_size);
        if(failure) return failure;
    }

    gt->arrLine=malloc(sizeof(nLineArray));
    if(!gt->arrLine) return GT_MALLOC;

    uint8_t failure= initnLineArr(gt->arrLine, arrline_size);
    if(failure) return failure;

    return GT_OK;
}//tested ok;



void freeGraphTab( GraphTable * gt){
    /*
    */
    if(!gt) return;

    for(uint32_t i=0; i<gt->table_size; i++){
        freeGtEntry(&gt->entries[i]);
    }
    free(gt->entries);

    freeLineArr(gt->arrLine);
    free(gt->arrLine);

    return ;
}//tested; ok


 uint8_t appNodeGt (GraphTable * gt, uint32_t node_index , uint32_t neighboor_num, \
                            nLine*  first_neighboor_ref){
    /*
    doesn't if node already set to smtg  ; sets the ref of first elem , and neighboor num 
    of a node at index given
    */
    if(!gt) return GT_NULL;

    if( (gt->table_size-1 < node_index) ) return GT_INDEX;

    gt->entries[node_index].first_neighboor_ref=first_neighboor_ref;
    gt->entries[node_index].neighboor_num=neighboor_num;
    
    return GT_OK;
    
}//not tested 


 uint8_t appLineGt( GraphTable * gt , uint32_t node_index, int32_t flux ){
    /*
    appends ONE line to the line tab of a graph table ;
    checks for valid node 
    */
    if(!gt) return GT_NULL;
    if(!gt->arrLine) return GT_NULL;
    if(node_index> gt->table_size) return GT_INDEX;
    if(gt->arrLine->cur_in==gt->arrLine->size) return GT_ARFULL;

    nLineArray* arrline = gt->arrLine;

    arrline->array[arrline->cur_in].node_index=node_index; 
    arrline->array[arrline->cur_in].flux=flux;
    arrline->array[arrline->cur_in].tabRef=&(gt->entries[node_index]);

    arrline->cur_in++;

    return GT_OK;
}//not tested 

//////////////

uint8_t printGraphTab(GraphTable * gt, FILE * stream){
    /*
    prints the graph of a GraphTab in a adjacency list format 
    probably faster to build one big string and then print it only once instead of calling print 
    every time 
    */
    if(!gt){
        return GT_NULL;
    }
    if(!gt->entries) return GT_NULL;
 
    fprintf(stream, "%u,%u\n", gt->table_size, gt->arrLine->size);
    
    for(uint32_t i=0; i<gt->table_size; i++){
        fprintf(stream, "%u,%u,",i ,gt->entries[i].neighboor_num);
        for(uint32_t j=0; j<gt->entries[i].neighboor_num; j++){
            if(gt->entries->first_neighboor_ref){
                if(j!=gt->entries[i].neighboor_num-1) 
                    fprintf(stream, "%u:%d;", (gt->entries[i].first_neighboor_ref+j)->node_index,\
                         (gt->entries[i].first_neighboor_ref)->flux );
                else
                    fprintf(stream, "%u:%d", (gt->entries[i].first_neighboor_ref+j)->node_index, \
                        (gt->entries[i].first_neighboor_ref+j)->flux);  
            }
        }
        fprintf(stream, "\n");
    }
    return GT_OK;
}// tested; seems ok


static inline bool peek(const char * str, char expected){
    return *str==expected;
}

uint8_t loadGraphTab(GraphTable *gt, char *path, uint32_t we_size){
    /*
    takes a non initialised ; non null , empty gt and loads a graph stored at path into it
    also pass walk table entry size parameter cuz it's better looking than to init w a global var

    if there is more line than space for the graph (i.e : the size of the graph is actually inferior to)
    the number of lines) the lines after the limit of the graph will be ignored
    */
    if(!gt) return GT_NULL;

    FILE * f = fopen(path, "r");
    if(!f) return GT_READFAIL;

    char line[256];
    memset(line, 0, 256);

    if(!fgets(line, 256, f)) {//consume first line to retrieve size of graph and size of arrline
        fclose(f);
        return GT_NOREAD;
    }

    char * end1,*cur1=line;
    uint32_t  table_size = (uint32_t) strtol(cur1, &end1, 10);
    if(peek(end1, ',') && cur1!=end1) cur1=(++end1);
    else{ fclose(f); return GT_PARSE; }
    
    uint32_t arrline_size = (uint32_t) strtol(cur1, &end1, 10);
   
    if(cur1!=end1) cur1=(++end1);
    else{fclose(f); return GT_PARSE;}

    //initialises graph with values consumed on the first line
    uint8_t failure= initGraphTab(gt, arrline_size,  table_size, we_size);
    if(failure) { fclose(f); return failure;}

    uint32_t cpt=0; 
    //keep track of nblines consumed ; report error if nbline consumed higher than counter

    while(fgets(line, 256, f) && cpt<table_size){
        
        cpt++;
        char* end, *cur=line;

        uint32_t node_index = (uint32_t) strtol(cur, &end, 10);
        if(peek(end, ',') && cur!=end) cur=(++end);
        else{return GT_PARSE;}

        uint32_t neighboor_num = (uint32_t) strtol(cur, &end, 10);
        if(peek(end, ',') && cur!=end) cur=(++end);
        else {  return GT_PARSE;}

        uint8_t errflag= appNodeGt(gt, node_index, neighboor_num, &(gt->arrLine->array[gt->arrLine->cur_in]));
        if(errflag) return errflag; 

        for(uint32_t i=0; i<neighboor_num; i++){
            /*
            adds the lines / flux
            */
            uint32_t new_neighboor= (uint32_t) strtol(cur, &end, 10);
            if(peek(end, ':') && cur!=end) cur=(++end);
            else {
              
                return GT_PARSE;
            }

            int32_t flux= strtol(cur, &end, 10);
            if(i!=neighboor_num-1){
                if(peek(end, ';') && cur!=end ) cur=(++end);
                else {
                  
                    return GT_PARSE;
                }
            }else{
                if( cur==end) return GT_PARSE; 
            }

            uint8_t errflag_in = appLineGt(gt, new_neighboor, flux);
            if(errflag_in ) return errflag_in;
        }
    }  
    fclose(f);
    return GT_OK;
}//not tested

uint8_t writeGraphTab(GraphTable * gt, char *path ){
    /*
    more of a wrapper on the graph print function than anything else tbh 
    security flaws ; not sure they are really relevant though
    */
    FILE * f = fopen(path, "w");
    if(!f){
        return GT_OPENFAIL;
    }

    uint8_t succes= printGraphTab(gt, f);
    fclose(f);

    return succes;
}//tested; some security flaws ig