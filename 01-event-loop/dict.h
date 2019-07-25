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

//
// dict字典
//
typedef struct dict {
    // 类型特定函数
    dictType *type; // type里面主要记录了一系列的函数, 可以说是规定了一系列的接口

    // 私有数据
    void *privdata; // privdata 保存了需要传递给那些类型特定函数的可选参数

    // 哈希表 
    dictht ht[2]; // 有两张hash表

    // rehash 索引
    // 并没有rehash时，值为 -1
    int rehashidx; /* rehashing not in progress if rehashidx == -1 */

    int iterators; // 目前正在运行的安全迭代器的数量

} dict;

//
// dictIterator 字典迭代器
//
// 如果 safe 属性的值为 1, 那么在迭代进行的过程中，
// 程序仍然可以执行 dictAdd, dictFind 和其他函数， 对字典进行修改。
//
// 如果 safe 不为 1， 那么程序只会调用 dictNext 对字典进行迭代
// 而不对字典进行修改。
//
typedef struct dictIterator {

    // 被迭代的字典
    dict *d;

    // table ：正在被迭代的哈希表号码，值可以是0 或 1.
    // index：迭代器当前所指向的哈希表索引位置。table[0]的索引位置？没索引啊
    // safe：标识这个迭代器是否安全
    int table, index, safe;

    // entry ：当前迭代到的节点的指针
    // nextEntry : 当前迭代节点的下一个节点
    //             因为在安全迭代器运作时，entry 所指向的节点可能会被修改，
    //             所以需要一个额外的指针来保存下一节点的位置，
    //             从而防止指针丢失
    dictEntry *entry, *nextEntry;

    long long fingerprint; // unsafe iterator fingerprint for misuse detection
} dictIterator;

typedef void (dictScanFunction)(void *privdata, const dictEntry *de);

/* This is the initial size of every hash table */
/* 哈希表的初始大小 */
#define DICT_HT_INITIAL_SIZE    4

/* ------------------------------- Macros ------------------------------------*/
// 释放给定字典节点的值 释放的是 union v.val
#define dictFreeVal(d, entry) \
    if ((d)->type->valDestructor) \
        (d)->type->valDestructor((d)->privdata, (entry)->v.val)

// 设置给定字典节点的值
#define dictSetVal(d, entry, _val_) do { \
    if ((d)->type->valDup) \
        entry->v.val = (d)->type->valDup((d)->privdata, _val_); \
    else \
        entry->v.val = (_val_); \
} while(0)

// 将一个有符号整数设为节点的值
#define dictSetSignedIntegerVal(entry, _val_) \
    do { entry->v.s64 = _val_; } while(0)

// 将一个无符号整数设为节点的值
#define dictSetUnsignedIntegerVal(entry, _val_) \
    do { entry->v.u64 = _val_; } while(0)

// 释放给定字典节点的键
#define dictFreeKey(d, entry) \
    if ((d)->type->keyDestructor) \
        (d)->type->keyDestructor((d)->privdata, (entry)->key)

// 设置给定字典节点的键
#define dictSetKey(d, entry, _key_) do { \
    if ((d)->type->keyDup) \
        entry->key = (d)->type->keyDup((d)->privdata, _key_); \
    else \
        entry->key = (_key_); \
} while(0)

// 比对两个键
#define dictCompareKeys(d, key1, key2) \
    (((d)->type->keyCompare)) ? \
        (d)->type->keyCompare((d)->privdata, key1, key2) : \
        (key1) == (key2)

// 计算给定键的哈希值
#define dictHashKey(d, key) (d)->type->hashFunction(key)
// 返回获取给定节点的键
#define dictGetKey(he) ((he)->key)
// 返回获取给定节点的值
#define dictGetVal(he) ((he)->v.val)






































#endif
