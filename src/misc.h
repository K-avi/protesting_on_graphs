#ifndef misc_h 
#define misc_h 

#include "common.h"
#include "graph_protesters/protester.h"
#include "graph_protesters/graph_table.h"

//arrays are now static so these constants r useless 
#define DEFAULT_CAPA_TACTICS 8 //maybe less ?
#define DEFAULT_CAPA_protestTABLE 100 //completely random
#define DEFAULT_CAPA_WTE 16 //protest table entry 
#define DEFAULT_CAPA_protestARRAY 30 
#define DEFAULT_CAPA_LINE 256
#define DEFAULT_CAPA_NODE 256 //might be set to weird numbers during tests 
//deprecated

#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, oldCount, newCount) \
    (type*)reallocate(pointer, sizeof(type) * (oldCount), sizeof(type) * (newCount))

#define FREE_ARRAY(type, pointer, oldCount) reallocate(pointer, sizeof(type) * (oldCount), 0)

void* reallocate(void* pointer, size_t oldSize, size_t newSize);
void report_err( char * msg , uint8_t flag);
void report_warning( char * msg , uint8_t flag);

void dump_trace(GraphTable * gt, FILE * stream_curnum , FILE * stream_flux, FILE * stream_protester );
uint8_t write_lines( GraphTable * gt ,FILE * stream );

#endif 