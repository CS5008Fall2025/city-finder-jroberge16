
#ifndef ADJ_LIST_H
#define ADJ_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "NeuHashtable.h"


typedef struct node_t {
    int vertex;
    int weight;
    struct node_t* next;
} AdjListNode;

typedef struct {
    int numVertices;
    int capacity;
    AdjListNode** adjList;
    bool directed;
} AdjListGraph;

#define SCALE_FACTOR 2


AdjListGraph* createGraph(int capacity, bool directed); 
void freeGraph(AdjListGraph* graph);

void addEdge(AdjListGraph* graph, int src, int dest, int weight);
int getDegree(AdjListGraph* graph, int vertex);
int* getNeighbors(AdjListGraph* graph, int vertex);
int getWeight(AdjListGraph* graph, int src, int dest);
void printGraph(AdjListGraph* graph);
void loadFromFile(AdjListGraph* graph, const char* filename,  NeuHashtable* hashtable);




#endif /* ADJ_LIST_H */