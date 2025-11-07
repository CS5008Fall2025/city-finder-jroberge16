/**
* Graph Reader - helps read in files of the format
* <source> <destination> <weight>
* Each time a line is read, it returns an array of 3 integers
* representing the source, destination, and weight.

* The GraphReader structure contains a file pointer and a buffer for the current line.
* It handles its own memory management, freeing the previous line when a new one is read.
*
* Example usage:
* GraphReader* reader = reader_open("graph.txt");
* if (reader != NULL) {
*     int* line;
*     while ((line = reader_next(reader)) != NULL) {
*         printf("Source: %d, Destination: %d, Weight: %d\n", line[0], line[1], line[2]);
*     }
*     reader_close(reader);
* }
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "NeuHashtable.h"
#include "GraphReader.h"


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
GraphReader* reader_open(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }
    
    GraphReader* reader = (GraphReader*)malloc(sizeof(GraphReader));
    if (reader == NULL) {
        fclose(file);
        return NULL;
    }
    
    reader->file = file;
    reader->currentLine = NULL;
    
    return reader;
}

/**
* reader_next - reads the next line from the graph file
* @param reader - a pointer to the GraphReader structure
* @return - an array of 3 integers representing the source, destination, and weight,
*           or NULL if end of file or error
**/
int* reader_next(GraphReader* reader) {
    if (reader == NULL || reader->file == NULL) {
        return NULL;
    }
    
    // Free previous line if exists
    if (reader->currentLine != NULL) {
        free(reader->currentLine);
        reader->currentLine = NULL;
    }
    
    // Allocate memory for the new line (3 integers)
    reader->currentLine = (int*)malloc(3 * sizeof(int));
    if (reader->currentLine == NULL) {
        return NULL;
    }
    
    // Read the next line with 3 integers
    if (fscanf(reader->file, "%d %d %d", 
               &reader->currentLine[0],
               &reader->currentLine[1],
               &reader->currentLine[2]) != 3) {
        // If we couldn't read 3 integers, clean up and return NULL
        free(reader->currentLine);
        reader->currentLine = NULL;
        return NULL;
    }
    
    return reader->currentLine;
}








/**
* reader_close - closes the GraphReader and frees allocated memory
* This function should be called when done with the GraphReader
* to avoid memory leaks.
* It closes the file and frees the memory allocated for the current line.
* @param reader - a pointer to the GraphReader structure
**/
void reader_close(GraphReader* reader) {
    if (reader != NULL) {
        if (reader->file != NULL) {
            fclose(reader->file);
        }
        if (reader->currentLine != NULL) {
            free(reader->currentLine);
        }
        free(reader);
    }
}


NeuHashtable* read_vertices(const char* file_name){
    FILE *file;
    char line[256]; 
    int index = 0;

    NeuHashtable* hashtable = create_hashtable(25);

    file = fopen(file_name, "r");
    if (file == NULL){
        printf("❌ Invalid File");
        return NULL;
    }
    
    while(fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0';
        
        if (strlen(line) == 0) {
            continue;
        }
        add_item(hashtable, line, index);
        index++;
    }
    fclose(file);
    return hashtable;
}