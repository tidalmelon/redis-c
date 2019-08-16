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

     int argc;   // 参数数量

     robj **argv;  // 参数对象数组

     struct redisCommand *cmd, *lastcmd;    // 记录被客户端执行的命令

     int reqtype  // 请求的类型，是内联命令还是多条命令

     int multibulklen;  // 剩余未读取的命令内容数量

     long bulklen;      // 命令内容的长度

     list *reply;      // 回复链表

     unsigned long reply_bytes;   // 回复链表中对象的总大小

     int bufpos;    // 回复偏移量

     char buf[REDIS_REPLY_CHUNK_BYTES];
} redisClient;



struct redisServer {

    /* Generic */
    char *configfile;      // 配置文件的绝对路径

    int hz;                // serverCron()  每秒调用的次数

    redisDb *db;           // 一个数组，保存着服务器中所有的数据库

    dict *commands;        // 命令表 （受到 rename 配置选项的作用）

    dict *orig_commands;   // Command table before command renaming.

    aeEventLoop *el;   // 事件状态

    int shutdown_asap;   // 关闭服务器的标识

    int port;
    int tcp_backlog;
    int *bindaddr[REDIS_BINDADDR_MAX]; // ip 地址
    int bindaddr_count;    // 地址的数量

    int ipfd[REDIS_BINDADDR_MAX];    // tcp 描述符
    int ipfd_count;                  //已经使用了的描述符的数目

    list *clients;        // 一个链表，保存了所有的客户端状态结构
    list *clients_to_close;  // 链表，保存了所有待关闭的客户端

    redisClient *current_client;   // 服务器当前服务的客户端, 仅用于崩溃报告

    char neterr[ANET_ERR_LEN];     // 用于记录网络错误

    int tcpkeepalive;        // 是否开启 SO_KEEPALIVE 选项
    int dbnum;               //  数据库的总数目

    /* Limits */
    int maxclients;             //max number of simultaneous clients
    
};


typedef void redisCommandProc(redisClient *c);
typedef int *redisGetKeysProc(struct redisCommand *cmd, robj **argv, int argc, int *numkeys);

//
// redisCommand Redis 命令
//
struct redisCommand {
    
    char *name;

    redisCommandProc *proc;

    int arity;

    char *sflags;

    int flags;

    redisGetKeysProc *getkeys_proc;

    int firstkey;
    int lastkey;
    int keystep;

    long long microseconds, calls;
};












/* api */
int processCommand(redisClient *c);

#endif
