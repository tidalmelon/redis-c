#ifndef _AE_H
#define _AE_H

#include <time.h>

/* 事件执行状态 */
// 成功
#define AE_OK 0
// 出错
#define AE_ERR -1

/* 文件事件状态 */
// 未设置
#define AE_NONE 0
// 可读
#define AE_READABLE 1
// 可写
#define AE_WRITABLE 2

/* 时间处理器的执行 flags */
// 文件事件
#define AE_FILE_EVENTS 1
// 时间事件
#define AE_TIME_EVENTS 2
// 所有事件
#define AE_ALL_EVENTS (AE_FILE_EVENTS|AE_TIME_EVENTS)
// 不阻塞，也不进行等待
#define AE_DONT_WAIT 4

/* 决定时间事件是否要持续执行的 flag */

#define AE_NOMORE -1

/* Macros */
#define AE_NOTUSED(V) ((void) V)

/* 事件处理状态 */
struct aeEventLoop;



typedef struct aeFileEvent {
    
    int mask;

    aeFileProc *rfileProc;  // 读事件处理器

    aeFileProc *wfileProc;  // 写事件处理器

    void *clientData;       // 多路复用库的私有数据
} aeFileEvent; 



typedef struct aeEventLoop {
    int maxfd;    // 目前已注册的最大描述符

    int setsize;  // 目前已追踪的最大描述符

    long long timeEventNextId;    // 用于生成时间事件 id

    time_t lastTime;              // 最后一次执行时间事件的时间

    aeFiredEvent *events;         // 已注册的文件事件

    aeFiredEvent *fired;          // 已就绪的文件事件

    aeTimeEvent *timeEventHead;   // 时间事件

    int stop;    // 事件处理器的开关

    void *apidata;   // 多路复用库的私有数据， 一般用于存放aeApiState对象的指针
                     // 而aeApiState有着epoll_events结构的一个数组

    aeBeforeSleepProc *beforesleep;   // 在处理事件前要执行的函数

} aeEventLoop;


#endif
