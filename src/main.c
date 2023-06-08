#include "common.h"
#include "graph_table.h"
#include "misc.h"
#include "movement.h"
#include "walker.h"
#include "tactics.h"
#include <time.h>


int main(int argc , char ** argv){

    if(argc < 4){ //checks that the number of args is ok
        fprintf(stderr, "usage : ./walking_on_graphs path/of/graph nb_walker nb_iterations rule1:coeff rule2:coeff\n");
        return ERRFLAG_NOFILE;
    }
    char * path = argv[1]; 
    char * end=argv[2];

    //parses number of walkers
    uint32_t walker_num=  (uint32_t) strtol( argv[2], &end , 10);
    if(end == argv[2]){
        fprintf(stderr, "usage : ./walking_on_graphs path/of/graph nb_walker nb_iterations rule1:coeff rule2:coeff\n");
        return ERRFLAG_INVALID_ARG;
    }

    //parses number of iterations
    end= argv[3];
    uint32_t iteration_num = (uint32_t ) strtol( argv[2], &end , 10);
    if(end== argv[3]){
        fprintf(stderr, "usage : ./walking_on_graphs path/of/graph nb_walker nb_iterations rule1:coeff rule2:coeff\n");     
        return ERRFLAG_INVALID_ARG;
    }

    //init timer
    time_t timer;
    time( &timer);
    srand(timer);


    //init graph 
    GraphTable gtable; 
    uint8_t failure = loadGraphTab(&gtable, path, walker_num, 0);
    if(failure){ report_err("in main loadGraphTab call", failure); exit(failure);}


    //init tactics
    Tactics tactics; 
    failure= initTactics(&tactics, DEFAULT_CAPA_TACTICS);
    if(failure) {report_err("in main loadGraphTab call", failure); exit(failure);}

    //tries to parse the tactics arg if they are present 
    //if the program is called without it ; simply uses the rand rule
    if(argc>4){
        failure=parse_args(&tactics, argc-4, (argv+4));
        if(failure){ report_err("in main loadGraphTab call", failure); exit(failure);}
    }else{
        failure = addRule(&tactics, 255,  &rule_rand);
        if(failure){ report_err("in main loadGraphTab call", failure); exit(failure);}
    }
    
    //starts the simulation 

    failure= init_pos(&gtable);
    if(failure){report_err("in main init_pos call", failure); exit(failure);}
    failure=iterate_ntimes(&gtable, &tactics, iteration_num);
    if(failure){report_err("in main iterate_ntimes call", failure); exit(failure);}


    //frees memory
    freeGraphTab(&gtable);
    freeTactics(&tactics);
    
    return 0;
}