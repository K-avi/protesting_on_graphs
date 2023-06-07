#include "common.h"
#include "graph_table.h"
#include "misc.h"
#include "movement.h"
#include "walker.h"
#include "tactics.h"
#include <stdint.h>
#include <stdlib.h>


int main(int argc , char ** argv){

    //placeholder; I'll prolly need to pass some parameters to the programs to setup the simulation
    if(argc < 4){ 
        fprintf(stderr, "usage : ./walking_on_graphs path/of/graph nb_walker nb_iterations rule1:coeff rule2:coeff\n");
        return ERRFLAG_NOFILE;
    }

    char * path = argv[1]; 
    char * end=argv[2];

    uint32_t walker_num=  (uint32_t) strtol( argv[2], &end , 10);
    if(end == argv[2]){
        fprintf(stderr, "usage : ./walking_on_graphs path/of/graph nb_walker nb_iterations rule1:coeff rule2:coeff\n");
        return ERRFLAG_INVALID_ARG;
    }

    end= argv[3];
    uint32_t iteration_num = (uint32_t ) strtol( argv[2], &end , 10);
    if(end== argv[3]){
        fprintf(stderr, "usage : ./walking_on_graphs path/of/graph nb_walker nb_iterations rule1:coeff rule2:coeff\n");
        return ERRFLAG_INVALID_ARG;
    }

    GraphTable gtable; 
    uint8_t failure = loadGraphTab(&gtable, path, walker_num, 0);
    if(failure) report_err("in main loadGraphTab call", failure);

    Tactics tactics; 
    failure= initTactics(&tactics, DEFAULT_CAPA_TACTICS);
    if(failure) report_err("in main loadGraphTab call", failure);

    failure=parse_args(&tactics, argc-4, (argv+4));
    if(failure) report_err("in main loadGraphTab call", failure);

    failure=iterate_ntimes(&gtable, &tactics, iteration_num);
    if(failure) report_err("in main iterate_ntimes call", failure);

    freeGraphTab(&gtable);
    freeTactics(&tactics);
    
    return 0;
}