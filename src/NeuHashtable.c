/**
*  Solution file for code along hashtable.
*
*  @author Albert Lionelle
*  @date 2025-05-15
**/


#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "NeuHashtable.h"

/**
* private functions to creat table 
*/
NeuNode**  __node_create_table(int capacity) {
    NeuNode** table = (NeuNode**)malloc(capacity * sizeof(NeuNode*));
    // memory check
    if (table == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    // initializes entries to NULL
    for (int i = 0; i < capacity; i++) {
        table[i] = NULL;
    }
    return table;
}

/**
 * Creates a new hashtable with the given capacity.
 * For the capacity, it will find the nearest power of two greater than or equal to the given capacity.
 *
 * @param capacity The initial capacity of the hashtable.
 * @return A pointer to the newly created hashtable.
 */
NeuHashtable* create_hashtable(int capacity) {
    // first find the nearest power of two greater than or equal to capacity
    int new_capacity = 1;
    while (new_capacity < capacity) {
        new_capacity <<= 1; // multiply by 2
    }

    // allocate memory for the hashtable
    NeuHashtable* hashtable = (NeuHashtable*)malloc(sizeof(NeuHashtable));
    if (hashtable == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    // assign initial meta data
    hashtable->capacity = new_capacity;
    hashtable->size = 0;
    // create table
    hashtable->table = __node_create_table(new_capacity);
    return hashtable;
}

/**
 * Frees the memory allocated for the hashtable.
 * @param hashtable A pointer to the hashtable to free.
 */
void free_hashtable(NeuHashtable* hashtable) {
    if (hashtable != NULL) {
        // iterate through table and then free linked list nodes
        for (int i = 0; i < hashtable->capacity; i++) {
            NeuNode* current = hashtable->table[i];
            while (current != NULL) {
                NeuNode* temp = current;
                current = current->next;
                free(temp);
            }
        }
        free(hashtable->table);
        free(hashtable);
    }
}
/**
 * hash functions for hashing values
 * @param key The key to hash.
 * @return The hashed value.
*/
size_t __djb2_hash_function(const char* key) {
    size_t hash = 5381;
    int c;

    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash;
}
/**
 * computes index value for hastable
 * @param vertextID The key to hash.
 * @param capacity The capacity of the hashtable.
 */
size_t __get_index(const char* vertextID, size_t capacity) {
    return __djb2_hash_function(vertextID) & (capacity-1); // faster than %
}


/**
 * creates a new node for table
 * @param vertextID The ID of the item.
 * @param vertextIndex The name of the item.
 */
NeuNode * __create_node(const char* vertextID, int vertextIndex) {
    NeuNode* newNode = (NeuNode*)malloc(sizeof(NeuNode));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->data.vertextID, vertextID);
    newNode->data.vertextIndex = vertextIndex;
    newNode->next = NULL;
    return newNode;
}

/**
 * dynamically increases hastable size by doubling capacity
 * triggered by loadfactor
 * @param hashtable A pointer to the hashtable.
 */
void __double_capacity(NeuHashtable * hashtable) {
    int new_capacity = hashtable->capacity * SCALE_FACTOR;
    // make new table
    NeuNode** new_table = __node_create_table(new_capacity);

    // loop trhough old table re-hash items and insert into new table
    for (int i = 0; i < hashtable->capacity; i++) {
        NeuNode* current = hashtable->table[i];
        // while is for itams that are stacked due to collisions
        while (current != NULL) {
            size_t hash_index = __get_index(current->data.vertextID, new_capacity);
            NeuNode* next_node = current->next;

            current->next = new_table[hash_index];
            new_table[hash_index] = current;

            current = next_node;
        }
    }
    // free old table and re-assign new
    free(hashtable->table);
    hashtable->table = new_table;
    hashtable->capacity = new_capacity;    
}

/**
 * Adds an item to the hashtable.
 * @param hashtable A pointer to the hashtable.
 * @param itemID The ID of the item.
 * @param vertextIndex The name of the item.
 */
void add_item(NeuHashtable* hashtable, const char* vertextID, int vertextIndex) {
    // ensure vertex is unique
    if (get_item(hashtable, vertextID) != NULL) {
        fprintf(stderr, "Item with ID %s already exists\n", vertextID);
        return;
    }
    
    // Check if the hashtable needs to be resized
    if (get_load_factor(hashtable) > LOAD_FACTOR) {
        __double_capacity(hashtable);
    }
    // hashes value and creates node
    size_t hash_index = __get_index(vertextID, hashtable->capacity);
    NeuNode* newNode = __create_node(vertextID, vertextIndex);
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    // allocates node to table
    newNode->next = hashtable->table[hash_index];
    hashtable->table[hash_index] = newNode;
    hashtable->size++;
}

/**
 * Gets an item from the hashtable by its ID.
 * @param hashtable A pointer to the hashtable.
 * @param itemID The ID of the item to retrieve.
 * @return A pointer to the item if found, or NULL if not found.
 */
Item* get_item(NeuHashtable* hashtable, const char* vertextID) {
    // hash get request
    size_t hash_index = __get_index(vertextID, hashtable->capacity);
    // get lsit of nodes at index
    NeuNode* current = hashtable->table[hash_index];
    // loop through until we find what we are looking for
    while (current != NULL) {
        if (strcmp(current->data.vertextID, vertextID) == 0) {
            return &current->data;
        }
        current = current->next;
    }
    // node not found
    return NULL;
}

/**
 * Gets the load factor of the hashtable.
 * @param hashtable A pointer to the hashtable.
 * @return The load factor of the hashtable.
 */
inline double get_load_factor(NeuHashtable* hashtable) {
    // calculates load factor
    return (double)hashtable->size / hashtable->capacity;
}

/**
 * Removes an item from the hashtable by its ID.
 * @param hashtable A pointer to the hashtable.
 * @param vertextID The ID of the item to remove.
 */
void remove_item(NeuHashtable* hashtable, const char* vertextID) {
    // get hash for index
    size_t hash_index = __get_index(vertextID, hashtable->capacity);
    // get list at index
    NeuNode* current = hashtable->table[hash_index];
    NeuNode* prev = NULL;
    // loop through until we find the item
    while (current != NULL) {
        // checks for match
        if (strcmp(current->data.vertextID, vertextID) == 0) {
            // remove and rearrange ptrs if needed
            if (prev == NULL) {
                hashtable->table[hash_index] = current->next;
            } else {
                prev->next = current->next;
            }
            // free and reduce size
            free(current);
            hashtable->size--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

/**
 * Prints an item from the hastable.
 * @param item A pointer to the item to print.
 */
void __print_item(Item* item) {
    if (item != NULL) {
        // prints id and index of the item
        printf("item(ID: %s, index: %d)", 
            item->vertextID, item->vertextIndex);
    }
    else {
        printf("NULL");
    }
}

/**
 * Prints the contents of the hashtable.
 * Format is key:value seperated by commas.
 * Example: {key1:value1, key2:value2}
 * @param hashtable A pointer to the hashtable.
 */
void print_hashtable(NeuHashtable* hashtable) {
    printf("{");
    for (int i = 0; i < hashtable->capacity; i++) {
        NeuNode* current = hashtable->table[i];
        while (current != NULL) {
            // prints key and value items
            printf("%s:", current->data.vertextID);
            __print_item(&current->data);
            current = current->next;
            if (current != NULL) {
                printf(", ");
            }
        }
         if (i < hashtable->capacity - 1 && hashtable->table[i] != NULL) {
             printf(", ");
         }
    }
    printf("}\n");
}

/**
 * Prints the array with a count of number of items in 
 * each index of the hashtable. An example layout would be
 * [1, 0, 0, 0, 0, 0, 0, 1]
 * where the first index has 1 item and the last index has 1 item.
 * @param hashtable A pointer to the hashtable.
 */
void print_table_visual(NeuHashtable *hashtable) {
    printf("[");
    // loops through table and print the total items at each index
    for (int i = 0; i < hashtable->capacity; i++) {
        NeuNode* current = hashtable->table[i];
        int count = 0;
        while (current != NULL) {
            count++;
            current = current->next;
        }
        printf("%d", count);
        if (i < hashtable->capacity - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

/**
 * prints the keys of the hashtable
 * @param hashtable hashtable to print keys from
 */
void print_keys(NeuHashtable *hashtable){
    // loops through table and print keys of the table out
    for (int i = 0; i < hashtable->capacity; i++) {
        NeuNode* current = hashtable->table[i];
        while (current != NULL) {
            printf("\n%s", current->data.vertextID);
            current = current->next;
        }

    }
}