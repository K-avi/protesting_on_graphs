#include "common.h"
#include "graph_table.h"
#include "misc.h"
#include "movement.h"
#include "walker.h"
#include "tactics.h"
#include "restart_sim.h"


#include <time.h>


int main(int argc , char ** argv){

    int8_t c;
    uint8_t helpset=0 , dumpset=0, loadset=0;
    char * trace_name =NULL, *warray_name =NULL;
    while ((c = getopt(argc, argv, "hd:w:")) != -1) {
        
        switch (c) {
        case 'h':
            helpset=1;
            break;
 
        case 'd':
            dumpset=2;
            trace_name = optarg;
            break;
        case 'w':
            loadset = 2; 
            warray_name = optarg;   
            break;     

        case '?':
            if(optopt=='d'){
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                
            }else if(optopt=='w'){
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            }else{
                fprintf(stderr, "Unknown option character `\\x%x'.", optopt);
                exit(ERRFLAG_INVALIDOPT);
            }
            break;
        default:
            abort();
        }
    }

    if(helpset){ //prints help
        fprintf(stdout, "usage : ./walking_on_graphs path/of/graph nb_walker nb_iterations rule1:coeff rule2:coeff\n \
        check out the docu directory for more informations\n");
        exit(0);
    }
    if(argc < 4){ //checks that the number of args is ok
        fprintf(stderr, "1usage : ./walking_on_graphs path/of/graph nb_walker nb_iterations rule1:coeff rule2:coeff\n");
        return ERRFLAG_NOFILE;
    }

    uint8_t optset = dumpset + loadset; //number of args to remove 

    char * path = argv[1+optset]; 
    char * end=argv[2+optset];

    //parses number of walkers
    double walker_coeff=  (double) strtod( argv[2+optset], &end );
    if(end == argv[2+optset]){
        fprintf(stderr, "2usage : ./walking_on_graphs path/of/graph nb_walker nb_iterations rule1:coeff rule2:coeff\n");
        return ERRFLAG_INVALID_ARG;
    }

    //parses number of iterations
    end= argv[3+optset];
    uint32_t iteration_num = (uint32_t ) strtol( argv[3+optset], &end , 10);
 
    if(end== argv[3+optset]){
        fprintf(stderr, "3usage : ./walking_on_graphs path/of/graph nb_walker nb_iterations rule1:coeff rule2:coeff\n");     
        return ERRFLAG_INVALID_ARG;
    }

    //init timer
    time_t timer;
    time( &timer);
    srand(timer);

    //init graph 
    GraphTable gtable; 
    uint8_t failure = loadGraphTab(&gtable, path, walker_coeff, 0);
    if(failure){ report_err("in main loadGraphTab call", failure); exit(failure);}


    //init tactics
    Tactics tactics; 
    failure= initTactics(&tactics, DEFAULT_CAPA_TACTICS);
    if(failure) {report_err("in main loadGraphTab call", failure); exit(failure);}
    //tries to parse the tactics arg if they are present 
    //if the program is called without it ; simply uses the rand rule
    if(argc>4){
        failure=parse_args(&tactics, argc-4-optset, (argv+4+optset));
        if(failure){ report_err("in main loadGraphTab call", failure); exit(failure);}
    }else{//hmmm
        failure = parse_args(&tactics, 0, NULL);
         if(failure){ report_err("in main loadGraphTab call", failure); exit(failure);}
    }
   
    //starts the simulation w randpos or pos stored at warray_name
    if(!loadset){
        failure= init_pos(&gtable);
        if(failure){report_err("in main init_pos call", failure); exit(failure);}
    }else{
     
        failure = load_warray(&gtable, warray_name);
        if(failure){report_err("in main load_warray call", failure); exit(failure);}       
    }

    if(!optset){
        failure=iterate_ntimes(&gtable, &tactics, iteration_num);
        if(failure){report_err("in main iterate_ntimes call", failure); exit(failure);}
    }else{

        failure=iterate_ntimes_dump(&gtable, &tactics, iteration_num, trace_name);
        if(failure){report_err("in main iterate_ntimes_dump call", failure); exit(failure);}

    }

    //frees memory
    freeGraphTab(&gtable);
    freeTactics(&tactics);
    
    return 0;
}