#include "movement.h"
#include "common.h"
#include "graph_table.h"
#include "walker.h"
#include <stdint.h>


uint8_t initPos(GraphTable * gtable, WalkerArray *walkerArr){
    /*
    randomly inits the position of the walkers contained 
    in warray passed as argument in the 
    table wtab
    */
    if(!gtable) return GT_NULL;
    if(!walkerArr) return WA_NULL;

    for(uint32_t i=0; i<walkerArr->size;i++){
       uint8_t failure= addEntryGT( gtable , rand()%gtable->table_size, &walkerArr->array[i]);
       if(failure) return failure;
    }

    return MV_OK;
}// tested  ; ok

uint8_t moveEntry(GraphTable * gtable, Walker* wkref,uint32_t node_from , uint32_t node_to){
    /*
    moves the references to a walker from a node to another one 

    to do so , deletes the walker ref from the walker table stored at index node_from of the table 
    and adds it to the node_to walker table

    doesn't report wkref== null
    
    */  
    if(!gtable) return GT_NULL;

    uint8_t failure= removeEntryGT(gtable, node_from, wkref->id);
    if (failure) return failure;

    failure= addEntryGT(gtable, node_to, wkref);
    if(failure) return failure;

    return MV_OK;
}//tested; ok should make variant of fn where I pass node_to by ref of the entry

uint8_t chooseRandDest( GraphTable * gtable , uint32_t node_from, uint32_t * index_node_to){
    /*
    chooses a random neighboor node at entry node from in a gt;
    
    WARNING : in order to repport errors the node chosen is
    returned by reference in the index_node_to argument
    O(1)
    */
    if(!gtable) return GT_NULL;
    if(!index_node_to) return NDREF_NULL;
    if(node_from>gtable->table_size) return GT_SIZE;

    if(gtable->entries[node_from].neighboor_num==0) return MV_NONEIGHBOORS;
    *index_node_to=(gtable->entries[node_from].first_neighboor_ref+(rand()%gtable->entries[node_from].neighboor_num))->node_index;

    return MV_OK;
}//not tested 
//should make equivalent of the func where I just return the ref 
//this is clearly a rule fn why the hell is it there 

uint8_t iterateGen(GraphTable * gtable, Tactics* tactics){
    /*
    the main movement function ; takes a fully intialised gtable and iterates
    1 generation on it the function is in ~O(n*t) where n is the number of 
    walkers and t the time needed to calculate the tactic for 1 walker 

    (removing an entry actually is in O(a) where a is the number of 
    elements in an entry but if the graph table is scaled correctly it should 
    be close to constant however a more "correct" complexity would be smtg like 
    O( (a0+...+aN)*n*t) where a0...aN is the number of walkers at each entry
    */
    if(!gtable) return GT_NULL;

    return MV_OK;
}//not done