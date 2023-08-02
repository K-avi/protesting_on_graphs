#include "attra.h"
#include "misc.h"

uint8_t rule_rand( GraphTable * gtable , uint32_t node_from, uint32_t walker_index, SEARCH_UTILS * sutils){
    /*
    chooses a random neighboor node at entry node from in a gt;
    
    WARNING : in order to repport errors the node chosen is
    returned by reference in the index_node_to argument

    WARNING : will update the flux field of gtable ; position 
    of walker and wkcn->next_num;
    O(1)
    */
    if(!gtable) { report_err( "rule_rand", GT_NULL ) ; return GT_NULL;} 
  
    if(node_from>gtable->table_size) { report_err( "rule_rand", GT_SIZE ) ; return GT_SIZE;} 
    if(gtable->entries[node_from].neighboor_num==0) { report_err( "rule_rand", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;} 

    Line * line_to = gtable->entries[node_from].first_neighboor_ref+ rand()%gtable->entries[node_from].neighboor_num;
   
    gtable->arrLine->next_flux[ line_to- gtable->arrLine->array]++;
    gtable->wkcn->next_num[line_to->node_index]++;
    gtable->warray->array[walker_index].index_entry= line_to->node_index;

    return T_OK;
}// new version ;tested ; seems ok

uint8_t rule_teleport( GraphTable * gtable , uint32_t node_from, uint32_t walker_index, SEARCH_UTILS * sutils){
    /*
    chooses a random node in the graph and teleports to it 
    O(1)
    */
    if(!gtable) { report_err( "rule_teleport", GT_NULL ) ; return GT_NULL;} 
    if(node_from>gtable->table_size) { report_err( "rule_teleport", GT_SIZE ) ; return GT_SIZE;} 
    
    Line  line_to = gtable->arrLine->array[rand()%gtable->arrLine->size];
   
    
    gtable->wkcn->next_num[line_to.node_index]++;
    gtable->warray->array[walker_index].index_entry= line_to.node_index;

    return T_OK;
}// not tested, prolly ok tested ; seems ok

uint8_t rule_sleep(GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils){
    /*
    will node move the walker 
    does so by setting it's next node to it's current node and not updating any flux.
    
    WARNING: will update the number of elements at the node of index 
    node_from for the next iteration
    O(1)
    */
    if(!gtable) { report_err( "rule_speed_constant", GT_NULL ) ; return GT_NULL;} 
    if(node_from>gtable->table_size) { report_err( "rule_speed_constant", GT_SIZE ) ; return GT_SIZE;} 
    gtable->wkcn->next_num[node_from]++;
   
    return T_OK;
}//tested ; seems ok
//make a metarule

uint8_t rule_propulsion(GraphTable * gtable, uint32_t node_from, uint32_t walker_index, SEARCH_UTILS * sutils){
    /*the propulsion itself */

    if(!gtable) { report_err( "rule_propulsion", GT_NULL ) ; return GT_NULL;} 
  
    if(node_from>gtable->table_size) { report_err( "rule_propulsion", GT_SIZE ) ; return GT_SIZE;} 
    if(gtable->entries[node_from].neighboor_num==0) { report_err( "rule_propulsion", MV_NONEIGHBOORS ) ; return MV_NONEIGHBOORS;}

    GraphTableEntry * cur_entry = &gtable->entries[node_from];
    Line * line_to=NULL;

    if(cur_entry->neighboor_num==1 &&  //checks if u can't move ; sleep if it's the case 
       gtable->warray->array_prev[walker_index].index_entry==
       (cur_entry->first_neighboor_ref)->node_index){
   
         
        return rule_sleep(gtable, node_from, walker_index,  sutils);
    }

    uint32_t neighbor_num =gtable->entries[node_from].neighboor_num;
    Line * filtered_tab[neighbor_num-1];
    uint32_t cpt=0;

    //generates array of neighbor to pull from 
    for(uint32_t i=0; i<neighbor_num ; i++){
        line_to = (gtable->entries[node_from].first_neighboor_ref +i);
        if( line_to->node_index != gtable->warray->array_prev[walker_index].index_entry)
            filtered_tab[cpt++]= line_to;
    }
    if(cpt!=0){
        line_to = filtered_tab[rand()%cpt];
    }else{ //weird ass case ; happened to me bc a node had "two lines" to the same node which 
    //like some type of multi graph
        
        return rule_sleep(gtable, node_from, walker_index, sutils);
    }
    //update pos / flux
    gtable->arrLine->next_flux[ line_to- gtable->arrLine->array]++;
    gtable->wkcn->next_num[line_to->node_index]++;
   //printf("%u \n",walker_index);
    gtable->warray->array[walker_index].index_entry= line_to->node_index;

    
    return T_OK;
}// tested ;ok ; hate it