#ifndef _REDIS_H
#define _REDIS_H

#include "fmacros.h"
#include "dict.h"
#include "adlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <inttypes.h>
#include <pthread.h>
#include <syslog.h>
#include <netinet/in.h>
#include <signal.h>
#include "anet.h"
#include "ae.h"
#include "sds.h"
#include "zmalloc.h"

/* Error codes */
#define REDIS_OK
#define REDIS_ERR

/* Objects encoding. Some kind of objects like String and Hashes can be
 * internally represented in multiple ways. The 'encoding' field of the object 
 * is set to one of this field for this object. */
 // 对象编码
#define REDIS_ENCODING_RAW 0           /* Raw representation */

/* 对象类型 */
#define REDIS_STRING 0
#define REDIS_LIST 1
#define REDIS_SET 2
#define REDIS_ZSET 3
#define REDIS_HASH 4

/* Protocol and I/O related defines */
#define REDIS_REPLY_CHUNK_BYTES (16*1024) /* 16k output buffer */
#define REDIS_IOBUF_LEN         (1024*16) /* Generic I/O buffer size */
#define REDIS_INLINE_MAX_SIZE   (1024*64) /* Max size of inline reads */
#define REDIS_MBULK_BIG_ARG     (1024*32)
#define REDIS_MIN_RESERVED_FDS 32
#define REDIS_EVENTLOOP_FDSET_INCR (REDIS_MIN_RESERVED_FDS+96)
#define REDIS_MAX_CLIENTS 10000  /* 最大所支持的用户数目 */

/* Static server configuration */
#define REDIS_DEFAULT_HZ       10
#define REDIS_SERVERPORT       6379  /* TCP port */
#define REDIS_TCP_BACKLOG      511       /* TCP listen backlog */
#define REDIS_BINDADDR_MAX     16
#define REDIS_IP_STR_LEN INET6_ADDRSTRLEN
#define REDIS_DEFAULT_DBNUM    16
#define REDIS_DEFAULT_TCP_KEEPALIVE 0

/* Client request types */
#define REDIS_REQ_INLINE    1
#define REDIS_REQ_MULTIBULK 2 /* 多条查询 */

/* 对象编码 */
#define REDIS_ENCODING_RAW 0     /* Raw representation */
#define REDIS_ENCODING_INT 1     /* Encoded as integer */
#define REDIS_ENCODING_EMBSTR 8  /* Embedded sds string encodig */

/* 命令标志 */
#define REDIS_CMD_WRITE 1               /* 'w' flag */
#define REDIS_CMD_READONLY 2            /* 'r' flag */
#define REDIS_CMD_DENYOOM 4             /* 'm' flag */
#define REDIS_CMD_NOT_USED_1 8          /* 'no longer used flag */
#define REDIS_CMD_ADMIN 16              /* 'a' flag */
#define REDIS_CMD_PUBSUB 32             /* 'p' flag */
#define REDIS_CMD_NOSCRIPT 64           /* 's' flag */
#define REDIS_CMD_RANDOM  128           /* 'R' flag */
#define REDIS_CMD_SORT_FOR_SCRIPT 256   /* "S" flag */
#define REDIS_CMD_LOADING 512           /* 'l' flag */
#define REDIS_CMD_STALE 1024            /* 't' flag */
#define REDIS_CMD_SKIP_MONITOR 2048     /* 'M' flag */
#define REDIS_CMD_ASKING 4096           /* 'k' flag */

/* Command call flags, see call() function */
#define REDIS_CALL_NONE 0
#define REDIS_CALL_SLOWLOG 1
#define REDIS_CALL_STATS 2
#define REDIS_CALL_PROPAGATE 4
#define REDIS_CALL_FULL (REDIS_CALL_SLOWLOG | REDIS_CALL_STATS | REDIS_CALL_PROPAGATE)

#define REDIS_LRU_BITS 24

//
// redisObject
//
typedef struct redisObject {

    unsigned type : 4;   //类型

    unsigned encoding : 4;  //编码

    unsigned lru : REDIS_LRU_BITS;  //对象最后一次被访问的时间

    int refcount;  //引用计数

    void *ptr;     // 指向实际值的指针
} robj;


typedef struct redisDb {

    dict *dict;          // 数据库键空间，保存着数据库中的所有键值对

    dict *expires;       // 键的过期时间，字典的键为键，字典的值为过期事件(时间吧，估计写错了) UNIX 时间戳

    dict *blocking_keys; // 正处于阻塞状态的键

    dict *ready_keys;    // 可以解除阻塞的键

    int id;              // 数据库号码
} redisDb;

/*
 * 因为 I/O 复用的缘故， 需要为每个客户端维持一个状态
 *
 * 多个客户端状态被服务器用链表连接起来
 */
 typedef struct redisClient {
     int fd;   //套接字描述符

     redisDb *db;   // 当前正在使用的数据库

     int dictid;    // 当前正在使用的数据库的 id (号码)

     robj *name;    // 客户端的名字

     sds querybuf;  // 查询缓冲区

     size_t querybuf_peak;   // 查询缓冲区长度峰值
 }


























































#endif
