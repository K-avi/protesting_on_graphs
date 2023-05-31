#include "common.h"
#include "graph.h"
#include <stdint.h>
#include <stdio.h>

int main(int argc , char ** argv){

    //placeholder; I'll prolly need to pass some parameters to the programs to setup the simulation
    if(argc != 2){ 
        fprintf(stderr, "pass the path of the file to load as argument\n");
        return ERRFLAG_NOFILE;
    }

    char * path = argv[1]; 
    
    Graph g; 
    initGraph(&g);

    uint8_t succes_flag=loadGraph( &g, path ); //should prolly change load graph actually bc this is stupid

    if(succes_flag){
        freeGraph(&g);
        return succes_flag;
    }

    freeGraph(&g);
    return 0;
}