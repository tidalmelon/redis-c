/* Hash Tables Implementation. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <sys/time.h>
#include <ctype.h>
#include <assert.h>
#include "dict.h"
//#include "zmalloc.h"

/*
 * 通过 dictEnableResize() 和 dictDisableResize() 两个函数.
 * 程序可以手动地允许或阻止哈希表进行 rehash
 * 就在 Redis 使用子进程进行保存操作时，可以有效地利用 copy-on-write 机制.
 *
 * 需要注意的是，并非所有 rehash 都会被 dictDisableResize 阻止：
 * 如果已使用节点的数量和字典大小之间的比率。
 * 大于字典强制 rehash 的比率 dict_force_resize_ratio,
 * 那么 rehash 仍然会（强制）进行。
 */


 // 指示字典是否启用 rehash 的标识
 static int dict_can_resize = 1;
 // 强制 rehash 的比率
 static unsigned int dict_force_resize_ratio = 5;

/* -------------------------- private prototypes ---------------------------- */

static int _dictExpandIfNeeded(dict *ht);
static unsigned long _dictNextPower(unsigned long size);
static int _dictKeyIndex(dict *ht, const void *key);
static int _dictInit(dict *ht, dictType *type, void *privDataPtr);

/* -------------------------- private prototypes ---------------------------- */


/* Thomas Wang's 32 bit Mix Function */
unsigned int dictIntHashFunction(unsigned int key) {
    key += ~(key << 15);
    key ^= (key >> 10);
    key += (key << 3);
    key ^= (key >> 6);
    key += ~(key << 11);
    key ^= (key >> 16);
    return key;
}

/* Identity hash function for integer keys */
unsigned int dictIdentityHashFunction(unsigned int key) {
    return key;
}

static uint32_t dict_hash_function_seed = 5381;

/*
 * 设定种子值
 */
void dictSetHashFunctionSeed(uint32_t seed) {
     dict_hash_function_seed = seed;
}

/*
 * 获取种子值
 */
uint32_t dictGetHashFunctionSeed(void) {
     return dict_hash_function_seed;
}
