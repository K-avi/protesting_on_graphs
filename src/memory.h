#ifndef memory_h 
#define memory_h 

#include "common.h"


#define DEFAULT_CAPA_TACTICS 8 //maybe less ?
#define DEFAULT_CAPA_LINE 256
#define DEFAULT_CAPA_NODE 256 //might be set to weird numbers during tests 

#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, oldCount, newCount) \
    (type*)reallocate(pointer, sizeof(type) * (oldCount), sizeof(type) * (newCount))

#define FREE_ARRAY(type, pointer, oldCount) reallocate(pointer, sizeof(type) * (oldCount), 0)

void* reallocate(void* pointer, size_t oldSize, size_t newSize);


#endif 