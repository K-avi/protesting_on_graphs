#include "protester.h"
#include "graph_table.h"
#include "../misc.h"

/* RAW protester ARRAY MANIPULATION */

uint8_t initprotesterArray( protesterArray * wArray, uint32_t size, uint8_t prevset){
    /*
    initialises a non null wArray
    will memleak if called on an already initialised wArray
    */
    if(!wArray) { report_err( "initprotesterArray", WA_NULL ) ; return WA_NULL;} 

    wArray->size=size;

    wArray->array=NULL;
    wArray->array = (protester*) GROW_ARRAY(protester, wArray->array, 0, size);
    if(!wArray->array) { report_err( "initprotesterArray", WA_ALLOC ) ; return WA_ALLOC;} 
    memset(wArray->array, 0, wArray->size*sizeof(protester));

    wArray->array_prev=NULL;
    
        wArray->array_prev = (protester*) GROW_ARRAY(protester, wArray->array_prev, 0, size);
        if(!wArray->array_prev) { report_err( "initprotesterArray", WA_ALLOC ) ; return WA_ALLOC;} 
        memset(wArray->array_prev, 0, wArray->size*sizeof(protester));
    

    return WA_OK;
}//ok


void freeprotesterArray(protesterArray * wArray){
    /*yes*/
    free(wArray->array);
    if(wArray->array_prev) free(wArray->array_prev);
}//ok

void printWarray(const GraphTableEntry * gt_ref,  protesterArray *wArray, FILE * stream){
    /*
    */
    if(!wArray) {fprintf(stream,"warray is null\n"); return;}
    fprintf(stream, "protester array of size %u\n", wArray->size);
    for(uint32_t i=0 ; i<wArray->size; i++){
        fprintf(stream,"%u,%u\n", i,  wArray->array[i].index_entry  );
    }
}

uint8_t initprotesterCurNext(protesterCurNext * wkcn , uint32_t size){
    /*init non null wkcn to sizes */

    if(!wkcn) { report_err( "initprotesterCurNext", WKCN_NULL ) ; return WKCN_NULL;} 

    wkcn->cur_num=NULL;
    wkcn->cur_num=GROW_ARRAY(uint32_t, wkcn->cur_num, 0, size);
    if(!wkcn->cur_num) { report_err( "initprotesterCurNext malloc 1", WKCN_MALLOC ) ; return WKCN_MALLOC;} 

    wkcn->next_num=NULL;
    wkcn->next_num=GROW_ARRAY(uint32_t, wkcn->next_num, 0, size);
        if(!wkcn->next_num) { report_err( "initprotesterCurNext malloc 2", WKCN_MALLOC ) ; return WKCN_MALLOC;} 

    memset(wkcn->next_num, 0, size*sizeof(uint32_t));
    memset(wkcn->cur_num, 0, size*sizeof(uint32_t));

    wkcn->size=size;

    return WKCN_OK;
}//tested ; ok 

void freeprotesterCurNext(protesterCurNext *wkcn){
    /*frees content of a wkcn*/
    if(!wkcn) return;
    
    if(wkcn->cur_num){
        free(wkcn->cur_num);
    }
    if(wkcn->next_num){
        free(wkcn->next_num);
    }
}//tested; ok

uint8_t swapprotesterCurNext(protesterCurNext *wkcn){
    /* swaps the cur and next array pointers of a wkcn and 
    then flushes the next array to make it ready for 
    an iteration*/

    if(!wkcn)  { report_err( "swapprotesterCurNext null check 1", WKCN_NULL ) ; return WKCN_NULL;} 
    if(!(wkcn->cur_num && wkcn->next_num))  { report_err( "swapprotesterCurNext null check 2", WKCN_NULL ) ; return WKCN_NULL;} 
    
    uint32_t * tmp = wkcn->cur_num;
    
    wkcn->cur_num= wkcn->next_num;
    wkcn->next_num=tmp;
    memset( wkcn->next_num, 0, wkcn->size*sizeof(uint32_t));

    return WKCN_OK;
}//tested, ok

void printWKCN( protesterCurNext * wkcn, FILE * stream){
    if(!wkcn){ fprintf(stream, "wkcn is null\n"); return ;}
    for(unsigned i=0; i<wkcn->size; i++){
        fprintf(stream, "%u,%u,%u\n", i, wkcn->cur_num[i], wkcn->next_num[i]);
    }
}//tested ; ok 
