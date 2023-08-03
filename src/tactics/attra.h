#ifndef ATTRA_H
#define ATTRA_H

#include "../common.h"
#include "../graph_walkers/graph_table.h"
#include "../graph_walkers/search.h"

// deterministic attraction rules
uint8_t rule_attraction( GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils); //dt attraction 
uint8_t rule_attra_vision( GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils);// vision attraction

// probabilistic attraction
uint8_t rule_attraction_proba( GraphTable * gtable , uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils);

//silly attraction 
uint8_t rule_agoraphobic( GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils);
uint8_t rule_repulsion( GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils);

#endif