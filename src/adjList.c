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
AdjListGraph* createGraph(NeuHashtable* name2Index, bool directed) {

    DEBUG_PRINT(DEBUG_INFO, "Creating graph with capacity %zu\n", name2Index->size);
    // putting graph on heap
    AdjListGraph* graph = (AdjListGraph*)malloc(sizeof(AdjListGraph));
    //null check
    if (graph == NULL) {
        fprintf(stderr, "Memory allocation failed for graph.\n");
        exit(EXIT_FAILURE);
    }
    
    // assign index value to graph
    graph->numVertices = name2Index->size;
    graph->capacity = name2Index->size; 
    graph->directed = directed; // true = directed
    graph->nodeName2Index = name2Index; // hashtable lu converts node char to id

    // Allocate memory to adjacency list
    graph->adjList = (AdjListNode**)malloc(name2Index->size * sizeof(AdjListNode*));
    
    // maps id 2 name
    graph->vertexIndex2Name = (char**)malloc(name2Index->size * sizeof(char*));

    // Null check
    if (graph->adjList == NULL || graph->vertexIndex2Name == NULL) {
        fprintf(stderr, "Memory allocation failed for adjacency list.\n");
        free(graph);
        exit(EXIT_FAILURE);
    }

    // populate vertexIndex2Name array index = id which maps to name
    for (int i = 0; i < name2Index->capacity; i++) {
        NeuNode* current = name2Index->table[i];
        while (current != NULL) {
            int index = current->data.vertextIndex;
            DEBUG_PRINT(DEBUG_INFO, "Connecting index %d to proper name %s\n", index, current->data.vertextID);
            graph->vertexIndex2Name[index] = strdup(current->data.vertextID);
            current = current->next;
        }
    }
    
    // init adjacency list to NULL
    for (int i = 0; i < name2Index->size; i++) {
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
    // ittterate through grpah freeing each ndoe
    for (int i = 0; i < graph->numVertices; i++) {
        AdjListNode* current = graph->adjList[i];
        while (current != NULL) {
            AdjListNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    // Free the remaining items
    free(graph->adjList);
    free(graph);
}

/**
 * Doubles the capacity of the graph's adjacency list.
 * @param graph A pointer to the AdjListGraph.
 */
void __resizeGraph(AdjListGraph* graph) {
    
    // increases graph by scaling factor
    DEBUG_PRINT(DEBUG_INFO, "Resizing graph from %d to %d\n", graph->capacity, graph->capacity * SCALE_FACTOR);
    int newCapacity = graph->capacity * SCALE_FACTOR;

    // loop through all items in old graph and re-assign to new graph
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
void addEdge(AdjListGraph* graph, char* src_name, char* dest_name, int weight) {

    DEBUG_PRINT(DEBUG_INFO, "Making/Add edge from %s to %s with weight %d\n", src_name, dest_name, weight);
    // we get char string for a src name and we use a hasmap to aquire index/id
    int src = get_item(graph->nodeName2Index, src_name)->vertextIndex;
    int dest = get_item(graph->nodeName2Index, dest_name)->vertextIndex;
    
    // checks for valid index
    if (src < 0  ||  dest < 0 ) {
        DEBUG_PRINT(DEBUG_ERROR, "Vertex index out of bounds.");
        return;
    }

    // if the number of nodes is greater than capacity then we have to increase the grpah
    graph->numVertices = (src >= graph->numVertices) ? src + 1 : graph->numVertices;
    graph->numVertices = (dest >= graph->numVertices) ? dest + 1 : graph->numVertices;
    if (graph->numVertices >= graph->capacity) {
        __resizeGraph(graph);
    }

    // add memory for new node
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    if (newNode == NULL) {
        DEBUG_PRINT(DEBUG_ERROR, "Memory allocation failed for new node.\n");
        fprintf(stderr, "Memory allocation failed for new node.\n");
        exit(EXIT_FAILURE);
    }
    // add meta dat and ptr to node
    newNode->vertex = dest;
    newNode->weight = weight;
    strcpy(newNode->name, dest_name); // add name for reverse lu
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;

    // if the graph is undirected we add the reverse edge
    if (!graph->directed) {
        // Add the edge in the opposite direction for undirected graphs
        AdjListNode* reverseNode = (AdjListNode*)malloc(sizeof(AdjListNode));
        if (reverseNode == NULL) {
            fprintf(stderr, "Memory allocation failed for reverse node.\n");
            exit(EXIT_FAILURE);
        }
        reverseNode->vertex = src;
        reverseNode->weight = weight;
        strcpy(reverseNode->name, src_name);
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
    // iterate through adjacency and count neighbors
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
    // get number of neighbors
    int degree = getDegree(graph, vertex);
    // assign array to hold neighbors
    int* neighbors = (int*)malloc(degree * sizeof(int));
    if (neighbors == NULL) {
        fprintf(stderr, "Memory allocation failed for neighbors array.\n");
        return NULL;
    }
    // iterate through adjacency and add neighbors to array
    AdjListNode* current = graph->adjList[vertex];
    for (int i = 0; i < degree; i++) {
        if (current != NULL) {
            neighbors[i] = current->vertex;
            current = current->next;
        }
    }

    // return array of neighbors
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
    // itterate trhough and find src to dest edge
    AdjListNode* current = graph->adjList[src]; // start at src
    while (current != NULL) {
        if (current->vertex == dest) {
            return current->weight; // found edge get weight
        }
        current = current->next; // move to the next node
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
        // print vertex proper name with index
        printf("[%s,%d]: ", graph->vertexIndex2Name[i], i);
        printf("[");
        AdjListNode* current = graph->adjList[i];
        while (current != NULL) {
            // print neighbore info with proper name, index id and weight
            printf("(%s, %d, %d)", current->name, current->vertex, current->weight);
            current = current->next;
            if (current != NULL) {
                printf(", ");
            }
        }
        printf("]\n");
    }
}

/**
 * Loads graph and adds in edges connections
 * @param graph A pointer to the AdjListGraph.
 * @param filename The name of the file to load edges from.
 */
void loadFromFile(AdjListGraph* graph, const char* filename) {
    // substantiate file pointer and line buffer
    FILE *file = fopen(filename, "r");
    char line[250];
    char src[100];
    char dest[100];

    int value;
    // null check file
    if (file == NULL){
        printf("❌ Invalid File");
        perror("Error with file");
        return;
    }
    // loop through file and build out graph
    while(fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0';
        if (sscanf(line, "%s %s %d", 
                src,
                dest,
                &value) == 3) {
            addEdge(graph, src, dest, value);
        }   
         
    }
    // close file when done
    fclose(file);
}

/**
 * prints adj list and index to proper name mapping
 * @param graph A pointer to the adj list.
 */
void print_array(AdjListGraph* graph){
    for(int i = 0; i < graph->numVertices; i++) {
        printf("Index %d: %s\n", i, graph->vertexIndex2Name[i]);
    }
}