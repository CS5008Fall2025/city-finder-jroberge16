/** 
 *  * Main Entry Point for City Finder Application
 */

 #include <stdbool.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <errno.h>
 #include <string.h>

 #include "utils.h"



/**
 * Main entry point for the program.
 * - comandline breakdown: map.out <vertices> <edges> <debug level([0-4])>
 * @param argc arg count.
 * @param argv arg lsit.
 */
int main(int argc, char *argv[]) {
    return main_program(argc, argv);
}