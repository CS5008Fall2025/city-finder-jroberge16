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


void shorttest_path_service(char *folder_path_edges, char *folder_path_nodes){
    NeuHashtable* hashtable = read_vertices(folder_path_nodes);
    char src[10];
    char dest[10];
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

    // substantiate graph
    print_keys(hashtable);
    AdjListGraph * graph = createGraph(hashtable, false);
    loadFromFile(graph, folder_path_edges);
    printGraph(graph);
    
    // find the shortest paths
    int dist[graph->numVertices];
    int prev[graph->numVertices];
    
    dijkstra(graph, get_item(hashtable, src)->vertextIndex, dist, prev);
    printAllSolutions(dist, prev, graph);


    printTheShortestPath(src, dest, dist, prev, graph);
    
    freeGraph(graph);
    free_hashtable(hashtable);
}