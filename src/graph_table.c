#include "graph_table.h"
#include "memory.h"
#include "walker.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum{
    GT_OK, GT_NULL, GT_MALLOC, GT_REALLOC, GT_READ, GT_WRITE , GT_INDEX, GT_NOREAD, GT_READFAIL
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
    if(!gentry) return;
    freeWalkerEntry(&gentry->walker_entry);
    
}//tested; ok

uint8_t initGraphTab(GraphTable *gt, uint32_t table_size, uint32_t we_size , nLineArray * line_arr_ref){
    /*
    initialises a non null graph table 
    */
    if(!gt) return GT_NULL;

    gt->table_size= table_size; 
    gt->arrLine= line_arr_ref;
    
    gt->entries=NULL;
    gt->entries=GROW_ARRAY(GraphTableEntry, gt->entries, 0, table_size );

    if(!gt->entries){
        return GT_REALLOC;
    }

    for(uint32_t i=0; i<table_size; i++){
        uint8_t failure = initGtEntry(&gt->entries[i], we_size);
        if(failure) return failure;
    }

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


 uint8_t appLine( GraphTable * gt , uint32_t node_index, uint32_t flux ){
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
                    fprintf(stream, "%u:%d;", (gt->entries->first_neighboor_ref)->node_index,\
                         (gt->entries->first_neighboor_ref)->flux );
                else
                    fprintf(stream, "%u:%d", (gt->entries->first_neighboor_ref)->node_index, \
                        (gt->entries->first_neighboor_ref)->flux);  
            }
        }
        fprintf(stream, "\n");
    }
    return GT_OK;
}//not tested; based on the print graph function


uint8_t loadGraphTab(GraphTable *gt, char *path){

    /*
    initialises a non nul , non initialised graph table and loads a graph into it 
    */
    if(!gt) return GT_NULL;
    
    int acc_right= access(path, R_OK);
    if(acc_right){
        fprintf(stderr, "in loadGraph , can't write at the path given %d\n", acc_right);
        return GT_NOREAD;
    }

    FILE * f = fopen(path, "r");
    if(!f) return GT_READFAIL;

    char line[256];
    memset(line, 0, 256);

    fclose(f);
    return GT_OK;
}