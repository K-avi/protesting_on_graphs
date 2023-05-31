#include "walker.h"
#include "memory.h"
#include <stdint.h>

enum{ 
    WTE_OK, WTE_NULL, WTE_REALLOC, WTE_IN
}WalkerTableEntryErrFlags;

static uint8_t initWalkerEntry( WalkerTableEntry * tabEntry ){
    /*
    */
    if(!tabEntry) return WTE_NULL;

    tabEntry->capa= DEFAULT_CAPA_WTE;
    tabEntry->curr_in=0;
    tabEntry->walkers= GROW_ARRAY(Walker, tabEntry->walkers, 0, DEFAULT_CAPA_WTE);

    if(!tabEntry->walkers){
        return WTE_REALLOC;
    }
    return WTE_OK;
}//not tested; prolly ok

static void freeWalkerEntry(WalkerTableEntry * tabEntry){

    if(!tabEntry) return;
    if(tabEntry->walkers) free(tabEntry->walkers);
    free(tabEntry);
}//not tested; prolly ok ; watch out for freeing walkers cuz they might become more intricate n so on

static uint8_t addWalkerEntry( WalkerTableEntry * tabEntry, uint32_t walker_id){
    if(!tabEntry) return WTE_NULL;


    return WTE_OK ;
}//doesnt check for presence ?
//dunno if it should atm
