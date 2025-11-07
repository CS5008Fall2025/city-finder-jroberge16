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





// NeuHashtable* read_vertices(const char* file_name){
//     FILE *file;
//     char line[256]; 
//     char *token;
//     int index = 0;
//     NeuHashtable* hashtable = create_hashtable(25);

//     file = fopen(file_name, "r");
//     if (file == NULL){
//         printf("❌ Invalid File");
//         return;
//     }
    
//     while(fgets(line, sizeof(line), file)) {
//         line[strcspn(line, "\r\n")] = '\0';
        
//         if (strlen(line) == 0) {
//             continue;
//         }
//         add_item(hashtable, line, index);
//         index++;
//     }
//     fclose(file);
//     return hashtable;
// }




int main(int argc, char *argv[]) {

    int user_selection = 0;

    char folder_path_edges[] = "./data/distances.txt";
    char folder_path_nodes[] = "./data/vertices.txt";



    shorttest_path_service(folder_path_edges, folder_path_nodes);

    // NeuHashtable* hashtable;
    // hashtable = read_vertices(folder_path_nodes);
    // print_keys(hashtable);

    // process_command_line_args(argc, argv,  folder_path_edges, folder_path_nodes);

    // while(user_selection!=3){
    //     helper(folder_path_edges, folder_path_nodes);        
    //     scanf("%d", &user_selection);

    //     switch (user_selection) {
    //         case 1:
    //             shorttest_path_service(folder_path_edges, folder_path_nodes);
    //             break;
    //         case 2:
    //             printf("in exit App");
    //             return 0;
    //         default:
    //             printf("❌ Invalid Selection please select a number 1-3");
    //             break;
    //     }
    // }
    // return 0;
}