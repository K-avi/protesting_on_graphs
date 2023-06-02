#include "movement.h"
#include "common.h"

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
    if(!wkref) return WKR_NULL;

    uint8_t failure= removeEntryGT(gtable, node_from, wkref->id);
    if (failure) return failure;

    failure= addEntryGT(gtable, node_to, wkref);
    if(failure) return failure;

    return MV_OK;
}//tested; ok should make variant of fn where I pass node_to by ref of the entry


uint8_t moveEntryVar(GraphTable * gtable, Walker* wkref,uint32_t node_from , uint32_t node_to){
    /*
    */
    if(!gtable) return GT_NULL;
    if(!wkref) return WKR_NULL;

    uint8_t failure= addEntryGT(gtable, node_to, wkref);
    if(failure) return failure;

    return MV_OK;
}//do NOT use that

//crap 
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
    
    for(uint32_t i=0; i<gtable->table_size;i++){
        
        for(uint32_t j=0; j<gtable->entries[i].walker_entry.curr_in; j++){
            
            //moves check that the generation matches 
            if(gtable->entries->walker_entry.walkers[j]->curgen==gtable->curgen){

                gtable->entries->walker_entry.walkers[j]->curgen++;

                //wrong bc the walkers are moved around in the array when u move em im stupid ffs 
               
                Line line_to;
                uint8_t failure = chooseNodeVar(tactics, gtable, i, &line_to);
                if(failure) return failure;
            //this still doesn't work though       
                //updating flux should happen when choosing a node or I should return the line w the node 
                //to 
                failure =addWalkerEntry(&line_to.tabRef->walker_entry, gtable->entries[i].walker_entry.walkers[j]);
                if(failure) return failure;
            }
        }
    }
    gtable->curgen++;
    return MV_OK;
}//doesnt update flux atm 
//far from finished 
//actually awful
//damnit
/*
needs to : 

-update flux 
-update gen of walker 
-update gen of nodes or smtg
*/

