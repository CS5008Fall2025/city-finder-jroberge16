#include <stdio.h>

/**
 * This displays the help menu
 * TODO: Add default starting file
 */
void helper(void){
    printf(
        "\n=====================================================================\n"
        "=====\t\t\t 🏢   City Finder 🏢 \t\t\t=====\n"
        "=====================================================================\n"
        "Welcome to the City Finder application. This application is designed \nto find you the shortest path between two cities.\n"
        "\n"
        "Select a Number:\n"
        "\t1. Change City Information (Default)\n"
        "\t2. Find the Shortest Path\n"
        "\t3. Exit Application\n\n"
        "\n\nℹ️   add -d [1,2,3] to enter debug mode for printing extra information \n"
        "Example: '2 -d 3' enters you in debug mode level 3 for find Shortest path\n"
        "PLEASE MAKE A SELECTION:"
    );
}


