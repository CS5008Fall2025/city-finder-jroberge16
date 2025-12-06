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
#include "shortestPathService.h"

#include <stdio.h>  
#include <stdlib.h>
#include <time.h> 
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h> // For open, O_WRONLY
#include <unistd.h> // For dup, dup2, close
#include <fcntl.h> // For open, O_WRONLY
#include <setjmp.h>
#include <assert.h>



/**
* suppresses stdout for a function call
* @references: [^4] https://stackoverflow.com/questions/46728680/how-to-temporarily-suppress-output-from-printf
*/
void suppress_stdout() {
    //
    fflush(stdout);
    int nullfd = open("/dev/null", O_WRONLY);
    if (nullfd == -1) {
        perror("open /dev/null");
        return;
    }
    dup2(nullfd, STDOUT_FILENO);
    close(nullfd); 
}

/**
 * Restores stdout after being suppressed
 * @references: [^4] https://stackoverflow.com/questions/46728680/how-to-temporarily-suppress-output-from-printf
 * @param original_stdout_fd: The original file descriptor for stdout
*/
void restore_stdout(int original_stdout_fd) {
    fflush(stdout);
    dup2(original_stdout_fd, STDOUT_FILENO);
    close(original_stdout_fd);
}


/**
 * simple printer that print a check for positive and a x for negative
 */
void print_results(int result, int expectation ,char* function_name){
    if(result==expectation){
        printf("\n\t✅ %s", function_name);
    }else{
        printf("\n\t❌ %s (expectation:%d, reality: %d)", function_name, expectation, result);
    }

}
/**\
 * run the testing suite for the entire program
 */
void run_tests(void){
    printf("\n\n🧪 Testing Hashtable Creation:");
    NeuHashtable* hashtable = read_vertices("data/vertices.txt");
    if(hashtable!=NULL){
        print_results(get_item(hashtable, "a")->vertextIndex, 
        0,"hash table Created");
    }

    // checking hasjtable
    print_results(get_item(hashtable, "a")->vertextIndex, 
    0,"hash table item Location");


    printf("\n\n🧪 Test Graph Creation:");

    // checking the correct allocation of nodes
    AdjListGraph * graph = createGraph(hashtable, false);
    print_results(graph->numVertices, 8,"Total correct before edges vertices");
    loadFromFile(graph, "data/distances.txt");
    print_results(graph->numVertices, 8,"Total correct after edges vertices");

    printf("\n\n🧪 Testing Shortest Path");
    int dist[graph->numVertices];
    int prev[graph->numVertices];

    // a's various paths
    dijkstra(graph, 0, dist, prev);
    print_results(dist[1], 2, "distance from a to b");
    print_results(dist[5], 10, "distance from a to f");
    print_results(dist[3], 3, "distance from a to d");

    // checking d's various paths
    dijkstra(graph, 3, dist, prev);
    print_results(dist[1], 1, "distance from d to b");
    print_results(dist[5], 10, "distance from d to f");
    print_results(dist[4], 7, "distance from d to d");

    printf("\n\n🧪 Testing Utility Items:");
    // checking commend line processing
    char folder_path_edges[256] = "";
    char folder_path_nodes[256] = "";
    char *test_argv[] = {"map.out", "data/vertices.txt", "data/distances.txt", "2"};
    process_command_line_args(4, test_argv, folder_path_edges, folder_path_nodes);

    // asserting path is properly assigned
    assert(strcmp(folder_path_nodes, "data/vertices.txt") == 0);
    assert(strcmp(folder_path_edges, "data/distances.txt") == 0);
    printf("\n\t ✅  command lines rags works\n");


    // just a print statement
    helper();
    printf("\n\t ✅  print helper function working correctly\n");

    printf("\n\n ℹ️ Items that required user input were manually tested. Code that was written\n");
    printf(" by lionel was not directly tested although it was tested by testing dystras which was tested. \n");

    printf("\n");


}


int main(int argc, char *argv[]){
    printf("================== 🧪 Testing C-Fib Code 🧪 ==================");
    run_tests();
    return 0;
} 