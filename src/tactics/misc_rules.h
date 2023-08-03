#ifndef MISC_RULES_H
#define MISC_RULES_H

#include "../common.h"
#include "../graph_walkers/graph_table.h"
#include "../graph_walkers/search.h"

uint8_t rule_rand( GraphTable * gtable , uint32_t node_from, uint32_t walker_index, SEARCH_UTILS * sutils);
uint8_t rule_teleport( GraphTable * gtable , uint32_t node_from, uint32_t walker_index, SEARCH_UTILS * sutils);

uint8_t rule_sleep(GraphTable * gtable, uint32_t node_from , uint32_t walker_index, SEARCH_UTILS * sutils);
uint8_t rule_propulsion(GraphTable * gtable, uint32_t node_from, uint32_t walker_index, SEARCH_UTILS * sutils);

#endif