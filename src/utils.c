#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "debug.h"




/**
 * This displays the help menu
 * TODO: Add default starting file
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
        "\n ℹ️   comandline breakdown: map.out <vertices> <edges> <debug level([0-4])>\n"
        "\nPLEASE MAKE A SELECTION:", folder_path_nodes, folder_path_edges
    );
}


void process_command_line_args(int argc, char *argv[], char *folder_path_edges, char *folder_path_nodes){
    if(argc > 1) {
        strncpy(argv[1], folder_path_edges, sizeof(argv[1]) - 1);
    }
    if(argc > 2) {
        strncpy(argv[2], folder_path_edges, sizeof(argv[2]) - 1);
    }
    if(argc > 3) {
        set_debug_level(atoi(argv[3]));
    }else{
        set_debug_level(3);
    }
}



