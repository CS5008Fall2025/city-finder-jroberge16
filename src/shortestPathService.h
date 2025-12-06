#ifndef SHORT_PATH_H
#define SHORT_PATH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debug.h"


// void shortest_path_menu(char *folder_path_edges, char *folder_path_nodes);

/**
 * Contains of the logic for presenting the dijkstra shortest path service to the user
 * @param hashtable hashtable of vertices->index.
 * @param graph adjacency list.
 * @param src source proper name
 * @param dest destination proper name 
 */
void shorttest_path_service(NeuHashtable* hashtable, AdjListGraph * graph, char *src, char *dest);

#endif