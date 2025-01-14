#include "misc.h"
#include "common.h"
#include "graph_protesters/graph_table.h"

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
  case WA_NULL: return "protester array is null"; 
  case WA_ALLOC: return "couldn't allocate memory to protester entry "; 
  case WTE_NULL: return "protester table entry is null"; 
  case WTE_REALLOC: return "coulnd't reallocate memory to protester table entry";
  case WTE_IN: return "deprecated flag";
  case WT_NULL: return "protester table is null";
  case WT_REALLOC: return "realloc of protester table failed";
  case WT_INDEX_TOO_BIG: return "deprecated flag";
  case WT_NOT_FOUND: return "deprecated flag";
  case GT_SIZE: return "index of graph table given is too big";
  case GTE_NULL: return "deprecated flag";
  case NDREF_NULL:return "deprecated flag";
  case MV_NONEIGHBOORS: return "node has no neighboors to move to";
  case WKR_NULL: return "protester reference is null"; 
  case LINEREF_NULL: return "line reference is null";
  case WRS_NULL: return "deprecated flag";
  case WRS_MALLOC:return "deprecated flag";
  case WRS_REALLOC:return "deprecated flag";
  case WRS_EMPTYSTACK :return "deprecated flag";
  case WKCN_NULL : return "protester cur next array is null";
  case WKCN_MALLOC : return "couldn't allocate memory to protester cur next array";
  case WKCN_REALLOC : return "realloc of protester cur next array failed";
  case MV_NULL : return "deprecated flag";
  case L_ARRNULL :return "line array is null";
  case T_CANTCHOOSE :return "couldn't choose a rule based on value given";
  case ERRFLAG_REALLOC : return "failed to allocate memory";
  case PRS_NULL : return "string passed is null";
  case PRS_INVALID_FORMAT: return "format of rule string not recognised";
  case PRS_COEFF : return "coeff passed is too big";
  case AR_MALLOC : return "failed to allocate memory to the memory pool\n";
  case AR_NULL : return "the memory pool is null\n";
  case AR_FULL : return "jesus christ you filled the memory pool???\n";
  case ERRGLAG_CANTWRITE: return "couldn't open the files to dump the trace\n";
  case WA_NOREAD : return "path given is null\n";
  case WA_OPEN : return "can't open file at path given\n";
  case WA_READFAIL : return "couldn't properly read file at path given\n";
  case ERRFLAG_WARNING : return "default warning flag\n";
  case PRS_NOSUM : return "sum of coefficients of arguments is 0; first argument set to 1\n";
  case MV_WARNING : return "couldn't choose a node; defaulting to rand\n";
  case GA_NULL : return "group array passed is NULL\n";
  case GA_MALLOC : return "failed to allocate memory to group array's element field\n"; 
  case GA_REALLOC : return "failed to reallocate group array's elements\n";
  case ERRFLAG_NULL : return "null pointer caught\n";
  case GA_NOWK : return "no protesters on the graph passsed\n";
  case STACK_NULL : return "queue passed is NULL\n";
  case STACK_MALLOC : return "couldn't allocate memory for queue\n";
  case STACK_REALLOC : return "realloc of queue failed\n";
  case WA_ZERDEPTH : return "the depth of the search is zero\n";
  case SU_NULL : return "search utilitaries pointer is NULL\n";
  case SU_MALLOC : return "couldn't allocate memory for search util array\n";
  default : return "unknown error ; how did you get here?\n";
  }
}//ok

void report_err( char * msg , uint8_t flag){ //could be a macro 
  fprintf(stderr, "error : %s at %s\n", str_flag(flag), msg);
}

void report_warning( char * msg , uint8_t flag){ //could be a macro 
  fprintf(stderr, "warning : %s at %s\n", str_flag(flag), msg);
}


uint8_t write_lines( GraphTable * gt ,FILE * stream ){
  /*
  writes the lines in the order they appear in the linearr 
  in order for the python script to be able to make sense of the flux 
  */
  if(!gt){report_err("dump_lines", GT_NULL); return GT_NULL;}
  if(!stream){ report_err("dump_lines", ERRFLAG_NOFILE); return ERRFLAG_NOFILE;}

  for(uint32_t i =0 ; i<gt->table_size; i++){
    for(uint32_t j =0 ; j<gt->entries[i].neighboor_num; j++){
     
      fprintf(stream, "%u %u\n", i, (gt->entries[i].first_neighboor_ref+j)->node_index);
    }
  }

  return GT_OK;
}


void dump_trace(GraphTable * gt, FILE * stream_curnum , FILE * stream_flux, FILE * stream_protester ){
  //the trace function to dump graph after it

   //dumps the bin array of the protester's position in the file for curnum trace
   if(stream_curnum)
    fwrite(gt->wkcn->cur_num, sizeof(uint32_t), gt->wkcn->size, stream_curnum);
   //dumps the binary array of the flux at each line in the file for flux trace
   if(stream_flux)
    fwrite(gt->arrLine->cur_flux, sizeof(uint32_t), gt->arrLine->size, stream_flux);  
   //dumps the bin array of protesters
   if(stream_protester)
    fwrite(gt->warray->array, sizeof(protester), gt->warray->size , stream_protester);
}//tested; ok 
//might be an issue if the protesters have more fields added 
//if it happens separate their immutable states from their mutable ones 


uint8_t printGraphTabVar(GraphTable * gt, FILE * stream){
    /*
    prints the graph of a GraphTab in a adjacency list format 
    probably faster to build one big string and then print it only once instead of calling print 
    every time 
    */
    if(!gt){ report_err("printGraphTab", GT_NULL); return GT_NULL;}
    if(!gt->entries) { report_err("printGraphTab", GT_NULL); return GT_NULL;}
    
    for(uint32_t i=0; i<gt->table_size; i++){
        fprintf(stream, "%u,",i );
        for(uint16_t j=0; j<gt->entries[i].neighboor_num; j++){
            if(gt->entries->first_neighboor_ref){
                if(j!=gt->entries[i].neighboor_num-1) {

                    fprintf(stream, "%u,", (gt->entries[i].first_neighboor_ref+j)->node_index );
         
                }else{
                    fprintf(stream, "%u", (gt->entries[i].first_neighboor_ref+j)->node_index); 
                } 
            }
        }
        fprintf(stream, "\n");
    }
    return GT_OK;
}// tested; seems ok
