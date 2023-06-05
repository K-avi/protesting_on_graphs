#include "walker.h"
#include "common.h"
#include "memory.h"

#include <bits/types/FILE.h>
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

void printWarray(WalkerArray *wArray, FILE * stream){
    /*
    */
    if(!wArray) {fprintf(stream,"warray is null\n"); return;}

    for(uint32_t i=0 ; i<wArray->size; i++){
        fprintf(stream,"%u:%u\n", wArray->array[i].id , wArray->array[i].curgen);
    }
}


/* TABLE ENTRY MANIPULATION : */

static uint8_t initWalkerRefStack ( WalkerRefStack * wrs , uint32_t capa){
    /*
    initialises a non null, empty wrs
    */
    if(!wrs) return WRS_NULL;

    wrs->walker_stack=NULL;
    wrs->walker_stack=(Walker**) GROW_ARRAY(Walker*, wrs->walker_stack, 0, capa);
    if(!wrs->walker_stack) return WRS_MALLOC;

    wrs->stack_in=0;
    wrs->stack_capa=capa;

    return WRS_OK;
}//tested;  ok

uint8_t initWalkerEntry( WalkerTableEntry * tabEntry , uint32_t capa ){
    /*
    initialises a non null , empty tabEntry ; 
    will cause memleak if used on already allocated tabEntry 
    */
    if(!tabEntry) return WTE_NULL;
    tabEntry->nb_walker_cur=0;
    uint8_t failure=initWalkerRefStack(&tabEntry->cur_stack, capa);
    if(failure) return failure;

    failure = initWalkerRefStack(&tabEntry->next_stack, capa);
    return failure;

    
   
}//  tested; prolly ok  ok

void freeWalkerEntry(WalkerTableEntry * tabEntry){
    /*frees the content of a walker entry; doesn't free tabEntry */
    if(!tabEntry) return;
    if(tabEntry->next_stack.walker_stack) free(tabEntry->next_stack.walker_stack);
    if(tabEntry->cur_stack.walker_stack) free(tabEntry->cur_stack.walker_stack);
    
}//  tested;  ok 



static uint8_t push_wkref_stack(WalkerRefStack * wrs, Walker * wkref ){
    /*
    stack push function on a wrs; checks for realloc
    */
    if(!wrs) return WRS_NULL;
    if(wrs->stack_in==wrs->stack_capa-1){
        uint32_t oldCapa= wrs->stack_capa;
        wrs->stack_capa= GROW_CAPACITY(wrs->stack_capa);
        wrs->walker_stack= (Walker**)GROW_ARRAY(Walker*, wrs->walker_stack, oldCapa, wrs->stack_capa);
        if(!wrs->walker_stack) return WRS_REALLOC;
    }
    wrs->walker_stack[wrs->stack_in]= wkref;
    wrs->stack_in++;

    return WRS_OK;
}// tested; ok

static uint8_t pop_wkref_stack( WalkerRefStack *wrs, Walker ** wkref_ret){
    /*
    stack pop function on a wrs returns value by reference to check for failure
    */
    if(!wrs) return WRS_NULL;
 
    if(wrs->stack_in==0){
        *wkref_ret=NULL;
        return WRS_EMPTYSTACK;
    }
    wrs->stack_in--;
    *wkref_ret= wrs->walker_stack[wrs->stack_in];

    return WRS_OK;
}//tested  ok

uint8_t push_wte_nextstack( WalkerTableEntry * tabEntry, Walker * walker_ref){
    /*
    addss the reference to a walker in the next_stack of  WalkerTableEntry 
    O(1)
    */
    if(!tabEntry) return WTE_NULL;
    return push_wkref_stack(&tabEntry->next_stack, walker_ref); ;
}//doesnt check for presence ?
//dunno if it should atm
// tested ok


int64_t getWalkerCurStack ( WalkerTableEntry * tabEntry , uint32_t walker_id){
    /*
    checks for the existence of a walker of id passed inside of the CUR_STACK of 
    a tabEntry 
    O(a) a is the number of walkers in a WalkerEntry (small)
    */
    if(!tabEntry) return -1;

    for(uint32_t i=0; i <tabEntry->cur_stack.stack_in; i++){
        if(tabEntry->cur_stack.walker_stack[i]->id==walker_id) return i;
    }

    return -1;
}//not tested maybe useless


uint8_t pop_wte_curstack( WalkerTableEntry * tabEntry , Walker ** wkref_ret){
    /*
    pops a walker from curstack and returns it in the wkref_ret arg
    O(1)
    */
    if(!tabEntry) return WTE_NULL;
    uint8_t failure= pop_wkref_stack(&tabEntry->cur_stack, wkref_ret);

    return failure;
}//tested ok


static void printWrs(WalkerRefStack * wrs , FILE * stream){
    /*prints the content of a wrs to stream*/
    if(!wrs){ fprintf(stream, "null stack in wrs\n"); return;}

    for(uint32_t i=0; i<wrs->stack_in; i++){
        fprintf(stream, "%u,", wrs->walker_stack[i]->id);
    }
}// tested ok

void printWalkerEntry( WalkerTableEntry * tabEntry,  FILE* stream){
    /*pops the two stacks of a tabentry */
    if(!tabEntry) {printf("null tab in pwe\n"); return;}


    fprintf(stream,"\ncurrent stack:");
    printWrs(&tabEntry->cur_stack, stream);
    fprintf(stream,"\nnext stack:");
    printWrs(&tabEntry->next_stack, stream);
    fprintf(stream, "\n");
}// tested ok