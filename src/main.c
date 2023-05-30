#include "common.h"
#include "graph.h"
#include <stdio.h>

int main(int argc , char ** argv){

    if(argc != 2){
        fprintf(stderr, "pass the path of the file to load as argument\n");
        return ERRFLAG_NOFILE;
    }

    char * path = argv[1]; //boilerplate to prevent linters from whining
    printf("%s\n", path);


    Graph g;
    initGraph(&g);

    printGraph(&g, stdout);
    
    freeGraph(&g);

    return 0;
}