#include "common.h"
#include "graph_table.h"
#include "memory.h"
#include "tactics.h"
#include "walker.h"
#include "movement.h"

#include <stdio.h>



int main(){

    /*tests 29/05


   // Graph g;
    //initGraph(&g);

   // uint32_t test_neighboors[2]= { 1 , 3};

   // addAdjList(&g, 0, 2, test_neighboors);
   */

    /*tests 30/05

    printf("%u\n", g.ArrNode->array[0].node_index);
    printf("%u\n", g.ArrLine->array[0].node_index);
    printf("%u\n", g.ArrLine->array[1].node_index);

   // printGraph(&g, stdout);

    
    uint32_t test_neighboors2[3]= { 0 , 2, 3};
    addAdjList(&g, 1, 3, test_neighboors2);

    printGraph(&g, stdout);

    writeGraph(&g, "test_graph/test_g1.csv");
    
    freeGraph(&g);
    

    Graph g1; 

    initGraph(&g1);
    loadGraph(&g1, "test_graph/test_g1.csv");

    printf("g1\n");

    printGraph(&g1, stdout);

    freeGraph(&g1);
    */

    /*
    tests 01/06
    

    WalkerArray warray; warray.array=NULL;
    WalkerTable wtable; wtable.table=NULL;

    initWalkerArray(&warray, DEFAULT_CAPA_WALKARRAY);
    initWalkerTable(&wtable, DEFAULT_CAPA_WALKTABLE, DEFAULT_CAPA_WTE);

    //initPos(&wtable, &warray);

    freeWalkerArray(&warray);
    freeWalkerTable(&wtable);
    

    GraphTable gt; 
 

    initGraphTab(&gt, 12 ,10, DEFAULT_CAPA_WTE);

    appLineGt(&gt, 1, 0);
    appLineGt(&gt, 2, 0);
    appNodeGt(&gt, 0, 2, &gt.arrLine->array[0]);

    appNodeGt(&gt, 1, 0, NULL);
    appNodeGt(&gt, 2, 0, NULL);

    //writeGraphTab(&gt, "test_graph/gt_test1.csv");

  //  freeGraphTab(&gt);

    GraphTable gt1; 
    
    uint8_t failure = loadGraphTab( &gt1, "test_graph/gt_test0.csv", DEFAULT_CAPA_WTE);

    if(failure) printf("failure code : %u\n", failure);
    printGraphTab(&gt1, stdout);

    freeGraphTab(&gt1);

    Graph g; 
    initGraph(&g);
    uint8_t failure1 = loadGraph(&g, "test_graph/test_g1.csv" );

    if(failure1) printf("failure code : %u\n", failure1);
    printGraph(&g, stdout);

    freeGraph(&g);
    */

    /*
    tests 02/06
    */
    /*
    GraphTable gt1; 
    
    uint8_t failure = loadGraphTab( &gt1, "test_graph/gt_test0.csv", DEFAULT_CAPA_WTE, 0);
    if(failure) printf("failure code : %u\n", failure);
   
   // printGraphTab(&gt1, stdout);

    WalkerArray warray; 

    initWalkerArray(&warray, 30);

    initPos(&gt1, &warray);

    printEntriesGT(&gt1, stdout);

   // uint8_t test_fail=  addEntryGT(&gt1, 15, &warray.array[2] );
    //if(test_fail) printf("test_fail code : %u\n", test_fail);

   // uint8_t rmfail = removeEntryGT(&gt1, 1, gt1.entries[1].walker_entry.walkers[0]->id);
//    if(rmfail) printf("rmfail code : %u\n", rmfail);

   // printf("%p",  gt1.entries[0].walker_entry.walkers[0]);

    removeEntryGT(&gt1, 0 , gt1.entries[0].walker_entry.walkers[0]->id);


    //printf("%p",  gt1.entries[0].walker_entry.walkers[0]);
     printEntriesGT(&gt1, stdout);

    //printEntriesGT(&gt1, stdout);


    moveEntry(&gt1, gt1.entries[1].walker_entry.walkers[0], 1, 0);

    printf("\n-------------------\n");
    printEntriesGT(&gt1, stdout);

    freeWalkerArray(&warray);
    freeGraphTab(&gt1);


    Tactics t ; 

    initTactics(&t, 5);

    addRule(&t, 1, &ruleRandVar);

    freeTactics(&t);
*/
    GraphTable gtParis; 

    uint8_t failure = loadGraphTab(&gtParis, "city_graph/paris_test.csv", 200, 0);
    if(failure) printf("failure code : %u\n", failure);

  //printGraphTab(&gtParis, stdout);

    freeGraphTab(&gtParis);

    return 0;
}