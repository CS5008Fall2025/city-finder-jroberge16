#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debug.h"



void helper(char *folder_path_edges, char *folder_path_nodes);
void process_command_line_args(int argc, char *argv[], char *folder_path_edges,char *folder_path_nodes);
void shortest_path_menu(char *folder_path_edges, char *folder_path_nodes);

#endif