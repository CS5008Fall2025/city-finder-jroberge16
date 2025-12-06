
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

/**
 * Creates a new adjacency list graph with the given capacity.
 * @param capacity The initial capacity (number of vertices) of the graph.
 * @param directed Whether the graph is directed (true) or undirected (false).
 * @return A pointer to the newly created AdjListGraph.
 */
AdjListGraph* createGraph(NeuHashtable* name2Index, bool directed);


/**
 * Frees the memory allocated for the adjacency list graph.
 * @param graph A pointer to the AdjListGraph to free.
 */
void freeGraph(AdjListGraph* graph);

/**
 * Gets the degree (number of neighbors) of a vertex.
 * @param graph A pointer to the AdjListGraph.
 * @param vertex The vertex to get the degree of.
 * @return The degree of the vertex.
 */
int getDegree(AdjListGraph* graph, int vertex);

/**
 * Gets the neighbors of a vertex.
 * @param graph A pointer to the AdjListGraph.
 * @param vertex The vertex to get the neighbors of.
 * @return An array of neighbor vertex indices. The caller is responsible for freeing this array.
 */
int* getNeighbors(AdjListGraph* graph, int vertex);

/**
 * Gets the weight of the edge from src to dest.
 * @param graph A pointer to the AdjListGraph.
 * @param src The source vertex.
 * @param dest The destination vertex.
 * @return The weight of the edge, or 0 if no edge exists.
 */
int getWeight(AdjListGraph* graph, int src, int dest);

/**
 * Prints the adjacency list of the graph.
 * @param graph A pointer to the AdjListGraph.
 */
void printGraph(AdjListGraph* graph);

/**
 * Adds an edge to the graph.
 * @param graph A pointer to the AdjListGraph.
 * @param src The source vertex name.
 * @param dest The destination vertex name.
 * @param weight The weight of the edge.
 */
void addEdge(AdjListGraph* graph, char* src, char* dest, int weight);

/**
 * Loads graph and adds in edges connections
 * @param graph A pointer to the AdjListGraph.
 * @param filename The name of the file to load edges from.
 */
void loadFromFile(AdjListGraph* graph, const char* filename);

/**
 * prints adj list and index to proper name mapping
 * @param graph A pointer to the adj list.
 */
void print_array(AdjListGraph* graph);







#endif /* ADJ_LIST_H */