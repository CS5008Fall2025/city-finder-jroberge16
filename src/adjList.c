/**
* Solution Code for Adjacency List Representation of Graph Code Along
* @author Albert Lionelle
* @date 2025-05-20
**/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "adjList.h"
#include "debug.h"
#include "GraphReader.h"
#include "NeuHashtable.h"





/**
 * Creates a new adjacency list graph with the given capacity.
 * @param capacity The initial capacity (number of vertices) of the graph.
 * @param directed Whether the graph is directed (true) or undirected (false).
 * @return A pointer to the newly created AdjListGraph.
 */
AdjListGraph* createGraph(int capacity, bool directed) {
    DEBUG_PRINT(DEBUG_INFO, "Creating graph with capacity %d\n", capacity);
    AdjListGraph* graph = (AdjListGraph*)malloc(sizeof(AdjListGraph));
    if (graph == NULL) {
        fprintf(stderr, "Memory allocation failed for graph.\n");
        exit(EXIT_FAILURE);
    }
    graph->numVertices = 0;
    graph->capacity = capacity;
    graph->directed = directed;
    graph->adjList = (AdjListNode**)malloc(capacity * sizeof(AdjListNode*));
    if (graph->adjList == NULL) {
        fprintf(stderr, "Memory allocation failed for adjacency list.\n");
        free(graph);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < capacity; i++) {
        graph->adjList[i] = NULL;
    }
    return graph;
}

/**
 * Frees the memory allocated for the adjacency list graph.
 * @param graph A pointer to the AdjListGraph to free.
 */
void freeGraph(AdjListGraph* graph) {
    if (graph == NULL) {
        return;
    }
    for (int i = 0; i < graph->numVertices; i++) {
        AdjListNode* current = graph->adjList[i];
        while (current != NULL) {
            AdjListNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph->adjList);
    free(graph);
}

/**
 * Doubles the capacity of the graph's adjacency list.
 * @param graph A pointer to the AdjListGraph.
 */
void __resizeGraph(AdjListGraph* graph) {
     DEBUG_PRINT(DEBUG_INFO, "Resizing graph from %d to %d\n", graph->capacity, graph->capacity * SCALE_FACTOR);
    int newCapacity = graph->capacity * SCALE_FACTOR;
    AdjListNode** newAdjList = (AdjListNode**)malloc(newCapacity * sizeof(AdjListNode*));
    if (newAdjList == NULL) {
        fprintf(stderr, "Memory allocation failed while doubling capacity.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < newCapacity; i++) {
        newAdjList[i] = NULL;
    }
    for (int i = 0; i < graph->capacity; i++) {
        newAdjList[i] = graph->adjList[i];
    }
    free(graph->adjList);
    graph->adjList = newAdjList;
    graph->capacity = newCapacity;
}

/**
 * Adds an edge to the graph from src to dest with the given weight.
 * @param graph A pointer to the AdjListGraph.
 * @param src The source vertex.
 * @param dest The destination vertex.
 * @param weight The weight of the edge.
 */
void addEdge(AdjListGraph* graph, int src, int dest, int weight) {
    if (src < 0  ||  dest < 0 ) {
        fprintf(stderr, "Vertex index out of bounds.\n");
        return;
    }
    graph->numVertices = (src >= graph->numVertices) ? src + 1 : graph->numVertices;
    graph->numVertices = (dest >= graph->numVertices) ? dest + 1 : graph->numVertices;

    if (graph->numVertices >= graph->capacity) {
        __resizeGraph(graph);
    }
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed for new node.\n");
        exit(EXIT_FAILURE);
    }
    newNode->vertex = dest;
    newNode->weight = weight;
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;

    if (!graph->directed) {
        // Add the edge in the opposite direction for undirected graphs
        AdjListNode* reverseNode = (AdjListNode*)malloc(sizeof(AdjListNode));
        if (reverseNode == NULL) {
            fprintf(stderr, "Memory allocation failed for reverse node.\n");
            exit(EXIT_FAILURE);
        }
        reverseNode->vertex = src;
        reverseNode->weight = weight;
        reverseNode->next = graph->adjList[dest];
        graph->adjList[dest] = reverseNode;
    }
}

/**
 * Gets the degree (number of neighbors) of a vertex.
 * @param graph A pointer to the AdjListGraph.
 * @param vertex The vertex to get the degree of.
 * @return The degree of the vertex.
 */
int getDegree(AdjListGraph* graph, int vertex) {
    if (vertex < 0 || vertex >= graph->numVertices) {
        fprintf(stderr, "Vertex index out of bounds.\n");
        return -1;
    }
    int degree = 0;
    AdjListNode* current = graph->adjList[vertex];
    while (current != NULL) {
        degree++;
        current = current->next;
    }
    return degree;
}

/**
 * Gets the neighbors of a vertex.
 * @param graph A pointer to the AdjListGraph.
 * @param vertex The vertex to get neighbors for.
 * @return A pointer to an array of neighbor vertex indices (caller must free).
 */
int* getNeighbors(AdjListGraph* graph, int vertex) {
    if (vertex < 0 || vertex >= graph->numVertices) {
        fprintf(stderr, "Vertex index out of bounds.\n");
        return NULL;
    }
    int degree = getDegree(graph, vertex);
    int* neighbors = (int*)malloc(degree * sizeof(int));
    if (neighbors == NULL) {
        fprintf(stderr, "Memory allocation failed for neighbors array.\n");
        return NULL;
    }
    AdjListNode* current = graph->adjList[vertex];
    for (int i = 0; i < degree; i++) {
        if (current != NULL) {
            neighbors[i] = current->vertex;
            current = current->next;
        }
    }
    return neighbors;
}

/**
 * Gets the weight of the edge from src to dest.
 * @param graph A pointer to the AdjListGraph.
 * @param src The source vertex.
 * @param dest The destination vertex.
 * @return The weight of the edge, or 0 if no edge exists.
 */
int getWeight(AdjListGraph* graph, int src, int dest) {
    if (src < 0 || src >= graph->numVertices || dest < 0 || dest >= graph->numVertices) {
        fprintf(stderr, "Vertex index out of bounds.\n");
        return 0;
    }
    AdjListNode* current = graph->adjList[src];
    while (current != NULL) {
        if (current->vertex == dest) {
            return current->weight;
        }
        current = current->next;
    }
    return 0; // No edge exists
}

/**
 * Prints the adjacency list of the graph.
 * Prints in the format of "Vertex: [(neighbor1, weight1), (neighbor2, weight2), ...]"
 * @param graph A pointer to the AdjListGraph.
 */
void printGraph(AdjListGraph* graph) {
   for(int i = 0; i < graph->numVertices; i++) {
        printf("%d: ", i);
        printf("[");
        AdjListNode* current = graph->adjList[i];
        while (current != NULL) {
            printf("(%d, %d)", current->vertex, current->weight);
            current = current->next;
            if (current != NULL) {
                printf(", ");
            }
        }
        printf("]\n");
    }
}

/**
 * Loads a graph from a file.
 * @param graph A pointer to the AdjListGraph.
 * @param filename The name of the file to load from.
 */
// void loadFromFile(AdjListGraph* graph, const char* filename, NeuHashtable* hashtable) {
//     GraphReader* reader = reader_open(filename);
//     if (reader == NULL) {
//         fprintf(stderr, "Failed to open file: %s\n", filename);
//         return;
//     }

//     int* line;
//     while ((line = reader_next(reader)) != NULL) {
//         Item* src_item = get_item(hashtable, line[0]);
//         Item* dest_item = get_item(hashtable, line[1]);

//         if (src_item == NULL || dest_item == NULL) {
//             printf("❌ Vertex not found in hastable");
//             continue;
//         }
        
//         int src = src_item->vertextIndex;
//         int dest = dest_item->vertextIndex;

//         int weight = line[2];
//         DEBUG_PRINT(DEBUG_INFO, "Adding edge from %d to %d with weight %d\n", src, dest, weight);
//         addEdge(graph, src, dest, weight);
//     }
//     reader_close(reader);
// }



void loadFromFile(AdjListGraph* graph, const char* filename, NeuHashtable* hashtable) {
    FILE *file = fopen(filename, "r");

    char line[256];
    char src[10];
    char dest[10];
    int value;

    if (file == NULL){
        printf("❌ Invalid File");
        return;
    }

    while(fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0';
        if (sscanf(line, "%s %s %d", 
                src,
                dest,
                &value) == 3) {

            Item* src_item = get_item(hashtable, src);
            Item* dest_item = get_item(hashtable, dest);

            if (src_item!=NULL && dest_item!=NULL) {
                printf("addding edge from %s (%d) to %s (%d) with weight %d\n", src, src_item->vertextIndex, dest, dest_item->vertextIndex, value);
                addEdge(graph, src_item->vertextIndex, dest_item->vertextIndex, value);
                addEdge(graph, dest_item->vertextIndex, src_item->vertextIndex, value);
                continue;
            }else{
                
                printf("❌ Vertex not found in hastable\n");
                printf("adding edge from %s (%d) to %s (%d) with weight %d\n", src, src_item->vertextIndex, dest, dest_item->vertextIndex, value);

                continue;
            }
        }   
         
    }
    fclose(file);
}