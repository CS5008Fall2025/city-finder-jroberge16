/** 
 * Main Entry Point for City Finder Application
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "adjList.h"
#include "utils.h"
#include "dijkstra.h"
#include "debug.h"
#include "GraphReader.h"
#include "NeuHashtable.h"



int __check_city_selection(NeuHashtable* hashtable, char* src, char* dest ){

    Item* srcitem = get_item(hashtable, src);
    Item* destitem = get_item(hashtable, dest);

    if(srcitem == NULL|| destitem == NULL){
        printf("\n\nInvalid city selection");
        return 1;
    } else {
        printf("good Selection");
        return 0;
    }
}


int __proccess_city_selection(NeuHashtable* hashtable, char* src, char* dest){
    Item* srcitem = get_item(hashtable, src);
    Item* destitem = get_item(hashtable, dest);
    
    if(srcitem == NULL|| destitem == NULL){
        printf("Invalid Command");
        DEBUG_PRINT(DEBUG_ERROR,"\n❌ Invalid city selection");
        return 1;
    } else {
        DEBUG_PRINT(DEBUG_INFO,"\n✅ Good selection");
        return 0;
    }
}

bool __continue_finding_menu(){
    char userchoice =  '\0';
    while(1){
        printf("\n🏢  Would you like to find find another shorter path? (y/n)\n");
        scanf(" %c", &userchoice);
        if(userchoice == 'y' || userchoice=='Y'){
            return 1;
        } else if (userchoice == 'n' || userchoice=='N'){
            return 0;
        } else {
            printf("❌ Invalid selection. Please select y or n");
        }
    }
}



void shorttest_path_service(NeuHashtable* hashtable, AdjListGraph * graph, char *src, char *dest){

    __proccess_city_selection(hashtable, src, dest);
    int dist[graph->numVertices];
    int prev[graph->numVertices];
    
    dijkstra(graph, get_item(hashtable, src)->vertextIndex, dist, prev);
    
    if (debug_level >= DEBUG_INFO){
        printf("\nℹ️ Hash Table Visual");
        print_table_visual(hashtable);
        printf("\nℹ️ Graph Print Out");
        printGraph(graph);
        printf("\nℹ️ Keys to hastable");
        print_keys(hashtable);
        printf("\nℹ️ All Solutions");
        printAllSolutions(dist, prev, graph);
    }

    printTheShortestPath(src, dest, dist, prev, graph);
    get_next_command(hashtable, graph);
}

