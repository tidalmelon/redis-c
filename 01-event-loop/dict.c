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
#include "zmalloc.h"

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

/* MurmurHash2, by Austin Appleby
 * Note - This code makes a few assumptions about how your machine behaves -
 * 1. We can read a 4-byte value from any address without crashing
 * 2. sizeof(int) == 4
 *
 * And it has a few limitations -
 *
 * 1. It will not work incrementally.
 * 2. It will not produce the same results on little-endian and big-endian
 *    machines.
 */
unsigned int dictGenHashFunction(const void *key, int len) {
	/* 'm' and 'r' are mixing constants generated offline.
	They're not really 'magic', they just happen to work well.  */
	uint32_t seed = dict_hash_function_seed;
	const uint32_t m = 0x5bd1e995;
	const int r = 24;

	/* Initialize the hash to a 'random' value */
	uint32_t h = seed ^ len;

	/* Mix 4 bytes at a time into the hash */
	const unsigned char *data = (const unsigned char *)key;

	while (len >= 4) {
		uint32_t k = *(uint32_t*)data;

		k *= m;
		k ^= k >> r;
		k *= m;

		h *= m;
		h ^= k;

		data += 4;
		len -= 4;
	}

	/* Handle the last few bytes of the input array  */
	switch (len) {
	case 3: h ^= data[2] << 16;
	case 2: h ^= data[1] << 8;
	case 1: h ^= data[0]; h *= m;
	};

	/* Do a few final mixes of the hash to ensure the last few
	* bytes are well-incorporated. */
	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return (unsigned int)h;
}

/* And a case insensitive hash function (based on djb hash) */
unsigned int dictGenCaseHashFunction(const unsigned char *buf, int len) {
	unsigned int hash = (unsigned int)dict_hash_function_seed;

	while (len--)
		hash = ((hash << 5) + hash) + (tolower(*buf++)); /* hash * 33 + c */
	return hash;
}

/* ----------------------------- API implementation ------------------------- */

/* 
 * 重置 (或初始化) 给定哈希表的各项属性值
 *
 * T = o(1)
 */
static void _dictReset(dictht *ht) {
     ht->table = NULL;
     ht->size = 0;
     ht->sizemask = 0;
     ht->used = 0;
 }

/*
 * 创建一个新的字典
 *
 * T = o(1)
 */
dict *dictCreate(dictType *type, 
    void *privDataPtr) {
    dict *d = zmalloc(sizeof(*d));
    //pause
                     
}



























