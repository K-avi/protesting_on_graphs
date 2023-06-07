#include "walker.h"
#include "misc.h"

/* RAW WALKER ARRAY MANIPULATION */

uint8_t initWalkerArray( WalkerArray * wArray, uint32_t size){
    /*
    initialises a non null wArray

    will memleak if called on an already initialised wArray
    */
    if(!wArray) return  WA_NULL;

    wArray->size=size;

    wArray->array=NULL;
    wArray->array = (Walker*) GROW_ARRAY(Walker, wArray->array, 0, size);
    if(!wArray->array) return WA_ALLOC;

    for(uint32_t i=0; i<size; i++){ //initialises the walkers 
        wArray->array[i].id=i; 
    }

    return WA_OK;
}//ok


void freeWalkerArray(WalkerArray * wArray){
    /*yes*/
    free(wArray->array);
}//ok

void printWarray(WalkerArray *wArray, FILE * stream){
    /*
    */
    if(!wArray) {fprintf(stream,"warray is null\n"); return;}
    fprintf(stream, "walker array of size %u\n", wArray->size);
    for(uint32_t i=0 ; i<wArray->size; i++){
        fprintf(stream,"%u,%u\n", wArray->array[i].id, (wArray->array[i].cur_entry)->node_key  );
    }
}

uint8_t initWalkerCurNext(WalkerCurNext * wkcn , uint32_t size){
    /*init non null wkcn to sizes */

    if(!wkcn) return WKCN_NULL;

    wkcn->cur_num=NULL;
    wkcn->cur_num=GROW_ARRAY(uint32_t, wkcn->cur_num, 0, size);
    if(!wkcn->cur_num) return WKCN_MALLOC;

    wkcn->next_num=NULL;
    wkcn->next_num=GROW_ARRAY(uint32_t, wkcn->next_num, 0, size);
    if(!wkcn->next_num) return WKCN_MALLOC;

    memset(wkcn->next_num, 0, size*sizeof(uint32_t));
    memset(wkcn->cur_num, 0, size*sizeof(uint32_t));

    wkcn->size=size;

    return WKCN_OK;
}//not tested ; prolly ok 

void freeWalkerCurNext(WalkerCurNext *wkcn){
    /*frees content of a wkcn*/
    if(!wkcn) return;
    
    if(wkcn->cur_num){
        free(wkcn->cur_num);
    }
    if(wkcn->next_num){
        free(wkcn->next_num);
    }
}//not tested; prolly ok

uint8_t swapWalkerCurNext(WalkerCurNext *wkcn){
    /* swaps the cur and next array pointers of a wkcn and 
    then flushes the next array to make it ready for 
    an iteration*/

    if(!wkcn) return WKCN_NULL;
    if(!(wkcn->cur_num && wkcn->next_num)) return WKCN_NULL;
    
    uint32_t * tmp = wkcn->cur_num;
    
    wkcn->cur_num= wkcn->next_num;
    wkcn->next_num=tmp;
    memset( wkcn->next_num, 0, wkcn->size*sizeof(uint32_t));

    return WKCN_OK;
}//not tested, prolly ok

void printWKCN( WalkerCurNext * wkcn, FILE * stream){
    if(!wkcn){ fprintf(stream, "wkcn is null\n"); return ;}
    for(unsigned i=0; i<wkcn->size; i++){
        fprintf(stream, "%u,%u,%u\n", i, wkcn->cur_num[i], wkcn->next_num[i]);
    }
}//not tested ; prolly ok 