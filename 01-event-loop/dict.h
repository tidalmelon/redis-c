/* Hash Tables Implementation.
 *
 * 这个文件实现了一个内存哈希表，
 * 它支持插入、删除、替换、查找和获取随机元素等操作。
 *
 * 哈希表会自动在表的大小的二次方之间进行调整。
 *
 * 键的冲突通过链表来解决。
 */

#include <stdint.h>

#ifndef _DICT_H
#define _DICT_H

/* 字典的操作状态 */

// 操作成功
#define DICT_OK 0
// 操作失败（或出错）
#define DICT_ERR 1

/* Unused arguments generate annoying warnings... */
// 如果字典的私有数据不使用时
// 用这个宏来避免编译器错误
#define DICT_NOTUSED(V) ((void) V)

//
// dictEntry 哈希表节点
//
typedef struct dictEntry {
    // 键
    void *key;

    // 值
    union {
        void *val;
        uint64_t u64;
        int64_t s64;
    } v;

    // 指向下个哈希表节点
    struct dictEntry *next;

} dictEntry;


//
// dictType 用于操作字典类型函数
//
typedef struct dictType {
    
    // 计算哈希值的函数
    unsigned int(*hashFunction)(const void *key);

    // 复制键的函数
    void *(*keyDup)(void *privdata, const void *key);

    // 复制值的函数
    void *(*valDup)(void *privdata, const void *obj);

    // 对比键的函数
    int(*keyCompare)(void *privdata, const void *key1, const void *key2);

    // 销毁键的函数
    void(*keyDestructor)(void *privdata, void *key);

    // 销毁值的函数
    void(*valDestructor)(void *privdata, void *obj);

} dictType;



/* This is our hash table structure. Every dictionary has two of this as we
* implement incremental rehashing, for the old to the new table. */

//
// dictht 哈希表
//每个字典都使用两个哈希表，从而实现渐进式 rehash
// 这是字典的头部
typedef struct dictht { 
    // 哈希表数组，每个元素都是一条链表
    dictEntry **table;

    // 哈希表大小
    unsigned long size;

    // 哈希表大小掩码， 用于计算索引值
    // 总数等于 size - 1
    unsigned long sizemask;

    // 该哈希表已有节点的数量
    unsigned long used;

} dictht;

















#endif
