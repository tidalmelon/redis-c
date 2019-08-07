#ifndef _SDS_H
#define _SDS_H

/* 最大预分配长度 */
#define SDS_MAX_PREALLOC (1024*1024)

#include <sys/types.h>
#include <stdarg.h>
#include "zmalloc.h"

/* 类型别名, 用于指向 sdshdr 的 buf 属性 */
typedef char *sds;

//
// sdshdr 保存字符串对象的结构
//
struct sdshdr {

    // buf 中已占用空间的长度
    int len;

    // buf 中剩余可用空间的长度
    int free;

    // 数据空间
    char buf[];
};

/*
 * 返回 sds 实际保存的字符串的长度
 * 
 * T = o(1)
 */
 static inline size_t sdslen(const sds s) {
     struct sdshdr *sh = (void*)(s - (sizeof(struct sdshdr)));
     return sh->len;
 }













#endif
