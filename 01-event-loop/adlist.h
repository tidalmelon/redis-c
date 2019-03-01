#ifndef _ADLIST_H
#define _ADLIST_H

typedef struct listNode {
    struct listNode *prev;
    struct listNode *next;
    void *value;
} listNode;

指针详解
http://www.runoob.com/w3cnote/c-pointer-detail.html
void指针
http://www.runoob.com/w3cnote/c-void-intro.html
范性编程
http://www.runoob.com/w3cnote/c-general-function.html
#endif
