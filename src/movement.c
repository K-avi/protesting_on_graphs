#include "movement.h"
#include "common.h"
#include "graph_table.h"
#include "memory.h"
#include "tactics.h"
#include "walker.h"
#include <stdint.h>
#include <stdio.h>

uint8_t initPos(GraphTable * gtable, WalkerArray *walkerArr){
    /*
    randomly inits the position of the walkers contained 
    in warray passed as argument in the 
    table wtab
    */
    if(!gtable) return GT_NULL;
    if(!walkerArr) return WA_NULL;

    for(uint32_t i=0; i<walkerArr->size;i++){
       uint8_t failure= pushEntryGT( gtable , rand()%gtable->table_size, &walkerArr->array[i]);
       if(failure) return failure;
    }

    return MV_OK;
}// new version , test at some point  ; ok

uint8_t moveEntry(GraphTable * gtable,uint32_t node_from , uint32_t node_to){
    /*
    pops a node in the gtable[ node_from] entry to the gtable[ node_to] entr
    doesn't report wkref== null 
    O(1)
    */  
    if(!gtable) return GT_NULL;

    Walker * popped_ref;
    uint8_t failure= popEntryGT(gtable, node_from, &popped_ref);

    if (failure) return failure;

    failure= pushEntryGT(gtable, node_to, popped_ref);
    if(failure) return failure;

    return MV_OK;
}//new version ; not tested 
//doesnt update flux 

uint8_t mvEntryVar( GraphTable * gtable, uint32_t node_from , uint32_t neighboor_num_to){
    /*
    O(1)
    */
    if(!gtable) return GT_NULL;

    Walker * popped_ref;
    uint8_t failure= popEntryGT(gtable, node_from, &popped_ref);

    if (failure) return failure;
    //(gtable->entries[node_from].first_neighboor_ref+neighboor_num_to)->flux_next++;
    return MV_OK;

}//doesnt upadte flux 

static uint8_t swapStacks(WalkerTableEntry * wtentry){
    /*
    swaps the 2 stacks at an entry of the table
    */
    if(!wtentry) return WTE_NULL;
    if(!(wtentry->cur_stack.walker_stack && wtentry->next_stack.walker_stack)) return WRS_NULL;

    wtentry->nb_walker_cur=wtentry->next_stack.stack_in; //sets new value of number of
    //elem at beginning of iteration

/*
    while(wtentry->cur_stack.stack_capa< wtentry->next_stack.stack_capa){
        
       uint32_t oldCapa= wtentry->cur_stack.stack_capa;
       wtentry->cur_stack.stack_capa= GROW_CAPACITY(wtentry->cur_stack.stack_capa);
       wtentry->cur_stack.walker_stack= (Walker**) GROW_ARRAY(Walker*, wtentry->cur_stack.walker_stack, 
       oldCapa, wtentry->cur_stack.stack_capa);

       if(!wtentry->cur_stack.walker_stack) return WTE_REALLOC;
    }
    */
    WalkerRefStack wkres= wtentry->cur_stack;

    wtentry->cur_stack.walker_stack=wtentry->next_stack.walker_stack; //check for side effects cuz I don't trust this tbh
    wtentry->cur_stack.stack_capa=wtentry->next_stack.stack_capa;
    wtentry->cur_stack.stack_in= wtentry->next_stack.stack_in;

    wtentry->next_stack.stack_capa= wkres.stack_capa;
    wtentry->next_stack.stack_in=0;
    wtentry->next_stack.walker_stack=wkres.walker_stack;

    return WTE_OK;
}

uint8_t prepareIteration( GraphTable * gtable, WalkerArray * warray){
    /*
    gets the table ready for the next iteration ; my goal is to not use this function 
    and find a way to do it on the fly in iterate gen 
    O(a+b+c) , a number of nodes ,b number of lines, c number of walkers  
    */
    if(!gtable) return GT_NULL;
    gtable->curgen++;
    for(uint32_t i=0; i<gtable->table_size;i++){ 
     //   printf("swappyti swap %u %p %p\n", i,gtable->entries->walker_entry.cur_stack.walker_stack ,  gtable->entries->walker_entry.next_stack.walker_stack ); 
        swapStacks(&gtable->entries[i].walker_entry);
        //printf("swap swappity %p %p\n", gtable->entries->walker_entry.cur_stack.walker_stack ,  gtable->entries->walker_entry.next_stack.walker_stack );
    }

    for(uint32_t i=0 ; i<gtable->arrLine->cur_in;i++){
        gtable->arrLine->array[i].flux_cur=gtable->arrLine->array[i].flux_next;
      //  printf("flux cur %d flux next %d i %u\n ",gtable->arrLine->array[i].flux_cur, gtable->arrLine->array[i].flux_next, i );
    }

    for(uint32_t i=0; i<warray->size; i++){
        warray->array[i].curgen++;
    }

    return GT_OK;        
}//do not use!!! test fn to update easily 
//terrible performances ; find smtg smarter ffs


uint8_t iterateGen(GraphTable * gtable, Tactics* tactics){
    /*
    the main movement function ; takes a fully intialised gtable and iterates
    1 generation on it the function is in ~O(n*t) where n is the number of 
    walkers and t the time needed to calculate the tactic (select a node) for 1 walker 
    O(n) n is the number of walkers
    */
    if(!gtable) return GT_NULL;
    
    for(uint32_t i=0; i<gtable->table_size;i++){
      // printf("here i goes %u\nima loop %u\n", i,gtable->entries[i].walker_entry.cur_stack.stack_in); 
        for(uint32_t j=0; j<gtable->entries[i].walker_entry.nb_walker_cur; j++){
            
          //  printf("yippe n°%u\n", j);
            
            Walker * wkref;
          
            uint8_t failure = popEntryGT(gtable, i, &wkref);
            if(failure){printf("oh hell no, %u %u %u %u \n", failure,i, j, gtable->entries[i].walker_entry.cur_stack.stack_in); return failure;}

            //wkref->curgen++; //updates gen of walker 
            
            Line line_chosen;
            failure= chooseNodeVar(tactics, gtable, i, &line_chosen);
            if(failure) {printf("oh hellhell no %u\n", failure); return failure;}

            //printf("line chosen :%u\n", line_chosen.node_index);
          
           // line_chosen.flux_next++;
         //   printf("linechosen flux next %u %d\n", line_chosen.node_index, line_chosen.flux_next);
           // printf("oh yeah it's pushin time\n");
            pushEntryGte(line_chosen.tabRef, wkref);
        }
       
    }
   // gtable->curgen++;
    return MV_OK;
}//tested seems ok;  this version will have to run w prepareIteration() 
/*
the problem is that combining this w prepareIt will have a total runtime of 
O(2n+b+c) where n is nb of walkers, b nb of lines, c nb of nodes 
*/


uint8_t prepareIterationVAR1( GraphTable * gtable){
    /*
    gets the table ready for the next iteration ; my goal is to not use this function 
    and find a way to do it on the fly in iterate gen 
    O(a+b) , a number of nodes ,b number of lines 
    */
    if(!gtable) return GT_NULL;
   
    for(uint32_t i=0; i<gtable->table_size;i++){ 
     //   printf("swappyti swap %u %p %p\n", i,gtable->entries->walker_entry.cur_stack.walker_stack ,  gtable->entries->walker_entry.next_stack.walker_stack ); 
        swapStacks(&gtable->entries[i].walker_entry);
        //printf("swap swappity %p %p\n", gtable->entries->walker_entry.cur_stack.walker_stack ,  gtable->entries->walker_entry.next_stack.walker_stack );
    }

    for(uint32_t i=0 ; i<gtable->arrLine->cur_in;i++){
        gtable->arrLine->array[i].flux_cur=gtable->arrLine->array[i].flux_next;
        gtable->arrLine->array[i].flux_next=0;
    }
    return GT_OK;        
}

uint8_t iterateGenVAR1(GraphTable * gtable, Tactics* tactics){
    /*
    the main movement function ; takes a fully intialised gtable and iterates
    1 generation on it the function is in ~O(n*t) where n is the number of 
    walkers and t the time needed to calculate the tactic (select a node) for 1 walker 
    O(w) w is the number of walkers
    */
    if(!gtable) return GT_NULL;
    
    for(uint32_t i=0; i<gtable->table_size;i++){
      // printf("here i goes %u\nima loop %u\n", i,gtable->entries[i].walker_entry.cur_stack.stack_in); 
        for(uint32_t j=0; j<gtable->entries[i].walker_entry.nb_walker_cur; j++){
            
          //  printf("yippe n°%u\n", j);
            
            Walker * wkref;
           
            uint8_t failure = popEntryGT(gtable, i, &wkref);
            if(failure){printf("oh hell no, %u %u %u %u \n", failure,i, j, gtable->entries[i].walker_entry.cur_stack.stack_in); return failure;}

            wkref->curgen++; //updates gen of walker 
            
            Line line_chosen;
            failure= chooseNodeVar(tactics, gtable, i, &line_chosen);
            if(failure) {printf("oh hellhell no %u\n", failure); return failure;}

            printf("line chosen :%u %d\n", line_chosen.node_index, line_chosen.flux_cur);

            
           // printf("oh yeah it's pushin time\n");
            pushEntryGte(line_chosen.tabRef, wkref);
        }
       
    }
    gtable->curgen++;
    return MV_OK;
}//tested seems ok;  this version will have to run w prepareIteration() 
/*
I swear to god I'll make this fool faster it ain't gonna beat me like a punk 
total comp of iterate + prepare : 

O(w+n+l) w num of walkers, n number of nodes , l number of lines
*/


uint8_t iterate_ntimes( GraphTable * gtable, Tactics * tactics, WalkerArray * warray, uint32_t iter_num){
    /**/

    for(unsigned i=0; i<iter_num; i++){
        uint8_t failure = prepareIteration(gtable,warray );
        if(failure) return failure;

        failure= iterateGen(gtable, tactics);
        if(failure) return failure;
    }
    return IT_OK;
}

uint8_t iterate_ntimesVAR1( GraphTable * gtable, Tactics * tactics, uint32_t iter_num){
    /**/

    for(unsigned i=0; i<iter_num; i++){
        uint8_t failure = prepareIterationVAR1(gtable );
        if(failure) return failure;

        failure= iterateGenVAR1(gtable, tactics);
        if(failure) return failure;
    }
    return IT_OK;
}