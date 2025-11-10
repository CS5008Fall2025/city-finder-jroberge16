
#ifndef ADJ_LIST_H
#define ADJ_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "NeuHashtable.h"


typedef struct node_t {
    int vertex;
    int weight;
    char name[25];
    struct node_t* next;
} AdjListNode;

typedef struct {
    int numVertices;
    int capacity;
    bool directed;
    AdjListNode** adjList;
    NeuHashtable* nodeName2Index;
    char **vertexIndex2Name;
} AdjListGraph;

#define SCALE_FACTOR 2


AdjListGraph* createGraph(NeuHashtable* name2Index, bool directed); 
void freeGraph(AdjListGraph* graph);

int getDegree(AdjListGraph* graph, int vertex);
int* getNeighbors(AdjListGraph* graph, int vertex);
int getWeight(AdjListGraph* graph, int src, int dest);
void printGraph(AdjListGraph* graph);
void addEdge(AdjListGraph* graph, char* src, char* dest, int weight);
void loadFromFile(AdjListGraph* graph, const char* filename);




#endif /* ADJ_LIST_H */