#ifndef ALIGN_H
#define ALIGN_H

#include "common.h"
#include "graph_table.h"
#include "search.h"


//deterministic alignement rules 

uint8_t rule_alignement(GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils); //basic align rule 
uint8_t rule_align_follow(GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils); //align where u ignore the incomign flux 
uint8_t rule_align_vision(GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils); //align with vision


//probabilistic align rules

uint8_t rule_align_proba(GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils); //the terrible basic proba 
//the one w renormalization 

uint8_t rule_align_proba_threshold(GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils); //the proba align 
//where negative is set to 0 

uint8_t rule_align_proba_follow(GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils); //the proba align where 
//the incoming flux is ignored

uint8_t rule_align_proba_exclusion(GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils);//the proba align where 
//nodes with incoming flux >0 are excluded from choice

#endif