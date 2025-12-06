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



/**
 * Check that city selection was valid
 * @param hashtable hashtable of proper name -> index.
 * @param src source proper name .
 * @param dest destination proper name
 * @return 0 is valid else 1
 */
int __proccess_city_selection(NeuHashtable* hashtable, char* src, char* dest){
    // get index of proper name
    Item* srcitem = get_item(hashtable, src);
    Item* destitem = get_item(hashtable, dest);
    // if NULL bad names
    if(srcitem == NULL|| destitem == NULL){
        printf("Invalid Command");
        DEBUG_PRINT(DEBUG_ERROR,"\n❌ Invalid city selection");
        return 1;
    } else {
        // good selection otherwise
        DEBUG_PRINT(DEBUG_INFO,"\n✅ Good selection");
        return 0;
    }
}


/**
 * Contains of the logic for presenting the dijkstra shortest path service to the user
 * @param hashtable hashtable of vertices->index.
 * @param graph adjacency list.
 * @param src source proper name
 * @param dest destination proper name 
 */
void shorttest_path_service(NeuHashtable* hashtable, AdjListGraph * graph, char *src, char *dest){

    __proccess_city_selection(hashtable, src, dest);
    int dist[graph->numVertices];
    int prev[graph->numVertices];
    
    dijkstra(graph, get_item(hashtable, src)->vertextIndex, dist, prev);

    // debugger helpers
    if (debug_level >= DEBUG_INFO){
        printf("\nℹ️ Array in grpah:\n");
        print_array(graph);
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

