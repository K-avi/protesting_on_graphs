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