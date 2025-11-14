#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "debug.h"




/**
 * This displays the help menu
 */
void helper(char *folder_path_edges, char *folder_path_nodes){
    printf(
        "\n=====================================================================\n"
        "=====\t\t\t 🏢   City Finder 🏢 \t\t\t=====\n"
        "=====================================================================\n"
        "Welcome to the City Finder application. This application is designed \nto find you the shortest path between two cities.\n"
        "\n"
        "Select a Number:\n"
        "\t1. Find the Shortest Path\n"
        "\t2. Exit Application\n\n"
        "\n ℹ️   Current Files: {'Nodes': '%s', 'Edges': '%s'}"
        "\n ℹ️   comandline breakdown: map.out <vertices> <edges> <debug level([0-4])>"
        "\n ℹ️   current debug level: %d\n"

        "\nPLEASE MAKE A SELECTION:", folder_path_nodes, folder_path_edges, debug_level
    );
}

/**
 * Takes Comandline arguments and processes them and set the debug level.
 * 
 * @param argc arg count
 * @param argv arg vector
 * @param folder_path_edges edge path
 * @param folder_path_nodes node path
 */
void process_command_line_args(int argc, char *argv[], char *folder_path_edges, char *folder_path_nodes){
    if(argc > 3) {
        int level = atoi(argv[3]);
        set_debug_level(level);
        DEBUG_PRINT(DEBUG_INFO, "Setting debug level to: %d\n", level);  // Add this to verify
    }
    if(argc > 1) {
        DEBUG_PRINT(DEBUG_INFO, "Setting Node Folder: %s\n", argv[1]);
        strncpy(folder_path_nodes, argv[1], 100);
        folder_path_nodes[255] = '\0';
    }
    if(argc > 2) {
        DEBUG_PRINT(DEBUG_INFO, "Setting Edge Folder: %s\n", argv[2]);
        strncpy(folder_path_edges, argv[2], 100);
        folder_path_edges[255] = '\0';
    }
    if(argc < 3) {
        DEBUG_PRINT(DEBUG_INFO, "No Debug Level Provided\n");
        set_debug_level(DEBUG_NONE);
    }
}



