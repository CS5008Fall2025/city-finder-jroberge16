#ifndef NEU_HASHTABLE_H
#define NEU_HASHTABLE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCALE_FACTOR 2
#define LOAD_FACTOR 0.7
#define INITIAL_CAPACITY 8

typedef struct {
    char vertextID[255];
    int vertextIndex;
} Item;

typedef struct NeuNode {
    Item data;
    struct NeuNode* next;
} NeuNode;

typedef struct {
    NeuNode** table;
    size_t size;
    size_t capacity;
} NeuHashtable;


NeuHashtable* create_hashtable(int capacity);
void free_hashtable(NeuHashtable* hashtable);
void add_item(NeuHashtable* hashtable, const char* vertextID, int vertextIndex);
Item* get_item(NeuHashtable* hashtable, const char* vertextID);
void remove_item(NeuHashtable* hashtable, const char* vertextID);
void print_hashtable(NeuHashtable* hashtable);
void print_table_visual(NeuHashtable* hashtable);
double get_load_factor(NeuHashtable* hashtable);
void print_keys(NeuHashtable *hashtable);



#endif
