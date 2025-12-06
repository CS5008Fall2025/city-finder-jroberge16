#ifndef GRAPH_READER_H
#define GRAPH_READER_H

/* 
 * GraphReader.h
 * Header file for reading graph data from files
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "NeuHashtable.h"



// Structure to hold the file pointer and current line data
typedef struct GraphReader {
    FILE* file;
    int* currentLine;
} GraphReader;

/**
* GraphReader - structure to hold the file pointer and current line data
* Opens a file for reading in graphe data. The file 
* should contain lines of the format:
* <source> <destination> <weight>
* Each time a line is read, it returns an array of 3 integers
* representing the source, destination, and weight.
* @param filename - the name of the file to read
* @return - a pointer to a GraphReader structure, or NULL if the file cannot be opened
**/
GraphReader* reader_open(const char* filename);

/**
* reader_next - reads the next line from the graph file
* @param reader - a pointer to the GraphReader structure
* @return - an array of 3 integers representing the source, destination, and weight,
*           or NULL if end of file or error
**/
int* reader_next(GraphReader* reader);

/**
* reader_close - closes the GraphReader and frees allocated memory
* This function should be called when done with the GraphReader
* to avoid memory leaks.
* It closes the file and frees the memory allocated for the current line.
* @param reader - a pointer to the GraphReader structure
**/
void reader_close(GraphReader* reader);


/**
 * Read vertex file and assigsn each vertext to a hashtable
 * map does the following: vertex name -> index position
 * @param file_name file path  
 */
NeuHashtable* read_vertices(const char* file_name);

#endif /* GRAPH_READER_H */