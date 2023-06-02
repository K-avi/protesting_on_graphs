#include "walker.h"
#include "memory.h"

#include <stdint.h>
#include <stdio.h>

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
        wArray->array[i].curgen=0;
    }

    return WA_OK;
}//ok


void freeWalkerArray(WalkerArray * wArray){
    /*yes*/
    free(wArray->array);
}//ok


/* TABLE ENTRY MANIPULATION : */

uint8_t initWalkerEntry( WalkerTableEntry * tabEntry , uint32_t size ){
    /*
    initialises a non null , empty tabEntry ; 
    will cause memleak if used on already allocated tabEntry 
    */
    if(!tabEntry) return WTE_NULL;

    tabEntry->capa= size;
    tabEntry->curr_in=0;
    
    tabEntry->walkers=NULL;
    tabEntry->walkers= (Walker**) GROW_ARRAY(Walker*, tabEntry->walkers, 0, size);

    if(!tabEntry->walkers){
        return WTE_REALLOC;
    }
    return WTE_OK;
}// tested;  ok

void freeWalkerEntry(WalkerTableEntry * tabEntry){
    /*frees the content of a walker entry; doesn't free tabEntry */
    if(!tabEntry) return;
    if(tabEntry->walkers) free(tabEntry->walkers);

}// tested; ok 

uint8_t addWalkerEntry( WalkerTableEntry * tabEntry, Walker * walker_ref){
    /*
    addss the reference to a walker in an WalkerTableEntry 

    O(1)
    */
    if(!tabEntry) return WTE_NULL;

    if(tabEntry->capa==tabEntry->curr_in-1){
        uint32_t oldCapa= tabEntry->capa;
        tabEntry->capa=GROW_CAPACITY(tabEntry->capa);
        tabEntry->walkers=(Walker**) GROW_ARRAY(Walker*, tabEntry->walkers,oldCapa , tabEntry->capa);

        if(!tabEntry->walkers) return WTE_REALLOC;      
    }

    tabEntry->walkers[tabEntry->curr_in]=walker_ref;
    tabEntry->curr_in++;

    return WTE_OK ;
}//doesnt check for presence ?
//dunno if it should atm
//not tested 


int64_t getWalkerIndex ( WalkerTableEntry * tabEntry , uint32_t walker_id){
    /*
    simple existence test for walker in an entry; returns index of walker in table; 
    -1 on failure 

    O(a) a is the number of walkers in a WalkerEntry (small)
    */
    if(!tabEntry) return -1;

    for(uint32_t i=0; i <tabEntry->curr_in; i++){
        if(tabEntry->walkers[i]->id==walker_id) return i;
    }

    return -1;
}//not tested 


uint8_t removeWalkerFromEntry( WalkerTableEntry * tabEntry , uint32_t walker_arr_index ){
    /*
    "removes" a walker from an entry array by putting the walker at the last index in it's place 
    and diminishing the number of walkers currently in the array

    O(1)
    */
    if(!tabEntry) return WTE_NULL;

    Walker * last_walker = tabEntry->walkers[tabEntry->curr_in--];

    tabEntry->walkers[walker_arr_index]=last_walker; //replaces walker at desired index

    return WTE_OK;
}//not tested 
//should move around references to walkers if they get any more complex

void printWalkerEntry( WalkerTableEntry * tabEntry,  FILE* stream){
    /* */
    for(uint32_t i=0; i<tabEntry->curr_in ; i++){
        fprintf(stream, "%u\n",  tabEntry->walkers[i]->id);
    }
    fprintf(stream, "\n");
}//not tested 