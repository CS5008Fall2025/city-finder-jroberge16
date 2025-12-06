#include <stdio.h>

#include "debug.h"


int debug_level = DEBUG_NONE; /* Definition of the global variable */

/**
 * set debug leve for program
 * @param level debug (0-4)
 */
void set_debug_level(int level) {

    if (level >= DEBUG_NONE && level <= DEBUG_ALL) {
        debug_level = level;
    } else {
        fprintf(stderr, "Invalid debug level. Use 0-4.\n");
    }
}