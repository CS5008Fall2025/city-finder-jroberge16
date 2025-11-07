#ifndef GRAPH_READER_H
#define GRAPH_READER_H

/* 
 * GraphReader.h
 * Header file for reading graph data from files
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Structure to hold the file pointer and current line data
typedef struct GraphReader {
    FILE* file;
    int* currentLine;
} GraphReader;


GraphReader* reader_open(const char* filename);
int* reader_next(GraphReader* reader);
void reader_close(GraphReader* reader);

#endif /* GRAPH_READER_H */