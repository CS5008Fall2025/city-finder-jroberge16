#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>

#include "adjList.h"
#include "utils.h"
#include "dijkstra.h"
#include "debug.h"
#include "GraphReader.h"
#include "NeuHashtable.h"
#include "shortestPathService.h"





/**
 * This displays the help menu
 */
void helper(){
    // prints the help menu
    printf(
        "\n*****Welcome to the shortest path finder!******\n"
        "Commands:"
        "\n\tlist - list all cities"
        "\n\t<city1> <city2> - find the shortest path between two cities"
        "\n\thelp - print this help message"
        "\n\texit - exit the program"
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
    // sets debug level
    if(argc > 3) {
        int level = atoi(argv[3]);
        set_debug_level(level);
        DEBUG_PRINT(DEBUG_INFO, "Setting debug level to: %d\n", level);  // Add this to verify
    }

    // maps the node folder to folder path these are the files we read from
    if(argc > 1) {
        DEBUG_PRINT(DEBUG_INFO, "Setting Node Folder: %s\n", argv[1]);
        if (access(argv[1], F_OK) == 0) {
            strncpy(folder_path_nodes, argv[1], 100);
            folder_path_nodes[255] = '\0';
        } else {
            DEBUG_PRINT(DEBUG_ERROR, "Node folder path '%s' does not exist.\n", argv[1]);
            exit(EXIT_FAILURE);
        }
    }

    // maps in edge folder path which contains edge connections
    if(argc > 2) {
        DEBUG_PRINT(DEBUG_INFO, "Setting Edge Folder: %s\n", argv[2]);
        if (access(argv[2], F_OK) == 0) {
            strncpy(folder_path_edges, argv[2], 100);
            folder_path_edges[255] = '\0';
        } else {
            DEBUG_PRINT(DEBUG_ERROR, "Edge folder path '%s' does not exist.\n", argv[2]);
            exit(EXIT_FAILURE);
        }

    }
    // default debugger level
    if(argc < 3) {
        DEBUG_PRINT(DEBUG_INFO, "No Debug Level Provided\n");
        set_debug_level(DEBUG_NONE);
    }
}

/**
 * exits the program cleanly
 * @param hashtable hashtable of vertices->index.
 * @param graph adjacency list.
 */
void __clean_exit(NeuHashtable* hashtable, AdjListGraph* graph){
    // cleans up resources afer exit
    free_hashtable(hashtable);
    freeGraph(graph);
}

/**
 * Get the next command and runs it (list, help, exit, or shortest path)
 * @param hashtable hashtable of vertices->index.
 * @param graph adjacency list.
 */
void get_next_command(NeuHashtable* hashtable, AdjListGraph* graph){
    char user_selection[20];
    char src[50];
    char dest[50];
    while(1){
        // user can ony choose list help exit or shortest path
        printf("\nWhere do you want to go today?");

        if (!fgets(user_selection, sizeof(user_selection), stdin)) {
            continue;  // input error, retry
        }
        user_selection[strcspn(user_selection, "\n")] = '\0';
        
        // gets src and dest from user
        int count = sscanf(user_selection, "%49s %49s", src, dest);

        if (strcmp(user_selection, "list") == 0){
            // print all vertex's proper names
            print_keys(hashtable);
        }
        // pulls up help menu
        else if (strcmp(user_selection, "help") == 0){
            helper();
        }
        // exits the program
        else if (strcmp(user_selection, "exit") == 0){
            printf("\nGoodbye!");
            __clean_exit(hashtable, graph);
            exit(0);
        }
        // 
        else if (count == 2){
            // run shortest path service ie dyjkstra    
            shorttest_path_service(hashtable, graph, src, dest);
        } else {
            printf("Invalid Command.");
        }
    }
}



/**
 * Main entry point for the program.
 * - comandline breakdown: map.out <vertices> <edges> <debug level([0-4])>
 * @param argc arg count.
 * @param argv arg lsit.
 */
int main_program(int argc, char *argv[]) {
    // default file paths
    char folder_path_edges[] = "./data/distances.txt";
    char folder_path_nodes[] = "./data/vertices.txt";
    
    // assign comandline argsto proper paths and debug level
    process_command_line_args(argc, argv,  folder_path_edges, folder_path_nodes);

    // Read in graph and vertexes:
    NeuHashtable* hashtable = read_vertices(folder_path_nodes);
    
    // create graph from hashtable
    AdjListGraph * graph = createGraph(hashtable, false);

    // load in edges from file to graph
    loadFromFile(graph, folder_path_edges);
    
    // print help menue
    helper();

    // process user commands
    get_next_command(hashtable, graph);


    // functions that require user input where manually checked
    return 0;
}

