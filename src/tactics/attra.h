// walking_on_graphs: A simulation of .... walkers on graphs
// Copyright (C) 2023 Ivan Mulot-Radojcic 
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 or later.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details: https://www.gnu.org/licenses/

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