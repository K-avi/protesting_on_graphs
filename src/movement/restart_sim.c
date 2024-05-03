#include "restart_sim.h"
#include "../misc.h"
#include "../graph_protesters/protester.h"



uint8_t load_warray( GraphTable * gtable, char * path){
    /*
    loads position of protesters from a binary array stored at path
    */
    if(!gtable){ report_err("load_warray", GT_NULL); return GT_NULL;}
    if(!path){ report_err("load_warray", WA_NOREAD); return WA_NOREAD;}

    FILE * f = fopen(path, "r");
    if(!f){
        report_err("load_warray", WA_OPEN); return WA_OPEN;
    }
    uint32_t count= gtable->warray->size;
    //clears the protester position
    memset(gtable->warray->array, 0, sizeof(uint32_t)* count);

    //loads it ; might have an error there i dunno
    uint32_t blocs_read = fread( gtable->warray->array, sizeof(protester), count, f);
    if( blocs_read != count){// fread returns the nb of blocs read so this is a way to check for failure
        fclose(f); 
        report_err("load_warray", WA_READFAIL); 
        return WA_READFAIL;
    }

    memset(gtable->wkcn->next_num, 0, sizeof(uint32_t)* gtable->table_size);

    for(uint32_t i=0; i<count ; i++){//sets protester counter node to new values
        (gtable->wkcn->next_num[gtable->warray->array[i].index_entry])++; //error prone
    }
    fclose(f);
    return WA_OK;
}//done ; tested ; seems ok 