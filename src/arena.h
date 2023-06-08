#ifndef ARENA_H 
#define ARENA_H 
#include "common.h"


//the arena will be used to store all of the 
//memory at one place
typedef struct s_arena{
    uint64_t size; //size of the arena
    uint8_t top_index; //top of the memory pool
    uint8_t * memory;
}Arena;

//oh boy 

uint64_t calculate_size( uint32_t nb_nodes , uint32_t nb_lines , 
                        uint32_t nb_rules , uint32_t nb_walkers);
uint8_t init_arena(Arena * arena, uint64_t size);
void* get_memory(Arena * arena, uint8_t * failure, size_t alloc_size);//christ

void free_arena(Arena * arena);

#endif