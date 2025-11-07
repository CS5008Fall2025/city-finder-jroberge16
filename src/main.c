/** 
 * Main Entry Point for City Finder Application
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdlib.h>


#include "adj_list.h"
#include "utils.h"
#include "dijkstra.h"
#include "debug.h"
#include "GraphReader.h"





int main(int argc, char *argv[]) {

    int user_selection = 0;

    char folder_path_edges[] = "./data/distances.txt";
    char folder_path_nodes[] = "./data/vertices.txt";

    process_command_line_args(argc, argv,  folder_path_edges, folder_path_nodes);

    while(user_selection!=3){
        helper(folder_path_edges, folder_path_nodes);        
        scanf("%d", &user_selection);

        switch (user_selection) {
            case 1:
                printf("Shortest Path");
                break;
            case 2:
                printf("in exit App");
                return 0;
            default:
                printf("❌ Invalid Selection please select a number 1-3");
                break;
        }
    }
    return 0;
}