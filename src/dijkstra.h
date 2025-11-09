/**
 * @file dijkstra.h
 * @brief Implementation of Dijkstra's algorithm for finding shortest paths in a graph
 */

#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "adjList.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

/**
 * @brief Implements Dijkstra's algorithm to find the shortest path from a source vertex to all other vertices
 *
 * @param graph The adjacency list representation of the graph
 * @param src The source vertex
 * @param dist Output array to store the shortest distance from source to each vertex
 * @param prev Output array to store the previous node in the optimal path
 */
void dijkstra(AdjListGraph* graph, int src, int* dist, int* prev);

/**
 * @brief Prints the shortest path from source to a given vertex
 *
 * @param dest The destination vertex
 * @param prev Array containing the previous node information
* @param V The maximum number of vertices in the graph.
 */
void printPath(int dest, int *prev, AdjListGraph *graph);

/**
 * @brief Prints the distance array showing shortest distances from source to all vertices
 *
 * @param dist Array containing shortest distances
 * @param prev Array containing previous nodes in the optimal path.
 * @param V The number of vertices
 */
void printAllSolutions(int *dist, int *prev, AdjListGraph *graph) ;


void printTheShortestPath(char* src, char* dest, int *dist, int *prev, AdjListGraph *graph);

#endif /* DIJKSTRA_H */