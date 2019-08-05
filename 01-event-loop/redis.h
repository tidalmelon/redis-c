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



























#endif
