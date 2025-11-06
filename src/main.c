/** 
 * Main Entry Point for City Finder Application
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "adj_list.h"
#include "utils.h"
#include "dijkstra.h"




int main(int argc, char *argv[]) {

    int user_selection = 0;

    while(user_selection!=3){
        // user selection
        helper();
        scanf("%d", &user_selection);
        switch (user_selection) {
            case 1:
                printf("in load data");
                break;
            case 2:
                printf("Shortest Path");
                break;
            case 3:
                printf("in exit App");
                return 0;
            default:
                printf("❌ Invalid Selection please select a number 1-3");
                break;
        }
    }
    return 0;
}