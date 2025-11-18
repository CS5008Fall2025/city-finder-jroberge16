
/** 
 * Starter Code for Dijkstra's Shortest Path Algorithm Code Along
**/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "dijkstra.h"
#include "NeuHashtable.h"


/// Supporting Heap Data Structures

typedef struct {
  int data; // Vertex index
  int dist; // Distance from source
  char name[10];
} NeuHeapNode;

typedef struct {
  int size;            // Current size of the heap
  int capacity;        // Maximum capacity of the heap
  int *heap_position;            // Position in heap
  NeuHeapNode **array; // Array of heap nodes
} NeuHeap;

/**
 * Creates a new min heap with the given capacity.
 * @param capacity The initial capacity of the heap.
 * @return A pointer to the created min heap.
 */
NeuHeap *__createHeap(int capacity) {
  NeuHeap *minHeap = (NeuHeap *)malloc(sizeof(NeuHeap));
  minHeap->size = 0;
  minHeap->capacity = capacity; // Initial capacity
  minHeap->array =
      (NeuHeapNode **)malloc(minHeap->capacity * sizeof(NeuHeapNode *));
  minHeap->heap_position = (int *)malloc(capacity * sizeof(int));
  return minHeap;
}

/**
 * Frees the memory allocated for the min heap.
 * @param minHeap The min heap to be freed.
 */
void __freeHeap(NeuHeap *minHeap) {
  for (int i = 0; i < minHeap->size; i++) {
    free(minHeap->array[i]);
  }
  free(minHeap->array);
  free(minHeap->heap_position);
  free(minHeap);
}

/**
 * Swaps two nodes in the min heap.
 * @param a Pointer to the first node.
 * @param b Pointer to the second node.
 */
void __heapSwap(NeuHeapNode **a, NeuHeapNode **b) {
  NeuHeapNode *temp = *a;
  *a = *b;
  *b = temp;
}

/**
 * Helper functions for heap operations.
 */
int __heapParent(int i) { return (i - 1) / 2; }
int __heapLeft(int i) { return (2 * i + 1); }
int __heapRight(int i) { return (2 * i + 2); }
bool __heapIsEmpty(NeuHeap *minHeap) { return minHeap->size == 0; }

/**
 * Swaps two nodes in the min heap and updates position tracking.
 * @param minHeap The min heap.
 * @param a Index of first node.
 * @param b Index of second node.
 */
void __heapSwapWithPos(NeuHeap *minHeap, int a, int b) {
  // Update position array
  minHeap->heap_position[minHeap->array[a]->data] = b;
  minHeap->heap_position[minHeap->array[b]->data] = a;
  
  // Swap the nodes
  __heapSwap(&minHeap->array[a], &minHeap->array[b]);
}

/**
 * Decreases the key (distance) of a node in the min heap.
 * @param minHeap The min heap to update.
 * @param node The node to update.
 * @param newDist The new distance value.
 */
void __heapDecreaseKey(NeuHeap *minHeap, NeuHeapNode *node, int newDist) {
  node->dist = newDist;
  int i = minHeap->heap_position[node->data];  // Get actual position in heap
  while (i != 0 &&
         minHeap->array[__heapParent(i)]->dist > minHeap->array[i]->dist) {
    __heapSwapWithPos(minHeap, i, __heapParent(i));
    i = __heapParent(i);
  }
}

/**
 * Inserts a new node into the min heap.
 * @param minHeap The min heap to insert into.
 * @param data The vertex index.
 * @param dist The distance from the source.
 * @return A pointer to the newly created node.
 */
NeuHeapNode *__heapInsert(NeuHeap *minHeap, int data, int dist) {
  NeuHeapNode *newNode = (NeuHeapNode *)malloc(sizeof(NeuHeapNode));
  newNode->data = data;
  newNode->dist = dist;
  minHeap->array[minHeap->size] = newNode;
  minHeap->heap_position[data] = minHeap->size;
  minHeap->size++;
  __heapDecreaseKey(minHeap, newNode, dist);
  
  return newNode;
}

/**
 * Heapifies a subtree rooted at index i.
 * @param minHeap The min heap to heapify.
 * @param i The index of the root of the subtree.
 */
void __heapify(NeuHeap *minHeap, int i) {
  int smallest = i;
  int left = __heapLeft(i);
  int right = __heapRight(i);

  // Check if left child is smaller than root
  if (left < minHeap->size &&
      minHeap->array[left]->dist < minHeap->array[smallest]->dist) {
    smallest = left;
  }

  // Check if right child is smaller than smallest so far
  if (right < minHeap->size &&
      minHeap->array[right]->dist < minHeap->array[smallest]->dist) {
    smallest = right;
  }

  // If smallest is not root
  if (smallest != i) {
    // Swap the nodes
    __heapSwapWithPos(minHeap, i, smallest);
    // Recursively heapify the affected sub-tree
    __heapify(minHeap, smallest);
  }
}

/**
 * Extracts the minimum node from the min heap.
 * @param minHeap The min heap to extract from.
 * @return The extracted node.
 */
NeuHeapNode *__heapExtractMin(NeuHeap *minHeap) {
  if (minHeap->size == 0) {
    return NULL;
  }
  NeuHeapNode *root = minHeap->array[0];
  if (minHeap->size > 1) {
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    minHeap->heap_position[minHeap->array[0]->data] = 0;
    minHeap->size--;
    __heapify(minHeap, 0);
  } else {
    minHeap->size--;
  }
  return root;
}

///////   Dijkstra's Algorithm Implementation   //////////
/**
 * Implements Dijkstra's algorithm to find the shortest path from a source
 * vertex to all other vertices.
 * @param graph The adjacency list representation of the graph.
 * @param src The source vertex.
 * @param dist Output array to store the shortest distance from source to each
 * vertex.
 * @param prev Output array to store the previous node in the optimal path.
 */
void dijkstra(AdjListGraph *graph, int src, int *dist, int *prev) {
  
  for(int i=0; i < graph->numVertices;i++){
    dist[i] = INT_MAX;
    prev[i] = -1;

  }

  dist[src] = 0;

  NeuHeap* minHeap = __createHeap(graph->numVertices);
  NeuHeapNode *nodes[graph->numVertices];

  for(int i=0; i < graph->numVertices; i++){
    nodes[i]  =__heapInsert(minHeap, i, dist[i]);
  }

  while(!__heapIsEmpty(minHeap)){
    NeuHeapNode *minNode = __heapExtractMin(minHeap);
    int u = minNode->data;
    
    if(dist[u]==INT_MAX){
      free(minNode);
      break;
    }
    
    AdjListNode *curr = graph->adjList[u];
    while(curr!=NULL){
      int v = curr->vertex;
      int weight = curr->weight;

      if(dist[u] != INT_MAX && dist[u] + weight< dist[v]){
        dist[v] = dist[u] + weight;
        prev[v] = u;
        __heapDecreaseKey(minHeap, nodes[v], dist[v]);
      }
      curr = curr->next;

    }
  }

  __freeHeap(minHeap);
}



/**
 * Prints the shortest path from source to a given vertex.
 * @param src The source vertex.
 * @param dest The destination vertex.
 * @param prev Array containing the previous node information.
 * @param V The maximum number of vertices in the graph.
 */
void printPath(int dest, int *prev, AdjListGraph *graph) {
  // Create a temporary array to store the path
  int V = graph->numVertices;
  int path[V]; 
  int pathLength = 0;


  // First, collect the path in reverse order
  for (int v = dest; v != -1; v = prev[v]) {
    path[pathLength++] = v;
  }

  // Then print the path in correct order (source to destination)
  for (int i = pathLength - 1; i >= 0; i--) {
    char* city_name = graph->vertexIndex2Name[path[i]];
    printf("%s", city_name);
    if (i != 0) {
      printf("\n");
    }
  }
}

/**
 * Prints the distance array showing shortest distances from source to all
 * vertices.
 * @param dist Array containing shortest distances.
 * @param prev Array containing previous nodes in the optimal path.
 * @param V The number of vertices.
 */
void printAllSolutions(int *dist, int *prev, AdjListGraph *graph) {

  int V = graph->numVertices;

  printf("Shortest Path from Source to Destination:\n");
  for (int i = 0; i < V; i++) {
    if (dist[i] != INT_MAX) {
      printf("Shortest path to vertex [%s,%d] is %d with path: ", graph->vertexIndex2Name[i], i, dist[i]);
      printPath(i, prev, graph);
      printf("\n");
    } else {
      printf("Vertex [%s,%d] is unreachable from source\n", graph->vertexIndex2Name[i], i);
    }
  }
  printf("\n");
}


/**
 * Prints a single solution for dijkstra's algorithm.
 * @param dist Array containing shortest distances.
 * @param prev Array containing previous nodes in the optimal path.
 * @param V The number of vertices.
 */
void printTheShortestPath(char* src, char* dest, int *dist, int *prev, AdjListGraph *graph) {

  int dest_index = get_item(graph->nodeName2Index, dest)->vertextIndex;
  int src_index = get_item(graph->nodeName2Index, src)->vertextIndex;
  
  if (dist[dest_index] != INT_MAX && dist[src_index] != INT_MAX && prev[dest_index] != -1) {
        printf("Path Found...\n");
        printPath(dest_index, prev, graph);
        printf("\n");
        printf("Total Distance: %d", dist[dest_index]);
  }
  else {
      printf("Invalid Command");
  }
}


