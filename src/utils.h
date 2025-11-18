#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debug.h"

#include "adjList.h"
#include "NeuHashtable.h"


void helper();
void process_command_line_args(int argc, char *argv[], char *folder_path_edges,char *folder_path_nodes);
void shortest_path_menu(char *folder_path_edges, char *folder_path_nodes);
int main_program(int argc, char *argv[]);
void get_next_command(NeuHashtable* hashtable, AdjListGraph* graph);

#endif