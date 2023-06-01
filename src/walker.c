#include "walker.h"
#include "memory.h"

#include <stdint.h>
#include <stdio.h>


enum{ 
    WA_OK, WA_NULL, WA_ALLOC
}WalkerArrayErrFlags;

enum{ 
    WTE_OK, WTE_NULL, WTE_REALLOC, WTE_IN
}WalkerTableEntryErrFlags;

enum{
    WT_OK, WT_NULL, WT_REALLOC, WT_INDEX_TOO_BIG, WT_NOT_FOUND
}WalkerTableErrFlags; 

// this code is very wrong cuz I should pass a reference to a walker maybe i'm not sure actually tbh

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

    for(uint32_t i=0; i<size; i++){ //initialises the walkers 
        wArray->array[i].id=i; 
        wArray->array[i].curgen=0;
    }

    if(!wArray->array) return WA_ALLOC;

    return WA_OK;
}//ok


void freeWalkerArray(WalkerArray * wArray){
    /*
    */
    free(wArray->array);
}


/* TABLE ENTRY MANIPULATION : */

static uint8_t initWalkerEntry( WalkerTableEntry * tabEntry , uint32_t size ){
    /*
    initialises a tabEntry ; 
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
}//not tested; prolly ok

static void freeWalkerEntry(WalkerTableEntry * tabEntry){
    /*
    */
    if(!tabEntry) return;
    if(tabEntry->walkers) free(tabEntry->walkers);
  
}//not tested; prolly ok ; watch out for freeing walkers cuz they might become more intricate n so on

static uint8_t addWalkerEntry( WalkerTableEntry * tabEntry, Walker * walker_ref){
    /*
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


static int64_t getWalkerIndex ( WalkerTableEntry * tabEntry , uint32_t walker_id){
    /*
    simple existence test for walker in an entry; returns index of walker in table; 
    -1 on failure 
    */
    if(!tabEntry) return -1;

    for(uint32_t i=0; i <tabEntry->curr_in; i++){
        if(tabEntry->walkers[i]->id==walker_id) return i;
    }

    return -1;
}//not tested 


static uint32_t removeWalkerFromEntry( WalkerTableEntry * tabEntry , uint32_t walker_arr_index ){
    /*
    "removes" a walker from an entry array by putting the walker at the last index in it's place 
    and diminishing the number of walkers currently in the array
    */
    if(!tabEntry) return WTE_NULL;

    Walker * last_walker = tabEntry->walkers[tabEntry->curr_in--];

    tabEntry->walkers[walker_arr_index]=last_walker; //replaces walker at desired index

    return WTE_OK;
}//not tested 

//should move around references to walkers if they get any more complex


static void printWalkerEntry( WalkerTableEntry * tabEntry,  FILE* stream){
    /*
    */
    for(uint32_t i=0; i<tabEntry->curr_in ; i++){
        fprintf(stream, "%u\n",  tabEntry->walkers[i]->id);
    }
    fprintf(stream, "\n");
}


/* TABLE MANIPULATION : */

uint8_t initWalkerTable( WalkerTable * wtable, uint32_t tabsize, uint32_t entry_size){
    /*
    intitialises an allocated wtable
    */

    if(!wtable) return WT_NULL;

    wtable->size=tabsize;

    wtable->table=NULL;
    wtable->table=(WalkerTableEntry*) GROW_ARRAY(WalkerTableEntry, wtable->table, 0, tabsize);

    for(uint32_t i=0; i<tabsize ; i++){
        uint8_t failure= initWalkerEntry(&wtable->table[i], entry_size);
        if(failure) return failure;
    }

    return WT_OK;
}//not tested ; prolly ok

void freeWalkerTable( WalkerTable * wtable ){
    /*
    frees EVERY ARRAY IN THE TABLE but not the TABLE POINTER ITSELF 
    */
    if(!wtable) return;

    for(uint32_t i=0; i<wtable->size; i++){
        freeWalkerEntry(&wtable->table[i]);
    }

    free(wtable->table);
}//not tested 

uint8_t addEntry( WalkerTable* wtable, uint32_t index_entry, Walker * walker_ref ){
    /*
    adds a walker at index index_entry in the wtable
    */
    if(!wtable) return WT_NULL;
    if(index_entry> wtable->size) return WT_INDEX_TOO_BIG;

    uint8_t succes = addWalkerEntry(&wtable->table[index_entry], walker_ref);
    return succes ;
}//not tested

uint8_t removeEntry( WalkerTable * wtable, uint32_t index_entry, uint32_t walker_id){
    /*
    */
    if(!wtable) return WT_NULL;
    if(index_entry> wtable->size) return WT_INDEX_TOO_BIG;

    int64_t deletion_index=  getWalkerIndex(&wtable->table[index_entry], walker_id);
    if(deletion_index==-1) return WT_NOT_FOUND;

    uint8_t succes= removeWalkerFromEntry(&wtable->table[index_entry],deletion_index);
    return succes;
}//not tested 


void printWalkerTable(WalkerTable * wtable, FILE * stream){
    /*
    */
    for(unsigned i=0; i<wtable->size; i++){
        fprintf(stream, "%u: ", i);
        printWalkerEntry(&wtable->table[i], stream);

    }
    return;
}//not done