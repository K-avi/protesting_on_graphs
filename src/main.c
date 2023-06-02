#include "common.h"
#include <stdint.h>
#include <stdio.h>

int main(int argc , char ** argv){

    //placeholder; I'll prolly need to pass some parameters to the programs to setup the simulation
    if(argc != 2){ 
        fprintf(stderr, "pass the path of the file to load as argument\n");
        return ERRFLAG_NOFILE;
    }

    char * path = argv[1]; 

    return 0;
}