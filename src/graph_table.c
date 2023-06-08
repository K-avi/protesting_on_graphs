#include "graph_table.h"
#include "common.h"
#include "misc.h"

static uint8_t initGtEntry( GraphTableEntry * gentry ){
    /*
    initialises a non null GraphTableentry
    */
    if(!gentry) return GE_NULL;

    gentry->first_neighboor_ref=NULL;
    gentry->neighboor_num=0;

    return GE_OK;
}//tested; ok


uint8_t swap_flux_curnext(LineArray * larr ){
    /*swaps the flux to get them 
    ready for next iteration
    O(1)*/
    if(!larr) return L_ARRNULL;

    uint32_t * tmp= larr->cur_flux;

    larr->cur_flux= larr->next_flux;

    larr->next_flux=tmp;
    memset(larr->next_flux, 0, larr->size*sizeof(uint32_t));

    return LARR_OK;
}

static uint8_t initnLineArr( LineArray * lineArr, uint32_t arrline_size){
    /*
    initialises an already allocated lineArr
    */
    if(!lineArr) return GT_NULL;

    lineArr->array=NULL; 
    lineArr->array=(Line*) GROW_ARRAY(Line, lineArr->array, 0, arrline_size);
    if(!lineArr->array) return GT_MALLOC;

    lineArr->cur_in=0; 
    lineArr->size=arrline_size;

    lineArr->cur_flux=NULL;
    lineArr->cur_flux=GROW_ARRAY(uint32_t, lineArr->cur_flux, 0, arrline_size);
    if(!lineArr->cur_flux) return GT_MALLOC;

    lineArr->next_flux=NULL;
    lineArr->next_flux=GROW_ARRAY(uint32_t, lineArr->next_flux, 0, arrline_size);
    if(!lineArr->next_flux) return GT_MALLOC;

    return GT_OK;
}// new version not tested ; 

static void freeLineArr( LineArray * lineArr){
    /*
    frees the content of a lineArray
    warning : doesn't free the line array itself
    */
    if(!lineArr) return;
    if(lineArr->array) free(lineArr->array);
    if(lineArr->cur_flux) free(lineArr->cur_flux);
    if(lineArr->next_flux) free(lineArr->next_flux);
} // tested ok

void printLineArr( LineArray * lineArr, FILE * stream){
    /**/
    if(!lineArr){ fprintf(stream," line arr is null\n"); return;}
    
    for(unsigned i=0; i<lineArr->size;i++){
        fprintf(stream,"line,%u:%u:%u\n", lineArr->array[i].node_index, lineArr->cur_flux[i], lineArr->next_flux[i]);
    }
}

uint8_t initGraphTab(GraphTable *gt, uint32_t arrline_size ,uint32_t table_size,  uint32_t warray_size ,uint32_t curgen ){
    /*
    initialises a non null graph table ; sets it's entries to default values and initialises 
    it's walker entries
    */
    if(!gt) return GT_NULL;

    gt->table_size= table_size; 
    gt->curgen=curgen;
    gt->entries=NULL;
    gt->entries=GROW_ARRAY(GraphTableEntry, gt->entries, 0, table_size );

    if(!gt->entries){
        return GT_REALLOC;
    }

    for(uint32_t i=0; i<table_size; i++){
        uint8_t failure = initGtEntry(&gt->entries[i]);
        if(failure) return failure;
    }

    gt->arrLine=malloc(sizeof(LineArray));
    if(!gt->arrLine) return GT_MALLOC;

    gt->warray= malloc(sizeof(WalkerArray));
    if(!gt->warray) return WA_ALLOC;
    uint8_t failure=initWalkerArray(gt->warray, warray_size);
    if(failure) return failure;

    failure= initnLineArr(gt->arrLine, arrline_size);
    if(failure) return failure;

    gt->wkcn= malloc(sizeof(WalkerCurNext));
    if(!gt->wkcn) return GT_MALLOC;

    failure=initWalkerCurNext(gt->wkcn, table_size);
    if(failure)return failure;

    return GT_OK;
}//tested ok;



void freeGraphTab( GraphTable * gt){
    /*
    */
    if(!gt) return;

    free(gt->entries);
    freeWalkerArray(gt->warray);
    free(gt->warray);
    freeLineArr(gt->arrLine);
    free(gt->arrLine);

    freeWalkerCurNext(gt->wkcn);
    free(gt->wkcn);

    return ;
}//tested; ok


static uint8_t appNodeGt (GraphTable * gt, uint32_t node_index , uint32_t neighboor_num, \
                            Line*  first_neighboor_ref){
    /*
    doesn't if node already set to smtg  ; sets the ref of first elem , and neighboor num 
    of a node at index given

    O(1)
    */
    if(!gt) return GT_NULL;

    if( (gt->table_size-1 < node_index) ) return GT_INDEX;

    gt->entries[node_index].first_neighboor_ref=first_neighboor_ref;
    gt->entries[node_index].neighboor_num=neighboor_num;

    return GT_OK;
    
}//tested ; ok


static uint8_t appLineGt( GraphTable * gt , uint32_t node_index, int32_t flux_cur ,int32_t flux_next ){
    /*
    appends ONE line to the line tab of a graph table ;
    checks for valid node 

    O(1)
    */
    if(!gt){ report_err("appLineGt", GT_NULL); return GT_NULL;}
    if(!gt->arrLine){ report_err("appLineGt", GT_NULL); return GT_NULL;}
    if(node_index> gt->table_size) { report_err("appLineGt", GT_INDEX); return GT_INDEX;}
    if(gt->arrLine->cur_in==gt->arrLine->size){ { report_err("appLineGt", GT_ARFULL); return GT_ARFULL;}}

    LineArray* arrline = gt->arrLine;

    arrline->array[arrline->cur_in].node_index=node_index; 
    arrline->cur_flux[arrline->cur_in]=flux_cur;
    arrline->next_flux[arrline->cur_in]=flux_next;
  
    arrline->cur_in++;

    return GT_OK;
}//tested  ; ok

//////////////

uint8_t printGraphTab(GraphTable * gt, FILE * stream){
    /*
    prints the graph of a GraphTab in a adjacency list format 
    probably faster to build one big string and then print it only once instead of calling print 
    every time 
    */
    if(!gt){ report_err("printGraphTab", GT_NULL); return GT_NULL;}
    if(!gt->entries) { report_err("printGraphTab", GT_NULL); return GT_NULL;}
 
    fprintf(stream, "%u,%u\n", gt->table_size, gt->arrLine->size);
    
    for(uint32_t i=0; i<gt->table_size; i++){
        fprintf(stream, "%u,%u,",i ,gt->entries[i].neighboor_num);
        for(uint16_t j=0; j<gt->entries[i].neighboor_num; j++){
            if(gt->entries->first_neighboor_ref){
                if(j!=gt->entries[i].neighboor_num-1) {

                    fprintf(stream, "%u:%u;", (gt->entries[i].first_neighboor_ref+j)->node_index ,
                     gt->arrLine->cur_flux[gt->entries[i].first_neighboor_ref+j - gt->arrLine->array] );
         
                }else{
                    fprintf(stream, "%u:%u", (gt->entries[i].first_neighboor_ref+j)->node_index, \
                        gt->arrLine->cur_flux[gt->entries[i].first_neighboor_ref+j - gt->arrLine->array]); 
                } 
            }
        }
        fprintf(stream, "\n");
    }
    return GT_OK;
}// tested; seems ok


static  bool peek(const char * str, char expected){
    return *str==expected;
}

static bool emptyLine( char * str){
    while(*str==' ' || *str=='\r') str++;
    return (*str=='\0' || *str=='\n');
}

uint8_t loadGraphTab(GraphTable *gt, char *path, uint32_t warray_size,uint32_t curgen){
    /*
    takes a non initialised ; non null , empty gt and loads a graph stored at path into it
    also pass walk table entry size parameter cuz it's better looking than to init w a global var

    if there is more line than space for the graph (i.e : the size of the graph is actually inferior to)
    the number of lines) the lines after the limit of the graph will be ignored
    */
    if(!gt){ report_err("loadGraphTab", GT_NULL); return GT_NULL;}

    FILE * f = fopen(path, "r");
    if(!f) { report_err("loadGraphTab", GT_READFAIL); return GT_READFAIL;};

    char line[256];
    memset(line, 0, 256);

    if(!fgets(line, 256, f)) {//consume first line to retrieve size of graph and size of arrline
        fclose(f);
        report_err("loadGraphTab", GT_NOREAD); 
        return GT_NULL;
    }

    char * end1,*cur1=line;
    uint32_t  table_size = (uint32_t) strtol(cur1, &end1, 10);
    if(peek(end1, ',') && cur1!=end1) cur1=(++end1);
    else{ fclose(f);  report_err("loadGraphTab parse0", GT_PARSE); return GT_PARSE; }
    
    uint32_t arrline_size = (uint32_t) strtol(cur1, &end1, 10);
   
    if(cur1!=end1) cur1=(++end1);
    else{fclose(f); report_err("loadGraphTab parse1", GT_PARSE); return GT_PARSE;}

    //initialises graph with values consumed on the first line
    uint8_t failure= initGraphTab(gt, arrline_size,  table_size,  warray_size,curgen);
    if(failure) { fclose(f);  report_err("loadGraphTab" ,failure); return failure;}

    uint32_t cpt=0; 
    //keep track of nblines consumed ; report error if nbline consumed higher than counter
    
    while(fgets(line, 256, f) && cpt<table_size){
        char* end, *cur=line;
        if(emptyLine(cur)) continue; // ignores empty lines
        cpt++; 

        uint32_t node_index = (uint32_t) strtol(cur, &end, 10); //retrieves index
        if(peek(end, ',') && cur!=end) cur=(++end);
        else{  fclose(f);  report_err("loadGraphTab parse2", GT_PARSE);return GT_PARSE;}

        uint32_t neighboor_num = (uint32_t) strtol(cur, &end, 10); //retrieves neighboor num
        if(peek(end, ',') && cur!=end) cur=(++end);
        else {  fclose(f);  report_err("loadGraphTab parse3", GT_PARSE); return GT_PARSE;}

        //appends node to the node section
        uint8_t errflag= appNodeGt(gt, node_index, neighboor_num, &(gt->arrLine->array[gt->arrLine->cur_in]));
        if(errflag) return errflag; 
        
        //loop to retrieve the formated neighboors 
        for(uint32_t i=0; i<neighboor_num; i++){
            /*
            adds the lines / flux
            */
          
            uint32_t new_neighboor= (uint32_t) strtol(cur, &end, 10);
            if(peek(end, ':') && cur!=end) cur=(++end);    
            else {
                fclose(f);
                report_err("loadGraphTab parse4", GT_PARSE);
                return GT_PARSE;
            }

            int32_t flux= strtol(cur, &end, 10);
            if(i!=neighboor_num-1){
                if(peek(end, ';') && cur!=end ) cur=(++end);
                else {
                    fclose(f);
                    report_err("loadGraphTab parse5", GT_PARSE);
                    return GT_PARSE;
                }
            }else{
                if( cur==end){
                    fclose(f);
                    report_err("loadGraphTab parse6", GT_PARSE);
                    return GT_PARSE; 
                }
            }

            uint8_t errflag_in = appLineGt(gt, new_neighboor, flux, flux);
            if(errflag_in ) return errflag_in;
            
        }
        memset(line, 0, 256);
    }  
    fclose(f);
    return GT_OK;
}//tested; seems ok ; ugly 

//WARNING : initialises both flux_cur and flux_next to the flux value read; it might be a problem 
//if the fn is used wrong ; I might change format to path 2 flux values


uint8_t writeGraphTab(GraphTable * gt, char *path ){
    /*
    more of a wrapper on the graph print function than anything else tbh 
    security flaws ; not sure they are really relevant though
    */
    FILE * f = fopen(path, "w");
    if(!f){
         report_err("writeGraphTab parse0", GT_OPENFAIL);
        return GT_OPENFAIL;
    }

    uint8_t failure= printGraphTab(gt, f);
    fclose(f);
    if(failure) report_err("writeGraphTab",failure);
    return failure;
}//tested; some security flaws ig


