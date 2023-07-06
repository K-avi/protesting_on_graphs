#include "restart_sim.h"
#include "misc.h"
#include "walker.h"
#include <stdint.h>



uint8_t load_warray( GraphTable * gtable, char * path){
    /*
    loads position of walkers from a binary array stored at path
    */
    if(!gtable){ report_err("load_warray", GT_NULL); return GT_NULL;}
    if(!path){ report_err("load_warray", WA_NOREAD); return WA_NOREAD;}

    FILE * f = fopen(path, "r");
    if(!f){
        report_err("load_warray", WA_OPEN); return WA_OPEN;
    }
    uint32_t count= gtable->wknum;
    //clears the walker position
   
    uint32_t warray[count];
    memset(warray,0, count*sizeof(uint32_t));
    //loads it ; might have an error there i dunno
    uint32_t blocs_read = fread( warray, sizeof(uint32_t), count, f);
    if( blocs_read != count){// fread returns the nb of blocs read so this is a way to check for failure
        fclose(f); 
        report_err("load_warray", WA_READFAIL); 
        return WA_READFAIL;
    }

    memset(gtable->wkcn->next_num, 0, sizeof(uint32_t)* gtable->table_size);

    for(uint32_t i=0; i<count ; i++){//sets walker counter node to new values
        (gtable->wkcn->next_num[warray[i]])++; //error prone
    }
    fclose(f);
    return WA_OK;
}//done ; tested ; seems ok 