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



void print_results(int result, int expectation ,char* function_name){
    if(result==expectation){
        printf("\n\t✅ %s", function_name);
    }else{
        printf("\n\t❌ %s (expectation:%d, reality: %d)", function_name, expectation, result);
    }

}

void run_tests(void){
    printf("\n\n🧪 Testing Hashtable Creation:");
    NeuHashtable* hashtable = read_vertices("data/vertices.txt");
    if(hashtable!=NULL){
        print_results(get_item(hashtable, "a")->vertextIndex, 
        0,"hash table Created");
    }
    print_results(hashtable->capacity, 32,"Total correct Items");
    print_results(get_item(hashtable, "a")->vertextIndex, 
    0,"hash table item Location");


    printf("\n\n🧪 Test Graph Creation:");
    AdjListGraph * graph = createGraph(hashtable, false);
    print_results(graph->numVertices, 0,"Total correct before edges vertices");
    loadFromFile(graph, "data/distances.txt");
    print_results(graph->numVertices, 7,"Total correct after edges vertices");

    printf("\n\n🧪 Testing Shortest Path");
    int dist[graph->numVertices];
    int prev[graph->numVertices];
    
    dijkstra(graph, 0, dist, prev);
    print_results(dist[1], 2, "distance from a to b");
    print_results(dist[5], 10, "distance from a to f");
    print_results(dist[3], 3, "distance from a to d");
    
    dijkstra(graph, 3, dist, prev);
    print_results(dist[1], 1, "distance from d to b");
    print_results(dist[5], 10, "distance from d to f");
    print_results(dist[4], 7, "distance from d to d");

    printf("\n");
















}


int main(int argc, char *argv[]){
    printf("================== 🧪 Testing C-Fib Code 🧪 ==================");
    run_tests();
    return 0;
} 