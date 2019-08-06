/* zmalloc - total amount of allocated memory aware version of malloc() */

#include<stdio.h>
#include<stdlib.h>

/* This function provide us access to the original libc free(). This is useful
 * for instance to free results obtained by backtrace_symbols(). We need
 * to define this function before including zmalloc.h that may shadow the 
 * free implementation if we use jemalloc or another non standard allocator. */
 // 总结；不要覆盖这些函数, 引入zmalloc就覆盖了
void zlibc_free(void *ptr) {
     free(ptr);
 }
#include <string.h>
#include <pthread.h>
#include "zmalloc.h"

#define PREFIX_SIZE (sizeof(size_t))

 /* Explicitly override malloc/free etc when using tcmalloc. */

#define update_zmalloc_stat_add(__n) do { \
    pthread_mutex_lock(&used_memory_mutex); \
    used_memory += (__n); \
    pthread_mutex_unlock(&used_memory_mutex); \
} while(0)

#define update_zmalloc_stat_sub(__n) do { \
    pthread_mutex_lock(&used_memory_mutex); \
    used_memory -= (__n); \
    pthread_mutex_unlock(&used_memory_mutex); \
} while(0)


#define update_zmalloc_stat_alloc(__n) do { \
    size_t _n = (__n); \
    if (_n&(sizeof(long)-1)) _n += sizeof(long)-(_n&(sizeof(long)-1)); \
    if (zmalloc_thread_safe) { \
        update_zmalloc_stat_add(_n); \
    } else { \
        used_memory -= _n; \
    } \
} while(0)

#define update_zmalloc_stat_free(__n) do { \
    size_t _n = (__n); \
























