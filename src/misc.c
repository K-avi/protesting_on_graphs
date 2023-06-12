#include "misc.h"
#include "graph_table.h"
#include "walker.h"
#include <stdint.h>
#include <stdio.h>
/*
this file contains the realloc functions for every dynamic array in the project 
to avoid boilerplates; dynamic arrays reallocations will be handled here
actually now most of the project is static so it's kinda useless ig ? 
I might keep it cuz I think it's a nice wrapper around malloc
it also contains the error reporting and graph dump function 
*/

void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
    /*
    */
  if (newSize == 0) {
    if(pointer) free(pointer);
    return NULL;
  }

  void* result = realloc(pointer, newSize);
  if(!result)report_err("reallocate", ERRFLAG_REALLOC);
  
  return result;
}//now that every structure is static the function is kinda useless but I'll keep 
//it cuz it's a nice wrapper around malloc 
    
char * str_flag(uint8_t flag){

  switch (flag) {

  case FG_OK : return "how did you get here this flag is supposed to mean everything is okay";
  case GT_NULL :return "graphtable is null";
  case GT_MALLOC: return "couldn't allocate memory to graphtable"; 
  case GT_REALLOC : return "realloc of graphtable failed" ;
  case GT_READ : return "couldn't open graphtable file";
  case GT_WRITE : return "couldn't write graphtable";
  case GT_INDEX : return "warning unused flag";
  case GT_NOREAD: return "couldn't retrieve lines from file"; 
  case GT_READFAIL: return "couldn't read graphtable from file";
  case GT_PARSE: return "parsing of file failed check that the file is formatted correctly"; 
  case GT_NOWRITE: return "couldn't write to fail for graphtable";
  case GT_OPENFAIL: return "couldn't open graphtable file"; 
  case ERRFLAG_NOFILE: return "file doesn't exist";  
  case GE_NULL: return "graph entry is null";
  case GE_MALLOC: return "coulnd't allocate memory to graphentry";  
  case GE_REALLOC:return "realloc of graphentry failed"; 
  case GT_ARFULL: return "coulnd't add a node to the graphtable"; 
  case T_NULL: return "tactics ptr is null"; 
  case T_REALLOC: return "realloc of tactics failed";
  case WA_NULL: return "walker array is null"; 
  case WA_ALLOC: return "couldn't allocate memory to walker entry "; 
  case WTE_NULL: return "walker table entry is null"; 
  case WTE_REALLOC: return "coulnd't reallocate memory to walker table entry";
  case WTE_IN: return "deprecated flag";
  case WT_NULL: return "walker table is null";
  case WT_REALLOC: return "realloc of walker table failed";
  case WT_INDEX_TOO_BIG: return "deprecated flag";
  case WT_NOT_FOUND: return "deprecated flag";
  case GT_SIZE: return "index of graph table given is too big";
  case GTE_NULL: return "deprecated flag";
  case NDREF_NULL:return "deprecated flag";
  case MV_NONEIGHBOORS: return "node has no neighboors to move to";
  case WKR_NULL: return "walker reference is null"; 
  case LINEREF_NULL: return "line reference is null";
  case WRS_NULL: return "deprecated flag";
  case WRS_MALLOC:return "deprecated flag";
  case WRS_REALLOC:return "deprecated flag";
  case WRS_EMPTYSTACK :return "deprecated flag";
  case WKCN_NULL : return "walker cur next array is null";
  case WKCN_MALLOC : return "couldn't allocate memory to walker cur next array";
  case WKCN_REALLOC : return "realloc of walker cur next array failed";
  case MV_NULL : return "deprecated flag";
  case L_ARRNULL :return "line array is null";
  case T_CANTCHOOSE :return "couldn't choose a rule based on value given";
  case ERRFLAG_REALLOC : return "failed to allocate memory";
  case PRS_NULL : return "string passed is null";
  case PRS_INVALID_FORMAT: return "format of rule string not recognised";
  case PRS_COEFF : return "coeff passed is too big";
  case AR_MALLOC : return "failed to allocate memory to the memory pool\n";
  case AR_NULL : return "the memory pool is null\n";
  case AR_FULL : return "jesus christ you filled the memory pool???";
  case ERRGLAG_CANTWRITE: return "couldn't open the files to dump the trace\n";
  default : return "unknown error ; how did you get here?\n";
  }
}//ok

void report_err( char * msg , uint8_t flag){ //could be a macro 
  fprintf(stderr, "error : %s at %s\n", str_flag(flag), msg);
}


void dump_trace(GraphTable * gt, FILE * stream_curnum , FILE * stream_flux, FILE * stream_walker ){
  //the trace function to dump graph after it

   //dumps the bin array of the walker's position in the file for curnum trace
   fwrite(gt->wkcn->cur_num, sizeof(uint32_t), gt->wkcn->size, stream_curnum);
   //dumps the binary array of the flux at each line in the file for flux trace
   fwrite(gt->arrLine->cur_flux, sizeof(uint32_t), gt->arrLine->size, stream_flux);  
   //dumps the bin array of walkers
   fwrite(gt->warray->array, sizeof(Walker), gt->warray->size , stream_walker);
}//tested; ok 
//might be an issue if the walkers have more fields added 
//if it happens separate their immutable states from their mutable ones 