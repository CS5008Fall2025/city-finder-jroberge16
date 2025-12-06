#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debug.h"

#include "adjList.h"
#include "NeuHashtable.h"

/**
 * This displays the help menu
 */
void helper();

/**
 * Takes Comandline arguments and processes them and set the debug level.
 * 
 * @param argc arg count
 * @param argv arg vector
 * @param folder_path_edges edge path
 * @param folder_path_nodes node path
 */
void process_command_line_args(int argc, char *argv[], char *folder_path_edges,char *folder_path_nodes);

// void shortest_path_menu(char *folder_path_edges, char *folder_path_nodes);

/**
 * Main entry point for the program.
 * - comandline breakdown: map.out <vertices> <edges> <debug level([0-4])>
 * @param argc arg count.
 * @param argv arg lsit.
 */
int main_program(int argc, char *argv[]);

/**
 * Get the next command and runs it (list, help, exit, or shortest path)
 * @param hashtable hashtable of vertices->index.
 * @param graph adjacency list.
 */
void get_next_command(NeuHashtable* hashtable, AdjListGraph* graph);

#endif