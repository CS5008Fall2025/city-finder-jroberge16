/* debug.h */
#ifndef DEBUG_H
#define DEBUG_H

extern int debug_level; /* Declaration of the global variable */

/* Define debug macros */
#define DEBUG_NONE  0
#define DEBUG_ERROR 1
#define DEBUG_WARN  2
#define DEBUG_INFO  3
#define DEBUG_ALL   4

/* Debug print macro */
#ifdef NDEBUG
    #define DEBUG_PRINT(level, fmt, ...) ((void)0)
#else
    #define DEBUG_PRINT(level, fmt, ...) \
        do { \
            if (debug_level >= level) { \
                fprintf(stderr, "[DEBUG] %s:%d:%s(): " fmt, \
                        __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
            } \
        } while (0)
#endif


void set_debug_level(int level);

#endif /* DEBUG_H */