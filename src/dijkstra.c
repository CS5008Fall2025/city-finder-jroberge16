
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
  // asign array to heap
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
  // free each node
  for (int i = 0; i < minHeap->size; i++) {
    free(minHeap->array[i]);
  }
  // free remaining ptrs
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
  // update distance value
  node->dist = newDist;
  int i = minHeap->heap_position[node->data];  // Get actual position in heap

  // we bubble up the node inorder to maintain heap, smaller goes value goes up
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
  // add new node
  NeuHeapNode *newNode = (NeuHeapNode *)malloc(sizeof(NeuHeapNode));
  // meta data up date
  newNode->data = data;
  newNode->dist = dist;
  minHeap->array[minHeap->size] = newNode;
  minHeap->heap_position[data] = minHeap->size;
  minHeap->size++;
  // bubble up changes
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
  // grab top node ==min
  NeuHeapNode *root = minHeap->array[0];
  // heapify to find replacement for root
  if (minHeap->size > 1) {
    // move last to root
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    // update position of new root
    minHeap->heap_position[minHeap->array[0]->data] = 0;
    minHeap->size--;
    __heapify(minHeap, 0);// adjust heap
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
  // un visted get assigned to inf == INT_MAX
  for(int i=0; i < graph->numVertices;i++){
    dist[i] = INT_MAX;
    prev[i] = -1; // src is negative one, array allows us to trace shortest path back
  }
  // src has dist of 0
  dist[src] = 0;
  
  // create min heap/priority queue top value is shortest distance (greedy)
  NeuHeap* minHeap = __createHeap(graph->numVertices);
  NeuHeapNode *nodes[graph->numVertices];

  // add all vertex to min heap
  for(int i=0; i < graph->numVertices; i++){
    nodes[i]  =__heapInsert(minHeap, i, dist[i]);
  }
  // itterate through heap until empty
  while(!__heapIsEmpty(minHeap)){

    // grabbing smallest distance
    NeuHeapNode *minNode = __heapExtractMin(minHeap);
    int u = minNode->data;
    
    // means node is disconnected from the rest of the graph 
    if(dist[u]==INT_MAX){
      free(minNode);
      break;
    }
    // explore all the u's neighbors and update distances if shorter path is found
    AdjListNode *curr = graph->adjList[u];
    while(curr!=NULL){
      int v = curr->vertex;
      int weight = curr->weight;
      // new distance found updates path
      if(dist[u] != INT_MAX && dist[u] + weight< dist[v]){
        dist[v] = dist[u] + weight;
        prev[v] = u;
        // update queue with new distance
        __heapDecreaseKey(minHeap, nodes[v], dist[v]);
      }
      curr = curr->next;// move to next neighbor

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
    // uses aray index to get proper name
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
  // loop though all vertices and print 
  for (int i = 0; i < V; i++) {
    if (dist[i] != INT_MAX) {
      // print shortest path name with proper name and index name
      printf("Shortest path to vertex [%s,%d] is %d with path: ", graph->vertexIndex2Name[i], i, dist[i]);
      printPath(i, prev, graph);
      printf("\n");
    } else {
      // no path found
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
  // convert proper name to index position
  int dest_index = get_item(graph->nodeName2Index, dest)->vertextIndex;
  int src_index = get_item(graph->nodeName2Index, src)->vertextIndex;
  // print path shortsets paths
  if (dist[dest_index] != INT_MAX && dist[src_index] != INT_MAX && prev[dest_index] != -1) {
        printf("Path Found...\n");
        printPath(dest_index, prev, graph);// prints the path
        printf("\n");
        printf("Total Distance: %d", dist[dest_index]);// prints distance
  }
  else {
      printf("Invalid Command");
  }
}


