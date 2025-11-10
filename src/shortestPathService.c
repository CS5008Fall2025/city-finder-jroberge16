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
        printf("\n❌ Invalid city selection");
        return 1;
    } else {
        printf("\n✅ Good selection");
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

void __city_selection_menu(char* src, char* dest, NeuHashtable* hashtable){
    int city_checker = 1;

    while(city_checker){

    printf("\n\n🏢   From the list below, please select two cities\n");
    print_keys(hashtable);
    
    printf("Please select Two Cities, Example: city1 city2\n\n");
    printf("SELECTION: ");
    scanf("%s %s", src, dest);
    city_checker = __proccess_city_selection(hashtable, src, dest);
    printf("\n\n");
    }
}


void shorttest_path_service(char *folder_path_edges, char *folder_path_nodes){
    char src[150];
    char dest[150];
    bool continue_finding = true;
    
    NeuHashtable* hashtable = read_vertices(folder_path_nodes);
    AdjListGraph * graph = createGraph(hashtable, false);
    loadFromFile(graph, folder_path_edges);
    int dist[graph->numVertices];
    int prev[graph->numVertices];
    
    while(continue_finding){
        printf("debug level %d", debug_level);

        __city_selection_menu(src, dest, hashtable);
        dijkstra(graph, get_item(hashtable, src)->vertextIndex, dist, prev);
        
        // debug printing
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
        
        continue_finding = __continue_finding_menu();
    }
    freeGraph(graph);
    free_hashtable(hashtable);

}

